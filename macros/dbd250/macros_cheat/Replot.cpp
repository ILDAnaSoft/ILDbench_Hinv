#include "style.h"

void Replot(string pass="output/full/mrec_cut7_eRpL.root"){
  
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
  TH1F *hB = (TH1F*)result->Get("hB_all");

  TCanvas *c = new TCanvas("c","",800,650);
  hB->SetFillColor(kGray);
  hB->SetLineColor(kBlack);
  hB->SetFillStyle(1001);
  hS->SetFillColor(kRed-10);
  hS->SetLineColor(kRed);
  hS->SetFillStyle(1001);
  
  string pol;
  //cout << pass[pass.rfind(".")-1] << endl;
  if (pass[pass.rfind(".")-1]=='R') pol = "(-0.8,+0.3)";
  if (pass[pass.rfind(".")-1]=='L') pol = "(+0.8,-0.3)";
  string cond; 
  //cond = pass.substr(pass.find("/")+1,pass.rfind("/")-pass.find("/")-1);
  if (pass.substr(pass.find("/")+1,pass.rfind("/")-pass.find("/")-1)=="full") cond = "#it{full simulation}";
  else cond = pass.substr(pass.find("/")+1,pass.rfind("/")-pass.find("/")-1);
  string ls;
  //ls = pass.substr(pass.find("/")-2,2);
  if (pass[pass.find("/")-2]=='l') ls = "Large";
  if (pass[pass.find("/")-2]=='s') ls = "Small";

  THStack *stk = new THStack("stk",Form("#sqrt{s} = 250 GeV, (Pe^{-},Pe^{+}) = %s, #int#it{L}dt = 900 fb^{-1}, Cut: No.1~No.7, %s",pol.c_str(),ls.c_str()));

  stk->Add(hB);
  stk->Add(hS);
  stk->Draw("HISTE");
  
  stk->GetXaxis()->SetTitle("M_{recoil} [GeV]");
  stk->GetYaxis()->SetTitle("Events / 2.0 GeV");
  ((TGaxis*)stk->GetYaxis())->SetMaxDigits(3);
  stk->GetYaxis()->SetNdivisions(505);
  
  TLegend *leg = new TLegend(0.7,0.65,0.85,0.84);

  leg->AddEntry(hS,"H#rightarrowinv.","f");
  leg->AddEntry((TObject*)0," BR = 10%","");
  leg->AddEntry(hB,"all bkg","f");

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
  lcheat->DrawLatexNDC(.7, .87, cond.c_str());
  c->Modified();

  //gSystem->Exec(Form("mkdir -p pdf/%s",cond.c_str()));
  string savename = "pdf"+pass.substr(6,pass.size()-10)+"pdf";
  c->SaveAs(savename.c_str());
  
}
