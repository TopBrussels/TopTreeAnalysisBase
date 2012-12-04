#include "../interface/PlotSelectionObservables.h"


using namespace std;

  
  
PlotSelectionObservables::PlotSelectionObservables(){;}

PlotSelectionObservables::PlotSelectionObservables(const Observables& obs){
	for(unsigned int i= 0; i<obs.Variables().size();i++){
		char name[200];
		sprintf(name,"PlotSel_%s",obs.Variables()[i].first.c_str());
		PlotSelection a(obs.Variables()[i].first,name,name,200,obs.RangeVariables()[i].first,obs.RangeVariables()[i].second);
		PlotSel.push_back(a);
	}
}

PlotSelectionObservables::PlotSelectionObservables(const PlotSelectionObservables& psel):PlotSel(psel.PlotSel){;}
  
PlotSelectionObservables::~PlotSelectionObservables(){}

void PlotSelectionObservables::Fill(const Selection& sel, const Observables& obs){
   for(unsigned int i=0;i<PlotSel.size();i++){
   	if(i<obs.Variables().size())
		PlotSel[i].Fill(sel,obs.Variables()[i].second);
   	else cerr<<"Warning: size of PlotSel is not correct in PlotSelectionObservables object"<<endl;
   }
}

void PlotSelectionObservables::Write(TFile* fout){
   for(unsigned int i=0;i<PlotSel.size();i++){
   	PlotSel[i].Write(fout);
   }
}

