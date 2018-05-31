#include "CaloTrigNN/CaloNtupler/interface/FillerRH.hh"
#include "CaloTrigNN/DataFormats/interface/TRHPart.hh"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"
#include <TClonesArray.h>
#include <TMath.h>
#include <TLorentzVector.h>

using namespace baconhep;

//--------------------------------------------------------------------------------------------------
FillerRH::FillerRH(const edm::ParameterSet &iConfig,edm::ConsumesCollector && iC):
  fRHName      (iConfig.getUntrackedParameter<edm::InputTag>("edmRecHitName")),
  fChanInfoName(iConfig.getUntrackedParameter<edm::InputTag>("edmChanInfoName"))
{
  fTokRH       = iC.consumes<HBHERecHitCollection>(fRHName);
  fTokChanInfo = iC.consumes<HBHEChannelInfoCollection>(fChanInfoName);
}

//--------------------------------------------------------------------------------------------------
FillerRH::~FillerRH(){}

//--------------------------------------------------------------------------------------------------
void FillerRH::fill(TClonesArray *array,const edm::Event &iEvent,const edm::EventSetup &iSetup,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber){
  assert(array);
  edm::ESHandle<CaloGeometry> geoHandle;
  iSetup.get<CaloGeometryRecord>().get(geoHandle);
  const CaloSubdetectorGeometry *gTmp0 = geoHandle->getSubdetectorGeometry(DetId::Hcal,1);
  fHCAL = dynamic_cast< const CaloSubdetectorGeometry* > (gTmp0);

  fResponse = new CaloHitResponse(NULL, (CaloShapes*)NULL);
  fResponse->setGeometry(&*geoHandle);

  const HBHERecHitCollection *recHitHCAL   = 0;
  edm::Handle<HBHERecHitCollection> hRecHitHCAL;
  iEvent.getByToken(fTokRH,hRecHitHCAL);
  recHitHCAL = hRecHitHCAL.product();

  const HBHEChannelInfoCollection *channelInfo   = 0;
  edm::Handle<HBHEChannelInfoCollection> hChannelInfo;
  iEvent.getByToken(fTokChanInfo, hChannelInfo);
  channelInfo = hChannelInfo.product();
  
  TClonesArray &rArray = *array;
  int pId = 0; 
  for(HBHERecHitCollection::const_iterator itRH = recHitHCAL->begin(); itRH != recHitHCAL->end(); itRH++) {
    pId++;
    // construct object and place in array
    assert(rArray.GetEntries() < rArray.GetSize());
    const int index = rArray.GetEntries();
    new(rArray[index]) baconhep::TRHPart();
    baconhep::TRHPart *pRH = (baconhep::TRHPart*) rArray[index];
    GlobalPoint pCellPos  = fHCAL->getGeometry(itRH->detid())->getPosition();
    TLorentzVector pVec; 
    double pR = sqrt(pCellPos.x()*pCellPos.x() + pCellPos.y()*pCellPos.y() + pCellPos.z()*pCellPos.z());
    pVec.SetPxPyPzE( pCellPos.x()/pR * itRH->energy(),       pCellPos.y()/pR * itRH->energy(),       pCellPos.z()/pR * itRH->energy(),itRH->energy()); 
    // Kinematics
    //==============================    
    pRH->pt     = pVec.Pt();
    pRH->eta    = pVec.Eta();
    pRH->phi    = pVec.Phi();
    pRH->rho    = pCellPos.perp();
    pRH->ieta   = itRH->id().ieta();
    pRH->iphi   = itRH->id().iphi();
    pRH->depth  = itRH->id().depth();
    //pRH->timefalling   = itRH->timeFalling();
    pRH->energy = itRH->energy();
    pRH->eraw   = itRH->eraw();
    pRH->em3    = itRH->eaux();
    pRH->time   = itRH->time();
    pRH->x      = pCellPos.x();
    pRH->y      = pCellPos.y();
    pRH->z      = pCellPos.z();
    HcalDetId pId = itRH->id();
    fillGen(pId,itRH->id().ieta(),itRH->id().iphi(),pRH,iSimHits,iRecNumber);
    fillTS(pId,pRH,channelInfo);
  } 
}
void FillerRH::fillGen(HcalDetId &iDetId,int iIEta,int iIPhi,baconhep::TRHPart *iHcal,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber) { 
  double samplingFactor = 1;
  if(iDetId.subdet() == HcalBarrel) {
    samplingFactor = fSimParameterMap.hbParameters().samplingFactor(iDetId);
  } else if (iDetId.subdet() == HcalEndcap) {
    samplingFactor = fSimParameterMap.heParameters().samplingFactor(iDetId);
  }
  HcalDetId simIdneed;
  int    maxEnk=0;
  double sHitEn=0;
  double maxEn=0; 
  int    maxDepth=-1;
  double sHitTime = -99.;
  for (int j=0; j < (int) iSimHits.size(); j++) {
    HcalDetId simId = HcalHitRelabeller::relabel((iSimHits)[j].id(), iRecNumber);
    if (simId == iDetId ){
      sHitEn += samplingFactor*((iSimHits)[j].energy());
      if((iSimHits)[j].energy() > maxEn){
	maxEnk=j;
	simIdneed = simId;
	maxEn = (iSimHits)[j].energy();
	maxDepth=simId.depth();
      }
    }
    if(maxEn>0) {
      double tof = fResponse->timeOfFlight(simIdneed);
      sHitTime = (iSimHits)[maxEnk].time() - tof;
    }
  }
  iHcal->genE      = sHitEn;
  iHcal->genEDepth = maxDepth;
  iHcal->genETime  = sHitTime;
}
void FillerRH::fillTS(HcalDetId &iDetId,baconhep::TRHPart *iHcal,const HBHEChannelInfoCollection *iChannelInfo) { 
  iHcal->ts.clear();
  iHcal->raw.clear();
  iHcal->ped.clear();
  iHcal->inNoiseADC.clear();
  iHcal->inPedestal.clear();
  iHcal->inNoisePhoto.clear();
  iHcal->inputTDC.clear();
  for (HBHEChannelInfoCollection::const_iterator iter = iChannelInfo->begin(); iter != iChannelInfo->end(); iter++) {
    const HBHEChannelInfo& pChannel(*iter);
    const HcalDetId        pDetId = pChannel.id();
    if(pDetId != iDetId) continue; 
    iHcal->samples  = pChannel.nSamples();
    iHcal->soi      = pChannel.soi();
    iHcal->inPedAvg = 0.25*( pChannel.tsPedestalWidth(0)*pChannel.tsPedestalWidth(0)+
			     pChannel.tsPedestalWidth(1)*pChannel.tsPedestalWidth(1)+
			     pChannel.tsPedestalWidth(2)*pChannel.tsPedestalWidth(2)+
			     pChannel.tsPedestalWidth(3)*pChannel.tsPedestalWidth(3) );
    iHcal->gain     = pChannel.tsGain(0);
    for (unsigned int iTS=0; iTS<pChannel.nSamples(); ++iTS) {
      double raw = pChannel.tsRawCharge(iTS);
      double ped = pChannel.tsPedestal(iTS);
      iHcal->ts        .push_back(iTS);
      iHcal->raw       .push_back(raw);
      iHcal->ped       .push_back(ped);
      iHcal->inNoiseADC.push_back((1./sqrt(12))*pChannel.tsDFcPerADC(iTS));
      iHcal->inPedestal.push_back(pChannel.tsPedestalWidth(iTS));    
      if ( (raw-ped)>pChannel.tsPedestalWidth(iTS)) {iHcal->inNoisePhoto.push_back(sqrt((raw-ped)*pChannel.fcByPE()));} 
      else {iHcal->inNoisePhoto.push_back(0); }
      if (pChannel.hasTimeInfo()) {iHcal->inputTDC.push_back(pChannel.tsRiseTime(iTS));}
      else {iHcal->inputTDC.push_back(-1); }
    }
  }

}

