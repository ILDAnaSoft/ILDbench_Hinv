// *****************************************************
// MarlinKinfit Processor for
// e+e- ------> Z H ------> q q inv
//                        --------  Y.Kato
// *****************************************************
#include "ZH1CFit.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "UTIL/LCRelationNavigator.h"
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/SimTrackerHit.h>
#include <EVENT/ReconstructedParticle.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <CLHEP/Vector/LorentzVector.h>
#include <marlin/Exceptions.h>
#include "TextTracer.h"
#include "JetFitObject.h"
#include "ISRPhotonFitObject.h"
#include "OPALFitterGSL.h"
#include "NewFitterGSL.h"
#include "NewtonFitterGSL.h"
#include "FourJetZHPairing.h"
#include "MomentumConstraint.h"
#include "MassConstraint.h"
#include "SoftBWMassConstraint.h"

#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TSystem.h"

#include "Utilities.h"

using namespace lcio ;
using namespace marlin ;
using namespace std ;
using namespace CLHEP ;
using namespace mylib ;

ZH1CFit aZH1CFit ;

Double_t ZH1CFit::getResolution( string type, Double_t Energy, Double_t costheta, string detector){
  
  if ( type!="energy" && type != "costheta" && type != "phi" && type != "theta" ){
    std::cerr << "type parameter " << type.c_str() << " is invalid!. Please choose energy or costheta or phi or theta." << std::endl;
    return -1;
  }

  /*if ( Energy < 15 || Energy > 250 ){
    std::cerr << "Energy " << Energy << " GeV is out of range!. Please choose Energy 15 ~ 250" << std::endl;
    return  1;
    }*/

  if ( costheta < 0 || costheta > 1.0 ){
    std::cerr << "|costheta| " << costheta << " rad. is out of range!. Please choose costheta 0 ~ 1.0" << std::endl;
    return  1;
  }
  
  if ( detector != "l5_o1" && detector != "s5_o1" && detector != "l4" && detector != "s4" && detector != "o1" ){
    std::cerr << "detector parameter " << detector.c_str() << " is invalid!. Please choose l4 or s4 or o1." << std::endl;
    return -1;
  }
  
  string name;
  if ( type == "energy" )
    name = "single_precise";
  else if ( type == "costheta" )
    name = "costheta_precise";
  else if ( type == "phi" )
    name = "phi_precise";
  else if ( type == "theta" )
    name = "theta_precise";
  
  if ( detector == "o1" ){
    //std::cout << "Use ILD_l4_v02 table for ILD_o1_v05." << std::endl;
    detector = "l5_o1";
  }

  const Int_t nEnergy = 12;
  const Int_t E[nEnergy] = {30,40,60,91,120,160,200,240,300,350,400,500};
  
  const char *filename=Form("%s/output/ILD_%s_v02/REC/%s/table_%s_%s.txt",
			    gSystem->Getenv("JER"),detector.c_str(),name.c_str(),name.c_str(),detector.c_str());
  std::ifstream ifs(filename);
  if ( !ifs.is_open() ){
    std::cerr << "file " << filename << " cannot open!!" << std::endl;
    return -1;
  }
  
  string str;
  Double_t dbl;
  Double_t res[nEnergy];
  while (ifs>>str){
    char* cstr = new char[str.size() + 1];
    std::char_traits<char>::copy(cstr, str.c_str(), str.size() + 1);
    dbl = std::strtod(cstr,NULL);
    //std::cout << "cstr: " << cstr   << std::endl;
    //std::cout << "dbl: " << fixed << setprecision(2) << dbl << std::endl;
    if (dbl == 0 && str != "0.00"){
      getline(ifs,str);
      continue;
    }
    if (dbl <= costheta){
      getline(ifs,str);
      ifs >> str;
      for (Int_t i=0;i<nEnergy;i++)
	ifs >> res[i];
    }
    else if (dbl > costheta)
      break;
  }
  ifs.close();

  Int_t count = 0;
  while (Energy >= E[count]/2 && count < 11){
    count++;
  }

  Double_t Res = res[count-1]+(res[count]-res[count-1])*(Energy-E[count-1]/2)/(E[count]/2-E[count-1]/2);

  if ( Energy < 15 )
    Res = res[0];
  else if ( Energy > 250 )
    Res = res[11];
  
  if ( type == "energy" )
    Res = Res*Energy/100;
  //std::cout << "costheta: " << dbl-0.1 << "<" << costheta << "<" << dbl << std::endl;
  //std::cout << "Energy: " << E[count-1]/2 << "<" << Energy << "<" << E[count]/2 << std::endl;
  //std::cout << "Resolution: " << res[count-1] << "<" << Res << "<" << res[count] << std::endl;
  //std::cout << type << " Resolution: " << Res << std::endl;
  
  return Res;
}


