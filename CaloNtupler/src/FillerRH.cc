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
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include <TClonesArray.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <stdlib.h>
using namespace baconhep;

//--------------------------------------------------------------------------------------------------
FillerRH::FillerRH(const edm::ParameterSet &iConfig,edm::ConsumesCollector && iC):
  fRHName      (iConfig.getUntrackedParameter<edm::InputTag>("edmRecHitName")),
  fChanInfoName(iConfig.getUntrackedParameter<edm::InputTag>("edmChanInfoName")),
  fGenParName    (iConfig.getUntrackedParameter<std::string>("edmGenParticlesName","genParticles")),
  fPFClustHName  (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterHCAL"))
{
  fTokRH       = iC.consumes<HBHERecHitCollection>(fRHName);
  fTokChanInfo = iC.consumes<HBHEChannelInfoCollection>(fChanInfoName);
  fTokGenPar   = iC.mayConsume<reco::GenParticleCollection>( fGenParName    );
  fTokPFClustHName    = iC.consumes<reco::PFClusterCollection>    (fPFClustHName);
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

  //edm::Handle<reco::GenParticleCollection> hGenParProduct;
  //iEvent.getByToken(fTokGenPar,hGenParProduct);
  //assert(hGenParProduct.isValid());
  //const reco::GenParticleCollection genParticles = *(hGenParProduct.product());

  const reco::GenParticleCollection * GenCol = 0;
  edm::Handle<reco::GenParticleCollection> hGenParProduct;
  iEvent.getByToken(fTokGenPar,hGenParProduct);
  assert(hGenParProduct.isValid());
  //GenCol = hGenParProduct.product();

  const reco::PFClusterCollection *pfCHCAL   = 0;
  edm::Handle<reco::PFClusterCollection> hPFCHCAL;
  iEvent.getByToken(fTokPFClustHName,hPFCHCAL);
  assert(hPFCHCAL.isValid()); 
  pfCHCAL = hPFCHCAL.product();
 
  TClonesArray &rArray = *array;
  int pId = 0; 

  std::map<HcalDetId, std::pair<int,float>> RHClusterMap = getRHCluster(recHitHCAL, pfCHCAL, fHCAL);

  for(HBHERecHitCollection::const_iterator itRH = recHitHCAL->begin(); itRH != recHitHCAL->end(); itRH++) {
    pId++;
    // construct object and place in array
    HcalDetId pId = itRH->id();
    double genE = getGen(pId,itRH->id().ieta(),itRH->id().iphi(),iSimHits,iRecNumber);
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
    pRH->timeFalling   = itRH->timeFalling();
    pRH->energy = itRH->energy();
    pRH->eta    = pVec.Eta();
    pRH->phi    = pVec.Phi();
    pRH->time   = itRH->time();
    pRH->x      = pCellPos.x();
    pRH->y      = pCellPos.y();
    pRH->z      = pCellPos.z();
    if(genE>0.)  pRH->hit    = 1;
    else         pRH->hit    = 0;
    //match RH to closest cluster
    pRH->RHClusterMatch =   RHClusterMap[pId].first;
    pRH->RHClusterDist  =   RHClusterMap[pId].second;
    //match RH within deltaR to cluster
    //pRH->nRHClusterMatches = RHClusterMap[pId].size();
    //pRH->RHClusterMatch    = (RHClusterMap[pId].size() ? *std::min_element(RHClusterMap[pId].begin(), RHClusterMap[pId].end()) : -1);
    //pRH->particleMatch = getParticleMatch(pVec.Eta(), pVec.Phi(), GenCol);
    //fillGen(pId,itRH->id().ieta(),itRH->id().iphi(),pRH,iSimHits,iRecNumber);
    //fillTS(pId,pRH,channelInfo);
  } 


}
/*std::map<HcalDetId, std::vector<int>> FillerRH::getRHCluster(const HBHERecHitCollection *recHitHCAL, const reco::PFClusterCollection *pfCHCAL, const CaloSubdetectorGeometry *fHCAL){
    //std::cout << "getParticleMatch"<< RHeta << "/" << RHphi << std::endl;

    std::map<HcalDetId, std::vector<int>> RHClusterMap;
    
    for(HBHERecHitCollection::const_iterator itRH = recHitHCAL->begin(); itRH != recHitHCAL->end(); itRH++) {
        //std::vector<int> pMatch;
        GlobalPoint pCellPos  = fHCAL->getGeometry(itRH->detid())->getPosition();
        TLorentzVector pVec; 
        double pR = sqrt(pCellPos.x()*pCellPos.x() + pCellPos.y()*pCellPos.y() + pCellPos.z()*pCellPos.z());
        pVec.SetPxPyPzE( pCellPos.x()/pR * itRH->energy(),       pCellPos.y()/pR * itRH->energy(),       pCellPos.z()/pR * itRH->energy(),itRH->energy());

        for(reco::PFClusterCollection::const_iterator itC = pfCHCAL->begin(); itC!=pfCHCAL->end(); itC++) {

            if(std::sqrt(reco::deltaR2(itC->eta(), itC->phi(), pVec.Eta(), pVec.Phi())) < 0.3) 
              {HcalDetId pId = itRH->id(); RHClusterMap[pId].push_back(std::distance(pfCHCAL->begin(), itC));}
        }   
    }
    return RHClusterMap;
}
*/
std::map<HcalDetId, std::pair<int,float>> FillerRH::getRHCluster(const HBHERecHitCollection *recHitHCAL, const reco::PFClusterCollection *pfCHCAL, const CaloSubdetectorGeometry *fHCAL){

    std::map<HcalDetId, std::pair<int,float>> RHClusterMap;
    for(HBHERecHitCollection::const_iterator itRH = recHitHCAL->begin(); itRH != recHitHCAL->end(); itRH++) {
        GlobalPoint pCellPos  = fHCAL->getGeometry(itRH->detid())->getPosition();
        TLorentzVector pVec; 
        double pR = sqrt(pCellPos.x()*pCellPos.x() + pCellPos.y()*pCellPos.y() + pCellPos.z()*pCellPos.z());
        pVec.SetPxPyPzE( pCellPos.x()/pR * itRH->energy(),       pCellPos.y()/pR * itRH->energy(),       pCellPos.z()/pR * itRH->energy(),itRH->energy());

        float minClusterDistance = 999.;
        int clusterMatch = -1;
        for(reco::PFClusterCollection::const_iterator itC = pfCHCAL->begin(); itC!=pfCHCAL->end(); itC++) {
              float d = std::sqrt(reco::deltaR2(itC->eta(), itC->phi(), pVec.Eta(), pVec.Phi())) ;
              if (d < minClusterDistance) {minClusterDistance = d; clusterMatch = std::distance(pfCHCAL->begin(), itC);}
        }  
        HcalDetId pId = itRH->id(); 
        RHClusterMap[pId].first = clusterMatch;
        RHClusterMap[pId].second = minClusterDistance;
    }
    return RHClusterMap;
}

int FillerRH::getParticleMatch(float RHeta,float RHphi, const reco::GenParticleCollection *genParticles){
    int pMatch(0);
    for (reco::GenParticleCollection::const_iterator itGenP = genParticles->begin(); itGenP!=genParticles->end(); ++itGenP) {
        if(itGenP->status() == 1 && itGenP->isLastCopy()) 
            if(std::sqrt(reco::deltaR2(itGenP->eta(), itGenP->phi(), RHeta, RHphi)) < 0.1) pMatch = std::distance(genParticles->begin(), itGenP);
    } 
    return pMatch;
}
double FillerRH::getGen(HcalDetId &iDetId,int iIEta,int iIPhi,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber) { 
  double samplingFactor = 1;
  if(iDetId.subdet() == HcalBarrel) {
    samplingFactor = fSimParameterMap.hbParameters().samplingFactor(iDetId);
  } else if (iDetId.subdet() == HcalEndcap) {
    samplingFactor = fSimParameterMap.heParameters().samplingFactor(iDetId);
  }
  HcalDetId simIdneed;
  double sHitEn=0;
  for (int j=0; j < (int) iSimHits.size(); j++) {
    HcalDetId simId = HcalHitRelabeller::relabel((iSimHits)[j].id(), iRecNumber);
    if (simId == iDetId ){
      sHitEn += samplingFactor*((iSimHits)[j].energy());
    }
  }
  return sHitEn;
}
