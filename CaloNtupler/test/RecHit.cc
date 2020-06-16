#include "RecHit.hh"
#include <stdlib.h>     /* srand, rand */
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

    TH1F * tempHE = new TH1F("tempHE","tempHE",500,0,500);
    TH1F * tempHB = new TH1F("tempHB","tempHB",500,0,500);
    //lTree->Draw("HcalRecHit.genE>>temp");
 
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
    lOut->Branch("darkCurrent"  , &fDarkCurrent  , "darkCurrent/f");
    lOut->Branch("lambda"       , &fLambda       , "lambda/f");
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
    //lOut->Branch("inputTDC"  	, &fInputTDC   );


    int lCount = 1;
    int l0Count = 0;
    for( int i0 = 0; i0 < lTree->GetEntriesFast(); i0++ ){ 

        
        //if(i0>200000) break; 
        lTree->GetEntry(i0);
        if (i0%10000 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;

        const baconhep::TEventInfo *evtInfo = (baconhep::TEventInfo*)((*fEvtInfo)[0]);

        for( int i1 = 0; i1 < fRecHits->GetEntriesFast(); i1++ ){


           fRaw.clear(); fPed.clear(); fInNoiseADC.clear(); fInPedestal.clear(); fInNoisePhoto.clear(); fInputTDC.clear();
           const baconhep::TRHPart *RecHit = (baconhep::TRHPart*)((*fRecHits)[i1]); 


           /*if (abs(RecHit->ieta) > 15){
  	   	tempHE->Fill(RecHit->genE);
  		if( tempHE->GetBinContent(tempHE->FindBin(RecHit->genE)) > 10000) continue;
	   	}
           if (abs(RecHit->ieta) <= 15){
		tempHB->Fill(RecHit->genE);
  		if( tempHB->GetBinContent(tempHB->FindBin(RecHit->genE)) > 10000) continue;
	   }*/
           //std::cout << Nentries << "\t\t" <<RecHit->energy << std::endl;
           //if(RecHit->genE < 3. && rand()%2 != 0) continue; 
 	   //if(RecHit->genE < 1. && rand() % 2 != 0) continue;
 	   //if(RecHit->genE < 2 && RecHit->genE > 1 && rand() % 2 != 0) continue;
           //if(RecHit->genE < 3 && RecHit->genE > 2 && rand() % 1 != 0) continue;
           //if(RecHit->genE < 4 && RecHit->genE > 3 && rand() % 2 != 0) continue;
           //if(RecHit->genE < 5 && RecHit->genE > 4 && rand() % 1 != 0) continue;
	   //if(rand()%Nentries > 100000) continue;
           //fEvent         = lEvent;
           fGenE          = RecHit->genE;
           fLambda        = RecHit->lambda;
           fDarkCurrent   = RecHit->darkCurrent;
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
	     //fInputTDC.push_back(RecHit->inputTDC[i2]);
	   }
           lOut->Fill();
        }   
    }
    lFile1->cd();
    lOut->Write();
    lFile->Close();

} 

void RecHit(){

  //loop over directory
  //loop("Pion_noPU_106X_mcRun3_2024_realistic_v4.root"); //Output_0pu_1.root");
  loop("/eos/uscms/store/user/jkrupa/rh_106X_upgrade2021_realistic_v5_RH_ZSgenEgt01_PUFlat55to75Run3OOTPU/total.root");
  //loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_rh_106X_upgrade2021_realistic_v5_RH_ZSgenEgt02/Output.root"); ///eos/uscms/store/user/jkrupa/pf_studies/pion_106X_upgrade2021_realistic_v5_RH_ZS_genEgt1/Output.root");///eos/uscms/store/user/jkrupa/pf_studies/pion_40puMinBias_14TeV_Run3_RH_ZS/Output.root");

}
