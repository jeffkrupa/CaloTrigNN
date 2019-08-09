#include "RecHit.hh"
#include "TFile.h"
#include <cmath>
#include <iostream> 
#include "TSystem.h"
//#include <vector> 
#include <sstream> 
#include "CaloTrigNN/DataFormats/interface/TRHPart.hh"
#include "CaloTrigNN/DataFormats/interface/THcalDep.hh"
#include "CaloTrigNN/DataFormats/interface/TEventInfo.hh"
#include <string>

#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif


//simple baconanalyzer for facile calo regression studies
using namespace std;
void loop(std::string lName){

    //infile
    //std::cout << "Process file name " << lName.c_str() << std::endl;
    TFile *lFile = new TFile(lName.c_str());
    TTree *lTree = (TTree*) lFile->Get("Events");
 
    //outfile
    std::string fOut = lName + "_skimmedRH.root";
    TFile *lFile1 = TFile::Open(fOut.c_str(),"RECREATE");
    TTree *lOut  = new TTree("Events","Events");

    //rechit object
    TClonesArray *fRecHits = new TClonesArray("baconhep::TRHPart");
    TClonesArray *fEvtInfo = new TClonesArray("baconhep::TEventInfo");
    lTree->SetBranchAddress("HcalRecHit", &fRecHits);
    lTree->SetBranchAddress("EventInfo",  &fEvtInfo);
    //branches to write
    lOut->Branch("event"        , &fEvent        , "event/i");
    lOut->Branch("genE"         , &fGenE         , "genE/f");
    lOut->Branch("em3"          , &fEm3          , "em3/f");
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


    lOut->Branch("raw"         , &fRaw         );
    lOut->Branch("ped"         , &fPed         );
    lOut->Branch("inNoiseADC"  , &fInNoiseADC  );
    lOut->Branch("inPedestal"  , &fInPedestal  );
    lOut->Branch("inNoisePhoto", &fInNoisePhoto);
    lOut->Branch("inputTDC"  	, &fInputTDC   );


    int lCount = 1;
    int l0Count = 0;
    for( int i0 = 0; i0 < lTree->GetEntriesFast(); i0++ ){ 

        if(i0>100000) break; 
        lTree->GetEntry(i0);
        if (i0%100 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;

        const baconhep::TEventInfo *evtInfo = (baconhep::TEventInfo*)((*fEvtInfo)[0]);

        for( int i1 = 0; i1 < fRecHits->GetEntriesFast(); i1++ ){
           fRaw.clear(); fPed.clear(); fInNoiseADC.clear(); fInPedestal.clear(); fInNoisePhoto.clear(); fInputTDC.clear();
           const baconhep::TRHPart *RecHit = (baconhep::TRHPart*)((*fRecHits)[i1]); 
           if(fabs(RecHit->ieta) < 17 || fabs(RecHit->ieta) > 30) continue;
           if(RecHit->genE < 1 && l0Count / 500 != 0) continue;

           //fEvent         = lEvent;
           fGenE          = RecHit->genE;
           fEm3           = RecHit->em3;
           fPU            = evtInfo->nPU;
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
           
           for(int i2 = 0; i2 < RecHit->raw.size(); i2++){
	     fRaw.push_back(RecHit->raw[i2]);
	     fPed.push_back(RecHit->ped[i2]);
	     fInNoiseADC.push_back(RecHit->inNoiseADC[i2]);
	     fInPedestal.push_back(RecHit->inPedestal[i2]);
	     fInNoisePhoto.push_back(RecHit->inNoisePhoto[i2]);
	     fInputTDC.push_back(RecHit->inputTDC[i2]);
	   }
           lOut->Fill();
           lCount++;
           if(fGenE == 0) l0Count++;
        }   
    }
    lFile1->cd();
    lOut->Write();
    lFile->Close();

} 

void RecHit(){

  //loop over directory

  loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_40puMinBias_13TeV_2018_PFRHGPE/Out.root");

}
