#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
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
#include <algorithm>

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace baconhep;

//--------------------------------------------------------------------------------------------------
FillerPF::FillerPF(const edm::ParameterSet &iConfig,edm::ConsumesCollector && iC):
  fPFName        (iConfig.getUntrackedParameter<std::string>("edmName","particleFlow")),
  fPFClustHName  (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterHCAL")),
  fPFClustEName  (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterECAL")),
  fPFClustOName  (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterHO")),
  fPFClustFName  (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterHF")),
  fPFClustSName  (iConfig.getUntrackedParameter<std::string>("edmName","particleFlowClusterPS")),
  fGenEvtInfoName(iConfig.getUntrackedParameter<std::string>("edmGenEventInfoName","generator")),
  fGenParName    (iConfig.getUntrackedParameter<std::string>("edmGenParticlesName","genParticles"))
{
  fTokGenEvent        = iC.mayConsume<GenEventInfoProduct>        (fGenEvtInfoName);
  fTokGenPar          = iC.mayConsume<reco::GenParticleCollection>(fGenParName);
  fTokPFName          = iC.consumes<reco::PFCandidateCollection>  (fPFName);
  fTokPFClustHName    = iC.consumes<reco::PFClusterCollection>    (fPFClustHName);
  fTokPFClustEName    = iC.consumes<reco::PFClusterCollection>    (fPFClustEName);
  fTokPFClustOName    = iC.consumes<reco::PFClusterCollection>    (fPFClustOName);
  fTokPFClustFName    = iC.consumes<reco::PFClusterCollection>    (fPFClustFName);
  fTokPFClustSName    = iC.consumes<reco::PFClusterCollection>    (fPFClustSName);
}

//--------------------------------------------------------------------------------------------------
FillerPF::~FillerPF(){}

//--------------------------------------------------------------------------------------------------
void FillerPF::fill(TClonesArray *array,const edm::Event &iEvent,const edm::EventSetup &iSetup,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber){
  assert(array);

  const reco::PFCandidateCollection *PFCol = 0;
  edm::Handle<reco::PFCandidateCollection> hPFProduct;
  iEvent.getByToken(fTokPFName,hPFProduct);
  assert(hPFProduct.isValid());
  PFCol = hPFProduct.product();
  

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

  edm::Handle<GenEventInfoProduct> hGenEvtInfoProduct;
  iEvent.getByToken(fTokGenEvent,hGenEvtInfoProduct);
  assert(hGenEvtInfoProduct.isValid());

  const reco::GenParticleCollection * GenCol = 0;
  edm::Handle<reco::GenParticleCollection> hGenParProduct;
  iEvent.getByToken(fTokGenPar,hGenParProduct);
  assert(hGenParProduct.isValid()); 
  GenCol = hGenParProduct.product();

  std::vector<int> pdgId; std::vector<float> eta; std::vector<float> phi;
  int genId = 0;
  for (reco::GenParticleCollection::const_iterator itGenP = GenCol->begin(); itGenP!=GenCol->end(); ++itGenP){
       genId++;
       //std::cout << "------Gen particle " << genId << " ------" << std::endl;
       //std::cout << "pdg/pt/eta/phi: " << itGenP->pdgId() << "/" << itGenP->pt() << "/" << itGenP->eta() << "/" << itGenP->phi() << std::endl;
       pdgId.push_back(itGenP->pdgId()); eta.push_back(itGenP->eta()); phi.push_back(itGenP->phi());
  }

  TClonesArray &rArray = *array;
  int pId = 0; 
  int nGenMatch = 0;
  for(reco::PFCandidateCollection::const_iterator itPF = PFCol->begin(); itPF!=PFCol->end(); itPF++) {
    //std::cout << "PF Candidate: " << pId << std::endl;
    pId++;
    // construct object and place in array
    //pPF->genE = depthGenSum(&(*itPF),iSimHits,iRecNumber);
    
    //if (std::abs(itPF->eta()) > 3. || std::abs(itPF->eta()) < 1.7) continue;
    //if (genE / itPF->energy() < 0.1) continue;

    assert(rArray.GetEntries() < rArray.GetSize());
    const int index = rArray.GetEntries();
    new(rArray[index]) baconhep::TPFPart();
    baconhep::TPFPart *pPF = (baconhep::TPFPart*) rArray[index];

    //std::cout << "------PF Candidate " << pId << " ------" << std::endl;
    //std::cout << "e/pt/eta/phi: " << itPF->energy() << "/" << itPF->pt() << "/" << itPF->eta() << "/" << itPF->phi() << std::endl;
    // Kinematics
    //==============================    
    pPF->pt        = itPF->pt();
    pPF->eta       = itPF->eta();
    pPF->phi       = itPF->phi();
    pPF->m         = itPF->mass();
    pPF->e         = itPF->energy();
    pPF->q         = itPF->charge();
    pPF->pfType    = itPF->particleId();
    pPF->ecalE     = itPF->ecalEnergy();
    pPF->hcalE     = itPF->hcalEnergy();
    pPF->genE = depthGenSum(&(*itPF),iSimHits,iRecNumber);
    //pPF->avgdepth  = depth(&(*itPF),pPF,iSimHits,iRecNumber);
    float genE = 0.;
    genE           = depth(&(*itPF),pPF,iSimHits,iRecNumber);

    for(unsigned int i0 = 0; i0 < itPF->hcalDepthEnergyFractions().size(); i0++) {
	pPF->depthFrac[i0] = itPF->hcalDepthEnergyFractions()[i0];
        pPF->depthE[i0]    = itPF->hcalDepthEnergyFractions()[i0]*itPF->hcalEnergy();}
    //std::cout << "\n" << "CANDIDATE: pftype/pt/eta/phi/hcalE/rawhcalE: " << itPF->particleId() << "/" << itPF->pt() << "/" << itPF->eta() << "/" << itPF->phi() << "/" << itPF->hcalEnergy() << "/" << itPF->rawHcalEnergy() << "\t\t-- depthFrac0..6: " << itPF->hcalDepthEnergyFractions()[0] << " -- " <<itPF->hcalDepthEnergyFractions()[1] << " -- " << itPF->hcalDepthEnergyFractions()[2] << " -- " << itPF->hcalDepthEnergyFractions()[3] << " -- " << itPF->hcalDepthEnergyFractions()[4] << " -- " << itPF->hcalDepthEnergyFractions()[5] << " -- " << itPF->hcalDepthEnergyFractions()[6] << "\n" << std::endl;

    float dRmin  = 999.;
    for(unsigned int i0 = 0; i0 < eta.size(); i0++){
      float dR = deltaR( eta[i0], phi[i0], itPF->eta(), itPF->phi());
      if (dR < dRmin) 
	dRmin = dR; 
    }
    int match    = 0;
    if ( dRmin < 0.2 && genE>0 ){ 
	match = 1;
        ++nGenMatch;
    }
    pPF->genMatch = match;
    //if (match == 1) std::cout << "Gen match: " << match << std::endl;
    //if (itPF == PFCol->end() - 1){ pPF->NGenMatch = nGenMatch; } 
  } 
}
float FillerPF::depth(const reco::PFCandidate *iPF,baconhep::TPFPart *iPFPart,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber) { 
    float lTotRho  = 0; 
    float lTotE    = 0;
    float genSum   = 0;
    //Get Calo Depth of PF Clusters
    float lRhoE    = iPF->positionAtECALEntrance().rho();
    assert(!iPF->elementsInBlocks().empty() );
    //iPFPart->fraction.clear();
    //iPFPart->detid.clear();
    //iPFPart->ieta.clear()PF->hcalDepthEnergyFraction;
    //iPFPart->iphi.clear();
    //iPFPart->depth.clear();
    //iPFPart->gene.clear();
    /*for(int i0 = 0; i0 < 7; i0++) { 
      iPFPart->depthE[i0]       = 0; 
      iPFPart->depthFrac[i0]    = 0; 
      iPFPart->depthESum[i0]    = 0; 
      iPFPart->depthgenE[i0]    = 0;
      iPFPart->depthgenESum[i0] = 0;
    }*/
    //std::cout << "# of blocks: " << iPF->elementsInBlocks().size() << std::endl;
    //std::cout << "PF energy: " << iPF->energy() << "\t eta: " << iPF->eta() << "\t phi: " << iPF->phi() << std::endl;
    for(unsigned int i0 = 0; i0 < iPF->elementsInBlocks().size(); i0++ ) { 
      if(i0 >= 1) break; //consider only first block
      //std::cout << "Block " << i0 << " of " << iPF->elementsInBlocks().size() - 1 << std::endl;
      reco::PFBlockRef blockRef = iPF->elementsInBlocks()[i0].first;
      if(blockRef.isNull()) continue;
      const reco::PFBlock& block = *blockRef;
      const edm::OwnVector<reco::PFBlockElement>& elements = block.elements();
      float totGenE    = 0; 
      float totRecHitE = 0; 
      //std::cout << "# of elements: " << elements.size() << std::endl; 
      for(unsigned int iEle=0; iEle< elements.size(); iEle++) {
        //PFBlockElement::Type type = elements[iEle].type();
        //assert( type == PFBlockElement::HCAL );
	// Find the tracks in the block
        //std::cout << "Element type: " << elements[iEle].type() << std::endl;
	if(elements[iEle].type() != 5) continue;// && elements[iEle].type() != 4) continue; //consider only HCAL element of PF cand 
        assert(elements[iEle].type() == 5);
	//PFBlockElement::Type type = elements[iEle].type();
	//assert( type == PFBlockElement::HCAL );

        //std::cout << "Element type: " <<  elements[iEle].type() << ", energy: " << iPF->energy() << " (eta,phi) = (" << iPF->eta() << "," << iPF->phi() << std::endl;
	reco::PFClusterRef pCluster = elements[iEle].clusterRef();
	if(pCluster.isNull()) continue;
	std::array<double,7> energyPerDepth; 
	std::array<double,7> genenergyPerDepth; 
	std::fill(energyPerDepth.begin(), energyPerDepth.end(), 0.0);
	std::fill(genenergyPerDepth.begin(), genenergyPerDepth.end(), 0.0);
	float ecalESum = 0.;
   	//for( int x = 0; x < 7; ++x){ std::cout << iPF->hcalDepthEnergyFractions()[x] << " - ";}
	//std::cout << "\n";
	for (auto & hitRefAndFrac : pCluster->recHitFractions()) {
	  const auto & hit = *hitRefAndFrac.recHitRef();
	  if (DetId(hit.detId()).det() == DetId::Hcal) {

	    if (hit.depth() == 0) continue;
	    HcalDetId pDetId(hit.detId());
            //std::cout << "CL depth: " << hit.depth() << "\tenergy: " << hit.energy() << "\tieta: " << pDetId.ieta() << "\tiphi: " << pDetId.iphi() << std::endl;
	    //iPFPart->ieta.push_back(pDetId.ieta());
	    //iPFPart->iphi.push_back(pDetId.iphi());
	    //iPFPart->depth.push_back(pDetId.depth());
	    //iPFPart->gene.push_back(float(genE(pDetId,iSimHits,iRecNumber)));
	    //iPFPart->fraction.push_back(hitRefAndFrac.fraction());
	    energyPerDepth[hit.depth()-1] += hitRefAndFrac.fraction()*hit.energy();
	    float tmpgenE = float(genE(pDetId,iSimHits,iRecNumber));
	    genenergyPerDepth[hit.depth()-1] += hitRefAndFrac.fraction()*tmpgenE;
	    //totGenE    +=  tmpgenE*hitRefAndFrac.fraction();
	    //totRecHitE +=  hit.energy()*hitRefAndFrac.fraction();
	    //std::cout << " --> " << tmpgenE*hitRefAndFrac.fraction() << " -- " << hit.energy()*hitRefAndFrac.fraction() << " cluster---> " << pCluster->energy()  << " --ieta " <<  pDetId.ieta() << " --iphi  " << pDetId.iphi() << " --depth " << pDetId.depth() << " -- " << hitRefAndFrac.fraction() << std::endl;
	    //pCluster->position().rho() << " -- " << pCluster->layer() << " -- " << hit.depth() << " -- " << pDetId.ieta() << " -- " << hitRefAndFrac.fraction() << " -- " << hit.energy() << std::endl;
	    //std::cout << hit.depth() << " - " << pDetId.depth() << std::endl;
	  }
	  else if(DetId(hit.detId()).det() == 3) {
	    float tmpE = hit.energy();
            ecalESum += hitRefAndFrac.fraction()*tmpE;
	    //std::cout << ecalESum << std::endl;
	  }
	}
	double sum = std::accumulate(energyPerDepth.begin(), energyPerDepth.end(), 0.);
        genSum     = std::accumulate(genenergyPerDepth.begin(), genenergyPerDepth.end(), 0.);

	//std::cout << "sum: " << sum << std::endl;
	/*if (sum > 0) {
	  for (unsigned int i = 0; i < energyPerDepth.size(); ++i) {
	    iPFPart->depthE[i]        = energyPerDepth[i];
	    iPFPart->depthgenE[i]     = genenergyPerDepth[i];
	    iPFPart->depthFrac[i]     = energyPerDepth[i]/sum;
	    iPFPart->depthESum[i]     += energyPerDepth[i];
	    iPFPart->depthgenESum[i]  += genenergyPerDepth[i];
	  }
	}*/
	//std::cout << "Linked cluster: " << energyPerDepth[0]/sum << " -- "<< energyPerDepth[1]/sum << " -- " << energyPerDepth[2]/sum << " -- " << energyPerDepth[3]/sum << " -- " << energyPerDepth[4]/sum << " -- " << energyPerDepth[5]/sum << " -- " << energyPerDepth[6]/sum << std::endl;
	iPFPart->ecalSum = ecalESum;
	if(pCluster.isNull()) continue;
	lTotRho += (pCluster->position().rho() - lRhoE)*pCluster->energy();
	lTotE   += pCluster->energy();
      }
      //std::cout << "===> " << totGenE << " -- " << totRecHitE << " -- " << iPF->hcalEnergy() << " -- " << iPF->eta() << std::endl;
    }
    if(lTotE == 0) return 0;
    //return lTotRho/lTotE;
    return genSum;
    //std::cout << iPFPart->depthE[0] << " - " << iPFPart->depthE[1] << " -- " << iPFPart->depthE[2] << " -- " << iPFPart->depthE[3] << " -- " << iPFPart->depthE[4] << " -- " << iPFPart->depthE[5] << " -- " << iPFPart->depthE[6] << std::endl; 
}



float FillerPF::depthGenSum(const reco::PFCandidate *iPF,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber) {
    float gensum = 0.;
    assert(!iPF->elementsInBlocks().empty() );
    for(unsigned int i0 = 0; i0 < iPF->elementsInBlocks().size(); i0++ ) {
      if(i0 >= 1) break; //consider only first block
      reco::PFBlockRef blockRef = iPF->elementsInBlocks()[i0].first;
      if(blockRef.isNull()) continue;
      const reco::PFBlock& block = *blockRef;
      const edm::OwnVector<reco::PFBlockElement>& elements = block.elements();
      float totGenE    = 0;
      //std::cout << "# of elements: " << elements.size() << std::endl; 
      for(unsigned int iEle=0; iEle< elements.size(); iEle++) {
        //std::cout << "Element " << iEle << " of " << elements.size() - 1 <<std::endl;
        //PFBlockElement::Type type = elements[iEle].type();
        //assert( type == PFBlockElement::HCAL );
        // Find the tracks in the block
        if(elements[iEle].type() != 5) continue;// && elements[iEle].type() != 4) continue; //consider only HCAL element of PF cand 
        //std::cout << "Element type: " <<  elements[iEle].type() << ", energy: " << iPF->energy() << " (eta,phi) = (" << iPF->eta() << "," << iPF->phi() << std::endl;
        reco::PFClusterRef pCluster = elements[iEle].clusterRef();
        if(pCluster.isNull()) continue;
        std::array<double,7> genenergyPerDepth;
        std::fill(genenergyPerDepth.begin(), genenergyPerDepth.end(), 0.0);
        for (auto & hitRefAndFrac : pCluster->recHitFractions()) {
          const auto & hit = *hitRefAndFrac.recHitRef();
          if (DetId(hit.detId()).det() == DetId::Hcal) {

            if (hit.depth() == 0) continue;
            HcalDetId pDetId(hit.detId());
            float tmpgenE = float(genE(pDetId,iSimHits,iRecNumber));
            genenergyPerDepth[hit.depth()-1] += hitRefAndFrac.fraction()*tmpgenE;
          }
        }
        gensum = std::accumulate(genenergyPerDepth.begin(), genenergyPerDepth.end(), 0.);
      }
    }
    return gensum;
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

