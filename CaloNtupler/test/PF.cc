#include "PF.hh"
#include "TFile.h"
#include <cmath>
#include <iostream> 
#include "TSystem.h"
//#include <vector> 
#include <sstream> 
#include "CaloTrigNN/DataFormats/interface/TPFPart.hh"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include <string>

using namespace std;
void loop(std::string lName){

    TFile *lFile = new TFile(lName.c_str(), "READ");
    TTree *lTree = (TTree*) lFile->Get("Events");
 
    std::string fOut = lName + "skimmed.root";
    TFile *lFile1 = TFile::Open(fOut.c_str(),"RECREATE");
    TTree *lOut  = new TTree("Events","Events");

    TClonesArray *fPFDepths= new TClonesArray("baconhep::TPFPart");
    lTree->SetBranchAddress("PFDepth", &fPFDepths);

    lOut->Branch("genE"         , &fGenE         , "genE/f");
    lOut->Branch("LV"           , &fLV           , "LV/i");
    lOut->Branch("pt"		, &fPt		 , "pt/f");
    lOut->Branch("energy"	, &fEnergy	 , "energy/f");
    lOut->Branch("ecalE"        , &fEcalE        , "ecalE/f");
    lOut->Branch("eta"		, &fEta      	 , "eta/f");
    lOut->Branch("phi"       	, &fPhi      	 , "phi/f");
    lOut->Branch("depthFrac0"   , &fdepthFrac0   , "depthFrac0/f");
    lOut->Branch("depthFrac1"   , &fdepthFrac1   , "depthFrac1/f");
    lOut->Branch("depthFrac2"   , &fdepthFrac2   , "depthFrac2/f");
    lOut->Branch("depthFrac3"   , &fdepthFrac3   , "depthFrac3/f");
    lOut->Branch("depthFrac4"   , &fdepthFrac4   , "depthFrac4/f");
    lOut->Branch("depthFrac5"   , &fdepthFrac5   , "depthFrac5/f");
    lOut->Branch("depthFrac6"   , &fdepthFrac6   , "depthFrac6/f");
    int lCount = 1;
    int l0Count = 0;
    int lEvent = 0;
    for( int i0 = 0; i0 < lTree->GetEntriesFast(); i0++ ){ 
        lTree->GetEntry(i0);
        if (i0%10000 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;
        for( int i1 = 0; i1 < fPFDepths->GetEntriesFast(); i1++ ){
           const baconhep::TPFPart *PF = (baconhep::TPFPart*)((*fPFDepths)[i1]); 
           if(fabs(PF->eta) < 1.7) continue;
	   if(fabs(PF->eta) > 3.0) continue;
           if(PF->pt < 1.) continue;
           //if(RecHit->genE == 0 && l0Count / 50 != 0) continue;
           //fEvent         = lEvent;
	   float tmpgenE = 0.;
	   for (int i = 0; i < 7; i++){
             tmpgenE  += PF->depthgenE[i];
	   }

	   fLV             = PF->genMatch;
	   fGenE           = tmpgenE;
	   fdepthFrac0     = PF->depthFrac[0];
           fdepthFrac1     = PF->depthFrac[1];
           fdepthFrac2     = PF->depthFrac[2];
           fdepthFrac3     = PF->depthFrac[3];
           fdepthFrac4     = PF->depthFrac[4];
           fdepthFrac5     = PF->depthFrac[5];
           fdepthFrac6     = PF->depthFrac[6];

	   if(PF->ecalE <= 0) { fEcalE = 0.;}
	   else{ fEcalE   = PF->ecalE / (PF->ecalE + PF->hcalE);}
           fPt 		  = PF->pt;
           fEnergy 	  = PF->e;
           fEta           = PF->eta;
           fPhi           = PF->phi;
           lOut->Fill();
           lCount++;
           if(fGenE == 0) l0Count++;
        }   
    lEvent++;
    }
    lFile1->cd();
    lOut->Write();
    lFile->Close();

} 

void PF(){

  //loop("/eos/user/j/jekrupa/pf_studies/0pu_era2018/Output_0pu_154_7036.root");
   //loop("/eos/user/j/jekrupa/pf_studies/40pu_era2018/output.root");
   //loop("/eos/user/j/jekrupa/pf_studies/newMinBiaspu_gen0_dR2_Mar26.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/newMinBiaspu_gen0_dR2/oldfn/Output.root");
   loop("/eos/uscms/store/user/jkrupa/pf_studies/newMinBiaspu_gen0_dR2/oldfn/Apr2.root");
}
