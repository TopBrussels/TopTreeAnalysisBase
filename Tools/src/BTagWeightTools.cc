// -*- C++ -*-
//
// Package:    TopTreeAnalysisBase/Tools
// Class:      BTagWeightTools
// 
/**\class BTagWeightTools TopBrussels/TopTreeAnalysisBase/Tools/interface/BTagWeightTools.cxx TopBrussels/Tools/interface/BTagWeightTools.h

 Description: Container class to parse BTV POG payloads which can be updated from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG

Unfortunately the BTV POG does not give the parameterizations in a parsable format that is consistent even per measurements, so you need to edit the files yourself to the level that they are usable. THe parsing code currently is tuned for the Moriond 2012 parameterizations from ttbar+mujet file

 Implementation:
     Loads a text file. Accessors return weights as a function of jet eta,pt for multiple options (central value, syst up, syst down, etc.)
*/
//
// Original Author:  "fblekman"
//         Created:  Fri Feb  1 12:10:50 CET 2013
// $Id: BTagWeightTools.cc,v 1.1.2.1 2013/02/01 17:17:59 fblekman Exp $
//
//

#include "../interface/BTagWeightTools.h"


#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
using namespace TopTree;


// main parsing code - 
void BTagWeightTools::parsefile(){
  ifstream file;
  file.open(_filename.c_str());
  if (!file){
    cout << "BTagWeightTools:: ERROR in openening file, currently set to " << _filename << endl;
    cout << "BTagWeightTools:: ERROR in opening file, do not expect any sensible results from b-tag scale factors! " << endl;
    return;
  }

  std::string lineread;

  bool startinglines=true;
  bool formula=false;
  bool parsingarray=false;
  bool findtagger=false;

  _etamax=2.4;// hardcoded
  _ptmin=100000;
  _ptmax=0;
  size_t p0=0; //workers
  size_t p1, p2; //workers
  while(std::getline(file, lineread)) // Read line by line
    {
      // and manipulate the lines to get what you want
      //      cout << lineread << std::endl;
     
      if(lineread.size()<1) // non-empty
	continue;
      //      cout << "non-empty " << endl;
      //      cout << lineread << std::endl;

      if(startinglines){
	//	cout << "parsing startinglines" << endl;
	p0=0;
	p1 = lineread.find_first_of("{",p0);
	p2 = lineread.find_first_of("}",p0);
	string rest1 = lineread.substr(p1+1,p2-p1-1);
	//	cout << rest1 << endl;
	std::getline(file, lineread); // expect this to be two lines!
	
	p1 = lineread.find_first_of("{",p0);
	p2 = lineread.find_first_of("}",p0);

	string rest2 = lineread.substr(p1+1,p2-p1-1);

	///	cout << rest2 << endl;
	// now start pushing these back and parsing them:
	p0=0;
	while(p0 != string::npos){
	  p1 = rest1.find_first_of(",",p0);
	  if(p1!=p0){
	    string number=rest1.substr(p0,p1-p0);
	    //	    cout <<"__" << number << "__" << endl;
	    float value = atof(number.c_str()); 
	    if(value<_ptmin)
	      _ptmin=value;
	    if(value>_ptmax)
	      _ptmax=value;
	    _ptrangesysts.push_back(value);
	  }
	  p0 = rest1.find_first_not_of(",", p1);
	}	
	p0=0;
	while(p0 != string::npos){
	  p1 = rest2.find_first_of(",",p0);
	  if(p1!=p0){
	    string number=rest2.substr(p0,p1-p0);
	    //	    cout <<"__" << number << "__" << endl;

	    float value = atof(number.c_str());
	    if(value<_ptmin)
	      _ptmin=value;
	    if(value>_ptmax)
	      _ptmax=value;
	    _ptrangesysthi.push_back(value);
	  }
	  p0 = rest2.find_first_not_of(",", p1);
	}
	startinglines = false;
      }
      else if(lineread.find_first_of("Tagger:",0) != string::npos && lineread.find_first_of("within",0) != string::npos){
	// found tagger
	p1 = lineread.find_first_of(": ",lineread.find_first_of("Tagger:",0));
	p1+=2; // compensate for size of ": "
	p2 = lineread.find_first_of(" ",p1);
	string taggername = lineread.substr(p1,p2-p1);
	//	cout << lineread << endl;

	// now read second line, which should have formula:
	std::getline(file, lineread);
	//	cout << lineread << endl;
	string formula = lineread.substr(lineread.find_first_of("=",0)+1,lineread.find_first_of(";",0)-1);
	TFormula tempformula(taggername.c_str(), formula.c_str());
	// logic is not working yet
// 	if(_defaultalgo.find("none")==string::npos || taggername.find(_defaultalgo)!=string::npos){
// 	    cout << "BTagWeightTools::INFO: ignoring tagger " << taggername << " as only parsing " << _defaultalgo << "information." << std::endl; 
// 	    cout << "BTagWeightTools::INFO: change behaviour by using the BTagWeightTools(file) constructor instead of the  BTagWeightTools(file,algo) constructor" << std::endl;

	
//	cout << "tagger name:" << taggername << endl;
//	cout << formula << std::endl;
//	cout << tempformula.Eval(50) << " " << tempformula.Eval(100) << endl;
	_functions[taggername]=tempformula;
	_weightsUp[taggername]=std::vector<float>(0);
	_weightsDown[taggername]=std::vector<float>(0);
	std::getline(file, lineread);// not interesting
	//	cout << lineread << endl;
	for(size_t ibin=0; ibin<_ptrangesysts.size(); ibin++){
	  std::getline(file, lineread);
	  string cleanstring = lineread.substr(lineread.find(" "),lineread.find(","));
	  cleanstring = cleanstring.substr(0,cleanstring.find("}"));
	  float value = atof(cleanstring.c_str());
	  //	  cout  << lineread << " ---> " << cleanstring << " ---> "  << value << endl;

	  _weightsUp[taggername].push_back(value);
	  _weightsDown[taggername].push_back(value*-1.);
	}
      }
    } 

  

  file.close();
  cout << "BTagWeightTools:INFO: successfully parsed file " << _filename <<", b-tag parameterizations now loaded!" << endl;
  return ;
}


