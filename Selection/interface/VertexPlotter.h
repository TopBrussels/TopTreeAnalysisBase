#ifndef VertexPlotter_h
#define VertexPlotter_h

#include "TopTreeProducer/interface/TRootVertex.h"

// system include files
#include <memory>
#include <vector>
#include <cmath>

//ROOT
#include "TFile.h"
#include "TH1F.h"

using namespace std;
using namespace TopTree;
				
/**
Aim: making standard distribution for muons with a given selection described by a label. Plots are then saved inside a TFile with organized substructure
*/

class VertexPlotter{

   public:
      VertexPlotter(string label);
      VertexPlotter(const VertexPlotter & mp);
      ~VertexPlotter();
      
      void Fill(vector<TRootVertex> vertex);
      void Write(TFile* fout);
      string GetLabel(){return label_;};

   private:
      
      string label_;
      //plots for Primary Vertex
      TH1F* h_PVertex_isValid_;
      TH1F* h_PVertex_isFake_;
      TH1F* h_PVertex_chi2_;
      TH1F* h_PVertex_ndof_;
      TH1F* h_PVertex_tracksSize_;
      TH1F* h_PVertex_absZ_;
      TH1F* h_PVertex_rho_;
      //plots for all vertices
      TH1F* h_AllVertex_mult_;
      TH1F* h_AllVertex_isValid_;
      TH1F* h_AllVertex_isFake_;
      TH1F* h_AllVertex_chi2_;
      TH1F* h_AllVertex_ndof_;
      TH1F* h_AllVertex_tracksSize_;
      TH1F* h_AllVertex_absZ_;
      TH1F* h_AllVertex_rho_;
};

#endif
