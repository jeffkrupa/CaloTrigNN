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
#include "tdrstyle.C"

using namespace std;

void drawTH1(TH1F * fY, TH1F * ch, TH1F * nh, std::string xlabel, std::string ylabel){
    setTDRStyle();
    TCanvas * c = new TCanvas("c","c",800,700);
    auto legend = new TLegend(0.75,0.75,0.9,0.9);
 
    fY->SetLineColor(1);
    ch->SetLineColor(3);
    nh->SetLineColor(5);
    fY->SetLineWidth(2);
    ch->SetLineWidth(2);
    nh->SetLineWidth(2);

    legend->AddEntry(fY,"Gamma");
    legend->AddEntry(ch,"Charged hadron");
    legend->AddEntry(nh,"Neutral hadron");

   
    fY->GetXaxis()->SetTitle("Fraction");
    fY->GetYaxis()->SetTitle("Events");
    fY->GetXaxis()->SetTitleSize(0.06);
    fY->GetYaxis()->SetTitleSize(0.06);

 
    fY->Draw("hist");
    ch->Draw("hist same");
    nh->Draw("hist same");
    legend->Draw();
    c->Update();

    c->SaveAs("plots/match_breakdown.pdf");
    c->SaveAs("plots/match_breakdown.png");
    
}

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

    lOut->Branch("Ngenmatch"    , &fNum          , "Ngenmatch/i");
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

    TH1F * fY  = new TH1F("gamma","gamma",50,0.,1.);
    TH1F * fCH = new TH1F("ch","ch",50,0.,1.);
    TH1F * fNH = new TH1F("nh","nh",50,0.,1.);

    for( int i0 = 0; i0 < lTree->GetEntriesFast(); i0++ ){ 
        lTree->GetEntry(i0);
	vgenE.clear();
	vpt.clear(); 
	vpftype.clear();
	vdepth0.clear(); 
	vdepth1.clear(); 
	vdepth2.clear(); 
	vdepth3.clear(); 
	vdepth4.clear(); 
	vdepth5.clear(); 
	vdepth6.clear();
        if (i0%10000 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;
	std::cout << "evt " << i0 << " of " << lTree->GetEntriesFast() << std::endl;
        //if (i0 == 100) break;
        for( int i1 = 0; i1 < fPFDepths->GetEntriesFast(); i1++ ){
           const baconhep::TPFPart *PF = (baconhep::TPFPart*)((*fPFDepths)[i1]); 
	   std::cout << i1 << std::endl;
	   vpt.push_back(PF->pt);  
 	   veta.push_back(PF->eta);
	   vphi.push_back(PF->phi);
	   vpftype.push_back(PF->pfType);
  	   vdepth0.push_back(PF->depthE[0]);
  	   vdepth1.push_back(PF->depthE[1]);
  	   vdepth2.push_back(PF->depthE[2]);
  	   vdepth3.push_back(PF->depthE[3]);
  	   vdepth4.push_back(PF->depthE[4]);
  	   vdepth5.push_back(PF->depthE[5]);
  	   vdepth6.push_back(PF->depthE[6]);
	   vgenE.push_back(PF->genE);
 	}

	std::vector<int> idx_to_del; 

   	for( int i1 = 0; i1 < fGenParts->GetEntriesFast(); i1++ ){

           const baconhep::TGenParticle *GP = (baconhep::TGenParticle*)((*fGenParts)[i1]); 

	   float pt = 0, eta = 0, phi = 0, depth0 = 0, depth1 = 0, depth2 = 0, depth3 = 0, depth4 = 0, depth5 = 0, depth6 =0;

	   int num = 0;
	   //std::cout << "gen pt/eta/phi " << GP->pt << "/" << GP->eta << "/" << GP->phi << std::endl;
	   for( unsigned i2 = 0; i2 < vpt.size(); i2++){

	      if(reco::deltaR(GP->eta, GP->phi, veta[i2], vphi[i2]) > 0.1) continue;
	      if(vpftype[i2] == 4) continue;
	      if(vgenE[i2] <= 0.) continue;
	      //std::cout << "depths " << vdepth0[i2] << "/" << vdepth1[i2] << "/" << vdepth2[i2] << std::endl;

	      std::cout << "pt/eta/phi/dR " << vpt[i2] << "/" << veta[i2] << "/" << vphi[i2] << "/" << reco::deltaR(GP->eta, GP->phi, veta[i2], vphi[i2])  << std::endl;
	      if(vpftype[i2] == 4) {
		  fY->Fill(vpt[i2] / GP->pt); 
      		  continue;
	      }
	      if(vpftype[i2] == 1){
		  fCH->Fill(vpt[i2] / GP->pt);
	      }
	      if(vpftype[i2] == 5){
		  fNH->Fill(vpt[i2] / GP->pt);
	      }

	      pt     += vpt[i2]; 
	      depth0 += vdepth0[i2];
	      depth1 += vdepth1[i2];
	      depth2 += vdepth2[i2];
	      depth3 += vdepth3[i2];
	      depth4 += vdepth4[i2];
	      depth5 += vdepth5[i2];
	      depth6 += vdepth6[i2];
      	      idx_to_del.push_back(i2); 
	      
              ++num;
	   }

           std::cout << num << std::endl;

	   if (pt < 0.2*GP->pt) continue;
          
	   float sumE = depth0 + depth1 + depth2 + depth3 + depth4 + depth5 + depth6;

	   fPt = pt; fEta = GP->eta; fPhi = GP->phi; 
           if (sumE != 0){ 
	     fdepthFrac0 = depth0 / sumE; 
	     fdepthFrac1 = depth1 / sumE; 
	     fdepthFrac2 = depth2 / sumE; 
	     fdepthFrac3 = depth3 / sumE; 
	     fdepthFrac4 = depth4 / sumE; 
	     fdepthFrac5 = depth5 / sumE; 
	     fdepthFrac6 = depth6 / sumE; 
	     fLV = 1;
	     fNum = num;
	     lOut->Fill();
	   }
	   else{

	     std::cout << "Gen matched depth sum = 0!!" << std::endl;
	   }
	}

        for( int i1 = 0; i1 < fPFDepths->GetEntriesFast(); i1++ ){
	   const baconhep::TGenParticle *GP1 = (baconhep::TGenParticle*)((*fGenParts)[0]);

           const baconhep::TPFPart *PF = (baconhep::TPFPart*)((*fPFDepths)[i1]);
	   if (std::find(idx_to_del.begin(), idx_to_del.end(), i1) != idx_to_del.end()) continue;
	   if (PF->pt < 1.) continue;
	   if (reco::deltaR(PF->eta,PF->phi,GP1->eta,GP1->phi) < 0.2) continue;
	   if (PF->pfType == 4) continue;

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

    drawTH1(fY,fCH,fNH,"pT fraction","Events");
} 

void PF(){

  //loop("/eos/user/j/jekrupa/pf_studies/0pu_era2018/Output_0pu_154_7036.root");
   //loop("/eos/user/j/jekrupa/pf_studies/40pu_era2018/output.root");
   //loop("/eos/user/j/jekrupa/pf_studies/newMinBiaspu_gen0_dR2_Mar26.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/newMinBiaspu_gen0_dR2/oldfn/Output.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/newMinBiaspu_gen0_dR2/oldfn/Apr2.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/Output_40pu_1_30965.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/t3_0pu/Output_all_0pu.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/t3/Output_40pu_17_21055.root");
   loop("Output_old.root");
   //loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/t3/Output_all_40pu.root");
}
