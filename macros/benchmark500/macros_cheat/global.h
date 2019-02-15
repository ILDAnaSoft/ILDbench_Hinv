#ifndef __GLOBAL__
#define __GLOBAL__

#include "TROOT.h"
#include "TCut.h"
#include "TTree.h"

const Double_t fEcm = 500.;
const Double_t inteLumi = 1600.;

const Double_t kMass_H  = 125.;
const Double_t BRinv = 0.1;
const string vILCSOFT = "v02-00-02";

const Int_t nSignal = 4 + 1;
const Int_t nBackground = 58 + 1;

const Int_t nCutMax = 10 + 1;
TCut myCutS[nCutMax];
TCut myCutB[nCutMax];
TFile *fS[nSignal], *fB[nBackground];
TTree *hAnl_S[nSignal],*hAnl_B[nBackground];
TTree *hGen_S[nSignal],*hGen_B[nBackground];

Double_t xsecS[nSignal],xsecB[nBackground];
Double_t nexpS[nSignal],nexpB[nBackground];
Double_t ngenS[nSignal],ngenB[nBackground];
Double_t weightS[nSignal],weightB[nBackground];

#endif
