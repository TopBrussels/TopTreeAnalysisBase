#include "../interface/AnalysisEnvironment.h"

ClassImp(AnalysisEnvironment)


AnalysisEnvironment::AnalysisEnvironment ()
{
  loadGenJetCollection = false;
  loadGenEventCollection = false;
  loadNPGenEventCollection = false;
  loadMCParticles = false;
  loadTrackMETCollection = false;
  JetType = 0;
  METType = 0;
  isMC = true;
  isSMPseudo=true;
  MCRound=0;
  Vars_ByFile=false;
  VarsFile= string("");
  Verbose = 0;
  Luminosity = 1;
  JES = 1;
  doABCD = true;
  doVJEstim = true;
  doVJEstPE = true;
  doTtJEstim = true;
  doTemplComp = false;
  nPseudoExp = 0;
  MCExpFilename = string("");
  LuminosityError = 0;
  TriggerEff = 1;
  TriggerEffError = 0;
  SkimEff = 1;
  SkimEffError = 0;
  MuonSelEff = 1;
  MuonSelEffError = 0;
  SecondLeptonVetoEff = 1;
  SecondLeptonVetoEffError = 0;
  JetSelEff = 0;
  JetSelEffError = 0;
  NofSingleTopEvts = 0;
  NofSingleTopEvtsError = 0;
  doBkgEstim = true;
  doDumpPseudoExpInfoInTTree = false;
  PVertexNdofCut = -999;
  PVertexZCut = -999;
  PVertexRhoCut = -999;
  ElectronPtCut = -999;
  ElectronEtaCut = -999;
  ElectronRelIsoCut = -999;
  MuonPtCutSR = -999;
  MuonEtaCutSR = -999;
  MuonRelIsoCutSR = -999;
  MuonPtCutVetoSR = -999;
  MuonEtaCutVetoSR = -999;
  MuonRelIsoCutVetoSR = -999;
  MuonNHitsCutSR = -999;
  MuonChi2CutSR = -999;
  MuonECALVetoSR = -999;
  MuonHCALVetoSR = -999;
  MuonD0CutSR = -999;
  MuonDRJetsCut = -999;
  JetsPtCutSR = -999;
  JetsEtaCutSR = -999;
  applyJetID = false;
  JetEMFCut = -999;
  n90HitsCut = -999;
  fHPDCut = -999;
  NofJets = -999;
  NofJetBins = -999;
  MuonPtCutCR = -999;
  MuonEtaCutCR = -999;
  MuonRelIsoCutCR = -999;
  MblCut = -999;
  DRBBCut = -999;
  HTBBCut = -999;
  BtagAlgo_ttjEst = -999;
  BtagDiscriCut_ttjEst = -999;
  //btagAlgo = -999;
  //btagDiscriCut = -999;
  NREvtFraction = 1;
  BtagAlgo_vjEst = -999;
  NofBtagWorkingPoint_vjEst = 1;
  BtagWorkingPoint_vjEst = new float[NofBtagWorkingPoint_vjEst];
  for (int i=0; i<NofBtagWorkingPoint_vjEst; i++) BtagWorkingPoint_vjEst[i]=0.;
  //NofIterationsVJestEstim = -999;
  useMJLE = false;
  useUnBinMLE = true;
  MinMethod = string("Minuit2");
  MinOption = string("Combined");
  NVJetPE = 1;
  NofIterationsVJestShapeEstim = -999;
  NXbinsABCD = -999;
  NYbinsABCD = -999;
  XbinMinABCD = -999;
  XbinMaxABCD = -999;
  YbinMinABCD = -999;
  YbinMaxABCD = -999;
  cutXmin = -999;
  cutX0 = -999;
  cutX1 = -999;
  cutXmax = -999;
  cutYmin = -999;
  cutY0 = -999;
  cutY1 = -999;
  cutYmax = -999;
  region = -999;
  runOnObsByString = 0;
  allObsDefined = false;
  binningFile = string("");
  //nbins = 20;
  nbins = 0;
  eventsperbin=0;
  EpsilonValue=0;
  Correl_cut=0;
  IntToCut=0;
  runonTTrees=0;
  Systematics=0;
}

