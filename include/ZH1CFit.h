#ifndef ZH1CFit_h
#define ZH1CFit_h 1

#include "marlin/Processor.h"
#include "lcio.h"
#include <string>
#include <TFile.h>

using namespace lcio ;
using namespace marlin ;


/**  Processor for Hinv kinematic fit 
 *   
 *   ... testing Z -> 2jets and H -> invisible hypothesis
 *   with soft Z mass constraint
 * 
 * @author Yu Kato, U.Tokyo
 * @version $Id: ZH1CFit.h,v 1.0 2019/02/15 20:21 Yu Kato Exp $ 
 */

class ZH1CFit : public Processor {
  
 public:
  
  virtual Processor*  newProcessor() { return new ZH1CFit ; }
  
  
  ZH1CFit() ;
  
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
  
  Double_t getResolution(std::string type,Double_t Energy, Double_t costheta, std::string detector);
  Double_t JetEnergyResolution(Double_t E);
  Double_t JetEnergyRes(Double_t E, Double_t costheta);
  Double_t JetThetaRes(Double_t E, Double_t costheta);
  Double_t JetPhiRes(Double_t E, Double_t costheta);

 protected:

  /** Input collection name.
   */
  std::string _colJets, _colMKFJets, _detector;
  /** Input parameter: center of mass energy.
   */
  float _ecm, _isrpzmax;
  int _fitISR, _ifitter, _ievttrace,_ijetpar;
  bool _traceall;
  double _errene, _errtheta, _errphi;     
  double _gammaZ;
  double b, ISRPzMaxB;

 
  float prob, bestprob, bestnit, bestmassZ, bestmassH, beststartmassZ, beststartmassH, bestphotonenergy, startmassZ, startmassH, variable;
  float momentum[3], energy;
           
  int _nRun, _nEvt, nit;
 
  int bestperm, errorflag;
   
  
  //output
  // TTree *outTree;
         
} ;

#endif



