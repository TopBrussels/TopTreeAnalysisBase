// -*- C++ -*-
//
// Package:    TopTreeAnalysisBase/Tools
// Class:      BTagWeightTools
// 
/**\class BTagWeightTools TopBrussels/TopTreeAnalysisBase/Tools/src/BTagWeightTools.cxx TopBrussels/TopTreeAnalysisBase/Tools/interface/BTagWeightTools.h

 Description: Container class to parse BTV POG payloads which can be updated from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG

Unfortunately the BTV POG does not give the parameterizations in a parsable format that is consistent even per measurements, so you need to edit the files yourself to the level that they are usable. THe parsing code currently is tuned for the Moriond 2012 parameterizations from ttbar+mujet file

Fake rates: Now also using the Fake Rate parameterizations that are documented (for Moriond 2013) here: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_Moriond2013.C or (for EPS 2013) here: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_EPS2013.C
As this is a bare Root Macro this has been c++ized into a separate set of functions.



 Implementation:
  Efficiencies:  Loads a text file. Accessors return weights as a function of jet eta,pt for multiple options (central value, syst up, syst down, etc.)
   Fake rates: Contains hard-coded functions that are supplied in example code by BTV group

*/
//
// Original Author:  "fblekman"
//         Created:  Fri Feb  1 12:10:50 CET 2013
// $Id: BTagWeightTools.h,v 1.1.2.2 2013/02/01 17:29:44 fblekman Exp $
//
//
#ifndef BTagWeightTools_h
#define BTagWeightTools_h

//
// class declaration
//

#include "TopTreeProducer/interface/TRootJet.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <TF1.h>
#include <TH2F.h>
#include <TFile.h>
#include <TString.h>

using namespace std;
using namespace TopTree;

class BTagWeightTools
{
 public:


  // constructors, destructors
  BTagWeightTools();
	BTagWeightTools(std::string filename,std::string defaultalgo,std::string sflightprescription); //sflightprescription default set to "EPS2013", but can be set to "MORIONDS2013"
  BTagWeightTools(std::string filename,std::string defaultalgo);
  BTagWeightTools(std::string filename);
	BTagWeightTools(std::string filename,std::string algo_L,std::string algo_T,std::string sflightprescription); //only in case you use two operating points, a loose and a tight one
  ~BTagWeightTools();
  
  
  // useful setters
  void setDefaultAlgo(std::string defaultalgo){_defaultalgo=defaultalgo;}
  
  float getSFlight(float pt, float eta, string algo, int syst);
  void setABCDRangeFakeRates(TString range){_abcdrange=range;} // set to "ABCD" for default. Consult BTV documentation for options (essentially "ABCD","AB","C","D")

  // useful getters
  float getSF(float pt, float eta, int flavor,int syst);
  float getSF(float pt, float eta, int flavor,string algo,int syst);
  //  float getSF(TRootJet *jet,int syst=0); // uses getSF(float pt, float eta,int flavor,int syst=0);
  //  float getSF(TRootJet *jet,string algo,int syst=0); // uses getSF(float pt, float eta,int flavor,int syst=0);

  float getUncertainty(float pt, float eta, int flavor, int syst);
  float getUncertainty(float pt, float eta, int flavor, string algo, int syst);
  //  float getUncertainty(float pt, float eta, int flavor, int syst); // uses getUncertainty (float pt, float eta, int flavor, int syst);
  //  float getUncertainty(float pt, float eta, int flavor, string algo, int syst);  // uses getUncertainty (float pt, float eta, int flavor, int syst);

  void InitializeMCEfficiencyHistos(int NofPtBins=15,float PtMin=30.,float PtMax=340.,int NofEtaBins=2);  //the binning is more or less arbitrary; but you should have enough statistics in each bin
  void FillMCEfficiencyHistos(vector< TRootJet* >& selectedJets);
	void FillMCEfficiencyHistos(vector< TLorentzVector >& selectedJets, vector<int>& selectedJets_partonFlavour, vector<float>& selectedJets_bTagValues);
	void WriteMCEfficiencyHistos(std::string outfilename);
	void ReadMCEfficiencyHistos(std::string infilename);
	void ReadMCEfficiencyHistos(std::string infilename_L,std::string infilename_T); //in case you use two operating points, a loose and a tight one
	float getMCEventWeight(vector< TRootJet* >& selectedJets, int syst);
  float getMCEventWeight(vector< TLorentzVector >& selectedJets, vector<int>& selectedJets_partonFlavour, vector<float>& selectedJets_bTagValues, int syst);
	float getMCEventWeight_LT(vector< TLorentzVector >& selectedJets, vector<int>& selectedJets_partonFlavour, vector<float>& selectedJets_bTagValues, int syst); //only in case you use two operating points, a loose and a tight one
  float getTagEff(float pt, float eta, int flavor);
	float getTagEff_T(float pt, float eta, int flavor); //only useful in case you use two operating points, same as getTagEff but with the histograms obtained with a 'tighter' WP (e.g. inBtaggedJets_T)

  // other getters
  std::string getCurrentFile() {return _filename;} // returns path to currently used file

  // various workers

  //  void clear(){;}
  //  void setNewTextFile(std::string filename){}

  //    void printContent(void){;}
  
  // private workers
 private:

  std::map<string,TFormula> _functions ;
 
  std::vector<float> _etarangesysts;
  std::vector<float> _etarangesystshi;
  std::vector<float> _ptrangesysts;
  std::vector<float> _ptrangesysthi;

  std::map<string,std::vector<float> > _weightsUp;
  std::map<string,std::vector<float> > _weightsDown;
  int findindex(string algo, float pt, float eta,int flavor);
  void parsefile();
 
  std::string _filename;
  float _ptmin;
  float _ptmax;
  float _etamax;
  std::string _defaultalgo;
	std::string _algo_L, _algo_T; //in case you use two operating points, a loose and a tight one
  std::string _sflightprescription;
	
  TString _abcdrange;

  TF1* fillfakerates(TString meanminmax, TString tagger, TString TaggerStrength, Float_t Etamin, Float_t Etamax, TString DataPeriod, TString Prescription);
  std::pair<float,float> getfakeraterange(TString tagger, float eta);
  TString makefakeratename(TString meanminmax, TString tagger, TString TaggerStrength, Float_t Etamin, Float_t Etamax, TString DataPeriod, TString Prescription);
  std::map<TString,TF1*> _fakeratesfunctions;
	
	void setalgoWPcut(std::string algo);
	void setalgoWPcut(std::string algo_L,std::string algo_T); //in case you use two operating points, a loose and a tight one
	map<string,TH2F*> _histo2D;
	bool MCEfficiencyHistosInitialized;
	float _PtMaxhistos;
	float _algoWPcut;
	float _algoWPcut_L;
	float _algoWPcut_T;
	TH2F *inBtaggedJets;
	TH2F *inBtaggedBJets;
	TH2F *inBtaggedCJets;
	TH2F *inBtaggedLightJets;
	TH2F *inTotalNofBJets;
	TH2F *inTotalNofCJets;
	TH2F *inTotalNofLightJets;
	
	TH2F *inBtaggedJets_T;
	TH2F *inBtaggedBJets_T;
	TH2F *inBtaggedCJets_T;
	TH2F *inBtaggedLightJets_T;
	TH2F *inTotalNofBJets_T;
	TH2F *inTotalNofCJets_T;
	TH2F *inTotalNofLightJets_T;
	
};

#endif
