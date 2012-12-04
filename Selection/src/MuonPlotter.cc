#include "../interface/MuonPlotter.h"


MuonPlotter::MuonPlotter (string label)
{
  label_ = label;
  h_HPTMuon_Pt_ = new TH1F ( ("h_HPTMuon_Pt_"+label_).c_str(), "Muon Pt", 75, 0, 150);
  h_HPTMuon_Pt_->GetXaxis()->SetTitle("Muon p_{T}");
  h_HPTMuon_Eta_ = new TH1F ( ("h_HPTMuon_Eta_"+label_).c_str(), "Muon Eta", 60, -3, 3);
  h_HPTMuon_Eta_->GetXaxis()->SetTitle("Muon #eta");
  h_HPTMuon_Type_ = new TH1F ( ("h_HPTMuon_Type_"+label_).c_str(), "Muon type", 3, 0, 3);
  h_HPTMuon_Type_->GetXaxis()->SetBinLabel(1,"isStandAloneMuon");
  h_HPTMuon_Type_->GetXaxis()->SetBinLabel(2,"isTrackerMuon");
  h_HPTMuon_Type_->GetXaxis()->SetBinLabel(3,"isGlobalMuon");
  h_HPTMuon_Type_vs_Type_ = new TH2F ( ("h_HPTMuon_Type_vs_Type_"+label_).c_str(), "Muon type vs type", 3, 0, 3, 3, 0, 3);
  h_HPTMuon_Type_vs_Type_->GetXaxis()->SetBinLabel(1,"isStandAloneMuon"); h_HPTMuon_Type_vs_Type_->GetXaxis()->SetBinLabel(2,"isTrackerMuon"); h_HPTMuon_Type_vs_Type_->GetXaxis()->SetBinLabel(3,"isGlobalMuon");
  h_HPTMuon_Type_vs_Type_->GetYaxis()->SetBinLabel(1,"isStandAloneMuon"); h_HPTMuon_Type_vs_Type_->GetYaxis()->SetBinLabel(2,"isTrackerMuon"); h_HPTMuon_Type_vs_Type_->GetYaxis()->SetBinLabel(3,"isGlobalMuon");
  h_HPTMuon_ID_ = new TH1F ( ("h_HPTMuon_ID_"+label_).c_str(), "Muon ID", 2, 0, 2);
  h_HPTMuon_ID_->GetXaxis()->SetTitle("Muon ID");
  h_HPTMuon_NHits_ = new TH1F ( ("h_HPTMuon_NHits_"+label_).c_str(), "NHits (muon)", 50, -0.5, 49.5);
  h_HPTMuon_NHits_->GetXaxis()->SetTitle("Muon Nof Valid Hits");
  h_HPTMuon_NChiSq_ = new TH1F ( ("h_HPTMuon_NChiSq_"+label_).c_str(), "Normalized #chi^{2} of the global fit (muon)", 250, 0., 50);
  h_HPTMuon_NChiSq_->GetXaxis()->SetTitle("#chi^{2}/Ndf");
  h_HPTMuon_d0_ = new TH1F ( ("h_HPTMuon_d0_"+label_).c_str(), "d0 (muon)", 500, -0.1, 0.1);
  h_HPTMuon_d0_->GetXaxis()->SetTitle("d0");
  h_HPTMuon_d0Sig_ = new TH1F ( ("h_HPTMuon_d0Sig_"+label_).c_str(), "d0/#sigma (muon)", 400, 0, 20);
  h_HPTMuon_d0Sig_->GetXaxis()->SetTitle("|d0|/#sigma");
  //h_HPTMuon_dB_ = new TH1F ( ("h_HPTMuon_dB_"+label_).c_str(), "dB (muon)", 250, 0, 0.1);
  //h_HPTMuon_dB_->GetXaxis()->SetTitle("dB");
  h_HPTMuon_DeltaRJets_ = new TH1F ( ("h_HPTMuon_DeltaRJets_"+label_).c_str(), "Min(#DeltaR(#mu,jets))", 35, 0, 3.5);
  h_HPTMuon_DeltaRJets_->GetXaxis()->SetTitle("Min #DeltaR(#mu,jets)");
  h_HPTMuon_RelIso_ = new TH1F( ("h_HPTMuon_RelIso_"+label_).c_str(),"Muon RelIso",50,0,1);
  h_HPTMuon_RelIso_->GetXaxis()->SetTitle("RelIso");
  //
  h_AllMuons_mult_ = new TH1F ( ("h_AllMuons_mult_"+label_).c_str(), "Nof muons", 20, 0, 20);
  h_AllMuons_mult_->GetXaxis()->SetTitle("nof muons");
  h_AllMuons_Pt_ = new TH1F ( ("h_AllMuons_Pt_"+label_).c_str(), "All Muons Pt", 75, 0, 150);
  h_AllMuons_Pt_->GetXaxis()->SetTitle("Muon P_{T}");
  h_AllMuons_Eta_ = new TH1F ( ("h_AllMuons_Eta_"+label_).c_str(), "All Muons Eta", 60, -3, 3);
  h_AllMuons_Eta_->GetXaxis()->SetTitle("Muon #eta");
  h_AllMuons_Type_ = new TH1F ( ("h_AllMuons_Type_"+label_).c_str(), "All Muons type", 3, 0, 3);
  h_AllMuons_Type_->GetXaxis()->SetBinLabel(1,"isStandAloneMuon");
  h_AllMuons_Type_->GetXaxis()->SetBinLabel(2,"isTrackerMuon");
  h_AllMuons_Type_->GetXaxis()->SetBinLabel(3,"isGlobalMuon");
  h_AllMuons_Type_vs_Type_ = new TH2F ( ("h_AllMuons_Type_vs_Type_"+label_).c_str(), "All Muons type vs type", 3, 0, 3, 3, 0, 3);
  h_AllMuons_Type_vs_Type_->GetXaxis()->SetBinLabel(1,"isStandAloneMuon"); h_AllMuons_Type_vs_Type_->GetXaxis()->SetBinLabel(2,"isTrackerMuon"); h_AllMuons_Type_vs_Type_->GetXaxis()->SetBinLabel(3,"isGlobalMuon");
  h_AllMuons_Type_vs_Type_->GetYaxis()->SetBinLabel(1,"isStandAloneMuon"); h_AllMuons_Type_vs_Type_->GetYaxis()->SetBinLabel(2,"isTrackerMuon"); h_AllMuons_Type_vs_Type_->GetYaxis()->SetBinLabel(3,"isGlobalMuon");
  h_AllMuons_ID_ = new TH1F ( ("h_AllMuons_ID_"+label_).c_str(), "All Muons ID", 2, 0, 2);
  h_AllMuons_ID_->GetXaxis()->SetTitle("Muon ID");
  h_AllMuons_NHits_ = new TH1F ( ("h_AllMuons_NHits_"+label_).c_str(), "NHits (all muons)", 50, -0.5, 49.5);
  h_AllMuons_NHits_->GetXaxis()->SetTitle("Muon Nof Valid Hits");
  h_AllMuons_NChiSq_ = new TH1F ( ("h_AllMuons_NChiSq_"+label_).c_str(), "Normalized #chi^{2} of the global fit (all muon)", 250, 0., 50);
  h_AllMuons_NChiSq_->GetXaxis()->SetTitle("#chi^{2}/Ndf");
  h_AllMuons_d0_ = new TH1F ( ("h_AllMuons_d0_"+label_).c_str(), "d0 (all muons)", 500, -0.1, 0.1);
  h_AllMuons_d0_->GetXaxis()->SetTitle("d0");
  h_AllMuons_d0Sig_ = new TH1F ( ("h_AllMuons_d0Sig_"+label_).c_str(), "d0/#sigma (all muons)", 400, 0, 20);
  h_AllMuons_d0Sig_->GetXaxis()->SetTitle("|d0|/#sigma");
  //h_AllMuons_dB_ = new TH1F ( ("h_AllMuons_dB_"+label_).c_str(), "dB (all muons)", 250, 0, 0.1);
  //h_AllMuons_dB_->GetXaxis()->SetTitle("dB");
  h_AllMuons_DeltaRJets_ = new TH1F ( ("h_AllMuons_DeltaRJets_"+label_).c_str(), "Min(#DeltaR(#mu,jets))", 35, 0, 3.5);
  h_AllMuons_DeltaRJets_->GetXaxis()->SetTitle("Min #DeltaR(#mu,jets)");
  h_AllMuons_RelIso_ = new TH1F( ("h_AllMuons_RelIso_"+label_).c_str(),"All Muons RelIso",50,0,1);
  h_AllMuons_RelIso_->GetXaxis()->SetTitle("RelIso");
}

