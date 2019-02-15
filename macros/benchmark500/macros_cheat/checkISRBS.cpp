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

struct var isrbs={"ISRBS","","",-10.,300.,5.,"E [GeV]"};
//struct var cmrec={"mrecwoISROverlay",0.,500.,5.,"Recoil Mass [GeV] w/o ISR/Overlay"};
//struct var cmz={"lrzZwoISROverlay.M()",0.,200.,2.,"di-jet Mass [GeV] w/o ISR/Overlay"};

void checkISRBS(string detector="s5", string pol="lr", struct var plot=isrbs){

  setILDStyle();

  string Pol;
  if (pol=="lr") Pol = "(-1.0,+1.0)";
  if (pol=="rl") Pol = "(+1.0,-1.0)";
  
  string dir = "cheatOZB";
  string condition[2] = {"Energy of ISR","Energy of Beamstrahlung"};
  string var[2] = {"lrzISR1mc.E()+lrzISR2mc.E()","500-lrzEcm.E()"};
  Int_t color[2] = {kRed,kBlue};
  const Int_t nbin = (plot.xmax-plot.xmin)/plot.binwidth;
  TH1D *h[2];
  Int_t entry[2];
  Double_t mean[2];
  Double_t sigma[2];

  THStack *stk = new THStack("stk",Form("[qqh,h->invisible] iLCSoft: %s, #sqrt{s} = %.0f GeV, Pol.: %s, mILD_%s_o1_v02",vILCSOFT.c_str(),fEcm,Pol.c_str(),detector.c_str()));

  TFile *f = new TFile(Form("../output_new/%s/%s/root_merge/qqh_zz_4n_%s.root",dir.c_str(),detector.c_str(),pol.c_str()));
  TTree *tr = (TTree*)f->Get("hAnl");
  for (Int_t i=0;i<2;i++){
    h[i] = new TH1D(Form("h%d",i),"",nbin,plot.xmin,plot.xmax);
    h[i]->SetLineColor(color[i]);
    tr->Draw(Form("%s >> h%d",var[i].c_str(),i));
    entry[i] = h[i]->GetEntries();
    mean[i]  = h[i]->GetMean();
    sigma[i] = h[i]->GetStdDev();
    h[i]->Scale((Double_t)1/entry[i]);
    stk->Add(h[i]);
  }
  
  TCanvas *c = new TCanvas("c","c",800,600);
  c->SetGrid();
  c->SetLogy();
  stk->Draw("nostackHIST");
  stk->GetYaxis()->SetTitle(Form("/ %.1f GeV",plot.binwidth));
  stk->GetXaxis()->SetTitle(plot.label.c_str());
  //stk->GetXaxis()->SetTitle(Form("%s",plot.x.c_str()));

  gStyle->SetLegendTextSize(0.03);
  gStyle->SetLegendFont(42);
  TLegend *leg = new TLegend(0.55,0.55,0.9,0.875);
  leg->SetFillStyle(0);
  //leg->AddEntry((TObject*)0,Cut,"");
  for (Int_t i=0;i<2;i++){  
    leg->AddEntry(h[i],Form("#bf{%s}",condition[i].c_str()),"f");
    leg->AddEntry((TObject*)0,Form("#mu = %.1f, #sigma = %.1f",mean[i],sigma[i]),"");
    //leg->AddEntry((TObject*)0,Form("event = %d",entry[i]),"");
    //leg->AddEntry((TObject*)0,Form(" mean = %.1f",mean[i]),"");
    //leg->AddEntry((TObject*)0,Form("sigma = %.3f",sigma[i]),"");
  }
  leg->Draw();

  gSystem->Exec("mkdir -p pdf");
  c->SaveAs(Form("pdf/checkISRBS_%s_%s.pdf",
		 detector.c_str(),pol.c_str()));

  //for (Int_t i=0;i<2;i++) f[i]->Close();
  
  return;
}
