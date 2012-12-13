#ifndef PlotTopology_h
#define PlotTopology_h
#include "TopMatching.h"
#include <iostream>
#include <iomanip>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TObjArray.h"
#include "TObject.h"

#include <string>
#include <sstream>
#include <cmath>

using namespace std;
using namespace TopTree;

struct histo {
     TH1F* Multiplicity;
     TH1F* Pt;
     TH1F* Eta;
     TH1F* Phi;
     TH1F* Gamma;
};

struct histomultibody {
     TH1F* InvMass;
     TH1F* DeltaR;
     TH1F* MinDeltaR;
     TH1F* MaxDeltaR;
     TH1F* Ht;	/*scalar sum of transverse momenta*/
	
     TH2F* DeltaR_vs_Pt;
     TH2F* DeltaR_vs_Eta;
     TH2F* DeltaR_vs_Gamma;
};


class PlotTopology {
  
 public:
  
     PlotTopology();
     PlotTopology(const PlotTopology& psel);
     ~PlotTopology();
 
     bool doSemiLeptonicTopology;	/* set true when you want to select events with "semileptonic ttbar-like topology", i.e. 1 lepton and 4 quarks*/
     int nEvents;
     int nSelectedEvents;
     double Efficiency;
     
     void Compute();
     void InitStruct(histo &h, string name);
     void InitStruct(histomultibody &h, string name);
     void Fill(TRootNPGenEvent& NPgenEvt);
     void Fill(TRootGenEvent& genEvt);	/*called when looping over TRootGenEvents in stead of TRootNPGenEvents*/
     void Write(TFile* fout);

 private:
      
      histo ElectronHisto;
      histo MuonHisto;
      histo TauonHisto;  
      histo ElectronneutrinoHisto;
      histo MuonneutrinoHisto;
      histo TauonneutrinoHisto;    
      histo GluinoHisto;   
      histo StopHisto;
      histo TopHisto;
      
      histomultibody LeptonsHisto;
      histomultibody TopqqHisto;	/*quark and quarkbar from W (from top)*/
      histomultibody Topqb_1Histo;	/*quark from W and bquark (from top)*/
      histomultibody Topqb_2Histo;	/*quarkbar from W and bquark (from top)*/
      histomultibody ToplnuHisto;	/*lepton and neutrino from W (from top)*/
      histomultibody ToplbHisto;	/*lepton from W and bquark (from top)*/
      histomultibody TopnubHisto;	/*neutrino from W and bquark (from top)*/
      
      histomultibody Topqq_vs_TopHisto;	/* quark and quarkbar from W (from top), versus top*/
      histomultibody Topqq_vs_WHisto;	/* quark and quarkbar from W (from top), versus W*/
      
      histomultibody TopqqbHisto;	/*quark, quarkbar and bquark (from top)*/
      
      histomultibody leadingmuqHisto;	/*leading (highest Pt) muon and quark*/
 
      /*
      TH1F* h_MinDeltaRqqb;
      TH1F* h_MinDeltaRqqb_cut;
      TH2F* h_MinDeltaRqqb_vs_PtTop;
      TH2F* h_MinDeltaRqqb_vs_EtaTop;
      TH2F* h_MinDeltaRqqb_vs_GammaTop;
      
      TH1F* h_MaxDeltaRqqb;
      TH1F* h_MaxDeltaRqqb_cut;
      TH2F* h_MaxDeltaRqqb_vs_PtTop;
      TH2F* h_MaxDeltaRqqb_vs_EtaTop;
      TH2F* h_MaxDeltaRqqb_vs_GammaTop;
      
      TH1F* h_DeltaRqq;
      TH1F* h_DeltaRqq_cut;
      TH2F* h_DeltaRqq_vs_PtTop;
      TH2F* h_DeltaRqq_vs_EtaTop;
      TH2F* h_DeltaRqq_vs_GammaTop;
      TH2F* h_DeltaRqq_vs_PtW;
      TH2F* h_DeltaRqq_vs_EtaW;
      TH2F* h_DeltaRqq_vs_GammaW;
      */
};

#endif
