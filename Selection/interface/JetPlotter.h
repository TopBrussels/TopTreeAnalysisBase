#ifndef JetPlotter_h
#define JetPlotter_h

#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootCaloJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"

// system include files
#include <memory>
#include <vector>

//ROOT
#include "TFile.h"
#include "TH1F.h"

using namespace std;
using namespace TopTree;
				
/**
Aim: making standard distribution for jets with a given selection described by a label. Plots are then saved inside a TFile with organized substructure
*/

class JetPlotter{

   public:
      JetPlotter(string label);
      JetPlotter(const JetPlotter & mp);
      ~JetPlotter();
      
      void Fill(vector<TRootJet> jets);
      void Fill(vector<TRootCaloJet> jets);
      void Fill(vector<TRootPFJet> jets);
      void Write(TFile* fout);
      string GetLabel(){return label_;};

   private:
      string label_;
      //plots for all jets
      TH1F* h_AllJets_mult_;
      TH1F* h_AllJets_Pt_;
      TH1F* h_AllJets_Eta_;
      TH1F* h_AllJets_partonFlavour_;
      //CaloJets plots
      TH1F* h_AllJets_EMF_;
      TH1F* h_AllJets_fHPD_;
      TH1F* h_AllJets_n90Hits_;
      //btagging
      TH1F* h_AllJets_trackCountingHighPurBJetTag_;
      TH1F* h_AllJets_trackCountingHighEffBJetTag_;
      TH1F* h_AllJets_simpleSecondaryVertexHighEffBJetTag_;
      TH1F* h_AllJets_simpleSecondaryVertexHighPurBJetTag_;
      TH1F* h_AllJets_combinedSecondaryVertexBJetTag_;
      TH1F* h_AllJets_combinedSecondaryVertexMVABJetTag_;
};

#endif
