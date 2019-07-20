#ifndef CaloTrigNN_NTUPLER_FILLERGENINFO_HH
#define CaloTrigNN_NTUPLER_FILLERGENINFO_HH

#include <string>

// forward class declarations
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "CaloTrigNN/DataFormats/interface/TGenParticle.hh"

class TClonesArray;
class FillerGenInfo
{

    public:
       FillerGenInfo(const edm::ParameterSet &iConfig,edm::ConsumesCollector && iC);
      ~FillerGenInfo();
      
      void fill(TClonesArray     *particlesArr,  const edm::Event &iEvent);
    protected:

      std::string fPFCandName;
      std::string fPUInfoName;
      std::string fPVName;
      std::string fBSName;
      std::string fCaloMETName;
      std::string fMETName;
      edm::InputTag fPFMETName;
      std::string fPFMETCName;
      std::string fMVAMETName;
      edm::InputTag fPUPPETName;
      std::string fPUPPETCName;
      std::string fALPACAMETName;
      std::string fPALPACAMETName;
      std::string fRhoIsoName;
      std::string fRhoJetName;
      bool fUseFilters;
      bool fUseAOD;
    protected:        
      // EDM object collection names
      std::string fGenEvtInfoName;
      std::string fLHEEvtInfoName;
      std::string fGenParName;
      bool fFillAll;
      edm::EDGetTokenT<GenEventInfoProduct>         fTokGenEvent;
      edm::EDGetTokenT<reco::GenParticleCollection> fTokGenPar;
      edm::EDGetTokenT<LHEEventProduct>             fTokLHEEventInfo;
      bool fFillLHEWeights;
};
#endif
