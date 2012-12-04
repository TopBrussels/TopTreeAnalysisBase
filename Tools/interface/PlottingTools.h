#ifndef PlottingTools_h
#define PlottingTools_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TFile.h"
#include "TF1.h"
#include "TF2.h"
#include "TLegend.h"
#include "TGaxis.h"
#include "TProfile.h"

//user
#include "TopTreeAnalysis/Content/interface/Dataset.h"

using namespace std;

//general
void CopyHisto(TH1F* original, TH1F* copy);
void ConfigureHisto(TH1F* histo, Dataset* dataset, bool fillMode = false);
float YMax(vector<TH1F*> listOfHistos);

//Differences between histo
float DiffEstimation(TH1F* h1, TH1F* h2);
float Chi2Normalized(TH1F* h1, TH1F* h2, bool scaled = true);

float WilsonErrorHigh(float Non, float Ntot);
float WilsonErrorLow(float Non, float Ntot);

//Compute cumulative diff of 2 histograms
TH1F* PlotCumulDiffEstimation(TH1F* h1, TH1F* h2, TString);

//Compute ratio of 2 histograms
TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator);
TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator); 
TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator, string histoName);
TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator, string histoName);
TH2F* PlotRatio(TH2F* hnumerator, TF2* fdenominator, string histoName);
// with fit
TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator, TF1* func, bool doFit = false);
TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator, TF1* func, bool doFit = false); 
TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator, TF1* func, string histoName, bool doFit = false);
TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator, TF1* func, string histoName, bool doFit = false); 

//Compute a tstack starting with a ordered collection of histograms
THStack* THStackCreator(vector<TH1F*> listOfHistos);
THStack* THStackCreator(vector<TH1D*> listOfHistos);
THStack* THStackCreator(vector<TH1F*> listOfHistos, vector<int> listOfColor);
THStack* THStackCreator(vector<TH1D*> listOfHistos, vector<int> listOfColor);
THStack* THStackCreator(vector<TH1F*> listOfHistos, vector<Dataset*> datasets, TLegend* leg, bool showNbEntries=true);
THStack* THStackCreator(vector<TH1D*> listOfHistos, vector<Dataset*> datasets, TLegend* leg, bool showNbEntries=true);

//Make a TGraphAsymmErrors with B.G. scale factors.
TGraphAsymmErrors* BGScaling(TH1F* hJetMultiplicity);
//Make a TGraphAsymmErrors from two histo's
TGraphAsymmErrors* GraphRatio(TH1F* hnumerator, TH1F* hdenominator, string label, string xAxisLabel = "", string yAxisLabel = "");

//Canvas for a single histo
TCanvas* TCanvasCreator(TH1F* histo, string label);
TCanvas* TCanvasCreator(TH2F* histo, string label, string drawOption="colz");
TCanvas* TCanvasCreator(TH1* histo, TF1* fit, string label);
TCanvas* TCanvasCreator(TH2F* histo, TF2* fit, string label, string drawOption="colz");

//Canvas for a TGraphAsymmErrors
TCanvas* TCanvasCreator(TGraphAsymmErrors* graph, string label);

//Canvas for a TGraphErrors
TCanvas* TCanvasCreator(TGraphErrors* graph, string label, string options = "AP");

//Canvas for TProfile
TCanvas* TCanvasCreator(TProfile* profile, string label);

//Canvas for TGraph
TCanvas* TCanvasCreator(TGraph* graph, string label, string options = "AP");
TCanvas* TCanvasCreator(vector<std::pair<TGraph*,std::string> > listOfHistos, string label);

//Canvas for a list of Histos
TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset> datasets, TLegend* leg, string option = string("l"), string label = string("c1"));
TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset> datasets, TH1F* hData, string dataLabel, TLegend* leg, string option = string("l"), string label = string("c1"));

TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset*> datasets, TLegend* leg, string option = string("l"), string label = string("c1"));
TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset*> datasets, TH1F* hData, string dataLabel, TLegend* leg, string option = string("l"), string label = string("c1"));
TCanvas* TCanvasCreator(vector<std::pair<TH1F*,std::string> > listOfHistos, string option = string("l"), string label = string("c1"), bool norm=false, bool doStack=false);
TCanvas* TCanvasCreator(vector<std::pair<TH1F*,std::string> > listOfHistos, vector<int> listOfColor, string option = string("l"), string label = string("c1"), bool norm=false, bool doStack=false);

//Return a TCanvas with plots on top and bottom
//TCanvas* TCanvasCreator(TH1F* histoTop, TH1F* histoBottom);
//TCanvas* TCanvasCreator(TH1D* histoTop, TH1D* histoBottom);
//TCanvas* TCanvasCreator(vector<TH1F*> histoTop, vector<TH1F*> histoBottom);
//TCanvas* TCanvasCreator(vector<TH1D*> histoTop, vector<TH1D*> histoBottom);

TCanvas* TCanvasCreator(vector<TH1F*> histoTop, TH1F* histoBottom, TLine* line, TLegend* leg = 0, string label = string(""), string canvasName = string(""), float MaxY = 2.0, bool doFit = false);
TCanvas* TCanvasCreator(vector<TH1F*> histoTop, vector<TH1F*> histoBottom, TLine* line, TLegend* leg = 0, string label = string(""), string canvasName = string(""), float MaxY = 2.0, bool doFit = false);
TCanvas* TCanvasCreator(TH1F* histoTop, TH1F* histoBottom, TLine* line, TLegend* leg = 0, string label = string(""), string canvasName = string(""), float MaxY = 2.0, bool doFit = false);
TCanvas* TCanvasCreator(TH1F* histoTop, TGraphAsymmErrors* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit);
TCanvas* TCanvasCreator(THStack* histoTop, TH1F* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit);
TCanvas* TCanvasCreator(THStack* histoTop, TGraphAsymmErrors* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit);
//superimpose the histograms with 2 diff scales
//second plot is in red
TCanvas* TwoScales(TH1F* h1, TH1F* h2, string label);
TCanvas* TwoScales(TH1F* h1, TH1F* h2, vector<TH1F*> histos, string label, float LeftMax = -9999, float RighMax = -9999, bool scaled = false);
#endif
