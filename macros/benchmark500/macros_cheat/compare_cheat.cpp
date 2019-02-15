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

struct var mrec={"mrec","mrec","mrecCheat",0.,500.,5.,"Recoil Mass [GeV]"};
struct var mz={"mjj","lrzZ.M()","lrzZCheat.M()",0.,200.,2.,"di-jet Mass [GeV]"};
//struct var cmrec={"mrecwoISROverlay",0.,500.,5.,"Recoil Mass [GeV] w/o ISR/Overlay"};
//struct var cmz={"lrzZwoISROverlay.M()",0.,200.,2.,"di-jet Mass [GeV] w/o ISR/Overlay"};

void compare_cheat(struct var plot=mrec, string detector="l5", string pol="lr"){

  setILDStyle();

  string Pol;
  if (pol=="lr") Pol = "(-1.0,+1.0)";
  if (pol=="rl") Pol = "(+1.0,-1.0)";
  
  const Int_t nch = 8;
  string dir[nch] = {"full","cheatOverlay","cheatZbbcc","cheatBS","cheatISR","cheatOZ","cheatOZB","cheatOZBI"};
  string condition[nch] = {"full simulation","w/o Overlay","w/o Z#rightarrowbb/cc","w/o BS","w/o ISR",
			   "w/o Overlay,Z#rightarrowbb/cc","w/o Overlay,Z#rightarrowbb/cc,BS",
			   "w/o Overlay,Z#rightarrowbb/cc,BS,ISR"};
  Int_t color[nch] = {kBlack,kBlue,kAzure+1,kGreen+2,kGreen,kOrange,kRed,kMagenta};
  const Int_t nbin = (plot.xmax-plot.xmin)/plot.binwidth;
  TFile *f[nch];
  TTree *tr;
  TH1F *h[nch];
  Int_t entry[nch];
  Double_t mean[nch];
  Double_t sigma[nch];

  THStack *stk = new THStack("stk",Form("[qqh,h->invisible] iLCSoft: %s, #sqrt{s} = %.0f GeV, Pol.: %s, mILD_%s_o1_v02",
					vILCSOFT.c_str(),fEcm,Pol.c_str(),detector.c_str()));

  for (Int_t i=0;i<nch;i++){
    f[i] = new TFile(Form("../output_new/%s/%s/root_merge/qqh_zz_4n_%s.root",dir[i].c_str(),detector.c_str(),pol.c_str()));
    tr = (TTree*)f[i]->Get("hAnl");
    h[i] = new TH1F(Form("h%s",dir[i].c_str()),"",nbin,plot.xmin,plot.xmax);
    h[i]->SetLineColor(color[i]);
    if (i==0){
      tr->Draw(Form("%s >> h%s",plot.xfull.c_str(),dir[i].c_str()));
      h[i]->SetFillColor(kGray);
    }
    else
      tr->Draw(Form("%s >> h%s",plot.xcheat.c_str(),dir[i].c_str()));
    entry[i] = h[i]->GetEntries();
    mean[i]  = h[i]->GetMean();
    sigma[i] = h[i]->GetStdDev();
    h[i]->Scale((Double_t)1/entry[i]);
    //if (i==3)
    //h[i]->Scale(entry[2]/entry[i]);
    stk->Add(h[i]);
  }
  
  TCanvas *c = new TCanvas("c","c",800,600);
  c->SetGrid();
  stk->Draw("nostackHIST");
  stk->GetYaxis()->SetTitle(Form("Events / %.1f GeV",plot.binwidth));
  stk->GetXaxis()->SetTitle(plot.label.c_str());
  //stk->GetXaxis()->SetTitle(Form("%s",plot.x.c_str()));

  gStyle->SetLegendTextSize(0.03);
  gStyle->SetLegendFont(42);
  TLegend *leg = new TLegend(0.55,0.35,0.9,0.875);
  leg->SetFillStyle(0);
  //leg->AddEntry((TObject*)0,Cut,"");
  for (Int_t i=0;i<nch;i++){  
    leg->AddEntry(h[i],Form("#bf{%s}",condition[i].c_str()),"f");
    leg->AddEntry((TObject*)0,Form("#mu = %.1f, #sigma = %.1f",mean[i],sigma[i]),"");
    //leg->AddEntry((TObject*)0,Form("event = %d",entry[i]),"");
    //leg->AddEntry((TObject*)0,Form(" mean = %.1f",mean[i]),"");
    //leg->AddEntry((TObject*)0,Form("sigma = %.3f",sigma[i]),"");
  }
  leg->Draw();

  gSystem->Exec("mkdir -p pdf");
  c->SaveAs(Form("pdf/compare_cheat_%s_%s_%s.pdf",
		 plot.name.c_str(),detector.c_str(),pol.c_str()));

  //for (Int_t i=0;i<2;i++) f[i]->Close();
  
  return;
}
