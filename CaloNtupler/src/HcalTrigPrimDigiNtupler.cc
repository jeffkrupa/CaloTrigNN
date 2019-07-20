#include "CaloTrigNN/CaloNtupler/interface/HcalTrigPrimDigiNtupler.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"
#include "DataFormats/HcalDigi/interface/HFDataFrame.h"
#include "DataFormats/HcalDigi/interface/HcalTriggerPrimitiveDigi.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGCoder.h"
#include "CalibFormats/CaloTPG/interface/HcalTPGCompressor.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGRecord.h"
#include "CalibFormats/CaloTPG/interface/CaloTPGTranscoder.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "CondFormats/HcalObjects/interface/HcalLutMetadata.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "RecoParticleFlow/PFProducer/interface/PFAlgo.h"

#include <algorithm>

using namespace std;
using namespace edm;
using namespace boost; 

HcalTrigPrimDigiNtupler::HcalTrigPrimDigiNtupler(const edm::ParameterSet& iPS)  
  //fNtuple(iPS.getParameter<bool>("peakFilter"),iPS.getParameter<std::vector<double> >("weights"),
  //	  iPS.getParameter<int>("numberOfSamples"),iPS.getParameter<int>("numberOfPresamples"),
  //	  iPS.getParameter<int>("numberOfSamplesHF"),iPS.getParameter<int>("numberOfPresamplesHF")
  //	  ),
  //fInputLabel(iPS.getParameter<std::vector<edm::InputTag> >("inputLabel")),
  //fInputUpgradeLabel(iPS.getParameter<std::vector<edm::InputTag> >("inputUpgradeLabel")) { 
  {
  /*//check upgrade
  std::vector<bool> upgrades = {iPS.getParameter<bool>("upgradeHB"), iPS.getParameter<bool>("upgradeHE"), iPS.getParameter<bool>("upgradeHF")};
  fUpgrade = std::any_of(std::begin(upgrades), std::end(upgrades), [](bool a) { return a; });
  fLegacy  = std::any_of(std::begin(upgrades), std::end(upgrades), [](bool a) { return !a; });
  fHFEMB = false;
  if(iPS.exists("LSConfig")) {
    fLongShortCut    = iPS.getUntrackedParameter<edm::ParameterSet>("LSConfig");
    fHFEMB           = fLongShortCut.getParameter<bool>("HcalFeatureHFEMBit");
    fMinLongEnergy   = fLongShortCut.getParameter<double>("Min_Long_Energy"); //minimum long energy
    fMinShortEnergy  = fLongShortCut.getParameter<double>("Min_Short_Energy"); //minimum short energy
    fLongShortSlope  = fLongShortCut.getParameter<double>("Long_vrs_Short_Slope"); //slope of the line that cuts are based on
    fLongShortOffset = fLongShortCut.getParameter<double>("Long_Short_Offset"); //offset of line
  }

  fHB        = consumes<HBHEDigiCollection>(fInputLabel[0]);
  fHF        = consumes<HFDigiCollection>  (fInputLabel[1]);
  fHBHEUp    = consumes<QIE11DigiCollection>(fInputUpgradeLabel[0]);
  fHFUp      = consumes<QIE10DigiCollection>(fInputUpgradeLabel[1]);
  */fSHitToken = consumes<edm::PCaloHitContainer>(edm::InputTag("g4SimHits","HcalHits"));
  //produces<HcalTrigPrimDigiCollection>();
  //fNtuple.setPeakFinderAlgorithm(iPS.getParameter<int>("PeakFinderAlgorithm"));
  fFile        = new TFile("Output_old.root","RECREATE");
  //fHcalArr     = new TClonesArray("baconhep::THcalDep",5000);
  fRHParArr    = new TClonesArray("baconhep::TRHPart",5000);
  fEvtArr      = new TClonesArray("baconhep::TEventInfo",5000);
  fGenParArr   = new TClonesArray("baconhep::TGenParticle",5000);
  fPFParArr    = new TClonesArray("baconhep::TPFPart",5000);
 
  fFillerEventInfo = new FillerEventInfo(iPS, consumesCollector()); 
  fFillerRH = new FillerRH(iPS,consumesCollector());
  fFillerGenInfo = new FillerGenInfo(iPS,consumesCollector()); 
  fFillerPF    = new FillerPF(iPS,consumesCollector()); 
  fTree        = new TTree("Events","Events");
  //fTree->Branch("HcalPulse",  &fHcalArr); 
  fTree->Branch("EventInfo", &fEvtArr); 
  fTree->Branch("HcalRecHit", &fRHParArr); 
  fTree->Branch("PFDepth"   , &fPFParArr);
  fTree->Branch("GenParticle", &fGenParArr); 
}
void HcalTrigPrimDigiNtupler::endJob() {
  fFile->cd();
  fTree->Write();
  fFile->Write();
  fFile->Close();
}
void HcalTrigPrimDigiNtupler::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::ESHandle<HcalDDDRecConstants> pHRNDC;
  iSetup.get<HcalRecNumberingRecord>().get( pHRNDC );
  fRecNumber= &*pHRNDC;
  
  // Step A: get the conditions, for the decoding
  //edm::ESHandle<HcalTPGCoder> inputCoder;
  //iSetup.get<HcalTPGRecord>().get(inputCoder);
 
  edm::ESHandle<HcalTrigTowerGeometry> pG;
  iSetup.get<CaloGeometryRecord>().get(pG);
  //Get the Gen Info
  edm::Handle<edm::PCaloHitContainer> hSimHits;      // create handle
  iEvent.getByToken(fSHitToken, hSimHits);   // SimHits

  edm::ESHandle<CaloGeometry> geometry;
  iEvent.getByToken(fSHitToken, hSimHits);   // SimHits
  iSetup.get<HcalRecNumberingRecord>().get( pHRNDC );
  iSetup.get<CaloGeometryRecord>().get( geometry );
  
  edm::PCaloHitContainer lSimHits  = *hSimHits;
  fGeometry = &*geometry;
  fRecNumber= &*pHRNDC;

  /*
  // Step B: Create empty output
  std::unique_ptr<HcalTrigPrimDigiCollection> result(new HcalTrigPrimDigiCollection());
  edm::Handle<HBHEDigiCollection>  hbheDigis;
  edm::Handle<HFDigiCollection>    hfDigis;
  edm::Handle<QIE11DigiCollection> hbheUpDigis;
  edm::Handle<QIE10DigiCollection> hfUpDigis;
  iEvent.getByToken(fHB,    hbheDigis);
  iEvent.getByToken(fHF,    hfDigis);
  iEvent.getByToken(fHBHEUp,hbheUpDigis);
  iEvent.getByToken(fHFUp, hfUpDigis);
    
  edm::ESHandle < HcalDbService > pSetup;
  iSetup.get<HcalDbRecord> ().get(pSetup);

  HcalFeatureBit* hfembit = nullptr;
  if(fHFEMB) {
    hfembit = new HcalFeatureHFEMBit(fMinShortEnergy, fMinLongEnergy, fLongShortSlope, fLongShortOffset, *pSetup); //inputs values that cut will be based on
  }*/
  // Step C: Invoke the algorithm, passing in inputs and getting back outputs.
  //fHcalArr->Clear();
  fEvtArr->Clear();
  fRHParArr->Clear();
  fGenParArr->Clear();
  fPFParArr->Clear();
  //fNtuple.fill(fHcalArr,inputCoder.product(), pSetup.product(), &(*pG),fGeometry,hfembit,lSimHits,fRecNumber,*hbheDigis,*hbheUpDigis,*hfUpDigis,*hfUpDigis);
  fFillerRH->fill(fRHParArr,iEvent,iSetup,lSimHits,fRecNumber);
  fFillerEventInfo->fill(fEvtArr,iEvent); 
  fFillerGenInfo->fill(fGenParArr,iEvent);
  fFillerPF->fill(fPFParArr,iEvent,iSetup,lSimHits,fRecNumber);
  fTree->Fill();
  // Step C.1: Run FE Format Error / ZS for real data.
  //iEvent.put(std::move(result));
}
