#ifndef PlotTriggerObservables_h
#define PlotTriggerObservables_h
#include "Selection.h"
#include "TopTreeAnalysis/Tools/interface/Efficiency.h"
#include "PlotTrigger.h"
#include "TopTreeAnalysis/Reconstruction/interface/Observables.h"

#include <iostream>
#include <iomanip>
#include "TFile.h"

using namespace std;

class PlotTriggerObservables {

  
 public:
  
  PlotTriggerObservables();
  PlotTriggerObservables(const Observables& obs);
  PlotTriggerObservables(const PlotTriggerObservables& ptrig);
  ~PlotTriggerObservables();

  void Fill(TRootRun& trig, const Observables& obs, int runID);
  void Write(TFile* fout);

 private:

  vector<PlotTrigger> PlotTrig;
       
};

#endif
