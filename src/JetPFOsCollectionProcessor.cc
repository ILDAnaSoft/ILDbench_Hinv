// *****************************************************
// Processor for overlay optimization
//                        ----Junping
// *****************************************************
#include "JetPFOsCollectionProcessor.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/ReconstructedParticle.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <EVENT/Cluster.h>
#include <UTIL/LCTypedVector.h>
#include <EVENT/Track.h>
#include <UTIL/LCRelationNavigator.h>
#include <EVENT/ParticleID.h>
#include <marlin/Exceptions.h>

// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TNtupleD.h"
#include "TVector3.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include "TMVA/Reader.h"

#include "Utilities.h"

using namespace lcio ;
using namespace marlin ;
using namespace std;

using namespace TMVA;
using namespace mylib;

JetPFOsCollectionProcessor aJetPFOsCollectionProcessor ;


JetPFOsCollectionProcessor::JetPFOsCollectionProcessor() : Processor("JetPFOsCollectionProcessor") {
  
  // modify processor description
  _description = "JetPFOsCollectionProcessor does whatever it does ..." ;
  

  // register steering parameters: name, description, class-variable, default value

  registerInputCollection( LCIO::MCPARTICLE,
			   "InputMCParticlesCollection" , 
			   "Name of the MCParticle collection"  ,
			   _colMCP ,
			   std::string("MCParticlesSkimmed") ) ;

  registerInputCollection( LCIO::LCRELATION,
			   "InputMCTruthLinkCollection" , 
			   "Name of the MCTruthLink collection"  ,
			   _colMCTL ,
			   std::string("RecoMCTruthLink") ) ;

  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			   "InputPandoraPFOsCollection" , 
			   "Name of the PandoraPFOs collection"  ,
			   _colPFOs ,
			   std::string("PandoraPFOs") ) ;

  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			   "InputJetCollection" , 
			   "Name of the Jet collection"  ,
			   _colJets ,
			   std::string("FastJets") ) ;

  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			    "OutputPFOsWithoutOverlayCollection",
			    "Name of the PFOs collection without overlaid gam-gam to hadron background",
			    _colPFOsWithoutOverlay,
			    std::string("PFOsWithoutOverlay") );

  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			    "OutputJetPFOsCollection",
			    "Name of the PFOs collection selected by Jet clustering",
			    _colJetPFOs,
			    std::string("JetPFOs") );

}

void JetPFOsCollectionProcessor::init() { 

  streamlog_out(DEBUG) << "   init called  " 
		       << std::endl ;
  
  
  // usually a good idea to
  printParameters() ;

  _nRun = 0 ;
  _nEvt = 0 ;

}

void JetPFOsCollectionProcessor::processRunHeader( LCRunHeader* run) { 

  _nRun++ ;
} 

void JetPFOsCollectionProcessor::processEvent( LCEvent * evt ) { 

    
  // this gets called for every event 
  // usually the working horse ...
  _nEvt++;

  // -- Read out MC information --  
  LCCollection *colMC = evt->getCollection(_colMCP);
  if (!colMC) {
    std::cerr << "No MC Collection Found!" << std::endl;
    throw marlin::SkipEventException(this);
  }

  // -- Get the MCTruth Linker --
  LCCollection *colMCTL = evt->getCollection(_colMCTL);
  if (!colMCTL) {
    std::cerr << "No MCTruthLink Collection Found!" << std::endl;
    throw marlin::SkipEventException(this);
  }
  //  LCRelationNavigator *navMCTL = new LCRelationNavigator(colMCTL);

  // -- Read out PFO information --
  LCCollection *colPFO = evt->getCollection(_colPFOs);
  if (!colPFO) {
    std::cerr << "No PFO Collection Found!" << std::endl;
    throw marlin::SkipEventException(this);
  }
  LCCollectionVec *pPFOsWithoutOverlayCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  pPFOsWithoutOverlayCollection->setSubset(true);
  Int_t nPFOs = colPFO->getNumberOfElements();
  for (Int_t i=0;i<nPFOs;i++) {
    ReconstructedParticle *pfo = dynamic_cast<ReconstructedParticle*>(colPFO->getElementAt(i));
    Double_t weight = 1.;
    Int_t nMCTL = 1;
    MCParticle *mc = getMCParticle(pfo,colMCTL,weight,nMCTL);
    Bool_t iOverlay = mc ? mc->isOverlay() : kFALSE; // corrected 10/14
    if (!iOverlay) {
      pPFOsWithoutOverlayCollection->addElement(pfo);
    }
    
  }
  // add new collections
  evt->addCollection(pPFOsWithoutOverlayCollection,_colPFOsWithoutOverlay.c_str());

  addCollectionFastJetPFOs(evt, _colJets, _colJetPFOs);


  //-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !

  streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber() 
  		       << "   in run:  " << evt->getRunNumber() 
  		       << std::endl ;

  //  _nEvt ++ ;

}



void JetPFOsCollectionProcessor::check( LCEvent * evt ) { 
  // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void JetPFOsCollectionProcessor::end(){ 

  cerr << "JetPFOsCollectionProcessor::end()  " << name() 
       << " processed " << _nEvt << " events in " << _nRun << " runs "
       << endl ;
  //  cerr << endl;
  cerr << "  =============" << endl;
  cerr << "   Cut Summary " << endl;
  cerr << "  =============" << endl;
  cerr << "   ll+4 Jet    " << endl;
  cerr << "  =============" << endl;
  cerr << endl
       << "  -----------------------------------------------------------" << endl
       << "   ID   No.Events    Cut Description                         " << endl
       << "  -----------------------------------------------------------" << endl;
  
}

void JetPFOsCollectionProcessor::addCollectionFastJetPFOs(LCEvent *evt, std::string _colFastJet, std::string _colPFOsAfterFJ ) { 
  // -- Read out FastJet information --
  LCCollection *colFastJet = evt->getCollection(_colFastJet);
  if (!colFastJet) {
    std::cerr << "No FastJets Collection Found!" << std::endl;
    throw marlin::SkipEventException(this);
  }
  LCCollectionVec *pPFOsAfterFJCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  pPFOsAfterFJCollection->setSubset(true);
  Int_t nJets = colFastJet->getNumberOfElements();
  for (Int_t i=0;i<nJets;i++) {
    ReconstructedParticle *jet = dynamic_cast<ReconstructedParticle*>(colFastJet->getElementAt(i));
    std::vector<lcio::ReconstructedParticle*> partVec = jet->getParticles();
    for (std::vector<lcio::ReconstructedParticle*>::const_iterator iPart=partVec.begin();iPart!=partVec.end();++iPart) {
      pPFOsAfterFJCollection->addElement((*iPart));
    }
  }
  evt->addCollection(pPFOsAfterFJCollection,_colPFOsAfterFJ.c_str());
}
