import FWCore.ParameterSet.Config as cms

process = cms.Process("USER")

process.load("FWCore.MessageService.MessageLogger_cfi")

#how many events to process.  -1 means all of them
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
       '/store/data/Run2012B/JetHT/AOD/22Jan2013-v1/20000/00992A80-DF70-E211-9872-0026189437FE.root',
       '/store/data/Run2012B/JetHT/AOD/22Jan2013-v1/20000/00BC8D92-7371-E211-B38E-003048678E80.root',
       '/store/data/Run2012B/JetHT/AOD/22Jan2013-v1/20000/02B3FF85-1170-E211-B26E-003048FF9AA6.root'
    )
)

#this lets cmsRun know we want TFile output (histograms, TGraphs, etc.)
process.TFileService = cms.Service("TFileService",
      fileName = cms.string("plots.root")
)
#add the example analyzer to the process object
process.analyze = cms.EDAnalyzer('ExampleAnalyzer',
	#jets is a variable representing an InputTag (a descriptor of a certain object in the event, in this case, the genParticles).  The InputTag desired can be found by doing an edmDumpEventContent on one of the files in the dataset to see all the objects in the event.  Then, choose whatever you want to use.
	jets = cms.InputTag("ak5PFJets")
)

#the path tells cmsRun which modules to be run in which order. In our case, we just need to run the analyzer
process.p = cms.Path(process.analyze)
