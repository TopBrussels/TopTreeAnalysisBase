#ifndef PlotSelectionObservables_h
#define PlotSelectionObservables_h
#include "Selection.h"
#include "TopTreeAnalysis/Tools/interface/Efficiency.h"
#include "PlotSelection.h"
#include "TopTreeAnalysis/Reconstruction/interface/Observables.h"

#include <iostream>
#include <iomanip>
#include "TFile.h"

using namespace std;

class PlotSelectionObservables {

  
 public:
  
  PlotSelectionObservables();
  PlotSelectionObservables(const Observables& obs);
  PlotSelectionObservables(const PlotSelectionObservables& psel);
  ~PlotSelectionObservables();

  void Fill(const Selection& sel, const Observables& obs);
  void Write(TFile* fout);

 private:

  vector<PlotSelection> PlotSel;
       
};

#endif
