#ifndef OverlayISRRemovalByMCProcessor_h
#define OverlayISRRemovalByMCProcessor_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>

#include <EVENT/LCCollection.h>


/**  Benchmark processor for cheating .
 * 
 * 
 * @author J. Tian, ICEPP
 * @version $Id: OverlayISRRemovalByMCProcessor.h,v 1.0 2019/02/15 20:31 Yu Exp $ 
 */

class OverlayISRRemovalByMCProcessor : public marlin::Processor {
  
 public:
  
  virtual Processor*  newProcessor() { return new OverlayISRRemovalByMCProcessor ; }
  
  
  OverlayISRRemovalByMCProcessor() ;
  
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
  
 protected:

  /** Input collection name.
   */
  std::string _colMCP ;
  std::string _colMCTL ;
  std::string _colPFOs ;
  std::string _colPFOsWithoutOverlay ;
  std::string _colPFOsWithoutISR ;
  std::string _colPFOsWithoutOverlayISR ;
  int _serialISR1 ;
  int _serialISR2 ;

} ;

#endif



