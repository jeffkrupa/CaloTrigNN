#!/bin/bash

date 
PU=$2
ID=$3
#export X509_USER_PROXY=$4
CMSSW="CMSSW_11_2_0_pre8"
CMSSW_TGZ="CMSSW_11_2_0_pre8.tgz"

#voms-proxy-info -all
#voms-proxy-info -all -file $4

#cd /uscms_data/d3/jkrupa/pf_studies/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test
#eval `scramv1 runtime -sh`
#cd -
#echo $PWD

tar -xf $CMSSW_TGZ 
rm $CMSSW_TGZ
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc820

scramv1 project CMSSW $CMSSW
cd $CMSSW/src/CaloTrigNN/CaloNtupler/test

echo "HELLO"
echo $PWD
scram b ProjectRename
eval `scramv1 runtime -sh`

echo "CMSSW: "$CMSSW_BASE

cmsRun TTbar_14TeV_TuneCP5_cfi_GEN_SIM_PU.py
cmsRun step2_DIGI_L1_DIGI2RAW_HLT_PU.py 
rm step1.root
cmsRun ntuple.py #finalstep.py #step3_RAW2DIGI_L1Reco_RECO_RECOSIM_PU.py
rm step2.root

fName="Output_${PU}pu_${ID}_${RANDOM}.root"
fName3="Output_${PU}pu_${ID}_${RANDOM}_step3.root"

mv Output_old.root $fName3
#cp $fName /data/t3home000/jkrupa/pf_studies/pion_40pu_minbias_genpart/$fName
#xrdcp $fName3 root://cmseos.fnal.gov//eos/uscms/store/user/jkrupa/pf_studies/pion_rh_106X_upgrade2021_realistic_v5_RH_ZSgenEgt02_0pu #eos/uscms/store/user/jkrupa/pf_studies/pion_40puMinBias_14TeV_Run3_RH_ZS_genEgt1
xrdcp $fName3 root://cmseos.fnal.gov//store/user/jkrupa/rh_studies/112X_mcRun3_2021_realistic_v10_PUFlat55to75Run3OOTPU_v2
#cp $fName3 /data/t3home000/jkrupa/rh_out/$fName3
rm $fName 

date
