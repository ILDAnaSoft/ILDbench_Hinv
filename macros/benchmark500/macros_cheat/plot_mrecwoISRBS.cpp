#include <iostream>
#include <sstream>
#include <iomanip>
#include "readPara.C"
#include "readData.C"
#include "readCut.C"
#include "readCutwoISRBS.C"
#include "global.h"
#include "/home/ilc/tianjp/bin/root_macros/style.C"

#include "TROOT.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TCut.h"
#include "THStack.h"
#include "TLatex.h"

using namespace std;

// parameter setting ------------------------
const TString X0 = "mrec";
const Int_t cutmax0 = 7;
const Double_t binwidth0 = 5;
const Double_t xmin0 = 50.;
const Double_t xmax0 = 300.;
//const Int_t nbin = (xmax-xmin)/binwidth;
// ------------------------------------------

TString fCanvasName = "canvas";
TString fSaveName = "eps/test.eps";
Bool_t iSave = kTRUE;
Bool_t iLegend = kFALSE;
Bool_t iPrintParas = kFALSE;
TString fStackMode = "HIST";
Bool_t iLogScale =  kFALSE;
void setCanvasName(TString str) {
  fCanvasName = str;
}
void setSave(Bool_t i) {
  iSave = i;
}
void setSaveName(TString str) {
  fSaveName = str;
}
void setLegend(Bool_t i) {
  iLegend = i;
}
void setPrintParas(Bool_t i) {
  iPrintParas = i;
}

void setStackMode(TString str) {
  fStackMode = str;
}
void setLogScale(Bool_t i) {
  iLogScale = i;
}

void getHist(TCut cutS, TCut cutB, TString varS, TString varB, Int_t nbin, Double_t xmin, Double_t xmax, TH1F *hS[], TH1F *hB[], TString sname, TString bname) {

  TCanvas *c = new TCanvas("temp","",1024,768);
  //  Int_t nbin = 100;
  //  Double_t xmin = 0.;
  //  Double_t xmax = 200.;
  for (Int_t i=1;i<nSignal;i++) {
    stringstream hname;
    hname << sname.Data() << setw(1) << setfill('0') << i << ends;
    cerr << hname.str().data() << endl;
    hS[i] = new TH1F(hname.str().data(),"",nbin,xmin,xmax);
    hS[i]->SetOption("HIST");
    stringstream drname;
    if (i==3||i==4)
      drname << varS.Data() << ">>" << sname.Data() << setw(1) << setfill('0') << i << ends;
    else
      drname << varB.Data() << ">>" << sname.Data() << setw(1) << setfill('0') << i << ends;
    if (!hAnl_S[i]) continue;
    if (i==3||i==4)
      hAnl_S[i]->Draw(drname.str().data(),cutS);
    else
      hAnl_S[i]->Draw(drname.str().data(),cutB);
    hS[i]->Scale(weightS[i]);
  }

  for (Int_t i=1;i<nBackground;i++) {
    stringstream hname;
    hname << bname.Data() << setw(2) << setfill('0') << i << ends;
    cerr << hname.str().data() << endl;
    hB[i] = new TH1F(hname.str().data(),"",nbin,xmin,xmax);
    hB[i]->SetOption("HIST");
    stringstream drname;
    drname << varB.Data() << ">>" << bname.Data() << setw(2) << setfill('0') << i << ends;
    if (!hAnl_B[i]) continue;
    hAnl_B[i]->Draw(drname.str().data(),cutB);
    hB[i]->Scale(weightB[i]);
  }

}


