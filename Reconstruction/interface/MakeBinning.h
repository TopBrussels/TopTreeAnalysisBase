#ifndef MakeBinning_h
#define MakeBinning_h
#include "TopTreeAnalysis/Reconstruction/interface/Observables.h"

#include <iostream>
#include <iomanip>
#include <string.h>
#include <sstream>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
using namespace std;
using namespace TopTree;

class MakeBinning {

  
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



  MakeBinning();
  MakeBinning(const vector<string>& vec, const vector<pair<float,float> >& range);
  ~MakeBinning();
 
 void Fill(const Observables& obs, float & weight_);
 void Fill(string &var_, float &val_,  float & weight_);
void Binning(string & decision_, int nbins_, int evtsperbin_);
void Binning(string & decision_,  vector < pair <string,float > > & contents_,int nbins_, int evtsperbin_, float & weight_);
void Binning_forTprimeAnalysis(string myxvariable, string myyvariable, map<string,vector<float > > VariableValuesMap, map<string,int> nbinsMap, vector<float> eventweightvector, string outputfilename);


private:
  vector<string> label_;
  vector < pair < float, float > >  range_; 
  vector <double > PlotContentValue_;
  vector <double > PlotContentFraction_;
  vector <string> variable_string_;
// vector < pair < string, float > > PlotContentAll_;
  int NofVar_;
  float* var_;
 };


#endif