MuonPlotter::MuonPlotter (const MuonPlotter & mp)
{
  h_HPTMuon_Pt_           = mp.h_HPTMuon_Pt_;
  h_HPTMuon_Eta_          = mp.h_HPTMuon_Eta_;
  h_HPTMuon_Type_         = mp.h_HPTMuon_Type_;
  h_HPTMuon_Type_vs_Type_ = mp.h_HPTMuon_Type_vs_Type_;
  h_HPTMuon_ID_           = mp.h_HPTMuon_ID_;
  h_HPTMuon_NHits_        = mp.h_HPTMuon_NHits_;
  h_HPTMuon_NChiSq_       = mp.h_HPTMuon_NChiSq_;
  h_HPTMuon_d0_           = mp.h_HPTMuon_d0_;
  h_HPTMuon_d0Sig_        = mp.h_HPTMuon_d0Sig_;
  //h_HPTMuon_dB_           = mp.h_HPTMuon_dB_;
  h_HPTMuon_DeltaRJets_   = mp.h_HPTMuon_DeltaRJets_;
  h_HPTMuon_RelIso_       = mp.h_HPTMuon_RelIso_;
  //
  h_AllMuons_mult_         = mp.h_AllMuons_mult_;
  h_AllMuons_Pt_           = mp.h_AllMuons_Pt_;
  h_AllMuons_Eta_          = mp.h_AllMuons_Eta_;
  h_AllMuons_Type_         = mp.h_AllMuons_Type_;
  h_AllMuons_Type_vs_Type_ = mp.h_AllMuons_Type_vs_Type_;
  h_AllMuons_ID_           = mp.h_AllMuons_ID_;
  h_AllMuons_NHits_        = mp.h_AllMuons_NHits_;
  h_AllMuons_NChiSq_       = mp.h_AllMuons_NChiSq_;
  h_AllMuons_d0_           = mp.h_AllMuons_d0_;
  h_AllMuons_d0Sig_        = mp.h_AllMuons_d0Sig_;
  //h_AllMuons_dB_           = mp.h_AllMuons_dB_;
  h_AllMuons_DeltaRJets_   = mp.h_AllMuons_DeltaRJets_;
  h_AllMuons_RelIso_       = mp.h_AllMuons_RelIso_;
}

