// *****************************************************
// e+e- ------> Z H ------> q q inv
// Processor for final selection
//                        --------  Y.Kato
// *****************************************************
#include "MKFqqhinvAnalysisProcessor.h"
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
//#include "HelicityAngle.cc"

using namespace lcio ;
using namespace marlin ;
using namespace std;

using namespace mylib;


MKFqqhinvAnalysisProcessor aMKFqqhinvAnalysisProcessor ;

Double_t MKFqqhinvAnalysisProcessor::getCosHel(TLorentzVector particle, TLorentzVector parent,
	       TLorentzVector grandparent) {

  TVector3 boosttoparent = -(parent.BoostVector());

  particle.Boost(boosttoparent);
  grandparent.Boost(boosttoparent);

  TVector3 particle3 = particle.Vect();
  TVector3 grandparent3 = grandparent.Vect();
  Double_t numerator = particle3.Dot(grandparent3);
  Double_t denominator = (particle3.Mag())*(grandparent3.Mag());
  Double_t temp = numerator/denominator;

  return temp;

}


MKFqqhinvAnalysisProcessor::MKFqqhinvAnalysisProcessor() : Processor("MKFqqhinvAnalysisProcessor") {
  
  // modify processor description
  _description = "MKFqqhinvAnalysisProcessor does whatever it does ..." ;
  

  // register steering parameters: name, description, class-variable, default value

  registerInputCollection( LCIO::MCPARTICLE,
			   "InputMCParticlesCollection" , 
			   "Name of the MCParticle collection"  ,
			   _colMCP ,
			   std::string("MCParticle") ) ;

  /*registerInputCollection( LCIO::LCRELATION,
			   "InputMCTruthLinkCollection" , 
			   "Name of the MCTruthLink collection"  ,
			   _colMCTL ,
			   std::string("RecoMCTruthLink") ) ;*/

  registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			   "InputPandoraPFOsCollection" , 
			   "Name of the PandoraPFOs collection"  ,
			   _colPFOs ,
			   std::string("PandoraPFOs") ) ;

  /*registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE, 
			   "InputNewPFOsCollection",
			   "Name of the new PFOs collection after some pre-cuts",
			   _colNewPFOs,
			   std::string("NewPFOs_Uncluster") );*/

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

  registerProcessorParameter( "CenterOfMassEnergy",
			      "Center of mass energy"  ,
			      _ecm ,
			      float(500) ) ;
  
  /*registerOptionalParameter( "OutputRootFile",
			     "Name of output root file",
			     _outRootFile,
			     std::string("output.root") );*/
}

//TFile *output = 0;

