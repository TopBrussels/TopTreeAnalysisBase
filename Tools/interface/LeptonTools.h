#ifndef LeptonTools_h
#define LeptonTools_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <TGraphAsymmErrors.h>
#include <TFile.h>

//TopTreeProducer classes
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootElectron.h"

using namespace std;
using namespace TopTree;

class LeptonTools
{
  public:
    LeptonTools(bool verbose = false);
    ~LeptonTools();
    
    void readMuonSF(string IdIsoFile, string trigFileAB, string trigFileC, string trigFileD);
    double getMuonSF(double eta, double pt, string syst = "Nominal");
    
    void readElectronSF(); // hard-coded SF's
    double getElectronSF(double eta, double pt, string syst = "Nominal");
    
  private:
    double getSF(TGraphAsymmErrors* graph, double low, double high);
    
    vector<double> muonPt;
    vector<double> muonPtErr;
    
    vector<double> muonSFEtaLow;
    vector<double> muonSFEtaLowUncPlus;
    vector<double> muonSFEtaLowUncMinus;
    vector<double> muonSFEtaMed;
    vector<double> muonSFEtaMedUncPlus;
    vector<double> muonSFEtaMedUncMinus;
    vector<double> muonSFEtaHigh;
    vector<double> muonSFEtaHighUncPlus;
    vector<double> muonSFEtaHighUncMinus;
    
    vector<double> electronPt;
    vector<double> electronPtErr;
    
    vector<double> electronSFEtaLow;
    vector<double> electronSFEtaLowUnc;
    vector<double> electronSFEtaMed;
    vector<double> electronSFEtaMedUnc;
    vector<double> electronSFEtaHigh;
    vector<double> electronSFEtaHighUnc;
    
    bool verbose_;
    
};

#endif
