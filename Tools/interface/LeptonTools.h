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
		void readMuonSF(string IdFile, string IsoFile, string trigFile);  //for full 2012 ReReco data; https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs#Muon_reconstruction_identificati
    double getMuonSF(double eta, double pt, string syst = "Nominal");
    double getMuonIdIsoSF(double eta, double pt, string syst = "Nominal");
		
    void readElectronSF(); // hard-coded SF's
    double getElectronSF(double eta, double pt, string syst = "Nominal");
    double getElectronIdIsoSF(double eta, double pt, string syst = "Nominal");
		
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
		
		vector<double> muonTrigSFEtaLow;
    vector<double> muonTrigSFEtaLowUncPlus;
    vector<double> muonTrigSFEtaLowUncMinus;
    vector<double> muonTrigSFEtaMed;
    vector<double> muonTrigSFEtaMedUncPlus;
    vector<double> muonTrigSFEtaMedUncMinus;
    vector<double> muonTrigSFEtaHigh;
    vector<double> muonTrigSFEtaHighUncPlus;
    vector<double> muonTrigSFEtaHighUncMinus;
		
		vector<double> muonIdIsoSFEtaLow;
    vector<double> muonIdIsoSFEtaLowUncPlus;
    vector<double> muonIdIsoSFEtaLowUncMinus;
    vector<double> muonIdIsoSFEtaMed;
    vector<double> muonIdIsoSFEtaMedUncPlus;
    vector<double> muonIdIsoSFEtaMedUncMinus;
    vector<double> muonIdIsoSFEtaHigh;
    vector<double> muonIdIsoSFEtaHighUncPlus;
    vector<double> muonIdIsoSFEtaHighUncMinus;
		vector<double> muonIdIsoSFEtaVeryHigh;
    vector<double> muonIdIsoSFEtaVeryHighUncPlus;
    vector<double> muonIdIsoSFEtaVeryHighUncMinus;
    
    vector<double> electronPt;
    vector<double> electronPtErr;
    
    vector<double> electronSFEtaLow;
    vector<double> electronSFEtaLowUnc;
    vector<double> electronSFEtaMed;
    vector<double> electronSFEtaMedUnc;
    vector<double> electronSFEtaHigh;
    vector<double> electronSFEtaHighUnc;
		
		vector<double> electronIdIsoSFEtaLow;
    vector<double> electronIdIsoSFEtaLowUnc;
    vector<double> electronIdIsoSFEtaMed;
    vector<double> electronIdIsoSFEtaMedUnc;
    vector<double> electronIdIsoSFEtaHigh;
    vector<double> electronIdIsoSFEtaHighUnc;
    
		bool ReReco_;
    bool verbose_;
    
};

#endif
