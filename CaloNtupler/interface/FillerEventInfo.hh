#ifndef CALOTRIGNN_CALONTUPLER_FILLEREVENTINFO_HH
#define CALOTRIGNN_CALONTUPLER_FILLEREVENTINFO_HH

#include <string>

// forward class declarations
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
//#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
//#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "CaloTrigNN/DataFormats/interface/TEventInfo.hh"
#include "FWCore/Framework/interface/ConsumesCollector.h"
//#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
//#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/METReco/interface/CaloMET.h"
//#include "DataFormats/METReco/interface/CaloMETCollection.h"
//#include "DataFormats/METReco/interface/PFMET.h"
//#include "DataFormats/METReco/interface/PFMETCollection.h"
//#include "DataFormats/PatCandidates/interface/MET.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
//#include "DataFormats/Common/interface/TriggerResults.h"
//#include "DataFormats/METReco/interface/BeamHaloSummary.h"

class TClonesArray;
class TEventInfo;
class FillerEventInfo
  {
    public:
       FillerEventInfo(const edm::ParameterSet &iConfig, edm::ConsumesCollector && iC);
      ~FillerEventInfo();
      
      void fill(//TEventInfo         *evtInfo,       // output object to be filled
		TClonesArray       *fEvtArr,
                const edm::Event   &iEvent);        // EDM event info
		//const reco::Vertex &pv);            // event primary vertex

    std::string fPUInfoName;	      
    edm::EDGetTokenT<std::vector<PileupSummaryInfo> >  fTokPUInfoName; 
  };


#endif
