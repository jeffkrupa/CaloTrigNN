# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:phase1_2017_realistic --pileup_input das:/RelValMinBias_13/CMSSW_10_0_0_pre2-100X_mc2017_realistic_v1-v1/GEN-SIM -n 10 --era Run2_2017 --eventcontent RECOSIM --runUnscheduled -s RAW2DIGI,L1Reco,RECO,RECOSIM --datatier GEN-SIM-RECO --pileup AVE_35_BX_25ns --geometry DB:Extended --filein file:step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2017)

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
    fileNames = cms.untracked.vstring('file:step2.root'),
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
process.mix.input.nbPileupEvents.averageNumber = cms.double(35.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
process.mix.input.fileNames = cms.untracked.vstring(['/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/002C9BDE-400C-E811-B077-02163E014153.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/004755D7-400C-E811-8FAC-02163E01A27C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/006518E7-400C-E811-90C9-A4BF01125620.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/00C115F7-400C-E811-BF5B-02163E014491.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/00D9F152-410C-E811-B810-02163E019D4F.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/02247AD3-400C-E811-A18A-02163E0139CB.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/022E52DD-400C-E811-A30F-02163E01A2E2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0251F728-410C-E811-848B-02163E01A24C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0272453C-410C-E811-BC23-7845C4FC3AE5.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/02770949-7A0C-E811-9C75-02163E019BE9.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/028D3801-410C-E811-B470-549F3525C318.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/02AAF4D7-400C-E811-9A19-02163E0146FE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/02AE45E2-400C-E811-8A42-02163E013826.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/02BB97B0-400C-E811-8529-02163E01420B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04096177-420C-E811-94C4-02163E0144A9.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/044881D8-400C-E811-933A-02163E0124ED.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04A753B1-400C-E811-B08D-02163E0119C8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04B635E1-420C-E811-B045-02163E011F7D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04D6172B-420C-E811-8A77-02163E01A3A4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04D9D1C4-400C-E811-842B-02163E01A6A3.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04E777B1-400C-E811-987B-02163E01A3A2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04E9BB1A-410C-E811-B047-02163E01A749.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/04FFA86B-650C-E811-A63F-02163E01A409.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0614EA58-4D0C-E811-843E-FA163EEDFAA2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/062807E9-400C-E811-BBFE-02163E013623.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/06288AD4-400C-E811-BA7C-02163E014698.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/063AF6EC-400C-E811-811A-02163E01A476.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0660CCEB-400C-E811-BC03-02163E019BB6.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/06610DC8-400C-E811-A968-02163E01A520.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/06AB43E1-420C-E811-9053-02163E014353.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/06C9D4B1-400C-E811-B6B4-02163E0137AC.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0807BA12-650C-E811-BAC5-02163E019CCA.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/081F3FD1-400C-E811-82D4-02163E011A9D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0832FC19-410C-E811-9BBD-000101000920.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/08442A20-410C-E811-917D-0CC47AA53DBE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/08560AFA-420C-E811-AF6D-02163E019B6A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/086FE81D-410C-E811-9279-02163E01A2F9.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0875C9DC-400C-E811-BB07-02163E013851.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/08C1C62E-410C-E811-A7D8-02163E0125F8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/08D4DEE5-400C-E811-889B-02163E01A32D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/08EDA546-410C-E811-8AB4-02163E01A5C4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0A91A26C-410C-E811-85AA-001E67E7136D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0AAFD2B5-400C-E811-95C4-02163E014199.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0ACEF7DA-400C-E811-AD60-02163E01432D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0ACFD1EB-400C-E811-93DA-02163E01455A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0C81A134-420C-E811-B6C7-02163E01A2AC.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0C8D8FD7-400C-E811-9C08-02163E014660.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0C9C7F3B-650C-E811-9070-02163E011F9E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0CD2B2C9-400C-E811-98F3-02163E014769.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0CF42CD3-400C-E811-833F-02163E01A454.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E15F5F5-640C-E811-9C6A-0242AC1C0501.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E2DAF15-410C-E811-A27B-02163E01A4D7.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E342565-650C-E811-B629-0242AC1C0502.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E394860-650C-E811-AA25-0CC47A0AD63E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E407976-580C-E811-B023-00266CFC3B0C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E4F0995-860C-E811-8AD9-0CC47A6C06C4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E6EF9DF-400C-E811-AD3A-B083FECF837A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0E72A394-650C-E811-B0E5-008CFAC91BC4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0EA4D819-420C-E811-AECC-002590E2D9FE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0EC4E0CF-400C-E811-ABB6-02163E0143BE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0ED8591B-680C-E811-8462-0CC47A4D76B2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/0EF11436-420C-E811-B7BF-02163E019B82.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1007A9D1-400C-E811-8100-E0071B740D80.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/100F9EF7-400C-E811-9399-02163E01A500.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/104E7BE0-400C-E811-82CD-FA163EED3A98.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/106443DF-400C-E811-9305-0CC47A13D416.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/10872F02-410C-E811-8253-0CC47A13D2A4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/12375911-420C-E811-B7A9-EC0D9A822636.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1272F27E-410C-E811-B6CB-02163E019C1F.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/129C4EC2-400C-E811-B0EB-02163E01A287.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/12A9A80F-410C-E811-89B0-02163E01A694.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/12D5E360-650C-E811-9E9F-02163E014263.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/12E2EB60-410C-E811-816B-02163E01A58E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1406FED3-7C0C-E811-8FAF-E0071B6C9DE0.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/141787C6-400C-E811-82B6-02163E0145FF.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/141E86A9-4D0C-E811-9849-0CC47AC087AE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1455B8AC-400C-E811-9D1C-02163E0141D4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1457123B-420C-E811-99F0-02163E019D8C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/145FED11-420C-E811-BBF1-24BE05CE1E51.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/147CFFEC-640C-E811-9365-02163E0122AD.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/14913CC9-400C-E811-8C1B-02163E014416.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/14DFB2B9-400C-E811-8EEB-02163E011CBF.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/14E8D1DC-400C-E811-85C2-02163E01472F.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/160F1F25-410C-E811-BA70-02163E013823.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/161602F5-400C-E811-9BBE-02163E01A7A7.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/164316E9-740C-E811-A808-001E67E71CAE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/167AF8B2-410C-E811-995A-44A842CF0634.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1697C1FA-400C-E811-8E20-02163E019D50.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/16B39EC5-400C-E811-9BD0-02163E01240E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/16B9A8D7-410C-E811-B30C-02163E019B2F.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/16C450BC-400C-E811-BAD0-02163E01438E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/16D0F8AD-400C-E811-99BE-02163E0143FB.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/18120DDD-400C-E811-8657-02163E011CB6.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/18129832-420C-E811-9ED8-02163E019D00.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/181A05B0-400C-E811-A168-02163E01436D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/181B9A4A-420C-E811-B4A0-02163E01A391.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/182F3F19-420C-E811-BB13-0CC47A13CCEE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1841F380-410C-E811-8DDD-02163E019CE2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/186AFEB6-400C-E811-BDD6-02163E0119A9.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/187374F5-640C-E811-9D4F-02163E011BD8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/18CEC101-420C-E811-999F-FA163EDDB735.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/18D3F6D7-400C-E811-B0D9-02163E014424.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/18E05203-700C-E811-BBAA-001E67E7136D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/18F40803-650C-E811-B07A-FA163E3FBA0D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/18FC2DE6-400C-E811-9634-02163E01A2C4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1A596C25-410C-E811-B2FE-02163E01A79B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1A78D0F7-400C-E811-B97E-02163E01427B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1A8CDB1E-410C-E811-A618-02163E01A425.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1A932CE5-400C-E811-ACAD-02163E01A5D2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1ABBBCB9-400C-E811-876F-02163E0119F6.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1ADB57FE-400C-E811-957B-02163E01A387.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1C04DDC5-640C-E811-B4D5-0CC47A010010.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1C45810E-410C-E811-8B82-02163E01A6CD.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1C4B65C0-400C-E811-AE22-02163E01399B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1C5D0B5E-630C-E811-9856-FA163E658CA8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1C811C36-650C-E811-9A1B-02163E014208.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1CA625AF-400C-E811-9094-02163E012202.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1CAE2CDC-400C-E811-8696-02163E011C55.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1CB5A6FF-640C-E811-B25B-02163E01A466.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1CC03647-410C-E811-B178-001517FAB928.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1CF7C0BA-4D0C-E811-9581-008CFAFBED9C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1E0931A1-7E0C-E811-9B6D-FA163E787040.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1E499ED2-400C-E811-A043-02163E0139D0.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1E624A4E-410C-E811-A423-02163E019C65.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1E7070E3-400C-E811-9911-02163E014706.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1EAB3687-410C-E811-BA32-02163E01A689.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1EB224D8-410C-E811-901A-001E67E34165.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1EFDE1E9-400C-E811-A9F2-008CFAE45180.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/1EFE121B-420C-E811-9057-00259048A8F4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2011C8B1-400C-E811-B239-02163E011E3A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/203B4D5F-4D0C-E811-8535-FA163EBEDF19.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/203D89B3-420C-E811-AA64-44A842CFC9F3.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2083F1E3-400C-E811-BC51-0CC47A6C1866.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/208F7058-410C-E811-876E-68B59972BF62.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/20DB03E8-400C-E811-AE7C-02163E01A571.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/20F69DD3-400C-E811-A153-02163E01414C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/22299CDC-400C-E811-B740-02163E014725.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/224DBAD6-400C-E811-8ECC-24BE05CE2E81.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/22625EBE-400C-E811-9C57-02163E01384B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/22D16CFA-400C-E811-8014-C81F66B79074.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/22E3911C-420C-E811-A249-02163E01A771.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/240214D0-400C-E811-B509-008CFAC91A00.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/24C9ABD0-400C-E811-8BF2-02163E013940.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2610EF2C-440C-E811-A8B6-02163E0133DC.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2612B3EB-640C-E811-A3DD-001E67248A1B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2619C1E6-400C-E811-A522-0CC47A13D16A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/262D38B6-400C-E811-9884-02163E0135B4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/26AFC31F-420C-E811-9546-0CC47AC30F14.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/26E38054-710C-E811-8F62-0025901D08BF.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/26F373EB-400C-E811-948E-02163E011834.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/281D93D2-400C-E811-9D76-1866DA87AB73.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/28247D04-410C-E811-8528-02163E019E3E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/285070D6-400C-E811-8A46-02163E01461F.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/285E82DE-400C-E811-8A7B-02163E0141CE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/28648DE1-400C-E811-A75A-02163E0143B3.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/289970FB-400C-E811-A404-002590E39C46.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/28A359DD-430C-E811-8CEE-02163E01A3A7.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/28C82BAD-400C-E811-BA50-02163E014590.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/28CF5ADF-410C-E811-A745-001E67E6F9BD.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/28E8962F-650C-E811-8747-02163E0135E1.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/28E98533-980D-E811-AE51-00238BBD7674.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2A25AAE7-410C-E811-930F-001E67E719C5.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2A298081-410C-E811-AA5D-02163E019BFC.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2A381DEA-400C-E811-AF07-02163E01287C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2A52EA0C-410C-E811-91CB-02163E019CFE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2A593C08-420C-E811-816D-02163E019CF1.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2A7B71E4-400C-E811-A4B1-0242AC1C0500.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2A860B11-420C-E811-9806-EC0D9A82260E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2AFE5333-410C-E811-AC3B-02163E01A783.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2C4E2B5E-420C-E811-B5E9-02163E01A45B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2C62DBFF-400C-E811-B058-0CC47A6C0682.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2C69C6FE-400C-E811-8DFB-0CC47A2B0700.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2CD0329E-410C-E811-B135-02163E019D7E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2CF3E01C-420C-E811-BA12-02163E01A3A8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2E7A8392-410C-E811-AAE4-14187741212B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2E96260A-650C-E811-9D3E-0CC47A7E6A88.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/2EAEF9E7-640C-E811-B46F-02163E0135B0.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3016D4EA-400C-E811-B486-0242AC1C0502.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/308C1F46-420C-E811-86A4-02163E019C71.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/30A030E1-400C-E811-A096-02163E01A1F6.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/30B4C881-CF0D-E811-91B7-0CC47A7C360E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/30C636DD-400C-E811-97E4-008CFAC93B94.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/30E9C6DE-400C-E811-84EA-02163E01471B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3203DB57-420C-E811-BBBB-02163E019D3F.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3227E2C9-400C-E811-8D7A-02163E0118DC.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/32404FD0-400C-E811-B73C-02163E01368C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3258EAFE-400C-E811-9EFA-0CC47A13CB62.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/325AE916-410C-E811-BD3D-02163E019DEE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3264C6CB-400C-E811-BA19-02163E01A326.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3295C0B9-400C-E811-BE38-02163E01445A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/32B5C813-410C-E811-9524-02163E014307.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/32BCF0B7-400C-E811-8744-02163E014714.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/32CE56EA-400C-E811-8E48-0242AC1C0502.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/32D42FD4-400C-E811-A1A9-02163E01447A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/32F04CE6-400C-E811-B271-02163E01A370.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/340EDED5-400C-E811-8C1D-02163E014512.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/341B87F1-640C-E811-99E2-02163E01416D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/34283947-410C-E811-BCF8-02163E0141BE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/34766AFF-400C-E811-8E0F-02163E011CAE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/348AC5E9-640C-E811-93A9-02163E014640.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/348FC7AC-400C-E811-BA73-02163E012988.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/349CE115-420C-E811-B70B-02163E014715.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/34D505C2-420C-E811-9F23-0025904C7DF8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/361F380D-650C-E811-91E9-0026B9278637.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/36AC5261-410C-E811-9ED2-02163E01A237.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/36B7379B-A80C-E811-890E-0242AC1C0502.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/36F00023-420C-E811-ADB9-02163E019D6B.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/380F96DC-7C0C-E811-A4B4-001E677924B2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3813FBA3-650C-E811-BBFA-0242AC1C0501.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/383384E7-410C-E811-94E4-001E67E6F431.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/383803D3-400C-E811-BA11-02163E011F20.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/38537EC4-400C-E811-A401-02163E011BC8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/389391FD-400C-E811-8637-02163E019E4E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/38C43FD3-400C-E811-8892-02163E0138B4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/38CCECC2-410C-E811-BBE1-44A842CF0571.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/38D01B22-410C-E811-A7C8-02163E0134A4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/38D398D4-400C-E811-ACB6-02163E012B3D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/38ECE6E3-420C-E811-BA8C-02163E01446A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/38F2B9BC-400C-E811-B524-02163E01424A.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3A0D8C2A-430C-E811-879B-02163E01A527.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3A3860F4-420C-E811-AEF9-02163E0144E1.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3A8DF024-410C-E811-8D56-02163E01A5F8.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3AB991F6-420C-E811-AD68-02163E01A702.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3AE701D3-400C-E811-A210-02163E0142FF.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3C0466D7-400C-E811-BB64-02163E0143E4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3C129E31-410C-E811-9458-02163E01A650.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3C1D28E6-420C-E811-BFDE-02163E011D60.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3C859FE9-410C-E811-8EE8-A4BF01125BF0.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3CA69A77-650C-E811-8547-0242AC1C0500.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3CCA27FF-400C-E811-A3C9-90B11C0BCE25.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3CE21331-420C-E811-A6B8-002590FCB3E2.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3E04A98D-900C-E811-90F8-0CC47A5FA3BD.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3E1005B2-400C-E811-93FE-02163E012ACC.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3E14BA01-650C-E811-9F2B-02163E01A78E.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3E6E99B0-400C-E811-BDC6-02163E0141B7.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3E75ADEE-640C-E811-8265-0242AC1C0500.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3E83BAF7-400C-E811-A445-02163E01A461.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/3EC2DA60-410C-E811-A233-02163E019E17.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/4003FCCF-400C-E811-9EBF-02163E014351.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/402E1FFF-400C-E811-BA5E-90B11C0BCF42.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/4037F415-420C-E811-BA1F-02163E011CE0.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/404178ED-400C-E811-971A-02163E01A573.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/406883ED-400C-E811-B405-02163E01A52D.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/408A06EE-400C-E811-BC80-02163E014145.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/408C7C1B-410C-E811-853B-02163E0138D4.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/40912BE5-400C-E811-98AB-02163E0145FA.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/40E616E4-420C-E811-9A07-02163E0139BF.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/40F850DA-410C-E811-9CBB-000101000925.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/40FC6ABA-610C-E811-8FF6-02163E019BAE.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/423BF243-650C-E811-AB4B-02163E0139B9.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/424581D3-410C-E811-8659-02163E01A621.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/424D29B1-400C-E811-A1E4-02163E01448C.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/42ADA3CF-650C-E811-BE6C-02163E011F73.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/42EF9ED5-400C-E811-A983-02163E012488.root',
'/store/mc/RunIISpring18GS/MinBias_TuneCP5_13TeV-pythia8/GEN-SIM/100X_upgrade2018_realistic_v9-v1/20000/4408D7AF-400C-E811-80B3-02163E011A55.root'])
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
process = L1TReEmulMCFromRAWSimHcalTP(process)

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
