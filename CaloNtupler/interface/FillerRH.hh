#ifndef CaloTrigNN_NTUPLER_FILLERRH_HH
#define CaloTrigNN_NTUPLER_FILLERRH_HH

#include <string>

// forward class declarations
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitDefs.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloHitResponse.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameterMap.h"
#include "CaloTrigNN/DataFormats/interface/TRHPart.hh"

class TClonesArray;
class FillerRH
  {
  public:
    FillerRH(const edm::ParameterSet &iConfig,edm::ConsumesCollector && iC);
    ~FillerRH();
      
    void fill(TClonesArray       *array,    // output array to be filled
	      const edm::Event   &iEvent,const edm::EventSetup &iSetup,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber);  // event info

    void fillGen(HcalDetId &iDetId,int iIEta,int iIPhi,baconhep::TRHPart *iHcal,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber);
    void fillTS (HcalDetId &iDetId,baconhep::TRHPart *iHcal,const HBHEChannelInfoCollection *iChannelInfo);   

  protected:
    // EDM object collection names
    edm::InputTag fRHName;
    edm::InputTag fChanInfoName;
    const CaloSubdetectorGeometry *fHCAL;
    edm::EDGetTokenT<HBHERecHitCollection> fTokRH;
    edm::EDGetTokenT<HBHEChannelInfoCollection> fTokChanInfo;
    HcalSimParameterMap fSimParameterMap;
    CaloHitResponse* fResponse;
};
#endif
