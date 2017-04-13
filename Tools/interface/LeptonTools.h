#ifndef LeptonTools_h
#define LeptonTools_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "TH2.h"
#include <TGraphAsymmErrors.h>
#include <TFile.h>

//TopTreeProducer classes
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootElectron.h"

using namespace std;
using namespace TopTree;

class MuonSFWeight
{
 public:
  MuonSFWeight () {};
  MuonSFWeight (const string &, const string &, const bool &extendRange = false, const bool &debug = false, const bool &printWarning = true);
  ~MuonSFWeight ();
  double at (const double &eta, const double &pt, const int &shiftUpDown = 0);

 private:
  TH2 *muonSFWeight_;
  bool extendRange_;
  bool debug_;
  bool printWarning_;
};


class ElectronSFWeight
{
 public:
  ElectronSFWeight () {};
  ElectronSFWeight (const string &, const string &, const bool &extendRange = false, const bool &debug = false, const bool &printWarning = true, const bool &recoSF = false);
  ~ElectronSFWeight ();
  double at (const double &eta, const double &pt, const int &shiftUpDown = 0);

 private:
  TH2 *electronSFWeight_;
  bool extendRange_;
  bool recoSF_;
  bool debug_;
  bool printWarning_;
};


#endif
