// *****************************************************
// e+e- ------> Z H ------> q q inv
// Processor for final selection
//                        --------  Y.Kato
// *****************************************************
#include "ILDBenchHinvAnalysisProcessor.h"
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
#include "UTIL/PIDHandler.h"

// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TNtupleD.h"
#include "TVector3.h"
#include "TMath.h"
#include "TString.h"
#include "TLorentzVector.h"

#include "Utilities.h"

using namespace lcio ;
using namespace marlin ;
using namespace std;

using namespace mylib;


ILDBenchHinvAnalysisProcessor aILDBenchHinvAnalysisProcessor ;

ILDBenchHinvAnalysisProcessor::ILDBenchHinvAnalysisProcessor() : Processor("ILDBenchHinvAnalysisProcessor") {
  
  // modify processor description
  _description = "ILDBenchHinvAnalysisProcessor does whatever it does ..." ;
  

  // register steering parameters: name, description, class-variable, default value

  registerInputCollection( LCIO::MCPARTICLE,
			   "InputMCParticlesCollection" , 
			   "Name of the MCParticle collection"  ,
			   _colMCP ,
			   std::string("MCParticle") ) ;

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
			   "InputCheatPFOsCollection",
			   "Name of the PFOs collection with Cheat",
			   _colPFOswoISROverlay ,
			   std::string("PFOsWithoutOverlayISR") );

  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			   "InputLeptonsCollection",
			   "Name of collection with the selected leptons",
			   _colLeptons,
			   std::string("Leptons") );

  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			   "InputJetsCollection",
			   "Name of the jets collection",
			   _colJets,
			   std::string("Refined_2Jet") );

  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			   "InputMarlinKinfitJetsCollection",
			   "Name of the jets collection after MarlinKinfit",
			   _colMKFJets,
			   std::string("MKF_2Jet") );

  registerProcessorParameter( "useMKF",
			      "0: w/o MKF,  1: w/ MKF"  ,
			      _useMKF ,
			      bool(0) ) ;

  registerProcessorParameter( "useCheat",
			      "0: full simulation,  1: w/ cheat"  ,
			      _useCheat ,
			      bool(0) ) ;

  registerProcessorParameter( "OverlayRemovalCheat",
			      "0: w/ Overlay,  1: w/o Overlay"  ,
			      _Ovly ,
			      bool(0) ) ;
  
  registerProcessorParameter( "HeavyQuarksMissingEnergyCheat",
			      "0: w/ Z->bb/cc,  1: w/o Z->bb/cc"  ,
			      _Zbc ,
			      bool(0) ) ;
  
  registerProcessorParameter( "BeamStrahlungCheat",
			      "0: w/ BS,  1: w/o BS"  ,
			      _BS ,
			      bool(0) ) ;

  registerProcessorParameter( "InitialStateRadiationCheat",
			      "0: w/ ISR,  1: w/o ISR"  ,
			      _ISR ,
			      bool(0) ) ;
  
  registerProcessorParameter( "VisibleInitialStateRadiationCheat",
			      "0: w/ visISR,  1: w/o visISR"  ,
			      _visISR ,
			      bool(0) ) ;
  
  registerProcessorParameter( "CenterOfMassEnergy",
			      "Center of mass energy"  ,
			      _ecm ,
			      double(500) ) ;
  
  registerProcessorParameter( "NHelloAnalysis",
			      "Interval of 'Hello, Analysis!'"  ,
			      _nHello ,
			      int(1000) ) ;

  /*registerOptionalParameter( "OutputRootFile",
			     "Name of output root file",
			     _outRootFile,
			     std::string("output.root") );*/
}

//TFile *output = 0;

void ILDBenchHinvAnalysisProcessor::init() { 
  
  streamlog_out(DEBUG) << "   init called  " 
		       << std::endl ;
  
  
  // usually a good idea to
  printParameters() ;
  
  _nRun = 0 ;
  _nEvt = 0 ;
  
  //stringstream out;
  //out << _outRootFile << ends;
  //output = new TFile(out.str().data(),"RECREATE");
  hStatAnl = 0;
}

void ILDBenchHinvAnalysisProcessor::processRunHeader( LCRunHeader* run) { 
  _nRun++ ;
} 

