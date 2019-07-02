#include "style.h"

void Replot_bkg(string pass="output_o1/full/mrec_cut7_eLpR.root"){
  
  setILDStyle();
  gStyle->SetLegendTextSize(0.04);
  gStyle->SetPadTopMargin(0.16);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleOffset(.8,"y");
  gStyle->SetTitleSize(0.06,"xy");
  gStyle->SetLabelSize(0.05,"xy");
  gStyle->SetTitleY(0.998);
  
  TFile *result = new TFile(pass.c_str());
  TH1F *hS = (TH1F*)result->Get("hS_hinv");
  TH1F *hB[5];
  string hB_name[5] = {"hS_SM","hB_ZZ","hB_WW","hB_nnZ","hB_other"};
  string leg_name[5] = {"qqH(SM)","ZZ","WW","#nu#nuZ","other bkg"};
  Int_t Fcol[5] = {kBlue-9,kSpring+1,kGreen-3,kTeal  ,kGray};
  Int_t Lcol[5] = {kBlue,  kSpring+2,kGreen+2,kTeal-1,kBlack};
  for(Int_t i=0;i<5;i++)
    hB[i] = (TH1F*)result->Get(hB_name[i].c_str());

  TCanvas *c = new TCanvas("c","",800,650);
  hS->SetFillColor(kRed-10);
  hS->SetLineColor(kRed);
  hS->SetFillStyle(1001);
  for(Int_t i=0;i<5;i++){
    hB[i]->SetFillColor(Fcol[i]);
    hB[i]->SetLineColor(Lcol[i]);
    hB[i]->SetFillStyle(1001);
  }
  
  string pol;
  //cout << pass[pass.rfind(".")-1] << endl;
  if (pass[pass.rfind(".")-1]=='R') pol = "(-0.8,+0.3)";
  if (pass[pass.rfind(".")-1]=='L') pol = "(+0.8,-0.3)";
  string cond;
  Int_t lcol;
  //cond = pass.substr(pass.find("/")+1,pass.rfind("/")-pass.find("/")-1);
  if (pass.substr(pass.find("/")+1,pass.rfind("/")-pass.find("/")-1)=="full"||pass.substr(pass.find("/")+1,pass.rfind("/")-pass.find("/")-1)=="bkg"){
    cond = "#it{full simulation}";
    lcol = kAzure-2;
  }
  else{
    cond = pass.substr(pass.find("/")+1,pass.rfind("/")-pass.find("/")-1);
    lcol = kOrange+7;
  }
  string ls;
  //ls = pass.substr(pass.find("/")-2,2);
  if (pass[pass.find("/")-2]=='l') ls = "Large";
  if (pass[pass.find("/")-2]=='s') ls = "Small";
  if (pass[pass.find("/")-2]=='o') ls = "DBD";

  THStack *stk = new THStack("stk",Form("#sqrt{s} = 250 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = 900 fb^{-1}, Cut: No.1~No.7, %s",pol.c_str(),ls.c_str()));

  for(Int_t i=4;i>=0;i--)
    stk->Add(hB[i]);
  stk->Add(hS);
  stk->Draw("HISTE");
  
  stk->GetXaxis()->SetTitle("M_{recoil} [GeV]");
  stk->GetYaxis()->SetTitle("Events / 2.0 GeV");
  ((TGaxis*)stk->GetYaxis())->SetMaxDigits(3);
  stk->GetYaxis()->SetNdivisions(505);
  
  TLegend *leg = new TLegend(0.6,0.55,0.8,0.8);

  leg->AddEntry(hS,"H#rightarrowinv.(BR=10%)","f");
  //leg->AddEntry((TObject*)0," BR = 10%","");
  for(Int_t i=0;i<5;i++)
    leg->AddEntry(hB[i],leg_name[i].c_str(),"f");

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
  lcheat->SetTextColor(lcol);
  lcheat->DrawLatexNDC(.7, .87, cond.c_str());
  c->Modified();

  //gSystem->Exec(Form("mkdir -p pdf/%s",cond.c_str()));
  string savename = "pdf"+pass.substr(6,pass.size()-10)+"pdf";
  c->SaveAs(savename.c_str());
  
}
