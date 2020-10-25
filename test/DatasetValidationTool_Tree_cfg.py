import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")


process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.Geometry.GeometryRecoDB_cff') # Ideal geometry and interface

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff") # Global tag
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,'auto:run2_data','')

process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'root://cms-xrd-global.cern.ch//store/data/Run2016A/Cosmics/ALCARECO/TkAlCosmics0T-07Dec2018-v1/20000/783C9E32-480B-E911-92B2-20040FE8ECAC.root'
    )
)

process.demo = cms.EDAnalyzer('DatasetValidationTool_Tree',
     tracks = cms.InputTag("ALCARECOTkAlCosmicsCTF0T")
)

process.p = cms.Path(process.demo)
