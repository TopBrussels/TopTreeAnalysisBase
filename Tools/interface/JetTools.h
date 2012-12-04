#ifndef JetTools_h
#define JetTools_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

//TopTreeProducer classes
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootGenJet.h"
#include "TopTreeProducer/interface/TRootCaloJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootJPTJet.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootVertex.h"

#include "TopTreeAnalysis/Reconstruction/interface/FactorizedJetCorrector.h"
#include "TopTreeAnalysis/Reconstruction/interface/JetCorrectorParameters.h"
#include "TopTreeAnalysis/Reconstruction/interface/JetCorrectionUncertainty.h"

using namespace std;
using namespace TopTree;

class JetTools
{
  public:
    JetTools(vector<JetCorrectorParameters> vCorrParam, JetCorrectionUncertainty* jecUnc, bool startFromRaw = true);
    ~JetTools();
    
    // Jet uncorrectors (back from L2L3 corrected jets to raw jets), input is assumed to be L1L2L3 corrected!!!
    void unCorrectJet(TRootJet* inJet, bool isData = false);
    void unCorrectJets(vector<TRootJet*> inJets, bool isData = false);
    
    // Jet correctors
    void correctJet(TRootJet* inJet, int nPV = -1, bool isData = false);
    void correctJets(vector<TRootJet*> inJets, bool isData = false);
    void correctJets(vector<TRootJet*> inJets, vector<TRootVertex*> PVs, bool isData = false);
    void correctJet(TRootJet* inJet, float rhoPU, bool isData = false);
    void correctJets(vector<TRootJet*> inJets, float rhoPU, bool isData = false);

    
    // Jet correctors for JES systematic
    void correctJetJESUnc(TRootJet* inJet, string direction, float nSigma = 1); // direction = plus or minus
    void correctJetJESUnc(vector<TRootJet*> inJets, string direction, float nSigma = 1);
    // Jet in combination with MET correctors for JES systematic
    void correctJetJESUnc(TRootJet* inJet, TRootMET* inMET, string direction, float nSigma = 1); // direction = plus or minus
    void correctJetJESUnc(vector<TRootJet*> inJets, TRootMET*, string direction, float nSigma = 1);
    
    // Jet correctors for JER smearing and JER systematics
    void correctJetJER(TRootJet* inJet, TRootGenJet* inGenJet, string direction, bool oldnumbers = true);
    void correctJetJER(vector<TRootJet*> inJets, vector<TRootGenJet*> inGenJets, string direction, bool oldnumbers = true);
    // Jet in combination with MET correctors for JER smearing and systematic
    void correctJetJER(vector<TRootJet*> inJets, vector<TRootGenJet*> inGenJets, TRootMET* inMET, string direction, bool oldnumbers = true);
    
    // Jet scalers (with a fixed factor)
    void scaleJet(TRootJet* inJet, float scale=1);
    void scaleJets(vector<TRootJet*> inJets, float scale=1);    
    
    // Type I MET corrections
    void correctMETTypeOne(TRootJet* inJet, TRootMET* inMET, bool isData = false);
    void correctMETTypeOne(vector<TRootJet*> inJets, TRootMET* inMET, bool isData = false);
        
    // Jet convertors
    vector<TRootCaloJet*> convertToCaloJets(vector<TRootJet*> init_jets);
    vector<TRootPFJet*> convertToPFJets(vector<TRootJet*> init_jets);
    vector<TRootJPTJet*> convertToJPTJets(vector<TRootJet*> init_jets);
    TRootCaloJet* convertToCaloJets(TRootJet* init_jet);
    TRootPFJet* convertToPFJets(TRootJet* init_jet);
    TRootJPTJet* convertToJPTJets(TRootJet* init_jets);
    
  private:
    FactorizedJetCorrector *JEC_;
    JetCorrectionUncertainty* jecUnc_;
    bool startFromRaw_; // true: first correct to raw en then apply the necessary corrections
};

#endif
