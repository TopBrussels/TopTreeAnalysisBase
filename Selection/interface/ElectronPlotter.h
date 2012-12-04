#ifndef ElectronPlotter_h
#define ElectronPlotter_h

#include "TopTreeProducer/interface/TRootElectron.h"

// system include files
#include <memory>
#include <vector>

//ROOT
#include "TFile.h"
#include "TH1F.h"

using namespace std;
using namespace TopTree;
				
/**
Aim: making standard distribution for electrons with a given selection described by a label. Plots are then saved inside a TFile with organized substructure
*/

class ElectronPlotter{

   public:
      ElectronPlotter(string label);
      ElectronPlotter(const ElectronPlotter & mp);
      ~ElectronPlotter();
      
      void Fill(vector<TRootElectron> electrons);
      void Write(TFile* fout);
      string GetLabel(){return label_;};

   private:
      string label_;
      //plots for highest Pt Electron (HPTElectron)
      TH1F* h_HPTElectron_Pt_;
      TH1F* h_HPTElectron_Eta_;
      TH1F* h_HPTElectron_d0_;
      TH1F* h_HPTElectron_RelIso_;
      //plots for all electrons
      TH1F* h_AllElectrons_mult_;
      TH1F* h_AllElectrons_Pt_;
      TH1F* h_AllElectrons_Eta_;
      TH1F* h_AllElectrons_d0_;
      TH1F* h_AllElectrons_RelIso_;
};

#endif
