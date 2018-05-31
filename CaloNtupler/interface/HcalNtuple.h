#ifndef CaloTrigNN_HcalNtuple_h
#define CaloTrigNN_HcalNtuple_h

#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "CalibCalorimetry/HcalTPGAlgos/interface/HcaluLUTTPGCoder.h"
#include "CalibFormats/CaloTPG/interface/HcalTPGCompressor.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"

#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloHitResponse.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameterMap.h"
#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalFeatureHFEMBit.h"
#include "SimCalorimetry/HcalTrigPrimAlgos/interface/HcalFinegrainBit.h"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include "TClonesArray.h"

#include <map>
#include <vector>
#include <TObject.h>

class CaloGeometry;
class IntegerCaloSamples;

class HcalNtuple {
public:
  HcalNtuple(bool pf, const std::vector<double>& w, 
	     int numberOfSamples,   int numberOfPresamples,
	     int numberOfSamplesHF, int numberOfPresamplesHF);
  ~HcalNtuple();
  template<typename... Digis>
    void fill(TClonesArray *iArray, const HcalTPGCoder* incoder, const HcalDbService* conditions, const HcalTrigTowerGeometry* trigTowerGeometry,const CaloGeometry* geometry, const HcalFeatureBit* LongvrsShortCut,const edm::PCaloHitContainer& iSimHits,const HcalDDDRecConstants *iRecNumber, const Digis&... digis);
  template<typename T, typename... Args>
    void addDigis(const T& collection, const Args&... digis) {
    addDigis(collection);
    addDigis(digis...);
  };
  template<typename T>
    void addDigis(const T& collection) {
    for (const auto& digi: collection) {
      addSignal(digi);
    }
  };
  template<typename D>
  void addDigis(const HcalDataFrameContainer<D>& collection) {
     for (auto i = collection.begin(); i != collection.end(); ++i) {
        D digi(*i);
        addSignal(digi);
     }
  };

 private:

  /// adds the signal to the map
  void addSignal(const HBHEDataFrame & frame);
  void addSignal(const QIE10DataFrame& frame);
  void addSignal(const QIE11DataFrame& frame);
  void addSignal(const IntegerCaloSamples & samples);
  void addFG    (const HcalTrigTowerDetId& id, std::vector<bool>& msb);
  void addUpgradeFG(const HcalTrigTowerDetId& id, int depth, const std::vector<std::bitset<2>>& bits);
  void fillHBHE(TClonesArray *array,const edm::PCaloHitContainer& iSimHits,const HcalDDDRecConstants *iRecNumber,const IntegerCaloSamples& samples, const HcalFinegrainBit& fg_algo);
  void fillHF  (TClonesArray *array,const edm::PCaloHitContainer& iSimHits,const HcalDDDRecConstants *iRecNumber,const IntegerCaloSamples& samples, const HcalFeatureBit* embit);
  void fillGen(HcalDetId &iDetId,int iIEta,int iIPhi,baconhep::THcalDep *iHcal,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber);
  bool validUpgradeFG(const HcalTrigTowerDetId& id, int depth) const;
  bool validChannel(const QIE10DataFrame& digi, int ts) const;
  // Member initialized by constructor
  const HcaluLUTTPGCoder* fIncoder;
  const HcalDbService*    fConditions;
  double fThreshold;
  std::vector<double> fWeights;
  int fNumberOfSamples;
  int fNumberOfPresamples;
  int fNumberOfSamplesHF;
  int fNumberOfPresamplesHF;

  // Algo1: isPeak = TS[i-1] < TS[i] && TS[i] >= TS[i+1]
  // Algo2: isPeak = TSS[i-1] < TSS[i] && TSS[i] >= TSS[i+1],
  // TSS[i] = TS[i] + TS[i+1]
  // Default: Algo2
  int peak_finder_algorithm_;

  // Member not initialzed
  //std::vector<HcalTrigTowerDetId> towerIds(const HcalDetId & id) const;

  const HcalTrigTowerGeometry * fTrigTowerGeometry;
  CaloHitResponse* fResponse;

  typedef std::map<HcalTrigTowerDetId, IntegerCaloSamples> SumMap;
  SumMap fSumMap;  

  struct HFUpgradeDetails {
     IntegerCaloSamples samples;
     QIE10DataFrame digi;
     std::vector<bool> validity;
     std::vector<bool> fgbit;
  };
  typedef std::map<HcalTrigTowerDetId, std::map<uint32_t, std::array<HFUpgradeDetails, 4>>> HFUpgradeDetailMap;
  HFUpgradeDetailMap fHFUpgradeDetailMap;
  
