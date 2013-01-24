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
  TTreeObservables(string & setname,bool CREATE_);
  TTreeObservables(const vector<string>& vec);
  TTreeObservables(const vector<string>& vec,string & setname);
  TTreeObservables(const vector<string>& vec,string & setname,bool CREATE_);
  TTreeObservables(const TTreeObservables& pobs);
  ~TTreeObservables();


  //void Normalized();
  void Fill(const Observables& obs);
  void Fill(const Observables& obs,bool fill_);
  void Fill(const Observables& obs,float weight_, int &btagL_, int  &btagM_, int & btagT_,bool fill_);
  void FillTtree();
  void SaveInfo(string & fout, const vector<pair< string, float > > & vector_);
  void Write(TFile* f1, bool normalized = false);
 
  void Write(bool normalized = false);
  TTree* GetTree(){ return tree;};
 
 private:

  vector<string> label_;
  int NofVar_;
  int NofVarr_;
  float* var_;
  float* varr_;
  TFile* f1;
  TTree* tree;
  vector<string> vecc;
  vector<pair<string, float > >  vector_;
  

};

#endif
