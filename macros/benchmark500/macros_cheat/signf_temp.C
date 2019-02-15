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

void signf_temp(Int_t iPol = -1,string l5s5="l5",Double_t binwidth=binwidth0,Double_t xmin=xmin0,Double_t xmax=xmax0,string kinfit="Newton_jmass") {

  Int_t nbin = (xmax-xmin)/binwidth;  
  if (iPol != 1 && iPol !=0 && iPol != -1) {
    cerr << "Please use a correct Polarization:  0,1,-1" << endl;
    return;
  }

  TFile *result;
  if (iPol==-1) result = new TFile(Form("output_%s/%s/mrec_cut8_eLpR.root",l5s5.c_str(),kinfit.c_str()));
  if (iPol== 1) result = new TFile(Form("output_%s/%s/mrec_cut8_eRpL.root",l5s5.c_str(),kinfit.c_str()));
  TFile *temp;
  if (iPol==-1) temp = new TFile(Form("output_%s/%s/mrec_temp_eLpR.root",l5s5.c_str(),kinfit.c_str()));
  if (iPol== 1) temp = new TFile(Form("output_%s/%s/mrec_temp_eRpL.root",l5s5.c_str(),kinfit.c_str()));

  TH1F *hB = (TH1F*)result->Get("hB_all");
  TH1F *hS[9];
  for (Int_t j=0;j<9;j++) {
    hS[j] = (TH1F*)temp->Get(Form("hS%d",j));

    Double_t nSigAll2 = 0.;
    for (Int_t i=1;i<nbin+1;i++) {
      Double_t nSG = hS[j]->GetBinContent(i);
      Double_t nBG = hB->GetBinContent(i);
      Double_t nSig = nSG+nBG > 0? nSG/TMath::Sqrt(nSG+nBG) : 0.;
      //cerr << endl << "nS = " << nSG << "; nB = " << nBG << "; Significance = " << nSig << endl;

      nSigAll2 += nSig*nSig;
    }

    cerr << endl << "factor = " << 0.6+0.1*j << endl;
    cerr << "combined significance = " << TMath::Sqrt(nSigAll2) << endl;
    cerr << "upper limit = " << 16.5/TMath::Sqrt(nSigAll2) << endl;

  }
}
