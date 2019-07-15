# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:phase1_2018_realistic -s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2018 --datatier GEN-SIM-DIGI-RAW -n 10 --geometry DB:Extended --era Run2_2018 --eventcontent FEVTDEBUGHLT --filein file:step1.root --fileout file:step2.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
#from PUfiles_gensim import files
from minbias import files
process = cms.Process('HLT',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
#process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:step1_40pu.root'),
    inputCommands = cms.untracked.vstring(
        'keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition
process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step2_40pu.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.mix.input.nbPileupEvents.averageNumber = cms.double(40.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
process.mix.input.fileNames = cms.untracked.vstring(
files
#['root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext5-v1/100000/4068E628-99AC-FC47-8FF3-75F35308588E.root',
#'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/572A2058-1028-D944-A641-AFE7FF55451A.root',
#'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/8755A4A4-1671-0D47-BB88-C5A05F91777A.root',
#'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/E6E45F49-9EF1-2F48-88F1-2042C2E85039.root',
#'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext1-v2/110000/FB8E7030-C586-254E-B912-4C7F64B2DB7E.root',
#'root://cmsxrootd.fnal.gov://store/mc/RunIIFall18GS/SingleNeutrinoGun/GEN-SIM/102X_upgrade2018_realistic_v11_ext2-v1/270000/481C70AC-27BE-7741-813A-A204D0D671B8.root']

)
#process.mix.digitizers = cms.PSet(process.theDigitizersValid)
#from Configuration.AlCa.GlobalTag import GlobalTag
# Other statements
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2018_realistic', '')

process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
#process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.FEVTDEBUGHLToutput_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

#process.options.numberOfThreads=cms.untracked.uint32(4)
#process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
#from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
#process = customizeHLTforMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
