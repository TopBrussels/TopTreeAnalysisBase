// -*- C++ -*-
//
// Package:    TopTreeAnalysisBase/Tools
// Class:      BTagWeightTools
// 
/**\class BTagWeightTools TopBrussels/TopTreeAnalysisBase/Tools/src/BTagWeightTools.cc TopBrussels/TopTreeAnalysisBase/Tools/interface/BTagWeightTools.h

 Description: Container class To calculate MC event weights from b-tagging SF as explained in https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
 		--> Initialized with a BTagCalibrationReader that represents a certain B-Tagger with WP, Meaurementtype, etc ... 
		(see TopBrussels/TopTreeAnalysisBase/Tools/interface/BTagCalibrationStandalone.h)

 Implementation:
  Efficiencies:  The user should run at least ones over all the selected jets in the analysis and put them in bins of Pt and Eta (2D histograms)
	--> Use the FillMCEfficiencyHistos member function. Output will be a root file containing these 2D histograms
	
  MC event weights: MC event weights can be calculated using the selected jets in an event, their SFs which are deduced from the BTagCalibrationReader and their tagging
  efficiencies deduced from the 2D histograms (output from FillMCEfficiencyHistos)
  	--> Use the getMCEventWeight memeber function. Output will be a MC event weight for each event

*/
//
// Original Author:  "fblekman"
// Altered by: "Seth Moortgat"
//
//
#ifndef BTagWeightTools_h
#define BTagWeightTools_h

//
// class declaration
//

#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeAnalysisBase/Tools/interface/BTagCalibrationStandalone.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <map>

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
  BTagWeightTools(string histoFileName="HistosPtEta.root", bool verbose = true);
  BTagWeightTools(const BTagCalibrationReader *reader, string histoFileName="HistosPtEta.root", bool verbose = true, float minpt = 30, float maxpt = 999, float maxeta = 2.4);
  ~BTagWeightTools();
  
  void InitializeMCEfficiencyHistos(int NofPtBins=15,float PtMin=30.,float PtMax=999.,int NofEtaBins=2);  //the binning is more or less arbitrary; but you should have enough statistics in each bin
  void FillMCEfficiencyHistos(vector< TopTree::TRootPFJet* > allSelectedJets);
  float getMCEventWeight(vector< TopTree::TRootPFJet* > jetsPerEvent, bool usePartonFlavour = false);
  
  float getTagEff(float pt, float eta, int flavor);
  
  
  protected:
  TFile* _f;
  
  private:
  float _ptmin;
  float _ptmax;
  float _etamax;
  const BTagCalibrationReader* _reader;
  map<string,TH2F*> _histo2D;
  bool _verbose;
  bool _histogramsFilled;
	
};

#endif