void MKFqqhinvAnalysisProcessor::init() { 

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

void MKFqqhinvAnalysisProcessor::processRunHeader( LCRunHeader* run) { 
  _nRun++ ;
} 

void MKFqqhinvAnalysisProcessor::processEvent( LCEvent * evt ) { 
    
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

  if ( (_nEvt%10) == 0 )
    cerr << endl << "Hello, Analysis!" << " No: " << _nEvt << endl;

  /*static TNtupleD *hAnl = 0;
  if (!hAnl) {
    stringstream tupstr;
    tupstr << "ej1:ej2:cosj1:cosj2:ptz:cosz:mz:acop12:mrecoil" << ":"
	   << "cosj12:evis:evischg" << ":"
	   << "bprob1:bprob2:bmax1:bmax2" << ":"
	   << "yminus:yplus:yminus4:yplus4" << ":"
	   << "njet:npfo:ntrackj1:nneutralj1:ntrackj2:nneutralj2" << ":"
	   << "mmis:cosmis:pxj1:pyj1:pzj1:pxj2:pyj2:pzj2"
	   << ends;
    hAnl = new TNtupleD("hAnl","",tupstr.str().data());
    }*/

  // ------------------------------------------------------
  // -- New method to add TLorentzVector Branch in TTree --
  // ------------------------------------------------------

  TLorentzVector jmc[2];
  TLorentzVector lrzZmc;
  TLorentzVector j[2];
  TLorentzVector lrzZ;
  TLorentzVector mkf[2];
  TLorentzVector mkflrzZ;
  TLorentzVector lrzVis, lrzVisChg, lrzEcm, lrzMis;
  Int_t nlep, npfo, ntrackj1 ,ntrackj2, nneutralj1 ,nneutralj2;
  Double_t mrecmc, mrec, mkfmrec;
  Double_t acopmc, acop, mkfacop;
  Double_t cosj12mc, cosj12, mkfcosj12;
  Double_t coshelmc, coshel, mkfcoshel;
  Double_t prob,chi2,nite,ierr;
  
  static TTree *hAnl = 0;
  if (!hAnl){
    hAnl = new TTree("hAnl","");

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
    
    hAnl->Branch("lrzEcm" ,      &lrzEcm );
    hAnl->Branch("lrzVis" ,      &lrzVis );
    hAnl->Branch("lrzVisChg" ,   &lrzVisChg );
    hAnl->Branch("lrzMis" ,      &lrzMis );

    hAnl->Branch("nlep",       &nlep );
    hAnl->Branch("npfo",       &npfo );
    hAnl->Branch("ntrackj1",   &ntrackj1 );
    hAnl->Branch("nneutralj1", &nneutralj1 );
    hAnl->Branch("ntrackj2",   &ntrackj2 );
    hAnl->Branch("nneutralj2", &nneutralj2 );
    
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
    if ((pdg > 0 && pdg < 10) && motherpdg == 0 && ioverlay == 0) {
      jmc[0]  = lortz;
      lrzZmc += lortz;
    }
    if ((pdg > -10 && pdg < 0) && motherpdg == 0 && ioverlay == 0) {
      jmc[1]  = lortz;
      lrzZmc += lortz;
    }
    if (pdg == 25 && motherpdg == 0 && ioverlay == 0) {
      //lortzHMC = lortz;
    }
    if (i == 0) {
      //lortzISR1MC = lortz;
    }
    if (i == 1) {
      //lortzISR2MC = lortz;
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
  //  LCCollection *colPFO = evt->getCollection(_colPFOs);

  // ------------------------------------------------
  // -- read out the Thrust information
  // ------------------------------------------------
  //LCCollection *colSelRecPart = evt->getCollection("SelectedReconstructedParticle");
  //Double_t principleThrust = colSelRecPart->parameters().getFloatVal("principleThrustValue");
  //Double_t majorThrust = colSelRecPart->parameters().getFloatVal("majorThrustValue");
  //Double_t minorThrust = colSelRecPart->parameters().getFloatVal("minorThrustValue");
  //FloatVec tAxis;
  //FloatVec thrustAxis = colSelRecPart->parameters().getFloatVals("principleThrustAxis",tAxis);
  //TVector3 principleAxis = TVector3(thrustAxis[0],thrustAxis[1],thrustAxis[2]);
  //Double_t cosThrustAxis = principleAxis.CosTheta();

  // ------------------------------------------------
  // -- read out the PFOs information
  // ------------------------------------------------
  LCCollection *colNewPFO = evt->getCollection(_colPFOs);
  if (!colNewPFO) {
    cerr << "No PFOs Collection Found!" << endl;
    throw marlin::SkipEventException(this);
  }
  hStatAnl->Fill(++selid);
  gCutName[(Int_t)selid] << "PFOs Collection Found" << ends;
  // get the visible energy
  lrzVis.SetPxPyPzE(0.,0.,0.,0.);
  lrzVisChg.SetPxPyPzE(0.,0.,0.,0.);
  npfo = colNewPFO->getNumberOfElements();
  Int_t nParticles = 0;
  ReconstructedParticle *photonMax = 0;
  Double_t energyPhotonMax = -1.;
  for (Int_t i=0;i<npfo;i++) {
    ReconstructedParticle *pfo = dynamic_cast<ReconstructedParticle*>(colNewPFO->getElementAt(i));
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
  //  LCCollection *colMCTL = evt->getCollection(_colMCTL);
  //  LCRelationNavigator *navMCTL = new LCRelationNavigator(colMCTL);
  
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
  ntrackj1 = 0;
  ntrackj2 = 0;
  nneutralj1 = 0;
  nneutralj2 = 0;
  /*Int_t nPFOsCJ1 = 0, nPFOsCJ2 = 0; // # of charged pfos in a jet
  Int_t nPFOsNJ1 = 0, nPFOsNJ2 = 0; // # of neutral pfos in a jet
  // flavor tagging information
  PIDHandler pidh (colJet);
  Int_t algo = pidh.getAlgorithmID("lcfiplus");
  Double_t FLV[2][11];*/
  for (Int_t i=0;i<nJets;i++) {
    jets[i] = dynamic_cast<ReconstructedParticle*>(colJet->getElementAt(i));
    /*const ParticleID & jetID = pidh.getParticleID(jets[i], algo);
    FloatVec params = jetID.getParameters();
    FLV[i][0] = params[pidh.getParameterIndex(algo, "BTag")];
    FLV[i][1] = params[pidh.getParameterIndex(algo, "CTag")];
    FLV[i][2] = params[pidh.getParameterIndex(algo, "BCTag")];*/
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
  /*
  Int_t algo_y = pidh.getAlgorithmID("yth");
  const ParticleID & ythID = pidh.getParticleID(jets[0], algo_y);
  FloatVec params_y = ythID.getParameters();
  Double_t yMinus = params_y[pidh.getParameterIndex(algo_y, "y12")];
  Double_t yPlus  = params_y[pidh.getParameterIndex(algo_y, "y23")];
  Double_t yMinus4= params_y[pidh.getParameterIndex(algo_y, "y34")];
  Double_t yPlus4 = params_y[pidh.getParameterIndex(algo_y, "y45")];
  */

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

  ReconstructedParticle *MKFjets[2];
  for (Int_t i=0;i<nMKFJets;i++) 
    MKFjets[i] = dynamic_cast<ReconstructedParticle*>(colMKFJet->getElementAt(i));

  prob = (Double_t)colMKFJet->parameters().getFloatVal("ZH1CFitProb");
  chi2 = (Double_t)colMKFJet->parameters().getFloatVal("ZH1CFitChi2");
  ierr = (Int_t)colMKFJet->parameters().getIntVal("ZH1CFitErr");
  nite = (Int_t)colMKFJet->parameters().getIntVal("ZH1CFitNite");
  
  // ------------------------------------------------
  // -- get the useful physical quantities and save them to ntuple
  // ------------------------------------------------
  // ------------------------------------------------------
  // -- New method to add TLorentzVector Branch in TTree --
  // ------------------------------------------------------
  lrzEcm = getLorentzEcm(fEcm);
  lrzMis = lrzEcm - lrzVis;
  for (Int_t i=0;i<2;i++){ 
    j[i]   = getLorentzVector(jets[i]);
    mkf[i] = getLorentzVector(MKFjets[i]);
  }
  lrzZ = j[0] + j[1];
  mkflrzZ = mkf[0] + mkf[1];

  mrecmc = getRecoilMass(lrzEcm,lrzZmc);  
  mrec = getRecoilMass(lrzEcm,lrzZ);  
  mkfmrec = getRecoilMass(lrzEcm,mkflrzZ);

  acopmc  = getAcoPlanarity(jmc[0],jmc[1]);
  acop    = getAcoPlanarity(j[0],j[1]);
  mkfacop = getAcoPlanarity(mkf[0],mkf[1]);

  cosj12mc  = getCosTheta(jmc[0],jmc[1]);
  cosj12    = getCosTheta(j[0],j[1]);
  mkfcosj12 = getCosTheta(mkf[0],mkf[1]);

  coshelmc  = getCosHel(jmc[0],lrzZmc,lrzEcm);
  coshel    = getCosHel(j[0],lrzZ,lrzEcm);
  mkfcoshel = getCosHel(mkf[0],mkflrzZ,lrzEcm);
  
  // ------------------------------------------------------
  hAnl->Fill();

  //-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !

  //  streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber() 
  //		       << "   in run:  " << evt->getRunNumber() 
  //		       << std::endl ;

  //  _nEvt ++ ;

  last->cd();
}



void MKFqqhinvAnalysisProcessor::check( LCEvent * evt ) { 
  // nothing to check here - could be used to fill checkplots in reconstruction processor
}


void MKFqqhinvAnalysisProcessor::end(){ 

  cerr << "MKFqqhinvAnalysisProcessor::end()  " << name() 
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
