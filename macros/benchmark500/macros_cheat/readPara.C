#include "global.h"

void readPara(Int_t mode, Int_t iPol, Int_t intlumi, Bool_t isHighStats = kFALSE) {
  
  xsecS[ 1] = 114.69973;                 // qqh_lr
  xsecS[ 2] = 73.42555;                  // qqh_lr
  xsecS[ 3] = xsecS[1]*BRinv;            // qqh_inv_lr 0.12238461191 fb in Standard Model
  xsecS[ 4] = xsecS[2]*BRinv;            // qqh_inv_rl 0.07834506185 fb in Standard Model

  xsecB[ 1] = 2961.98 ;                  // 2f_z_bhabhag_ll
  xsecB[ 2] = 3645.16 ;                  // 2f_z_bhabhag_lr
  xsecB[ 3] = 3372.10 ;                  // 2f_z_bhabhag_rl
  xsecB[ 4] = 2956.92 ;                  // 2f_z_bhabhag_rr
  xsecB[ 5] = 5547.58 ;                  // 2f_z_l_lr
  xsecB[ 6] = 4344.86 ;                  // 2f_z_l_rl
  xsecB[ 7] = 32470.5 ;                  // 2f_z_h_lr
  xsecB[ 8] = 17994.7 ;                  // 2f_z_h_rl
  xsecB[ 9] = 60.0221 ;                  // 4f_zz_l_lr
  xsecB[10] = 36.8321 ;                  // 4f_zz_l_rl
  xsecB[11] = 680.225 ;                  // 4f_zz_h_lr
  xsecB[12] = 271.876 ;                  // 4f_zz_h_rl
  xsecB[13] = 608.57  ;                  // 4f_zz_sl_lr
  xsecB[14] = 288.362 ;                  // 4f_zz_sl_rl
  xsecB[15] = 790.713 ;                  // 4f_ww_l_lr
  xsecB[16] = 4.16979 ;                  // 4f_ww_l_rl
  xsecB[17] = 7680.69 ;                  // 4f_ww_h_lr
  xsecB[18] = 33.5218 ;                  // 4f_ww_h_rl
  xsecB[19] = 9521.45 ;                  // 4f_ww_sl_lr
  xsecB[20] = 45.5779 ;                  // 4f_ww_sl_rl
  xsecB[21] = 819.819 ;                  // 4f_zzorww_l_lr
  xsecB[22] = 19.0229 ;                  // 4f_zzorww_l_rl
  xsecB[23] = 6400.11 ;                  // 4f_zzorww_h_lr
  xsecB[24] = 78.6961 ;                  // 4f_zzorww_h_rl
  xsecB[25] = 278.797 ;                  // 4f_sznu_l_lr
  xsecB[26] = 14.8503 ;                  // 4f_sznu_l_rl
  xsecB[27] = 951.715 ;                  // 4f_sznu_sl_lr
  xsecB[28] = 58.9838 ;                  // 4f_sznu_sl_rl
  xsecB[29] = 6991.01 ;                  // 4f_sze_l_ll
  xsecB[30] = 7257.72 ;                  // 4f_sze_l_lr
  xsecB[31] = 7107.69 ;                  // 4f_sze_l_rl
  xsecB[32] = 6985.   ;                  // 4f_sze_l_rr
  xsecB[33] = 1775.49 ;                  // 4f_sze_sl_ll
  xsecB[34] = 1961.13 ;                  // 4f_sze_sl_lr
  xsecB[35] = 1726.55 ;                  // 4f_sze_sl_rl
  xsecB[36] = 1778.01 ;                  // 4f_sze_sl_rr
  xsecB[37] = 252.121 ;                  // 4f_sw_l_ll
  xsecB[38] = 2605.8  ;                  // 4f_sw_l_lr
  xsecB[39] = 8.37009 ;                  // 4f_sw_l_rl
  xsecB[40] = 252.001 ;                  // 4f_sw_l_rr
  xsecB[41] = 753.07  ;                  // 4f_sw_sl_ll
  xsecB[42] = 7805.53 ;                  // 4f_sw_sl_lr
  xsecB[43] = 22.8265 ;                  // 4f_sw_sl_rl
  xsecB[44] = 750.068 ;                  // 4f_sw_sl_rr
  xsecB[45] = 156.722 ;                  // 4f_szeorsw_l_ll
  xsecB[46] = 1010.93 ;                  // 4f_szeorsw_l_lr
  xsecB[47] = 28.5533 ;                  // 4f_szeorsw_l_rl
  xsecB[48] = 156.837 ;                  // 4f_szeorsw_l_rr
  xsecB[49] = 5.837461 ;                 // e1e1h_ll
  xsecB[50] = 15.001266 ;                // e1e1h_lr
  xsecB[51] = 7.460882 ;                 // e1e1h_rl
  xsecB[52] = 5.837683 ;                 // e1e1h_rr
  xsecB[53] = 5.679039 ;                 // e2e2h_lr
  xsecB[54] = 3.635537 ;                 // e2e2h_rl
  xsecB[55] = 5.667049 ;                 // e3e3h_lr
  xsecB[56] = 3.628031 ;                 // e3e3h_rl
  xsecB[57] = 289.08088 ;                // nnh_lr
  xsecB[58] = 21.559948 ;                // nnh_rl
  
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

  //Number of generated signal events
  ngenS[ 1] =     114691;
  ngenS[ 2] =      73417;
  ngenS[ 3] =      19990;
  ngenS[ 4] =      19988;
  //Number of generated background events
  ngenB[ 1] =     170335;
  ngenB[ 2] =     169892;
  ngenB[ 3] =     169892;
  ngenB[ 4] =     170253;
  ngenB[ 5] =     317309;
  ngenB[ 6] =     311967;
  ngenB[ 7] =    1503862;
  ngenB[ 8] =     844431;
  ngenB[ 9] =      24602;
  ngenB[10] =      14439;
  ngenB[11] =      38521;
  ngenB[12] =      19698;
  ngenB[13] =     303754;
  ngenB[14] =     139733;
  ngenB[15] =     143725;
  ngenB[16] =       1953;
  ngenB[17] =     375994;
  ngenB[18] =      16761;
  ngenB[19] =     476973;
  ngenB[20] =      22774;
  ngenB[21] =     385479;
  ngenB[22] =       7183;
  ngenB[23] =     312996;
  ngenB[24] =      20753;
  ngenB[25] =      80871;
  ngenB[26] =       5107;
  ngenB[27] =      71782;
  ngenB[28] =      29391;
  ngenB[29] =    1173013;
  ngenB[30] =    1073306;
  ngenB[31] =    1176537;
  ngenB[32] =    1065246;
  ngenB[33] =      87296;
  ngenB[34] =     126973;
  ngenB[35] =      87502;
  ngenB[36] =      87067;
  ngenB[37] =     112196;
  ngenB[38] =    1183653;
  ngenB[39] =       4155;
  ngenB[40] =     112359;
  ngenB[41] =      37139;
  ngenB[42] =     368836;
  ngenB[43] =      11412;
  ngenB[44] =      37120;
  ngenB[45] =      75210;
  ngenB[46] =     497920;
  ngenB[47] =      12916;
  ngenB[48] =      75263;
  ngenB[49] =       9998;
  ngenB[50] =      14990;
  ngenB[51] =       9994;
  ngenB[52] =       9998;
  ngenB[53] =       9976;
  ngenB[54] =       9976;
  ngenB[55] =       9993;
  ngenB[56] =       9989;
  ngenB[57] =     211529;
  ngenB[58] =      21406;

  /*//Number of generated signal events
  ngenS[ 1] =     346336; 
  ngenS[ 2] =     222351; 
  ngenS[ 3] =      19701;
  ngenS[ 4] =      19900;

  //Number of generated background events
  ngenB[ 1] =     503301;
  ngenB[ 2] =     507115;
  ngenB[ 3] =     483067; //-1
  ngenB[ 4] =     503234; 
  ngenB[ 5] =    2066417; //-59505
  ngenB[ 6] =    1584615; //-62154
  ngenB[ 7] =    1747094; 
  ngenB[ 8] =    1426198; //-2
  ngenB[ 9] =      38516; //-1484
  ngenB[10] =      28868; //-1132
  ngenB[11] =     350647;
  ngenB[12] =     151328;
  ngenB[13] =     356461; //-4 4f_zz_sl_lr
  ngenB[14] =     178634; //-4 4f_zz_sl_rl
  ngenB[15] =     398781; //-426
  ngenB[16] =       9991; //-9
  ngenB[17] =    1074479;
  ngenB[18] =      34576;
  ngenB[19] =    1919148; //   4f_ww_sl_lr 
  ngenB[20] =      43501; //   4f_ww_sl_rl
  ngenB[21] =     408792; //-1416
  ngenB[22] =      18915; //-1085
  ngenB[23] =    1074452;
  ngenB[24] =      56562;
  ngenB[25] =      55809; //-4191
  ngenB[26] =      18354; //-1646
  ngenB[27] =     114516; //-1 4f_sznu_sl_lr
  ngenB[28] =      32998; //-2 4f_sznu_sl_rl
  ngenB[29] =     255132; //-4
  ngenB[30] =     272921; //-2
  ngenB[31] =     254963; //-4
  ngenB[32] =     253111;
  ngenB[33] =      65871; 
  ngenB[34] =     115417;
  ngenB[35] =      79188;
  ngenB[36] =      64741;
  ngenB[37] =       9987; //-13
  ngenB[38] =     835555; //-162
  ngenB[39] =       9999; //-1
  ngenB[40] =       9993; //-7
  ngenB[41] =      30000; 
  ngenB[42] =    1927011; 
  ngenB[43] =      22000;
  ngenB[44] =      30000;
  ngenB[45] =       8000;
  ngenB[46] =     231046; //-6
  ngenB[47] =       7997; //-3
  ngenB[48] =       7995; //-5
  ngenB[49] =       1000;
  ngenB[50] =      17597;
  ngenB[51] =      11159;
  ngenB[52] =       1000;
  ngenB[53] =      17143;
  ngenB[54] =      10983;
  ngenB[55] =      17108;
  ngenB[56] =      10958;
  ngenB[57] =     128502; //-138
  ngenB[58] =      65020; //-78
  */

  if (mode == 2) {
  }

  for(Int_t i=1;i<nSignal;i++) {
    nexpS[i] = xsecS[i]*factS[i]*intlumi;
    //weightS[i] = nexpS[i]/ngenS[i] ;
    weightS[i] = nexpS[i]/hGen_S[i]->GetEntries() ;
  }
  for(Int_t i=1;i<nBackground;i++) {
    nexpB[i] = xsecB[i]*factB[i]*intlumi;
    //weightB[i] = nexpB[i]/ngenB[i] ;
    weightB[i] = nexpB[i]/hGen_B[i]->GetEntries() ;
  }

}
