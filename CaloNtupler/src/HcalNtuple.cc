#include "CaloTrigNN/CaloNtupler/interface/HcalNtuple.h"

#include "CalibFormats/CaloObjects/interface/IntegerCaloSamples.h"
#include "CondFormats/HcalObjects/interface/HcalTPParameters.h"
#include "CondFormats/HcalObjects/interface/HcalTPChannelParameters.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"

#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"

#include <iostream>

using namespace std;

HcalNtuple::HcalNtuple( bool pf, const std::vector<double>& w, 
			int numberOfSamples, int numberOfPresamples,
			int numberOfSamplesHF, int numberOfPresamplesHF
			)
  : fIncoder(nullptr), fThreshold(0), fWeights(w), fNumberOfSamples(numberOfSamples),
    fNumberOfPresamples(numberOfPresamples),fNumberOfSamplesHF(numberOfSamplesHF),
    fNumberOfPresamplesHF(numberOfPresamplesHF), peak_finder_algorithm_(2) {
}
HcalNtuple::~HcalNtuple() {}

void HcalNtuple::fillHBHE(TClonesArray *array,const edm::PCaloHitContainer& iSimHits,const HcalDDDRecConstants *iRecNumber,const IntegerCaloSamples& samples, const HcalFinegrainBit& fg_algo) {
  int shrink = fWeights.size() - 1;
  IntegerCaloSamples sum(samples.id(), samples.size());

   HcalDetId detId(samples.id());
   std::vector<HcalTrigTowerDetId> ids = fTrigTowerGeometry->towerIds(detId);
   //slide algo window
   TClonesArray &rArray = *array;
   const int index = rArray.GetEntries();
   new(rArray[index]) baconhep::THcalDep();
   baconhep::THcalDep *pHcal = (baconhep::THcalDep*)rArray[index];
   pHcal->hitsidx .clear();
   pHcal->hitssat .clear();
   pHcal->hitsE   .clear();
   pHcal->EOut .clear();
   pHcal->EId  .clear();
   pHcal->fine1.clear();
   pHcal->fine2.clear();
   pHcal->ieta    = detId.ieta();
   pHcal->iphi    = detId.iphi();
   pHcal->depth   = detId.depth();
   pHcal->genE    = 0; 
   fillGen(detId,detId.ieta(),detId.iphi(),pHcal,iSimHits,iRecNumber);
   for(int ibin = 0; ibin < int(samples.size())-shrink; ++ibin) {
      int algosumvalue = 0;
      for(unsigned int i = 0; i < fWeights.size(); i++) {
	//add up value * scale factor
	// In addition, divide by two in the 10 degree phi segmentation region
	// to mimic 5 degree segmentation for the trigger
	if(ids.size()==2) algosumvalue += int(samples[ibin+i] * 0.5 * fWeights[i]);
	else algosumvalue += int(samples[ibin+i] * fWeights[i]);
      }
      if (algosumvalue<0) sum[ibin]=0;            // low-side
      else sum[ibin] = algosumvalue;              //assign value to sum[]
   }

   // Align digis and TP
   int dgPresamples = samples.presamples(); 
   int tpPresamples = fNumberOfPresamples;
   int shift        = dgPresamples - tpPresamples;
   int dgSamples    = samples.size();
   int tpSamples    = fNumberOfSamples;
   if((shift<shrink) || (shift + tpSamples + shrink > dgSamples - (peak_finder_algorithm_ - 1) )   ){
      shift=shrink;
      //tpPresamples=dgPresamples-shrink;
      tpSamples=dgSamples-(peak_finder_algorithm_-1)-shrink-shift;
   }

   //std::vector<int> finegrain(tpSamples,false);
   //IntegerCaloSamples output(samples.id(), tpSamples);
   //output.setPresamples(tpPresamples);
   for (int ibin = 0; ibin < tpSamples; ++ibin) {
      int idx = ibin + shift;
      bool isPeak = (sum[idx] > sum[idx-1] && sum[idx] >= sum[idx+1] && sum[idx] > fThreshold);
      pHcal->EOut.push_back(std::min<unsigned int>(sum[idx],QIE11_MAX_LINEARIZATION_ET));
      pHcal->EId .push_back(idx);
      if (isPeak){
	//output[ibin] = std::min<unsigned int>(sum[idx],QIE11_MAX_LINEARIZATION_ET);
	//finegrain[ibin] = fg_algo.compute(msb[idx]).to_ulong();
	if (fFGMap.find(samples.id()) != fFGMap.end()) {
	  auto& msb = fFGMap[samples.id()];
	  pHcal->fine1.push_back(msb[idx]);
	} else if (fFGUpgradeMap.find(samples.id()) != fFGUpgradeMap.end()) {
	  auto& msb = fFGUpgradeMap[samples.id()];
	  pHcal->fine1.push_back(fg_algo.compute(msb[idx]).to_ulong());
	}
      } else {
         // Not a peak
         //output[ibin] = 0;
         //finegrain[ibin] = 0;
	 pHcal->fine1.push_back(0);
      }
   }
   //outcoder_->compress(output, finegrain, result);
}
void HcalNtuple::fillHF(TClonesArray *array,const edm::PCaloHitContainer& iSimHits,const HcalDDDRecConstants *iRecNumber,const IntegerCaloSamples& samples, const HcalFeatureBit* embit) {
    // Align digis and TP
    const int shift = samples.presamples() - fNumberOfPresamplesHF;

    // Try to find the HFDetails from the map corresponding to our samples
    const HcalTrigTowerDetId detId(samples.id());
    auto lHFUpgrade = fHFUpgradeDetailMap.find(detId);
    // Missing values will give an empty digi
    if (lHFUpgrade == fHFUpgradeDetailMap.end()) return;
    std::vector<std::bitset<2>> finegrain(fNumberOfSamplesHF, false);
    // Set up out output of IntergerCaloSamples
    //IntegerCaloSamples output(samples.id(),fNumberOfSamplesHF);
    //output.setPresamples(fNumberOfPresamplesHF);
    TClonesArray &rArray = *array;
    const int index = rArray.GetEntries();
    new(rArray[index]) baconhep::THcalDep();
    baconhep::THcalDep *pHcal = (baconhep::THcalDep*)rArray[index];
    pHcal->hitsidx .clear();
    pHcal->hitssat .clear();
    pHcal->hitsE   .clear();
    pHcal->EOut .clear();
    pHcal->EId  .clear();
    pHcal->fine1.clear();
    pHcal->fine2.clear();
    pHcal->ieta    = detId.ieta();
    pHcal->iphi    = detId.iphi();
    pHcal->depth   = detId.depth();
    pHcal->genE    = 0; 
    HcalDetId dummy;
    fillGen(dummy,detId.ieta(),detId.iphi(),pHcal,iSimHits,iRecNumber);
    for (const auto& lHFItem: lHFUpgrade->second) {
        auto& lHFDetails = lHFItem.second;
	for (int ibin = 0; ibin < fNumberOfSamplesHF; ibin++) {
            const int idx = ibin + shift;
	    int long_fiber_val    = 0;
            int long_fiber_count  = 0;
            int short_fiber_val   = 0;
            int short_fiber_count = 0;
            bool saturated = false;
	    for (auto i: {0, 2}) {
               if (idx < lHFDetails[i].samples.size() and lHFDetails[i].validity[idx]) {
		 pHcal->hitsidx.push_back(idx);
		 pHcal->hitsE  .push_back(lHFDetails[i].samples[idx]);
		 pHcal->hitssat.push_back((lHFDetails[i].samples[idx] == QIE10_LINEARIZATION_ET));
		 long_fiber_val += lHFDetails[i].samples[idx];
		 saturated = saturated || (lHFDetails[i].samples[idx] == QIE10_LINEARIZATION_ET);
		 ++long_fiber_count;
               }
            }
            for (auto i: {1, 3}) {
               if (idx < lHFDetails[i].samples.size() and lHFDetails[i].validity[idx]) {
		 pHcal->hitsidx.push_back(idx);
		 pHcal->hitsE  .push_back(lHFDetails[i].samples[idx]);
		 pHcal->hitssat.push_back((lHFDetails[i].samples[idx] == QIE10_LINEARIZATION_ET));
		 short_fiber_val += lHFDetails[i].samples[idx];
		 saturated = saturated || (lHFDetails[i].samples[idx] == QIE10_LINEARIZATION_ET);
		 ++short_fiber_count;
               }
            }
	    pHcal->EId.push_back(idx);
            if (saturated) {
	      //output[ibin] = QIE10_MAX_LINEARIZATION_ET;
	       pHcal->EOut.push_back(QIE10_MAX_LINEARIZATION_ET);
            } else {
		// For details of the energy handling, see:
		// https://cms-docdb.cern.ch/cgi-bin/DocDB/ShowDocument?docid=12306
		// If both readouts are valid, average of the two energies is taken
		// division by 2 is compensated by adjusting the total scale shift in the end
		if (long_fiber_count == 2) long_fiber_val >>=1;
		if (short_fiber_count == 2) short_fiber_val >>=1;
		auto sum = long_fiber_val + short_fiber_val;
		// Similar to above, if both channels are valid, 
		// average of the two energies is calculated
		// division by 2 here is also compensated by adjusting the total scale shift in the end
		if (long_fiber_count > 0 and short_fiber_count > 0) sum >>=1;
		//output[ibin] += sum;
		pHcal->EOut.push_back(sum);
            }
            for (const auto& detail: lHFDetails) {
               if (idx < int(detail.digi.size()) and detail.validity[idx] and HcalDetId(detail.digi.id()).ietaAbs() >= FIRST_FINEGRAIN_TOWER) {
                  finegrain[ibin][1] = finegrain[ibin][1] or detail.fgbit[idx];
               }
	       pHcal->fine2.push_back(finegrain[ibin][1]);//.to_ulong());
            }
            if (embit != nullptr) {
               finegrain[ibin][0] = embit->fineGrainbit(
                     lHFDetails[1].digi, lHFDetails[3].digi,
                     lHFDetails[0].digi, lHFDetails[2].digi,
                     lHFDetails[1].validity[idx], lHFDetails[3].validity[idx],
                     lHFDetails[0].validity[idx], lHFDetails[2].validity[idx],
                     idx
               );
	       pHcal->fine1.push_back(finegrain[ibin][0]);//.to_ulong());
            }
        }
    }
    /*
    for (int bin = 0; bin < numberOfSamplesHF_; ++bin) {
       output[bin] = min({(unsigned int) QIE10_MAX_LINEARIZATION_ET, output[bin] >> (hf_lumi_shift-2)});
    }
    std::vector<int> finegrain_converted;
    for (const auto& fg: finegrain) finegrain_converted.push_back(fg.to_ulong());
    outcoder_->compress(output, finegrain_converted, result);
    */
}
void HcalNtuple::fillGen(HcalDetId &iDetId,int iIEta,int iIPhi,baconhep::THcalDep *iHcal,const edm::PCaloHitContainer& iSimHits , const HcalDDDRecConstants *iRecNumber) { 
  double samplingFactor = 1;
  if(iDetId.subdet() == HcalBarrel) {
    samplingFactor = fSimParameterMap.hbParameters().samplingFactor(iDetId);
  } else if (iDetId.subdet() == HcalEndcap) {
    samplingFactor = fSimParameterMap.heParameters().samplingFactor(iDetId);
  }
  HcalDetId simIdneed;
  int    maxEnk=0;
  double sHitEn=0;
  double maxEn=0; 
  int    maxDepth=-1;
  double sHitTime = -99.;
  iHcal->genEDepth.clear();
  iHcal->genElayer.clear();
  iHcal->genETime .clear();
  for (int j=0; j < (int) iSimHits.size(); j++) {
    HcalDetId simId = HcalHitRelabeller::relabel((iSimHits)[j].id(), iRecNumber);
    // check that the SimHits are in the same channel as the rechit
    //      if (simId == detID_rh && (*hSimHits)[j].time() > 0 && (*hSimHits)[j].time() < 40)
    //Get all the gen in the same ieta and iphi and ignore depth => watch out for HO
    if(simId.ieta() == iIEta && simId.iphi() == iIPhi) { 
      sHitEn += samplingFactor*((iSimHits)[j].energy());
      if((iSimHits)[j].energy() > maxEn){
	maxEnk=j;
	simIdneed = simId;
	maxEn = (iSimHits)[j].energy();
	maxDepth=simId.depth();
      }
    }
    if(maxEn>0) {
      double tof = fResponse->timeOfFlight(simIdneed);
      sHitTime = (iSimHits)[maxEnk].time() - tof;
    }
  }
  iHcal->genE += sHitEn;
  iHcal->genEDepth.push_back(maxDepth);
  iHcal->genElayer.push_back(sHitEn);
  iHcal->genETime .push_back(sHitTime);
}
//Stolen straight from the Trigger Prim Algo code
void HcalNtuple::addSignal(const HBHEDataFrame & frame) {
   if (frame.id().depth()==5) return;
   std::vector<HcalTrigTowerDetId> ids = fTrigTowerGeometry->towerIds(frame.id());
   assert(ids.size() == 1 || ids.size() == 2);
   IntegerCaloSamples samples1(ids[0], int(frame.size()));

   samples1.setPresamples(frame.presamples());
   fIncoder->adc2Linear(frame, samples1);

   std::vector<bool> msb;
   fIncoder->lookupMSB(frame, msb);

   if(ids.size() == 2) {
      // make a second trigprim for the other one, and split the energy
      IntegerCaloSamples samples2(ids[1], samples1.size());
      for(int i = 0; i < samples1.size(); ++i) {
         samples1[i] = uint32_t(samples1[i]*0.5);
         samples2[i] = samples1[i];
      }
      samples2.setPresamples(frame.presamples());
      addSignal(samples2);
      addFG(ids[1], msb);
   }
   addSignal(samples1);
   addFG(ids[0], msb);
}
void HcalNtuple::addFG(const HcalTrigTowerDetId& id, std::vector<bool>& msb){
  FGbitMap::iterator itr = fFGMap.find(id);
  if (itr !=  fFGMap.end()){
    std::vector<bool>& _msb = itr->second;
    for (size_t i=0; i<msb.size(); ++i)
      _msb[i] = _msb[i] || msb[i];
  }
  else fFGMap[id] = msb;
}
void HcalNtuple::addSignal(const QIE10DataFrame& frame) {
   HcalDetId detId = frame.detid();
   // prevent QIE10 calibration channels from entering TP emulation
   if(detId.subdet() != HcalForward) return;
   auto ids = fTrigTowerGeometry->towerIds(frame.id());
   for (const auto& id: ids) {
      if (id.version() == 0) continue;

      int nsamples=frame.samples();
      IntegerCaloSamples samples(id, nsamples);
      samples.setPresamples(frame.presamples());
      fIncoder->adc2Linear(frame, samples);

      // Don't add to final collection yet
      // HF PMT veto sum is calculated in analyzerHF()
      IntegerCaloSamples zero_samples(id, nsamples);
      zero_samples.setPresamples(frame.presamples());
      addSignal(zero_samples);

      auto fid = HcalDetId(frame.id());
      auto& details = fHFUpgradeDetailMap[id][fid.maskDepth()];
      auto& detail = details[fid.depth()-1];
      detail.samples = samples;
      detail.digi = frame;
      detail.validity.resize(nsamples);
      fIncoder->lookupMSB(frame, detail.fgbit);
      for (int idx = 0; idx < nsamples; ++idx){
         detail.validity[idx] = validChannel(frame, idx);
      }
   }
}
void HcalNtuple::addSignal(const QIE11DataFrame& frame) {
   HcalDetId detId(frame.id());
   // prevent QIE11 calibration channels from entering TP emulation
   //if(detId.subdet() != HcalEndcap && detId.subdet() != HcalBarrel) return;

   std::vector<HcalTrigTowerDetId> ids = fTrigTowerGeometry->towerIds(detId);
   assert(ids.size() == 1 || ids.size() == 2);
   IntegerCaloSamples samples1(ids[0], int(frame.samples()));

   samples1.setPresamples(frame.presamples());
   fIncoder->adc2Linear(frame, samples1);

   std::vector<std::bitset<2>> msb(frame.samples(), 0);
   fIncoder->lookupMSB(frame, msb);
   
   if(ids.size() == 2) {
      // make a second trigprim for the other one, and share the energy
      IntegerCaloSamples samples2(ids[1], samples1.size());
      for(int i = 0; i < samples1.size(); ++i) {
         samples1[i] = uint32_t(samples1[i]);
         samples2[i] = samples1[i];
      }
      samples2.setPresamples(frame.presamples());
      addSignal(samples2);
      addUpgradeFG(ids[1], detId.depth(), msb);
   }
   addSignal(samples1);
   addUpgradeFG(ids[0], detId.depth(), msb);
}
void HcalNtuple::addSignal(const IntegerCaloSamples & samples) {
   HcalTrigTowerDetId id(samples.id());
   SumMap::iterator itr = fSumMap.find(id);
   if(itr == fSumMap.end()) {
      fSumMap.insert(std::make_pair(id, samples));
   }
   else {
      // wish CaloSamples had a +=
      for(int i = 0; i < samples.size(); ++i) {
         (itr->second)[i] += samples[i];
      }
   }
}
void HcalNtuple::addUpgradeFG(const HcalTrigTowerDetId& id, int depth, const std::vector<std::bitset<2>>& bits) {
  if (!validUpgradeFG(id, depth)) {
    //std::cout << "===> !!!" << " -- " << depth << " -- " << id.ieta() << " -- " << id.iphi() << std::endl;
     return;
   }
   auto it = fFGUpgradeMap.find(id);
   if (it == fFGUpgradeMap.end()) {
      FGUpgradeContainer element;
      element.resize(bits.size());
      it = fFGUpgradeMap.insert(std::make_pair(id, element)).first;
   }
   for (unsigned int i = 0; i < bits.size(); ++i) {
      it->second[i][0][depth] = bits[i][0];
      it->second[i][1][depth] = bits[i][1];
   }
}
bool HcalNtuple::validUpgradeFG(const HcalTrigTowerDetId& id, int depth) const {
   if (depth > LAST_FINEGRAIN_DEPTH) return false;
   if (id.ietaAbs() > LAST_FINEGRAIN_TOWER) return false;
   return true;
}

bool HcalNtuple::validChannel(const QIE10DataFrame& digi, int ts) const {
  HcalDetId lDetId(digi.detid());
  if(digi.linkError() || ts>=digi.samples() || !digi[ts].ok()) return false;
  auto mask = fConditions->getHcalTPChannelParameter(HcalDetId(digi.id()))->getMask();
  if (mask) return false;
  
  auto parameters = fConditions->getHcalTPParameters();
  auto adc_threshold = parameters->getADCThresholdHF();
  auto tdc_mask = parameters->getTDCMaskHF();
  if (digi[ts].adc() < adc_threshold) return true;
   return (1ul << digi[ts].le_tdc()) & tdc_mask;
}
