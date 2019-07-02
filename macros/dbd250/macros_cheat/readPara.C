#include "global.h"

void readPara(Int_t mode, Int_t iPol, Int_t intlumi, Bool_t isHighStats = kFALSE) {

  xsecS[ 1] = 346.01;                    // qqh_lr
  xsecS[ 2] = 221.95;                    // qqh_rl
  xsecS[ 3] = xsecS[1]*BRinv;            // qqh_inv_lr
  xsecS[ 4] = xsecS[2]*BRinv;            // qqh_inv_rl

  xsecB[ 1] = 25108.; 		         // 2f_z_bhabhag_ll
  xsecB[ 2] = 25287.; 		         // 2f_z_bhabhag_lr
  xsecB[ 3] = 24228.; 		         // 2f_z_bhabhag_rl
  xsecB[ 4] = 25128.; 		         // 2f_z_bhabhag_rr
  xsecB[ 5] = 21226.; 		         // 2f_z_l_lr
  xsecB[ 6] = 16470.; 		         // 2f_z_l_rl
  xsecB[ 7] = 129148.; 		         // 2f_z_h_lr
  xsecB[ 8] = 71273.; 		         // 2f_z_h_rl
  xsecB[ 9] = 158.0; 		         // 4f_zz_l_lr
  xsecB[10] = 99.51; 		         // 4f_zz_l_rl
  xsecB[11] = 1402.0; 		         // 4f_zz_h_lr
  xsecB[12] = 605.0; 		         // 4f_zz_h_rl
  xsecB[13] = 1422.; 		         // 4f_zz_sl_lr
  xsecB[14] = 713.5; 		         // 4f_zz_sl_rl
  xsecB[15] = 1564.; 		         // 4f_ww_l_lr
  xsecB[16] = 14.69; 		         // 4f_ww_l_rl
  xsecB[17] = 14874.; 		         // 4f_ww_h_lr
  xsecB[18] = 136.4; 		         // 4f_ww_h_rl
  xsecB[19] = 18781.; 		         // 4f_ww_sl_lr
  xsecB[20] = 172.7; 		         // 4f_ww_sl_rl
  xsecB[21] = 1636.; 		         // 4f_zzorww_l_lr
  xsecB[22] = 53.96; 		         // 4f_zzorww_l_rl
  xsecB[23] = 12383.; 		         // 4f_zzorww_h_lr
  xsecB[24] = 224.8; 		         // 4f_zzorww_h_rl
  xsecB[25] = 192.8; 		         // 4f_sznu_l_lr
  xsecB[26] = 39.32; 		         // 4f_sznu_l_rl
  xsecB[27] = 456.8; 		         // 4f_sznu_sl_lr
  xsecB[28] = 130.8; 		         // 4f_sznu_sl_rl
  xsecB[29] = 1010.; 		         // 4f_sze_l_ll
  xsecB[30] = 1084.; 		         // 4f_sze_l_lr
  xsecB[31] = 1020.; 		         // 4f_sze_l_rl
  xsecB[32] = 1008.; 		         // 4f_sze_l_rr
  xsecB[33] = 259.8; 		         // 4f_sze_sl_ll
  xsecB[34] = 459.0; 		         // 4f_sze_sl_lr
  xsecB[35] = 316.5; 		         // 4f_sze_sl_rl
  xsecB[36] = 259.0; 		         // 4f_sze_sl_rr
  xsecB[37] = 39.95; 		         // 4f_sw_l_ll
  xsecB[38] = 3335.; 		         // 4f_sw_l_lr
  xsecB[39] = 29.10; 		         // 4f_sw_l_rl
  xsecB[40] = 39.92; 		         // 4f_sw_l_rr
  xsecB[41] = 119.7; 		         // 4f_sw_sl_ll
  xsecB[42] = 10000.; 		         // 4f_sw_sl_lr
  xsecB[43] = 85.62; 		         // 4f_sw_sl_rl
  xsecB[44] = 119.3; 		         // 4f_sw_sl_rr
  xsecB[45] = 27.67; 		         // 4f_szeorsw_l_ll
  xsecB[46] = 922.0; 		         // 4f_szeorsw_l_lr
  xsecB[47] = 21.59; 		         // 4f_szeorsw_l_rl
  xsecB[48] = 27.62; 		         // 4f_szeorsw_l_rr
  xsecB[49] = 0.6122;                    // e1e1h_ll
  xsecB[50] = 17.60;                     // e1e1h_lr
  xsecB[51] = 11.16;                     // e1e1h_rl
  xsecB[52] = 0.6118;                    // e1e1h_rr
  xsecB[53] = 17.14;                     // e2e2h_lr
  xsecB[54] = 10.98;                     // e2e2h_rl
  xsecB[55] = 17.11;                     // e3e3h_lr
  xsecB[56] = 10.96;                     // e3e3h_rl
  xsecB[57] = 128.6;                     // nnh_lr
  xsecB[58] = 65.10;                     // nnh_rl
  
  Double_t factLL = 0.5*0.5;
  Double_t factLR = 0.5*0.5;
  Double_t factRL = 0.5*0.5;
  Double_t factRR = 0.5*0.5;
  if(iPol == -1) {
    factLL = 0.9*0.35;
    factLR = 0.9*0.65;
    factRL = 0.1*0.35;
    factRR = 0.1*0.65;
  }
  if(iPol == +1) {
    factLL = 0.1*0.65;
    factLR = 0.1*0.35;
    factRL = 0.9*0.65;
    factRR = 0.9*0.35;
  }
  if(iPol == -2) {
    factLL = 0.9*0.65;
    factLR = 0.9*0.35;
    factRL = 0.1*0.65;
    factRR = 0.1*0.35;
  }
  if(iPol == +2) {
    factLL = 0.1*0.35;
    factLR = 0.1*0.65;
    factRL = 0.9*0.35;
    factRR = 0.9*0.65;
  }
  if(iPol == -3) {
    factLL = 1.0*0.0;
    factLR = 1.0*1.0;
    factRL = 0.0*0.0;
    factRR = 0.0*1.0;
  }
  if(iPol == +3) {
    factLL = 0.0*1.0;
    factLR = 0.0*0.0;
    factRL = 1.0*1.0;
    factRR = 1.0*0.0;
  }
  Double_t factS[nSignal],factB[nBackground];
  
  factS[ 1] = factLR;
  factS[ 2] = factRL;
  factS[ 3] = factLR;
  factS[ 4] = factRL;

  factB[ 1] = factLL;	         // 2f_z_bhabhag_ll
  factB[ 2] = factLR;	         // 2f_z_bhabhag_lr
  factB[ 3] = factRL;	         // 2f_z_bhabhag_rl
  factB[ 4] = factRR;	         // 2f_z_bhabhag_rr
  factB[ 5] = factLR;	         // 2f_z_l_lr
  factB[ 6] = factRL;	         // 2f_z_l_rl
  factB[ 7] = factLR;	         // 2f_z_h_lr
  factB[ 8] = factRL;	         // 2f_z_h_rl
  factB[ 9] = factLR;	         // 4f_zz_l_lr
  factB[10] = factRL;	         // 4f_zz_l_rl
  factB[11] = factLR;	         // 4f_zz_h_lr
  factB[12] = factRL;	         // 4f_zz_h_rl
  factB[13] = factLR;	         // 4f_zz_sl_lr
  factB[14] = factRL;	         // 4f_zz_sl_rl
  factB[15] = factLR;	         // 4f_ww_l_lr
  factB[16] = factRL;	         // 4f_ww_l_rl
  factB[17] = factLR;	         // 4f_ww_h_lr
  factB[18] = factRL;	         // 4f_ww_h_rl
  factB[19] = factLR;	         // 4f_ww_sl_lr
  factB[20] = factRL;	         // 4f_ww_sl_rl
  factB[21] = factLR;	         // 4f_zzorww_l_lr
  factB[22] = factRL;	         // 4f_zzorww_l_rl
  factB[23] = factLR;	         // 4f_zzorww_h_lr
  factB[24] = factRL;	         // 4f_zzorww_h_rl
  factB[25] = factLR;	         // 4f_sznu_l_lr
  factB[26] = factRL;	         // 4f_sznu_l_rl
  factB[27] = factLR;	         // 4f_sznu_sl_lr
  factB[28] = factRL;	         // 4f_sznu_sl_rl
  factB[29] = factLL;	         // 4f_sze_l_ll
  factB[30] = factLR;	         // 4f_sze_l_lr
  factB[31] = factRL;	         // 4f_sze_l_rl
  factB[32] = factRR;	         // 4f_sze_l_rr
  factB[33] = factLL;	         // 4f_sze_sl_ll
  factB[34] = factLR;	         // 4f_sze_sl_lr
  factB[35] = factRL;	         // 4f_sze_sl_rl
  factB[36] = factRR;	         // 4f_sze_sl_rr
  factB[37] = factLL;	         // 4f_sw_l_ll
  factB[38] = factLR;	         // 4f_sw_l_lr
  factB[39] = factRL;	         // 4f_sw_l_rl
  factB[40] = factRR;	         // 4f_sw_l_rr
  factB[41] = factLL;	         // 4f_sw_sl_ll
  factB[42] = factLR;	         // 4f_sw_sl_lr
  factB[43] = factRL;	         // 4f_sw_sl_rl
  factB[44] = factRR;	         // 4f_sw_sl_rr
  factB[45] = factLL;	         // 4f_szeorsw_l_ll
  factB[46] = factLR;	         // 4f_szeorsw_l_lr
  factB[47] = factRL;	         // 4f_szeorsw_l_rl
  factB[48] = factRR;	         // 4f_szeorsw_l_rr
  factB[49] = factLL;            // e1e1h_ll
  factB[50] = factLR;            // e1e1h_lr
  factB[51] = factRL;            // e1e1h_rl
  factB[52] = factRR;            // e1e1h_rr
  factB[53] = factLR;            // e2e2h_lr
  factB[54] = factRL;            // e2e2h_rl
  factB[55] = factLR;            // e3e3h_lr
  factB[56] = factRL;            // e3e3h_rl
  factB[57] = factLR;            // nnh_lr
  factB[58] = factRL;            // nnh_rl

  if (mode == 2) {
  }

  for(Int_t i=1;i<nSignal;i++) {
    nexpS[i] = xsecS[i]*factS[i]*intlumi;
    //weightS[i] = nexpS[i]/ngenS[i];
    weightS[i] = nexpS[i]/hGen_S[i]->GetEntries();
  }
  for(Int_t i=1;i<nBackground;i++) {
    nexpB[i] = xsecB[i]*factB[i]*intlumi;
    //weightB[i] = nexpB[i]/ngenB[i];
    weightB[i] = nexpB[i]/hGen_B[i]->GetEntries();
  }

}
