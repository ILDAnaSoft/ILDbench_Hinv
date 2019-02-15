#include "style.h"

void Replot_scaled(Int_t iPol = -1, string ls = "l5", string cheat="All"){

  // parameter setting ------------------------
  const Double_t xmin = 0.;
  const Double_t xmax = 250.;
  //const Int_t nbin = (xmax-xmin)/binwidth;
  // ------------------------------------------
  
  setILDStyle();
  gStyle->SetLegendTextSize(0.04);
  gStyle->SetPadTopMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetTitleOffset(1.0,"y");
  gStyle->SetTitleSize(0.06,"xy");
  gStyle->SetTitleY(0.998);

  TFile *result;
  TFile *ref;
  string fPol;
  string fpol;
  if (iPol==-1) {
    fPol = "-0.8,+0.3";
    fpol = "eLpR";
  }
  if (iPol== 1) {
    fPol = "+0.8,-0.3";
    fpol = "eRpL";
  }
  result = new TFile(Form("output_%s/cheat%s/Newton_jmass/mrec_cut8_%s.root",ls.c_str(),cheat.c_str(),fpol.c_str()));
  ref = new TFile(Form("output_%s/cheat%s/Newton_jmass/mrec_cut7_%s.root",ls.c_str(),cheat.c_str(),fpol.c_str()));
  
  TH1F *hS = (TH1F*)result->Get("hS_hinv");
  TH1F *hS_ref = (TH1F*)ref->Get("hS_hinv");
  TH1F *hB = (TH1F*)result->Get("hB_all");

  Double_t NS = hS->Integral(hS->FindBin(xmin),hS->FindBin(xmax));
  Double_t NS_ref = hS_ref->Integral(hS_ref->FindBin(xmin),hS_ref->FindBin(xmax));
  
  Double_t weight_S = NS_ref/NS;
  hS->Scale(weight_S);

  TCanvas *c = new TCanvas("c","",800,600);
  hB->SetLineColor(kBlack);
  hB->SetFillColor(kGray);
  hB->SetFillStyle(1001);
  hS->SetLineColor(kRed);
  hS->SetFillColor(kRed-10);
  hS->SetFillStyle(1001);
  
  THStack *stk = new THStack("stk",Form("#sqrt{s} = 500 GeV, (Pe^{-},Pe^{+}) = (%s), #int#it{L}dt = 1600 fb^{-1}, Cut: No.1~No.8, %s",fPol.c_str(),ls.c_str()));

  stk->Add(hB);
  stk->Add(hS);
  stk->Draw("HIST");
  
  stk->GetXaxis()->SetTitle("M_{recoil} [GeV]");
  stk->GetYaxis()->SetTitle("Events / 5.0 GeV");
  ((TGaxis*)stk->GetYaxis())->SetMaxDigits(3);
  
  TLegend *leg = new TLegend(0.7,0.65,0.85,0.84);

  leg->AddEntry(hS,"H#rightarrowinv.","f");
  leg->AddEntry((TObject*)0," BR = 10%","");
  leg->AddEntry(hB,"all bkg","f");

  leg->SetFillStyle(0);
  leg->Draw();
  c->Modified();

  gSystem->Exec(Form("mkdir -p pdf_%s/cheat%s",ls.c_str(),cheat.c_str()));
  c->SaveAs(Form("pdf_%s/cheat%s/mrec_scaled_%s.pdf",ls.c_str(),cheat.c_str(),fpol.c_str()));
  
}
