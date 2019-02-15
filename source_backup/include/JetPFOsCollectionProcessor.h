#ifndef JetPFOsCollectionProcessor_h
#define JetPFOsCollectionProcessor_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include "TH1D.h"
//#include <iostream>
#include <sstream>

#include <EVENT/LCCollection.h>

using namespace lcio ;
using namespace marlin ;


/**  Example processor for marlin.
 * 
 *  If compiled with MARLIN_USE_AIDA 
 *  it creates a histogram (cloud) of the MCParticle energies.
 * 
 *  <h4>Input - Prerequisites</h4>
 *  Needs the collection of MCParticles.
 *
 *  <h4>Output</h4> 
 *  A histogram.
 * 
 * @param CollectionName Name of the MCParticle collection
 * 
 * @author F. Gaede, DESY
 * @version $Id: JetPFOsCollectionProcessor.h,v 1.4 2005-10-11 12:57:39 gaede Exp $ 
 */

class JetPFOsCollectionProcessor : public Processor {
  
 public:
  
  virtual Processor*  newProcessor() { return new JetPFOsCollectionProcessor ; }
  
  
  JetPFOsCollectionProcessor() ;
  
  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;
  
  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;
  
  /** Called for every event - the working horse.
   */
  virtual void processEvent( LCEvent * evt ) ; 
  
  
  virtual void check( LCEvent * evt ) ; 
  
  /** Called after data processing for clean up.
   */
  virtual void end() ;
  
  void addCollectionFastJetPFOs(LCEvent *evt, std::string _colFastJet, std::string _colPFOsAfterFJ );
  
 protected:

  /** Input collection name.
   */
  std::string _colMCP ;
  std::string _colMCTL ;
  std::string _colPFOs ;
  std::string _colPFOsWithoutOverlay ;
  std::string _colJets ;
  std::string _colJetPFOs ;

  int _nRun ;
  int _nEvt ;

} ;

#endif



