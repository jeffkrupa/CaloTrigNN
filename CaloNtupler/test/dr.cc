#include <iostream>
#include "TROOT.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TObject.h"
#include "THStack.h"
#include "TClonesArray.h"
#include <string.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <sstream>
#include "TSystem.h"
#include "TH2.h"
#include <TStyle.h>
#include <array>
#include <iterator>
#include <fstream>
#include <iostream>
#include <vector>
#include "CaloTrigNN/DataFormats/interface/TRHPart.hh"
#include "CaloTrigNN/DataFormats/interface/TPFPart.hh"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include "CaloTrigNN/DataFormats/interface/TGenParticle.hh"
#include "tdrstyle.C"
#include <iomanip>
#include "TLatex.h"
#include "TPaveText.h"
#include "DataFormats/Math/interface/deltaR.h"
using namespace std;

void histogrammer( TFile &f, TTree &T){

    TClonesArray *pf_branch = new TClonesArray("baconhep::TPFPart");
    TClonesArray *gp_branch = new TClonesArray("baconhep::TGenParticle");
    T.SetBranchAddress("PFDepth",&pf_branch);
    T.SetBranchAddress("GenParticle",&gp_branch);

    TH1F * dr = new TH1F("pu","pu",100,0.,1.);

    for(int i0 = 0; i0 < T.GetEntriesFast(); i0++){

	T.GetEntry(i0);
	baconhep::TGenParticle * gp0 = (baconhep::TGenParticle*)((*gp_branch)[0]);
        baconhep::TGenParticle * gp1 = (baconhep::TGenParticle*)((*gp_branch)[1]); 

        for (int i1 = 0; i1 < pf_branch->GetEntriesFast(); i1++) {
	   
 	    baconhep::TPFPart * pfhit = (baconhep::TPFPart*)((*pf_branch)[i1]);

	    if( pfhit->eta < -1.7)
	    //if( pfhit->eta < -1.7 && pfhit->pfType != 5 && pfhit->pfType != 4)
		dr->Fill(reco::deltaR(gp1->eta, gp1->phi, pfhit->eta, pfhit->phi),pfhit->pt);
	    else if( pfhit->eta > 1.7)
	    //else if( pfhit->eta > 1.7 && pfhit->pfType != 5 && pfhit->pfType != 4)
		dr->Fill(reco::deltaR(pfhit->eta, pfhit->phi, gp0->eta, gp0->phi),pfhit->pt);
	    else continue; 

	}
    }

    setTDRStyle();
    TCanvas * c= new TCanvas("c","c",800,700);
    dr->GetXaxis()->SetTitle("dR(PFcand, #pi^{#pm})");
    dr->GetXaxis()->SetTitleSize(0.06);
    dr->GetXaxis()->SetRangeUser(0.001,1.);
    dr->GetYaxis()->SetRangeUser(100.,100000.);
    //dr->GetYaxis()->SetTitle("Ent");
    dr->GetYaxis()->SetTitleSize(0.06);
    dr->SetLineWidth(2);
    dr->Scale(1./dr->Integral());
    dr->Draw("hist");
    c->SetLogy();
    //c->SetLogx();
    c->Update();
    c->SaveAs("plots/dr.pdf");
    c->SaveAs("plots/dr.png");
}

void dr() {

    std::cout << "Start by opening ROOT file." << std::endl;
    TFile *f = new TFile("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/t3_0pu/Output_all_0pu.root","READ");
    //TFile *f = new TFile("Output_old.rootskimmed.root","READ");
    TTree *T = (TTree*)f->Get("Events");

    TFile &rFile = *f; TTree &rTree = *T;

    histogrammer(rFile, rTree);
    delete T; delete f;
}





