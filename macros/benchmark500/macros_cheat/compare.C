  
#include "/home/ilc/tianjp/bin/root_macros/style.C"

#include "TROOT.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"


#include "TString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TCut.h"
#include "THStack.h"

#include <TF1.h>
#include <TH1.h>
#include <TRandom.h>
#include <TFile.h>
#include "TNtuple.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

void compare()
{

  setILDStyle();
 gStyle->SetTitleOffset(0.95,"y");

 TFile *f1 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/4f_szeorsw_l_lr.root");
 TFile *f2 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/4f_szeorsw_l_lr.root");
 TFile *f3 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/4f_sznu_l_lr.root");
 TFile *f4 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/4f_sznu_l_lr.root");
 TFile *f5 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/output-N1N2.root");
 TFile *f6 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/output-N1N2.root");
 TFile *f7 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/output-N1N2.root");
 TFile *f8 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/output-N1N2.root");
 TFile *f9 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/output-N1N2.root");
 TFile *f10 = new TFile("/group/ilc/users/jackie/SUSY1/steer/data_final/output-N1N2.root");
 

 TNtuple *n1 = f1->Get("hAnl");
 TNtuple *n2 = f2->Get("hAnl");
 TNtuple *n3 = f3->Get("hAnl");
 TNtuple *n4 = f4->Get("hAnl");
 TNtuple *n5 = f5->Get("hAnl");
 TNtuple *n6 = f6->Get("hAnl");
 TNtuple *n7 = f7->Get("hAnl");
 TNtuple *n8 = f8->Get("hAnl");
 TNtuple *n9 = f9->Get("hAnl");
 TNtuple *n10 = f10->Get("hAnl");


 Int_t nbin = 100;//150 for inv mass.pt   200 for Mrec
 Double_t xmin = 0;//0;  //-0.5-115
 Double_t xmax = 250;//250; //1
 TH1D *h1 = new TH1D("h1","",nbin,xmin,xmax);
 TH1D *h2 = new TH1D("h2","",nbin,xmin,xmax);
 TH1D *h3 = new TH1D("h3","",nbin,xmin,xmax);
 TH1D *h4 = new TH1D("h4","",nbin,xmin,xmax);
 TH1D *h5 = new TH1D("h5","",nbin,xmin,xmax);
 TH1D *h6 = new TH1D("h6","",nbin,xmin,xmax);
 TH1D *h7 = new TH1D("h7","",nbin,xmin,xmax);
 TH1D *h8 = new TH1D("h8","",nbin,xmin,xmax);
 TH1D *h9 = new TH1D("h9","",nbin,xmin,xmax);
 TH1D *h10 = new TH1D("h10","",nbin,xmin,xmax);

 
 h1->SetXTitle("E_{e^{+}e^{-}}  (GeV)");
 h1->SetYTitle("Events");

 h1->Scale(1./h1->Integral());
 
 //for modes
 h1->SetLineColor(1);//4
 h2->SetLineColor(2);//4
 h3->SetLineColor(3);
 h4->SetLineColor(4);//3
 h5->SetLineColor(5);
 h6->SetLineColor(6);//3
 //h7->SetLineColor(7);
 h7->SetLineColor(2);
 h8->SetLineColor(3);//3
 h9->SetLineColor(4);
 h10->SetLineColor(5);//3
 
 h1->SetStats(0);
  
 h1->Draw();
  
 cout<<"h1"<<"  "<<h1->GetEntries()<<<endl;
 
    
 TLegend *leg = new TLegend(0.54 ,0.65 ,0.73,0.88);//good
 leg->AddEntry(h1,"with Minv cut","l"); 
 leg->AddEntry(h2,"no Minv cut","l");       
 leg->SetFillStyle(0); //default
 leg->SetTextSize(0.032);//0.03
 leg->Draw(); 



}