// function to define the jet energy resolution (in GeV)
Double_t ZH1CFit::JetEnergyResolution(Double_t E)
{
  
  // examples here derived by Benjamin Hermberg from e+e- -> udsc:
  // 1) default is 120%/sqrt(E), gives best convergence of 1C fit on e+e- -> udsc
  Double_t result = _errene*std::sqrt(E);
  
  // 2) comparing jet-level to quark-level energies 
  //    (using MarlinReco/Analysis/RecoMCTruthLink/QuarkJetPairing.cc)
  if (_errene == 0 ) result = std::sqrt(pow(0.6908,2)*(E)+(pow(0.02596,2)*pow(E,2))); 
  
  return result;      
}

Double_t ZH1CFit::JetEnergyRes(Double_t E, Double_t costheta)
{
  Double_t result;

  if (_errene == 0)
    result = getResolution("energy",E,std::abs(costheta),_detector);
  else
    result = _errene*std::sqrt(E);
  
  return result;
}

Double_t ZH1CFit::JetThetaRes(Double_t E, Double_t costheta)
{
  Double_t result;

  if (_errtheta == 0)
    result = getResolution("theta",E,std::abs(costheta),_detector);
  else
    result = _errtheta;
  
return result;
}

Double_t ZH1CFit::JetPhiRes(Double_t E, Double_t costheta)
{
  Double_t result;

  if (_errphi == 0)
    result = getResolution("phi",E,std::abs(costheta),_detector);
  else
    result = _errphi;
  
  return result;
}


ZH1CFit::ZH1CFit() : Processor("ZH1CFit") {
  
  // modify processor description
  _description = "ZH1CFit does a 1C fit on 4 jet events (Px, Py, Pz, E, M12 = MZ (for all six permutations))" ;
  

  // register steering parameters: name, description, class-variable, default value

  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			   "InputJetsCollection" , 
			   "Name of the jets collection"  ,
			   _colJets ,
			   std::string("Refined_2Jet") ) ;
                           
  registerOutputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			    "OutputMarlinKinfitJetsCollection",
			    "Name of the jets collection after MarlinKinfit",
			    _colMKFJets,
			    std::string("MKF_2Jet") );

  registerProcessorParameter( "CenterOfMassEnergy" ,
                              "Center of mass energy",
                              _ecm,
                              (float)500.);
                              
  registerProcessorParameter( "FitISR" ,
                              "0: Fit hypothesis without ISR   1: Fit hypothesis including ISR",
                              _fitISR,
                              (int) 0);
                              
  registerProcessorParameter( "ISRPzMax" ,
                              "Maximum possible energy for a single ISR photon",
                              _isrpzmax,
                              (float)225.);

  registerOptionalParameter(  "DetectorModel",
			      "Name of detector model l4 s4 o1",
			      _detector,
			      std::string("l4") );

  registerProcessorParameter( "errene" ,
                              "assumed energy resolution for jets as x/sqrt(E) - if 0, then parametrisation is used",
                              _errene,
                              (double)0.);

  registerProcessorParameter( "errtheta" ,
                              "assumed theta resolution for jet axis",
                              _errtheta,
                              (double)0.);

  registerProcessorParameter( "errphi" ,
                              "assumed phi resolution for jet axis",
                              _errphi,
                              (double)0.);

  registerProcessorParameter( "gammaZ" ,
			      "natural width of Z boson",
			      _gammaZ,
			      (double)2.5);

  registerProcessorParameter( "fitter" ,
                              "0 = OPALFitter, 1 = NewFitter, 2 = NewtonFitter",
                              _ifitter,
                              (int)0);

  registerProcessorParameter( "JetConstraint" ,
                              "0 = massless, 1 = fixed beta, 2 = fixed mass",
                              _ijetpar,
                              (int)2);
                              
  registerProcessorParameter( "traceall" ,
                              "set true if every event should be traced",
                              _traceall,
                              (bool)false);
                              
  registerProcessorParameter( "ievttrace" ,
                              "number of individual event to be traced",
                              _ievttrace,
                              (int)0);
                              
}


