#ifndef AnalysisEnvironment_h
#define AnalysisEnvironment_h

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "TObject.h"

#include "TopTreeAnalysis/Reconstruction/interface/Observables.h"


using namespace std;

class AnalysisEnvironment: public TObject
{

public:

  AnalysisEnvironment ();
  ~AnalysisEnvironment ();
  AnalysisEnvironment (const AnalysisEnvironment & a);

  bool runOnObs(int iobs);/** return true if it should run on obs iobs*/
  bool runOnObs(string strobs);/** return true if it should run on obs strobs*/
  void ChecklistOfObs();/** check whether observables in listOfObs are defined in the Observables class*/

public:

  bool allObsDefined;
  int runOnObsByString;/** 0 means run on observables by integer, 1 means run on observables by string */
  vector<int> listOfObsInts;/** the list of observable integers, as specified in the xml file*/
  vector<string> listOfObsStrings;/** the list of observable string names, as specified in the xml file*/
  vector<int> listOfObs;/** the list of observable integers, corresponding to listOfObsInts or listOfObsStrings, according to the value of runOnObsByString*/
//Branch name
  bool loadGenJetCollection;
  bool loadGenEventCollection;
  bool loadNPGenEventCollection;
  bool loadMCParticles;
  bool loadTrackMETCollection;
  int JetType; //0: TRootJet - 1: CaloJet - 2: PFJet - 3: JPTJet
  int METType; //0: TRootMET - 1: CaloMET - 2: PFMET - 3: TCMET
  string PrimaryVertexCollection;
  string JetCollection;
  string METCollection;
  string TrackMETCollection;
  string MuonCollection;
  string ElectronCollection;
  string GenJetCollection;
  string GenEventCollection;
  string NPGenEventCollection;
  string MCParticlesCollection;

//Conditions
  bool isMC;
  bool isSMPseudo;
  bool Vars_ByFile;
  int MCRound;
  int Verbose;
  float Luminosity;//in pb-1
  float JES;//must be 1 nominaly: 0.9 means -10%, 1.1 means +10%
  bool doABCD;
  bool doVJEstim;
  bool doVJEstPE;
  bool doTtJEstim;
  bool doTemplComp;//comparison of template
  int nPseudoExp;
  string VarsFile;
  int IntToCut;

//For a cross-section measurement
  string MCExpFilename;
  float LuminosityError;
  float TriggerEff;
  float TriggerEffError;
  float SkimEff;
  float SkimEffError;
  float MuonSelEff;
  float MuonSelEffError;
  float SecondLeptonVetoEff;
  float SecondLeptonVetoEffError;
  float JetSelEff;
  float JetSelEffError;
  float NofSingleTopEvts;
  float NofSingleTopEvtsError;
  bool runonTTrees;
  int Systematics;

//For search
  bool doBkgEstim;
  bool doDumpPseudoExpInfoInTTree;/* Write a TTree with all the info (obs) per events per pseudo-exp - reuse by another program*/
  string DumpTreeName; /** Name of the file produced containg the above mentionned TTree*/
  vector<float> FractionHWEvts; /* Fraction of High Weight events used in the statistical procedure*/
 

//Cuts for Signal Region
  int   PVertexNdofCut;
  float PVertexZCut;
  float PVertexRhoCut;
  float ElectronPtCut;
  float ElectronEtaCut;
  float ElectronRelIsoCut;
  float MuonPtCutSR;
  float MuonEtaCutSR;
  float MuonRelIsoCutSR;
  int MuonNHitsCutSR;
  float MuonChi2CutSR;
  float MuonECALVetoSR;
  float MuonHCALVetoSR;
  float MuonD0CutSR;
  float MuonDRJetsCut;
  float MuonPtCutVetoSR;
  float MuonEtaCutVetoSR;
  float MuonRelIsoCutVetoSR;
  float JetsPtCutSR;
  float JetsEtaCutSR;
  bool applyJetID;
  float JetEMFCut;
  int n90HitsCut;
  float fHPDCut;
  int NofJets;
  int NofJetBins;

//Cut on CR for ttjetEstimation
  float MuonPtCutCR;
  float MuonEtaCutCR;
  float MuonRelIsoCutCR;
  float JetsPtCutCR;
  float JetsEtaCutCR;
//variables
  float MblCut;
  float DRBBCut;
  float HTBBCut;
//b-tagging
  int BtagAlgo_ttjEst;
  float BtagDiscriCut_ttjEst;
  //int btagAlgo;
  //float btagDiscriCut;
//normalisation
  float NREvtFraction;//[0,1]

//VJets estimation
  int BtagAlgo_vjEst;
  int NofBtagWorkingPoint_vjEst;
  float *BtagWorkingPoint_vjEst; //[NofBtagWorkingPoint_vjEst]
  //Minimization
  bool useMJLE;
  bool useUnBinMLE;
  string MinMethod;
  string MinOption;
  //Initialization
  vector<int> VJEstFixParam;
  vector< vector<float> > EffEbsel;
  vector<float> NVlikeInit;
  vector<float> NTTlikeInit;
  vector< vector< vector<float> > > TagEffInit;//dimensions: #jets, btag working point, vector of efficiencies:eb/eudsc/euds
  //Pseudo-Exp
  int NVJetPE;
  //Shape
  int NofIterationsVJestShapeEstim;


//QCD estimation
//histogram 2D
  int NXbinsABCD;
  int NYbinsABCD;
  float XbinMinABCD;
  float XbinMaxABCD;
  float YbinMinABCD;
  float YbinMaxABCD;
//cut on RelIso
  float cutXmin;
  float cutX0;
  float cutX1;
  float cutXmax;
//cut on d0Sign
  float cutYmin;
  float cutY0;
  float cutY1;
  float cutYmax;
  int region;
  
//Binning
  string binningFile;
  int nbins;//used only if binning for a given observable is not found
  int eventsperbin;
///NewPhysics
  float EpsilonValue;
  float Correl_cut;
  int nPseudoSession;

  ClassDef(AnalysisEnvironment,3) //was 2...
};

#endif
