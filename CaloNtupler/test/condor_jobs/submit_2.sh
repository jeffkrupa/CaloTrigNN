#!/bin/bash

exe=$1
PU=$2
ID=$3
mkdir log${ID}
echo  "submitting jobs ID ${ID} with PU ${PU} using "$exe""
echo "universe                =vanilla" > tmp.sub
echo "executable              = "$exe >> tmp.sub
echo "Should_Transfer_Files   = YES" >> tmp.sub
echo "WhenToTransferOutput    = ON_EXIT_OR_EVICT" >> tmp.sub
echo "+AcctGroup              = group_cmsuser.jkrupa" >> tmp.sub
echo "+AccountingGroup        = group_cmsuser.jkrupa" >> tmp.sub
#echo "requirements            = (HAS_GPU == False)" >> tmp.sub
echo "+HAS_GPU                = False" >> tmp.sub
echo "Transfer_Input_Files    = "$exe, /uscms/home/jkrupa/nobackup/hcal_gnn/CMSSW_11_1_0_pre8.tgz >> tmp.sub
echo "arguments               = \$(ClusterId)\$(ProcId)" $PU $ID /home/jkrupa/x509up >> tmp.sub
echo "output                  = log${ID}/${PU}pu.\$(ClusterId).\$(ProcId).out" >> tmp.sub
echo "error                   = log${ID}/${PU}pu.\$(ClusterId).\$(ProcId).err"  >> tmp.sub
echo "log                     = log${ID}/${PU}pu.\$(ClusterId).\$(ProcId).log"                >> tmp.sub
echo "request_memory          = 4000" >> tmp.sub
echo "x509userproxy           = /uscms/home/jkrupa/x509up" >> tmp.sub
#echo "+JobFlavour = \"longlunch\"  " >> tmp.sub
echo "queue 190" >> tmp.sub
condor_submit tmp.sub

