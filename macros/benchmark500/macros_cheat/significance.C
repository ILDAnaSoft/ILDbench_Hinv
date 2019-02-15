#include <iostream>
#include <sstream>
#include <iomanip>
#include "readPara.C"
#include "readData.C"
#include "readCut.C"
#include "readCutS.C"
#include "global.h"

#include "TROOT.h"
#include "TNtuple.h"

#include "TMath.h"

using namespace std;

// parameter setting ------------------------
const TString X0 = "mrec";
const Int_t cutmax0 = 7;
const Double_t binwidth0 = 10;
const Double_t xmin0 = 50.;
const Double_t xmax0 = 300.;
//const Int_t nbin = (xmax-xmin)/binwidth;
// ------------------------------------------

Double_t getSignificance( TCut cutAllS, TCut cutAllB, TNtuple *hS[], TNtuple *hB[], Double_t wS[], Double_t wB[]) {

  // # of events after cut
  Double_t temp = 0. ;
  Double_t nS[nSignal],nB[nBackground];
  Double_t nSG0 = 0., nBG = 0.;
  for (Int_t i=3;i<nSignal;i++) {
    if (hS[i]) {
      temp = hS[i]->GetEntries(cutAllS);
    }
    else {
      temp = 0.;
    }
    nS[i] = wS[i]*temp;
    nSG0 += nS[i];
  }
  for (Int_t i=1;i<3;i++) {
    if (hS[i]) {
      temp = hS[i]->GetEntries(cutAllB);
    }
    else {
      temp = 0.;
    }
    nS[i] = wS[i]*temp;
    nBG += nS[i];
  }
  for (Int_t i=1;i<nBackground;i++) {
    if (hB[i]) {
      temp = hB[i]->GetEntries(cutAllB);
    }
    else {
      temp = 0.;
    }
    nB[i] = wB[i]*temp;
    nBG += nB[i];    
  }

  // significance
  Double_t nSG = 0., nSig = 0.;
 
  nSG = nS[3] + nS[4];
  nSig = nSG0+nBG > 0? nSG/TMath::Sqrt(nSG0+nBG) : 0.;

  //  cutAll.Print();
  cerr << endl << "nS = " << nSG << "; nB = " << nBG << "; nS0 = " << nSG0 << "; Significance = " << nSig << endl;
  return nSig;
}

void significance(Int_t iPol = -1,TString X=X0,Int_t cutmax=cutmax0,Double_t binwidth=binwidth0,Double_t xmin=xmin0,Double_t xmax=xmax0,string kinfit="Newton_jmass") {

  
  if (iPol != 1 && iPol !=0 && iPol != -1) {
    cerr << "Please use a correct Polarization:  0,1,-1" << endl;
    return;
  }

  // get cuts
  readCutS(myCutS,iPol,kFALSE);
  readCut(myCutB,iPol,kFALSE);

  // get ntuple data
  readData(1,kinfit,kTRUE);

  // get x-section, # of generated events, weight, etc.
  readPara(1,iPol,inteLumi);

  TCut cut0S = "";
  TCut cut0B = "";
  for (Int_t i=1;i<=cutmax;i++) cut0S += myCutS[i];
  for (Int_t i=1;i<=cutmax;i++) cut0B += myCutB[i];

  //Double_t xmin = 120;  
  //Double_t xmax = 140;  
  //Int_t nbin = 20;
  Int_t nbin = (xmax-xmin)/binwidth;
  Double_t nSigAll2 = 0.;
  for (Int_t i=0;i<nbin;i++) {
    Double_t xLo = xmin + (xmax-xmin)/nbin*i;
    Double_t xHi = xmin + (xmax-xmin)/nbin*(i+1);
    stringstream strcutS;
    strcutS << "mrecwoISROverlay > " << xLo << " && " << "mrecwoISROverlay <= " << xHi << ends;
    stringstream strcutB;
    strcutB << "mrec > " << xLo << " && " << "mrec <= " << xHi << ends;
    //    strcut << "mmis > " << xLo << " && " << "mmis <= " << xHi << ends;
    TCut cutMassS = strcutS.str().data();
    cutMassS.Print();
    TCut cutMassB = strcutB.str().data();
    cutMassB.Print();
    TCut cutAllS = cut0S + cutMassS;
    TCut cutAllB = cut0B + cutMassB;
    Double_t nSig = getSignificance( cutAllS, cutAllB, hAnl_S, hAnl_B, weightS, weightB);
    nSigAll2 += nSig*nSig;
  }

  cerr << "combined significance = " << TMath::Sqrt(nSigAll2) << endl;
}