AnalysisEnvironment::~AnalysisEnvironment ()
{
  delete BtagWorkingPoint_vjEst;
}

AnalysisEnvironment::AnalysisEnvironment (const AnalysisEnvironment & a)
{
//branch name 
  loadGenJetCollection = a.loadGenJetCollection;
  loadGenEventCollection = a.loadGenEventCollection;
  loadNPGenEventCollection = a.loadNPGenEventCollection;
  loadMCParticles = a.loadMCParticles;
  loadTrackMETCollection = a.loadTrackMETCollection;
  JetType = a.JetType;
  METType = a.METType;
  PrimaryVertexCollection = a.PrimaryVertexCollection;
  JetCollection = a.JetCollection;
  METCollection = a.METCollection;
  MuonCollection = a.MuonCollection;
  ElectronCollection = a.ElectronCollection;
  GenJetCollection = a.GenJetCollection;
  TrackMETCollection = a.TrackMETCollection;
  GenEventCollection = a.GenEventCollection;
  NPGenEventCollection = a.NPGenEventCollection;
  MCParticlesCollection = a.MCParticlesCollection;
//Conditions
  isMC = a.isMC;
  MCRound=a.MCRound;
  isSMPseudo=a.isSMPseudo;
  Vars_ByFile=a.Vars_ByFile;
  Verbose = a.Verbose;
  Luminosity = a.Luminosity;
  JES = a.JES;
  doABCD = a.doABCD;
  doVJEstim = a.doVJEstim;
  doVJEstPE = a.doVJEstPE;
  doTtJEstim = a.doTtJEstim;
  nPseudoExp = a.nPseudoExp;
  VarsFile=a.VarsFile;
  nPseudoSession=a.nPseudoSession;
  runonTTrees=a.runonTTrees;
  Systematics=a.Systematics;
//for cross-section
  MCExpFilename = a.MCExpFilename;
  LuminosityError = a.LuminosityError;
  TriggerEff = a.TriggerEff;
  TriggerEffError = a.TriggerEffError;
  SkimEff = a.SkimEff;
  SkimEffError = a.SkimEffError;
  MuonSelEff = a.MuonSelEff;
  MuonSelEffError = a.MuonSelEffError;
  SecondLeptonVetoEff = a.SecondLeptonVetoEff;
  SecondLeptonVetoEffError = a.SecondLeptonVetoEffError;
  JetSelEff = a.JetSelEff;
  JetSelEffError = a.JetSelEffError;
  NofSingleTopEvts = a.NofSingleTopEvts; 
  NofSingleTopEvtsError = a.NofSingleTopEvtsError; 
  doTemplComp = a.doTemplComp;
//Search
  doBkgEstim = a.doBkgEstim;
  doDumpPseudoExpInfoInTTree = a.doDumpPseudoExpInfoInTTree;
  DumpTreeName = a.DumpTreeName;
  FractionHWEvts = a.FractionHWEvts;
//Cuts for Signal Region
  PVertexNdofCut = a.PVertexNdofCut;
  PVertexZCut = a.PVertexZCut;
  PVertexRhoCut = a.PVertexRhoCut;
  ElectronPtCut = a.ElectronPtCut;
  ElectronEtaCut = a.ElectronEtaCut;
  ElectronRelIsoCut = a.ElectronRelIsoCut;
  MuonPtCutSR = a.MuonPtCutSR;
  MuonEtaCutSR = a.MuonEtaCutSR;
  MuonRelIsoCutSR = a.MuonRelIsoCutSR;
  MuonPtCutVetoSR = a.MuonPtCutVetoSR;
  MuonEtaCutVetoSR = a.MuonEtaCutVetoSR;
  MuonRelIsoCutVetoSR = a.MuonRelIsoCutVetoSR;
  MuonNHitsCutSR = a.MuonNHitsCutSR;
  MuonChi2CutSR = a.MuonChi2CutSR;
  MuonECALVetoSR = a.MuonECALVetoSR;
  MuonHCALVetoSR = a.MuonHCALVetoSR;
  MuonD0CutSR = a.MuonD0CutSR;
  MuonDRJetsCut = a.MuonDRJetsCut;
  JetsPtCutSR = a.JetsPtCutSR;
  JetsEtaCutSR = a.JetsEtaCutSR;
  applyJetID = a.applyJetID;
  JetEMFCut = a.JetEMFCut;
  n90HitsCut = a.n90HitsCut;
  fHPDCut = a.fHPDCut;
  NofJets = a.NofJets;
  NofJetBins = a.NofJetBins;
//Cut on CR for ttjetEstimation
  MuonPtCutCR = a.MuonPtCutCR;
  MuonEtaCutCR = a.MuonEtaCutCR;
  MuonRelIsoCutCR = a.MuonRelIsoCutCR;
//variables
  MblCut = a.MblCut;
  DRBBCut = a.DRBBCut;
  HTBBCut = a.HTBBCut;
//b-tag
  BtagAlgo_ttjEst = a.BtagAlgo_ttjEst;
  BtagDiscriCut_ttjEst = a.BtagDiscriCut_ttjEst;
  //btagAlgo = a.btagAlgo;
  //btagDiscriCut = a.btagDiscriCut;
//norm
  NREvtFraction = a.NREvtFraction;
//VJets estimation
  BtagAlgo_vjEst               = a.BtagAlgo_vjEst;
  NofBtagWorkingPoint_vjEst    = a.NofBtagWorkingPoint_vjEst;
  BtagWorkingPoint_vjEst = new float[NofBtagWorkingPoint_vjEst];
  for (int i=0; i<NofBtagWorkingPoint_vjEst; i++)
    BtagWorkingPoint_vjEst[i] = a.BtagWorkingPoint_vjEst[i];  
  useMJLE = a.useMJLE;
  useUnBinMLE = a.useUnBinMLE;
  MinMethod = a.MinMethod;
  MinOption = a.MinOption;
  VJEstFixParam = a.VJEstFixParam;
  EffEbsel = a.EffEbsel;
  NVlikeInit = a.NVlikeInit;
  NTTlikeInit = a.NTTlikeInit;
  TagEffInit = a.TagEffInit;
  NVJetPE = a.NVJetPE;
  //NofIterationsVJestEstim = a.NofIterationsVJestEstim;
  NofIterationsVJestShapeEstim = a.NofIterationsVJestShapeEstim;
//QCD estimation
//histogram 2D
  NXbinsABCD = a.NXbinsABCD;
  NYbinsABCD = a.NYbinsABCD;
  XbinMinABCD = a.XbinMinABCD;
  XbinMaxABCD = a.XbinMaxABCD;
  YbinMinABCD = a.YbinMinABCD;
  YbinMaxABCD = a.YbinMaxABCD;
//cut on RelIso
  cutXmin = a.cutXmin;
  cutX0 = a.cutX0;
  cutX1 = a.cutX1;
  cutXmax = a.cutXmax;
//cut on d0Sign
  cutYmin = a.cutYmin;
  cutY0 = a.cutY0;
  cutY1 = a.cutY1;
  cutYmax = a.cutYmax;
  region = a.region;
//obs
  allObsDefined = a.allObsDefined;
  runOnObsByString = a.runOnObsByString;
  listOfObsInts = a.listOfObsInts;
  listOfObsStrings = a.listOfObsStrings;
  listOfObs = a.listOfObs;
//Binning
  binningFile = a.binningFile;
  nbins = a.nbins;
  EpsilonValue=a.EpsilonValue;
  Correl_cut=a.Correl_cut;
}

