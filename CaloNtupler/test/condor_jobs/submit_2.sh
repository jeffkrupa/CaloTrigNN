#!/bin/bash

exe=$1
PU=`echo "${2}"`
ID=`echo "${3}"`
echo  "submitting jobs ID ${ID} with PU ${PU} using "$exe""
echo "executable              = "$exe > tmp.sub
echo "arguments               = \$(ClusterId)\$(ProcId)" $PU $ID /afs/cern.ch/user/j/jekrupa/private/x509up>> tmp.sub
echo "output                  = output/${PU}pu.\$(ClusterId).\$(ProcId).out" >> tmp.sub
echo "error                   = error/${PU}pu.\$(ClusterId).\$(ProcId).err"  >> tmp.sub
echo "log                     = log/${PU}pu.\$(ClusterId).\$(ProcId).log"                >> tmp.sub
#echo "RequestCpus             = 4" >> tmp.sub
echo "+JobFlavour = \"workday\"  " >> tmp.sub
echo "queue " >> tmp.sub
condor_submit tmp.sub
