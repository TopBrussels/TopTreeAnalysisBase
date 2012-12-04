#ifndef PlotReco_h
#define PlotReco_h
#include "Observables.h"

#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TClass.h"
#include "stdio.h"
#include "time.h"

using namespace std;

class PlotReco {

  
 public:
  
  PlotReco();
  PlotReco(const int& NcombMax, const float& Pt, const float& Eta);
  PlotReco(const PlotReco& pobs);
  ~PlotReco();


  int GetNcombMax()const {return NcombMax_;};
  float GetPtJEt() const {return PtJet_;};
  float GetEtaJEt() const {return EtaJet_;};
  void Compute();
  void Normalized();
  void Fill(const bool& signal, const double& ChiSquare, const int& Ncomb);
  void FillMC(const bool& signal, const double& ChiSquare, const int& Ncomb);
  void Write(TFile* fout);
  TGraphErrors* GetHistoEffvsPur(int Ncomb=4)const{ if(Ncomb<=NcombMax_) return h_EffvsPur[Ncomb-4]; else return NULL;};
  TGraphErrors** GetAllHistoEffvsPur()const{  return h_EffvsPur;};
  TH1F* GetHistoPurVsMult()const{return h_PurVsMult;};
  TH1F* GetHistoPurVsMultMC()const{return h_PurVsMultMC;};

 private:

  int NcombMax_;
  float PtJet_;
  float EtaJet_;
  TGraphErrors** h_EffvsPur;
  TH1F** h_ChiSquare_Signal;
  TH1F** h_ChiSquare_Bgk;
  TH1F* h_PurVsMult;
  TH1F* h_PurVsMultRef;
  TH1F* h_PurVsMultMC;
  TH1F* h_PurVsMultMCRef;
};

#endif
