//#include "../macros_cheat/style.h"
#include "ildStyle.h"

void plot_performance_all() {

  setILDStyle();
  
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetTitleSize(0.06,"xyz");
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(0.035);

  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.0);
  //gStyle->SetLegendBorderSize(0);
  
  TCanvas *c = new TCanvas("c","");
  c->SetGrid();
  TH1F *frame = gPad->DrawFrame(0.5,0.15,1.5,.85);
  //frame->GetXaxis()->SetTitle("JER scale relative to l5 model");
  frame->GetXaxis()->SetTitle("scale factor of JER _{(relative to IDR-L)} [30%/#sqrt{E_{j}}]");
  frame->GetYaxis()->SetTitle("UL_{95% CL} on BR(#font[12]{h#rightarrowinv}) [%]");
  //c->SetLogy();
  TLatex *latex = new TLatex();
  latex->SetTextFont(62);
  latex->SetTextSize(0.08);
  latex->SetTextAlign(12);
  latex->SetTextColor(kGray+3);
  //latex->DrawLatex(.9, 0.9, "#it{ILD preliminary}");
  //latex->DrawLatexNDC(.6, 0.875, "#it{ILD preliminary}");
  latex->DrawLatexNDC(.615, 0.875, "ILD #scale[0.75]{#bf{preliminary}}");
  TLatex *date = new TLatex();
  date->SetTextSize(0.04);
  date->SetTextAlign(12);
  date->SetTextColor(kGray);
  date->DrawLatexNDC(.75, 0.95, "( 27. Feb. 2019 )");
  TMultiGraph *mg = new TMultiGraph();
  TLegend *lg1 = new TLegend(0.175,0.5,0.5,0.9,"#bf{IDR #sqrt{s} = 500 GeV}");
  lg1->SetBorderSize(0);
  lg1->SetFillStyle(0);
  TLegend *lg2 = new TLegend(0.175,0.275,0.5,0.40,"#bf{DBD #sqrt{s} = 250 GeV}");
  lg2->SetBorderSize(0);
  lg2->SetFillStyle(0);
  
  TF1 *fS = new TF1("fS","",0,3);
  //TF1 *f2 = new TF1("f2","10*x^2+x - 50",0,3);
  //TF1 *f3 = new TF1("f3","-x -8",0,3);

  const Int_t Np = 8;
  Double_t xS = (0.505-0.496)/(0.511-0.497)*0.1+1.0;
  
  TGraph *gL[Np];
  TGraph *gS[Np];
  TGraph *gG[Np];
  TGraph *gB[Np];
  
  for (Int_t i=0; i<Np; i++){
    gL[i] = new TGraph(1);
    gS[i] = new TGraph(1);
    gG[i] = new TGraph(1);
    gB[i] = new TGraph(1);
    if (i==0){
      gL[i]->SetMarkerColor(kBlue);
      gS[i]->SetMarkerColor(kRed);
      gL[i]->SetMarkerStyle(29);
      gS[i]->SetMarkerStyle(29);
      gL[i]->SetMarkerSize(1.5);
      gS[i]->SetMarkerSize(1.5);
    }
    else{
      gL[i]->SetMarkerColor(kAzure+1);
      gS[i]->SetMarkerColor(kMagenta);
      gG[i]->SetMarkerColor(kGray+1);
      gB[i]->SetMarkerColor(kTeal-1);
    }
  }
  
  gL[0]->SetPoint(0,1,0.782);
  lg1->AddEntry(gL[0],"IDR-L full simulation","p");
  mg->Add(gL[0]);

  gS[0]->SetPoint(0,xS,0.788);
  lg1->AddEntry(gS[0],"IDR-S full simulation","p");
  mg->Add(gS[0]);
   
  gL[4]->SetLineColor(kAzure+1);
  lg1->AddEntry(gL[4],"IDR-L","l");
  gS[4]->SetLineColor(kMagenta);
  lg1->AddEntry(gS[4],"IDR-S","l");

  
  gL[6]->SetPoint(0,1,0.775);
  gL[6]->SetMarkerStyle(24);
  mg->Add(gL[6]);

  gS[6]->SetPoint(0,xS,0.782);
  gS[6]->SetMarkerStyle(24);
  mg->Add(gS[6]);

  gG[6]->SetMarkerStyle(24);
  lg1->AddEntry(gG[6],"w/o Overlay","p");
  
  
  gL[7]->SetPoint(0,1,0.741);
  gL[7]->SetMarkerStyle(25);
  mg->Add(gL[7]);

  gS[7]->SetPoint(0,xS,0.744);
  gS[7]->SetMarkerStyle(25);
  mg->Add(gS[7]);

  gG[7]->SetMarkerStyle(25);
  lg1->AddEntry(gG[7],"w/o Z#rightarrowbb/cc","p");
  

  gL[5]->SetPoint(0,1,0.749);
  gL[5]->SetMarkerStyle(26);
  mg->Add(gL[5]);

  gS[5]->SetPoint(0,xS,0.757);
  gS[5]->SetMarkerStyle(26);
  mg->Add(gS[5]);

  gG[5]->SetMarkerStyle(26);
  lg1->AddEntry(gG[5],"w/o BS","p");


  gL[4]->SetPoint(0,1,0.632);
  gL[4]->SetMarkerStyle(32);
  mg->Add(gL[4]);

  gS[4]->SetPoint(0,xS,0.638);
  gS[4]->SetMarkerStyle(32);
  mg->Add(gS[4]);

  gG[4]->SetMarkerStyle(32);
  lg1->AddEntry(gG[4],"w/o ISR","p");

  
  gL[1]->SetPoint(0,1,0.726);
  gL[1]->SetMarkerStyle(20);
  mg->Add(gL[1]);

  gS[1]->SetPoint(0,xS,0.731);
  gS[1]->SetMarkerStyle(20);
  mg->Add(gS[1]);

  gG[1]->SetMarkerStyle(20);
  lg1->AddEntry(gG[1],"w/o O/Z","p");

  
  gL[2]->SetPoint(0,1,0.668);
  gL[2]->SetMarkerStyle(21);
  mg->Add(gL[2]);
  gS[2]->SetPoint(0,xS,0.677);
  gS[2]->SetMarkerStyle(21);
  mg->Add(gS[2]);

  gG[2]->SetMarkerStyle(21);
  lg1->AddEntry(gG[2],"w/o O/Z/B","p");

  
  gL[3]->SetPoint(0,1,0.496);
  gL[3]->SetMarkerStyle(22);
  gL[3]->SetMarkerSize(1.2);
  //mg->Add(gL[3]);
  gS[3]->SetPoint(0,xS,0.505);
  gS[3]->SetMarkerStyle(22);
  gS[3]->SetMarkerSize(1.2);
  //mg->Add(gS[3]);

  gG[3]->SetMarkerStyle(22);
  gG[3]->SetMarkerSize(1.2);
  lg1->AddEntry(gG[3],"w/o O/Z/B/I","p");

  
  Double_t fadjust = 1.0;
  
  TGraph *gT = new TGraph(9);
  gT->SetPoint(0,0.6,0.400*fadjust);
  gT->SetPoint(1,0.7,0.426*fadjust);
  gT->SetPoint(2,0.8,0.451*fadjust);
  gT->SetPoint(3,0.9,0.471*fadjust);
  gT->SetPoint(4,1.0,0.497*fadjust);
  gT->SetPoint(5,1.1,0.511*fadjust);
  gT->SetPoint(6,1.2,0.534*fadjust);
  gT->SetPoint(7,1.3,0.549*fadjust);
  gT->SetPoint(8,1.4,0.565*fadjust);
  gT->SetMarkerColor(kGreen+1);
  gT->SetMarkerStyle(34);
  gT->SetLineColor(kGreen);
  gT->SetLineStyle(2);
  lg1->AddEntry(gT,"IDR-L benchmarks","p");
  mg->Add(gT);
  
  mg->Add(gL[3]);
  mg->Add(gS[3]);


  //lg1->AddEntry((TObject*)0,"","");
  gB[1]->SetPoint(0,1,0.247);
  gB[1]->SetMarkerStyle(29);
  gB[1]->SetMarkerSize(1.5);
  mg->Add(gB[1]);
  lg2->AddEntry(gB[1],"full simulation","p");

  gB[2]->SetPoint(0,1,0.198);
  gB[2]->SetMarkerStyle(22);
  gB[2]->SetMarkerSize(1.2);
  //mg->Add(gB[2]);
  lg2->AddEntry(gB[2],"w/o O/Z/B/I","p");

  TGraph *gD = new TGraph(9);
  gD->SetPoint(0,0.6,0.171);
  gD->SetPoint(1,0.7,0.178);
  gD->SetPoint(2,0.8,0.185);
  gD->SetPoint(3,0.9,0.192);
  gD->SetPoint(4,1.0,0.197);
  gD->SetPoint(5,1.1,0.204);
  gD->SetPoint(6,1.2,0.211);
  gD->SetPoint(7,1.3,0.215);
  gD->SetPoint(8,1.4,0.222);
  gD->SetMarkerColor(kOrange+7);
  gD->SetMarkerStyle(34);
  gD->SetLineColor(kOrange+1);
  gD->SetLineStyle(2);
  lg2->AddEntry(gD,"benchmarks","p");
  mg->Add(gD);

  mg->Add(gB[2]);
  
  
  TF1 *fT = new TF1("fT","[0]*x+[1]",0.6,1.4);
  fT->SetLineColor(kGreen);
  fT->SetParameters(0.225,0.32);
  //gT->Fit("fT");
  
  mg->Draw("PL");
  lg1->Draw();
  lg2->Draw();
}
