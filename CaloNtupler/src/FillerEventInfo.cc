#include "CaloTrigNN/CaloNtupler/interface/FillerEventInfo.hh"
#include "CaloTrigNN/DataFormats/interface/TEventInfo.hh"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/Handle.h"
#include <TLorentzVector.h>
#include <string>
#include <TClonesArray.h>
using namespace baconhep;

//--------------------------------------------------------------------------------------------------
FillerEventInfo::FillerEventInfo(const edm::ParameterSet &iConfig, edm::ConsumesCollector && iC):
     fPUInfoName    (iConfig.getUntrackedParameter<std::string>("edmPileupInfoName","addPileupInfo"))
{
      fTokPUInfoName     = iC.consumes< std::vector<PileupSummaryInfo> > (fPUInfoName);
}
//--------------------------------------------------------------------------------------------------
FillerEventInfo::~FillerEventInfo(){}

//--------------------------------------------------------------------------------------------------
void FillerEventInfo::fill(TClonesArray * fEvtArr,
                           const edm::Event &iEvent)//, const reco::Vertex &pv)
{


    TClonesArray & rEvtArr = *fEvtArr;
    const int index = rEvtArr.GetEntries();
    new(rEvtArr[index]) baconhep::TEventInfo();

    baconhep::TEventInfo * pEvt = (baconhep::TEventInfo*)rEvtArr[index];

    pEvt->runNum  = iEvent.id().run();
    pEvt->lumiSec = iEvent.luminosityBlock();
    pEvt->evtNum  = iEvent.id().event();
    //
    // Pile-up info
    //==============================
    edm::Handle< std::vector<PileupSummaryInfo> > hPileupInfoProduct;
    iEvent.getByToken(fTokPUInfoName,hPileupInfoProduct);
    assert(hPileupInfoProduct.isValid());
    const std::vector<PileupSummaryInfo> *inPUInfos = hPileupInfoProduct.product();
    for (std::vector<PileupSummaryInfo>::const_iterator itPUInfo = inPUInfos->begin(); itPUInfo!=inPUInfos->end(); ++itPUInfo) {
      if(itPUInfo->getBunchCrossing()==0) {
        pEvt->nPU      = itPUInfo->getPU_NumInteractions();
        pEvt->nPUmean  = itPUInfo->getTrueNumInteractions();
      } else if(itPUInfo->getBunchCrossing()==-1) { 
        pEvt->nPUm     = itPUInfo->getPU_NumInteractions();
        pEvt->nPUmeanm = itPUInfo->getTrueNumInteractions();
      } else if(itPUInfo->getBunchCrossing()==1) {
        pEvt->nPUp     = itPUInfo->getPU_NumInteractions();
        pEvt->nPUmeanp = itPUInfo->getTrueNumInteractions();
    }
  }
}
