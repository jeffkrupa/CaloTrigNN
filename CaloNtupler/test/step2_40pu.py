# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:phase1_2018_realistic -s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2018 --datatier GEN-SIM-DIGI-RAW -n 10 --geometry DB:Extended --era Run2_2018 --eventcontent FEVTDEBUGHLT --filein file:step1.root --fileout file:step2.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

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
process.mix.input.fileNames = cms.untracked.vstring([
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/6810F3EA-6D47-E811-8D64-FA163EB2D127.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/6810F3EA-6D47-E811-8D64-FA163EB2D127.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/18E0906B-A747-E811-A8F3-FA163EE42437.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/302848D5-6C47-E811-8DBB-FA163EE2C022.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/E0BF52F7-7F47-E811-AB60-FA163EBDA738.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/B87E1C5F-A347-E811-860A-02163E01A071.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/B04315B3-A347-E811-BD50-FA163EC782F0.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/94A78D99-A747-E811-8E6F-FA163E9D9A3E.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/1C285B13-8A47-E811-87BA-FA163EF92103.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/B2E780F7-7F47-E811-9F94-FA163E2919CB.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/74968200-D747-E811-9F24-FA163EB33E04.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/7E9950EB-DE47-E811-853C-FA163E258573.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/CCD95203-D747-E811-B91A-FA163E22D151.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/9248E872-A747-E811-A377-FA163E07A8A8.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/10F9B78A-F547-E811-8ECF-FA163E90D84E.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/46454B84-0948-E811-8F0D-FA163E3F07C8.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/2E52C18C-4A48-E811-BB28-FA163E95395D.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/32B63289-5648-E811-B27E-FA163EC05D24.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/D20CA7D7-6048-E811-91A4-FA163EAC15A1.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/A49799DA-F948-E811-8B3C-FA163E6C5A08.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/A0BF0E43-3449-E811-9C12-FA163E19E5C2.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/DCBA0E43-3449-E811-9C1C-FA163E19E5C2.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/3253A139-3549-E811-8666-FA163E63A2A3.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/44124B5A-3049-E811-B642-FA163E281402.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/44124B5A-3049-E811-AE9B-FA163E281402.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/C63CDD8B-EC49-E811-BCDF-FA163E8C2851.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/A0EAB1A5-7347-E811-8B59-FA163EE29F3E.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/CE41EBEE-7047-E811-9E8D-FA163E088735.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/2EA0920D-7447-E811-BD95-FA163EE70A79.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/18473FF9-F947-E811-84C2-FA163E6659B9.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/1CF9DC24-D547-E811-9F47-FA163ECA540F.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/9C2137F6-F647-E811-A2EE-FA163E29CDFE.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/7E1EAEEC-1D48-E811-8C38-FA163EA71BD5.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/04312B0B-EF47-E811-A51C-FA163E8FE329.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/D8423100-4048-E811-84AC-FA163EE42437.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/BEE67D90-5648-E811-93FA-FA163EC3055A.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/9EC71795-5648-E811-9165-FA163E6E06A7.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/6C76E095-5648-E811-A817-FA163E895918.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/DC10A8D7-6048-E811-822E-FA163EAC15A1.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/32BAC90C-6048-E811-9ACB-FA163E5B0035.root',
'file:/eos/cms/store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/44A69393-EC49-E811-9620-FA163ED42175.root'

])
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
