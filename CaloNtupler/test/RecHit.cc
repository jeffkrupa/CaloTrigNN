#include "RecHit.hh"
#include "TFile.h"
#include <cmath>
#include <iostream> 
#include "TSystem.h"
//#include <vector> 
#include <sstream> 
#include "CaloTrigNN/DataFormats/interface/TRHPart.hh"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include <string>

using namespace std;
void loop(std::string lName, int lEvent){

    //infile
    //std::cout << "Process file name " << lName.c_str() << std::endl;
    TFile *lFile = new TFile(lName.c_str());
    TTree *lTree = (TTree*) lFile->Get("Events");
 
    //outfile
    std::string fOut = lName.erase(0,50);
    fOut = "/eos/user/j/jekrupa/puppi_training/40pu_nosupress_skimmed/" + fOut;
    TFile *lFile1 = TFile::Open(fOut.c_str(),"RECREATE");
    TTree *lOut  = new TTree("Events","Events");

    //rechit object
    TClonesArray *fRecHits = new TClonesArray("baconhep::TRHPart");
    lTree->SetBranchAddress("HcalRecHit", &fRecHits);
    //branches to write
    lOut->Branch("event"        , &fEvent        , "event/i");
    lOut->Branch("genE"         , &fGenE         , "genE/f");
    lOut->Branch("PU"           , &fPU           , "PU/i");
    lOut->Branch("pt"		, &fPt		 , "pt/f");
    lOut->Branch("energy"	, &fEnergy	 , "energy/f");
    lOut->Branch("eraw"     	, &fEraw     	 , "eraw/f");
    lOut->Branch("rho"	    	, &fRho   	 , "rho/f");
    lOut->Branch("ieta"		, &fIeta      	 , "ieta/f");
    lOut->Branch("iphi"       	, &fIphi      	 , "iphi/f");
    lOut->Branch("depth"       	, &fDepth        , "depth/f");
    lOut->Branch("x"      	, &fX      	 , "x/f");
    lOut->Branch("y"       	, &fY      	 , "y/f");
    lOut->Branch("z"            , &fZ            , "z/f");
    lOut->Branch("gain"         , &fGain         , "gain/f");
    lOut->Branch("inPedAvg"     , &fInPedAvg     , "inPedAvg/f");
    lOut->Branch("raw0"         , &fRaw0         , "raw0/f");
    lOut->Branch("raw1"         , &fRaw1         , "raw1/f");
    lOut->Branch("raw2"         , &fRaw2         , "raw2/f");
    lOut->Branch("raw3"         , &fRaw3         , "raw3/f");
    lOut->Branch("raw4"         , &fRaw4         , "raw4/f");
    lOut->Branch("raw5"         , &fRaw5         , "raw5/f");
    lOut->Branch("raw6"         , &fRaw6         , "raw6/f");
    lOut->Branch("raw7"         , &fRaw7         , "raw7/f");
    lOut->Branch("ped0"         , &fPed0         , "ped0/f");
    lOut->Branch("ped1"         , &fPed1         , "ped1/f");
    lOut->Branch("ped2"         , &fPed2         , "ped2/f");
    lOut->Branch("ped3"         , &fPed3         , "ped3/f");
    lOut->Branch("ped4"         , &fPed4         , "ped4/f");
    lOut->Branch("ped5"         , &fPed5         , "ped5/f");
    lOut->Branch("ped6"         , &fPed6         , "ped6/f");
    lOut->Branch("ped7"         , &fPed7         , "ped7/f");
    lOut->Branch("inNoiseADC0"  , &fInNoiseADC0  , "inNoiseADC0/f");
    lOut->Branch("inNoiseADC1"  , &fInNoiseADC1  , "inNoiseADC1/f");
    lOut->Branch("inNoiseADC2"  , &fInNoiseADC2  , "inNoiseADC2/f");
    lOut->Branch("inNoiseADC3"  , &fInNoiseADC3  , "inNoiseADC3/f");
    lOut->Branch("inNoiseADC4"  , &fInNoiseADC4  , "inNoiseADC4/f");
    lOut->Branch("inNoiseADC5"  , &fInNoiseADC5  , "inNoiseADC5/f");
    lOut->Branch("inNoiseADC6"  , &fInNoiseADC6  , "inNoiseADC6/f");
    lOut->Branch("inNoiseADC7"  , &fInNoiseADC7  , "inNoiseADC7/f");
    lOut->Branch("inPedestal0"  , &fInPedestal0  , "inPedestal0/f");
    lOut->Branch("inPedestal1"  , &fInPedestal1  , "inPedestal1/f");
    lOut->Branch("inPedestal2"  , &fInPedestal2  , "inPedestal2/f");
    lOut->Branch("inPedestal3"  , &fInPedestal3  , "inPedestal3/f");
    lOut->Branch("inPedestal4"  , &fInPedestal4  , "inPedestal4/f");
    lOut->Branch("inPedestal5"  , &fInPedestal5  , "inPedestal5/f");
    lOut->Branch("inPedestal6"  , &fInPedestal6  , "inPedestal6/f");
    lOut->Branch("inPedestal7"  , &fInPedestal7  , "inPedestal7/f");
    lOut->Branch("inNoisePhoto0", &fInNoisePhoto0, "inPedestal0/f");
    lOut->Branch("inNoisePhoto1", &fInNoisePhoto1, "inPedestal1/f");
    lOut->Branch("inNoisePhoto2", &fInNoisePhoto2, "inPedestal2/f");
    lOut->Branch("inNoisePhoto3", &fInNoisePhoto3, "inPedestal3/f");
    lOut->Branch("inNoisePhoto4", &fInNoisePhoto4, "inPedestal4/f");
    lOut->Branch("inNoisePhoto5", &fInNoisePhoto5, "inPedestal5/f");
    lOut->Branch("inNoisePhoto6", &fInNoisePhoto6, "inPedestal6/f");
    lOut->Branch("inNoisePhoto7", &fInNoisePhoto7, "inPedestal7/f");
    lOut->Branch("inputTDC0"  	, &fInputTDC0  	 , "inputTDC0/f");
    lOut->Branch("inputTDC1"    , &fInputTDC1    , "inputTDC1/f");
    lOut->Branch("inputTDC2"    , &fInputTDC2    , "inputTDC2/f");
    lOut->Branch("inputTDC3"    , &fInputTDC3    , "inputTDC3/f");
    lOut->Branch("inputTDC4"    , &fInputTDC4    , "inputTDC4/f");
    lOut->Branch("inputTDC5"    , &fInputTDC5    , "inputTDC5/f");
    lOut->Branch("inputTDC6"    , &fInputTDC6    , "inputTDC6/f");
    lOut->Branch("inputTDC7"    , &fInputTDC7    , "inputTDC7/f");

    int lCount = 1;
    int l0Count = 0;
    for( int i0 = 0; i0 < lTree->GetEntriesFast(); i0++ ){ 
        lTree->GetEntry(i0);
        if (i0%100 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;
        for( int i1 = 0; i1 < fRecHits->GetEntriesFast(); i1++ ){
           const baconhep::TRHPart *RecHit = (baconhep::TRHPart*)((*fRecHits)[i1]); 
           if(fabs(RecHit->ieta) < 17) continue;
           //if(RecHit->genE == 0 && l0Count / 50 != 0) continue;
           fEvent         = lEvent;
           fGenE          = RecHit->genE;
           fPU            = 1;
           if(RecHit->genE < 0.1)
               fPU = 0;
           fPt 		  = RecHit->pt;
           fEnergy 	  = RecHit->energy;
           fEraw          = RecHit->eraw;
           fRho           = RecHit->rho;
           fIeta          = RecHit->ieta;
           fIphi          = RecHit->iphi;
           fDepth         = RecHit->depth;
           fX             = RecHit->x;
           fY             = RecHit->y;
           fZ             = RecHit->z;
           fGain          = RecHit->gain;
           fInPedAvg      = RecHit->inPedAvg;
           fRaw0          = RecHit->raw[0];
           fRaw1          = RecHit->raw[1];
           fRaw2          = RecHit->raw[2];
           fRaw3          = RecHit->raw[3];
           fRaw4          = RecHit->raw[4];
           fRaw5          = RecHit->raw[5];
           fRaw6          = RecHit->raw[6];
           fRaw7          = RecHit->raw[7];
	   fPed0          = RecHit->ped[0];
           fPed1          = RecHit->ped[1];
           fPed2          = RecHit->ped[2];
           fPed3          = RecHit->ped[3];
           fPed4          = RecHit->ped[4];
           fPed5          = RecHit->ped[5];
           fPed6          = RecHit->ped[6];
           fPed7          = RecHit->ped[7];
           fInNoiseADC0   = RecHit->inNoiseADC[0];
           fInNoiseADC1   = RecHit->inNoiseADC[1];
           fInNoiseADC2   = RecHit->inNoiseADC[2];
           fInNoiseADC3   = RecHit->inNoiseADC[3];
           fInNoiseADC4   = RecHit->inNoiseADC[4];
           fInNoiseADC5   = RecHit->inNoiseADC[5];
           fInNoiseADC6   = RecHit->inNoiseADC[6];
           fInNoiseADC7   = RecHit->inNoiseADC[7];
	   fInPedestal0   = RecHit->inPedestal[0];
           fInPedestal1   = RecHit->inPedestal[1];
           fInPedestal2   = RecHit->inPedestal[2];
           fInPedestal3   = RecHit->inPedestal[3];
           fInPedestal4   = RecHit->inPedestal[4];
           fInPedestal5   = RecHit->inPedestal[5];
           fInPedestal6   = RecHit->inPedestal[6];
           fInPedestal7   = RecHit->inPedestal[7];
	   fInNoisePhoto0 = RecHit->inNoisePhoto[0];
           fInNoisePhoto1 = RecHit->inNoisePhoto[1];
           fInNoisePhoto2 = RecHit->inNoisePhoto[2];
           fInNoisePhoto3 = RecHit->inNoisePhoto[3];
           fInNoisePhoto4 = RecHit->inNoisePhoto[4];
           fInNoisePhoto5 = RecHit->inNoisePhoto[5];
           fInNoisePhoto6 = RecHit->inNoisePhoto[6];
           fInNoisePhoto7 = RecHit->inNoisePhoto[7];
           fInputTDC0     = RecHit->inputTDC[0];
           fInputTDC1     = RecHit->inputTDC[1];
           fInputTDC2     = RecHit->inputTDC[2];
           fInputTDC3     = RecHit->inputTDC[3];
           fInputTDC4     = RecHit->inputTDC[4];
           fInputTDC5     = RecHit->inputTDC[5];
           fInputTDC6     = RecHit->inputTDC[6];
           fInputTDC7     = RecHit->inputTDC[7];

           lOut->Fill();
           lCount++;
           if(fGenE == 0) l0Count++;
        }   
    lEvent++;
    }
    lFile1->cd();
    lOut->Write();
    lFile->Close();
    //delete lFile;  delete lFile1; delete lOut;

} 

void RecHit(){

  //loop over directory
  const char* ext = ".root";
  const char* inDir = "/eos/user/j/jekrupa/puppi_training/40pu_nosupress/";

  char* dir = gSystem->ExpandPathName(inDir);
  void* dirp = gSystem->OpenDirectory(dir);

  const char* entry;
  const char* filename[300];
  Int_t n = 0;
  Int_t count = 0;
  TString str;

  while((entry = (char*)gSystem->GetDirEntry(dirp))) {
    str = entry;
    if(str.EndsWith(ext)){
      filename[n++] = gSystem->ConcatFileName(dir, entry);
      count++;
    }
  }
 
  for (Int_t i = 0; i < count; i++){   
    std::cout << "Processing file " << i << " of " << count << std::endl;
    int evt = 500*i;
    loop(filename[i],evt);
   }
}
