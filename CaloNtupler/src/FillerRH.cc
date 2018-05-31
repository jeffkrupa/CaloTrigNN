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
  fRHName      (iConfig.getUntrackedParameter<edm::InputTag>("edmRecHitName"))
{
  fTokRH = iC.consumes<HBHERecHitCollection>(fRHName);
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
