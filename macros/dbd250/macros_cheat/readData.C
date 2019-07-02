#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "global.h"

void readData(Int_t mode, string readdir="full", Bool_t isCheatSignal = kFALSE) {

  if (mode == 0) { // for only main bkg
    //signal
    //fS[ 1]   = new TFile("data/qqh_lr.root");
    //fS[ 2]   = new TFile("data/qqh_rl.root");
    fS[ 3]   = new TFile(Form("data/%s/qqh_zz_4n_lr.root",readdir.c_str()));
    fS[ 4]   = new TFile(Form("data/%s/qqh_zz_4n_rl.root",readdir.c_str()));
     
    //all bkg
    fB[13]   = new TFile(Form("data/%s/4f_zz_sl_lr.root",readdir.c_str()));//pseudo signal(main bkg)
    fB[14]   = new TFile(Form("data/%s/4f_zz_sl_rl.root",readdir.c_str()));//pseudo signal(main bkg)
    fB[19]   = new TFile(Form("data/%s/4f_ww_sl_lr.root",readdir.c_str()));//pseudo signal(main bkg)
    fB[20]   = new TFile(Form("data/%s/4f_ww_sl_rl.root",readdir.c_str()));//pseudo signal(main bkg)
    fB[27]   = new TFile(Form("data/%s/4f_sznu_sl_lr.root",readdir.c_str()));//pseudo signal(main bkg)
    fB[28]   = new TFile(Form("data/%s/4f_sznu_sl_rl.root",readdir.c_str()));//pseudo signal(main bkg)
  }
  if (mode == 1) { // for all bkg data
    //signal
    fS[ 1]   = new TFile("data_final/qqh_lr.root");
    fS[ 2]   = new TFile("data_final/qqh_rl.root");
    if (isCheatSignal){
      fS[ 3]   = new TFile("data_cheat/qqh_zz_4n_lr.root");
      fS[ 4]   = new TFile("data_cheat/qqh_zz_4n_rl.root");
    }else{
      fS[ 3]   = new TFile("data_final/qqh_zz_4n_lr.root");
      fS[ 4]   = new TFile("data_final/qqh_zz_4n_rl.root");
    }
    //all bkg
    fB[ 1]   = new TFile("data_final/2f_z_bhabhag_ll.root");//no
    fB[ 2]   = new TFile("data_final/2f_z_bhabhag_lr.root");
    fB[ 3]   = new TFile("data_final/2f_z_bhabhag_rl.root");
    fB[ 4]   = new TFile("data_final/2f_z_bhabhag_rr.root");
    fB[ 5]   = new TFile("data_final/2f_z_l_lr.root");//no
    fB[ 6]   = new TFile("data_final/2f_z_l_rl.root");
    fB[ 7]   = new TFile("data_final/2f_z_h_lr.root");//few
    fB[ 8]   = new TFile("data_final/2f_z_h_rl.root");
    fB[ 9]   = new TFile("data_final/4f_zz_l_lr.root");//no
    fB[10]   = new TFile("data_final/4f_zz_l_rl.root");
    fB[11]   = new TFile("data_final/4f_zz_h_lr.root");//few
    fB[12]   = new TFile("data_final/4f_zz_h_rl.root");
    fB[13]   = new TFile("data_final/4f_zz_sl_lr.root");//pseudo signal(main bkg)
    fB[14]   = new TFile("data_final/4f_zz_sl_rl.root");//pseudo signal(main bkg)
    fB[15]   = new TFile("data_final/4f_ww_l_lr.root");//no
    fB[16]   = new TFile("data_final/4f_ww_l_rl.root");
    fB[17]   = new TFile("data_final/4f_ww_h_lr.root");//a few
    fB[18]   = new TFile("data_final/4f_ww_h_rl.root");
    fB[19]   = new TFile("data_final/4f_ww_sl_lr.root");//pseudo signal(main bkg)
    fB[20]   = new TFile("data_final/4f_ww_sl_rl.root");//pseudo signal(main bkg)
    fB[21]   = new TFile("data_final/4f_zzorww_l_lr.root");//no
    fB[22]   = new TFile("data_final/4f_zzorww_l_rl.root");
    fB[23]   = new TFile("data_final/4f_zzorww_h_lr.root");//a few
    fB[24]   = new TFile("data_final/4f_zzorww_h_rl.root");
    fB[25]   = new TFile("data_final/4f_sznu_l_lr.root");//no
    fB[26]   = new TFile("data_final/4f_sznu_l_rl.root");
    fB[27]   = new TFile("data_final/4f_sznu_sl_lr.root");//pseudo signal(main bkg)
    fB[28]   = new TFile("data_final/4f_sznu_sl_rl.root");//pseudo signal(main bkg)
    fB[29]   = new TFile("data_final/4f_sze_l_ll.root");//no
    fB[30]   = new TFile("data_final/4f_sze_l_lr.root");
    fB[31]   = new TFile("data_final/4f_sze_l_rl.root");
    fB[32]   = new TFile("data_final/4f_sze_l_rr.root");
    fB[33]   = new TFile("data_final/4f_sze_sl_ll.root");//few
    fB[34]   = new TFile("data_final/4f_sze_sl_lr.root");
    fB[35]   = new TFile("data_final/4f_sze_sl_rl.root");
    fB[36]   = new TFile("data_final/4f_sze_sl_rr.root");
    fB[37]   = new TFile("data_final/4f_sw_l_ll.root");//no
    fB[38]   = new TFile("data_final/4f_sw_l_lr.root");
    fB[39]   = new TFile("data_final/4f_sw_l_rl.root");
    fB[40]   = new TFile("data_final/4f_sw_l_rr.root");
    fB[41]   = new TFile("data_final/4f_sw_sl_ll.root");//few
    fB[42]   = new TFile("data_final/4f_sw_sl_lr.root");
    fB[43]   = new TFile("data_final/4f_sw_sl_rl.root");
    fB[44]   = new TFile("data_final/4f_sw_sl_rr.root");
    fB[45]   = new TFile("data_final/4f_szeorsw_l_ll.root");//no
    fB[46]   = new TFile("data_final/4f_szeorsw_l_lr.root");
    fB[47]   = new TFile("data_final/4f_szeorsw_l_rl.root");
    fB[48]   = new TFile("data_final/4f_szeorsw_l_rr.root");
    fB[49]   = new TFile("data_final/e1e1h_ll.root");//a few 
    fB[50]   = new TFile("data_final/e1e1h_lr.root");
    fB[51]   = new TFile("data_final/e1e1h_rl.root");
    fB[52]   = new TFile("data_final/e1e1h_rr.root");
    fB[53]   = new TFile("data_final/e2e2h_lr.root");
    fB[54]   = new TFile("data_final/e2e2h_rl.root");
    fB[55]   = new TFile("data_final/e3e3h_lr.root");
    fB[56]   = new TFile("data_final/e3e3h_rl.root");
    fB[57]   = new TFile("data_final/nnh_lr.root");
    fB[58]   = new TFile("data_final/nnh_rl.root");
  }
  else if (mode == 2) { // for training
    //signal
    fS[ 1]   = new TFile("data/qqh_lr.root");
    fS[ 2]   = new TFile("data/qqh_rl.root");
    fS[ 3]   = new TFile("data/qqh_zz_4n_lr.root");
    fS[ 4]   = new TFile("data/qqh_zz_4n_rl.root");

    fB[ 1]   = new TFile("data/2f_z_bhabhag_ll.root");
    fB[ 2]   = new TFile("data/2f_z_bhabhag_lr.root");
    fB[ 3]   = new TFile("data/2f_z_bhabhag_rl.root");
    fB[ 4]   = new TFile("data/2f_z_bhabhag_rr.root");
    fB[ 5]   = new TFile("data/2f_z_l_lr.root");
    fB[ 6]   = new TFile("data/2f_z_l_rl.root");
    fB[ 7]   = new TFile("data/2f_z_h_lr.root");
    fB[ 8]   = new TFile("data/2f_z_h_rl.root");
    fB[ 9]   = new TFile("data/4f_zz_l_lr.root");
    fB[10]   = new TFile("data/4f_zz_l_rl.root");
    fB[11]   = new TFile("data/4f_zz_h_lr.root");
    fB[12]   = new TFile("data/4f_zz_h_rl.root");
    fB[13]   = new TFile("data/4f_zz_sl_lr.root");
    fB[14]   = new TFile("data/4f_zz_sl_rl.root");
    fB[15]   = new TFile("data/4f_ww_l_lr.root");
    fB[16]   = new TFile("data/4f_ww_l_rl.root");
    fB[17]   = new TFile("data/4f_ww_h_lr.root");
    fB[18]   = new TFile("data/4f_ww_h_rl.root");
    fB[19]   = new TFile("data/4f_ww_sl_lr.root");
    fB[20]   = new TFile("data/4f_ww_sl_rl.root");
    fB[21]   = new TFile("data/4f_zzorww_l_lr.root");
    fB[22]   = new TFile("data/4f_zzorww_l_rl.root");
    fB[23]   = new TFile("data/4f_zzorww_h_lr.root");
    fB[24]   = new TFile("data/4f_zzorww_h_rl.root");
    fB[25]   = new TFile("data/4f_sznu_l_lr.root");
    fB[26]   = new TFile("data/4f_sznu_l_rl.root");
    fB[27]   = new TFile("data/4f_sznu_sl_lr.root");
    fB[28]   = new TFile("data/4f_sznu_sl_rl.root");
    fB[29]   = new TFile("data/4f_sze_l_ll.root");
    fB[30]   = new TFile("data/4f_sze_l_lr.root");
    fB[31]   = new TFile("data/4f_sze_l_rl.root");
    fB[32]   = new TFile("data/4f_sze_l_rr.root");
    fB[33]   = new TFile("data/4f_sze_sl_ll.root");
    fB[34]   = new TFile("data/4f_sze_sl_lr.root");
    fB[35]   = new TFile("data/4f_sze_sl_rl.root");
    fB[36]   = new TFile("data/4f_sze_sl_rr.root");
    fB[37]   = new TFile("data/4f_sw_l_ll.root");
    fB[38]   = new TFile("data/4f_sw_l_lr.root");
    fB[39]   = new TFile("data/4f_sw_l_rl.root");
    fB[40]   = new TFile("data/4f_sw_l_rr.root");
    fB[41]   = new TFile("data/4f_sw_sl_ll.root");
    fB[42]   = new TFile("data/4f_sw_sl_lr.root");
    fB[43]   = new TFile("data/4f_sw_sl_rl.root");
    fB[44]   = new TFile("data/4f_sw_sl_rr.root");
    fB[45]   = new TFile("data/4f_szeorsw_l_ll.root");
    fB[46]   = new TFile("data/4f_szeorsw_l_lr.root");
    fB[47]   = new TFile("data/4f_szeorsw_l_rl.root");
    fB[48]   = new TFile("data/4f_szeorsw_l_rr.root");
    fB[49]   = new TFile("data/e1e1h_ll.root");
    fB[50]   = new TFile("data/e1e1h_lr.root");
    fB[51]   = new TFile("data/e1e1h_rl.root");
    fB[52]   = new TFile("data/e1e1h_rr.root");
    fB[53]   = new TFile("data/e2e2h_lr.root");
    fB[54]   = new TFile("data/e2e2h_rl.root");
    fB[55]   = new TFile("data/e3e3h_lr.root");
    fB[56]   = new TFile("data/e3e3h_rl.root");
    fB[57]   = new TFile("data/nnh_lr.root");
    fB[58]   = new TFile("data/nnh_rl.root");
  }

  for (Int_t i=1;i<nSignal;i++) {
    if (mode==0){
      if(i==3||i==4)
	hAnl_S[i] = (TTree*)fS[i]->Get("hAnl");
    }else{
      hAnl_S[i] = (TTree*)fS[i]->Get("hAnl");
      /*if (mode == 2)*/ hGen_S[i] = (TTree*)fS[i]->Get("hGen");
    }
  }
  
  for (Int_t i=1;i<nBackground;i++) {
    if (mode==0){
      if(i==13||i==14||i==19||i==20||i==27||i==28)
	hAnl_B[i] = (TTree*)fB[i]->Get("hAnl");
    } else{
      hAnl_B[i] = (TTree*)fB[i]->Get("hAnl");
      /*if (mode == 2)*/ hGen_B[i] = (TTree*)fB[i]->Get("hGen");
    }
  }

}