MuonPlotter::~MuonPlotter ()
{
  h_HPTMuon_Pt_->Delete ();
  h_HPTMuon_Eta_->Delete ();
  h_HPTMuon_Type_->Delete ();
  h_HPTMuon_Type_vs_Type_->Delete ();
  h_HPTMuon_ID_->Delete ();
  h_HPTMuon_NHits_->Delete ();
  h_HPTMuon_NChiSq_->Delete ();
  h_HPTMuon_d0_->Delete ();
  h_HPTMuon_d0Sig_->Delete ();
  //h_HPTMuon_dB_->Delete ();
  h_HPTMuon_DeltaRJets_->Delete ();
  h_HPTMuon_RelIso_->Delete();
  //
  h_AllMuons_mult_->Delete ();
  h_AllMuons_Pt_->Delete ();
  h_AllMuons_Eta_->Delete ();
  h_AllMuons_Type_->Delete ();
  h_AllMuons_Type_vs_Type_->Delete ();
  h_AllMuons_ID_->Delete ();
  h_AllMuons_NHits_->Delete ();
  h_AllMuons_NChiSq_->Delete ();
  h_AllMuons_d0_->Delete ();
  h_AllMuons_d0Sig_->Delete ();
  //h_AllMuons_dB_->Delete ();
  h_AllMuons_DeltaRJets_->Delete ();
  h_AllMuons_RelIso_->Delete();
}