  // ==============================
  // =  HF Veto
  // ==============================
  // Sum = Long + Short;" // intermediate calculation. 
  //  if ((Short < MinSignalThresholdET OR Long  < MinSignalThresholdET)
  //     AND Sum > PMTNoiseThresholdET) VetoedSum = 0; 
  //  else VetoedSum = Sum; 
  // ==============================
  // Map from FG id to veto booleans
  HcalFeatureBit* LongvrsShortCut;

  typedef std::map<HcalTrigTowerDetId, std::vector<bool> > FGbitMap;
  FGbitMap fFGMap;
 
  typedef std::vector<HcalFinegrainBit::Tower> FGUpgradeContainer;
  typedef std::map<HcalTrigTowerDetId, FGUpgradeContainer> FGUpgradeMap;
  FGUpgradeMap fFGUpgradeMap;

  HcalSimParameterMap fSimParameterMap;
  
  // HE constants
  static const int HBHE_OVERLAP_TOWER = 16;
  static const int LAST_FINEGRAIN_DEPTH = 6;
  static const int LAST_FINEGRAIN_TOWER = 28;

  // Fine-grain in HF ignores tower 29, and starts with 30
  static const int FIRST_FINEGRAIN_TOWER = 30;

  static const int QIE8_LINEARIZATION_ET = HcaluLUTTPGCoder::QIE8_LUT_BITMASK;
  static const int QIE10_LINEARIZATION_ET = HcaluLUTTPGCoder::QIE10_LUT_BITMASK;
  static const int QIE11_LINEARIZATION_ET = HcaluLUTTPGCoder::QIE11_LUT_BITMASK;
  // Consider CaloTPGTranscoderULUT.h for values
  static const int QIE10_MAX_LINEARIZATION_ET = 0x7FF;
  static const int QIE11_MAX_LINEARIZATION_ET = 0x7FF;
};

template<typename... Digis> void HcalNtuple::fill(TClonesArray* iArray, const HcalTPGCoder* incoder,const HcalDbService* conditions,const HcalTrigTowerGeometry* trigTowerGeometry,const CaloGeometry* geometry,const HcalFeatureBit* LongvrsShortCut,const edm::PCaloHitContainer& iSimHits,const HcalDDDRecConstants *iRecNumber,const Digis&... digis) {
  //Fill Stuff
  fTrigTowerGeometry = trigTowerGeometry;
  fIncoder    = dynamic_cast<const HcaluLUTTPGCoder*>(incoder);
  fConditions = conditions;
  fSumMap.clear();
  fFGUpgradeMap.clear();
  fHFUpgradeDetailMap.clear();
  //Merge
  addDigis(digis...);
  //Load stuff from Gen info
  fResponse = new CaloHitResponse(NULL, (CaloShapes*)NULL);
  fResponse->setGeometry(geometry);

  // Prepare the fine-grain calculation algorithm for HB/HE
  int version = 0;
  version = fConditions->getHcalTPParameters()->getFGVersionHBHE();
  HcalFinegrainBit fg_algo(version);

  // VME produces additional bits on the front used by lumi but not the trigger, this shift corrects those out by right shifting over them.
  for (auto& item: fSumMap) { //loop over our buddies
    HcalTrigTowerDetId detId(item.second.id());
    if(detId.ietaAbs() >= fTrigTowerGeometry->firstHFTower(detId.version())) { 
      if (detId.version() == 0) {
	std::cout << "===> Something is wrong !!!!! analyzeHF is dead" << std::endl;
      } else if (detId.version() == 1) {
	fillHF(iArray,iSimHits,iRecNumber,item.second, LongvrsShortCut);
      } else {
	// Things are going to go poorly
      }
    } else {
      // Determine which energy reconstruction path to take based on the
      // fine-grain availability:
      //  * QIE8 TP add entries into fgMap_
      //  * QIE11 TP add entries into fgUpgradeMap_
      //    (not for tower 16 unless HB is upgraded, too)
      //!!!!! Using the same fine grained algo for both barrel and endcap
      if (fFGMap.find(item.first) != fFGMap.end()) { 
	fillHBHE(iArray,iSimHits,iRecNumber, item.second, fg_algo);
      } else if (fFGUpgradeMap.find(item.first) != fFGUpgradeMap.end()) {
	fillHBHE(iArray,iSimHits,iRecNumber, item.second, fg_algo);
      }
    }
  }
  // Free up some memory
  fSumMap.clear();
  fFGUpgradeMap.clear();
  fHFUpgradeDetailMap.clear();
  return;
}


#endif
