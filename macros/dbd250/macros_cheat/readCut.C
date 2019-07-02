#include "global.h"
#include "TCut.h"
#include "TString.h"

void readCut_full(TCut &cut1, TCut &cut2, TCut &cut3, TCut &cut4, TCut &cut5, TCut &cut6, TCut &cut7, TCut &cut8, TCut &cut9, TCut &cut10, Int_t iPol);
void readCut_cheat(TCut &cut1, TCut &cut2, TCut &cut3, TCut &cut4, TCut &cut5, TCut &cut6, TCut &cut7, TCut &cut8, TCut &cut9, TCut &cut10, Int_t iPol);

void readCut(TCut cut[], Double_t xmin, Double_t xmax, Int_t iPol, Bool_t iCheat) {

  cut[0] = "";
  TCut cut1,cut2,cut3,cut4,cut5,cut6,cut7,cut8,cut9,cut10;
  if (iCheat)
    readCut_cheat(cut1,cut2,cut3,cut4,cut5,cut6,cut7,cut8,cut9,cut10,iPol);
  else
    readCut_full(cut1,cut2,cut3,cut4,cut5,cut6,cut7,cut8,cut9,cut10,iPol);
  
  cut[ 1]  = cut1;
  cut[ 2]  = cut2;
  cut[ 3]  = cut3;
  cut[ 4]  = cut4;
  cut[ 5]  = cut5;
  cut[ 6]  = cut6;
  if (iCheat)  
    cut[ 7]  = Form("mrecCheat>%.0f&&mrecCheat<%.0f",xmin,xmax);    
  else
    cut[ 7]  = Form("mrec>%.0f&&mrec<%.0f",xmin,xmax);    
  //cut[ 7]  = cut7;  
  cut[ 8]  = cut8;
  cut[ 9]  = cut9;
  cut[10]  = cut10;

}


void readCut_full(TCut &cut1, TCut &cut2, TCut &cut3, TCut &cut4, TCut &cut5, TCut &cut6, TCut &cut7, TCut &cut8, TCut &cut9, TCut &cut10, Int_t iPol) {

  cut1  = "nlep==0" ;
  cut2  = "lrzZ.Pt()>10&&abs(lrzZ.M()-91)<30&&mrec>50&&mrec<200" ;
  cut3  = "npfo>15&&(ntrackj1+ntrackj2)>6&&ntrackj1>1&&ntrackj2>1";
  cut4  = "lrzZ.Pt()>20&&lrzZ.Pt()<80" ;
  cut5  = "lrzZ.M()>80&&lrzZ.M()<100" ;
  cut6  = "abs(lrzZ.CosTheta())<0.9" ;
  cut7  = "mrec>100&&mrec<160" ;
  //cut7  = "mrec>50&&mrec<300" ;
  //cut7  = "mrec>0&&mrec<250" ;
  cut8  = "" ;
  
  if (iPol == -1) {
    //cut9  = "bdtl > -0.0867" ;
    cut9  = "" ;
  }
  else if (iPol == 1) {
    //cut9  = "bdtr > -0.1162" ;
    cut9  = "" ;
  }
  cut10 = "" ;

}

void readCut_cheat(TCut &cut1, TCut &cut2, TCut &cut3, TCut &cut4, TCut &cut5, TCut &cut6, TCut &cut7, TCut &cut8, TCut &cut9, TCut &cut10, Int_t iPol) {

  cut1  = "nlep==0" ;
  cut2  = "lrzZCheat.Pt()>10&&abs(lrzZCheat.M()-91)<30&&mrecCheat>50&&mrecCheat<200" ;
  cut3  = "npfo>15&&(ntrackj1+ntrackj2)>6&&ntrackj1>1&&ntrackj2>1";
  cut4  = "lrzZCheat.Pt()>20&&lrzZCheat.Pt()<80" ;
  cut5  = "lrzZCheat.M()>80&&lrzZCheat.M()<100" ;
  cut6  = "abs(lrzZCheat.CosTheta())<0.9" ;
  cut7  = "mrecCheat>100&&mrecCheat<160" ;
  cut8  = "" ;
  if (iPol == -1) {
    //cut9  = "bdtl > -0.0867" ;
    cut9  = "" ;
  }
  else if (iPol == 1) {
    //cut9  = "bdtr > -0.1162" ;
    cut9  = "" ;
  }
  cut10 = "" ;

}

