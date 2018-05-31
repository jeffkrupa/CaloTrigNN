# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1Ntuple -s RAW2DIGI --era=Run2_2018 --mc --python_filename=mc.py --no_output -n 202 --conditions=100X_upgrade2018_realistic_v11 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAWSimHcalTP --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMUGEN_MC --customise=L1Trigger/Configuration/customiseSettings.L1TSettingsToCaloParams_2018_v1_1_inconsistent --custom_conditions=HcalChannelQuality_2018_v3.0_mc,HcalChannelQualityRcd,frontier://FrontierProd/CMS_CONDITIONS --filein=/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/100001/1A22F20B-8321-E811-AABF-1866DAEA6C40.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

process.hbheprereco.saveInfos = cms.bool(False)
process.hbheprerecoMahi = process.hbheprereco.clone()
process.hbheprerecoMahi.algorithm.__setattr__('useMahi',cms.bool(True))
process.hbheprerecoMahi.algorithm.__setattr__('useM2',cms.bool(False))
process.hbheprerecoMahi.algorithm.__setattr__('useM3',cms.bool(False))
process.hbheprerecoMahi.algorithm.__setattr__('applyPedConstraint',cms.bool(False))
process.hbheprerecoMahi.algorithm.__setattr__('applyTimeConstraint',cms.bool(False))

process.hbheprerecoM2 = process.hbheprereco.clone()
process.hbheprerecoM2.algorithm.__setattr__('useMahi',cms.bool(False))
process.hbheprerecoM2.algorithm.__setattr__('useM2',cms.bool(True))
process.hbheprerecoM2.algorithm.__setattr__('useM3',cms.bool(False))

process.hbheprerecoM3 = process.hbheprereco.clone()
process.hbheprerecoM3.algorithm.__setattr__('useMahi',cms.bool(False))
process.hbheprerecoM3.algorithm.__setattr__('useM2',cms.bool(False))
process.hbheprerecoM3.algorithm.__setattr__('useM3',cms.bool(True))

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:step2.root'),
                            #fileNames = cms.untracked.vstring('/store/mc/RunIISpring18DR/ZToEE_NNPDF30_13TeV-powheg_M_2300_3500/GEN-SIM-RAW/NZSPU40to70_100X_upgrade2018_realistic_v10-v2/30000/48381B53-4221-E811-B3F3-02163E019EFF.root'),
                            #/store/relval/CMSSW_10_0_0/RelValTTbar_13/GEN-SIM-DIGI-RAW//PU25ns_100X_upgrade2018_realistic_v6_muVal_resub-v1/10000/46DD663C-C106-E811-B77D-0025905B85DE.root'),
                            #'/store/mc/RunIISpring18DR/ZToEE_NNPDF30_13TeV-powheg_M_2300_3500/GEN-SIM-RAW/NZSPU40to70_100X_upgrade2018_realistic_v10-v2/30000/48381B53-4221-E811-B3F3-02163E019EFF.root'),
                            #fileNames = cms.untracked.vstring('/store/mc/RunIISpring18DR/QCD_Pt-15to3000_TuneCP5_Flat_13TeV_pythia8/GEN-SIM-RAW/NZSPU0to70_100X_upgrade2018_realistic_v10-v1/100001/1A22F20B-8321-E811-AABF-1866DAEA6C40.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1Ntuple nevts:202'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '100X_upgrade2018_realistic_v11', 'HcalChannelQuality_2018_v3.0_mc,HcalChannelQualityRcd,frontier://FrontierProd/CMS_CONDITIONS')

# Path and EndPath definitions
process.digiPath = cms.Path(
    process.hcalDigis
)
process.recoPath = cms.Path(
    process.hbheprereco
    *process.hbheprerecoMahi
    *process.hbheprerecoM2
    *process.hbheprerecoM3
)
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("test.root")
)
process.outpath = cms.EndPath(process.out)


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.load('CaloTrigNN.CaloNtupler.hcaltpntupler_cfi')
process.ntuple_step = cms.Path(process.simHcalTriggerPrimitiveNtuple)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.digiPath,process.recoPath,process.ntuple_step,process.endjob_step)
#process.schedule = cms.Schedule(process.raw2digi_step,process.simHcalTriggerPrimitiveNtuple,process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulMCFromRAWSimHcalTP 

#call to customisation function L1TReEmulMCFromRAWSimHcalTP imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulMCFromRAWSimHcalTP(process)


# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
#from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleRAWEMUGEN_MC 

#call to customisation function L1NtupleRAWEMUGEN_MC imported from L1Trigger.L1TNtuples.customiseL1Ntuple
#process = L1NtupleRAWEMUGEN_MC(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseSettings
#from L1Trigger.Configuration.customiseSettings import L1TSettingsToCaloParams_2018_v1_1_inconsistent 

#call to customisation function L1TSettingsToCaloParams_2018_v1_1_inconsistent imported from L1Trigger.Configuration.customiseSettings
#process = L1TSettingsToCaloParams_2018_v1_1_inconsistent(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
