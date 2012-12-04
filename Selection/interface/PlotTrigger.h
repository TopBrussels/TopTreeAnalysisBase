#ifndef PlotTrigger_h
#define PlotTrigger_h
#include "Selection.h"
#include "TopTreeAnalysis/Tools/interface/Efficiency.h"
#include "TopTreeProducer/interface/TRootRun.h"

#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include "TFile.h"

using namespace std;
using namespace TopTree;


class PlotTrigger {

  
 public:
  
  PlotTrigger();
  PlotTrigger(string labelVariable_, char* name, char* title, int nbins, float min, float max, int lineWidth = 2, int lineColor = 1);
  PlotTrigger(const PlotTrigger& psel);
  ~PlotTrigger();

  void ConfigHisto(char* name, char* title, int nbins, float min, float max, int lineWidth = 2, int lineColor = 1);
  string GetLabelVariable() const{ return labelVariable_;};
  void SetLabel();
  void SetLabelVariable(string label);
  bool TriggerAccepted(TRootRun& trig, string trigSearched, int runID);

  void Fill(TRootRun& trig, float var, int runID);
  void Write(TFile* fout);

 private:

  Efficiency eff_HLT_QuadJet60;
  Efficiency eff_HLT_SumET120;
  Efficiency eff_HLT_MET75;
  Efficiency eff_HLT_MET35_HT350;
  Efficiency eff_HLT_TripleJet60_MET60;
  Efficiency eff_HLT_QuadJet35_MET60;
  Efficiency eff_HLT_IsoMu15;
  Efficiency eff_HLT_IsoMu7_Jet40;
  Efficiency eff_HLT_Mu5_TripleJet30;
 
  string labelVariable_;
       
};

#endif
