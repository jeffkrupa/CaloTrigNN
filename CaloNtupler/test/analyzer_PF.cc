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
void plotter(TH1F * pu, TH1F * pv, TH1F * count, double min_eta, double max_eta, double min_pt, double max_pt)
{


    setTDRStyle();

    std::ostringstream min_eta1; std::ostringstream max_eta1; std::ostringstream min_pt1; std::ostringstream max_pt1;
    min_eta1 << min_eta; max_eta1 << max_eta; min_pt1 << min_pt; max_pt1 << max_pt;
    min_eta1 << std::setprecision(1); max_eta1 << std::setprecision(1); min_pt1 << std::setprecision(1); max_pt1 << std::setprecision(1);
    std::ostringstream pu_N; std::ostringstream pv_N;
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
    pu->SetLineWidth(2);
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
    std::string pu_n = "PU " + std::to_string((int)count->GetBinContent(1));
    std::string pv_n = "LV " + std::to_string((int)count->GetBinContent(0));
    legend->AddEntry(pu,pu_n.c_str());
    legend->AddEntry(pv,pv_n.c_str());

    std::cout << pu_n.c_str() << " " << pv_n.c_str() << std::endl;

    //pv->Write("pv0");
    //pv->Write("pv");

    //thFile->Write();
    pv->GetYaxis()->SetRangeUser(0.,std::max(pu->GetMaximum(),pv->GetMaximum())+ 0.05);
    pv->GetYaxis()->SetTitle("PF Energy Fraction/Depth");
    pv->GetYaxis()->SetTitleSize(0.06);
    pv->GetXaxis()->SetTitle("Depth");
    pv->GetXaxis()->SetTitleSize(0.06);
    pv->Draw("hist e");
    pu->Draw("hist same e");
    addInfo->Draw();
    legend->Draw();
    c0->Update();
    c0->SaveAs(namepng.c_str());
    c0->SaveAs(namepdf.c_str());
    //delete c0; delete pu; delete pv; //delete thFile; 

}
void histogrammer( TFile &f, TTree &T, double min_ieta, double max_ieta, double min_pt, double max_pt)
{
    TClonesArray *pf_branch = new TClonesArray("baconhep::TPFPart");
    T.SetBranchAddress("PFDepth",&pf_branch);

    TH1F * pu = new TH1F("pu","pu",7,0.5,7.5);
    TH1F * pv = new TH1F("pv","pv",7,0.5,7.5);


    std::cout << "histogramming for ieta: " << min_ieta << " to " << max_ieta << ", pt: " << min_pt << " to " << max_pt << std::endl;

    //for (int i=1; i < 8; i++){ pu->SetBinContent(i,0.); pv->SetBinContent(i,0.);}
    TH1F * count = new TH1F("count","count",2,0,1);


    TH1F * tmp = new TH1F("tmp","tmp",100,-0.01,1.01);

    for (int i=0; i<7; i++){

       std::string var = "depthFrac" + std::to_string(i) + ">>tmp";
       std::string con_LV = "(pt>"+std::to_string(min_pt)+")&& (pt <"+std::to_string(max_pt)+")&& (std::abs(eta)>"+std::to_string(min_ieta) + ")&& (std::abs(eta)<"+std::to_string(max_ieta)+")&&(LV)";
       std::string con_PU = "pt>"+std::to_string(min_pt)+"&& pt <"+std::to_string(max_pt)+"&& std::abs(eta)>"+std::to_string(min_ieta) + "&& std::abs(eta)<"+std::to_string(max_ieta)+"&&(!LV)";
 
       T.Draw(var.c_str(),con_LV.c_str(),"");
       std::cout << var << " " << tmp->GetMean() << std::endl;
       pv->SetBinContent(i+1, tmp->GetMean());
       pv->SetBinError(i+1, tmp->GetStdDev()/std::pow(tmp->GetEntries(),0.5));
       count->SetBinContent(0, tmp->GetEntries());
       tmp->Reset();
       T.Draw(var.c_str(),con_PU.c_str(),"");
       pu->SetBinContent(i+1, tmp->GetMean());
       pu->SetBinError(i+1, tmp->GetStdDev()/std::pow(tmp->GetEntries(),0.5));
       count->SetBinContent(1, tmp->GetEntries());
       tmp->Reset();
    }

    delete tmp;
    

    std::cout << "done histogramming..." << std::endl;
    pu->Scale(1./pu->Integral());
    pv->Scale(1./pv->Integral());
    pu->Print("all");
    pv->Print("all");
    plotter( pu, pv, count, min_ieta, max_ieta, min_pt, max_pt);


}

void analyzer_PF() {

    std::cout << "Start by opening ROOT file." << std::endl;
    //TFile *f = new TFile("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart.rootskimmed.root","READ");
    TFile *f = new TFile("/eos/uscms/store/user/jkrupa/pf_studies/pion_40pu_minbias_genpart/t3/Output_all_40pu.rootskimmed.root","READ");
    //TFile *f = new TFile("Output_old.rootskimmed.root","READ");
    TTree *T = (TTree*)f->Get("Events");

    TFile &rFile = *f; TTree &rTree = *T;

    double etal[] = {2.0,2.5,2.8,3.0}; double ptl[] = {5.,10.,20.,10000.};

    for (int i0=0; i0 < sizeof(etal)/sizeof(etal[0]) -1; i0++){
     for (int i1=0; i1 < sizeof(ptl)/sizeof(ptl[0]) -1; i1++){
       histogrammer(rFile, rTree, etal[i0],etal[i0+1],ptl[i1],ptl[i1+1]);
     }
    }
    delete T; delete f;
}





