

#ifndef ANALYSIS_INTERFACE_TRIGGER_H
#define ANALYSIS_INTERFACE_TRIGGER_H

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
#include "TopTreeAnalysisBase/Selection/interface/Run2Selection.h"


class Trigger{
	public:
		Trigger(bool isMuon, bool isElectron);
		~Trigger();
		void bookTriggers();
		void checkAvail(int currentRun, vector < Dataset* > datasets, unsigned int d,TRootEvent* event);
		int checkIfFired();
	private:
		bool muon;
		bool electron;
		bool trigged;
		bool redotrigmap;
	    std::vector<std::string> triggerList;
	    int currentRun;
	    int previousRun;
		string currentFilename;
		string previousFilename;
		int iFile;
	    std::map<std::string,std::pair<int,bool> > triggermap;

};

#endif