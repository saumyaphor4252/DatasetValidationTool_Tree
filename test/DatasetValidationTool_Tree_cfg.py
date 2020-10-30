import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

####################################################################
# Get the Magnetic Field
####################################################################
process.load('Configuration.StandardSequences.MagneticField_cff')

###################################################################
# Standard loads
###################################################################
process.load("Configuration.Geometry.GeometryRecoDB_cff")

####################################################################
# Get the BeamSpot
####################################################################
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")

####################################################################
# Get the MessageLogger
####################################################################
process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff") # Global tag
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,'auto:run2_data','')

process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))

import Alignment.CommonAlignment.tools.trackselectionRefitting as trackselRefit
process.seqTrackselRefit = trackselRefit.getSequence(process, 'ALCARECOTkAlCosmicsCTF0T',
                                                     isPVValidation=False, 
                                                     TTRHBuilder='WithAngleAndTemplate',
                                                     usePixelQualityFlag=True,
                                                     openMassWindow=False,
                                                     cosmicsDecoMode=True,
                                                     cosmicsZeroTesla=False,
                                                     momentumConstraint=None,
                                                     cosmicTrackSplitting=False,
                                                     use_d0cut=False)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                #     'root://cms-xrd-global.cern.ch//store/data/Run2016A/Cosmics/ALCARECO/TkAlCosmics0T-07Dec2018-v1/20000/783C9E32-480B-E911-92B2-20040FE8ECAC.root',
                                'root://cms-xrd-global.cern.ch//store/data/Run2018B/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v2/000/318/779/00000/EA2D00B8-A67B-E811-80C9-FA163E814F38.root',
                                #     'root://cms-xrd-global.cern.ch//store/data/Run2018B/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v2/000/318/768/00000/CE658A6C-977B-E811-A502-02163E01A0EA.root',
                                #     'root://cms-xrd-global.cern.ch//store/data/Run2018C/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v3/000/319/890/00000/68BA5D1D-8C8C-E811-B162-FA163E6F94C5.root',
                                #     'root://cms-xrd-global.cern.ch//store/data/Run2017A/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v1/000/295/002/00000/BCC65A9E-EC44-E711-983F-02163E01A73E.root',
                                #     'root://cms-xrd-global.cern.ch//store/data/Run2017A/Cosmics/ALCARECO/TkAlCosmics0T-PromptReco-v1/000/295/717/00000/F64A2345-A847-E711-AB41-02163E0134A2.root' 
                            )
                        )

process.demo = cms.EDAnalyzer('DatasetValidationTool_Tree',
     tracks = cms.InputTag("FinalTrackRefitter")
)

process.p = cms.Path(process.seqTrackselRefit*process.demo)
