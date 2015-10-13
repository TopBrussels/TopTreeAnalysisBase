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
  MuonSFWeight (const string &, const string &, const bool &debug = false);
  ~MuonSFWeight ();
  double at (const double &, const double &, const int &shiftUpDown = 0);

 private:
  TH2 *muonSFWeight_;
  bool debug_;
};


class ElectronSFWeight
{
 public:
  ElectronSFWeight () {};
  ElectronSFWeight (const string &, const string &, const bool &debug = false);
  ~ElectronSFWeight ();
  double at (const double &, const double &, const int &shiftUpDown = 0);

 private:
  TH2 *electronSFWeight_;
  bool debug_;
};


#endif
