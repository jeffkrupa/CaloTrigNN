#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockFwd.h"
#include "CaloTrigNN/CaloNtupler/interface/FillerPF.hh"
#include "CaloTrigNN/DataFormats/interface/TPFPart.hh"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"
#include <TClonesArray.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <numeric>

using namespace baconhep;

//--------------------------------------------------------------------------------------------------
FillerPF::FillerPF(const edm::ParameterSet &iConfig,edm::ConsumesCollector && iC):
  fPFName      (iConfig.getUntrackedParameter<std::string>("edmName","particleFlow")),
  fPFClustHName (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterHCAL")),
  fPFClustEName (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterECAL")),
  fPFClustOName (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterHO")),
  fPFClustFName (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterHF")),
  fPFClustSName (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterPS"))
{
  fTokPFName         = iC.consumes<reco::PFCandidateCollection>(fPFName);
  fTokPFClustHName    = iC.consumes<reco::PFClusterCollection>  (fPFClustHName);
  fTokPFClustEName    = iC.consumes<reco::PFClusterCollection>  (fPFClustEName);
  fTokPFClustOName    = iC.consumes<reco::PFClusterCollection>  (fPFClustOName);
  fTokPFClustFName    = iC.consumes<reco::PFClusterCollection>  (fPFClustFName);
  fTokPFClustSName    = iC.consumes<reco::PFClusterCollection>  (fPFClustSName);
}

//--------------------------------------------------------------------------------------------------
FillerPF::~FillerPF(){}

//--------------------------------------------------------------------------------------------------
void FillerPF::fill(TClonesArray *array,const edm::Event &iEvent,const edm::EventSetup &iSetup,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber){
  assert(array);

  edm::Handle<reco::PFCandidateCollection> hPFProduct;
  iEvent.getByToken(fTokPFName,hPFProduct);
  assert(hPFProduct.isValid());
  const reco::PFCandidateCollection *PFCol = hPFProduct.product();
  

  //const PFClusterCollection *pfCHCAL   = 0;
  edm::Handle<reco::PFClusterCollection> hPFCHCAL;
  iEvent.getByToken(fTokPFClustHName,hPFCHCAL);
  assert(hPFCHCAL.isValid());

  edm::Handle<reco::PFClusterCollection> hPFCECAL;
  iEvent.getByToken(fTokPFClustEName,hPFCECAL);
  assert(hPFCECAL.isValid());

  edm::Handle<reco::PFClusterCollection> hPFCHO;
  iEvent.getByToken(fTokPFClustOName,hPFCHO);
  assert(hPFCHO.isValid());

  edm::Handle<reco::PFClusterCollection> hPFCHF;
  iEvent.getByToken(fTokPFClustFName,hPFCHF);
  assert(hPFCHF.isValid());

  edm::Handle<reco::PFClusterCollection> hPFCPS;
  iEvent.getByToken(fTokPFClustSName,hPFCPS);
  assert(hPFCPS.isValid());
  //pfCHCAL = hPFCHCAL.product();

  TClonesArray &rArray = *array;
  int pId = 0; 
  for(reco::PFCandidateCollection::const_iterator itPF = PFCol->begin(); itPF!=PFCol->end(); itPF++) {
    pId++;
    // construct object and place in array
    assert(rArray.GetEntries() < rArray.GetSize());
    const int index = rArray.GetEntries();
    new(rArray[index]) baconhep::TPFPart();
    baconhep::TPFPart *pPF = (baconhep::TPFPart*) rArray[index];

    // Kinematics
    //==============================    
    pPF->pt     = itPF->pt();
    pPF->eta    = itPF->eta();
    pPF->phi    = itPF->phi();
    pPF->m      = itPF->mass();
    pPF->e      = itPF->energy();
    pPF->q      = itPF->charge();
    pPF->pfType = itPF->particleId();
    pPF->ecalE  = itPF->ecalEnergy();
    pPF->hcalE  = itPF->hcalEnergy();
    pPF->avgdepth  = depth(&(*itPF),pPF,iSimHits,iRecNumber);
  } 
}
float FillerPF::depth(const reco::PFCandidate *iPF,baconhep::TPFPart *iPFPart,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber) { 
    float lTotRho  = 0; 
    float lTotE    = 0;
    //Get Calo Depth of PF Clusters
    float lRhoE    = iPF->positionAtECALEntrance().rho();
    assert(!iPF->elementsInBlocks().empty() );
    iPFPart->fraction.clear();
    iPFPart->detid.clear();
    iPFPart->ieta.clear();
    iPFPart->iphi.clear();
    iPFPart->depth.clear();
    iPFPart->gene.clear();
    for(unsigned int i0 = 0; i0 < iPF->elementsInBlocks().size(); i0++ ) { 
      reco::PFBlockRef blockRef = iPF->elementsInBlocks()[i0].first;
      if(blockRef.isNull()) continue;
      const reco::PFBlock& block = *blockRef;
      const edm::OwnVector<reco::PFBlockElement>& elements = block.elements();
      for(unsigned int iEle=0; iEle< elements.size(); iEle++) {
	// Find the tracks in the block
	reco::PFClusterRef pCluster = elements[iEle].clusterRef();
	if(pCluster.isNull()) continue;
	std::array<double,7> energyPerDepth; 
	std::fill(energyPerDepth.begin(), energyPerDepth.end(), 0.0);
	for (auto & hitRefAndFrac : pCluster->recHitFractions()) {
	  const auto & hit = *hitRefAndFrac.recHitRef();
	  if (DetId(hit.detId()).det() == DetId::Hcal) {
	    if (hit.depth() == 0) continue;
	    HcalDetId pDetId(hit.detId());
	    iPFPart->ieta.push_back(pDetId.ieta());
	    iPFPart->iphi.push_back(pDetId.iphi());
	    iPFPart->depth.push_back(pDetId.depth());
	    iPFPart->gene.push_back(float(genE(pDetId,iSimHits,iRecNumber)));
	    iPFPart->fraction.push_back(hitRefAndFrac.fraction());
	    energyPerDepth[hit.depth()-1] += hitRefAndFrac.fraction()*hit.energy();
	    //std::cout << " --> " << pCluster->energy()  << " -- " << pCluster->position().rho() << " -- " << pCluster->layer() << " -- " << hit.depth() << " -- " << pDetId.ieta() << " -- " << hitRefAndFrac.fraction() << " -- " << hit.energy() << std::endl;
	  }
	}
	double sum = std::accumulate(energyPerDepth.begin(), energyPerDepth.end(), 0.);
	if (sum > 0) {
	  for (unsigned int i = 0; i < energyPerDepth.size(); ++i) {
	    iPFPart->depthE[i]     = energyPerDepth[i];
	    iPFPart->depthFrac[i]  = energyPerDepth[i]/sum;
	  }
	}
	if(pCluster.isNull()) continue;
	lTotRho += (pCluster->position().rho() - lRhoE)*pCluster->energy();
	lTotE   += pCluster->energy();
      }
    }
    if(lTotE == 0) return 0;
    return lTotRho/lTotE;
}
double FillerPF::genE(HcalDetId &iDetId,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber) { 
  double samplingFactor = 1;
  if(iDetId.subdet() == HcalBarrel) {
    samplingFactor = fSimParameterMap.hbParameters().samplingFactor(iDetId);
  } else if (iDetId.subdet() == HcalEndcap) {
    samplingFactor = fSimParameterMap.heParameters().samplingFactor(iDetId);
  }
  HcalDetId simIdneed;
  //int    maxEnk=0;
  double sHitEn=0;
  double maxEn=0; 
  //int    maxDepth=-1;
  // double sHitTime = -99.;
  for (int j=0; j < (int) iSimHits.size(); j++) {
    HcalDetId simId = HcalHitRelabeller::relabel((iSimHits)[j].id(), iRecNumber);
    if (simId == iDetId ){
      sHitEn += samplingFactor*((iSimHits)[j].energy());
      if((iSimHits)[j].energy() > maxEn){
	//maxEnk=j;
	simIdneed = simId;
	maxEn = (iSimHits)[j].energy();
	//maxDepth=simId.depth();
      }
    }
    /* 
   if(maxEn>0) {
      double tof = fResponse->timeOfFlight(simIdneed);
      sHitTime = (iSimHits)[maxEnk].time() - tof;
    }
    */
  }
  return sHitEn;
}

