#ifndef PlotRecoScan_h
#define PlotRecoScan_h
#include "PlotReco.h"

#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

using namespace std;

class PlotRecoScan {

  
 public:
  
  PlotRecoScan();
  PlotRecoScan(const int& NcombMax, const vector<float>& Pt, const vector<float>& Eta);
  PlotRecoScan(const PlotRecoScan& pobs);
  ~PlotRecoScan();


  int GetNcombMax()const {return NcombMax_;};
  vector<float> GetPtJEt() const {return PtJet_;};
  vector<float> GetEtaJEt() const {return EtaJet_;};
  void Compute();
  void Normalized();
  void Fill(const bool& signal, const double& ChiSquare, const int& Ncomb, const float& Pt, const float& Eta);
  void FillMC(const bool& signal, const double& ChiSquare, const int& Ncomb, const float& Pt, const float& Eta);
  void Write(TFile* fout);
  PlotReco GetPlotReco(const float& Pt, const float& Eta)const;

 private:

  int NcombMax_;
  vector<float> PtJet_;
  vector<float> EtaJet_;
  vector< pair<vector< pair<PlotReco, float > >,float > > PlotReco_;
};

#endif