void ZH1CFit::init() { 

  // usually a good idea to
  printParameters() ;

  _nRun = 0 ;
  _nEvt = 0 ;

  b = (Double_t) 0.00464564*( std::log(_ecm*_ecm*3814714.)-1. );
  //= 2*alpha/pi*( ln(s/m_e^2)-1 )
  ISRPzMaxB = std::pow((Double_t)_isrpzmax,b);
  
}

void ZH1CFit::processRunHeader( LCRunHeader* run) { 

  _nRun++ ;
} 

void ZH1CFit::processEvent( LCEvent * evt ) { 
  
  
  message<DEBUG>( log() 
		    << " processing event " << evt->getEventNumber() 
		    << "  in run "          << evt->getRunNumber() 
		    ) ;
  // this gets called for every event 
  // usually the working horse ...
  
  Int_t debug = 0;
  if ( evt->getEventNumber() == _ievttrace || _traceall) debug = 10;
  
  // fill histogram from LCIO data :

  //////////////////   JETS ///////////////////////////
  
  LCCollection* colJet = evt->getCollection( _colJets ) ;
  if (!colJet) {
    cerr << "No Jet Collection Found!" << endl;
    throw marlin::SkipEventException(this);
  }
  
  Int_t nJets = colJet->getNumberOfElements()  ;
  message<DEBUG>( log() 
		    << " found " << nJets
		    << " jets in event " << evt->getEventNumber() 
		    << "  in run "          << evt->getRunNumber() 
		    ) ;
  if (nJets != 2){
    cerr << "No: " << _nEvt << "  Number of Jets is not 2 : nJets =" << nJets << endl;
    throw marlin::SkipEventException(this);
  }
    
  //float yminus = colJet ->parameters().getFloatVal( "YMinus");              
  //message<DEBUG>( log()  << " yminus = " << yminus ) ;
    
  //float yplus = colJet ->parameters().getFloatVal( "YPlus");              
  //message<DEBUG>( log()  << " yplus = " << yplus ) ;
    
  // original fit objects - save for next permutation
  JetFitObject* j1 = 0;
  JetFitObject* j2 = 0;
  
  //ReconstructedParticle* jrps[2];
  ReconstructedParticleImpl* jout[2];
  //Double_t mom[3] = {10.,10.,10.};

  //HepLorentzVector lvec;
  TLorentzVector lvec[2];
  Double_t beta[2];
  Double_t mass[2];
  
  for(Int_t i=0; i< nJets ; i++){
         
    ReconstructedParticle* j = dynamic_cast<ReconstructedParticle*>( colJet->getElementAt( i ) ) ;
    //ReconstructedParticleImpl* jimpl = dynamic_cast<ReconstructedParticleImpl*>( colJet->getElementAt( i ) ) ;
    jout[i]  = new ReconstructedParticleImpl() ;
    
    if (j) {
      //jrps[i] = j;
      //jout[i] = jimpl;
      
      //lvec = HepLorentzVector ((j->getMomentum())[0],(j->getMomentum())[1],(j->getMomentum())[2],j->getEnergy());
      //lvec = getLorentzVector(j);
      if (i == 0) {
	lvec[0] = getLorentzVector(j);
	beta[0] = lvec[0].Beta();
	if (_ijetpar==0)
	  mass[0] = 0;
	else if (_ijetpar==1)
	  mass[0] = lvec[0].E()*std::sqrt(1-beta[0]*beta[0]);
	else if (_ijetpar==2)
	  mass[0] = lvec[0].M();
	else 
	  mass[0] = lvec[0].M();
	j1 = new JetFitObject (lvec[0].E(), lvec[0].Theta(), lvec[0].Phi(),
			       //JetEnergyResolution(lvec[0].E()), _errtheta, _errphi, lvec[0].M());
			       //JetEnergyRes(lvec[0].E(),lvec[0].CosTheta()), _errtheta, _errphi, lvec[0].M());
			       JetEnergyRes(lvec[0].E(),lvec[0].CosTheta()), JetThetaRes(lvec[0].E(),lvec[0].CosTheta()),
			       JetPhiRes(lvec[0].E(),lvec[0].CosTheta()), mass[0]);
	j1->setName("Jet1");
	message<DEBUG>( log()  << " start four-vector of first  jet: " << *j1  ) ;
      }
      else if (i == 1) { 
	lvec[1] = getLorentzVector(j);
	beta[1] = lvec[1].Beta();
	if (_ijetpar==0)
	  mass[1] = 0;
	else if (_ijetpar==1)
	  mass[1] = lvec[1].E()*std::sqrt(1-beta[1]*beta[1]);
	else if (_ijetpar==2)
	  mass[1] = lvec[1].M();
	else 
	  mass[1] = lvec[1].M();
	j2 = new JetFitObject (lvec[1].E(), lvec[1].Theta(), lvec[1].Phi(),
			       //JetEnergyResolution(lvec[1].E()), _errtheta, _errphi, lvec[1].M());
			       //JetEnergyRes(lvec[1].E(),lvec[1].CosTheta()), _errtheta, _errphi, lvec[1].M());
			       JetEnergyRes(lvec[1].E(),lvec[1].CosTheta()), JetThetaRes(lvec[1].E(),lvec[1].CosTheta()),
			       JetPhiRes(lvec[1].E(),lvec[1].CosTheta()), mass[1]);
	j2->setName("Jet2");
	message<DEBUG>( log() << " start four-vector of second  jet: " << *j2  ) ;
      }
      //jout[i]->setMass(0.);
      //jout[i]->setEnergy(10.);
      //jout[i]->setMomentum(mom);
      //message<DEBUG>( log() << "set Mass 0.  : " << jout[i]->getMass() ) ;
      //message<DEBUG>( log() << "set Energy 10.  : " << jout[i]->getEnergy() ) ;
      //message<DEBUG>( log() << "get Momentum : " << jout[i]->getMomentum() ) ;
    }else
      cerr<<"Couldn't convert jet collection to ReconstructedParticle !!"<<endl;
  }

  const Int_t NJETS = 2;
  // these don't get changed by the fit -> to obtain start values later!
  JetFitObject startjets[NJETS] = {*j1,*j2};
  for (Int_t i = 0; i < nJets; ++i)
    message<DEBUG>( log()  << "startjets[ " << i << "]: " << startjets[i]  ) ;
 
  // these get changed by the fit -> reset after each permutation!
  JetFitObject fitjets[NJETS] = {*j1, *j2};
  for (Int_t i = 0; i < nJets; ++i)
    message<DEBUG>( log()  << "fitjets[ " << i << "]: " << fitjets[i]  ) ;
 
  // these point allways to the fitjets array, which gets reset.
  /*JetFitObject *jets[NJETS];
  for (Int_t i = 0; i < nJets; ++i){
    jets[i] = &fitjets[i];
    message<DEBUG>( log()  << "start four-vector of jets[ " << i << "]: " << *(jets[i])  ) ;
    }*/
  
  Double_t bestprob = 0.;
  Int_t bestnit = 0;
  Double_t bestmass1 = 0., bestmass2 = 0.;
  Double_t beststartmassZ = 0., beststartmassH = 0.;
  Double_t startmassZ = 0., startmassH = 0.;
  Double_t bestphotonenergy = 0.;
  Int_t besterr = 999;
  Double_t bestzvalue = 10000.;
  Double_t chi2startmassZ = 0., chi2startmassH = 0.;
  
  // important: (re-)set fitjets array!
  //fitjets[0] = *j1;
  //fitjets[1] = *j2;
  
  //MomentumConstraint pxc (1, 0);
  // crossing angle 14 mrad : px = (14/2)/1000 *Ebeam *2
  MomentumConstraint pxc (0, 1, 0, 0, 1.75);
  pxc.setName("sum(p_x)");
  for (Int_t i = 0; i < nJets; ++i)
    pxc.addToFOList (fitjets[i]);
        
  MomentumConstraint pyc (0, 0, 1);
  pyc.setName("sum(p_y)");
  for (Int_t i = 0; i < nJets; ++i)
    pyc.addToFOList (fitjets[i]);
        
  MomentumConstraint pzc (0, 0, 0, 1);
  pzc.setName("sum(p_z)");
  for (Int_t i = 0; i < nJets; ++i)
    pzc.addToFOList (fitjets[i]);
       
  message<DEBUG>( log() << "ECM = " << _ecm  ); 
  MomentumConstraint ec(1, 0, 0, 0, _ecm);
  ec.setName("sum(E)");
  for (Int_t i = 0; i < nJets; ++i)
    ec.addToFOList (fitjets[i]);
       
  message<DEBUG>( log()  << "Value of pxc before fit: " << pxc.getValue() ) ;
  message<DEBUG>( log()  << "Value of pyc before fit: " << pyc.getValue() ) ;
  message<DEBUG>( log()  << "Value of pzc before fit: " << pzc.getValue() ) ;
  message<DEBUG>( log()  << "Value of ec before fit: " << ec.getValue() ) ;
                      

  // ISR Photon initialized with missing p_z
  ISRPhotonFitObject *photon = new ISRPhotonFitObject (0., 0., -pzc.getValue(), b, ISRPzMaxB);
       
  if(_fitISR){
    message<DEBUG>( log()  << "start four-vector of ISR photon: " << *(photon) ) ;
         
    pxc.addToFOList (*(photon));
    pyc.addToFOList (*(photon));
    pzc.addToFOList (*(photon));
    ec.addToFOList  (*(photon));
  }
  
  SoftBWMassConstraint z(_gammaZ,91.2);
  if(_gammaZ<0.1){
    delete &z;
    MassConstraint z(91.2);
  }
  z.addToFOList (fitjets[0], 1);
  z.addToFOList (fitjets[1], 1);
       
  //MassConstraint h(125.);
  //h.addToFOList (*(fitjets[2]), 1);
  //h.addToFOList (*(fitjets[3]), 1);
       
  startmassZ = z.getMass(1);
  //startmassH = h.getMass(1);
       
  message<DEBUG>( log() << "start mass of Z: " << startmassZ ) ;
  //message<DEBUG>( log() << "start mass of H: " << startmassH ) ;
                      
  BaseFitter *pfitter;
  if (_ifitter == 1) {
    pfitter = new NewFitterGSL();
    if (evt->getEventNumber()== _ievttrace || _traceall) (dynamic_cast<NewFitterGSL*>(pfitter))->setDebug (debug);
  }
  else if (_ifitter == 2) {
    pfitter = new NewtonFitterGSL();
    if (evt->getEventNumber()== _ievttrace || _traceall) (dynamic_cast<NewtonFitterGSL*>(pfitter))->setDebug (debug);
  }
  else {
    // OPALFitter has no method setDebug !
    pfitter = new OPALFitterGSL();
    if (evt->getEventNumber()== _ievttrace || _traceall) (dynamic_cast<OPALFitterGSL*>(pfitter))->setDebug (debug);
  }
  BaseFitter &fitter = *pfitter;
       
  TextTracer tracer (std::cout);
  if (evt->getEventNumber()== _ievttrace || _traceall) fitter.setTracer (tracer);
         
  for (Int_t i = 0; i < nJets; ++i)
    fitter.addFitObject (fitjets[i]);
  if(_fitISR){
    fitter.addFitObject (*(photon));
  }
  //fitter.addConstraint (pxc);
  //fitter.addConstraint (pyc);
  //fitter.addConstraint (pzc);
  //fitter.addConstraint (ec);
  fitter.addConstraint (z);
  // don't constrain Higgs mass, just use constraints for convenient mass calculation 
  //fitter.addConstraint (h);
       
  // initial value of Z mass constraint
  if (std::abs(startmassZ-91.2) < bestzvalue) {
    chi2startmassZ = startmassZ;
    bestzvalue = std::abs(startmassZ-91.2);
  }
  /*if (fabs(startmassZ-91.2) + fabs(startmassH-125.) < bestzvalue) {
    chi2startmassH = startmassH;
    bestzvalue = fabs(startmassZ-91.2) + fabs(startmassH-125.);
    }*/
       
  Double_t prob = fitter.fit();
  Double_t chi2 = fitter.getChi2();
  Int_t nit = fitter.getIterations();

  message<DEBUG>( log() << "fit probability = " << prob ) ;  
  message<DEBUG>( log() << "fit chi2 = " << chi2  ) ; 
  message<DEBUG>( log() << "error code: " << fitter.getError() ) ;
       
  for (Int_t i = 0; i < nJets; ++i) {
    message<DEBUG>( log()  << "final four-vector of jet " << i << ": " << fitjets[i]) ;
  }
  if(_fitISR){
    message<DEBUG>( log()  << "final four-vector of ISR photon: " << *(photon) ) ;
  }
  
  message<DEBUG>( log()  << "final mass of Z: " << z.getMass(1) ) ;
  //message<DEBUG>( log()  << "final mass of H: " << h.getMass(1) ) ;
       
  Int_t ierr = fitter.getError();
  TLorentzVector out[2];
  if ((besterr > 0 && ierr < besterr) || ( besterr < 0 && ierr == 0)) besterr = ierr;
  // ierr == -1 only means that error calculation for fitted parameters failed!
  //if (ierr <= 0) {
  Double_t pull[3][2];
    // require successfull error calculation for pulls!
  if (ierr == 0) {
    for (Int_t ifo = 0; ifo < 2; ifo++){
      Double_t start;
      Double_t fitted = 0;
      Double_t errfit, errmea, sigma;
      for (Int_t ipar = 0; ipar < 3; ipar++) {
	fitted = fitjets[ifo].getParam(ipar);  
	start  = startjets[ifo].getParam(ipar);  
	errfit = fitjets[ifo].getError(ipar);  
	errmea = startjets[ifo].getError(ipar);  
	sigma  = errmea*errmea-errfit*errfit;
	if (ipar==0){
	  out[ifo].SetPxPyPzE(1,1,1,fitted);
	  if(_ijetpar==0)
	    out[ifo].SetRho(fitted); //mass less jet
	  else if (_ijetpar==1)
	    out[ifo].SetRho(fitted*beta[ifo]); //beta constraint
	  else if (_ijetpar==2)
	    out[ifo].SetRho(fitjets[ifo].getP());
	}
	else if (ipar==1) out[ifo].SetTheta(fitted);
	else if (ipar==2) out[ifo].SetPhi(fitted);
	if (sigma > 0) {
	  sigma = sqrt(sigma);
	  pull[ipar][ifo] = (fitted - start)/sigma;
	}
	else {
	  pull[ipar][ifo] = -10000;
	}
      }
    }
    
    //           if (prob > bestprob && h.getMass(1) > 70 && w.getMass(1) < 150) {
    if (prob > bestprob) {
      bestprob = prob;
      bestnit  = nit;
      bestmassZ = z.getMass(1);
      //bestmassH = h.getMass(1);
      beststartmassZ = startmassZ;
      //beststartmassH = startmassH;
      if (_fitISR) bestphotonenergy = photon->getE();     
      /*if (ierr == 0) {
	for (Int_t ifo = 0; ifo < 2; ifo++){
	  hPullEJetBest->fill (pull[0][ifo]);
	  hPullThJetBest->fill(pull[1][ifo]);
	  hPullPhJetBest->fill(pull[2][ifo]);
	  }
      }
      else {
	message<WARNING>( log() << " ERROR CALCULATION FAILED for best permutation " 
			  << " in event " << evt->getEventNumber() 
			  << " for permutation " << iperm ) ;
	for (Int_t ifo = 0; ifo < 2; ifo++){
	  hPullEJetBest->fill (-6.);
	  hPullThJetBest->fill(-6.);
	  hPullPhJetBest->fill(-6.);
	}
	}*/
    }
  }
  else {
    message<DEBUG>( log() << "FIT ERROR = " << fitter.getError() 
		      << " in event " << evt->getEventNumber() ) ; 
		      //<< ", not filling histograms!"  ) ;
    message<DEBUG>( log()  << "start mass of Z: " << startmassZ) ;
    //message<DEBUG>( log()  << "start mass of H: " << startmassH ) ;
    message<DEBUG>( log()  << "final mass of Z: " << z.getMass(1) ) ;
    //message<DEBUG>( log()  << "final mass of H: " << h.getMass(1) ) ;
    //cerr << "ZH1CFit: Fitter error = " << ierr << " ! No: " << _nEvt << endl;
    out[0] = lvec[0];
    out[1] = lvec[1];
    //throw marlin::SkipEventException(this);
  }
  if(_fitISR)delete photon;
  

  message<DEBUG>( log() << "==============  end of fit for event " << evt->getEventNumber() <<  " ==============" ) ;
  message<DEBUG>( log()  << "min chi2 start mass of Z: " << chi2startmassZ ) ;
  //message<DEBUG>( log()  << "min chi2 start mass of H: " << chi2startmassH ) ;
  message<DEBUG>( log()  << "best start mass of Z: " << beststartmassZ ) ;
  //message<DEBUG>( log()  << "best start mass of H: " << beststartmassH ) ;
  message<DEBUG>( log()  << "best mass of Z: " << bestmassZ ) ;
  //message<DEBUG>( log()  << "best mass of H: " << bestmassH ) ;
  

  message<DEBUG>( log()  << "final mass of Z: " << z.getMass(1) ) ;


  delete j1;
  delete j2;

  Double_t momout[3];
  for (Int_t i=0;i<nJets;i++) {
    momout[0] = out[i].Px();
    momout[1] = out[i].Py();
    momout[2] = out[i].Pz();
    //cerr << "TLorentzVector::Rho() = " << out[i].Rho() << endl;
    //cerr << "TLorentzVector::E() = " << out[i].E() << endl;
    jout[i]->setMomentum(momout);
    jout[i]->setEnergy(out[i].E());
  }
  LCCollectionVec *pJetsAfterKFCollection = new LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  pJetsAfterKFCollection->setSubset(true);
  for (Int_t i=0;i<nJets;i++) {
    pJetsAfterKFCollection->addElement(dynamic_cast<ReconstructedParticle*>(jout[i]));
  }
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitProb",(float)prob);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitChi2",(float)chi2);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitErr" ,ierr);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitNite",nit);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitPullE1",(float)pull[0][0]);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitPullTheta1",(float)pull[1][0]);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitPullPhi1",(float)pull[2][0]);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitPullE2",(float)pull[0][1]);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitPullTheta2",(float)pull[1][1]);
  pJetsAfterKFCollection->parameters().setValue("ZH1CFitPullPhi2",(float)pull[2][1]);
  evt->addCollection(pJetsAfterKFCollection,_colMKFJets.c_str());

  _nEvt ++ ;
}



void ZH1CFit::check( LCEvent * evt ) { 
  // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void ZH1CFit::end(){ 

}

