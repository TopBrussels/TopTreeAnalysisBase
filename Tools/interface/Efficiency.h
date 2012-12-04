#ifndef Efficiency_h
#define Efficiency_h

#include <iostream>
#include <iomanip>
#include <string>
#include "stdio.h"
#include "math.h"
#include "TH1F.h"

using namespace std;

class Efficiency{

  
 public:
  
  Efficiency();
  Efficiency(string label_, string labelVariable_, char* name, char* title, int nbins, float min, float max, int lineWidth = 2, int lineColor = 1);
  Efficiency(const Efficiency& eff);
  ~Efficiency();

  void Compute();
  
  void Fill(float variable, bool b);
 
  float GetEfficiency() const{ return ((float)(subSetEvents_/subSetEvents_));};
  TH1F GetHisto() const{ return * histo_;};
  void ConfigHisto(char* name, char* title, int nbins, float min, float max, int lineWidth = 2, int lineColor = 1);
  string GetLabel() const{ return label_;};
  string GetLabelVariable() const{ return labelVariable_;};
  void SetLabel(string label){ label_ = label;};
  void SetLabelVariable(string label){ labelVariable_ = label;};
  
  friend std::ostream& operator<< (std::ostream& stream, const Efficiency& eff){
     stream <<" Effiency for "<< eff.GetLabel()<<" : "<<eff.GetEfficiency();
     return stream;
  };
  
 
 private:
   int subSetEvents_;
   int nofEvents_;
   TH1F* histo_;
   TH1F* histoRef_;
   string label_;
   string labelVariable_;
 
};

#endif
