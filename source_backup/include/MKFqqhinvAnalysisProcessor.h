#ifndef MKFqqhinvAnalysisProcessor_h
#define MKFqqhinvAnalysisProcessor_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>
#include <string>
#include "TFile.h"
#include "TH1D.h"
#include "TString.h"
#include "TLorentzVector.h"
//#include <iostream>
#include <sstream>


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
 * @version $Id: MKFqqhinvAnalysisProcessor.h,v 1.4 2005-10-11 12:57:39 gaede Exp $ 
 */

class MKFqqhinvAnalysisProcessor : public marlin::Processor {
  
 public:
  
  virtual Processor*  newProcessor() { return new MKFqqhinvAnalysisProcessor ; }  
  
  MKFqqhinvAnalysisProcessor() ;
  
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

  Double_t getCosHel(TLorentzVector particle, TLorentzVector parent,
		     TLorentzVector grandparent);

 protected:

  /** Input collection name.
   */
  std::string _colMCP ;
  std::string _colPFOs ;
  std::string _colLeptons ;
  std::string _colJets;
  std::string _colMKFJets;
  //std::string _outRootFile;
  
  int _nRun ;
  int _nEvt ;

  float _ecm ;

  //TFile *output;
  TH1D *hStatAnl;
  std::stringstream gCutName[20];
} ;

#endif



