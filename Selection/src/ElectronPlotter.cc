#include "../interface/ElectronPlotter.h"


ElectronPlotter::ElectronPlotter (string label)
{
  label_ = label;
  h_HPTElectron_Pt_ = new TH1F ( ("h_HPTElectron_Pt_"+label_).c_str(), "Electron Pt", 75, 0, 150);
  h_HPTElectron_Pt_->GetXaxis()->SetTitle("Electron P_{T}");
  h_HPTElectron_Eta_ = new TH1F ( ("h_HPTElectron_Eta_"+label_).c_str(), "Electron Eta", 60, -3, 3);
  h_HPTElectron_Eta_->GetXaxis()->SetTitle("Electron #eta");
  h_HPTElectron_d0_ = new TH1F ( ("h_HPTElectron_d0_"+label_).c_str(), "d0 (electron)", 50, 0, 0.1);
  h_HPTElectron_d0_->GetXaxis()->SetTitle("d0");
  h_HPTElectron_RelIso_ = new TH1F ( ("h_HPTElectron_RelIso_"+label_).c_str(), "Electron RelIso", 100, 0, 0.2);
  h_HPTElectron_RelIso_->GetXaxis()->SetTitle("RelIso");
  //
  h_AllElectrons_mult_ = new TH1F ( ("h_AllElectrons_mult_"+label_).c_str(), "Nof Electrons ", 30, -0.5, 29.5);
  h_AllElectrons_mult_->GetXaxis()->SetTitle("nof electrons");
  h_AllElectrons_Pt_ = new TH1F ( ("h_AllElectrons_Pt_"+label_).c_str(), "All Electrons Pt", 75, 0, 150);
  h_AllElectrons_Pt_->GetXaxis()->SetTitle("Electron P_{T}");
  h_AllElectrons_Eta_ = new TH1F ( ("h_AllElectrons_Eta_"+label_).c_str(), "All Electrons Eta", 60, -3, 3);
  h_AllElectrons_Eta_->GetXaxis()->SetTitle("Electron #eta");
  h_AllElectrons_d0_ = new TH1F ( ("h_AllElectrons_d0_"+label_).c_str(), "d0 (all electrons)", 50, 0, 0.1);
  h_AllElectrons_d0_->GetXaxis()->SetTitle("d0");
  h_AllElectrons_RelIso_ = new TH1F ( ("h_AllElectrons_RelIso_"+label_).c_str(), "Electron RelIso", 100, 0, 0.2);
  h_AllElectrons_RelIso_->GetXaxis()->SetTitle("RelIso");
}

ElectronPlotter::ElectronPlotter (const ElectronPlotter & mp)
{
  h_HPTElectron_Pt_ = mp.h_HPTElectron_Pt_;
  h_HPTElectron_Eta_ = mp.h_HPTElectron_Eta_;
  h_HPTElectron_d0_ = mp.h_HPTElectron_d0_;
  h_HPTElectron_RelIso_ = mp.h_HPTElectron_RelIso_;
  //
  h_AllElectrons_mult_ = mp.h_AllElectrons_mult_;
  h_AllElectrons_Pt_ = mp.h_AllElectrons_Pt_;
  h_AllElectrons_Eta_ = mp.h_AllElectrons_Eta_;
  h_AllElectrons_d0_ = mp.h_AllElectrons_d0_;
  h_AllElectrons_RelIso_ = mp.h_AllElectrons_RelIso_;
}

ElectronPlotter::~ElectronPlotter ()
{
  h_HPTElectron_Pt_->Delete ();
  h_HPTElectron_Eta_->Delete ();
  h_HPTElectron_d0_->Delete ();
  h_HPTElectron_RelIso_->Delete();
  //
  h_AllElectrons_mult_->Delete ();
  h_AllElectrons_Pt_->Delete ();
  h_AllElectrons_Eta_->Delete ();
  h_AllElectrons_d0_->Delete ();
  h_AllElectrons_RelIso_->Delete();
}

void
ElectronPlotter::Fill (vector < TRootElectron > electrons)
{
  if (electrons.size () > 0) {
    h_HPTElectron_Pt_->Fill (electrons[0].Pt ());
    h_HPTElectron_Eta_->Fill (electrons[0].Eta ());
    h_HPTElectron_d0_->Fill (electrons[0].d0 ());
    h_HPTElectron_RelIso_->Fill((electrons[0].caloIso(3)+electrons[0].trackerIso(3)) / electrons[0].Et());
  }
  h_AllElectrons_mult_->Fill (electrons.size());
  for (unsigned int i = 0; i < electrons.size (); i++) {
    h_AllElectrons_Pt_->Fill (electrons[i].Pt ());
    h_AllElectrons_Eta_->Fill (electrons[i].Eta ());
    h_AllElectrons_RelIso_->Fill((electrons[i].caloIso(3)+electrons[i].trackerIso(3)) / electrons[i].Et());
  }
}


void
ElectronPlotter::Write (TFile * fout)
{
  string dirname("ElectronPlots");
  if(fout->Get(dirname.c_str())==0)
    fout->mkdir(dirname.c_str());
  ((TDirectory *) (fout->Get (dirname.c_str ())))->mkdir (label_.c_str());
  char subdirName[200];
  sprintf (subdirName, "%s/%s", dirname.c_str (), label_.c_str());
  fout->cd (subdirName);
  h_HPTElectron_Pt_->Write ();
  h_HPTElectron_Eta_->Write ();
  h_HPTElectron_d0_->Write ();
  h_HPTElectron_RelIso_->Write ();
  //
  h_AllElectrons_mult_->Write ();
  h_AllElectrons_Pt_->Write ();
  h_AllElectrons_Eta_->Write ();
  h_AllElectrons_d0_->Write ();
  h_AllElectrons_RelIso_->Write ();
}
