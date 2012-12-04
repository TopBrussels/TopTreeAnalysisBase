#include "../interface/PlotTriggerObservables.h"


using namespace std;

  
  
PlotTriggerObservables::PlotTriggerObservables(){;}

PlotTriggerObservables::PlotTriggerObservables(const Observables& obs){
        for(unsigned int i=0; i<obs.Variables().size();i++){
		char name[200];
		sprintf(name,"PlotTrig_%s",obs.Variables()[i].first.c_str());
		PlotTrigger a(obs.Variables()[i].first,name,name,200,obs.RangeVariables()[i].first,obs.RangeVariables()[i].second);
		PlotTrig.push_back(a);
	}
										
}

PlotTriggerObservables::PlotTriggerObservables(const PlotTriggerObservables& psel):PlotTrig(psel.PlotTrig){;}
  
PlotTriggerObservables::~PlotTriggerObservables(){}

void PlotTriggerObservables::Fill(TRootRun& trig, const Observables& obs, int runID){
   	for(unsigned int i=0;i<PlotTrig.size();i++){
        	   if(i<obs.Variables().size())
	                  PlotTrig[i].Fill(trig,obs.Variables()[i].second, runID);
		   else cerr<<"Warning: size of PlotTrig is not correct in PlotTrigectionObservables object"<<endl;

	}
}

void PlotTriggerObservables::Write(TFile* fout){
   for(unsigned int i=0;i<PlotTrig.size();i++){
           PlotTrig[i].Write(fout);
   }
}

