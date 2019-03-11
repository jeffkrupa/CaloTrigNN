#ifndef ANALYZER_HH
void histogrammer( TFile &f, TTree &T, double min_ieta, double max_ieta, double min_pt, double max_pt);
void plotter(TH1F * pu, TH1F * pv, TH1F * pu_e, TH1F * pv_e, double min_ieta, double max_ieta, double min_pt, double max_pt);
#endif
