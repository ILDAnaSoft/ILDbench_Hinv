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
const Double_t binwidth0 = 5;
const Double_t xmin0 = 0.;
const Double_t xmax0 = 250.;
//const Int_t nbin = (xmax-xmin)/binwidth;
// ------------------------------------------

void signf_temp_new(Int_t iPol = -1,string l5s5="l5",Double_t binwidth=binwidth0,Double_t xmin=xmin0,Double_t xmax=xmax0) {

  Int_t nbin = (xmax-xmin)/binwidth;
  cerr << "nbin = " << nbin << endl;
  
  if (iPol != 1 && iPol !=0 && iPol != -1) {
    cerr << "Please use a correct Polarization:  0,1,-1" << endl;
    return;
  }

  TFile *result;
  if (iPol==-1) result = new TFile(Form("output_%s/cheatOZBI/mrec_cut7_eLpR.root",l5s5.c_str()));
  if (iPol== 1) result = new TFile(Form("output_%s/cheatOZBI/mrec_cut7_eRpL.root",l5s5.c_str()));
  TFile *temp;
  if (iPol==-1) temp = new TFile(Form("output_%s/cheatTemp/mrec_temp_eLpR.root",l5s5.c_str()));
  if (iPol== 1) temp = new TFile(Form("output_%s/cheatTemp/mrec_temp_eRpL.root",l5s5.c_str()));

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
