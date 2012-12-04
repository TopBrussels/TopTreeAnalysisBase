#ifndef MCObsExpectation_h
#define MCObsExpectation_h

#include <string>
#include "TH1.h"
#include "THStack.h"
#include "TLegend.h"
#include "Dataset.h"
#include "TObjArray.h"

class MCObsExpectation: public TObject{

 public:
	MCObsExpectation();
	//MCObsExpectation(TAxis* axis, string VarLabel, float Luminosity);
	MCObsExpectation(vector<Dataset*> datasets, TAxis* axis, string VarLabel, float Luminosity);
	MCObsExpectation(vector<Dataset*> datasets, int nbins, float min, float max, string VarLabel, float Luminosity);
	MCObsExpectation(const MCObsExpectation& a);
	~MCObsExpectation();

	void SetColors(vector<Dataset*> datasets);
	void Fill(Dataset *dataset, float variable, float weight);
	void Compute();

	TH1F* GetHistogram(string hname);
	
	TH1F* Get_hNewPhysicsSignal(){return hNewPhysicsSignal;};
	THStack* GetTHStackSMProcesses(){return hstSMProcess;};/* return the sum of all SM processes*/
	THStack* GetTHStackAll(){return hstAll;};/*return the sum of all processes including new physics*/
	TH1F* GetHistoSMProcesses(){return hSMProcess;};/* return the sum of all SM processes*/
	TH1F* GetHistoAll(){return hAll;};/*return the sum of all processes including new physics*/
	TLegend* GetlegSMProcess(){return legSMProcess;};
	vector<string> datasetNames; /*make public so that he won't try to write this non-writable (???) members in a file when MCObsExp is written in file??*/
	vector<string> datasetTitles;
	
 private:
	float Luminosity;
	string Name;

	TLegend* legSMProcess;
	TLegend* legAll;
	
	THStack* hstSMProcess;
	THStack* hstAll;
	TH1F* hSMProcess;
	TH1F* hAll;
	
	//SetDatasets(vector<Dataset*> datasets)
	
	TObjArray* hSMSubProcesses;
		
	TH1F* hNewPhysicsSignal;
	
  ClassDef(MCObsExpectation,5); //was 4...
};

#endif
