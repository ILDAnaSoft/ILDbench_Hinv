#include <iomanip>
#include <iostream>
#include "global.h"
#include "readData.C"

void readNGen(Bool_t isHighStats = kFALSE,Bool_t iPrint = kTRUE) {

  readData(1);

  if (iPrint) cerr << "//Number of generated signal events ( N_hAnl - N_hGen )" << endl;
  for (Int_t i=1;i<nSignal;i++) {
    Int_t nevts = 0;
    if (hAnl_S[i]) {
      nevts = hAnl_S[i]->GetEntries();
    }
    Int_t ngens = 0;
    if (hGen_S[i]) {
      ngens = hGen_S[i]->GetEntries();
    }
    ngenS[i] = nevts;
    if (iPrint) cerr << "  ngenS[" << setw(2) << i << "] = " << setw(10) << ngens << ";" << " // " << nevts-ngens << endl;
  }

  if (iPrint)  cerr << "//Number of generated background events ( N_hAnl - N_hGen )" << endl;
  for (Int_t i=1;i<nBackground;i++) {
    Int_t nevts = 0;
    if (hAnl_B[i]) {
      nevts = hAnl_B[i]->GetEntries();
    }
    Int_t ngens = 0;
    if (hGen_B[i]) {
      ngens = hGen_B[i]->GetEntries();
    }
    ngenB[i] = nevts;
    if (iPrint)    cerr << "  ngenB[" << setw(2) << i << "] = " << setw(10) << ngens << ";" << " // " << nevts-ngens << endl;
  }

}
