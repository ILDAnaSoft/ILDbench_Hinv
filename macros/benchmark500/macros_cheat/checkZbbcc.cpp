#include "global.h"
#include "style.h"

using namespace std;

struct var{
  string name;
  string xfull;
  string xcheat;
  Double_t xmin;
  Double_t xmax;
  Double_t binwidth;
  string label;
};

struct var Left={"Zbbcc","","",0.,150.,2.,"E_{left} [GeV]"};
//struct var cmrec={"mrecwoISROverlay",0.,500.,5.,"Recoil Mass [GeV] w/o ISR/Overlay"};
//struct var cmz={"lrzZwoISROverlay.M()",0.,200.,2.,"di-jet Mass [GeV] w/o ISR/Overlay"};

void checkZbbcc(string detector="l5", string pol="lr", struct var plot=Left){

  setILDStyle();

  string Pol;
  if (pol=="lr") Pol = "(-1.0,+1.0)";
  if (pol=="rl") Pol = "(+1.0,-1.0)";
  
  string dir = "cheatZbbcc";
  //string dir = "cheatOZBvI";
  string var = "lrzLeftmc.E()";
  string condition[3] = {"uds","c","b"};
  TCut cut[3] = {"flvq1mc<4","flvq1mc==4","flvq1mc==5"};
  Int_t lcolor[3] = {kBlack,kOrange+1,kAzure};
  Int_t fcolor[3] = {kGray,kOrange-4,kAzure-4};
  const Int_t nbin = (plot.xmax-plot.xmin)/plot.binwidth;
  TH1D *h[3];
  Int_t entry[3];
  Double_t mean[3];
  Double_t sigma[3];

  THStack *stk = new THStack("stk",Form("[qqh,h->invisible] iLCSoft: %s, #sqrt{s} = %.0f GeV, Pol.: %s, mILD_%s_o1_v02",vILCSOFT.c_str(),fEcm,Pol.c_str(),detector.c_str()));

  TFile *f = new TFile(Form("../output_new/%s/%s/root_merge/qqh_zz_4n_%s.root",dir.c_str(),detector.c_str(),pol.c_str()));
  TTree *tr = (TTree*)f->Get("hAnl");
  for (Int_t i=0;i<3;i++){
    h[i] = new TH1D(Form("h%d",i),"",nbin,plot.xmin,plot.xmax);
    h[i]->SetLineColor(lcolor[i]);
    h[i]->SetFillColor(fcolor[i]);
    tr->Draw(Form("%s >> h%d",var.c_str(),i),cut[i]);
    entry[i] = h[i]->GetEntries();
    mean[i]  = h[i]->GetMean();
    sigma[i] = h[i]->GetStdDev();
    //h[i]->Scale((Double_t)1/entry[i]);
    stk->Add(h[i]);
  }
  
  TCanvas *c = new TCanvas("c","c",800,600);
  c->SetGrid();
  c->SetLogy();
  stk->Draw("HIST");
  stk->GetYaxis()->SetTitle(Form("Events / %.1f GeV",plot.binwidth));
  stk->GetXaxis()->SetTitle(plot.label.c_str());
  //stk->GetXaxis()->SetTitle(Form("%s",plot.x.c_str()));

  gStyle->SetLegendTextSize(0.03);
  gStyle->SetLegendFont(42);
  TLegend *leg = new TLegend(0.55,0.55,0.9,0.875,"#bf{Stacked.}");
  leg->SetFillStyle(0);
  //leg->AddEntry((TObject*)0,Cut,"");
  for (Int_t i=0;i<3;i++){  
    leg->AddEntry(h[i],Form("#bf{%s}",condition[i].c_str()),"f");
    leg->AddEntry((TObject*)0,Form("#mu = %.1f, #sigma = %.1f",mean[i],sigma[i]),"");
    //leg->AddEntry((TObject*)0,Form("event = %d",entry[i]),"");
    //leg->AddEntry((TObject*)0,Form(" mean = %.1f",mean[i]),"");
    //leg->AddEntry((TObject*)0,Form("sigma = %.3f",sigma[i]),"");
  }
  leg->Draw();

  gSystem->Exec("mkdir -p pdf");
  c->SaveAs(Form("pdf_%s/checkZbbcc_%s_%s.pdf",
		 detector.c_str(),detector.c_str(),pol.c_str()));

  //for (Int_t i=0;i<2;i++) f[i]->Close();
  
  return;
}
