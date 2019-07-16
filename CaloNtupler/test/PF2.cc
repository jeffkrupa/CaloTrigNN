#include "PF2.hh"
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

	const baconhep::TGenParticle *GP = (baconhep::TGenParticle*)((*fGenParts)[0]);


	//std::cout << GP->eta << "/" << GP->phi << "/" << GP->pt << std::endl;
        if (i0%10000 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;
        for( int i1 = 0; i1 < fPFDepths->GetEntriesFast(); i1++ ){

           const baconhep::TPFPart *PF = (baconhep::TPFPart*)((*fPFDepths)[i1]);

	   if(std::abs(PF->eta) > 3.) continue;
 	   if(std::abs(PF->eta) < 1.7) continue;
	   //if(PF->genE > 0 && (PF->pt > 0.2 * GP->pt) && reco::deltaR(PF->eta, PF->phi, GP->eta, GP->phi) < 0.1){

	   float dR = reco::deltaR(PF->eta, PF->phi, GP->eta, GP->phi) ;
	   if((PF->genE > 0) && (dR < 0.1)){

	     if(GP->pt > 5*PF->pt) continue;
	     if(PF->pfType == 4) continue;
	     fPt = PF->pt; 
	     fEta = PF->eta; 
	     fPhi = PF->phi; 
	     fdepthFrac0 = PF->depthFrac[0]; 
	     fdepthFrac1 = PF->depthFrac[1]; 
	     fdepthFrac2 = PF->depthFrac[2]; 
	     fdepthFrac3 = PF->depthFrac[3]; 
	     fdepthFrac4 = PF->depthFrac[4]; 
	     fdepthFrac5 = PF->depthFrac[5]; 
	     fdepthFrac6 = PF->depthFrac[6]; 
	     fLV = 1;
	     lOut->Fill();
	   }
	   
	   else if (reco::deltaR(PF->eta, PF->phi, GP->eta, GP->phi) > 0.2) {

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
	   //else{ 
	   //   std::cout << "what to do??" << std::endl;
	   //}
 	}




    }
    lFile1->cd();
    lOut->Write();
    lFile->Close();

    drawTH1(fY,fCH,fNH,"pT fraction","Events");
} 

void PF2(){

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
