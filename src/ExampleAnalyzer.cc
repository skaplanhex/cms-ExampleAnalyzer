// -*- C++ -*-
//
// Package:    ExampleAnalyzer
// Class:      ExampleAnalyzer
// 
/**\class ExampleAnalyzer ExampleAnalyzer.cc Analyzers/ExampleAnalyzer/src/ExampleAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  steven kaplan
//         Created:  Mon Mar 17 13:26:43 CDT 2014
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//you need the TFileService in order to make plots
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
//for reco::Candidate
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
//for the GenParticleCollection and GenParticles
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
//for the jets
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include <vector>
//
// class declaration
//

class ExampleAnalyzer : public edm::EDAnalyzer {
   public:
      explicit ExampleAnalyzer(const edm::ParameterSet&);
      ~ExampleAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      //the TFileService object
      edm::Service<TFileService> fs;
      //the handle which will be holding the ak5PFJets from the event.  Don't confuse this with the particles InputTag!  These are two different things.  reco::PFJetCollection is just a typedef for std::vector< reco::PFJet >.
      edm::Handle< reco::PFJetCollection > jets;
      //this object represents the InputTag that is passed to the analyzer in the config file
      edm::InputTag jets_;
      //declaration of a few histograms that we'll fill
      TH1D* jetPt;
      TH1D* jetEta;
      TH1D* jetPhi;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ExampleAnalyzer::ExampleAnalyzer(const edm::ParameterSet& iConfig)

{
  //This line looks at the paramater set that is passed to the analyzer via the config file.  The jets_ object will represent whatever is passed to the particles variable in the config file (in our case, the jets).
  jets_ = iConfig.getParameter<edm::InputTag>("jets");

}


ExampleAnalyzer::~ExampleAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ExampleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   //jets_ is the InputTag object.  Look at the constructor for how it was initialized.  This line says to look in the event for the object with the InputTag that jets_ represents (in our case, the ak5PFJets) and copy the content to the jets edm::Handle.  We can then do whatever we want with the jets!
   iEvent.getByLabel(jets_,jets);

   for (reco::PFJetCollection::const_iterator iJet = jets->begin(); iJet != jets->end(); ++iJet){

    //fill histograms with phi, eta, and pt.  Note that the reco::GenParticle class inherits from reco::Candidate where these methods are defined.  All of the reco jet classes inherit from this class as well.
    jetPhi->Fill( iJet->phi() );
    jetEta->Fill( iJet->eta() );
    jetPt->Fill( iJet->pt() );

   }


}


// ------------ method called once each job just before starting event loop  ------------
void 
ExampleAnalyzer::beginJob()
{
  //these lines book our histograms.  Look at http://root.cern.ch/root/html/TH1.html for the constructors.  Unlike regular ROOT macros, when using CMSSW, the histogram booking must be done through the TFileService object!
  jetPhi = fs->make<TH1D>("jetPhi","Higgs Phi",150,0,3.141593);
  jetEta = fs->make<TH1D>("jetEta","Higgs Eta",600,-6,6);
  jetPt = fs->make<TH1D>("jetPt", "Higgs pT",1200,0,1200);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
ExampleAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
ExampleAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
ExampleAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
ExampleAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ExampleAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ExampleAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ExampleAnalyzer);
