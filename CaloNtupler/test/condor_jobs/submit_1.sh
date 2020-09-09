#!/bin/bash

#rm log/*
#rm output/*
#rm error/*

PU=$1
x=$2
#source tar.sh
tar --exclude-caches-all --exclude-vcs -zcf ../../../../../../CMSSW_11_1_0_pre8.tgz -C ../../../../../../CMSSW_11_1_0_pre8/.. CMSSW_11_1_0_pre8 --exclude=tmp --exclude=*root
eos root://cmseos.fnal.gov mkdir /store/user/jkrupa/hcal_gnn/$x
voms-proxy-init -voms cms -valid 192:00 -out ~/x509up
bash submit_2.sh submit_3.sh $PU $x