void plot_mrecwoISRBS(Int_t iPol=-1,TString X=X0,Int_t cutmax=cutmax0,Double_t binwidth=binwidth0,Double_t xmin=xmin0,Double_t xmax=xmax0,string kinfit="cheatBS") {

  setILDStyle();
  gStyle->SetLegendTextSize(0.04);
  gStyle->SetPadTopMargin(0.12);

  if ((X=="mkfmrec"||X=="mrec")&&(cutmax==8||cutmax==9)){
    setStackMode("HIST");
    setLogScale(0);
  }
  else if (X=="bdtl"||X=="bdtr"||X=="bdtlwokf"||X=="bdtrwokf")
    setLogScale(0);

  
  if (iPol != 1 && iPol !=0 && iPol != -1) {
    cerr << "Please use a correct Polarization:  0,1,-1" << endl;
    return;
  }

  string pol;
  if (iPol==-1) pol = "(-0.8,+0.3)";
  else if (iPol==+1) pol = "(+0.8,-0.3)";

  Bool_t iKF = kFALSE;
  if (X=="mrec"||X=="bdtlwokf"||X=="bdtrwokf")
    iKF = kFALSE;
  
  // get cuts
  readCutS(myCutS,iPol,iKF);
  readCut(myCutB,iPol,iKF);

  // get ntuple data
  readData(1,kinfit,kTRUE);

  // get x-section, # of generated events, weight, etc.
  readPara(1,iPol,inteLumi);

  TCut cut0S = "";
  TCut cut0B = "";
  if (cutmax){
    for (Int_t i=1;i<=cutmax;i++) cut0S += myCutS[i];
    for (Int_t i=1;i<=cutmax;i++) cut0B += myCutB[i];
  }
  TH1F *hS[nSignal];
  TH1F *hB[nBackground];
  Int_t nbin = (xmax-xmin)/binwidth;

  getHist(cut0S,cut0B,X+"woISRBS",X,nbin,xmin,xmax,hS,hB,"hS","hB");

  for (Int_t i=1;i<2;i++) hS[2]->Add(hS[i]); // all qqH
  for (Int_t i=3;i<4;i++) hS[4]->Add(hS[i]); // qqH_inv
  
  for (Int_t i=2;i<nBackground;i++) hB[1]->Add(hB[i]); // all background
  hB[1]->Add(hS[2]);
  
  /*for (Int_t i=13;i<14;i++) hB[14]->Add(hB[i]); // 4f_zz_sl
  for (Int_t i=19;i<20;i++) hB[20]->Add(hB[i]); // 4f_ww_sl
  for (Int_t i=27;i<28;i++) hB[28]->Add(hB[i]); // 4f_sznu_sl

  for (Int_t i=2;i<nBackground;i++) {
    if ((i>=2&&i<=12) || (i>=15&&i<=18) || (i>=21&&i<=26) || (i>=29&&i<=58))
      hB[1]->Add(hB[i]); 
      }*/
  
  setCanvasName("cvs1");
  //  setSave(kTRUE);
  //setLegend(kTRUE);

  TCanvas *cvs = new TCanvas(fCanvasName,"",800,600);
  hB[1]->SetFillColor(kBlack);
  hB[1]->SetLineColor(kBlack);
  hS[4]->SetFillColor(kRed);
  hS[4]->SetLineColor(kRed);
  //hS[2]->SetFillColor(4);
  //hS[2]->SetLineColor(4);

  /*hB[14]->SetFillColor(3);
  hB[14]->SetLineColor(3);
  hB[20]->SetFillColor(kGreen+2);
  hB[20]->SetLineColor(kGreen+2);
  hB[28]->SetFillColor(7);
  hB[28]->SetLineColor(7);*/

  if (fStackMode=="nostackHIST"){
    hS[4]->SetFillStyle(0);
    //hS[2]->SetFillStyle(0);
  
    hB[1]->SetFillStyle(0);
    /*hB[14]->SetFillStyle(0);
    hB[20]->SetFillStyle(0);
    hB[28]->SetFillStyle(0);*/
  }else{
    hS[4]->SetFillStyle(3002);
    //hS[2]->SetFillStyle(3003);
  
    hB[1]->SetFillStyle(3002);
    /*hB[14]->SetFillStyle(3001);
    hB[20]->SetFillStyle(3001);
    hB[28]->SetFillStyle(3001);*/
  }
  
  if (iLogScale) cvs->SetLogy();
  TLatex lmkf;
  //lmkf.SetTextAlign(12);
  
  THStack *stk = new THStack("stk",Form("#sqrt{s} = 500 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = %.0f fb^{-1}, Cut: No.1~No.%d",pol.c_str(),inteLumi,cutmax));

  if (cutmax==0)
    stk->SetTitle(Form("#sqrt{s} = 500 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = %.0f fb^{-1}",pol.c_str(),inteLumi));
  else if (cutmax==1)
    stk->SetTitle(Form("#sqrt{s} = 500 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = %.0f fb^{-1}, Cut: No.1",pol.c_str(),inteLumi));
  
  
  stk->Add(hB[1]);
    
  /*stk->Add(hB[28]);
  stk->Add(hB[20]);
  stk->Add(hB[14]);*/
  
  //stk->Add(hS[2]);
  stk->Add(hS[4]);


  /*if ((X=="mkfmrec"||X=="mrec")&&(cutmax==8||cutmax==9)){
    if(iPol==-1)
      //stk->SetMaximum(2600);
    else if(iPol== 1)
      //stk->SetMaximum(950);
      }*/
  stk->Draw(fStackMode);

  if (X=="mrec"){
    stk->GetXaxis()->SetTitle("M_{recoil} [GeV]");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f GeV",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kBlack);
    //lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/o kinematic fit}");
  }else if (X=="mkfmrec"){
    stk->GetXaxis()->SetTitle("M_{recoil}^{kf} [GeV]");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f GeV",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kRed);
    lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/ kinematic fit}");
  }else if (X=="lrzZ.Pt()"){
    stk->GetXaxis()->SetTitle("#it{p_{Tjj}} [GeV]");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f GeV",binwidth));
  }else if (X=="lrzZ.M()"){
    stk->GetXaxis()->SetTitle("M_{jj} [GeV]");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f GeV",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kBlack);
    if (cutmax>6)
      lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/o kinematic fit}");
  }else if (X=="mkflrzZ.M()"){
    stk->GetXaxis()->SetTitle("M_{jj}^{kf} [GeV]");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f GeV",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kRed);
    if (cutmax>6)
      lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/ kinematic fit}");
  }else if (X=="abs(lrzZ.CosTheta())"){
    stk->GetXaxis()->SetTitle("|cos#theta_{jj}|");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f ",binwidth));
  }else if (X=="bdtlwokf"){
    stk->GetXaxis()->SetTitle("BDT_{left}");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kBlack);
    if (cutmax>6)
      lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/o kinematic fit}");
  }else if (X=="bdtrwokf"){
    stk->GetXaxis()->SetTitle("BDT_{right}");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kBlack);
    if (cutmax>6)
      lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/o kinematic fit}");
  }else if (X=="bdtl"){
    stk->GetXaxis()->SetTitle("BDT_{left}^{kf}");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kRed);
    if (cutmax>6)
      lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/ kinematic fit}");
  }else if (X=="bdtr"){
    stk->GetXaxis()->SetTitle("BDT_{right}^{kf}");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
    lmkf.SetTextSize(0.05);
    lmkf.SetTextColor(kRed);
    if (cutmax>6)
      lmkf.DrawLatexNDC(0.7,0.93,"#bf{w/ kinematic fit}");
  }else if (X=="nlep"){
    stk->GetXaxis()->SetTitle("N_{lep}");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
  }else if (X=="npfo"){
    stk->GetXaxis()->SetTitle("N_{pfo}");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
  }else if (X=="ntrackj1+ntrackj2"){
    stk->GetXaxis()->SetTitle("N_{charged}");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
  }else{
    stk->GetXaxis()->SetTitle(X);
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
  }
  //stk->GetYaxis()->CenterTitle();


  TLegend *leg = new TLegend(0.65,0.7,0.87,0.87);//R:0.6,0.7,0.9,0.9 L:0.2,0.7,0.5,0.9
  
  leg->AddEntry(hS[4],Form("H#rightarrowinv.(BR=%.0f%%)",100*BRinv),"f");  
  //leg->AddEntry((TObject*)0,Form(" BR = %.0f%%",100*BRinv),"");
  /*leg->AddEntry(hS[2],"qqH,SM","f");
  leg->AddEntry(hB[14],"ZZ","f");
  leg->AddEntry(hB[20],"WW","f");
  leg->AddEntry(hB[28],"#nu#nuZ","f");*/
  
  //leg->AddEntry(hB[1],"other bkg","f");
  leg->AddEntry(hB[1],"all bkg","f");

  
  if (iPol == -1) setSaveName(Form("pdf/%s/%s_cut%d_eLpR",kinfit.c_str(),X.Data(),cutmax));
  if (iPol ==  1) setSaveName(Form("pdf/%s/%s_cut%d_eRpL",kinfit.c_str(),X.Data(),cutmax));
    
  leg->SetFillStyle(0);
  leg->Draw();
  //lmkf.SetTextSize(0.05);
  //lmkf.DrawLatexNDC(0.5,0.5,"w/ kinematic fit");
  
  cvs->Modified();

  if (iSave) {
    gSystem->Exec(Form("mkdir -p pdf/%s",kinfit.c_str())); 
    cvs->SaveAs(fSaveName+".pdf");

    gSystem->Exec(Form("mkdir -p output/%s",kinfit.c_str())); 
    TFile *output;
    if (iPol==-1 ) output = new TFile(Form("output/%s/%s_cut%d_eLpR.root",kinfit.c_str(),X.Data(),cutmax),"RECREATE");
    if (iPol== 1 ) output = new TFile(Form("output/%s/%s_cut%d_eRpL.root",kinfit.c_str(),X.Data(),cutmax),"RECREATE");

    hS[4]->Write("hS_hinv");
    /*/hS[2]->Write("hS_SM");
      hB[14]->Write("hB_ZZ");
      hB[20]->Write("hB_WW");
      hB[28]->Write("hB_nnZ");*/
    //hB[1]->Write("hB_other");
    hB[1]->Write("hB_all");

    stk->Write("stk");
    leg->Write("leg");
    
    output->Close();
  }
  
  // close root files
  /* for (Int_t i=1;i<nSignal;i++) {
    fS[i]->Close();
  }
  for (Int_t i=1;i<nBackground;i++) {
    fB[i]->Close();
    }*/
}
