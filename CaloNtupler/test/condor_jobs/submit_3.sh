#!/bin/bash

date 
PU=$2
ID=$3
#export X509_USER_PROXY=$4
CMSSW="CMSSW_10_5_0_pre2"
CMSSW_TGZ="CMSSW_10_5_0_pre2.tgz"

#voms-proxy-info -all
#voms-proxy-info -all -file $4

#cd /uscms_data/d3/jkrupa/pf_studies/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test
#eval `scramv1 runtime -sh`
#cd -
#echo $PWD

tar -xf $CMSSW_TGZ 
rm $CMSSW_TGZ
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc700

scramv1 project CMSSW $CMSSW
cd $CMSSW/src/CaloTrigNN/CaloNtupler/test

echo "HELLO"
echo $PWD
scram b ProjectRename
eval `scramv1 runtime -sh`

echo "CMSSW: "$CMSSW_BASE

cmsRun Pion_${PU}pu.py
cmsRun step2_${PU}pu.py
rm step1_${PU}pu.root
cmsRun step3_${PU}pu.py
rm step2_${PU}pu.root

fName="Output_${PU}pu_${ID}_${RANDOM}.root"
mv Output_old.root $fName
xrdcp $fName root://cmseos.fnal.gov//eos/uscms/store/user/jkrupa/pf_studies/newMinBiaspu_gen0_dR2/oldfn/${PU}pu/
rm $fName 
rm *.py

date
