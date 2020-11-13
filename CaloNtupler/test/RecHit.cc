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

    //TH1F * temp= new TH1F("tempHE","tempHE",200,0,100);
    TH1F * genE = new TH1F("genE","genE",100,0,10);
    TH1F * mahi = new TH1F("mahi","mahi",100,0,10);
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
    lOut->Branch("event"        , &fEvent        );
    lOut->Branch("genE"         , &fGenE         );
    lOut->Branch("darkCurrent"  , &fDarkCurrent  );
    lOut->Branch("lambda"       , &fLambda       );
    lOut->Branch("em3"          , &fEm3          );
    lOut->Branch("PU"           , &fPU           );
    lOut->Branch("pt"		, &fPt		 );
    lOut->Branch("energy"	, &fEnergy	 );
    lOut->Branch("eraw"     	, &fEraw     	 );
    lOut->Branch("rho"	    	, &fRho   	 );
    lOut->Branch("ieta"		, &fIeta      	 );
    lOut->Branch("iphi"       	, &fIphi      	 );
    lOut->Branch("depth"       	, &fDepth        );
    lOut->Branch("x"      	, &fX      	 );
    lOut->Branch("y"       	, &fY      	 );
    lOut->Branch("z"            , &fZ            );
    lOut->Branch("gain"         , &fGain         );
    lOut->Branch("inPedAvg"     , &fInPedAvg     );


    lOut->Branch("raw"         , &fRaw         );
    //lOut->Branch("ped"         , &fPed         );
    //lOut->Branch("inNoiseADC"  , &fInNoiseADC  );
    //lOut->Branch("inPedestal"  , &fInPedestal  );
    //lOut->Branch("inNoisePhoto", &fInNoisePhoto);
    //lOut->Branch("inputTDC"  	, &fInputTDC   );


    int lCount = 1;
    int l0Count = 0;
    for( int i0 = 0; i0 < lTree->GetEntriesFast(); i0++ ){ 

        
        //if(i0>100000) break; 
        lTree->GetEntry(i0);
        if (i0%1000 == 0) std::cout << i0 << " of " << lTree->GetEntriesFast() << " events processed " << std::endl;

        const baconhep::TEventInfo *evtInfo = (baconhep::TEventInfo*)((*fEvtInfo)[0]);

        for( int i1 = 0; i1 < fRecHits->GetEntriesFast(); i1++ ){


           fRaw.clear(); //fPed.clear(); fInNoiseADC.clear(); fInPedestal.clear(); fInNoisePhoto.clear(); fInputTDC.clear();
           const baconhep::TRHPart *RecHit = (baconhep::TRHPart*)((*fRecHits)[i1]); 

           genE->Fill(RecHit->genE);
           mahi->Fill(RecHit->energy);
           if( //(RecHit->genE < 1 && ( RecHit->genE < 0.5* RecHit->energy
	     //|| RecHit->genE > 1.5* RecHit->energy))
	      ((genE->GetBinContent(genE->FindBin(RecHit->genE)) > 4000))
	      //&& mahi->GetBinContent(mahi->FindBin(RecHit->energy)) > 500
              && RecHit->genE <10){//||  RecHit->genE >10){

		continue;
	   }
 	   //if(RecHit->genE < 0.001) continue;
 	   //if(RecHit->genE < 0.02 && rand() % 50 != 0) continue;
           //if(RecHit->genE < 1  && rand() % 20 != 0) continue;
           //if(RecHit->genE < 2  && rand() % 5 != 0) continue;

           //if(RecHit->genE < 4 && RecHit->genE > 3 && rand() % 2 != 0) continue;
           //if(RecHit->genE < 5 && RecHit->genE > 4 && rand() % 1 != 0) continue;
	   //if(rand()%Nentries > 100000) continue;
           //fEvent         = lEvent;
           fGenE          = (float)RecHit->genE;
           fLambda        = (float)RecHit->lambda;
           fDarkCurrent   = (float)RecHit->darkCurrent;
           fEm3           = (float)RecHit->em3;
           fPU            = evtInfo->nPU;
           fPt 		  = (float)RecHit->pt;
           fEnergy 	  = (float)RecHit->energy;
           fEraw          = (float)RecHit->eraw;
           fRho           = (float)RecHit->rho;
           fIeta          = (int)RecHit->ieta;
           fIphi          = (int)RecHit->iphi;
           fDepth         = (int)RecHit->depth;
           fX             = (float) RecHit->x;
           fY             = (float)RecHit->y;
           fZ             = (float)RecHit->z;
           fGain          = (float)RecHit->gain;
           fInPedAvg      = (float)RecHit->inPedAvg;
           //std::cout << typeid(RecHit->x).name() << typeid(fZ).name() << std::endl; 
           for(int i2 = 0; i2 < RecHit->raw.size(); i2++){
	     fRaw.push_back(RecHit->raw[i2]);
	     //fPed.push_back(RecHit->ped[i2]);
	     //fInNoiseADC.push_back(RecHit->inNoiseADC[i2]);
	     //fInPedestal.push_back(RecHit->inPedestal[i2]);
	     //fInNoisePhoto.push_back(RecHit->inNoisePhoto[i2]);
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
  loop("out_test.root");
  //loop("/eos/uscms//store/user/jkrupa/rh_studies/piongun_111X_mcRun3_2021_realistic_v4-CMSSW_11_1_0_pre8/tot.root");
  //loop("/eos/uscms/store/user/jkrupa/rh_106X_upgrade2021_realistic_v5_RH_ZSgenEgt01_PUFlat55to75Run3OOTPU/total.root");
  //loop("/eos/uscms/store/user/jkrupa/pf_studies/pion_rh_106X_upgrade2021_realistic_v5_RH_ZSgenEgt02/Output.root"); ///eos/uscms/store/user/jkrupa/pf_studies/pion_106X_upgrade2021_realistic_v5_RH_ZS_genEgt1/Output.root");///eos/uscms/store/user/jkrupa/pf_studies/pion_40puMinBias_14TeV_Run3_RH_ZS/Output.root");

}
