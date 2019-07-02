#include "readPara.C"

void getXsec(Int_t iPol=-1){

  readPara(1,iPol,inteLumi);

  Double_t otherbkg = 0.;
  Int_t count = 0;
  
  for(Int_t i=1;i<nBackground;i++) {
    //nexpB[i] = xsecB[i]*factB[i]*intlumi;
    //weightB[i] = nexpB[i]/ngenB[i] ;

    //cout << "xsecB[" << i << "] = " << xsecB[i] << ", xsec*factB[" << i << "] = " << nexpB[i]/inteLumi << endl; 

    if(i!=13&&i!=14&&i!=19&&i!=20&&i!=27&&i!=28){
      //otherbkg += nexpB[i]/inteLumi;
      otherbkg += ngenB[i];
      count++;
    }
  }

  cout << "other bkg =" << otherbkg << endl;
  //cout << "count     =" << count << endl;
  
}
