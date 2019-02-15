#ifndef __STYLE__
#define __STYLE__

#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"

void setILDStyle() {

  int lw = 2;

  //ild TStyle
  TStyle* ildStyle = new  TStyle("ildStyle", "ILD Style");
  
  //set the background color to white
  ildStyle->SetFillColor(10);
  ildStyle->SetFrameFillColor(10);
  ildStyle->SetCanvasColor(10);
  ildStyle->SetPadColor(10);
  ildStyle->SetTitleFillColor(0);
  ildStyle->SetStatColor(10);
  
  //dont put a colored frame around the plots
  ildStyle->SetFrameBorderMode(0);
  ildStyle->SetCanvasBorderMode(0);
  ildStyle->SetPadBorderMode(0);
  ildStyle->SetLegendBorderSize(0);
  
  //use the primary color palette
  ildStyle->SetPalette(1,0);
  
  //set the default line color for a histogram to be black
  ildStyle->SetHistLineColor(kBlack);
  
  //set the default line color for a fit function to be red
  ildStyle->SetFuncColor(kRed);
  
  //make the axis labels black
  ildStyle->SetLabelColor(kBlack,"xyz");
  
  //set the default title color to be black
  ildStyle->SetTitleColor(kBlack);
  
  //set the margins
  ildStyle->SetPadBottomMargin(0.18);
  //ildStyle->SetPadTopMargin(0.08);
  ildStyle->SetPadTopMargin(0.1);
#if 1
  ildStyle->SetPadRightMargin(0.08);
  ildStyle->SetPadLeftMargin(0.17);
#else
  ildStyle->SetPadRightMargin(0.13);
  //  ildStyle->SetPadLeftMargin(0.12);
  ildStyle->SetPadLeftMargin(0.14);
#endif
  
  //set axis label and title text sizes
  ildStyle->SetLabelFont(42,"xyz");
  ildStyle->SetLabelSize(0.04,"xyz");
  ildStyle->SetLabelOffset(0.015,"xyz");
  ildStyle->SetTitleFont(42,"xyz");
  ildStyle->SetTitleSize(0.06,"xyz");
  ildStyle->SetTitleOffset(1.1,"yz");
  ildStyle->SetTitleOffset(1.0,"x");
  ildStyle->SetStatFont(42);
  ildStyle->SetStatFontSize(0.07);
  ildStyle->SetTitleBorderSize(0);
  ildStyle->SetStatBorderSize(0);
  ildStyle->SetTextFont(42);
  
  //set line widths
  ildStyle->SetFrameLineWidth(lw);
  ildStyle->SetFuncWidth(lw);
  ildStyle->SetLineWidth(lw);
  ildStyle->SetHistLineWidth(lw);
  
  //set the number of divisions to show
  ildStyle->SetNdivisions(510, "xy");
  
  //turn off xy grids
  ildStyle->SetPadGridX(0);
  ildStyle->SetPadGridY(0);
  
  //set the tick mark style
  ildStyle->SetPadTickX(1);
  ildStyle->SetPadTickY(1);
  
  //turn off stats
  ildStyle->SetOptStat(0);
  ildStyle->SetOptFit(0);
  
  //marker settings
  //  ildStyle->SetMarkerStyle(20);
  ildStyle->SetMarkerStyle(21);
  //  ildStyle->SetMarkerStyle(25);
  //  ildStyle->SetMarkerSize(1.3);
  ildStyle->SetMarkerSize(0.5);
  ildStyle->SetLineWidth(lw); 
  
  //done
  ildStyle->cd();
  gROOT->ForceStyle();
  gStyle->ls();
  
}

void setMyStyle() {

  gStyle->SetCanvasColor(kWhite);
  gStyle->SetPadColor(kWhite);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.02);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetTitleSize(0.04, "XYZ");

  gStyle->SetTitleXOffset(1.3);
  gStyle->SetTitleYOffset(1.5);

  gStyle->SetLabelFont(42,"xyz");
  gStyle->SetLabelSize(0.06,"xyz");
  gStyle->SetLabelOffset(0.015,"xyz");
  gStyle->SetTitleFont(42,"xyz");
  gStyle->SetTitleSize(0.07,"xyz");
  gStyle->SetTitleOffset(1.1,"yz");
  gStyle->SetTitleOffset(1.0,"x");

  return ;
}

#endif

