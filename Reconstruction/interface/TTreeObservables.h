#ifndef TTreeObservables_h
#define TTreeObservables_h
#include "Observables.h"

#include <iostream>
#include <iomanip>
#include <string.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

class TTreeObservables {

  
 public:
  
  TTreeObservables();
  TTreeObservables(string & setname);
  TTreeObservables(const vector<string>& vec,string & setname);
  TTreeObservables(const TTreeObservables& pobs);
  ~TTreeObservables();


  //void Normalized();
  void Fill(const Observables& obs);
  void Fill(const Observables& obs,bool fill_);
  void FillTtree();
  void Write(TFile* f1, bool normalized = false);
 
  void Write(bool normalized = false);
  TTree* GetTree(){ return tree;};
 
 private:

  vector<string> label_;
  int NofVar_;
  float* var_;
  TFile* f1;
  TTree* tree;
  vector<string> vecc;
  

};

#endif
