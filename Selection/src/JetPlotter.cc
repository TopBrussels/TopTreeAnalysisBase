#include "../interface/JetPlotter.h"


JetPlotter::JetPlotter (string label)
{
  label_ = label;
  h_AllJets_mult_ = new TH1F ( ("h_AllJets_mult_"+label_).c_str(), "Nof jets", 50, -0.5, 49.5);
  h_AllJets_mult_->GetXaxis ()->SetTitle ("nof jets");
  h_AllJets_Pt_ = new TH1F ( ("h_AllJets_Pt_"+label_).c_str(), "Jets PT", 100, 0, 200);
  h_AllJets_Pt_->GetXaxis ()->SetTitle ("jet p_{T}");
  h_AllJets_Eta_ = new TH1F ( ("h_AllJets_Eta_"+label_).c_str(), "Jets #eta", 50, -2.5, 2.5);
  h_AllJets_Eta_->GetXaxis ()->SetTitle ("jet #eta");
  h_AllJets_EMF_ = new TH1F ( ("h_AllJets_EMF_"+label_).c_str(), "Jets: EMF", 50, 0, 1.0001);
  h_AllJets_EMF_->GetXaxis ()->SetTitle ("EMF");
  h_AllJets_fHPD_ = new TH1F ( ("h_AllJets_fHPD_"+label_).c_str(), "Jets: fHPD", 50, 0, 1.0001);
  h_AllJets_fHPD_->GetXaxis ()->SetTitle ("fHPD");
  h_AllJets_n90Hits_ = new TH1F ( ("h_AllJets_n90Hits_"+label_).c_str(), "Jets: n90Hits", 50, -0.5, 49.5);
  h_AllJets_n90Hits_->GetXaxis ()->SetTitle ("n90Hits");
  h_AllJets_trackCountingHighPurBJetTag_ = new TH1F ( ("h_AllJets_trackCountingHighPurBJetTag_"+label_).c_str(), "Jets: trackCountingHighPurBJetTag", 360, -30, 30);
  h_AllJets_trackCountingHighPurBJetTag_->GetXaxis ()->SetTitle ("b-discri");
  h_AllJets_trackCountingHighEffBJetTag_ = new TH1F ( ("h_AllJets_trackCountingHighEffBJetTag_"+label_).c_str(), "Jets: trackCountingHighEffBJetTag", 360, -30, 30);
  h_AllJets_trackCountingHighEffBJetTag_->GetXaxis ()->SetTitle ("b-discri");
  h_AllJets_simpleSecondaryVertexHighEffBJetTag_ = new TH1F ( ("h_AllJets_simpleSecondaryVertexHighEffBJetTag"+label_).c_str(), "Jets: simpleSecondaryVertexBJetTag", 360, 0, 6);
  h_AllJets_simpleSecondaryVertexHighEffBJetTag_->GetXaxis ()->SetTitle ("b-discri");
  h_AllJets_simpleSecondaryVertexHighPurBJetTag_ = new TH1F ( ("h_AllJets_simpleSecondaryVertexHighPurBJetTag"+label_).c_str(), "Jets: simpleSecondaryVertexBJetTag", 360, 0, 6);
  h_AllJets_simpleSecondaryVertexHighPurBJetTag_->GetXaxis ()->SetTitle ("b-discri");
  h_AllJets_combinedSecondaryVertexBJetTag_ = new TH1F ( ("h_AllJets_combinedSecondaryVertexBJetTag_"+label_).c_str(), "Jets: combinedSecondaryVertexBJetTag", 100, 0, 1.0001);
  h_AllJets_combinedSecondaryVertexBJetTag_->GetXaxis ()->SetTitle ("b-discri");
  h_AllJets_combinedSecondaryVertexMVABJetTag_ = new TH1F ( ("h_AllJets_combinedSecondaryVertexMVABJetTag_"+label_).c_str(), "Jets: combinedSecondaryVertexMVABJetTag", 100, 0, 1.0001);
  h_AllJets_combinedSecondaryVertexMVABJetTag_->GetXaxis ()->SetTitle ("b-discri");
  h_AllJets_partonFlavour_ = new TH1F ( ("h_AllJets_partonFlavour_"+label_).c_str(), "Jets: parton flavour", 44, -22, 22);
}

JetPlotter::JetPlotter (const JetPlotter & mp)
{
  h_AllJets_mult_ = mp.h_AllJets_mult_;
  h_AllJets_Pt_ = mp.h_AllJets_Pt_;
  h_AllJets_Eta_ = mp.h_AllJets_Eta_;
  h_AllJets_EMF_ = mp.h_AllJets_EMF_;
  h_AllJets_fHPD_ = mp.h_AllJets_fHPD_;
  h_AllJets_n90Hits_ = mp.h_AllJets_n90Hits_;
  h_AllJets_trackCountingHighPurBJetTag_ = mp.h_AllJets_trackCountingHighPurBJetTag_;
  h_AllJets_trackCountingHighEffBJetTag_ = mp.h_AllJets_trackCountingHighEffBJetTag_;
  h_AllJets_simpleSecondaryVertexHighEffBJetTag_ = mp.h_AllJets_simpleSecondaryVertexHighEffBJetTag_;
  h_AllJets_simpleSecondaryVertexHighPurBJetTag_ = mp.h_AllJets_simpleSecondaryVertexHighPurBJetTag_;
  h_AllJets_combinedSecondaryVertexBJetTag_ = mp.h_AllJets_combinedSecondaryVertexBJetTag_;
  h_AllJets_combinedSecondaryVertexMVABJetTag_ = mp.h_AllJets_combinedSecondaryVertexMVABJetTag_;
  h_AllJets_partonFlavour_  = mp.h_AllJets_partonFlavour_;
}

