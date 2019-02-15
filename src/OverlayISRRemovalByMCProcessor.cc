// *****************************************************
// Processor for Overlay and ISR Removal By MC
//                        ----Junping
// *****************************************************
#include "OverlayISRRemovalByMCProcessor.h"

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
#include "Utilities.h"

using namespace lcio ;
using namespace marlin ;
using namespace std;
using namespace mylib;

OverlayISRRemovalByMCProcessor aOverlayISRRemovalByMCProcessor ;


OverlayISRRemovalByMCProcessor::OverlayISRRemovalByMCProcessor() : Processor("OverlayISRRemovalByMCProcessor") {
  
  // modify processor description
  _description = "OverlayISRRemovalByMCProcessor does whatever it does ..." ;
  

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

  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			    "OutputPFOsWithoutOverlayCollection",
			    "Name of the PFOs collection without Overlay",
			    _colPFOsWithoutOverlay,
			    std::string("PFOsWithoutOverlay") );
  
  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			    "OutputPFOsWithoutISRCollection",
			    "Name of the PFOs collection without ISR",
			    _colPFOsWithoutISR,
			    std::string("PFOsWithoutISR") );

  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			    "OutputPFOsWithoutOverlayISRCollection",
			    "Name of the PFOs collection without Overlay and ISR",
			    _colPFOsWithoutOverlayISR,
			    std::string("PFOsWithoutOverlayISR") );

  registerProcessorParameter("IndexOfISR1",
			     "Index of the 1st ISR in MCParticlesSkimmed",
			     _serialISR1,
			     int(4) );

  registerProcessorParameter("IndexOfISR2",
			     "Index of the 2nd ISR in MCParticlesSkimmed",
			     _serialISR2,
			     int(5) );
}

void OverlayISRRemovalByMCProcessor::init() { 
}

void OverlayISRRemovalByMCProcessor::processRunHeader( LCRunHeader* run) { 
} 

void OverlayISRRemovalByMCProcessor::processEvent( LCEvent * evt ) { 

  // -- Read out MC information --  
  LCCollection *colMC = evt->getCollection(_colMCP);
  if (!colMC) {
    std::cerr << "No MC Collection Found!" << std::endl;
    throw marlin::SkipEventException(this);
  }

  // -- Get the MCTruth Linker --
  LCCollection *colMCTL = evt->getCollection(_colMCTL);

  // -- Read out PFO information --
  LCCollection *colPFO = evt->getCollection(_colPFOs);
  if (!colPFO) {
    std::cerr << "No PFO Collection Found!" << std::endl;
    throw marlin::SkipEventException(this);
  }

  LCCollectionVec *pPFOsWithoutOverlayCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  LCCollectionVec *pPFOsWithoutISRCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  LCCollectionVec *pPFOsWithoutOverlayISRCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  pPFOsWithoutOverlayCollection->setSubset(true);
  pPFOsWithoutISRCollection->setSubset(true);
  pPFOsWithoutOverlayISRCollection->setSubset(true);

  MCParticle *isr1 = dynamic_cast<MCParticle *>(colMC->getElementAt(_serialISR1));
  MCParticle *isr2 = dynamic_cast<MCParticle *>(colMC->getElementAt(_serialISR2));

  Int_t nPFOs = colPFO->getNumberOfElements();
  for (Int_t i=0;i<nPFOs;i++) {
    ReconstructedParticle *pfo = dynamic_cast<ReconstructedParticle*>(colPFO->getElementAt(i));
    MCParticle *mcp = getMCParticle(pfo,colMCTL);
    Bool_t iOverlay = kFALSE;
    Bool_t iISR = kFALSE;
    if (mcp) {
      if (mcp->isOverlay()) iOverlay = kTRUE;
      if (isFromMCParticle(mcp,isr1) || isFromMCParticle(mcp,isr2)) iISR = kTRUE;
    }
    if (!iOverlay) pPFOsWithoutOverlayCollection->addElement(pfo);
    if (!iISR) pPFOsWithoutISRCollection->addElement(pfo);
    if (!iOverlay && !iISR) pPFOsWithoutOverlayISRCollection->addElement(pfo);    
  }
  // add new collections
  evt->addCollection(pPFOsWithoutOverlayCollection,_colPFOsWithoutOverlay.c_str());
  evt->addCollection(pPFOsWithoutISRCollection,_colPFOsWithoutISR.c_str());
  evt->addCollection(pPFOsWithoutOverlayISRCollection,_colPFOsWithoutOverlayISR.c_str());

}



void OverlayISRRemovalByMCProcessor::check( LCEvent * evt ) { 
  // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void OverlayISRRemovalByMCProcessor::end(){ 
}
