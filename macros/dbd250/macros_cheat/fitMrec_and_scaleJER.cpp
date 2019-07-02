
void fitMrec_and_scaleJER(string l5s5="l5", Int_t iPol=-1) {

  gStyle->SetOptFit();
  
  TFile *result;
  if (iPol==-1) result = new TFile(Form("output_%s/cheatAll/Newton_jmass/mrec_cut8_eLpR.root",l5s5.c_str()));
  if (iPol== 1) result = new TFile(Form("output_%s/cheatAll/Newton_jmass/mrec_cut8_eRpL.root",l5s5.c_str()));
  TFile *ref;
  if (iPol==-1) ref = new TFile(Form("output_%s/cheatAll/Newton_jmass/mrec_cut7_eLpR.root",l5s5.c_str()));
  if (iPol== 1) ref = new TFile(Form("output_%s/cheatAll/Newton_jmass/mrec_cut7_eRpL.root",l5s5.c_str()));

  TH1F *hS = (TH1F*)result->Get("hS_hinv");
  TH1F *hS_ref = (TH1F*)ref->Get("hS_hinv");
  Double_t NS = hS->GetEntries();
  //Double_t NS = hS->Integral(hS->FindBin(0),hS->FindBin(250));
  //Double_t NS_ref = hS_ref->GetEntries();
  Double_t NS_ref = hS_ref->Integral(hS_ref->FindBin(0),hS_ref->FindBin(250));
  TCanvas *c = new TCanvas("c","");
  c->SetGrid();
  hS->Scale((Double_t)1/NS);
  TF1 *GG = new TF1("GG","[2]*TMath::Gaus(x,[0],[1])+[5]*TMath::Gaus(x,[3],[4])",0,250);
  GG->SetParameters(125,10,0.07,125,30,0.03);
  hS->Fit("GG");
  GG->Draw("same");
  Double_t IntGG = GG->Integral(0,250);
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
    f[i] = new TF1(Form("f%d",i),"[6]*([2]*TMath::Gaus(x,[0],[1])+[5]*TMath::Gaus(x,[3],[4]))",0,250);
    f[i]->SetParameters(par[0],factor[i]*par[1],par[2],par[3],factor[i]*par[4],par[5],1.0);
    Int[i] = f[i]->Integral(0,250);
    cout << "Int[" << i << "] = " << Int[i];
    f[i]->SetParameter(6,(Double_t)1.0/Int[i]);
    if (i==0) f[i]->Draw();
    else f[i]->Draw("same");
    cout << "-> " << (Double_t)f[i]->Integral(0,250) << endl;
  }

  TCanvas *ctemp = new TCanvas("ctemp","");
  ctemp->Divide(3,3);
  TH1F *hS_new[Ntemp];
  for (Int_t i=0;i<Ntemp;i++){
    ctemp->cd(i+1);
    hS_new[i] = new TH1F(Form("hS%d",i),Form("%.1f",factor[i]),250/5,0,250);
    hS_new[i]->SetFillColor(kRed);
    hS_new[i]->SetLineColor(kRed);
    hS_new[i]->SetFillStyle(3002);
    hS_new[i]->FillRandom(Form("f%d",i),NS_ref);
    hS_new[i]->Draw();
    //hS_new[i]->GetXaxis()->SetRangeUser(0,250);
    cout << hS_new[i]->Integral(hS_new[i]->FindBin(0),hS_new[i]->FindBin(250)) << endl;
  }
  ctemp->Update();

  TFile *output;
  if (iPol==-1 ) output = new TFile(Form("output_%s/cheatAll/Newton_jmass/mrec_temp_eLpR.root",l5s5.c_str()),"RECREATE");
  if (iPol== 1 ) output = new TFile(Form("output_%s/cheatAll/Newton_jmass/mrec_temp_eRpL.root",l5s5.c_str()),"RECREATE");

  for (Int_t i=0;i<Ntemp;i++){
    f[i]->Write();
    hS_new[i]->Write();
  }
  //c->Write();
  //cf->Write();
  //ctemp->Write();
  output->Close();
}
