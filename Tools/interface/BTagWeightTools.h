// -*- C++ -*-
//
// Package:    TopTreeAnalysisBase/Tools
// Class:      BTagWeightTools
// 
/**\class BTagWeightTools TopBrussels/TopTreeAnalysisBase/Tools/src/BTagWeightTools.cxx TopBrussels/TopTreeAnalysisBase/Tools/interface/BTagWeightTools.h

 Description: Container class to parse BTV POG payloads which can be updated from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG

Unfortunately the BTV POG does not give the parameterizations in a parsable format that is consistent even per measurements, so you need to edit the files yourself to the level that they are usable. THe parsing code currently is tuned for the Moriond 2012 parameterizations from ttbar+mujet file

 Implementation:
     Loads a text file. Accessors return weights as a function of jet eta,pt for multiple options (central value, syst up, syst down, etc.)
*/
//
// Original Author:  "fblekman"
//         Created:  Fri Feb  1 12:10:50 CET 2013
// $Id: BTagWeightTools.h,v 1.1.2.1 2013/02/01 17:17:58 fblekman Exp $
//
//
#ifndef BTagWeightTools_h
#define BTagWeightTools_h

//
// class declaration
//

#include "TopTreeProducer/interface/TRootJet.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include <TF1.h>

using namespace std;
using namespace TopTree;

class BTagWeightTools
{
 public:


  // constructors, destructors
  BTagWeightTools();
  BTagWeightTools(std::string filename,std::string defaultalgo);
  BTagWeightTools(std::string filename);
  ~BTagWeightTools();
  
  
  // useful setters
  void setDefaultAlgo(std::string defaultalgo){_defaultalgo=defaultalgo;}
  
  // useful getters
  float getWeight(float pt, float eta, int flavor,int syst);
  float getWeight(float pt, float eta, int flavor,string algo,int syst);
  //  float getWeight(TRootJet *jet,int syst=0); // uses getWeight(float pt, float eta,int flavor,int syst=0);
  //  float getWeight(TRootJet *jet,string algo,int syst=0); // uses getWeight(float pt, float eta,int flavor,int syst=0);

  float getUncertainty(float pt, float eta, int flavor, int syst);
  float getUncertainty(float pt, float eta, int flavor, string algo, int syst);
  //  float getUncertainty(float pt, float eta, int flavor, int syst); // uses getUncertainty (float pt, float eta, int flavor, int syst);
  //  float getUncertainty(float pt, float eta, int flavor, string algo, int syst);  // uses getUncertainty (float pt, float eta, int flavor, int syst);

  // other getters
  std::string getCurrentFile()       {return _filename;} // returns path to currently used file

  // various workers

  //  void clear(){;}
  //  void setNewTextFile(std::string filename){}

  //    void printContent(void){;}
  
  // private workers
 private:

  std::map<string,TFormula> _functions ;
 
  std::vector<float> _etarangesysts;
  std::vector<float> _etarangesystshi;
  std::vector<float> _ptrangesysts;
  std::vector<float> _ptrangesysthi;

  std::map<string,std::vector<float> > _weightsUp;
  std::map<string,std::vector<float> > _weightsDown;
  int findindex(string algo, float pt, float eta,int flavor);
  void parsefile();
  std::string _filename;
  float _ptmin;
  float _ptmax;
  float _etamax;
  std::string _defaultalgo;
};

#endif