JetPlotter::~JetPlotter ()
{
  h_AllJets_mult_->Delete ();
  h_AllJets_Pt_->Delete ();
  h_AllJets_Eta_->Delete ();
  h_AllJets_EMF_->Delete ();
  h_AllJets_fHPD_->Delete ();
  h_AllJets_n90Hits_->Delete ();
  h_AllJets_trackCountingHighPurBJetTag_->Delete ();
  h_AllJets_trackCountingHighEffBJetTag_->Delete ();
  h_AllJets_simpleSecondaryVertexHighEffBJetTag_->Delete ();
  h_AllJets_simpleSecondaryVertexHighPurBJetTag_->Delete ();
  h_AllJets_combinedSecondaryVertexBJetTag_->Delete ();
  h_AllJets_combinedSecondaryVertexMVABJetTag_->Delete ();
  h_AllJets_partonFlavour_->Delete();
}

void
JetPlotter::Fill (vector < TRootJet > jets)
{
  h_AllJets_mult_->Fill (jets.size ());
  for (unsigned int i = 0; i < jets.size (); i++) {
    h_AllJets_Pt_->Fill (jets[i].Pt ());
    h_AllJets_Eta_->Fill (jets[i].Eta ());
    h_AllJets_trackCountingHighPurBJetTag_       ->Fill (jets[i].btag_trackCountingHighPurBJetTags());
    h_AllJets_trackCountingHighEffBJetTag_       ->Fill (jets[i].btag_trackCountingHighEffBJetTags());
    h_AllJets_simpleSecondaryVertexHighEffBJetTag_      ->Fill (jets[i].btag_simpleSecondaryVertexHighEffBJetTags());
    h_AllJets_simpleSecondaryVertexHighPurBJetTag_      ->Fill (jets[i].btag_simpleSecondaryVertexHighPurBJetTags());
    h_AllJets_combinedSecondaryVertexBJetTag_    ->Fill (jets[i].btag_combinedSecondaryVertexBJetTags());
    h_AllJets_combinedSecondaryVertexMVABJetTag_ ->Fill (jets[i].btag_combinedSecondaryVertexMVABJetTags());
    h_AllJets_partonFlavour_                     ->Fill (jets[i].partonFlavour());
  }
}

void
JetPlotter::Fill (vector < TRootCaloJet > jets)
{
  vector < TRootJet > vjets;
  for (unsigned int i = 0; i < jets.size (); i++)
    vjets.push_back ((TRootJet) jets[i]);
  Fill (vjets);
  for (unsigned int i = 0; i < jets.size (); i++) {
    h_AllJets_EMF_->Fill (jets[i].ecalEnergyFraction ());
    h_AllJets_fHPD_->Fill (jets[i].fHPD ());
    h_AllJets_n90Hits_->Fill (jets[i].n90Hits ());
  }
}

void
JetPlotter::Fill (vector < TRootPFJet > jets)
{
  vector < TRootJet > vjets;
  for (unsigned int i = 0; i < jets.size (); i++)
    vjets.push_back ((TRootJet) jets[i]);
  Fill (vjets);
}

void
JetPlotter::Write (TFile * fout)
{
  string dirname ("JetPlots");
  if(fout->Get(dirname.c_str())==0)
    fout->mkdir(dirname.c_str());
  ((TDirectory *) (fout->Get (dirname.c_str ())))->mkdir (label_.c_str ());
  char subdirName[200];
  sprintf (subdirName, "%s/%s", dirname.c_str (), label_.c_str ());
  fout->cd (subdirName);
  h_AllJets_mult_->Write ();
  h_AllJets_Pt_->Write ();
  h_AllJets_Eta_->Write ();
  h_AllJets_EMF_->Write ();
  h_AllJets_fHPD_->Write ();
  h_AllJets_n90Hits_->Write ();
  h_AllJets_trackCountingHighPurBJetTag_->Write ();
  h_AllJets_trackCountingHighEffBJetTag_->Write ();
  h_AllJets_simpleSecondaryVertexHighEffBJetTag_->Write ();
  h_AllJets_simpleSecondaryVertexHighPurBJetTag_->Write ();
  h_AllJets_combinedSecondaryVertexBJetTag_->Write ();
  h_AllJets_combinedSecondaryVertexMVABJetTag_->Write ();
  h_AllJets_partonFlavour_->Write ();
}
