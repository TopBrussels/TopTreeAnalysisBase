#ifndef PlotSelection_h
#define PlotSelection_h
#include "Selection.h"
#include "TopTreeAnalysis/Tools/interface/Efficiency.h"

#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include "TFile.h"

using namespace std;

class PlotSelection {

  
 public:
  
  PlotSelection();
  PlotSelection(string labelVariable_, char* name, char* title, int nbins, float min, float max, int lineWidth = 2, int lineColor = 1);
  PlotSelection(const PlotSelection& psel);
  ~PlotSelection();

  void ConfigHisto(char* name, char* title, int nbins, float min, float max, int lineWidth = 2, int lineColor = 1);
  string GetLabelVariable() const{ return labelVariable_;};
  void SetLabel();
  void SetLabelVariable(string label);
	  

  void Fill(const Selection& sel, float var);
  void Write(TFile* fout);

 private:

  Efficiency eff1Muon;
  Efficiency eff1MuonIso;
  Efficiency eff4Jets;
  Efficiency effAll;
 
  string label_;
  string labelVariable_;
       
};

#endif
