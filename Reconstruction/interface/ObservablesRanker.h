#ifndef ObservablesRanker_h
#define ObservablesRanker_h


#include "Observables.h"

#include <string.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "TList.h"
#include <string>
#include "TObject.h"
#include "TBranch.h"
#include <functional>
#include "TAxis.h"
using namespace std;



///**
//Your comments
///*/

class ObservablesRanker{

 public:
  
  ObservablesRanker();
  ObservablesRanker(const Observables & obs);
  ObservablesRanker(TTree *  tSignal_, TTree *tBkg_, string & merged_file, string & np_file, string &sm_file, bool &tbl_overlap_, bool &tbl_correlation_, bool &tbl_overlap_sorted_cleaned_, bool & DoCorrs_,vector <float> Cor_Cut_);
  ObservablesRanker( string & merged_file, string & np_file, string &sm_file, bool &tbl_overlap_, bool &tbl_correlation_, bool &tbl_overlap_sorted_cleaned_, float & Cor_Cut);
 ~ObservablesRanker();

  void ComputeOverlap(string &fout, string &merged_file,string &np_file);
  void ModifyHist(TH1F* &h, Color_t lcolor);
  void Impose( TDirectory*ttarget, TList *ssourcelist, string &np_legend );
  void PrintOverlapTable(bool & table_overlap_);
  void PrintCorrelationTable(bool &table_correlation_ , float & Cor_Cut);
  void DoOnlyCorrelation(bool &table_correlation_ );
  void DoCorrelation(bool &table_correlation_ ,  float &Cor_Cut);
  void PrintOnlyCorrelation(bool &table_correlation_ , float & Cor_Cut);
  void PrintOverlapSortedCleanedTable(bool &table_overlap_sorted_cleaned_, string & fin, string & fout);
  void PrintOverlapSortedCleanedTable(bool &table_overlap_sorted_cleaned_);
  void SaveBestObservablesList(string &fout, float & Cor);
  void SaveCorrelanceObservablesList(string &fout);
  void Read(string & fout, float & Cor);
  TAxis * getBinningStatisticsRequirement(TH1F *h1_, Int_t mnEntries);

struct sort_pred {
    bool operator()(const std::pair<string,float> &left, const std::pair<string,float> &right) {
        return left.second < right.second;
    }
};



struct sort_string_int {
    bool operator()(const std::pair<string,int> &left, const std::pair<string,int> &right) {
        return left.second < right.second;
    }
};


struct sort_two_int {
    bool operator()(const std::pair<int,int> &left, const std::pair<int,int> &right) {
        return left.second < right.second;
    }


};



 float GetRankedVariableByPos(int position);

 float GetRankedVariableByName(const string);

 string GetRankedVariableName(int position);

 string GetVariableName(int position);

string GetVarsForPseudo(int position);

 int GetRankingofVariable(const string );

 float GetCorrelation(const string, const string);

 private:

  Double_t CS;
  Double_t bincontS;
  Double_t bincontB;

   //TTrees
   TTree* tree1_;
   TTree* tree2_;
   TTree * tree_Signal;
   TTree * tree_Bkg;
   //overlap
   vector<pair<string , float> > overlaps_;
   vector<pair<string , float> > overlaps_sorted_;
   vector<pair<string, float> >  overlaps_sorted_cleaned_; 
   vector<pair<string, float> >  overlaps_sorted_pro_cleaned_; 
   vector <string> overlaps_sorted_cleaned_name_;
   vector <float> overlaps_sorted_cleaned_value_;
   //correlations
   vector<vector<float> >   correlations_;
   vector<vector<float> >   correlations_sorted_;
   vector<string> variables_;  
   vector<float> correlation_;
   vector<float> correlation_sorted_;
   vector<pair<string, float> > correlation_with_name_;
   vector<pair<string, float> > correlation_with_name_sorted_;
   vector<pair<string, float> > all_correlation_table_;
   
   vector<pair<string, int> > high_correlation_position_;
   vector<pair<string,string> > high_correlation_matrix_;
   vector<float> correlation_sorted_cleaned_;
   vector<vector<float> > correlations_sorted_cleaned_;
};

#endif