void
MuonPlotter::Fill (vector < TRootMuon > muons, vector < TRootJet > selJets)
{
  if (muons.size () > 0) {
    h_HPTMuon_Pt_      ->Fill (muons[0].Pt ());
    h_HPTMuon_Eta_     ->Fill (muons[0].Eta ());
    h_HPTMuon_ID_      ->Fill (muons[0].idGlobalMuonPromptTight ());
    h_HPTMuon_NHits_   ->Fill (muons[0].nofValidHits ());
    h_HPTMuon_NChiSq_  ->Fill (muons[0].chi2());
    h_HPTMuon_d0_      ->Fill (muons[0].d0 ());
    h_HPTMuon_d0Sig_   ->Fill((muons[0].d0error ()>0 ? fabs(muons[0].d0 ())/muons[0].d0error () : -1));
    //h_HPTMuon_dB_      ->Fill (muons[0].dB ());
    if(muons[0].isStandAloneMuon()){
	h_HPTMuon_Type_->Fill(0);
					h_HPTMuon_Type_vs_Type_->Fill(0.,0.);
	if(muons[0].isTrackerMuon())    h_HPTMuon_Type_vs_Type_->Fill(0.,1.);
	if(muons[0].isGlobalMuon())     h_HPTMuon_Type_vs_Type_->Fill(0.,2.);
    }
    if(muons[0].isTrackerMuon()){
	h_HPTMuon_Type_->Fill(1);
	if(muons[0].isStandAloneMuon()) h_HPTMuon_Type_vs_Type_->Fill(1.,0.);
					h_HPTMuon_Type_vs_Type_->Fill(1.,1.);
	if(muons[0].isGlobalMuon())     h_HPTMuon_Type_vs_Type_->Fill(1.,2.);
    }
    if(muons[0].isGlobalMuon()){
	h_HPTMuon_Type_->Fill(2);
	if(muons[0].isStandAloneMuon()) h_HPTMuon_Type_vs_Type_->Fill(2.,0.);
	if(muons[0].isTrackerMuon())    h_HPTMuon_Type_vs_Type_->Fill(2.,1.);
					h_HPTMuon_Type_vs_Type_->Fill(2.,2.);
    }
    h_HPTMuon_RelIso_->Fill(muons[0].relativeIso03());
    float mindRMuJet = 999.;
    TRootJet jet;
    for(unsigned int j=0;j<selJets.size();j++)
    {
    	jet = selJets.at(j);
	float dRMuJet = muons[0].DeltaR(jet);
	if(dRMuJet < mindRMuJet) mindRMuJet = dRMuJet;
    }
    h_HPTMuon_DeltaRJets_->Fill(mindRMuJet);
  }
  h_AllMuons_mult_->Fill (muons.size());
  for (unsigned int i = 0; i < muons.size (); i++) {
    h_AllMuons_Pt_     ->Fill (muons[i].Pt ());
    h_AllMuons_Eta_    ->Fill (muons[i].Eta ());
    h_AllMuons_ID_     ->Fill (muons[i].idGlobalMuonPromptTight ());
    h_AllMuons_NHits_  ->Fill (muons[i].nofValidHits ());
    h_AllMuons_NChiSq_ ->Fill (muons[i].chi2());
    h_AllMuons_d0_     ->Fill (muons[i].d0 ());
    h_AllMuons_d0Sig_  ->Fill((muons[i].d0error ()>0 ? fabs(muons[i].d0 ())/muons[i].d0error () : -1));
    //h_AllMuons_dB_     ->Fill (muons[i].dB ());
    if(muons[i].isStandAloneMuon()){
	h_AllMuons_Type_->Fill(0);
					h_AllMuons_Type_vs_Type_->Fill(0.,0.);
	if(muons[i].isTrackerMuon())    h_AllMuons_Type_vs_Type_->Fill(0.,1.);
	if(muons[i].isGlobalMuon())     h_AllMuons_Type_vs_Type_->Fill(0.,2.);
    }
    if(muons[i].isTrackerMuon()){
	h_AllMuons_Type_->Fill(1);
	if(muons[i].isStandAloneMuon()) h_AllMuons_Type_vs_Type_->Fill(1.,0.);
					h_AllMuons_Type_vs_Type_->Fill(1.,1.);
	if(muons[i].isGlobalMuon())     h_AllMuons_Type_vs_Type_->Fill(1.,2.);
    }
    if(muons[i].isGlobalMuon()){
	h_AllMuons_Type_->Fill(2);
	if(muons[i].isStandAloneMuon()) h_AllMuons_Type_vs_Type_->Fill(2.,0.);
	if(muons[i].isTrackerMuon())    h_AllMuons_Type_vs_Type_->Fill(2.,1.);
					h_AllMuons_Type_vs_Type_->Fill(2.,2.);
    }
    h_AllMuons_RelIso_->Fill(muons[i].relativeIso03());
    float mindRMuJet = 999.;
    TRootJet jet;
    for(unsigned int j=0;j<selJets.size();j++)
    {
    	jet = selJets.at(j);
	float dRMuJet = muons[i].DeltaR(jet);
	if(dRMuJet < mindRMuJet) mindRMuJet = dRMuJet;
    }
    h_AllMuons_DeltaRJets_->Fill(mindRMuJet);
  }
}


