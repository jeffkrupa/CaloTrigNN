#ifndef HcalTrigPrimDigiNtupler_h
#define HcalTrigPrimDigiNtupler_h

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalTriggerPrimitiveAlgo.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "CaloTrigNN/CaloNtupler/interface/HcalNtuple.h"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

class HcalTrigPrimDigiNtupler : public edm::stream::EDProducer<> {
public:

  explicit HcalTrigPrimDigiNtupler(const edm::ParameterSet& ps);
  ~HcalTrigPrimDigiNtupler() override { endJob(); delete fHcalArr;}

  /**Produces the EDM products,*/
  void produce(edm::Event& e, const edm::EventSetup& c) override;
  void endJob();
  
private:
  HcalNtuple fNtuple;

  /// input tags for HCAL digis
  std::vector<edm::InputTag> fInputLabel;
  std::vector<edm::InputTag> fInputUpgradeLabel;
  // this seems a strange way of doing things
  edm::EDGetTokenT<HBHEDigiCollection> fHB;
  edm::EDGetTokenT<HFDigiCollection> fHF;
  edm::EDGetTokenT<QIE11DigiCollection> fHBHEUp;
  edm::EDGetTokenT<QIE10DigiCollection> fHFUp;
  edm::EDGetTokenT<edm::PCaloHitContainer> fSHitToken;
  
  double fMinLongEnergy, fMinShortEnergy, fLongShortSlope, fLongShortOffset;
  bool fUpgrade;
  bool fLegacy;
  bool fHFEMB;
  edm::ParameterSet fLongShortCut;
  const HcalDDDRecConstants *fRecNumber;
  const CaloGeometry* fGeometry;
  TFile *fFile;
  TTree *fTree;
  TClonesArray *fHcalArr;
};

#endif

