#ifndef JetCombiner_h
#define JetCombiner_h

// Class that takes care of the Jet-parton matching and of the MVA-based jet combination

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstdio>

#include "TLorentzVector.h"
#include "TH1F.h"

#include <sys/stat.h>

#include "TopTreeAnalysisBase/Tools/interface/MVAComputer.h"
#include "TopTreeAnalysisBase/Tools/interface/MVATrainer.h"
#include "TopTreeAnalysisBase/Tools/interface/PlottingTools.h"
#include "TopTreeAnalysisBase/MCInformation/interface/ResolutionFit.h"
#include "TopTreeAnalysisBase/Tools/interface/MultiSamplePlot.h"
#include "TopTreeAnalysisBase/MCInformation/interface/JetPartonMatching.h"

#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootEvent.h"
// #include "TopTreeProducer/interface/TRootGenEvent.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"

using namespace std;
using namespace TopTree;

struct MVAValues {
    string MVAAlgo;
    float MVAValue;
    int WJet1;
    int WJet2;
    int HadrBJet;
    int LeptBJet;
} ;

class JetCombiner {

public:
    JetCombiner(bool trainMVA, float Luminosity, const vector<Dataset*>& datasets, string MVAMethod = "Likelihood", bool Tprime = false, string MVAfilePostfix = "", string postfix = "");
    ~JetCombiner();
    void ProcessEvent(Dataset* dataSet, const vector<TRootMCParticle*> mcParticles, const vector<TRootPFJet*> selectedJets, const TLorentzVector* selectedMuon, vector<TRootElectron*> vectEl, vector<TRootMuon*> vectMu, float scaleFactor=1, bool TprimeEvaluation=false);
    void ProcessEvent_SingleHadTop(Dataset* dataSet, const vector<TRootMCParticle*> mcParticles, const vector<TRootPFJet*> selectedJets, const TLorentzVector* selectedMuon, float scaleFactor=1);

    void ProcessEvent(Dataset* dataSet, const vector<TLorentzVector> mcParticlesForMatching, const vector<TLorentzVector> selectedJets, const vector<float> bTagValues, const TLorentzVector selectedLepton, bool isSemiLep, float scaleFactor, bool TprimeEvaluation);
    void FillResolutions(ResolutionFit* resFitLightJets, ResolutionFit* resFitBJets, ResolutionFit* resFitBJets_B=0, ResolutionFit* resFitBJets_Bbar=0);
    pair<float, vector<unsigned int> > getMVAValue(string MVAMethod, int rank); // rank 1 means the highest MVA value for this method, rank 2 the second highest, ...
    vector<unsigned int> GetGoodJetCombination(); // Good according to MC!
    bool isGoodJetCombination(string MVAMethod, int rank, bool fullcombination); // check if a certain MVA jet combination is	correct (correct for the 3 hadronic top jets OR correct for the full jet combination)
    bool All4JetsMatched_MCdef() { return all4JetsMatched_MCdef_; }; // true if the 4 highest pT jets are matched to the TTbar partons according to the MC
    bool HadronicTopJetsMatched_MCdef() { return hadronictopJetsMatched_MCdef_; }; // true if the 3 hadronic top jets are matched to		the	partons of the hadronic top according to the MC
    float GetRelEDiffJetPartonB() { return relDiffEJetParton_b_; };
    float GetRelEDiffJetPartonL1() { return relDiffEJetParton_l1_; };
    float GetRelEDiffJetPartonL2() { return relDiffEJetParton_l2_; };
    TLorentzVector GetHadrBQuark();
    TLorentzVector GetLightQuark1();
    TLorentzVector GetLightQuark2();
    TLorentzVector GetLeptBQuark();
    void EndJob(); // Do all the stuff which needs to be done after running over all the events has finished (except the plotting stuff)
    void Write(TFile* fout, bool savePNG = false, string pathPNG = string(""), bool plotMVAstuff = true);

private:
    map<string, MultiSamplePlot*> MSPlot_;
    map<string, TH1F*> histo1D_;
    map<string, TH2F*> histo2D_;
    map<string,TGraphAsymmErrors*> graphAsymmErr_;

    vector<Dataset*> datasets_;
    float Luminosity_;

    bool all4JetsMatched_MCdef_, hadronictopJetsMatched_MCdef_;
    bool all4JetsMatched_MVAdef_, hadronictopJetsMatched_MVAdef_;

    bool Tprime_; // true if tprime analysis, false if JES/mtop analysis
    string MVAfilePostfix_;
    string postfix_;

    float relDiffEJetParton_b_;
    float relDiffEJetParton_l1_;
    float relDiffEJetParton_l2_;

    bool trainMVA_; // true if the MVA needs to be trained, false if the trained MVA will be used to compute stuff
    MVATrainer* trainer_;
    MVAComputer* computer_;

    vector<MVAValues> vectorMVA_;

    pair<unsigned int, unsigned int> leptonicBJet_, hadronicBJet_, hadronicWJet1_, hadronicWJet2_; //First index is the JET number, second one is the parton
    vector<TRootMCParticle*> mcParticlesMatching_; // MCParticles used for the matching, need to be stored to be used after the ProcessEvent fuction was called
    vector<TRootPFJet*> selectedJets_; // need to be stored to be used after the ProcessEvent function was called

    bool EndJobWasRun_;

};

#endif
