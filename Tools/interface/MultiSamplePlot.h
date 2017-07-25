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
#include "TGraphAsymmErrors.h"
#include "TPad.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"
#include "TStyle.h"

#include "TopTreeAnalysisBase/Content/interface/Dataset.h"
#include "TopTreeAnalysisBase/Tools/interface/PlottingTools.h"

using namespace std;

/**
 Aim: producing plots for several datasets using color specified in the xml file.\\
 It produces several outputs: a THStack and 2 TCanvas which are written in a root-file inside a directory "MultiSamplePlot_"+label.\\
 One TCanvas contains normalized distributions for the several datasets, the other one contains a THStack with MC (many processes) + data superimposed if specified.\\
 The dataset containing data should be called "data" or "Data" or "DATA".\\
 */

class MultiSamplePlot{
  
public:
  //		MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float Min, float Max, string XaxisLabel = string("Variable"), string YaxisLabel = string("Events"), string AddText = string(""));
		MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float Min, float Max, string XaxisLabel = string("Variable"), string Units = string(""), string YaxisLabel = string("Events"), string AddText = string(""));
		MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float* binsX, string XaxisLabel = string("Variable"), string YaxisLabel = string("Events"), string AddText = string(""));
		MultiSamplePlot(vector<pair<TH1F*,Dataset*> > vec, string PlotName, string XaxisLabel, string YaxisLabel, string AddText);
		~MultiSamplePlot();
  
  void AddDataHisto(TH1F* histo); /**add a data histogram*/
		void Fill(float value, Dataset* data, bool scale = true, float Lumi = 1.); /**if scale==true: histo are scaled by data.NormFactor()*Lumi */
		void Draw(string label = string("CMSPlot"), unsigned int RatioType = 0, bool addRatioErrorBand = false, bool addErrorBand = false, bool ErrorBandAroundTotalInput = false, int scaleNPSignal = 1, double scaleFake = 1. ); /**RatioType=1: ratio = data/MC, RatioType=2: ratio=(data-MC)/MC and RatioType=0: no ratio plot drawn. ErrorBandAroundTotalInput set on false means that the error band will be centered around a nominal histogram read in from an external errorband file*/
		void Write(TFile* file, string label = string(""), bool savePNG = false, string pathPNG = string(""), string ext = string("png"));
  
		void setDataLumi(float lum) { lumi_ = lum; } /**if running over data, you don't need to use this method, as the lumi will be taken from the dataset info*/
		void setSqrtsTeV(float sqrts) { sqrts_ = sqrts; } /**"sqrt(s) = 8 TeV" displayed by default in title*/
		void setPreliminary(bool prelim) { prelim_ = prelim; } /**"CMS Preliminary" displayed by default in title; set it to "CMS" via this method*/
  void setChannel(bool chan, string channel){chan_ = chan; channel_ = channel; }
  void setBins(vector <string> vlabel){ setBinLabels_ = true; vlabel_ = vlabel;  } // give labels to the bins
  void doCutFlowPlot(){ doCutFlow_ = true;} // if you want a cutflow plot
		void showNumberEntries(bool a) { showNumberEntries_ = a; } /**show number of entries in the legend (default = true)*/
		void setMaxY(float y) { maxY_ = y; } /**set maximum value of y-axis*/
		void setMinLogY(float y) { minLogY_ = y; } /**set minimum value of log-scale y-axis*/
		void addText(string text) { text_ = text; } /**add some text on the plot (as alternative to specifying it in the constructor)*/
		string getplotName() {return plotName_; }
		void setErrorBandFile(string errorbandfile, bool dosystfile = false) { errorbandfile_ = errorbandfile; dosystfile_ = dosystfile;} /**set the file where to look for the errorbands. If systfile is set to true, this file actually only contains systematic shifted histograms, not yet the actual error 'band'*/
		
  
private:
  
  void Initialize(); /**initialize some private members common for all MultiSamplePlot constructors*/
  void DrawStackedPlot(TCanvas* canvas, TCanvas* canvasLogY, THStack* hstack, vector<TH1F*> histosForOverlay, int scaleNPSignal = 1, const char* xaxistitle = "", const char* yaxistitle = "", unsigned int RatioType = 0);
  void DrawErrorBand(TH1F* totalSM, TH1F* hErrorPlus, TH1F* hErrorMinus, TH1F* hNominal, TGraphAsymmErrors*  ErrorGraph, bool ErrorBandAroundTotalInput);
  
		void setTDRStyle();
		vector<pair<TH1F*,Dataset*> > plots_;
		TH1F*    hData_;
		TCanvas* hCanvas_;
		TCanvas* hCanvasStack_;
		TCanvas* hCanvasStackAreaNorm_;
		TCanvas* hCanvasStackLogY_;
		TCanvas* hCanvasStackAreaNormLogY_;
		THStack* hStack_;
		THStack* hStackAreaNorm_;
		TGraphAsymmErrors *  StackErrorGraph_; /**for error band on stacked plot */
		TGraphAsymmErrors *  RatioErrorGraph_; /**for error band on Data/MC ratio plot*/
		float maxY_;
		float minLogY_;
		float logYMultiplicator_;
		float maxLogY_;
		float maxLogYAreaNorm_;
		TString text_;
		bool showNumberEntries_;
		string errorbandfile_;
		bool dosystfile_;
		TLegend* leg_;
		TLegend* legAreaNorm_;
		int      Nbins_;
		float lumi_;   /**in pb^-1, for cms title of plot*/
		float sqrts_;  /**in TeV, for cms title of plot */
		bool prelim_;  /**to display "Preliminary" in cms title of plot*/
  bool chan_;  /** to displac decay channel */
  string channel_ = "" ; /** decaychannel */
  bool setBinLabels_;
  bool doCutFlow_; 
  vector<string> vlabel_ = {""};
		string plotName_;
		//Variables from May 2016 version of CMS Plotting style
  TString cmsText     = "CMS";
		float cmsTextFont   = 61;  // default is helvetic-bold
  
		bool writeExtraText = false;
		TString extraText   = "Preliminary";
		float extraTextFont = 52;  // default is helvetica-italics
  
		// text sizes and text offsets with respect to the top frame
		// in unit of the top margin size
		float lumiTextSize     = 0.6;
		float lumiTextOffset   = 0.2;
		float cmsTextSize      = 0.75;
		float cmsTextOffset    = 0.1;  // only used in outOfFrame version
  
		float relPosX    = 0.045;
		float relPosY    = 0.035;
		float relExtraDY = 1.2;
  
		// ratio of "CMS" and extra text size
		float extraOverCmsTextSize  = 0.76;
  
		TString lumi_13TeV = "20.1 fb^{-1}";
		TString lumi_8TeV  = "19.7 fb^{-1}";
		TString lumi_7TeV  = "5.1 fb^{-1}";
		TString lumi_sqrtS = "";
  
		bool drawLogo      = false;
};

#endif
