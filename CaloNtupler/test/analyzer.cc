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
#include "analyzer.hh"
#include "tdrstyle.C"
#include <iomanip>
#include "TLatex.h"
#include "TPaveText.h"
using namespace std;
void plotter(TH1F * pu, TH1F * pv, TH1F * pu_e, TH1F * pv_e, double min_eta, double max_eta, double min_pt, double max_pt)
{


    setTDRStyle();

    std::ostringstream min_eta1; std::ostringstream max_eta1; std::ostringstream min_pt1; std::ostringstream max_pt1;
    min_eta1 << min_eta; max_eta1 << max_eta; min_pt1 << min_pt; max_pt1 << max_pt;
    min_eta1 << std::setprecision(1); max_eta1 << std::setprecision(1); min_pt1 << std::setprecision(1); max_pt1 << std::setprecision(1);
    std::ostringstream pu_N; std::ostringstream pv_N;
    pu_N << pu_e->GetEntries(); pv_N << pv_e->GetEntries();
    pu_N << std::setprecision(1); pv_N << std::setprecision(1);
    std::string pu_N1 = pu_N.str(); std::string pv_N1 = pv_N.str();

    std::string name = "superimposed_hists/" + min_eta1.str() + "_" + max_eta1.str() + "_" + min_pt1.str() + "_" + max_pt1.str() + "_histograms.root";

    //TFile *thFile = new TFile(name.c_str(),"update");

 
    std::string namepng = "plots/PFprofile" + min_eta1.str() + "_" + max_eta1.str() + "_" + min_pt1.str() + "_" + max_pt1.str() + ".pdf";
    std::string namepdf = "plots/PFprofile" + min_eta1.str() + "_" + max_eta1.str() + "_" + min_pt1.str() + "_" + max_pt1.str() + ".png";

    std::string name_pv = min_eta1.str() + "_" + max_eta1.str() + "_" + min_pt1.str() + "_" + max_pt1.str() + "_pv"; 
    std::string name_pu = min_eta1.str() + "_" + max_eta1.str() + "_" + min_pt1.str() + "_" + max_pt1.str() + "_pu";

    TCanvas * c0 = new TCanvas("c0","c0",800,600);

    pu->SetStats(0); pv->SetStats(0);
    pu->SetLineColor(kRed);
    pu->SetLineWidth(1);
    pv->SetLineColor(kBlue);
    pv->SetLineWidth(2);


    TLatex latex;
    TPaveText * addInfo = new TPaveText(0.62,0.5,0.9,0.7,"NDC");

    std::string ieta_t = min_eta1.str()+" < ieta < "+max_eta1.str() ;
    std::string pt_t   = min_pt1.str() +" < pT < "+max_pt1.str() ;

    addInfo->AddText(ieta_t.c_str());
    addInfo->AddText(pt_t.c_str());
    addInfo->SetTextSize(0.04);
    addInfo->SetBorderSize(0);
    addInfo->SetFillColor(0);
    addInfo->SetLineColor(1);
    addInfo->SetFillStyle(1);

    auto legend = new TLegend(0.7,0.75,0.9,0.9); 
    std::string pu_n = "PU: " + pu_N1;
    std::string pv_n = "LV: " + pv_N1;
    legend->AddEntry(pu,pu_n.c_str());
    legend->AddEntry(pv,pv_n.c_str());

    std::cout << pu_n.c_str() << " " << pv_n.c_str() << std::endl;

    //pv->Write("pv0");
    //pv->Write("pv");

    //thFile->Write();
    pv->GetYaxis()->SetRangeUser(0.,std::max(pu->GetMaximum(),pv->GetMaximum())+ 0.05);
    pv->GetYaxis()->SetTitle("PF Energy Fraction/Depth");
    pv->GetXaxis()->SetTitle("Depth");
    pv->Draw("hist");
    pu->Draw("hist same");
    addInfo->Draw();
    legend->Draw();
    c0->Update();
    c0->SaveAs(namepng.c_str());
    c0->SaveAs(namepdf.c_str());
    delete c0; delete pu; delete pv; //delete thFile; 

}
void histogrammer( TFile &f, TTree &T, double min_ieta, double max_ieta, double min_pt, double max_pt)
{
    TClonesArray *pf_branch = new TClonesArray("baconhep::TPFPart");
    T.SetBranchAddress("PFDepth",&pf_branch);

    TH1F * pu = new TH1F("pu","pu",8,-0.5,7.5);
    TH1F * pv = new TH1F("pv","pv",8,-0.5,7.5);
    TH1F * pu_tmp = new TH1F("pu_tmp","pu_tmp",8,-0.5,7.5);
    TH1F * pv_tmp = new TH1F("pv_tmp","pv_tmp",8,-0.5,7.5);

    TH1F * pu_e = new TH1F("pu_e","pu_e",20,0.,100.);
    TH1F * pv_e = new TH1F("pv_e","pv_e",20,0.,100.);

    std::cout << "histogramming for ieta: " << min_ieta << " to " << max_ieta << ", pt: " << min_pt << " to " << max_pt << std::endl;

    for (int i=1; i < 9; i++){ pu->SetBinContent(i,0.); pv->SetBinContent(i,0.);}

    for (Long64_t i0=0;i0<T.GetEntriesFast();i0++) {
        T.GetEntry(i0);
	double last_sum = 0.;

     	if ( i0 % 1000 == 0){std::cout << "Event " << i0 << " of " << T.GetEntriesFast() <<std::endl;}

	float peta = 0., pphi = 0.;
	float pe = 0.;

        //for (int i1 = 0; i1 < pf_branch->GetEntriesFast(); i1++) {
        //    baconhep::TPFPart * pfhit = (baconhep::TPFPart*)((*pf_branch)[i1]);
	//    if (pfhit->e > pe) {peta = pfhit->eta; pphi = pfhit->phi;}
//	}

        for (int i1 = 0; i1 < 1000; i1++) {

            std::cout << i1 << std::endl;
            baconhep::TPFPart * pfhit = (baconhep::TPFPart*)((*pf_branch)[i1]);

	    
	    float sum    = std::accumulate(pfhit->depthE.begin(), pfhit->depthE.end(), 0.0);
	    float gensum = std::accumulate(pfhit->depthgenE.begin(),pfhit->depthgenE.end(), 0.0);
	    //if (last_sum == sum) continue;
            if (std::abs(pfhit->eta) > max_ieta) continue;
	    if (std::abs(pfhit->eta) < min_ieta) continue;
	    if (pfhit->pt > max_pt) continue; 
	    if (pfhit->pt < min_pt) continue;
            if (sum <= 0) continue; 
	    //if (pfhit->ecalSum / sum > 0.05) continue;


	    float dR = sqrt(pow((pfhit->eta - peta),2.0) + pow((pfhit->phi - pphi),2.0));

	    if( gensum > 1.){//if ( (gensum > 1.)){ // && (dR < 0.8) ) {

	     if (pfhit->ecalE>0){pv_tmp->SetBinContent(1,pfhit->ecalE/(pfhit->ecalE + pfhit->hcalE));}
	     else {pv_tmp->SetBinContent(1,0.);}
	     for (unsigned int i2 = 2; i2 < 9; i2++){
	      pv_tmp->SetBinContent(i2,pfhit->depthFrac[i2-2]);
	     }pv_e->Fill(pfhit->e);            
	    pv_tmp->Scale(1./pv_tmp->Integral()); pv->Add(pv_tmp); pv_tmp->Reset();
     	    }

	    else{
	     if (pfhit->ecalE>0){pu_tmp->SetBinContent(1,pfhit->ecalE/(pfhit->ecalE + pfhit->hcalE));}
             else {pu_tmp->SetBinContent(1,0.);}

	     for (unsigned int i2 = 2; i2 < 9; i2++){
              pu_tmp->SetBinContent(i2,pfhit->depthFrac[i2-2]);
             }pu_e->Fill(pfhit->e);
	    if(pu_tmp->Integral() > 0){pu_tmp->Scale(1./pu_tmp->Integral()); 
	    pu->Add(pu_tmp); pu_tmp->Reset();}
	    }

   	    last_sum = sum;
   
	}
    }
    std::cout << "done histogramming..." << std::endl;
    pu->Scale(1./pu->Integral());
    pv->Scale(1./pv->Integral());
    pu->Print("all");
    pv->Print("all");
    plotter( pu, pv, pu_e, pv_e, min_ieta, max_ieta, min_pt, max_pt);
    delete pu_tmp; delete pv_tmp;
}