void ILDBenchHinvAnalysisProcessor::processEvent( LCEvent * evt ) { 
    
  // this gets called for every event 
  // usually the working horse ...
  _nEvt++;

  // some constants
  static const Double_t fEnergyCut = -0.05; // minimum energy of each pfo
  static const Double_t fEcm = _ecm;    // center-of-mass energy

  // initialize histo, ntuple...
  if (!hStatAnl) hStatAnl = new TH1D("hStatAnl", "Cut Table", 20, 0, 20);
  Double_t selid = -0.5;
  hStatAnl->Fill(++selid);
  gCutName[(Int_t)selid] << "No Cuts" << ends;

  TDirectory *last = gDirectory;
  gFile->cd("/");

  if ( (_nEvt%_nHello) == 0 )
    cerr << endl << "Hello, Analysis!" << " No: " << _nEvt << endl;

  // ------------------------------------------------------
  // -- New method to add TLorentzVector Branch in TTree --
  // ------------------------------------------------------

  Int_t qpdg[2];
  TLorentzVector jmc[2];
  TLorentzVector lrzZmc;
  TLorentzVector j[2];
  TLorentzVector lrzZ;
  TLorentzVector mkf[2];
  TLorentzVector mkflrzZ;
  TLorentzVector lrzVis, lrzVisChg, lrzMis;
  Int_t nlep, npfo, ntrackj1 ,ntrackj2, nneutralj1 ,nneutralj2;
  Double_t mrecmc, mrec, mkfmrec;
  Double_t acopmc, acop, mkfacop;
  Double_t cosj12mc, cosj12, mkfcosj12;
  Double_t coshelmc, coshel, mkfcoshel;
  Double_t prob,chi2,nite,ierr;

  Double_t FLV[2][3];
  Double_t b1st, b2nd, c1st, c2nd;
  Double_t y[4]; 
  
  TLorentzVector lrzHmc;
  TLorentzVector lrzLeftmc;
  Int_t nLeftmc = 0;
  vector<Double_t> ELeftmc;
  TLorentzVector lrzISRmc[2];
  TLorentzVector lrzPFOs;
  Double_t mrecPFOs;
  TLorentzVector lrzEcm = getLorentzEcm(fEcm);
  TLorentzVector lrzEcmCheat = getLorentzEcm(fEcm);

  TLorentzVector lrzISRvis;
  
  static TTree *hAnl = 0;
  if (!hAnl){
    hAnl = new TTree("hAnl","");
    
    
    hAnl->Branch("flvq1mc",      &qpdg[0] );
    hAnl->Branch("flvq2mc",      &qpdg[1] );
    hAnl->Branch("lrzj1mc",      &jmc[0] );
    hAnl->Branch("lrzj2mc",      &jmc[1] );
    hAnl->Branch("lrzZmc" ,      &lrzZmc );
    hAnl->Branch("mrecmc" ,      &mrecmc );
    hAnl->Branch("acopmc" ,      &acopmc );
    hAnl->Branch("cosj12mc" ,    &cosj12mc );
    hAnl->Branch("coshelmc" ,    &coshelmc );

    hAnl->Branch("lrzj1",      &j[0] );
    hAnl->Branch("lrzj2",      &j[1] );
    hAnl->Branch("lrzZ" ,      &lrzZ );
    hAnl->Branch("mrec" ,      &mrec );
    hAnl->Branch("acop" ,      &acop );
    hAnl->Branch("cosj12" ,    &cosj12 );
    hAnl->Branch("coshel" ,    &coshel );

    hAnl->Branch("lrzISR1mc",      &lrzISRmc[0] );
    hAnl->Branch("lrzISR2mc",      &lrzISRmc[1] );
    hAnl->Branch("lrzHmc" ,      &lrzHmc );
    hAnl->Branch("lrzLeftmc" ,      &lrzLeftmc );    
    hAnl->Branch("nLeftmc" ,      &nLeftmc );    
    hAnl->Branch("ELeftmc" ,      &ELeftmc );    
    if (_useCheat){
      hAnl->Branch("lrzZCheat" ,      &lrzPFOs );
      hAnl->Branch("mrecCheat" ,      &mrecPFOs );    
      hAnl->Branch("lrzEcmCheat" ,      &lrzEcmCheat );
      if (_visISR)
	hAnl->Branch("lrzISRvis" ,      &lrzISRvis );
    }
    hAnl->Branch("lrzVis" ,      &lrzVis );
    hAnl->Branch("lrzVisChg" ,   &lrzVisChg );
    hAnl->Branch("lrzMis" ,      &lrzMis );

    hAnl->Branch("nlep",       &nlep );
    hAnl->Branch("npfo",       &npfo );
    hAnl->Branch("ntrackj1",   &ntrackj1 );
    hAnl->Branch("nneutralj1", &nneutralj1 );
    hAnl->Branch("ntrackj2",   &ntrackj2 );
    hAnl->Branch("nneutralj2", &nneutralj2 );

    hAnl->Branch("bprob1",       &FLV[0][0] );
    hAnl->Branch("bprob2",       &FLV[1][0] );
    hAnl->Branch("cprob1",       &FLV[0][1] );
    hAnl->Branch("cprob2",       &FLV[1][1] );
    hAnl->Branch("b1st",         &b1st );
    hAnl->Branch("b2nd",         &b2nd );
    hAnl->Branch("c1st",         &c1st );
    hAnl->Branch("c2nd",         &c2nd );
    hAnl->Branch("y12",          &y[0] );
    hAnl->Branch("y23",          &y[1] );
    hAnl->Branch("y34",          &y[2] );
    hAnl->Branch("y45",          &y[3] );
    
    if (_useMKF){
      hAnl->Branch("mkflrzj1",   &mkf[0] );
      hAnl->Branch("mkflrzj2",   &mkf[1] );
      hAnl->Branch("mkflrzZ",    &mkflrzZ );
      hAnl->Branch("mkfmrec",    &mkfmrec );
      hAnl->Branch("mkfacop" ,   &mkfacop );
      hAnl->Branch("mkfcosj12" , &mkfcosj12 );
      hAnl->Branch("mkfcoshel" , &mkfcoshel );
      hAnl->Branch("mkfprob" ,      &prob );
      hAnl->Branch("mkfchi2" ,      &chi2 );
      hAnl->Branch("mkferr" ,       &ierr );
      hAnl->Branch("mkfnite" ,      &nite );
    }
  }
  // ------------------------------------------------------

  // ------------------------------------------------
  // -- read out the MCParticles information
  // ------------------------------------------------
  LCCollection *colMC = evt->getCollection(_colMCP);
  // get the truth information
  Int_t nMCP = colMC->getNumberOfElements();
  //TLorentzVector lortzJ1MC,lortzJ2MC,lortzZMC,lortzHMC,lortzISRMC;
  //TLorentzVector lortzISR1MC, lortzISR2MC;
  lrzZmc.SetPxPyPzE(0.,0.,0.,0.);
  lrzLeftmc.SetPxPyPzE(0.,0.,0.,0.);
  for (Int_t i=0;i<nMCP;i++) {
    MCParticle *mcPart = dynamic_cast<MCParticle*>(colMC->getElementAt(i));
    Int_t pdg = mcPart->getPDG();
    Int_t nparents = mcPart->getParents().size();
    Int_t motherpdg = 0;
    if (nparents > 0) {
      MCParticle *mother = mcPart->getParents()[0];
      motherpdg = mother->getPDG();
    }
    Double_t energy = mcPart->getEnergy();
    TVector3 pv = TVector3(mcPart->getMomentum());
    Int_t ndaughters = mcPart->getDaughters().size();
    Int_t daughterpdg = 0;
    if (ndaughters > 0) {
      MCParticle *daughter = mcPart->getDaughters()[0];
      daughterpdg = daughter->getPDG();
    }
    TLorentzVector lortz = TLorentzVector(pv,energy);
    Int_t ioverlay = mcPart->isOverlay()? 1 : 0;
    Int_t ileft = mcPart->hasLeftDetector()? 1 : 0;
    Int_t igenstat = mcPart->getGeneratorStatus()? mcPart->getGeneratorStatus() : 0;
    if ((pdg > 0 && pdg < 10) && motherpdg == 0 && ioverlay == 0) {
      qpdg[0] = pdg;
      jmc[0]  = lortz;
      lrzZmc += lortz;
    }
    if ((pdg > -10 && pdg < 0) && motherpdg == 0 && ioverlay == 0) {
      qpdg[1] = pdg;
      jmc[1]  = lortz;
      lrzZmc += lortz;
    }
    if (pdg == 25 && motherpdg == 0 && ioverlay == 0) {
      lrzHmc = lortz;
    }
    if (i == 0) {
      lrzISRmc[0] = lortz;
    }
    if (i == 1) {
      lrzISRmc[1] = lortz;
    }
    if ((TMath::Abs(pdg) == 12 || TMath::Abs(pdg) == 14 || TMath::Abs(pdg) == 16 || TMath::Abs(pdg) == 18 )
	&& ((_colMCP == "MCParticle" && ileft == 1) || (_colMCP == "MCParticlesSkimmed")) && ioverlay == 0 && igenstat == 1 && motherpdg != 23 ) {
      lrzLeftmc += lortz;
      ELeftmc.emplace_back(lortz.E());
      nLeftmc++;
    }
  }
  // get Higgs decay modes
  /*std::vector<Int_t> nHDecay;
  nHDecay = getHiggsDecayModes(colMC);
  Double_t nHbb = nHDecay[0]; // H---> b b
  Double_t nHWW = nHDecay[1]; // H---> W W
  Double_t nHgg = nHDecay[2]; // H---> g g
  Double_t nHtt = nHDecay[3]; // H---> tau tau
  Double_t nHcc = nHDecay[4]; // H---> c c
  Double_t nHZZ = nHDecay[5]; // H---> Z Z
  Double_t nHaa = nHDecay[6]; // H---> gam gam
  Double_t nHmm = nHDecay[7]; // H---> mu mu
  */

  // ------------------------------------------------
  // -- read out the PandoraPFOs information
  // ------------------------------------------------
  LCCollection *colPandoraPFO = evt->getCollection(_colPFOs);
  if (!colPandoraPFO) {
    cerr << "No PandoraPFOs Collection Found!" << endl;
    throw marlin::SkipEventException(this);
  }
  hStatAnl->Fill(++selid);
  gCutName[(Int_t)selid] << "PandoraPFOs Collection Found" << ends;
  // get the visible energy
  lrzVis.SetPxPyPzE(0.,0.,0.,0.);
  lrzVisChg.SetPxPyPzE(0.,0.,0.,0.);
  npfo = colPandoraPFO->getNumberOfElements();
  Int_t nParticles = 0;
  ReconstructedParticle *photonMax = 0;
  Double_t energyPhotonMax = -1.;
  for (Int_t i=0;i<npfo;i++) {
    ReconstructedParticle *pfo = dynamic_cast<ReconstructedParticle*>(colPandoraPFO->getElementAt(i));
    lrzVis += getLorentzVector(pfo);
    if (pfo->getEnergy() >= fEnergyCut) nParticles++;
    // find the photon with largest energy
    if (TMath::Abs(pfo->getCharge()) < 0.5) {
      Int_t leptonID = getLeptonID(pfo);
      if (leptonID == 22 && pfo->getEnergy() > energyPhotonMax) {
	energyPhotonMax = pfo->getEnergy();
	photonMax = pfo;
      }
    }
    else {
      lrzVisChg += getLorentzVector(pfo);
    }
  }

  // ------------------------------------------------
  // -- read out the MCTruthLink information
  // ------------------------------------------------
  LCCollection *colMCTL = evt->getCollection(_colMCTL);
  MCParticle *isr1 = dynamic_cast<MCParticle *>(colMC->getElementAt(0));
  MCParticle *isr2 = dynamic_cast<MCParticle *>(colMC->getElementAt(1));
  lrzISRvis.SetPxPyPzE(0.,0.,0.,0.);
  
  // ------------------------------------------------
  // -- read out the cheatPFOs information
  // ------------------------------------------------
  if (_useCheat){
    LCCollection *colcheatPFO = evt->getCollection(_colPFOswoISROverlay);
    if (!colcheatPFO) {
      cerr << "No cheatPFOs Collection Found!" << endl;
      throw marlin::SkipEventException(this);
    }
    hStatAnl->Fill(++selid);
    gCutName[(Int_t)selid] << "cheatPFOs Collection Found" << ends;
    // get the visible energy
    lrzPFOs.SetPxPyPzE(0.,0.,0.,0.);
    Int_t nPFOs = colcheatPFO->getNumberOfElements();
    Int_t nParticlescheat = 0;
    for (Int_t i=0;i<nPFOs;i++) {
      ReconstructedParticle *cpfo = dynamic_cast<ReconstructedParticle*>(colcheatPFO->getElementAt(i));
      lrzPFOs += getLorentzVector(cpfo);
      if (cpfo->getEnergy() >= fEnergyCut) nParticlescheat++;
      if (_visISR){
	MCParticle *mcp = getMCParticle(cpfo,colMCTL);
	if (mcp && (isFromMCParticle(mcp,isr1) || isFromMCParticle(mcp,isr2)))
	  lrzISRvis += getLorentzVector(cpfo);
      }
    }
    if (_Zbc)
      lrzPFOs += lrzLeftmc;
    if (_visISR)
      lrzPFOs -= lrzISRvis;
    if (_BS)
      lrzEcmCheat = lrzHmc + lrzZmc + lrzISRmc[0] + lrzISRmc[1];
    if (_ISR)
      lrzEcmCheat = lrzEcmCheat - lrzISRmc[0] - lrzISRmc[1];
    if (_visISR)
      lrzEcmCheat = lrzEcmCheat - lrzISRvis;

    mrecPFOs = getRecoilMass(lrzEcmCheat,lrzPFOs);
  }
  
  // ------------------------------------------------
  // -- read out the Leptons information
  // ------------------------------------------------
  LCCollection *colLep = evt->getCollection(_colLeptons);
  nlep = colLep? colLep->getNumberOfElements() : 0;

  // ------------------------------------------------
  // -- read out the Jets information
  // ------------------------------------------------
  LCCollection *colJet = evt->getCollection(_colJets);
  if (!colJet) {
    cerr << "No Jet Collection Found!" << endl;
    throw marlin::SkipEventException(this);
  }
  hStatAnl->Fill(++selid);
  gCutName[(Int_t)selid] << "Jet Collection Found" << ends;
  Int_t nJets = colJet->getNumberOfElements();
  if (nJets != 2) {
    cerr << "No: " << _nEvt << "  Number of Jets is not 2 : nJets =" << nJets << endl;
    throw marlin::SkipEventException(this);
  }
  hStatAnl->Fill(++selid);
  gCutName[(Int_t)selid] << "2 Jets Found" << ends;

  ReconstructedParticle *jets[2];
  ntrackj1 = 0; // # of charged pfos in a jet
  ntrackj2 = 0;
  nneutralj1 = 0; // # of neutral pfos in a jet
  nneutralj2 = 0;
  // flavor tagging information
  PIDHandler pidh (colJet);
  Int_t algo = pidh.getAlgorithmID("lcfiplus");
  //Double_t FLV[2][3];
  for (Int_t i=0;i<nJets;i++) {
    jets[i] = dynamic_cast<ReconstructedParticle*>(colJet->getElementAt(i));
    const ParticleID & jetID = pidh.getParticleID(jets[i], algo);
    FloatVec params = jetID.getParameters();
    FLV[i][0] = params[pidh.getParameterIndex(algo, "BTag")];
    FLV[i][1] = params[pidh.getParameterIndex(algo, "CTag")];
    FLV[i][2] = params[pidh.getParameterIndex(algo, "BCTag")];  
    std::vector<lcio::ReconstructedParticle*> partVec = jets[i]->getParticles();
    for (std::vector<lcio::ReconstructedParticle*>::const_iterator iPart=partVec.begin();iPart!=partVec.end();++iPart) {
      TVector3 momPart = TVector3((*iPart)->getMomentum());
      Double_t pTPart = momPart.Pt();
      if ((*iPart)->getCharge() != 0 && i == 0) ntrackj1++; //&& pTPart > 0.5
      else if ((*iPart)->getCharge() == 0 && i == 0) nneutralj1++; 
      if ((*iPart)->getCharge() != 0 && i == 1) ntrackj2++;
      else if ((*iPart)->getCharge() == 0 && i == 1) nneutralj2++; 
    }
  }
  b1st = FLV[0][0] > FLV[1][0] ? FLV[0][0] : FLV[1][0] ;
  b2nd = FLV[0][0] > FLV[1][0] ? FLV[1][0] : FLV[0][0] ;
  c1st = FLV[0][1] > FLV[1][1] ? FLV[0][1] : FLV[1][1] ;
  c2nd = FLV[0][1] > FLV[1][1] ? FLV[1][1] : FLV[0][1] ;
  
  Int_t algo_y = pidh.getAlgorithmID("yth");
  const ParticleID & ythID = pidh.getParticleID(jets[0], algo_y);
  FloatVec params_y = ythID.getParameters();
  y[0] = params_y[pidh.getParameterIndex(algo_y, "y12")];
  y[1] = params_y[pidh.getParameterIndex(algo_y, "y23")];
  y[2] = params_y[pidh.getParameterIndex(algo_y, "y34")];
  y[3] = params_y[pidh.getParameterIndex(algo_y, "y45")];
  
  // ------------------------------------------------
  // -- read out the MKFJets information
  // ------------------------------------------------
  ReconstructedParticle *MKFjets[2];
  if (_useMKF){
    LCCollection *colMKFJet = evt->getCollection(_colMKFJets);
    if (!colMKFJet) {
      cerr << "No MKFJet Collection Found!" << endl;
      throw marlin::SkipEventException(this);
    }
    hStatAnl->Fill(++selid);
    gCutName[(Int_t)selid] << "MKFJet Collection Found" << ends;
    Int_t nMKFJets = colMKFJet->getNumberOfElements();
    if (nMKFJets != 2) {
      cerr << "Number of MKFJets is not 2" << endl;
      throw marlin::SkipEventException(this);
    }
    hStatAnl->Fill(++selid);
    gCutName[(Int_t)selid] << "2 MKFJets Found" << ends;
  
    for (Int_t i=0;i<nMKFJets;i++) 
      MKFjets[i] = dynamic_cast<ReconstructedParticle*>(colMKFJet->getElementAt(i));
  
    prob = (Double_t)colMKFJet->parameters().getFloatVal("ZH1CFitProb");
    chi2 = (Double_t)colMKFJet->parameters().getFloatVal("ZH1CFitChi2");
    ierr = (Int_t)colMKFJet->parameters().getIntVal("ZH1CFitErr");
    nite = (Int_t)colMKFJet->parameters().getIntVal("ZH1CFitNite");
  }
    
  // ------------------------------------------------
  // -- get the useful physical quantities and save them to ntuple
  // ------------------------------------------------
  // ------------------------------------------------------
  // -- New method to add TLorentzVector Branch in TTree --
  // ------------------------------------------------------
  //lrzEcm = getLorentzEcm(fEcm);
  lrzMis = lrzEcm - lrzVis;
  for (Int_t i=0;i<2;i++){ 
    j[i]   = getLorentzVector(jets[i]);
    if (_useMKF) mkf[i] = getLorentzVector(MKFjets[i]);
  }
  lrzZ = j[0] + j[1];
  if (_useMKF) mkflrzZ = mkf[0] + mkf[1];

  mrecmc = getRecoilMass(lrzEcm,lrzZmc);  
  mrec = getRecoilMass(lrzEcm,lrzZ);  
  if (_useMKF) mkfmrec = getRecoilMass(lrzEcm,mkflrzZ);

  acopmc  = getAcoPlanarity(jmc[0],jmc[1]);
  acop    = getAcoPlanarity(j[0],j[1]);
  if (_useMKF) mkfacop = getAcoPlanarity(mkf[0],mkf[1]);

  cosj12mc  = getCosTheta(jmc[0],jmc[1]);
  cosj12    = getCosTheta(j[0],j[1]);
  if (_useMKF) mkfcosj12 = getCosTheta(mkf[0],mkf[1]);

  coshelmc  = getCosHel(jmc[0],lrzZmc,lrzEcm);
  coshel    = getCosHel(j[0],lrzZ,lrzEcm);
  if (_useMKF) mkfcoshel = getCosHel(mkf[0],mkflrzZ,lrzEcm);
  
  // ------------------------------------------------------
  hAnl->Fill();

  //-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !
  
  //  streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber() 
  //		       << "   in run:  " << evt->getRunNumber() 
  //		       << std::endl ;

  //  _nEvt ++ ;

  last->cd();
}



void ILDBenchHinvAnalysisProcessor::check( LCEvent * evt ) { 
  // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void ILDBenchHinvAnalysisProcessor::end(){ 

  cerr << "ILDBenchHinvAnalysisProcessor::end()  " << name() 
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
  for (int id=0; id<20 && gCutName[id].str().data()[0]; id++) {
    cerr << "  " << setw( 3) << id
         << "  " << setw(10) << static_cast<int>(hStatAnl->GetBinContent(id+1))
         << "  : " << gCutName[id].str().data() << endl;
  }
  cerr << "  -----------------------------------------------------------" << endl;

  //output->Write();
  //output->Close();
}
