#ifndef HcalTrigPrimDigiNtupler_h
#define HcalTrigPrimDigiNtupler_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTriggerPrimitiveAlgo.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "CaloTrigNN/CaloNtupler/interface/FillerRH.hh"
#include "CaloTrigNN/DataFormats/interface/TRHPart.hh"
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "RecoParticleFlow/PFProducer/interface/PFAlgo.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockFwd.h"
//#include "RecoParticleFlow/PFProducer/plugins/PFProducer.h"


class HcalTrigPrimDigiNtupler : public edm::stream::EDProducer<> {
public:

  explicit HcalTrigPrimDigiNtupler(const edm::ParameterSet& ps);
  ~HcalTrigPrimDigiNtupler() override { endJob(); delete fRHParArr;}// delete fFillerRH; delete fEvtArr; delete fFillerEventInfo; delete fFillerGenInfo; delete fGenParArr;}
  //~HcalTrigPrimDigiNtupler() override { endJob(); delete fPFParArr; delete fFillerPF; delete fRHParArr; delete fFillerRH;}
  /**Produces the EDM products,*/
  void produce(edm::Event& e, const edm::EventSetup& c) override;
  void endJob();
  
private:
  //HcalNtuple fNtuple;
  FillerRH  *fFillerRH;
  //FillerEventInfo *fFillerEventInfo;
  //FillerGenInfo  *fFillerGenInfo;
  //FillerPF  *fFillerPF;

  /// input tags for HCAL digis
  std::vector<edm::InputTag> fInputLabel;
  std::vector<edm::InputTag> fInputUpgradeLabel;
  // this seems a strange way of doing things
  //edm::EDGetTokenT<HBHEDigiCollection> fHB;
  //edm::EDGetTokenT<HFDigiCollection> fHF;
  //edm::EDGetTokenT<QIE11DigiCollection> fHBHEUp;
  //edm::EDGetTokenT<QIE10DigiCollection> fHFUp;
  edm::EDGetTokenT<edm::PCaloHitContainer> fSHitToken;
  
  double fMinLongEnergy, fMinShortEnergy, fLongShortSlope, fLongShortOffset;
  bool fUpgrade;
  bool fLegacy;
  bool fHFEMB;
  edm::ParameterSet fLongShortCut;
  const HcalDDDRecConstants *fRecNumber;
  const CaloGeometry* fGeometry;
  TFile *fFile;
  TTree *fTree;
  //TClonesArray *fHcalArr;
  TClonesArray *fRHParArr;
  //TClonesArray *fEvtArr;
  //TClonesArray *fGenParArr;
  //TClonesArray *fPFParArr;
};

#endif

