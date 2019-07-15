# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:phase1_2017_realistic --pileup_input das:/RelValMinBias_13/CMSSW_10_0_0_pre2-100X_mc2017_realistic_v1-v1/GEN-SIM -n 10 --era Run2_2017 --eventcontent RECOSIM --runUnscheduled -s RAW2DIGI,L1Reco,RECO,RECOSIM --datatier GEN-SIM-RECO --pileup AVE_35_BX_25ns --geometry DB:Extended --filein file:step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
#from PUfiles_gensim import files
#from minbias import files
process = cms.Process('RECO',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
#process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.RecoSim_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.hbheprereco.saveInfos = cms.bool(True)
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
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:step2_40pu.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition
process.RAWRECOSIMHLTEventContent.outputCommands.extend(process.SimG4CoreRAW.outputCommands)
process.RAWRECOSIMHLTEventContent.outputCommands.extend(process.SimCalorimetryRAW.outputCommands)
process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step3.root'),
    outputCommands = process.RAWRECOSIMHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(40.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
process.mix.input.fileNames = cms.untracked.vstring(
['root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext5-v1/100000/4068E628-99AC-FC47-8FF3-75F35308588E.root'
'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/572A2058-1028-D944-A641-AFE7FF55451A.root',
'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/8755A4A4-1671-0D47-BB88-C5A05F91777A.root',
'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/E6E45F49-9EF1-2F48-88F1-2042C2E85039.root',
'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/FB8E7030-C586-254E-B912-4C7F64B2DB7E.root',
'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext2-v1/270000/481C70AC-27BE-7741-813A-A204D0D671B8.root']

)
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

process.recoPath = cms.Path(
    process.hbheprereco
    *process.hbheprerecoMahi
    *process.hbheprerecoM2
    *process.hbheprerecoM3
)
process.load('CaloTrigNN.CaloNtupler.hcaltpntupler_cfi')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.recosim_step = cms.Path(process.recosim)
process.ntuple_step = cms.Path(process.simHcalTriggerPrimitiveNtuple)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.recoPath,process.ntuple_step,process.recosim_step,process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

from L1Trigger.Configuration.customiseReEmul import L1TReEmulMCFromRAWSimHcalTP 
#process = L1TReEmulMCFromRAWSimHcalTP(process)

#do not add changes to your config after this point (unless you know what you are doing)
#from FWCore.ParameterSet.Utilities import convertToUnscheduled
#process=convertToUnscheduled(process)


# Customisation from command line

#Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
#from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
#process = customiseLogErrorHarvesterUsingOutputCommands(process)

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
