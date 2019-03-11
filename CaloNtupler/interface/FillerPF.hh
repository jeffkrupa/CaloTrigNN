#ifndef CaloTrigNN_NTUPLER_FILLERPF_HH
#define CaloTrigNN_NTUPLER_FILLERPF_HH

#include <string>

// forward class declarations
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitDefs.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloHitResponse.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameterMap.h"
#include "CaloTrigNN/DataFormats/interface/TPFPart.hh"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "RecoParticleFlow/PFProducer/interface/PFAlgo.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

class PFAlgo;
class TClonesArray;
class FillerPF
  {
  public:
    FillerPF(const edm::ParameterSet &iConfig,edm::ConsumesCollector && iC);
    ~FillerPF();
      
    void fill(TClonesArray       *array,    // output array to be filled
	      const edm::Event   &iEvent,const edm::EventSetup &iSetup,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber);  // event info

    float  depth(const reco::PFCandidate *iPF,baconhep::TPFPart *iPFPart,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber);
    double genE(HcalDetId &iDetId,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber);
    float  depthGenSum(const reco::PFCandidate *iPF, const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber);

  protected:
    // EDM object collection names
    std::string fPFName;
    edm::EDGetTokenT<reco::PFCandidateCollection>   fTokPFName;
    std::string fPFClustHName;
    std::string fPFClustEName;
    std::string fPFClustOName;
    std::string fPFClustFName;
    std::string fPFClustSName;
    std::string fGenEvtInfoName;
    std::string fLHEEvtInfoName;
    std::string fGenParName;
    edm::EDGetTokenT<GenEventInfoProduct>         fTokGenEvent;
    edm::EDGetTokenT<reco::GenParticleCollection> fTokGenPar;
    edm::EDGetTokenT<reco::PFClusterCollection>   fTokPFClustHName;
    edm::EDGetTokenT<reco::PFClusterCollection>   fTokPFClustEName;
    edm::EDGetTokenT<reco::PFClusterCollection>   fTokPFClustOName;
    edm::EDGetTokenT<reco::PFClusterCollection>   fTokPFClustFName;
    edm::EDGetTokenT<reco::PFClusterCollection>   fTokPFClustSName;
    HcalSimParameterMap fSimParameterMap;

    private:
      std::auto_ptr<PFAlgo> pfAlgo_;
};
#endif
