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
const Int_t cutmax0 = 8;
const Double_t binwidth0 = 5;
const Double_t xmin0 = 0.;
const Double_t xmax0 = 250.;
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
  nBG += (nS[1] + nS[2]);
  nSig = nSG0+nBG > 0? nSG/TMath::Sqrt(nSG0+nBG) : 0.;

  //  cutAll.Print();
  cerr << endl << "nS = " << nSG << "; nB = " << nBG << "; nS0 = " << nSG0 << "; Significance = " << nSig << endl;
  return nSig;
}

void signf_scaled(Int_t iPol = -1,TString X=X0,Double_t binwidth=binwidth0,Double_t xmin=xmin0,Double_t xmax=xmax0,string kinfit="Newton_jmass") {

  
  if (iPol != 1 && iPol !=0 && iPol != -1) {
    cerr << "Please use a correct Polarization:  0,1,-1" << endl;
    return;
  }

  TFile *result;
  if (iPol==-1) result = new TFile(Form("output/%s/%s_cut8_eLpR.root",kinfit.c_str(),X.Data()));
  if (iPol== 1) result = new TFile(Form("output/%s/%s_cut8_eRpL.root",kinfit.c_str(),X.Data()));
  TFile *ref;
  if (iPol==-1) ref = new TFile(Form("output/%s/%s_cut7_eLpR.root",kinfit.c_str(),X.Data()));
  if (iPol== 1) ref = new TFile(Form("output/%s/%s_cut7_eRpL.root",kinfit.c_str(),X.Data()));

  TH1F *hS = (TH1F*)result->Get("hS_hinv");
  TH1F *hS_ref = (TH1F*)ref->Get("hS_hinv");
  TH1F *hB = (TH1F*)result->Get("hB_all");

  Double_t NS = hS->Integral(hS->FindBin(xmin),hS->FindBin(xmax));
  Double_t NS_ref = hS_ref->Integral(hS_ref->FindBin(xmin),hS_ref->FindBin(xmax));
  
  //Double_t weight_S = hS_ref->GetEntries()/hS->GetEntries();
  Double_t weight_S = NS_ref/NS;
  
  Int_t nbin = (xmax-xmin)/binwidth;
  Double_t nSigAll2 = 0.;
  for (Int_t i=1;i<nbin+1;i++) {
    Double_t nSG = hS->GetBinContent(i)*weight_S;
    Double_t nBG = hB->GetBinContent(i);
    Double_t nSig = nSG+nBG > 0? nSG/TMath::Sqrt(nSG+nBG) : 0.;
    cerr << endl << "nS = " << nSG << "; nB = " << nBG << "; Significance = " << nSig << endl;

    nSigAll2 += nSig*nSig;
  }

  cerr << "combined significance = " << TMath::Sqrt(nSigAll2) << endl;
  cerr << "upper limit = " << 16.5/TMath::Sqrt(nSigAll2) << endl;
}