void
MuonPlotter::Write (TFile * fout)
{
  string dirname("MuonPlots");
  if(fout->Get(dirname.c_str())==0)
    fout->mkdir(dirname.c_str());
  ((TDirectory *) (fout->Get (dirname.c_str ())))->mkdir (label_.c_str());
  char subdirName[200];
  sprintf (subdirName, "%s/%s", dirname.c_str (), label_.c_str());
  fout->cd (subdirName);
  h_HPTMuon_Pt_          ->Write ();
  h_HPTMuon_Eta_         ->Write ();
  h_HPTMuon_Type_        ->Write ();
  h_HPTMuon_Type_vs_Type_->Write ();
  h_HPTMuon_ID_          ->Write ();
  h_HPTMuon_NHits_       ->Write ();
  h_HPTMuon_NChiSq_      ->Write ();
  h_HPTMuon_d0_          ->Write ();
  h_HPTMuon_d0Sig_       ->Write ();
  //h_HPTMuon_dB_          ->Write ();
  h_HPTMuon_DeltaRJets_  ->Write ();
  h_HPTMuon_RelIso_      ->Write();
  //
  h_AllMuons_mult_        ->Write ();
  h_AllMuons_Pt_          ->Write ();
  h_AllMuons_Eta_         ->Write ();
  h_AllMuons_Type_        ->Write ();
  h_AllMuons_Type_vs_Type_->Write ();
  h_AllMuons_ID_          ->Write ();
  h_AllMuons_NHits_       ->Write ();
  h_AllMuons_NChiSq_      ->Write ();
  h_AllMuons_d0_          ->Write ();
  h_AllMuons_d0Sig_       ->Write ();
  //h_AllMuons_dB_          ->Write ();
  h_AllMuons_DeltaRJets_  ->Write ();
  h_AllMuons_RelIso_      ->Write();
}
