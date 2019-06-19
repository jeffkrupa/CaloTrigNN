# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:phase1_2017_realistic --pileup_input das:/RelValMinBias_13/CMSSW_10_0_0_pre2-100X_mc2017_realistic_v1-v1/GEN-SIM -n 10 --era Run2_2017 --eventcontent RECOSIM --runUnscheduled -s RAW2DIGI,L1Reco,RECO,RECOSIM --datatier GEN-SIM-RECO --pileup AVE_35_BX_25ns --geometry DB:Extended --filein file:step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

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
process.mix.input.fileNames = cms.untracked.vstring([

'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/6810F3EA-6D47-E811-8D64-FA163EB2D127.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/6810F3EA-6D47-E811-8D64-FA163EB2D127.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/18E0906B-A747-E811-A8F3-FA163EE42437.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/302848D5-6C47-E811-8DBB-FA163EE2C022.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/E0BF52F7-7F47-E811-AB60-FA163EBDA738.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/B87E1C5F-A347-E811-860A-02163E01A071.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/B04315B3-A347-E811-BD50-FA163EC782F0.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/94A78D99-A747-E811-8E6F-FA163E9D9A3E.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/1C285B13-8A47-E811-87BA-FA163EF92103.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/B2E780F7-7F47-E811-9F94-FA163E2919CB.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/74968200-D747-E811-9F24-FA163EB33E04.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/7E9950EB-DE47-E811-853C-FA163E258573.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/CCD95203-D747-E811-B91A-FA163E22D151.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/9248E872-A747-E811-A377-FA163E07A8A8.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/10F9B78A-F547-E811-8ECF-FA163E90D84E.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/46454B84-0948-E811-8F0D-FA163E3F07C8.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/2E52C18C-4A48-E811-BB28-FA163E95395D.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/32B63289-5648-E811-B27E-FA163EC05D24.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/D20CA7D7-6048-E811-91A4-FA163EAC15A1.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/A49799DA-F948-E811-8B3C-FA163E6C5A08.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/A0BF0E43-3449-E811-9C12-FA163E19E5C2.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/DCBA0E43-3449-E811-9C1C-FA163E19E5C2.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/3253A139-3549-E811-8666-FA163E63A2A3.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/44124B5A-3049-E811-B642-FA163E281402.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/44124B5A-3049-E811-AE9B-FA163E281402.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10000/C63CDD8B-EC49-E811-BCDF-FA163E8C2851.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/A0EAB1A5-7347-E811-8B59-FA163EE29F3E.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/CE41EBEE-7047-E811-9E8D-FA163E088735.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/2EA0920D-7447-E811-BD95-FA163EE70A79.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/18473FF9-F947-E811-84C2-FA163E6659B9.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/1CF9DC24-D547-E811-9F47-FA163ECA540F.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/9C2137F6-F647-E811-A2EE-FA163E29CDFE.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/7E1EAEEC-1D48-E811-8C38-FA163EA71BD5.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/04312B0B-EF47-E811-A51C-FA163E8FE329.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/D8423100-4048-E811-84AC-FA163EE42437.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/BEE67D90-5648-E811-93FA-FA163EC3055A.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/9EC71795-5648-E811-9165-FA163E6E06A7.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/6C76E095-5648-E811-A817-FA163E895918.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/DC10A8D7-6048-E811-822E-FA163EAC15A1.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/32BAC90C-6048-E811-9ACB-FA163E5B0035.root',
'root://cms-xrd-global.cern.ch//store/mc/RunIISummer18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/101X_upgrade2018_realistic_v7-v1/10001/44A69393-EC49-E811-9620-FA163ED42175.root'


])
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
