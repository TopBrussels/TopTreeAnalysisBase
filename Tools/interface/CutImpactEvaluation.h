#ifndef CutImpactEvaluation_h
#define CutImpactEvaluation_h

#include <sstream>

#include "TF1.h"
#include "TH1F.h"
#include "THStack.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "TopTreeAnalysis/Content/interface/Dataset.h"
#include "PlottingTools.h"

using namespace std;


 	 //////////////////////////////////////////////////////////////////////////////////////
         //
	 //	Aim: Checking the impact of adding a cut C (ex: cut on Mbl) for the shape of a given variable (ex: MET)
	 //		Distorsion (via Chi2) & efficiency plots are calculated 
         //
 	 //////////////////////////////////////////////////////////////////////////////////////
				 

class CutImpactEvaluation{

	public:
		//Nbins, VarMin, VarMax is related to the distribution (ex: MET)
		//Nplots, Min, Max is related to the variable you cut on in your analysis (ex: Mbl)
		// lowercut boolean is true if the if statements are: Var>Cut
		// bkg is "what you want to reject"
		CutImpactEvaluation(string PlotName, int Nbins, float VarMin, float VarMax, int Nplots, float Min, float Max, vector<Dataset> bkg, string XaxisLabel = string("Cut On Variable"), bool lowercut = true);
		CutImpactEvaluation(string PlotName, int Nbins, float* binsX, int Nplots, float Min, float Max, vector<Dataset> bkg, string XaxisLabel = string("Cut On Variable"), bool lowercut = true);	
		~CutImpactEvaluation();

		//return chi2 and efficiency for a given cut
		pair<float,float> GetInformation(float cut);
		//return first: cut second: chi2
		pair<float,float> GetCutForAGivenEfficiency(float eff);
		//return first: cut second: eff
		pair<float,float> GetCutForAGivenChi2(float chi2);
	
		void Fill(float value, float variabl, bool isBkg = false, unsigned int iDataset = 0);
		void Draw(string label = string("cMCPlot"));
		void Write(TFile* file, string label = string(""));

	private:

		bool lowercut_;
		int Nplots_;
		float* cuts_;
		TH1F** plots_;
		TH1F* h_ref_;
		TCanvas* hCanvas_;
		TCanvas* hCanvas2_;
		TH1F* hDeformation_;
		TH1F* hEfficiency_;
		vector<Dataset> BkgList_;
		vector<TH1F*> hEffxRej_;
		vector<TH1F*> hRejection_;
		vector<TH1F*> hBkg_;

};

#endif
