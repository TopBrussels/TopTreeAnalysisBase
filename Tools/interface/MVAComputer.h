#ifndef MVAComputer_h
#define MVAComputer_h

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include "TStopwatch.h"
#include "TH1F.h"

#include "TopTreeAnalysis/TMVA/src/Reader.h"
#include "TopTreeAnalysis/TMVA/src/Tools.h"

using namespace std;

class MVAComputer {

 public: 

  MVAComputer(std::string Method, std::string TrainerOutputRootFile, std::string WeightsFile, std::vector<std::string> MVAvars, std::string postfix);
  ~MVAComputer();

  void FillVar(std::string var, Float_t val);

  //Float_t GetMVAValue();

  std::map<std::string,Float_t> GetMVAValues();

  std::string GetMethod() const { return Method_; };
  
  vector<string> GetAllMethods();

  void addMethod(std::string Method);

 private:

  std::string Method_;
	
  std::string postfix_;

  void bookInputVar(std::string var);

  std::map<std::string,int> Use; // container for default MVA methods to be trained and tested
  
  std::map<std::string,Float_t> vars; // support container for reading

  TMVA::Reader *reader;    

  std::string TrainerOutputRootFile_;
  std::string WeightsPrefix_;

  TFile* trainingOutput;

  TH1F *histLk, *histLkD, *histLkPCA, *histLkKDE, *histLkMIX, *histPD, *histPDD;
  TH1F *histPDPCA, *histPDEFoam, *histPDEFoamErr, *histPDEFoamSig, *histKNN, *histHm;
  TH1F *histFi, *histFiG,  *histFiB, *histLD, *histNn, *histNnC, *histNnT, *histBdt, *histBdtG, *histBdtD;
  TH1F *histRf, *histSVMG, *histSVMP, *histSVML, *histFDAMT, *histFDAGA, *histPBdt;
 
};

#endif