// various getters:
float BTagWeightTools::getWeight(float pt, float eta, int flavor, int syst=0){

  return getWeight(pt,eta,flavor,_defaultalgo,syst);
}

float  BTagWeightTools::getWeight(float pt, float eta,int flavor,string algo,int syst=0){
  if(pt<_ptmin || pt>_ptmax)
    cout << "BTagWeightTools::WARNING retrieving for pT value (" << pt << ") outside range of " << _ptmin << ","<< _ptmax << " which is fine but tread with caution..." << endl;
  if(fabs(eta)>_etamax)
    cout << "BTagWeightTools::WARNING retrieving for eta value (" << eta << ") outside range of " << _etamax  << " which is fine but tread with caution..." << endl;
  if(syst==0){
    if(abs(flavor)==5 ||abs(flavor)==4)
      if(_functions.find(algo)!=_functions.end())
	return _functions[algo].Eval(pt);
  }
  else{
    // get the uncertainty:
    float err = getUncertainty(pt,eta, flavor, algo,syst);
    // and do the multiplication and check if above 1:
    if(abs(flavor)==5 ||abs(flavor)==4)
      if(_functions.find(algo)!=_functions.end()){
	float multiplyer = 1. + err;
	if(multiplyer*_functions[algo].Eval(pt)>1.0)
	  return 1.0;
	else
	  return multiplyer*_functions[algo].Eval(pt);
      }
  }
  
  cout << "BTagWeightTools:: WARNING retrieving unphysical BTV scale factor central value for algo: " << algo << ", jet(pt,eta)="<< pt << "," << eta << " with flavor " << flavor << endl;
  return -1;
}
float BTagWeightTools::getUncertainty(float pt, float eta, int flavor, int syst){
  return getUncertainty(pt,eta,flavor,_defaultalgo,syst);
}

float BTagWeightTools::getUncertainty(float pt, float eta, int flavor, string algo, int syst){
    // figure out if there is a multiplyer:
  if(syst==0)
    return 0;
  
  float multip=1.;
  if(abs(flavor)==4)// c quarks double uncertainty
    multip*=2;
  //cout << "multi = " << multip << endl;
  
  for(size_t ii=0; ii<_ptrangesysts.size() && ii<_ptrangesysthi.size(); ii++){
    //    cout << pt << " " << _ptrangesysts[ii] << " " << _ptrangesysthi[ii] << endl;
    if(pt<_ptrangesysts[ii])
      continue;
    if(pt>_ptrangesysthi[ii])
      continue;
    if(syst>0){
      if(_weightsUp.find(algo)!=_weightsUp.end()){
	//	cout << multip*_weightsUp[algo][ii] << endl;
	return multip*_weightsUp[algo][ii];
      }
    }
    else if(syst<0){
      if(_weightsDown.find(algo)!=_weightsDown.end()){
	//	cout << multip*_weightsDown[algo][ii] << endl;
	return multip*_weightsDown[algo][ii];
      }
    }
  }
 // if outside the pt range it still works, you just get larger errors
 
  if(pt<_ptmin){
    multip*=2;
    if(syst>0){
      if(_weightsUp.find(algo)!=_weightsUp.end())
	return multip*_weightsUp[algo][0];
    }
    else if(syst<0){
      if(_weightsDown.find(algo)!=_weightsDown.end())
	return multip*_weightsDown[algo][0];
    }
  }
  if(pt>_ptmax){
    multip*=2;
    size_t ii=_ptrangesysts.size()-1;
    if(syst>0){
     if(_weightsUp.find(algo)!=_weightsUp.end())
	return multip*_weightsUp[algo][ii];
    }
    else if(syst<0){
      if(_weightsDown.find(algo)!=_weightsDown.end())
	return multip*_weightsDown[algo][ii];
    }

  }
  else return 0;
}

// constructor with text file:
BTagWeightTools::BTagWeightTools(){
  _filename="";
  _defaultalgo="none";
  
}

BTagWeightTools::BTagWeightTools(string filename){
  _filename=filename;
  _defaultalgo="none";
  parsefile();


}

BTagWeightTools::BTagWeightTools(std::string filename, std::string defaultalgo){
  _defaultalgo=defaultalgo;
  _filename=filename;
  parsefile();
}

// destructor:

BTagWeightTools::~BTagWeightTools(){

  _functions.clear();
  _etarangesysts.clear();
  _etarangesystshi.clear();
  _ptrangesysts.clear();
  _ptrangesysthi.clear();
  _weightsUp.clear();
  _weightsDown.clear();
  
}
