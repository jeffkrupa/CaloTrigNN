#include "PF.hh"
#include "TFile.h"
#include <cmath>
#include <iostream> 
#include "TSystem.h"
//#include <vector> 
#include <sstream> 
#include "CaloTrigNN/DataFormats/interface/TGenParticle.hh"
#include "CaloTrigNN/DataFormats/interface/TPFPart.hh"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include <string>
#include "DataFormats/Math/interface/deltaR.h"

using namespace std;
void loop(std::string lName){

    TFile *lFile = new TFile(lName.c_str(), "READ");
    TTree *lTree = (TTree*) lFile->Get("Events");
 
    std::string fOut = lName + "skimmed.root";
    TFile *lFile1 = TFile::Open(fOut.c_str(),"RECREATE");
    TTree *lOut  = new TTree("Events","Events");

    TClonesArray *fPFDepths= new TClonesArray("baconhep::TPFPart");
    TClonesArray *fGenParts= new TClonesArray("baconhep::TGenParticle");

    lTree->SetBranchAddress("PFDepth"    , &fPFDepths);
    lTree->SetBranchAddress("GenParticle", &fGenParts);

    lOut->Branch("LV"           , &fLV           , "LV/i");
    lOut->Branch("pt"		, &fPt		 , "pt/f");
    lOut->Branch("eta"		, &fEta      	 , "eta/f");
    lOut->Branch("phi"       	, &fPhi      	 , "phi/f");
    lOut->Branch("depthFrac0"   , &fdepthFrac0   , "depthFrac0/f");
    lOut->Branch("depthFrac1"   , &fdepthFrac1   , "depthFrac1/f");
    lOut->Branch("depthFrac2"   , &fdepthFrac2   , "depthFrac2/f");
    lOut->Branch("depthFrac3"   , &fdepthFrac3   , "depthFrac3/f");
    lOut->Branch("depthFrac4"   , &fdepthFrac4   , "depthFrac4/f");
    lOut->Branch("depthFrac5"   , &fdepthFrac5   , "depthFrac5/f");
    lOut->Branch("depthFrac6"   , &fdepthFrac6   , "depthFrac6/f");


    for( int i0 = 0; i0 < lTree->GetEntriesFast(); i0++ ){ 
        lTree->GetEntry(i0);
        if (i0%10000 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;


        std::vector<float> vpt; 
        std::vector<float> veta;
        std::vector<float> vphi;
        std::vector<float> vdepth0;
        std::vector<float> vdepth1; 
        std::vector<float> vdepth2;
        std::vector<float> vdepth3;
        std::vector<float> vdepth4;
        std::vector<float> vdepth5;
        std::vector<float> vdepth6;

        for( int i1 = 0; i1 < fPFDepths->GetEntriesFast(); i1++ ){

           const baconhep::TPFPart *PF = (baconhep::TPFPart*)((*fPFDepths)[i1]); 

	   vpt.push_back(PF->pt); 
           veta.push_back(PF->eta);
  	   vphi.push_back(PF->phi);
  	   vdepth0.push_back(PF->depthE[0]);
  	   vdepth1.push_back(PF->depthE[1]);
  	   vdepth2.push_back(PF->depthE[2]);
  	   vdepth3.push_back(PF->depthE[3]);
  	   vdepth4.push_back(PF->depthE[4]);
  	   vdepth5.push_back(PF->depthE[5]);
  	   vdepth6.push_back(PF->depthE[6]);
 	}

	std::vector<int> idx_to_del; 

   	for( int i1 = 0; i1 < fGenParts->GetEntriesFast(); i1++ ){

           const baconhep::TGenParticle *GP = (baconhep::TGenParticle*)((*fGenParts)[i1]); 

	   float pt = 0, eta = 0, phi = 0, depth0 = 0, depth1 = 0, depth2 = 0, depth3 = 0, depth4 = 0, depth5 = 0, depth6 =0;

	   for( unsigned i2 = 0; i2 < vpt.size(); i2++){

	      if(reco::deltaR(GP->eta, GP->phi, veta[i2], vphi[i2]) > 0.15) continue;

              //std::cout << i2 << std::endl;
	      pt     += vpt[i2]; 
 	      eta    += veta[i2];
 	      phi    += vphi[i2];
	      depth0 += vdepth0[i2];
	      depth1 += vdepth1[i2];
	      depth2 += vdepth2[i2];
	      depth3 += vdepth3[i2];
	      depth4 += vdepth4[i2];
	      depth5 += vdepth5[i2];
	      depth6 += vdepth6[i2];
      	      idx_to_del.push_back(i2); 
	   }

	   if (pt < 0.2*GP->pt) continue;
           //if ( fabs( (pt - GP->pt)/ GP->pt) > 0.4 ) continue;   
	   float sumE = depth0 + depth1 + depth2 + depth3 + depth4 + depth5 + depth6;

	   fPt = pt; fEta = GP->eta; fPhi = GP->phi; 
   
	   fdepthFrac0 = depth0 / sumE; 
	   fdepthFrac1 = depth1 / sumE; 
	   fdepthFrac2 = depth2 / sumE; 
	   fdepthFrac3 = depth3 / sumE; 
	   fdepthFrac4 = depth4 / sumE; 
	   fdepthFrac5 = depth5 / sumE; 
	   fdepthFrac6 = depth6 / sumE; 

 	   fLV = 1;

           lOut->Fill();
	}

	vpt.clear(); veta.clear(); vphi.clear();
	vdepth0.clear(); 
	vdepth1.clear(); 
	vdepth2.clear(); 
	vdepth3.clear(); 
	vdepth4.clear(); 
	vdepth5.clear(); 
	vdepth6.clear();

        for( int i1 = 0; i1 < fPFDepths->GetEntriesFast(); i1++ ){

           const baconhep::TPFPart *PF = (baconhep::TPFPart*)((*fPFDepths)[i1]);
	   if (std::find(idx_to_del.begin(), idx_to_del.end(), i1) != idx_to_del.end()) continue;
	   //if (PF->pt < 1.) continue;

           fPt = PF->pt ;
           fEta = PF->eta;
           fPhi = PF->phi;

           fdepthFrac0 = PF->depthFrac[0];
           fdepthFrac1 = PF->depthFrac[1];
           fdepthFrac2 = PF->depthFrac[2];
           fdepthFrac3 = PF->depthFrac[3];
           fdepthFrac4 = PF->depthFrac[4];
           fdepthFrac5 = PF->depthFrac[5];
           fdepthFrac6 = PF->depthFrac[6];

           fLV = 0;
           lOut->Fill();

	}

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
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/newMinBiaspu_gen0_dR2/oldfn/Apr2.root");
   loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/Output_40pu_1_30965.root");
   //loop("Output_old.root");
}
