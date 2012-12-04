#ifndef Resolution_h
#define Resolution_h

#include <iostream>
#include <iomanip>
#include <vector>
#include "TLorentzVector.h"

using namespace std;

class Resolution {

  
 public:
  
  Resolution();
  Resolution(pair<TLorentzVector,TLorentzVector> qFromW1, pair<TLorentzVector,TLorentzVector> qFromW2, pair<TLorentzVector,TLorentzVector> bQuark);
  Resolution(const Resolution& obs);
  ~Resolution();

  void ComputeVariables();

  float MassW() const {return MassW_;};
  float MassTop() const {return MassTop_;};
  //0: all - 1: l-jets - 2: b-jets
  //return reso & Pt
  std::vector<std::pair<float,float> > PtResolution(int jet=0);
  std::vector<std::pair<float,float> > EtaResolution(int jet=0);
  std::vector<std::pair<float,float> > PhiResolution(int jet=0);

 private:
 
   std::vector<pair<TLorentzVector,TLorentzVector> > lquark_;
   std::pair<TLorentzVector,TLorentzVector> bquark_;

   float MassW_;
   float MassTop_;
  
};

#endif
