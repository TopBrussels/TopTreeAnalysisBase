
#ifndef ANALYSIS_INTERFACE_EVENTBDT_h
#define ANALYSIS_INTERFACE_EVENTBDT_h


#define _USE_MATH_DEFINES

#include <cmath>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <map>

//user code
#include "TopTreeProducer/interface/TRootRun.h"
#include "TopTreeProducer/interface/TRootEvent.h"

#include "TopTreeAnalysisBase/Content/interface/AnalysisEnvironment.h"
#include "TopTreeAnalysisBase/Content/interface/Dataset.h"
#include "TopTreeAnalysisBase/Tools/interface/PlottingTools.h"
#include "TopTreeAnalysisBase/Tools/interface/MultiSamplePlot.h"
#include "TopTreeAnalysisBase/Tools/interface/TTreeLoader.h"
#include "TopTreeAnalysisBase/Tools/interface/AnalysisEnvironmentLoader.h"

#include "TopTreeAnalysisBase/Tools/interface/MVATrainer.h"
#include "TopTreeAnalysisBase/Tools/interface/MVAComputer.h"


using namespace std;
using namespace TopTree;

class EventBDT{
public:
	EventBDT();
	~EventBDT();
	void initaliseEventComp();
	void fillVariables(float ditopness, float muonpt, float muoneta, float HTH, float HTRat, float HTb, float nLtags, float nMtags, float nTtags, float nJets, float jet5Pt, float jet6Pt);
	void computeBDTScore();
	float returnBDTScore();
private:
	vector<string> mvaVars_;
	MVAComputer* eventComputer_;
	std::map<std::string,Float_t> mvaValues_;
	float scoreOfBDT_;
};

#endif
 