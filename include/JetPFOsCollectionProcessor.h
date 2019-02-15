#ifndef JetPFOsCollectionProcessor_h
#define JetPFOsCollectionProcessor_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include "TH1D.h"
#include <sstream>

#include <EVENT/LCCollection.h>

using namespace lcio ;
using namespace marlin ;


/**  Benchmark processor for produce JetPFOs Collection.
 * 
 *  produce PFOs collection from jet collection
 * 
 * @author J. Tian, ICEPP
 * @version $Id: JetPFOsCollectionProcessor.h,v 1.0 2019/02/15 20:27 Yu Exp $ 
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



