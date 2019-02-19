#!/bin/bash

PU=$2
ID=$3
export X509_USER_PROXY=$4
voms-proxy-info -all
voms-proxy-info -all -file $4

cd /afs/cern.ch/work/j/jekrupa/public/pf_studies/CMSSW_10_0_3/src/CaloTrigNN/CaloNtupler/test/
eval `scramv1 runtime -sh`
cd -
echo $PWD

cp /afs/cern.ch/work/j/jekrupa/public/pf_studies/CMSSW_10_0_3/src/CaloTrigNN/CaloNtupler/test/Pion.py . 
cp /afs/cern.ch/work/j/jekrupa/public/pf_studies/CMSSW_10_0_3/src/CaloTrigNN/CaloNtupler/test/step2.py . 
cp /afs/cern.ch/work/j/jekrupa/public/pf_studies/CMSSW_10_0_3/src/CaloTrigNN/CaloNtupler/test/step3.py .


cmsRun Pion.py
cmsRun step2.py
rm step1.root
cmsRun step3.py
rm step2.root

mv Output.root /eos/user/j/jekrupa/pf_studies/${PU}pu/Output_${PU}pu_${ID}_${RANDOM}.root
rm *.py

