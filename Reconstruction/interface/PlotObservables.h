#ifndef PlotObservables_h
#define PlotObservables_h
#include "TopTreeAnalysis/Reconstruction/interface/Observables.h"

#include <iostream>
#include <iomanip>
#include <string.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
using namespace std;
using namespace TopTree;

class PlotObservables {

  
 public:
  
struct sort_string_float {
    bool operator()(const std::pair<string,float> &left, const std::pair<string,float> &right) {
        return left.second < right.second;
    }
};

struct sort_string_double {
    bool operator()(const std::pair<string,double> &left, const std::pair<string,double> &right) {
        return left.second < right.second;
    }
};



  PlotObservables();
  PlotObservables(const vector<string>& vec, const vector<pair<float,float> >& range);
  PlotObservables(const PlotObservables& pobs);
  ~PlotObservables();

  void Normalized();
  void Fill(const Observables& obs, float & weight_,bool & fill);
  void Write(string &dset, bool normalized = false);
  
  void Write(TString &dset, string &decision_,const vector<pair<string,float> > & BinContentObsAll,int evtsPerBin_, int NBins_,float & weight_,bool normalized = false);
  
  void WriteEvtsPerBin(TString &dset,const vector<pair<string,float> > & BinContentObsAll,int evtsPerBin_, float & weight_,bool normalized = false);
  void WriteNBins(TString &dset,const vector<pair<string,float> > & BinContentObsAll,int NBins_,float & weight_,bool normalized = false);
  
  void Write(string &dset, const vector<pair<string,float> > & BinContentObsAll,const vector<pair<string,float> > & BinContentObs,int evtsPerBin_, bool normalized = false);

 private:
  vector<string> label_;
  vector<TH1F> h_variables;
  vector<TH1F> h_variables_bin;
  vector<TH1F> h_variables_binned;
  vector<TH2F> h_variables2D;
  vector < pair < float, float > >  range_; 
  vector <double > PlotContentValue_;
  vector <double > PlotContentFraction_;
  vector <string> variable_string_;
  int NofVar_;
  float* var_;
  TTree *tree;
  TTree *tr;
 };


#endif
