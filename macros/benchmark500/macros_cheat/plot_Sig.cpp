#include <unistd.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "readPara.C"
#include "readData.C"
#include "readCut.C"
#include "global.h"
#include "style.h"

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

using namespace std;

// parameter setting ------------------------
const TString X0 = "mrec";
const Int_t cutmax0 = 7;
const Double_t binwidth0 = 5;
const Double_t xmin0 = 80.;
const Double_t xmax0 = 330.;
//const Int_t nbin = (xmax-xmin)/binwidth;
const string condition0 = "cheatOverlay";
// ------------------------------------------

TString fCanvasName = "canvas";
TString fSaveName = "eps/test.eps";
Bool_t iSave = kTRUE;
Bool_t iLegend = kFALSE;
Bool_t iPrintParas = kFALSE;
TString fStackMode = "HISTE";
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

void getHist(TCut cutS, TString varS, Int_t nbin, Double_t xmin, Double_t xmax, TH1F *hS[], TString sname) {

  //TCanvas *c = new TCanvas("temp","",1024,768);
  for (Int_t i=3;i<nSignal;i++) {
    stringstream hname;
    hname << sname.Data() << setw(1) << setfill('0') << i << ends;
    cerr << hname.str().data() << endl;
    hS[i] = new TH1F(hname.str().data(),"",nbin,xmin,xmax);
    hS[i]->SetOption("HIST");
    stringstream drname;
    drname << varS.Data() << ">>" << sname.Data() << setw(1) << setfill('0') << i << ends;
    if (!hAnl_S[i]) continue;
    hAnl_S[i]->Draw(drname.str().data(),cutS);
    hS[i]->Scale(weightS[i]);
  }

}


void plot_Sig(Int_t iPol=-1,string condition=condition0,TString X=X0,Int_t cutmax=cutmax0,Double_t binwidth=binwidth0,Double_t xmin=xmin0,Double_t xmax=xmax0) {

  char linkname[50];
  Int_t r = readlink("data_cheat",linkname,50-1);
  linkname[r] = '\0';
  cout << linkname << endl;
  
  setILDStyle();
  gStyle->SetLegendTextSize(0.04);
  gStyle->SetPadTopMargin(0.12);

  Bool_t iCheat = kFALSE;
  if (condition.substr(0,5)=="cheat"){
    cerr << "Cheat Mode: w/o " << condition.substr(5,condition.size()-1) << endl;
    iCheat = kTRUE;
  }
  
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
  string ep;
  if (iPol==-1) {
    pol = "(-0.8,+0.3)";
    ep  = "eLpR";
  }
  if (iPol==+1) {
    pol = "(+0.8,-0.3)";
    ep  = "eRpL";
  }

  /*Bool_t iKF = kFALSE;
  if (X=="mrec"||X=="bdtlwokf"||X=="bdtrwokf")
  iKF = kFALSE;*/
  
  // get cuts
  readCut(myCutS,xmin,xmax,iPol,iCheat);

  // get ntuple data
  readData(1,condition,iCheat);

  // get x-section, # of generated events, weight, etc.
  readPara(1,iPol,inteLumi);

  TCut cut0S = "";
  if (cutmax){
    for (Int_t i=1;i<=cutmax;i++) cut0S += myCutS[i];
  }
  TH1F *hS[nSignal];
  Int_t nbin = (xmax-xmin)/binwidth;

  if (iCheat)
    getHist(cut0S,X+"Cheat",nbin,xmin,xmax,hS,"hS");
  else
    getHist(cut0S,X,nbin,xmin,xmax,hS,"hS");

  hS[4]->Add(hS[3]); // qqH_inv
    
  setCanvasName("cvs1");
  //  setSave(kTRUE);
  //setLegend(kTRUE);

  TCanvas *cvs = new TCanvas(fCanvasName,"",800,600);
  hS[4]->SetFillColor(kRed-10);
  hS[4]->SetLineColor(kRed);

  if (fStackMode=="nostackHIST")
    hS[4]->SetFillStyle(0);
  else
    hS[4]->SetFillStyle(1001);
  
  if (iLogScale) cvs->SetLogy();
  
  THStack *stk = new THStack("stk",Form("#sqrt{s} = 500 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = %.0f fb^{-1}, Cut: No.1~No.%d",pol.c_str(),inteLumi,cutmax));

  if (cutmax==0)
    stk->SetTitle(Form("#sqrt{s} = 500 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = %.0f fb^{-1}",pol.c_str(),inteLumi));
  else if (cutmax==1)
    stk->SetTitle(Form("#sqrt{s} = 500 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = %.0f fb^{-1}, Cut: No.1",pol.c_str(),inteLumi));
  
  stk->Add(hS[4]);
  ((TGaxis*)stk->GetYaxis())->SetMaxDigits(3);
  stk->Draw(fStackMode);

  if (X=="mrec"){
    stk->GetXaxis()->SetTitle("M_{recoil} [GeV]");
    stk->GetYaxis()->SetTitle(Form("Events / %.2f GeV",binwidth));
  }else{
    stk->GetXaxis()->SetTitle(X);
    stk->GetYaxis()->SetTitle(Form("Events / %.2f",binwidth));
  }
  //stk->GetYaxis()->CenterTitle();


  TLegend *leg = new TLegend(0.65,0.7,0.87,0.87);//R:0.6,0.7,0.9,0.9 L:0.2,0.7,0.5,0.9
  
  leg->AddEntry(hS[4],Form("H#rightarrowinv.(BR=%.0f%%)",100*BRinv),"f");  
  
  setSaveName(Form("pdf/%s/%s_cut%d_%s",condition.c_str(),X.Data(),cutmax,ep.c_str()));
    
  leg->SetFillStyle(0);
  leg->Draw();
  TLatex *latex = new TLatex();
  latex->SetTextSize(0.06);
  latex->SetTextAlign(12);
  latex->SetTextColor(kGray);
  latex->DrawLatexNDC(.2, .87, "#it{ILD preliminary}");
  TLatex *lcheat = new TLatex();
  lcheat->SetTextSize(0.05);
  lcheat->SetTextAlign(12);
  lcheat->SetTextColor(kAzure-2);
  lcheat->DrawLatexNDC(.7, .87, condition.c_str());
  
  cvs->Modified();

  if (iSave) {
    //gSystem->Exec(Form("mkdir -p pdf/%s",condition.c_str())); 
    //cvs->SaveAs(fSaveName+".pdf");

    TFile *bkg = new TFile(Form("output/bkg%.0f-%.0f/%s_cut%d_%s.root",xmin,xmax,X.Data(),cutmax,ep.c_str()));
    
    gSystem->Exec(Form("mkdir -p output/%s",condition.c_str())); 
    TFile *output = new TFile(Form("output/%s/%s_cut%d_%s.root",condition.c_str(),X.Data(),cutmax,ep.c_str()),"RECREATE");

    hS[4]->Write("hS_hinv");
    bkg->Get("hB_all")->Write("hB_all");

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
