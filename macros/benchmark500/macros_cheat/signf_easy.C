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

void signf_easy(Int_t iPol = -1,string detector="l5",string condition="full",Int_t cutmax=cutmax0,TString X=X0,Double_t binwidth=binwidth0,Double_t xmin=xmin0,Double_t xmax=xmax0) {

  Int_t nbin = (xmax-xmin)/binwidth;
  cerr << "nbin = " << nbin << endl;
  
  if (iPol != 1 && iPol !=0 && iPol != -1) {
    cerr << "Please use a correct Polarization:  0,1,-1" << endl;
    return;
  }

  TFile *result;
  if (iPol==-1) result = new TFile(Form("output_%s/%s/%s_cut%d_eLpR.root",detector.c_str(),condition.c_str(),X.Data(),cutmax));
  if (iPol== 1) result = new TFile(Form("output_%s/%s/%s_cut%d_eRpL.root",detector.c_str(),condition.c_str(),X.Data(),cutmax));

  TH1F *hS = (TH1F*)result->Get("hS_hinv");
  TH1F *hB = (TH1F*)result->Get("hB_all");
  if (nbin!=hS->GetXaxis()->GetNbins()){
    cerr << "hS->Rebin()" << endl;
    hS->Rebin();
  }
  if (nbin!=hB->GetXaxis()->GetNbins()){
    cerr << "hB->Rebin()" << endl;
    hB->Rebin();
  }
    
  Double_t nSigAll2 = 0.;
  for (Int_t i=1;i<nbin+1;i++) {
    Double_t xS = hS->GetBinCenter(i);
    Double_t xB = hB->GetBinCenter(i);
    if ( xS == xB ) {
      Double_t nSG = hS->GetBinContent(i);
      Double_t nBG = hB->GetBinContent(i);
      Double_t nSig = nSG+nBG > 0? nSG/TMath::Sqrt(nSG+nBG) : 0.;
      cerr << setw(2) << right << i
	   << ": " << setw(5) << xS << "[GeV]"
	   << "; nS = " << setw(7) << left << nSG
	   << "; nB = " << setw(7) << left << nBG
	   << "; Significance = " << setw(7) << left << nSig
	   << endl;
      nSigAll2 += nSig*nSig;
    }
    else{
      cerr << "Error: Bin Center is different between hS and hB!" << endl ;
      cerr << "xS = " << xS << ", xB = " << xB << endl;
      return;
    }
  }

  cerr << endl << "Result: combined significance = " << TMath::Sqrt(nSigAll2) << endl;
  cerr << "Result: upper limit = " << 16.5/TMath::Sqrt(nSigAll2) << " [%]" << endl;
}
