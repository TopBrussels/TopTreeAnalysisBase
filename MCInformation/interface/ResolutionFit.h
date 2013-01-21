#ifndef ResolutionFit_h
#define ResolutionFit_h

#include <iostream>
#include <iomanip>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TFile.h"
#include "TLatex.h"

#include "TopTreeAnalysisBase/Tools/interface/PlottingTools.h"

#include "TopTreeProducer/interface/TRootPFJet.h"

using namespace std;
using namespace TopTree;

// Calculation of the resolutions as done by Holger Enderle (slightly modified)
// see: http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Bromo/TopAnalysis/TopUtils/bin/?pathrev=MAIN

class ResolutionFit {

  public:
    ResolutionFit();
    ResolutionFit(string label);
    ResolutionFit(const ResolutionFit &r);
    ~ResolutionFit();
    void FillPFJets(TRootPFJet *jet, TLorentzVector *mcParticle, float weight=1.);
    void Fill(TLorentzVector *lorentzVector, TLorentzVector *mcParticle, float weight=1.);
    void CalculateResolutions();
    void WritePlots(TFile *fout, bool savePNG = false, string pathPNG = string(""));
    void WriteResolutions(string file);
    void LoadResolutions(string file);
    
    float EtResolution(TLorentzVector* jet, bool protect = false);
    float EtaResolution(TLorentzVector* jet, bool protect = false);
    float ThetaResolution(TLorentzVector* jet, bool protect = false);
    float PhiResolution(TLorentzVector* jet, bool protect = false);
    float EtCorrection(TLorentzVector* jet);
    
    vector<double> ExtractSigmaMean(TH1* theHisto);
    
  private:
    string label_;
    
    bool calculatedResolutions_;
    bool loadedResolutions_;
    bool StijnFlavorJES_;
    
    Float_t towerBinning_[13]; // HCAL tower bins (for jets)
    Float_t jetPtBinning_[16];
    
    UInt_t nEtaBins_;
    UInt_t nPtBins_;
    
    TF1* bFuncSigma_[13];
    TF1* bFuncEtaSigma_[13];
    TF1* bFuncThetaSigma_[13];
    TF1* bFuncPhiSigma_[13];
    TF1* bFuncRelSigma_[13];
    
    TF1* bFuncMean_[13];
    TF1* bFuncRelMean_[13];
    
    TH1F* chfHisto_[13];
    TH1F* nhfHisto_[13];
    TH1F* cefHisto_[13];
    TH1F* nefHisto_[13];
    TH1F* cmfHisto_[13];
    TH1F* ptHisto_[13];
    
    TH1F* binCenterHisto_[168]; // size = (nEtaBins_*nPtBins_)+nPtBins_
    TH1F* resHisto_[168];
    TH1F* resHistoEta_[168];
    TH1F* resHistoTheta_[168];
    TH1F* resHistoPhi_[168];
    TH1F* resRelHisto_[168];
    TH1F* binCenterHistoIncl_[14];
    TH1F* resRelHistoIncl_[14];
        
    TCanvas* controlCan_[13];
    TCanvas* controlCanEta_[13];
    TCanvas* controlCanTheta_[13];
    TCanvas* controlCanPhi_[13];
    TCanvas* controlCanRel_[13];
    TCanvas* controlCanRelIncl_;

    TCanvas* bPtEtSigma_[13];
    TCanvas* bPtEtaSigma_[13];
    TCanvas* bPtThetaSigma_[13];
    TCanvas* bPtPhiSigma_[13];
    TCanvas* bPtEtRelSigma_[13];
    
    TCanvas* bPtEtMean_[13];
    TCanvas* bPtEtaMean_[13];
    TCanvas* bPtThetaMean_[13];
    TCanvas* bPtPhiMean_[13];
    TCanvas* bPtEtRelMean_[13];
    TCanvas* bPtEtRelMeanIncl_;
    
    TGraphErrors* bGraphSigma_[13];
    TGraphErrors* bGraphEtaSigma_[13];
    TGraphErrors* bGraphThetaSigma_[13];
    TGraphErrors* bGraphPhiSigma_[13];
    TGraphErrors* bGraphRelSigma_[13];
    
    TGraphErrors* bGraphMean_[13];
    TGraphErrors* bGraphEtaMean_[13];
    TGraphErrors* bGraphThetaMean_[13];
    TGraphErrors* bGraphPhiMean_[13];
    TGraphErrors* bGraphRelMean_[13];
    TGraphErrors* bGraphRelMeanIncl_;
};

#endif
