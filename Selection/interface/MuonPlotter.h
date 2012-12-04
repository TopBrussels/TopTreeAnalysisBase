#ifndef MuonPlotter_h
#define MuonPlotter_h

#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"

// system include files
#include <memory>
#include <vector>
#include <math.h>

//ROOT
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std;
using namespace TopTree;
				
/**
Aim: making standard distribution for muons with a given selection described by a label. Plots are then saved inside a TFile with organized substructure
*/

class MuonPlotter{

   public:
      MuonPlotter(string label);
      MuonPlotter(const MuonPlotter & mp);
      ~MuonPlotter();
      
      void Fill(vector<TRootMuon> muons, vector<TRootJet> seljets);
      void Write(TFile* fout);
      string GetLabel(){return label_;};

   private:
      string label_;
      //plots for highest Pt Muon (HPTMuon)
      TH1F* h_HPTMuon_Pt_;
      TH1F* h_HPTMuon_Eta_;
      TH1F* h_HPTMuon_Type_;/** StandAloneMuon, TrackerMuon, GlobalMuon */
      TH2F* h_HPTMuon_Type_vs_Type_;/** StandAloneMuon, TrackerMuon, GlobalMuon */
      TH1F* h_HPTMuon_ID_;
      TH1F* h_HPTMuon_NHits_;/** Number of Hits in the silicon tracker */
      TH1F* h_HPTMuon_NChiSq_;/** Normalized Chi² of the global track fit */
      TH1F* h_HPTMuon_d0_;
      TH1F* h_HPTMuon_d0Sig_;
      //TH1F* h_HPTMuon_dB_; /** 2D absolute impact parameter (corrected for the beam-spot) */
      TH1F* h_HPTMuon_DeltaRJets_;/** MinDeltaR(muon,jets) */	
      TH1F* h_HPTMuon_RelIso_;
      //plots for all muons
      TH1F* h_AllMuons_mult_;
      TH1F* h_AllMuons_Pt_;
      TH1F* h_AllMuons_Eta_;
      TH1F* h_AllMuons_Type_;/** StandAloneMuon, TrackerMuon, GlobalMuon */
      TH2F* h_AllMuons_Type_vs_Type_;/** StandAloneMuon, TrackerMuon, GlobalMuon */
      TH1F* h_AllMuons_ID_;
      TH1F* h_AllMuons_NHits_;/** Number of Hits in the silicon tracker */
      TH1F* h_AllMuons_NChiSq_;/** Normalized Chi² of the global track fit */
      TH1F* h_AllMuons_d0_;
      TH1F* h_AllMuons_d0Sig_;
      //TH1F* h_AllMuons_dB_; /** 2D absolute impact parameter (corrected for the beam-spot) */
      TH1F* h_AllMuons_DeltaRJets_;/** MinDeltaR(muon,jets) */	
      TH1F* h_AllMuons_RelIso_;
};

#endif
