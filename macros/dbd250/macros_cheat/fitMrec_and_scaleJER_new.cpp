
void fitMrec_and_scaleJER_new(Int_t iPol = -1, string l5s5 = "o1") {

  gStyle->SetOptFit();

  Double_t xmin = 100.;
  Double_t xmax = 160.;
  Double_t bw   = 2.;
  Int_t nbin = (xmax-xmin)/bw;
  
  TFile *result;
  if (iPol==-1) result = new TFile(Form("output_%s/cheatOZBI/mrec_cut7_eLpR.root",l5s5.c_str()));
  if (iPol== 1) result = new TFile(Form("output_%s/cheatOZBI/mrec_cut7_eRpL.root",l5s5.c_str()));

  TH1F *hS = (TH1F*)result->Get("hS_hinv");
  Double_t NS = hS->GetEntries();
  cout << "hS->GetEntries() = " << NS << endl;
  Double_t NI = hS->Integral(hS->FindBin(xmin),hS->FindBin(xmax));
  cout << "hS->Integral() = " << NI << endl;
  TCanvas *c = new TCanvas("c","");
  c->SetGrid();
  hS->Scale((Double_t)1/NI);
  TF1 *GG = new TF1("GG","[2]*TMath::Gaus(x,[0],[1])+[5]*TMath::Gaus(x,[3],[4])",xmin,xmax);
  GG->SetParameters(125,10,0.07,125,30,0.03);
  hS->Fit("GG");
  GG->Draw("same");
  Double_t IntGG = GG->Integral(xmin,xmax);
  cout << "IntGG = " << IntGG << endl;
  Double_t par[6];
  for (Int_t i=0;i<6;i++)
    par[i]=GG->GetParameter(i);

  TCanvas *cf = new TCanvas("cf","");
  const Int_t Ntemp = 9;
  TF1 *f[Ntemp];
  Double_t Int[Ntemp];
  Double_t factor[Ntemp] = {0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4};
  for (Int_t i=0;i<Ntemp;i++){
    f[i] = new TF1(Form("f%d",i),"[6]*([2]*TMath::Gaus(x,[0],[1])+[5]*TMath::Gaus(x,[3],[4]))",xmin,xmax);
    f[i]->SetParameters(par[0],factor[i]*par[1],par[2],par[3],factor[i]*par[4],par[5],1.0);
    Int[i] = f[i]->Integral(xmin,xmax);
    cout << "Int[" << i << "] = " << Int[i];
    f[i]->SetParameter(6,(Double_t)1.0/Int[i]);
    if (i==0) f[i]->Draw();
    else f[i]->Draw("same");
    cout << "-> " << (Double_t)f[i]->Integral(xmin,xmax) << endl;
  }

  TCanvas *ctemp = new TCanvas("ctemp","");
  ctemp->Divide(3,3);
  TH1F *hS_new[Ntemp];
  for (Int_t i=0;i<Ntemp;i++){
    ctemp->cd(i+1);
    hS_new[i] = new TH1F(Form("hS%d",i),Form("%.1f",factor[i]),nbin,xmin,xmax);
    hS_new[i]->SetFillColor(kRed);
    hS_new[i]->SetLineColor(kRed);
    hS_new[i]->SetFillStyle(3002);
    hS_new[i]->FillRandom(Form("f%d",i),(Int_t)NI);
    hS_new[i]->Draw();
    //hS_new[i]->GetXaxis()->SetRangeUser(0,250);
    cout << "hS_new[" << i << "]->Integral() = " << hS_new[i]->Integral(hS_new[i]->FindBin(xmin),hS_new[i]->FindBin(xmax)) << endl;
  }
  //cout << "hS_new->Integral() = " << hS_new[(Ntemp-1)/2]->Integral(hS_new[(Ntemp-1)/2]->FindBin(xmin),hS_new[(Ntemp-1)/2]->FindBin(xmax)) << endl;
  ctemp->Update();

  gSystem->Exec(Form("mkdir -p output_%s/cheatTemp",l5s5.c_str()));
  TFile *output;
  if (iPol==-1 ) output = new TFile(Form("output_%s/cheatTemp/mrec_temp_eLpR.root",l5s5.c_str()),"RECREATE");
  if (iPol== 1 ) output = new TFile(Form("output_%s/cheatTemp/mrec_temp_eRpL.root",l5s5.c_str()),"RECREATE");

  for (Int_t i=0;i<Ntemp;i++){
    f[i]->Write();
    hS_new[i]->Write();
  }
  //c->Write();
  //cf->Write();
  //ctemp->Write();
  //output->Close();
}