bool AnalysisEnvironment::runOnObs(int iobs){
   for (int i = 0; i < (int)listOfObs.size(); i++)
   	if (listOfObs[i] == iobs || (listOfObs.size()==1 && listOfObs[0] == -1)) return true;
   return false;
}

bool AnalysisEnvironment::runOnObs(string strobs){
   for (int i = 0; i < (int)listOfObsStrings.size(); i++)
   	if (listOfObsStrings[i] == strobs || (listOfObsStrings.size()==1 && listOfObsStrings[0] == "-1")) return true;
   return false;
}

void AnalysisEnvironment::ChecklistOfObs(){
   int verbose = 1;
   Observables obs; 
   vector<string> nonDefinedObs;
   bool defined = false;   
   if(!runOnObsByString){
      if(listOfObsInts.size()==1 && listOfObsInts[0]==-1){ listOfObs.push_back(-1);}
      else{
         for (unsigned int i=0; i<listOfObsInts.size(); i++){
            defined = false;
            for (unsigned int j=0; j<obs.ListOfVariables().size(); j++){
               if(listOfObsInts[i] == (int)j){
	          defined = true;
		  listOfObs.push_back(j);
		  break;
	       }
	    }
	    if(!defined){
	       stringstream sstr;
	       sstr << listOfObsInts[i];
	       nonDefinedObs.push_back(sstr.str());
	    }
         }
      }
   }
   else{
      if(listOfObsStrings.size()==1 && listOfObsStrings[0]=="-1"){ listOfObs.push_back(-1);}
      else{
         for (unsigned int i=0; i<listOfObsStrings.size(); i++){
            defined = false;
            for (unsigned int j=0; j<obs.ListOfVariables().size(); j++){
               if(listOfObsStrings[i] == obs.ListOfVariables()[j]){ 
	          defined = true;
		  listOfObs.push_back(j);
		  break;
	       }
	    }
	    if(!defined) nonDefinedObs.push_back(listOfObsStrings[i]);
         }
      }
   }
   
   if(nonDefinedObs.size()==0){
      allObsDefined = true;
      if(!runOnObsByString){
      	 cout << "The following observables are specified [by integers] in the xml config file:" << endl;
         if(listOfObsInts.size()==1 && listOfObsInts[0]==-1){ cout << "   -1: All observables (" << obs.ListOfVariables().size () << " in total)" << endl;}
         else{
	    cout << " (" << listOfObsInts.size() << " observables in total)" << endl;
	    for (unsigned int i=0; i<listOfObsInts.size(); i++) cout << "   " << listOfObsInts[i] << ": " << obs.ListOfVariables()[listOfObsInts[i]]  << endl;
	 }
      }          
      else{
         cout << "The following observables are specified [by string names] in the xml config file:" << endl;
         if(listOfObsStrings.size()==1 && listOfObsStrings[0]=="-1"){ cout << "   -1: All observables (" << obs.ListOfVariables().size () << " in total)" << endl;}
         else{
	    cout << " (" << listOfObsStrings.size() << " observables in total)" << endl;
	    for (unsigned int i=0; i <listOfObsStrings.size(); i++) cout << "   " << listOfObs[i] << ": " << listOfObsStrings[i] << endl;
	 } 
      }
   }
   else{
      allObsDefined = false;
      if(!runOnObsByString){
         cerr << "Error: the following observables (specified in the xml config file [by integers]) are not defined in Observables.cc:" << endl;
         for(unsigned int i=0; i<nonDefinedObs.size(); i++){ cerr << "   " << nonDefinedObs[i] << endl;}
         cerr << "Note: observable integers needs to be in [0," << obs.ListOfVariables().size () << "], or -1 (to run on all observables)" << endl;
	 if(verbose>0){
	    cout << "These are the defined observables in Observables.cc:" << endl;
	    for (unsigned int i=0; i<obs.ListOfVariables().size (); i++) cout << " " << i << ": " << obs.ListOfVariables()[i] << endl;
	 }
      }
      else{
         cerr << "Error: the following observables (specified in the xml config file [by string names]) are not defined in Observables.cc:" << endl;
         for(unsigned int i=0; i<nonDefinedObs.size(); i++){ cerr << "   " << nonDefinedObs[i] << endl;}
         cerr << "Note: put -1 (only) to run on all observables" << endl;
	 if(verbose>0){
	    cout << "These are the defined observables in Observables.cc:" << endl;
	    for (unsigned int i=0; i<obs.ListOfVariables().size (); i++) cout << " " << i << ": " << obs.ListOfVariables()[i] << endl;
	 }
      }
   }	
}