void analyzer() {

    std::cout << "Start by opening ROOT file." << std::endl;
    //TFile *f = new TFile("/eos/user/j/jekrupa/pf_studies/0pu_era2018/Output_0pu_146_13137.root","READ");
    //TFile *f = new TFile("Output_old.root","READ");
    TFile *f = new TFile("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/t3/Output_all_40pu.rootskimmed.root","READ");
    //TFile *f = new TFile("/eos/user/j/jekrupa/pf_studies/0pu_era2018/Output.root", "READ");
    //TFile *f   = new TFile("/tmp/jekrupa/output.root","READ");
    //TFile *f = new TFile("Output_old.root","READ");
    TTree *T = (TTree*)f->Get("Events");

    TFile &rFile = *f; TTree &rTree = *T;

    std::cout << "ROOT file open. Begin histogramming." << std::endl;
    double etal[] = {2.0,2.5,2.8,3.0}; double ptl[] = {5.,10.,20.,10000.0};
    //double etal[] = {1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0}; double ptl[]  = {1.0,1000.0};
    for (int i0=0; i0 < sizeof(etal)/sizeof(etal[0]) -1; i0++){
     for (int i1=0; i1 < sizeof(ptl)/sizeof(ptl[0]) -1; i1++){
       histogrammer(rFile, rTree, etal[i0],etal[i0+1],ptl[i1],ptl[i1+1]);
     }
    }
    delete T; delete f;
}





