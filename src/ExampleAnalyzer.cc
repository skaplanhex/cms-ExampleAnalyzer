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
      //the handle which will be holding the genParticles from the event.  Don't confuse this with the particles InputTag!  These are two different things.  reco::GenParticleCollection is just a typedef for std::vector< reco::GenParticle >.
      edm::Handle< reco::GenParticleCollection > particles;
      //this object represents the InputTag that is passed to the analyzer in the config file
      edm::InputTag particles_;
      //declaration of a few histograms that we'll fill
      TH1D* higgsPt;
      TH1D* higgsEta;
      TH1D* higgsPhi;
      TH1D* higgsCount;
      TH1D* higgsStatus;
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
  //This line looks at the paramater set that is passed to the analyzer via the config file.  The particles_ object will represent whatever is passed to the particles variable in the config file (in our case, the genParticles).
  particles_ = iConfig.getParameter<edm::InputTag>("particles");

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
   //particles_ is the InputTag object.  Look at the constructor for how it was initialized.  This line says to look in the event for the object with the InputTag that particles_ represents (in our case, the genParticles) and copy the content to the particles edm::Handle.  We can then do whatever we want with the particles!
   iEvent.getByLabel(particles_,particles);
   int higgsCounter = 0;
   for (reco::GenParticleCollection::const_iterator iParticle = particles->begin(); iParticle != particles->end(); ++iParticle){
    //only want to look at higgs bosons, so we will cut on pdgid (for higgs, this is 25)
    //only look at status 3 higgs
    if ( (iParticle->pdgId() != 25) || (iParticle->status() != 3) ) continue;
    //increase higgs counter
    higgsCounter++;
    //fill histograms with phi, eta, and pt.  Note that the reco::GenParticle class inherits from reco::Candidate where these methods are defined.  All of the reco jet classes inherit from this class as well.
    higgsPhi->Fill( iParticle->phi() );
    higgsEta->Fill( iParticle->eta() );
    higgsPt->Fill( iParticle->pt() );
    higgsStatus->Fill( iParticle->status() );

   }
   //only fill this after the particle loop so we know how many higgs bosons were analyzed after the whole loop
   higgsCount->Fill(higgsCounter);


}


// ------------ method called once each job just before starting event loop  ------------
void 
ExampleAnalyzer::beginJob()
{
  //these lines book our histograms.  Look at http://root.cern.ch/root/html/TH1.html for the constructors.  Unlike regular ROOT macros, when using CMSSW, the histogram booking must be done through the TFileService object!
  higgsPhi = fs->make<TH1D>("higgsPhi","Higgs Phi",150,0,3.141593);
  higgsEta = fs->make<TH1D>("higgsEta","Higgs Eta",600,-6,6);
  higgsPt = fs->make<TH1D>("higgsPt", "Higgs pT",1200,0,1200);
  higgsStatus = fs->make<TH1D>("higgsStatus", "Higgs Status",4,-0.5,3.5);
  higgsCount = fs->make<TH1D>("higgsCount", "Number Of Higgs Bosons Per Event",4,-0.5,3.5);
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
