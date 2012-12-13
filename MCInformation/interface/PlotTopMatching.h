#ifndef PlotTopMatching_h
#define PlotTopMatching_h
#include "TopMatching.h"

#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;
using namespace TopTree;

class PlotTopMatching {

  
 public:
  
  PlotTopMatching();
  PlotTopMatching(vector<string> jetCollection, vector<string> jetLabels);
  PlotTopMatching(const PlotTopMatching& psel);
  ~PlotTopMatching();
 
  void Compute();
  void Fill(TopMatching* topmatch, vector<TRootJet>& jets, TRootGenEvent& genEvt, int& iter, bool& selected);
  void Fill(TopMatching* topmatch, vector<TRootJet>& jets, TRootNPGenEvent& genEvt, int& iter, bool& selected);
  void FillObs(int nobs, TopMatching* topmatch, vector<TRootJet>& jets, TRootGenEvent& genEvt, int& iter, float& variable);
  void FillObs(int nobs, TopMatching* topmatch, vector<TRootJet>& jets, TRootNPGenEvent& genEvt, int& iter, float& variable);
  void AddObsParam(vector<string> jetCollection, string obsName, string obsLabel, int Nbins, float* bins);
  void Write(TFile* fout);

 private:

      int* NofEvts_;
      int* NofEvtsSel_;
      
      vector<string> jetLabels_;
      vector<string> obsLabels_;
  
      TH1F* h_Matching; 
      TH1F* h_Matching_Sel; 
      TH1F* h_MatchingHadTop; 
      TH1F* h_MatchingHadTop_Sel; 
      vector<TH1F*> h_WMass; 
      vector<TH1F*> h_TopMass; 
      TH1F* h_WMass_RMS; 
      TH1F* h_TopMass_RMS; 
      TH1F* h_WMass_Mean; 
      TH1F* h_TopMass_Mean; 
      TH1F* h_WMass_Res; 
      TH1F* h_TopMass_Res; 
      
      TCanvas* c_MassLeadingJet;
      vector<TH1F*> h_MassLeadingJet; 
      vector<TCanvas*> c_MatchingHadTop_Var;
      vector<vector<TH1F*> > h_MatchingHadTop_vs_Var;
      vector<vector<TH1F*> > h_MatchingHadTop_vs_Var_Ref;
};

#endif
