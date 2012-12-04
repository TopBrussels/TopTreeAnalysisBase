#ifndef MultiSamplePlot_h
#define MultiSamplePlot_h

#include "TH1F.h"
#include "THStack.h"
#include "TFile.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TString.h"
#include "TGraph.h"

#include "Math/Random.h"
#include "Math/GSLRndmEngines.h"
#include "TopTreeAnalysis/Content/interface/Dataset.h"
#include "PlottingTools.h"

using namespace std;

/**
Aim: producing plots for several datasets using special color specified in the xml file.\\
It produces several outputs: a THStack and 2 TCanvas which are written in a root-file inside a directory "MultiSamplePlot_"+label.\\
One TCanvas contains normalized distributions for the several datasets, the other one contains a THStack with MC (many processes) + data/pseudo-data superimposed if specified.\\
The dataset containing data should be called "data" or "Data" or "DATA".\\
*/

class MultiSamplePlot{

	public:
		MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float Min, float Max, string XaxisLabel = string("Variable"), string YaxisLabel = string("#Events"));
		MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float* binsX, string XaxisLabel = string("Variable"), string YaxisLabel = string("#Events"));
		MultiSamplePlot(vector<pair<TH1F*,Dataset*> > vec);
		~MultiSamplePlot();

 		void AddDataHisto(TH1F* histo);/** Consider an additional histo as coming from data or pseudo-data*/
		void Fill(float value, Dataset* data, bool scale = false, float Lumi = 1.);/** if scale==true: histo are scaled by data.NormFactor()*Lumi */
		void Draw(bool addRandomPseudoData = false, string label = string("CMSPlot"), bool mergeTT = false, bool mergeQCD = false, bool mergeW = false, bool mergeZ = false, bool mergeST = false, int scaleNPSignal = 1, bool addRatio = false, bool mergeVV = false, bool mergeTTV = false);/** addRandomPseudoData will add a histo on top of the summed histograms which randomised according to a Poisson distribution. mergeTT/QCD/W/Z/ST/VV/TTV will merge all the ttbar/QCD/W/Z/single-top/diboson/ttW-Z together. */
		void Write(TFile* file, string label = string(""), bool savePNG = false, string pathPNG = string(""), string ext = string("png"));

		void setDataLumi(float lum) { lumi_ = lum; }
		void showNumberEntries(bool a) { showNumberEntries_ = a; }
		void setMaxY(float y) { maxY_ = y; }
		void addText(string text) { text_ = text; }
	
	private:
		vector<pair<TH1F*,Dataset*> > plots_;
		THStack* hStack_;
		THStack* hStackAreaNorm_;
		TH1F*    hRandomPseudoData_;
		TH1F*    hData_;
		TGraph*  hErrorPlus_; // for error band in Data/MC ratio
		TGraph*  hErrorMinus_;
		TCanvas* hCanvas_;
		TCanvas* hCanvasStack_;	
		TCanvas* hCanvasStackAreaNorm_;
		TCanvas* hCanvasStackLogY_;	
		TCanvas* hCanvasStackAreaNormLogY_;
		TLegend* leg_;
		TLegend* leg2_;
		int      Nbins_;
		float lumi_;
		bool showNumberEntries_;
		float maxY_;
		TString text_;
		string plotName_;
};

#endif
