#ifndef MVATrainer_h
#define MVATrainer_h

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"



//Changing to these include paths for compatibility with ROOT v5.34
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/Config.h"


class MVATrainer {


 public:

  MVATrainer(std::string Method,std::string OutputMVAWeights, std::string OutputRootFile);
  ~MVATrainer();

  void bookInputVar(std::string var);

  void bookWeight(std::string weight);

  void Fill(std::string eventType, std::string var, double value);

  void FillWeight(std::string eventType, std::string weight, double value);

  void addMethod(std::string Method);

  void TrainMVA(std::string SplitMode, std::string CutsS, int nTrainS, int nTestS, std::string CutsB, int nTrainB, int nTestB, std::string postfix, bool nlo);

 private:

  // output files

  std::string Method_;

  std::string OutputMVAWeights_;
  std::string OutputRootFile_;

  std::map<std::string,int> Use; //container for default MVA methods to be trained and tested

  std::map<std::string,double> vars; // support container for tree building
   std::map<std::string,double> weights; // support container for tree building

  TTree *TreeS, *TreeB;   // TTrees for MVATrainer

  TFile *outFile; // output rootfile

  TMVA::Factory *factory; // TMVA workspace

};


#endif
