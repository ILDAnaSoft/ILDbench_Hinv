#include "global.h"
#include "TCut.h"

void readCutS(TCut &cut1, TCut &cut2, TCut &cut3, TCut &cut4, TCut &cut5, TCut &cut6, TCut &cut7, TCut &cut8, TCut &cut9, TCut &cut10, Int_t iPol = -1, Bool_t iKF= kTRUE);

void readCutS(TCut cut[], Int_t iPol = -1, Bool_t iKF = kTRUE) {

  cut[0] = "";
  TCut cut1,cut2,cut3,cut4,cut5,cut6,cut7,cut8,cut9,cut10;
  readCutS(cut1,cut2,cut3,cut4,cut5,cut6,cut7,cut8,cut9,cut10,iPol,iKF);

  cut[ 1]  = cut1;
  cut[ 2]  = cut2;
  cut[ 3]  = cut3;
  cut[ 4]  = cut4;
  cut[ 5]  = cut5;
  cut[ 6]  = cut6;
  cut[ 7]  = cut7;
  cut[ 8]  = cut8;
  cut[ 9]  = cut9;
  cut[10]  = cut10;

}


void readCutS(TCut &cut1, TCut &cut2, TCut &cut3, TCut &cut4, TCut &cut5, TCut &cut6, TCut &cut7, TCut &cut8, TCut &cut9, TCut &cut10, Int_t iPol, Bool_t iKF) {

  cut1  = "nlep==0" ;
  cut2  = "lrzZwoISRBSOverlay.Pt()>10&&abs(lrzZwoISRBSOverlay.M()-91)<30&&mrecwoISRBS>0&&mrecwoISRBS<400" ;
  cut3  = "npfo>15&&(ntrackj1+ntrackj2)>6&&ntrackj1>1&&ntrackj2>1";
  cut4  = "lrzZwoISRBSOverlay.Pt()>50&&lrzZwoISRBSOverlay.Pt()<250" ;
  cut5  = "lrzZwoISRBSOverlay.M()>80&&lrzZwoISRBSOverlay.M()<120" ;
  cut6  = "abs(lrzZwoISRBSOverlay.CosTheta())<0.98" ;
  if (iKF){
    cut7  = "mkfmrec>100&&mkfmrec<160" ;
    cut8  = "mkfprob>-0.5&&mkfchi2<500" ;
  }else{
    //cut7  = "mrecwoISRBS>80&&mrecwoISRBS<330" ;
    cut7  = "mrecwoISRBS>50&&mrecwoISRBS<300" ;
    //cut7  = "mrecwoISRBS>0&&mrecwoISRBS<250" ;
    cut8  = "flvq1mc<4" ;
  }
  if (iPol == -1) {
    if (iKF)
      cut9  = "bdtl > -0.0867" ;
    else
      //cut9  = "bdtlwokf > -0.0718" ;
      cut9  = "" ;
  }
  else if (iPol == 1) {
    if (iKF)
      cut9  = "bdtr > -0.1162" ;
    else
      //cut9  = "bdtrwokf > -0.0840" ;
      cut9  = "" ;
  }
  if (iKF)
    cut10 = "mkfmrec>120&&mkfmrec<140" ;
  else
    cut10 = "" ;

}
