#!/bin/bash

PU=$2
ID=$3
export X509_USER_PROXY=$4
voms-proxy-info -all
voms-proxy-info -all -file $4

cd /afs/cern.ch/work/j/jekrupa/public/pf_studies/fixgit_4/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/
eval `scramv1 runtime -sh`
cd -
echo $PWD

cp /afs/cern.ch/work/j/jekrupa/public/pf_studies/fixgit_4/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test/Pion_${PU}pu.py . 
cp /afs/cern.ch/work/j/jekrupa/public/pf_studies/fixgit_4/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test/step2_${PU}pu.py . 
cp /afs/cern.ch/work/j/jekrupa/public/pf_studies/fixgit_4/CMSSW_10_5_0_pre2/src/CaloTrigNN/CaloNtupler/test/step3_${PU}pu.py .


cmsRun Pion_${PU}pu.py
cmsRun step2_${PU}pu.py
rm step1_${PU}pu.root
cmsRun step3_${PU}pu.py
rm step2_${PU}pu.root

mv Output_old.root /eos/user/j/jekrupa/pf_studies/newMinBiaspu_gen0_dR2/${PU}pu/Output_${PU}pu_${ID}_${RANDOM}.root
rm *.py

