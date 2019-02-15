
void fit_mrec_bkg(){
  gStyle->SetOptFit();
  TFile *file = TFile::Open("output/Newton_jmass/mrec_cut7_eLpR.root");
  //TTree *hAnl = (TTree*)file->Get("hAnl");
  TCut cut = "flvq1mc<4&&mrecwoISRBSOverlay>0&&mrecwoISRBSOverlay<300";
  //TH1D *htemp = new TH1D("htemp",Form("Cut: %s;Recoil Mass [GeV]",(const char*)cut),150,0.,300.);
  TH1D *htemp = (TH1D*)file->Get("hB_all");
  TCanvas *c = new TCanvas("c","c",800,600);
  c->SetGrid();
  //hAnl->Draw("mrecwoISRBSOverlay >> htemp",cut);
  htemp->Scale((Double_t)1/htemp->GetEntries());
  htemp->Fit("pol3","","",50.,300.);
  //TF1 *BW = (TF1*)gROOT->FindObject("pol4");
  TF1 *BW = htemp->GetFunction("pol3");
  //BW->SetParameters(1,125,20);
  BW->SetLineColor(kBlue);
  BW->Draw("same");
  TLegend *lg = new TLegend(0.09,0.85,0.5,0.9,"[0]*TMath::BreitWigner(x,[1],[2])");
  //gStyle->SetLegendBorderSize(0)
  lg->SetBorderSize(0);
  lg->SetFillStyle(0);
  lg->SetTextSize(0.032);
  lg->SetTextColor(kRed);
  lg->Draw();
  c->SaveAs("pdf/fit_mrec_bkg.pdf");
}
