#ifndef MultiCutPlot_h
#define MultiCutPlot_h


#include "TF1.h"
#include "TH1F.h"
#include "THStack.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "TopTreeAnalysis/Content/interface/Dataset.h"
#include "PlottingTools.h"

using namespace std;

class MultiCutPlot{

	public:
		MultiCutPlot(vector<pair<string,int> > cuts, string PlotName, int Nbins, float Min, float Max, string XaxisLabel = string("Variable"), string YaxisLabel = string("#Events"));
		MultiCutPlot(vector<pair<string,int> > cuts, string PlotName, int Nbins, float* binsX, string XaxisLabel = string("Variable"), string YaxisLabel = string("#Events"));
		MultiCutPlot(vector<pair<TH1F*,string> > vec, vector<int> color);
		~MultiCutPlot();

		void Fill(float value, string);
		void Draw(string label = string("cMCPlot"), float YMax = 2.);
		void TailEstimation(float threashold, string label = string("this variable"));
		void Write(TFile* file, string label = string(""));

	private:

		vector<pair<TH1F*,string> > plots_;
		vector<int> color_;
		TCanvas* hCanvas_;
		TLegend* leg_;
		TF1* func_;

};

#endif
