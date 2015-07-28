#ifndef ANALYSIS_INTERFACE_ZPEAK
#define ANALYSIS_INTERFACE_ZPEAK

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


class Zpeak{
public:
	Zpeak(vector < Dataset* > datasets);
	~Zpeak();
	void invariantMass(Run2Selection r2selection);
	void fillPlot(vector < Dataset* > datasets, unsigned int d, float Luminosity, float scaleFactor);
	void writeErase(TFile *fout, string pathPNG);
	float returnInvMass();
	bool requireTwoLeptons();

private:
	map<string,MultiSamplePlot*> MSPlot;
	vector<TRootMuon*> looseMuons_;
	vector<TRootMuon*> tightMuons_;	
	vector<TRootElectron*> looseElectrons_;
	vector<TRootElectron*> tightElectrons_;
	TLorentzVector diLeptonCollection_;
	float diLeptonInvMass_;	
	bool isTwoLeptons_;
};



#endif