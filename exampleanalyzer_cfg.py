import FWCore.ParameterSet.Config as cms

process = cms.Process("USER")

process.load("FWCore.MessageService.MessageLogger_cfi")

#how many events to process.  -1 means all of them
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
       'root://xrootd.unl.edu//store/mc/Summer12_DR53X/RadionToHHTo4B_M-1500_TuneZ2star_8TeV-nm-madgraph/AODSIM/PU_S10_START53_V7C-v1/10000/145F4514-979E-E211-90A1-00266CFFA048.root',
       'root://xrootd.unl.edu//store/mc/Summer12_DR53X/RadionToHHTo4B_M-1500_TuneZ2star_8TeV-nm-madgraph/AODSIM/PU_S10_START53_V7C-v1/10000/80CC2A81-849E-E211-9EE9-00266CFFA2B8.root',
       'root://xrootd.unl.edu//store/mc/Summer12_DR53X/RadionToHHTo4B_M-1500_TuneZ2star_8TeV-nm-madgraph/AODSIM/PU_S10_START53_V7C-v1/10000/B6DEC909-C49E-E211-AAF7-00266CFFA048.root',
       'root://xrootd.unl.edu//store/mc/Summer12_DR53X/RadionToHHTo4B_M-1500_TuneZ2star_8TeV-nm-madgraph/AODSIM/PU_S10_START53_V7C-v1/10000/D0A664D6-899E-E211-9F50-00266CFFA754.root'
    )
)

#this lets cmsRun know we want TFile output (histograms, TGraphs, etc.)
process.TFileService = cms.Service("TFileService",
      fileName = cms.string("plots.root")
)
#add the example analyzer to the process object
process.analyze = cms.EDAnalyzer('ExampleAnalyzer',
	#particles is a variable representing an InputTag (a descriptor of a certain object in the event, in this case, the genParticles).  The InputTag desired can be found by doing an edmDumpEventContent on one of the files in the dataset to see all the objects in the event.  Then, choose whatever you want to use.
	particles = cms.InputTag("genParticles")
)

#the path tells cmsRun which modules to be run in which order. In our case, we just need to run the analyzer
process.p = cms.Path(process.analyze)
