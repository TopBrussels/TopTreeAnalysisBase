// -*- C++ -*-
//
// Package:    TopTreeAnalysisBase/Tools
// Class:      BTagWeightTools
// 
/**\class BTagWeightTools TopBrussels/TopTreeAnalysisBase/Tools/interface/BTagWeightTools.cxx TopBrussels/Tools/interface/BTagWeightTools.h

 Description: Container class to parse BTV POG payloads which can be updated from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG

Unfortunately the BTV POG does not give the parameterizations in a parsable format that is consistent even per measurements, so you need to edit the files yourself to the level that they are usable. THe parsing code currently is tuned for the Moriond 2012 parameterizations from ttbar+mujet file


Fake rates: Now also using the Fake Rate parameterizations that are documented (for Moriond 2013) here: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_Moriond2013.C or (for EPS 2013) here: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_EPS2013.C
As this is a bare Root Macro this has been c++ized into a separate set of functions.

 Implementation:
     Efficiencies: Loads a text file. Accessors return weights as a function of jet eta,pt for multiple options (central value, syst up, syst down, etc.)
     Fake rates: Contains hard-coded functions that are supplied in example code by BTV group
*/
//
// Original Author:  "fblekman"
//         Created:  Fri Feb  1 12:10:50 CET 2013
// $Id: BTagWeightTools.cc,v 1.1.2.7 2013/02/21 10:46:28 fblekman Exp $
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

  _etamax=2.5;// hardcoded
  _ptmin=100000;
  _ptmax=0;
  size_t p0=0; //workers
  size_t p1, p2; //workers
  while(std::getline(file, lineread)) // Read line by line
    {
      // and manipulate the lines to get what you want
//            cout << lineread << std::endl;
     
      if(lineread.size()<1) // non-empty
	continue;
//            cout << "non-empty " << endl;
//            cout << lineread << std::endl;

      if(startinglines){
		cout << "parsing startinglines" << endl;
	p0=0;
	p1 = lineread.find_first_of("{",p0);
	p2 = lineread.find_first_of("}",p0);
	string rest1 = lineread.substr(p1+1,p2-p1-1);
//		cout << rest1 << endl;
	std::getline(file, lineread); // expect this to be two lines!
	
	p1 = lineread.find_first_of("{",p0);
	p2 = lineread.find_first_of("}",p0);

	string rest2 = lineread.substr(p1+1,p2-p1-1);

//		cout << rest2 << endl;
	// now start pushing these back and parsing them:
	p0=0;
	while(p0 != string::npos){
	  p1 = rest1.find_first_of(",",p0);
	  if(p1!=p0){
	    string number=rest1.substr(p0,p1-p0);
//	    	    cout <<"__" << number << "__" << endl;
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
//	    	    cout <<"__" << number << "__" << endl;

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
//		cout << lineread << endl;
//		cout<<" _ptrangesysts.size() = "<<_ptrangesysts.size()<<endl;
	for(size_t ibin=0; ibin<_ptrangesysts.size(); ibin++){
//	cout<<"   ibin = "<<ibin<<endl;
	  std::getline(file, lineread);
	  string cleanstring = lineread.substr(lineread.find(" "),lineread.find(","));   //NOTE: there should be a fix in the CSVSL case, since this contains less uncertainty bins than the other taggers.
	  cleanstring = cleanstring.substr(0,cleanstring.find("}"));
	  float value = atof(cleanstring.c_str());
//	  	  cout  << lineread << " ---> " << cleanstring << " ---> "  << value << endl;

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
float BTagWeightTools::getSF(float pt, float eta, int flavor, int syst=0){

  return getSF(pt,eta,flavor,_defaultalgo,syst);
}

float  BTagWeightTools::getSF(float pt, float eta,int flavor,string algo,int syst=0){
  if(pt<_ptmin || pt>_ptmax)
    cout << "BTagWeightTools::getSF WARNING retrieving for pT value (" << pt << ") outside range of " << _ptmin << ","<< _ptmax << " which is fine but tread with caution..." << endl;
  if(fabs(eta)>_etamax)
    cout << "BTagWeightTools::getSF WARNING retrieving for eta value (" << eta << ") outside range of " << _etamax  << " which is fine but tread with caution..." << endl;
 
  if(abs(flavor)==5 ||abs(flavor)==4){
    if(syst==0){
      if(_functions.find(algo)!=_functions.end())
	return _functions[algo].Eval(pt);
    }
    else{
    // get the uncertainty:
      float err = getUncertainty(pt,eta, flavor, algo,syst);
      // and do the multiplication and check if above 1:
      if(_functions.find(algo)!=_functions.end()){
	float multiplyer = 1. + err;
	return multiplyer*_functions[algo].Eval(pt);
      }
    }
  } 
  else {
   return getSFlight(pt,eta,algo,syst);
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
      if(_weightsUp.find(algo)!=_weightsUp.end()){
	return multip*_weightsUp[algo][ii];
      }
    }
    else if(syst<0){
      if(_weightsDown.find(algo)!=_weightsDown.end()){
	return multip*_weightsDown[algo][ii];
      }
    }
  }
  
  return 0;
}

// constructor with text file:
BTagWeightTools::BTagWeightTools(){
  _filename="";
  _defaultalgo="none";
  _abcdrange="ABCD";
	_sflightprescription="EPS2013";
	MCEfficiencyHistosInitialized = false;
}

BTagWeightTools::BTagWeightTools(string filename){
  _filename=filename;
  _defaultalgo="none";
  _abcdrange="ABCD";
	_sflightprescription="EPS2013";
	MCEfficiencyHistosInitialized = false;
  parsefile();

}

BTagWeightTools::BTagWeightTools(std::string filename, std::string defaultalgo){
  _defaultalgo=defaultalgo;
  _filename=filename;
  _abcdrange="ABCD";
	_sflightprescription="EPS2013";
	setalgoWPcut(defaultalgo);
	MCEfficiencyHistosInitialized = false;	
  parsefile();

}

BTagWeightTools::BTagWeightTools(std::string filename, std::string defaultalgo, std::string sflightprescription){
  _defaultalgo=defaultalgo;
  _filename=filename;
  _abcdrange="ABCD";
	_sflightprescription=sflightprescription;
  setalgoWPcut(defaultalgo);
	parsefile();

}

BTagWeightTools::BTagWeightTools(std::string filename, std::string algo_L, std::string algo_T, std::string sflightprescription){
  _algo_L=algo_L;
	_algo_T=algo_T;
  _filename=filename;
  _abcdrange="ABCD";
	_sflightprescription=sflightprescription;
  setalgoWPcut(algo_L,algo_T);
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

  _fakeratesfunctions.clear();
  
}

void BTagWeightTools::setalgoWPcut(std::string algo)
{
	//working points from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
	if(algo=="CSVL")
	  _algoWPcut = 0.244;
  else if(algo=="CSVM")
	  _algoWPcut = 0.679;
  else if(algo=="CSVT")
	  _algoWPcut = 0.898;
	else if(algo=="CSVV1L")
	  _algoWPcut = 0.405;
  else if(algo=="CSVV1M")
	  _algoWPcut = 0.783;
	else if(algo=="CSVV1T")
	  _algoWPcut = 0.920;
  else if(algo=="CSVSLV1L")
	  _algoWPcut = 0.527;
	else if(algo=="CSVSLV1M")
	  _algoWPcut = 0.756; 
	else if(algo=="CSVSLV1T")
	  _algoWPcut = 0.859;
	else if(algo=="TCHPT")
	  _algoWPcut = 3.41;
	else if(algo=="JPL")
	  _algoWPcut = 0.275;
	else if(algo=="JPM")
	  _algoWPcut =	0.545;
	else if(algo=="JPT")
	  _algoWPcut = 0.790;
	else std::cout<<" BTagWeightTools::BTagWeightTools WARNING: Working Point for algorithm not found!"<<std::endl;
		
}

void BTagWeightTools::setalgoWPcut(std::string algo_L, std::string algo_T)
{
	//working points from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
	if(algo_L=="CSVL")
	  _algoWPcut_L = 0.244;
  else if(algo_L=="CSVM")
	  _algoWPcut_L = 0.679;
  else if(algo_L=="CSVT")
	  _algoWPcut_L = 0.898;
	else if(algo_L=="CSVV1L")
	  _algoWPcut_L = 0.405;
  else if(algo_L=="CSVV1M")
	  _algoWPcut_L = 0.783;
	else if(algo_L=="CSVV1T")
	  _algoWPcut_L = 0.920;
  else if(algo_L=="CSVSLV1L")
	  _algoWPcut_L = 0.527;
	else if(algo_L=="CSVSLV1M")
	  _algoWPcut_L = 0.756; 
	else if(algo_L=="CSVSLV1T")
	  _algoWPcut_L = 0.859;
	else if(algo_L=="TCHPT")
	  _algoWPcut_L = 3.41;
	else if(algo_L=="JPL")
	  _algoWPcut_L = 0.275;
	else if(algo_L=="JPM")
	  _algoWPcut_L =	0.545;
	else if(algo_L=="JPT")
	  _algoWPcut_L = 0.790;
	else std::cout<<" BTagWeightTools::BTagWeightTools WARNING: Working Point for algorithm not found!"<<std::endl;
	
	if(algo_T=="CSVL")
	  _algoWPcut_T = 0.244;
  else if(algo_T=="CSVM")
	  _algoWPcut_T = 0.679;
  else if(algo_T=="CSVT")
	  _algoWPcut_T = 0.898;
	else if(algo_T=="CSVV1L")
	  _algoWPcut_T = 0.405;
  else if(algo_T=="CSVV1M")
	  _algoWPcut_T = 0.783;
	else if(algo_T=="CSVV1T")
	  _algoWPcut_T = 0.920;
  else if(algo_T=="CSVSLV1L")
	  _algoWPcut_T = 0.527;
	else if(algo_T=="CSVSLV1M")
	  _algoWPcut_T = 0.756; 
	else if(algo_T=="CSVSLV1T")
	  _algoWPcut_T = 0.859;
	else if(algo_T=="TCHPT")
	  _algoWPcut_T = 3.41;
	else if(algo_T=="JPL")
	  _algoWPcut_T = 0.275;
	else if(algo_T=="JPM")
	  _algoWPcut_T =	0.545;
	else if(algo_T=="JPT")
	  _algoWPcut_T = 0.790;
	else std::cout<<" BTagWeightTools::BTagWeightTools WARNING: Working Point for algorithm not found!"<<std::endl;	
		
}

float BTagWeightTools::getSFlight(float pt, float eta, string algo, int syst){
  std::pair<float,float> etarange = getfakeraterange((TString)algo,eta);
  
  TString meanminmax="mean";
  if(syst<0)
    meanminmax="min";
  else if(syst>0)
    meanminmax="max";
  TString tagger = algo;
  TString taggerstrength="";
  TString workname = makefakeratename(meanminmax,tagger,taggerstrength,etarange.first,etarange.second,_abcdrange,_sflightprescription); // default is to run over ABCD range, change via setABCDRangeFakeRates
  if(_fakeratesfunctions.find(workname)==_fakeratesfunctions.end()){
        //   cout <<"meanminmax "<< meanminmax <<"  tagger "<< tagger <<  taggerstrength << "  etarangef  "<< etarange.first << " etarange s  "<<etarange.second  << "  _abcdrange  "<< _abcdrange << "  _sflightprescription  "<< _sflightprescription <<endl;
      
    TF1 *newfunc = fillfakerates(meanminmax,tagger,taggerstrength,etarange.first,etarange.second,_abcdrange,_sflightprescription);
      newfunc->SetName(workname);
    _fakeratesfunctions[workname]=newfunc;

  }
//    cout <<" fake rate func "<< _fakeratesfunctions[workname]->GetExpFormula("")  << endl;
    
  return _fakeratesfunctions[workname]->Eval(pt);
}

TString BTagWeightTools::makefakeratename(TString meanminmax, TString tagger, TString TaggerStrength, Float_t Etamin, Float_t Etamax, TString DataPeriod, TString Prescription){
  TString result="";
  result+=meanminmax;
  result+="_";
  result+=tagger;
  result+="_";
  result+=TaggerStrength;
  result+="_";
  result+=Etamin;
  result+="-";
  result+=Etamax;
  result+="_";
  result+=DataPeriod;
	result+="_";
  result+=Prescription;

  return result;
}

std::pair<float,float> BTagWeightTools::getfakeraterange(TString tagger,float eta){

  std::pair<float,float> result;
  // default range, used for the tight taggers incl TCHPT
  result.first=0.;
  result.second=2.4;
  
  if(tagger=="CSVL" || tagger=="JPL" || tagger=="CSVV1L" || tagger=="CSVSLV1L"){
    if(fabs(eta)<0.5){
      result.first=0.;
      result.second=0.5;
      return result;
    }
    else if(fabs(eta)<1.0){
      result.first=0.5;
      result.second=1.0;
      return result;
    }
    else if(fabs(eta)<1.5){
      result.first=1.0;
      result.second=1.5;
      return result;
    }
    else if(fabs(eta)<=2.4){
      result.first=1.5;
      result.second=2.4;
      return result;
    }
  }
  else if(tagger=="CSVM" || tagger=="JPM" || tagger=="CSVV1M" || tagger=="CSVSLV1M"){
   if(fabs(eta)<0.8){
      result.first=0.;
      result.second=0.8;
      return result;
    }
    else if(fabs(eta)<1.6){
      result.first=0.8;
      result.second=1.6;
      return result;
    }
    else if(fabs(eta)<=2.4){
      result.first=1.6;
      result.second=2.4;
      return result;
    }
		else cout<<"BTagWeightTools::getfakeraterange WARNING: jet eta = "<<eta<<endl;
  }
  return result;
}

//// monster function copied from BTV group:


TF1* BTagWeightTools::fillfakerates(TString meanminmax, TString tagger, TString TaggerStrength, Float_t Etamin, Float_t Etamax, TString DataPeriod, TString Prescription){
 TF1 *tmpSFl = NULL;
  TString Atagger = tagger+TaggerStrength;
  TString sEtamin = Form("%1.1f",Etamin);
  TString sEtamax = Form("%1.1f",Etamax);
 // cout << sEtamin << endl;
  //cout << sEtamax << endl;

 if(Prescription == "Moriond2013")
 { 
  if (DataPeriod=="ABCD") {

    // Begin of definition of functions from SF_12ABCD ---------------

    Double_t ptmax;
    if( sEtamin == "1.5" || sEtamin == "1.6" ) ptmax = 700;
    else ptmax = 800;
    
    if( Atagger == "CSVL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.04901+(0.00152181*x))+(-3.43568e-06*(x*x)))+(2.17219e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.973773+(0.00103049*x))+(-2.2277e-06*(x*x)))+(1.37208e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.12424+(0.00201136*x))+(-4.64021e-06*(x*x)))+(2.97219e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.991915+(0.00172552*x))+(-3.92652e-06*(x*x)))+(2.56816e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.921518+(0.00129098*x))+(-2.86488e-06*(x*x)))+(1.86022e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.06231+(0.00215815*x))+(-4.9844e-06*(x*x)))+(3.27623e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.962127+(0.00192796*x))+(-4.53385e-06*(x*x)))+(3.0605e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.895419+(0.00153387*x))+(-3.48409e-06*(x*x)))+(2.30899e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.02883+(0.00231985*x))+(-5.57924e-06*(x*x)))+(3.81235e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.06121+(0.000332747*x))+(-8.81201e-07*(x*x)))+(7.43896e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.983607+(0.000196747*x))+(-3.98327e-07*(x*x)))+(2.95764e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.1388+(0.000468418*x))+(-1.36341e-06*(x*x)))+(1.19256e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.06238+(0.00198635*x))+(-4.89082e-06*(x*x)))+(3.29312e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.972746+(0.00104424*x))+(-2.36081e-06*(x*x)))+(1.53438e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.15201+(0.00292575*x))+(-7.41497e-06*(x*x)))+(5.0512e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.08048+(0.00110831*x))+(-2.96189e-06*(x*x)))+(2.16266e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.9836+(0.000649761*x))+(-1.59773e-06*(x*x)))+(1.14324e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.17735+(0.00156533*x))+(-4.32257e-06*(x*x)))+(3.18197e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.09145+(0.000687171*x))+(-2.45054e-06*(x*x)))+(1.7844e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.00616+(0.000358884*x))+(-1.23768e-06*(x*x)))+(6.86678e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.17671+(0.0010147*x))+(-3.66269e-06*(x*x)))+(2.88425e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01739+(0.00283619*x))+(-7.93013e-06*(x*x)))+(5.97491e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.953587+(0.00124872*x))+(-3.97277e-06*(x*x)))+(3.23466e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.08119+(0.00441909*x))+(-1.18764e-05*(x*x)))+(8.71372e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.05617+(0.000986016*x))+(-2.05398e-06*(x*x)))+(1.25408e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.918762+(0.000749113*x))+(-1.48511e-06*(x*x)))+(8.78559e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.19358+(0.00122182*x))+(-2.62078e-06*(x*x)))+(1.62951e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.02884+(0.000471854*x))+(-1.15441e-06*(x*x)))+(7.83716e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.893017+(0.000369124*x))+(-8.68577e-07*(x*x)))+(5.79006e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.16466+(0.000573985*x))+(-1.43899e-06*(x*x)))+(9.88387e-10*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.02463+(0.000907924*x))+(-2.07133e-06*(x*x)))+(1.37083e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.89415+(0.000712877*x))+(-1.57703e-06*(x*x)))+(1.02034e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.15511+(0.00110197*x))+(-2.56374e-06*(x*x)))+(1.72152e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.05387+(0.000951237*x))+(-2.35437e-06*(x*x)))+(1.66123e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.918611+(0.000781707*x))+(-1.8923e-06*(x*x)))+(1.312e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.1891+(0.00112006*x))+(-2.81586e-06*(x*x)))+(2.01249e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.980407+(0.00190765*x))+(-4.49633e-06*(x*x)))+(3.02664e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.813164+(0.00127951*x))+(-2.74274e-06*(x*x)))+(1.78799e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.14766+(0.00253327*x))+(-6.24447e-06*(x*x)))+(4.26468e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01783+(0.00183763*x))+(-4.64972e-06*(x*x)))+(3.34342e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.860873+(0.00110031*x))+(-2.48023e-06*(x*x)))+(1.73776e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.17479+(0.00257252*x))+(-6.81377e-06*(x*x)))+(4.94891e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.866685+(0.00396887*x))+(-1.11342e-05*(x*x)))+(8.84085e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.740983+(0.00302736*x))+(-8.12284e-06*(x*x)))+(6.281e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((0.992297+(0.00490671*x))+(-1.41403e-05*(x*x)))+(1.14097e-08*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.89627+(0.00328988*x))+(-8.76392e-06*(x*x)))+(6.4662e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.666092+(0.00262465*x))+(-6.5345e-06*(x*x)))+(4.73926e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.12648+(0.00394995*x))+(-1.0981e-05*(x*x)))+(8.19134e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "TCHPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.1676+(0.00136673*x))+(-3.51053e-06*(x*x)))+(2.4966e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.988346+(0.000914722*x))+(-2.37077e-06*(x*x)))+(1.72082e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.34691+(0.00181637*x))+(-4.64484e-06*(x*x)))+(3.27122e-09*(x*(x*x)))", 20.,ptmax);
      }

    // End of definition of functions from SF_12ABCD ---------------

  } else if (DataPeriod=="AB") { 

    // Begin of definition of functions from SF_12AB ---------------

    Double_t ptmax;
    if( sEtamin == "1.5" || sEtamin == "1.6" ) ptmax = 700;
    else ptmax = 800;

    if( Atagger == "CSVL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.00989+(0.00155686*x))+(-3.72647e-06*(x*x)))+(2.47025e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.947488+(0.00105091*x))+(-2.43972e-06*(x*x)))+(1.58902e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.07229+(0.00206098*x))+(-5.00971e-06*(x*x)))+(3.35179e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.958598+(0.00173458*x))+(-4.12744e-06*(x*x)))+(2.83257e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.900024+(0.00129392*x))+(-3.01708e-06*(x*x)))+(2.06723e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.01716+(0.00217335*x))+(-5.23419e-06*(x*x)))+(3.5986e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.963113+(0.00163674*x))+(-3.84776e-06*(x*x)))+(2.56918e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.90596+(0.00125465*x))+(-2.78863e-06*(x*x)))+(1.78602e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.02025+(0.0020171*x))+(-4.90389e-06*(x*x)))+(3.35329e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.04996+(0.00031979*x))+(-8.43322e-07*(x*x)))+(6.9451e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.983472+(0.000169396*x))+(-2.82848e-07*(x*x)))+(1.52744e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11645+(0.000469873*x))+(-1.40321e-06*(x*x)))+(1.23681e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.02213+(0.00189078*x))+(-4.59419e-06*(x*x)))+(3.0366e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.946+(0.000940317*x))+(-1.99048e-06*(x*x)))+(1.18343e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.09827+(0.00283897*x))+(-7.19354e-06*(x*x)))+(4.89013e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.0596+(0.00102926*x))+(-2.70312e-06*(x*x)))+(1.82871e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.974966+(0.000545735*x))+(-1.23123e-06*(x*x)))+(7.05661e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.14423+(0.00151156*x))+(-4.17277e-06*(x*x)))+(2.95233e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.04976+(0.000897158*x))+(-3.22829e-06*(x*x)))+(2.71316e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.977166+(0.000550586*x))+(-1.91114e-06*(x*x)))+(1.44817e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.12232+(0.00124269*x))+(-4.54368e-06*(x*x)))+(3.98079e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVT" && sEtamin == "0.0" && sEtamax == "2.4")
    {
        if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.985589+(0.00302526*x))+(-8.73861e-06*(x*x)))+(6.65051e-09*(x*(x*x)))", 20.,ptmax);
        if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.93612+(0.00131596*x))+(-4.30052e-06*(x*x)))+(3.45957e-09*(x*(x*x)))", 20.,ptmax);
        if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.03505+(0.00472994*x))+(-1.31661e-05*(x*x)))+(9.84151e-09*(x*(x*x)))", 20.,ptmax);
    }
    if( Atagger == "JPL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01004+(0.000693171*x))+(-1.71673e-06*(x*x)))+(1.13601e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.920801+(0.00048556*x))+(-1.14573e-06*(x*x)))+(7.29722e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.09929+(0.000899912*x))+(-2.28605e-06*(x*x)))+(1.54241e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.983323+(0.00021632*x))+(-8.21701e-07*(x*x)))+(6.67398e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.893817+(0.000139244*x))+(-5.53288e-07*(x*x)))+(4.54312e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.07283+(0.000292983*x))+(-1.08908e-06*(x*x)))+(8.80497e-10*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.00787+(0.00067391*x))+(-1.85829e-06*(x*x)))+(1.42239e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.922717+(0.000501621*x))+(-1.3493e-06*(x*x)))+(1.02068e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.09302+(0.000845356*x))+(-2.36546e-06*(x*x)))+(1.82448e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.03546+(0.000774019*x))+(-2.15928e-06*(x*x)))+(1.6934e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.946412+(0.000642931*x))+(-1.74696e-06*(x*x)))+(1.34402e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.12449+(0.000904468*x))+(-2.57084e-06*(x*x)))+(2.04473e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.95344+(0.00171952*x))+(-4.71763e-06*(x*x)))+(3.41607e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.821103+(0.0011014*x))+(-2.81576e-06*(x*x)))+(2.00088e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.08578+(0.00233518*x))+(-6.61409e-06*(x*x)))+(4.83128e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.994557+(0.00176506*x))+(-4.95785e-06*(x*x)))+(3.63594e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.874792+(0.000997253*x))+(-2.51511e-06*(x*x)))+(1.75184e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11431+(0.0025305*x))+(-7.39562e-06*(x*x)))+(5.52077e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.850708+(0.00373619*x))+(-1.10196e-05*(x*x)))+(9.0243e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.756769+(0.0028841*x))+(-8.02579e-06*(x*x)))+(6.29964e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((0.944553+(0.00458472*x))+(-1.40078e-05*(x*x)))+(1.1758e-08*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.869428+(0.00338068*x))+(-9.51813e-06*(x*x)))+(7.08382e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.657456+(0.00279437*x))+(-7.29415e-06*(x*x)))+(5.28578e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.08141+(0.00396192*x))+(-1.17308e-05*(x*x)))+(8.88194e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "TCHPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.10796+(0.00168368*x))+(-4.50964e-06*(x*x)))+(3.21561e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.948977+(0.0011449*x))+(-3.05912e-06*(x*x)))+(2.17813e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.26699+(0.00221974*x))+(-5.95423e-06*(x*x)))+(4.25254e-09*(x*(x*x)))", 20.,ptmax);
      }
    
    // End of definition of functions from SF_12AB ---------------
    
  } else if (DataPeriod=="C") { 

    // Begin of definition of functions from SF_12C ---------------
    
    Double_t ptmax;
    if( sEtamin == "1.5" || sEtamin == "1.6" ) ptmax = 700;
    else ptmax = 800;
    
    if( Atagger == "CSVL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.03512+(0.00172098*x))+(-4.10286e-06*(x*x)))+(2.72413e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.971321+(0.00117532*x))+(-2.71334e-06*(x*x)))+(1.77294e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.0989+(0.0022646*x))+(-5.48834e-06*(x*x)))+(3.67551e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.977454+(0.00186222*x))+(-4.30874e-06*(x*x)))+(2.82227e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.917942+(0.00139264*x))+(-3.13422e-06*(x*x)))+(2.02475e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.03695+(0.00232982*x))+(-5.47968e-06*(x*x)))+(3.62048e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.940154+(0.00214045*x))+(-5.30206e-06*(x*x)))+(3.75872e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.885078+(0.00170468*x))+(-4.08896e-06*(x*x)))+(2.85628e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((0.995215+(0.00257376*x))+(-6.5103e-06*(x*x)))+(4.66211e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.04882+(0.000373418*x))+(-1.00316e-06*(x*x)))+(8.52325e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.982642+(0.000211816*x))+(-4.11471e-07*(x*x)))+(2.88443e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11499+(0.000534645*x))+(-1.59409e-06*(x*x)))+(1.41682e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.0444+(0.00216756*x))+(-5.4224e-06*(x*x)))+(3.69351e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.966203+(0.00112979*x))+(-2.56147e-06*(x*x)))+(1.65183e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.1226+(0.00320252*x))+(-8.27754e-06*(x*x)))+(5.73519e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.05203+(0.00138588*x))+(-3.97677e-06*(x*x)))+(3.13655e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.966774+(0.000855535*x))+(-2.33883e-06*(x*x)))+(1.86063e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.13729+(0.00191432*x))+(-5.61018e-06*(x*x)))+(4.41282e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.06547+(0.000850114*x))+(-2.76694e-06*(x*x)))+(1.75015e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.992673+(0.000455214*x))+(-1.29572e-06*(x*x)))+(3.89704e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.13823+(0.00124422*x))+(-4.23813e-06*(x*x)))+(3.11339e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.00197+(0.00266395*x))+(-6.95018e-06*(x*x)))+(4.91042e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.948887+(0.00103466*x))+(-2.88118e-06*(x*x)))+(2.07782e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.05505+(0.00428961*x))+(-1.10115e-05*(x*x)))+(7.74319e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.04676+(0.00112324*x))+(-2.52493e-06*(x*x)))+(1.65931e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.954301+(0.000859775*x))+(-1.83391e-06*(x*x)))+(1.17383e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.13922+(0.00138539*x))+(-3.21336e-06*(x*x)))+(2.14483e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01822+(0.000554752*x))+(-1.44083e-06*(x*x)))+(9.9442e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.925322+(0.00044089*x))+(-1.09668e-06*(x*x)))+(7.37906e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11112+(0.000667913*x))+(-1.78357e-06*(x*x)))+(1.25108e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.00672+(0.000952424*x))+(-2.24525e-06*(x*x)))+(1.49885e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.922111+(0.000746635*x))+(-1.68093e-06*(x*x)))+(1.07795e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.09131+(0.00115721*x))+(-2.80782e-06*(x*x)))+(1.92031e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.0346+(0.00102282*x))+(-2.61072e-06*(x*x)))+(1.91999e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.946009+(0.00085601*x))+(-2.11306e-06*(x*x)))+(1.5133e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.12316+(0.00118887*x))+(-3.10773e-06*(x*x)))+(2.32911e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.976097+(0.00191673*x))+(-4.58557e-06*(x*x)))+(3.10331e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.838655+(0.00127127*x))+(-2.69532e-06*(x*x)))+(1.73384e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11354+(0.00255978*x))+(-6.47089e-06*(x*x)))+(4.47278e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01871+(0.00173675*x))+(-4.58934e-06*(x*x)))+(3.38512e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.896752+(0.000935708*x))+(-2.09673e-06*(x*x)))+(1.47779e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.14066+(0.00253557*x))+(-7.07715e-06*(x*x)))+(5.29295e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.857254+(0.00402389*x))+(-1.15649e-05*(x*x)))+(9.37845e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.766329+(0.00306103*x))+(-8.27326e-06*(x*x)))+(6.43552e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((0.948081+(0.00498302*x))+(-1.48512e-05*(x*x)))+(1.23315e-08*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.911203+(0.00300921*x))+(-8.03854e-06*(x*x)))+(5.97264e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.698853+(0.00240605*x))+(-5.86773e-06*(x*x)))+(4.23559e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.12356+(0.00360804*x))+(-1.01996e-05*(x*x)))+(7.70949e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "TCHPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.1393+(0.00148115*x))+(-3.72335e-06*(x*x)))+(2.6087e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.977185+(0.000957694*x))+(-2.36635e-06*(x*x)))+(1.65373e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.30146+(0.00200233*x))+(-5.07544e-06*(x*x)))+(3.56314e-09*(x*(x*x)))", 20.,ptmax);
      }
    
    // End of definition of functions from SF_12C ---------------
    
  } else if (DataPeriod=="D") { 
    
    // Begin of definition of functions from SF_12D ---------------

    Double_t ptmax;
    if( sEtamin == "1.5" || sEtamin == "1.6" ) ptmax = 700;
    else ptmax = 800;
    
    if( Atagger == "CSVL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.1121+(0.00156291*x))+(-3.72267e-06*(x*x)))+(2.54276e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.04345+(0.00100049*x))+(-2.27285e-06*(x*x)))+(1.53238e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.18074+(0.00212352*x))+(-5.16888e-06*(x*x)))+(3.55347e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.05107+(0.0018085*x))+(-4.42378e-06*(x*x)))+(3.12722e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.986937+(0.00132072*x))+(-3.17261e-06*(x*x)))+(2.25152e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11519+(0.00229425*x))+(-5.67093e-06*(x*x)))+(4.00366e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.984747+(0.00233796*x))+(-5.84283e-06*(x*x)))+(4.21798e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.927306+(0.00186598*x))+(-4.5141e-06*(x*x)))+(3.21483e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.04217+(0.0028073*x))+(-7.16639e-06*(x*x)))+(5.2225e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.0944+(0.000394694*x))+(-1.31095e-06*(x*x)))+(1.29556e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.0255+(0.000220197*x))+(-6.45505e-07*(x*x)))+(6.40579e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.1633+(0.000568652*x))+(-1.97487e-06*(x*x)))+(1.95111e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.13626+(0.00209868*x))+(-5.54303e-06*(x*x)))+(3.9911e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.05089+(0.00100001*x))+(-2.44384e-06*(x*x)))+(1.72918e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.22164+(0.00319447*x))+(-8.63596e-06*(x*x)))+(6.25306e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.13291+(0.00128329*x))+(-3.79952e-06*(x*x)))+(3.03032e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.04112+(0.000728221*x))+(-2.04996e-06*(x*x)))+(1.64537e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.22469+(0.0018366*x))+(-5.54498e-06*(x*x)))+(4.4159e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.18705+(0.000305854*x))+(-1.86925e-06*(x*x)))+(1.79183e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.10587+(-8.23503e-05*x))+(-3.06139e-07*(x*x)))+(2.38667e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.26821+(0.000693404*x))+(-3.43071e-06*(x*x)))+(3.34622e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "CSVT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.08603+(0.0027994*x))+(-8.44182e-06*(x*x)))+(6.847e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.02837+(0.00104078*x))+(-3.81136e-06*(x*x)))+(3.43028e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.14368+(0.00455363*x))+(-1.30615e-05*(x*x)))+(1.0264e-08*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "0.0" && sEtamax == "0.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.12238+(0.00152486*x))+(-3.2873e-06*(x*x)))+(2.17918e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.02366+(0.0012007*x))+(-2.45347e-06*(x*x)))+(1.58906e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.22108+(0.00184736*x))+(-4.11792e-06*(x*x)))+(2.76952e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "0.5" && sEtamax == "1.0")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.10302+(0.000874045*x))+(-1.99863e-06*(x*x)))+(1.39584e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.00257+(0.000719596*x))+(-1.5641e-06*(x*x)))+(1.07029e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.20346+(0.00102756*x))+(-2.43131e-06*(x*x)))+(1.72172e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.0" && sEtamax == "1.5")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.06244+(0.00149626*x))+(-3.55121e-06*(x*x)))+(2.51004e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.973582+(0.00122407*x))+(-2.81096e-06*(x*x)))+(1.94803e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.15128+(0.0017669*x))+(-4.28856e-06*(x*x)))+(3.07303e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPL" && sEtamin == "1.5" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.10432+(0.00129851*x))+(-3.26353e-06*(x*x)))+(2.32516e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.00996+(0.00109673*x))+(-2.66726e-06*(x*x)))+(1.84117e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.19864+(0.00149937*x))+(-3.85934e-06*(x*x)))+(2.81243e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.0" && sEtamax == "0.8")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.0075+(0.00257791*x))+(-5.91599e-06*(x*x)))+(4.142e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.867433+(0.00177435*x))+(-3.62606e-06*(x*x)))+(2.46206e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.14757+(0.00337847*x))+(-8.19968e-06*(x*x)))+(5.82223e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "0.8" && sEtamax == "1.6")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.02335+(0.00274819*x))+(-7.08829e-06*(x*x)))+(5.44469e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.903546+(0.0017276*x))+(-3.96021e-06*(x*x)))+(3.00594e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.14313+(0.00376554*x))+(-1.02094e-05*(x*x)))+(7.88496e-09*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPM" && sEtamin == "1.6" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.898963+(0.00428943*x))+(-1.12357e-05*(x*x)))+(8.35894e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.804719+(0.00322419*x))+(-7.66523e-06*(x*x)))+(5.18187e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((0.993109+(0.00535115*x))+(-1.48031e-05*(x*x)))+(1.15468e-08*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "JPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.887609+(0.00411151*x))+(-1.10861e-05*(x*x)))+(8.50678e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.681054+(0.00325624*x))+(-8.17194e-06*(x*x)))+(6.14789e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.09416+(0.00496121*x))+(-1.39877e-05*(x*x)))+(1.08667e-08*(x*(x*x)))", 20.,ptmax);
      }
    if( Atagger == "TCHPT" && sEtamin == "0.0" && sEtamax == "2.4")
      {
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.25209+(0.00136201*x))+(-3.90275e-06*(x*x)))+(3.1283e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.07448+(0.000816207*x))+(-2.38176e-06*(x*x)))+(1.97453e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.42974+(0.00190546*x))+(-5.41793e-06*(x*x)))+(4.28148e-09*(x*(x*x)))", 20.,ptmax);
      }

    // End of definition of functions from SF_12D ---------------
    
  } 
  
 }   //end "Moriond2013" prescription
 else if(Prescription == "EPS2013")
 { 
	  if( (TaggerStrength == "L" || TaggerStrength == "M") && sEtamin == "0.0" && sEtamax == "2.4" ) //to be adapted: taggerstrength currently not really used in class...
  	{
    	cout << "For L and M taggers, the function is not provided integrated over eta. Only eta subranges are provided " << endl;
    	return tmpSFl;
  	}

  	Double_t ptmax;
  	if( sEtamin == "1.5" || sEtamin == "1.6" ) ptmax = 850;
  	else ptmax = 1000;

	// Insert function def below here =====================================

	if( Atagger == "CSVL" && sEtamin == "0.0" && sEtamax == "0.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01177+(0.0023066*x))+(-4.56052e-06*(x*x)))+(2.57917e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.977761+(0.00170704*x))+(-3.2197e-06*(x*x)))+(1.78139e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.04582+(0.00290226*x))+(-5.89124e-06*(x*x)))+(3.37128e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVL" && sEtamin == "0.5" && sEtamax == "1.0")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.975966+(0.00196354*x))+(-3.83768e-06*(x*x)))+(2.17466e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.945135+(0.00146006*x))+(-2.70048e-06*(x*x)))+(1.4883e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.00683+(0.00246404*x))+(-4.96729e-06*(x*x)))+(2.85697e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVL" && sEtamin == "1.0" && sEtamax == "1.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.93821+(0.00180935*x))+(-3.86937e-06*(x*x)))+(2.43222e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.911657+(0.00142008*x))+(-2.87569e-06*(x*x)))+(1.76619e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((0.964787+(0.00219574*x))+(-4.85552e-06*(x*x)))+(3.09457e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVL" && sEtamin == "1.5" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.00022+(0.0010998*x))+(-3.10672e-06*(x*x)))+(2.35006e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.970045+(0.000862284*x))+(-2.31714e-06*(x*x)))+(1.68866e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.03039+(0.0013358*x))+(-3.89284e-06*(x*x)))+(3.01155e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVM" && sEtamin == "0.0" && sEtamax == "0.8")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.07541+(0.00231827*x))+(-4.74249e-06*(x*x)))+(2.70862e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.964527+(0.00149055*x))+(-2.78338e-06*(x*x)))+(1.51771e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.18638+(0.00314148*x))+(-6.68993e-06*(x*x)))+(3.89288e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVM" && sEtamin == "0.8" && sEtamax == "1.6")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.05613+(0.00114031*x))+(-2.56066e-06*(x*x)))+(1.67792e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.946051+(0.000759584*x))+(-1.52491e-06*(x*x)))+(9.65822e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.16624+(0.00151884*x))+(-3.59041e-06*(x*x)))+(2.38681e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVM" && sEtamin == "1.6" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.05625+(0.000487231*x))+(-2.22792e-06*(x*x)))+(1.70262e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.956736+(0.000280197*x))+(-1.42739e-06*(x*x)))+(1.0085e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.15575+(0.000693344*x))+(-3.02661e-06*(x*x)))+(2.39752e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVT" && sEtamin == "0.0" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.00462+(0.00325971*x))+(-7.79184e-06*(x*x)))+(5.22506e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.845757+(0.00186422*x))+(-4.6133e-06*(x*x)))+(3.21723e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.16361+(0.00464695*x))+(-1.09467e-05*(x*x)))+(7.21896e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1L" && sEtamin == "0.0" && sEtamax == "0.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.03599+(0.00187708*x))+(-3.73001e-06*(x*x)))+(2.09649e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.995735+(0.00146811*x))+(-2.83906e-06*(x*x)))+(1.5717e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.0763+(0.00228243*x))+(-4.61169e-06*(x*x)))+(2.61601e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1L" && sEtamin == "0.5" && sEtamax == "1.0")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.987393+(0.00162718*x))+(-3.21869e-06*(x*x)))+(1.84615e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.947416+(0.00130297*x))+(-2.50427e-06*(x*x)))+(1.41682e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.02741+(0.00194855*x))+(-3.92587e-06*(x*x)))+(2.27149e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1L" && sEtamin == "1.0" && sEtamax == "1.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.950146+(0.00150932*x))+(-3.28136e-06*(x*x)))+(2.06196e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.91407+(0.00123525*x))+(-2.61966e-06*(x*x)))+(1.63016e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((0.986259+(0.00178067*x))+(-3.93596e-06*(x*x)))+(2.49014e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1L" && sEtamin == "1.5" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01923+(0.000898874*x))+(-2.57986e-06*(x*x)))+(1.8149e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.979782+(0.000743807*x))+(-2.14927e-06*(x*x)))+(1.49486e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.05868+(0.00105264*x))+(-3.00767e-06*(x*x)))+(2.13498e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1M" && sEtamin == "0.0" && sEtamax == "0.8")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.06383+(0.00279657*x))+(-5.75405e-06*(x*x)))+(3.4302e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.971686+(0.00195242*x))+(-3.98756e-06*(x*x)))+(2.38991e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.15605+(0.00363538*x))+(-7.50634e-06*(x*x)))+(4.4624e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1M" && sEtamin == "0.8" && sEtamax == "1.6")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.03709+(0.00169762*x))+(-3.52511e-06*(x*x)))+(2.25975e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.947328+(0.00117422*x))+(-2.32363e-06*(x*x)))+(1.46136e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.12687+(0.00221834*x))+(-4.71949e-06*(x*x)))+(3.05456e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1M" && sEtamin == "1.6" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.01679+(0.00211998*x))+(-6.26097e-06*(x*x)))+(4.53843e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.922527+(0.00176245*x))+(-5.14169e-06*(x*x)))+(3.61532e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11102+(0.00247531*x))+(-7.37745e-06*(x*x)))+(5.46589e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVV1T" && sEtamin == "0.0" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.15047+(0.00220948*x))+(-5.17912e-06*(x*x)))+(3.39216e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.936862+(0.00149618*x))+(-3.64924e-06*(x*x)))+(2.43883e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.36418+(0.00291794*x))+(-6.6956e-06*(x*x)))+(4.33793e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1L" && sEtamin == "0.0" && sEtamax == "0.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.06344+(0.0014539*x))+(-2.72328e-06*(x*x)))+(1.47643e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((1.01168+(0.000950951*x))+(-1.58947e-06*(x*x)))+(7.96543e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11523+(0.00195443*x))+(-3.85115e-06*(x*x)))+(2.15307e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1L" && sEtamin == "0.5" && sEtamax == "1.0")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.0123+(0.00151734*x))+(-2.99087e-06*(x*x)))+(1.73428e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.960377+(0.00109821*x))+(-2.01652e-06*(x*x)))+(1.13076e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.06426+(0.0019339*x))+(-3.95863e-06*(x*x)))+(2.3342e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1L" && sEtamin == "1.0" && sEtamax == "1.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.975277+(0.00146932*x))+(-3.17563e-06*(x*x)))+(2.03698e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.931687+(0.00110971*x))+(-2.29681e-06*(x*x)))+(1.45867e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.0189+(0.00182641*x))+(-4.04782e-06*(x*x)))+(2.61199e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1L" && sEtamin == "1.5" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.04201+(0.000827388*x))+(-2.31261e-06*(x*x)))+(1.62629e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.992838+(0.000660673*x))+(-1.84971e-06*(x*x)))+(1.2758e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.09118+(0.000992959*x))+(-2.77313e-06*(x*x)))+(1.9769e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1M" && sEtamin == "0.0" && sEtamax == "0.8")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.06212+(0.00223614*x))+(-4.25167e-06*(x*x)))+(2.42728e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.903956+(0.00121678*x))+(-2.04383e-06*(x*x)))+(1.10727e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.22035+(0.00325183*x))+(-6.45023e-06*(x*x)))+(3.74225e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1M" && sEtamin == "0.8" && sEtamax == "1.6")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.04547+(0.00216995*x))+(-4.579e-06*(x*x)))+(2.91791e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.900637+(0.00120088*x))+(-2.27069e-06*(x*x)))+(1.40609e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.19034+(0.00313562*x))+(-6.87854e-06*(x*x)))+(4.42546e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1M" && sEtamin == "1.6" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.991865+(0.00324957*x))+(-9.65897e-06*(x*x)))+(7.13694e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.868875+(0.00222761*x))+(-6.44897e-06*(x*x)))+(4.53261e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.11481+(0.00426745*x))+(-1.28612e-05*(x*x)))+(9.74425e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "CSVSLV1T" && sEtamin == "0.0" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.09494+(0.00193966*x))+(-4.35021e-06*(x*x)))+(2.8973e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.813331+(0.00139561*x))+(-3.15313e-06*(x*x)))+(2.12173e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.37663+(0.00247963*x))+(-5.53583e-06*(x*x)))+(3.66635e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPL" && sEtamin == "0.0" && sEtamax == "0.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.991991+(0.000898777*x))+(-1.88002e-06*(x*x)))+(1.11276e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.930838+(0.000687929*x))+(-1.36976e-06*(x*x)))+(7.94486e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.05319+(0.00110776*x))+(-2.38542e-06*(x*x)))+(1.42826e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPL" && sEtamin == "0.5" && sEtamax == "1.0")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.96633+(0.000419215*x))+(-9.8654e-07*(x*x)))+(6.30396e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.904781+(0.000324913*x))+(-7.2229e-07*(x*x)))+(4.52185e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.0279+(0.00051255*x))+(-1.24815e-06*(x*x)))+(8.07098e-10*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPL" && sEtamin == "1.0" && sEtamax == "1.5")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.968008+(0.000482491*x))+(-1.2496e-06*(x*x)))+(9.02736e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.914619+(0.000330357*x))+(-8.41216e-07*(x*x)))+(6.14504e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.02142+(0.000633484*x))+(-1.6547e-06*(x*x)))+(1.18921e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPL" && sEtamin == "1.5" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.991448+(0.000765746*x))+(-2.26144e-06*(x*x)))+(1.65233e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.933947+(0.000668609*x))+(-1.94474e-06*(x*x)))+(1.39774e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.04894+(0.000861785*x))+(-2.57573e-06*(x*x)))+(1.90702e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPM" && sEtamin == "0.0" && sEtamax == "0.8")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.991457+(0.00130778*x))+(-2.98875e-06*(x*x)))+(1.81499e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.822012+(0.000908344*x))+(-1.89516e-06*(x*x)))+(1.1163e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.16098+(0.00170403*x))+(-4.07382e-06*(x*x)))+(2.50873e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPM" && sEtamin == "0.8" && sEtamax == "1.6")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.00576+(0.00121353*x))+(-3.20601e-06*(x*x)))+(2.15905e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.845597+(0.000734909*x))+(-1.76311e-06*(x*x)))+(1.16104e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.16598+(0.00168902*x))+(-4.64013e-06*(x*x)))+(3.15214e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPM" && sEtamin == "1.6" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.939038+(0.00226026*x))+(-7.38544e-06*(x*x)))+(5.77162e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.803867+(0.00165886*x))+(-5.19532e-06*(x*x)))+(3.88441e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.07417+(0.00285862*x))+(-9.56945e-06*(x*x)))+(7.66167e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "JPT" && sEtamin == "0.0" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((0.953235+(0.00206692*x))+(-5.21754e-06*(x*x)))+(3.44893e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.642947+(0.00180129*x))+(-4.16373e-06*(x*x)))+(2.68061e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.26372+(0.0023265*x))+(-6.2548e-06*(x*x)))+(4.20761e-09*(x*(x*x)))", 20.,ptmax);
	}
	if( Atagger == "TCHPT" && sEtamin == "0.0" && sEtamax == "2.4")
	{
	if( meanminmax == "mean" ) tmpSFl = new TF1("SFlight","((1.20175+(0.000858187*x))+(-1.98726e-06*(x*x)))+(1.31057e-09*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "min" ) tmpSFl = new TF1("SFlightMin","((0.968557+(0.000586877*x))+(-1.34624e-06*(x*x)))+(9.09724e-10*(x*(x*x)))", 20.,ptmax);
	if( meanminmax == "max" ) tmpSFl = new TF1("SFlightMax","((1.43508+(0.00112666*x))+(-2.62078e-06*(x*x)))+(1.70697e-09*(x*(x*x)))", 20.,ptmax);
	}
 } //end "EPS2013" prescription


 if( tmpSFl == NULL ) 
 {
   cout << "NULL pointer returned... Function seems not to exist" << endl;
   cout << "meanminmax: " << meanminmax << " tagger: " << tagger << " TaggerStrength: " << TaggerStrength << " Etamin: " <<  Etamin  << " Etamax: " << Etamax << " DataPeriod: " << DataPeriod << endl; 
   tmpSFl = new TF1("dummy","-1",20.,1000);
 }
    

  return tmpSFl;

}

void BTagWeightTools::InitializeMCEfficiencyHistos(int NofPtBins,float PtMin,float PtMax,int NofEtaBins)
{
  _PtMaxhistos = PtMax;
  _histo2D["BtaggedJets"] = new TH2F("BtaggedJets", "Total number of btagged jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
	_histo2D["BtaggedBJets"] = new TH2F("BtaggedBJets", "Total number of btagged b-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
	_histo2D["BtaggedCJets"] = new TH2F("BtaggedCJets", "Total number of btagged c-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
	_histo2D["BtaggedLightJets"] = new TH2F("BtaggedLightJets", "Total number of btagged light jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
	_histo2D["TotalNofBJets"] = new TH2F("TotalNofBJets", "Total number of b-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
	_histo2D["TotalNofCJets"] = new TH2F("TotalNofCJets", "Total number of c-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
	_histo2D["TotalNofLightJets"] = new TH2F("TotalNofLightJets", "Total number of light jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
	MCEfficiencyHistosInitialized = true;	
}

void BTagWeightTools::FillMCEfficiencyHistos(vector< TRootJet* >& selectedJets)
{
  if(MCEfficiencyHistosInitialized)
	{
	  for (unsigned int i=0; i < selectedJets.size(); i++)
	  {
					float localPt = selectedJets[i]->Pt();
					float localEta = fabs(selectedJets[i]->Eta());
					if (localPt >= _PtMaxhistos) localPt = _PtMaxhistos-1;
					if (localEta == 2.4) localEta = 2.4-0.01;
					
					if (fabs(selectedJets[i]->partonFlavour()) == 5.) {	//b-jet
						_histo2D["TotalNofBJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets[i]->partonFlavour()) == 4.) {	//c-jet
						_histo2D["TotalNofCJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets[i]->partonFlavour()) == 1. || fabs(selectedJets[i]->partonFlavour()) == 2. || fabs(selectedJets[i]->partonFlavour()) == 3. || fabs(selectedJets[i]->partonFlavour()) == 21.) {	//udsg-jet
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets[i]->partonFlavour()) == 0.) {
						//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets[i]->partonFlavour() << ". Is pileup jet -> consider as light jet." << endl;
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets[i]->partonFlavour()) > 100.) {
						//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets[i]->partonFlavour() << ". Is hadron -> consider as light jet." << endl;
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
					
					float btagValue = -100.;
					if(_defaultalgo=="CSVL" || _defaultalgo=="CSVM" || _defaultalgo=="CSVT") btagValue = selectedJets[i]->btag_combinedSecondaryVertexBJetTags();
					else if(_defaultalgo=="CSVV1L" || _defaultalgo=="CSVV1M" || _defaultalgo=="CSVV1T") btagValue = selectedJets[i]->btag_combinedSecondaryVertexRetrainedBJetTags();
					else if(_defaultalgo=="CSVSLV1L" || _defaultalgo=="CSVSLV1M" || _defaultalgo=="CSVSLV1T") btagValue = selectedJets[i]->btag_combinedCSVSLBJetTags();
					else if(_defaultalgo=="JPL" || _defaultalgo=="JPM" || _defaultalgo=="JPT") btagValue = selectedJets[i]->btag_jetProbabilityBJetTags();
					else if(_defaultalgo=="TCHP") btagValue = selectedJets[i]->btag_trackCountingHighPurBJetTags();
					else std::cout<<" BTagWeightTools::FillMCEfficiencyHistos WARNING: Working Point for algorithm not found!"<<std::endl;
					
					if (btagValue > _algoWPcut) {
						_histo2D["BtaggedJets"]->Fill(localPt,localEta);
						
						if (fabs(selectedJets[i]->partonFlavour()) == 5.) {	//b-jet
							_histo2D["BtaggedBJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets[i]->partonFlavour()) == 4.) {	//c-jet
							_histo2D["BtaggedCJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets[i]->partonFlavour()) == 1. || fabs(selectedJets[i]->partonFlavour()) == 2. || fabs(selectedJets[i]->partonFlavour()) == 3. || fabs(selectedJets[i]->partonFlavour()) == 21.) {	//udsg-jet
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets[i]->partonFlavour()) == 0.) {
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets[i]->partonFlavour() << ". Is pileup jet -> consider as light jet." << endl;
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets[i]->partonFlavour()) > 100.) {
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets[i]->partonFlavour() << ". Is hadron -> consider as light jet." << endl;
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
					}
	  }
	}
	else std::cout<<"BTagWeightTools::FillMCEfficiencyHistos WARNING: not filling histos because not initialized!"<<std::endl;
} 

void BTagWeightTools::FillMCEfficiencyHistos(vector<TLorentzVector>& selectedJets, vector<int>& selectedJets_partonFlavour, vector<float>& selectedJets_bTagValues)
{
  if(MCEfficiencyHistosInitialized)
	{
    for (unsigned int i=0; i < selectedJets.size(); i++)
	  {
					float localPt = selectedJets[i].Pt();
					float localEta = fabs(selectedJets[i].Eta());
					if (localPt >= _PtMaxhistos) localPt = _PtMaxhistos-1;
					if (localEta == 2.4) localEta = 2.4-0.01;
					
					if (fabs(selectedJets_partonFlavour[i]) == 5.) {	//b-jet
						_histo2D["TotalNofBJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets_partonFlavour[i]) == 4.) {	//c-jet
						_histo2D["TotalNofCJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets_partonFlavour[i]) == 1. || fabs(selectedJets_partonFlavour[i]) == 2. || fabs(selectedJets_partonFlavour[i]) == 3. || fabs(selectedJets_partonFlavour[i]) == 21.) {	//udsg-jet
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets_partonFlavour[i]) == 0.) {
						//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets[i]->partonFlavour() << ". Is pileup jet -> consider as light jet." << endl;
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
					else if (fabs(selectedJets_partonFlavour[i]) > 100.) {
						//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets[i]->partonFlavour() << ". Is hadron -> consider as light jet." << endl;
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
										
					//cout<<"selectedJets_bTagValues["<<i<<"] = "<<selectedJets_bTagValues[i]<<", _algoWPcut = "<<_algoWPcut<<", flavor = "<<selectedJets_partonFlavour[i]<<endl;
					if (selectedJets_bTagValues[i] > _algoWPcut) {
						_histo2D["BtaggedJets"]->Fill(localPt,localEta);
						
						if (fabs(selectedJets_partonFlavour[i]) == 5.) {	//b-jet
							_histo2D["BtaggedBJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets_partonFlavour[i]) == 4.) {	//c-jet
							_histo2D["BtaggedCJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets_partonFlavour[i]) == 1. || fabs(selectedJets_partonFlavour[i]) == 2. || fabs(selectedJets_partonFlavour[i]) == 3. || fabs(selectedJets_partonFlavour[i]) == 21.) {	//udsg-jet
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets_partonFlavour[i]) == 0.) {
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets_partonFlavour[i] << ". Is pileup jet -> consider as light jet." << endl;
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
						else if (fabs(selectedJets_partonFlavour[i]) > 100.) {
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << selectedJets_partonFlavour[i] << ". Is hadron -> consider as light jet." << endl;
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
					}
	  }
	}
	else std::cout<<"BTagWeightTools::FillMCEfficiencyHistos WARNING: not filling histos because not initialized!"<<std::endl;
}

void BTagWeightTools::WriteMCEfficiencyHistos(std::string outfilename)
{
  if(MCEfficiencyHistosInitialized)
	{
      cout << "BTagWeightTools::WriteMCEfficiencyHistos: Creating output file to store the MC efficiency plots for the btag weights: " << outfilename << endl;			
			TFile *outfile = new TFile (outfilename.c_str(), "RECREATE");			
			outfile->cd();			
			_histo2D["BtaggedJets"]->Write();
			_histo2D["BtaggedBJets"]->Write();
			_histo2D["BtaggedCJets"]->Write();
			_histo2D["BtaggedLightJets"]->Write();
			_histo2D["TotalNofBJets"]->Write();
			_histo2D["TotalNofCJets"]->Write();
			_histo2D["TotalNofLightJets"]->Write();			
			outfile->Close();
	}
	else std::cout<<"BTagWeightTools::WriteMCEfficiencyHistos WARNING: not writing histos because not initialized!"<<std::endl;
}

void BTagWeightTools::ReadMCEfficiencyHistos(std::string infilename)
{
			TFile *infile = new TFile(infilename.c_str(),"READ");
			infile->cd();
			inBtaggedJets  = (TH2F*) infile->Get("BtaggedJets");
			inBtaggedBJets = (TH2F*) infile->Get("BtaggedBJets");
			inBtaggedCJets = (TH2F*) infile->Get("BtaggedCJets");
			inBtaggedLightJets = (TH2F*) infile->Get("BtaggedLightJets");
			inTotalNofBJets = (TH2F*) infile->Get("TotalNofBJets");
			inTotalNofCJets = (TH2F*) infile->Get("TotalNofCJets");
			inTotalNofLightJets = (TH2F*) infile->Get("TotalNofLightJets");
}

void BTagWeightTools::ReadMCEfficiencyHistos(std::string infilename_L,std::string infilename_T)
{
			TFile *infile_L = new TFile(infilename_L.c_str(),"READ");
			infile_L->cd();
			inBtaggedJets  = (TH2F*) infile_L->Get("BtaggedJets");
			inBtaggedBJets = (TH2F*) infile_L->Get("BtaggedBJets");
			inBtaggedCJets = (TH2F*) infile_L->Get("BtaggedCJets");
			inBtaggedLightJets = (TH2F*) infile_L->Get("BtaggedLightJets");
			inTotalNofBJets = (TH2F*) infile_L->Get("TotalNofBJets");
			inTotalNofCJets = (TH2F*) infile_L->Get("TotalNofCJets");
			inTotalNofLightJets = (TH2F*) infile_L->Get("TotalNofLightJets");
			
			TFile *infile_T = new TFile(infilename_T.c_str(),"READ");
			infile_T->cd();
			inBtaggedJets_T  = (TH2F*) infile_T->Get("BtaggedJets");
			inBtaggedBJets_T = (TH2F*) infile_T->Get("BtaggedBJets");
			inBtaggedCJets_T = (TH2F*) infile_T->Get("BtaggedCJets");
			inBtaggedLightJets_T = (TH2F*) infile_T->Get("BtaggedLightJets");
			inTotalNofBJets_T = (TH2F*) infile_T->Get("TotalNofBJets");
			inTotalNofCJets_T = (TH2F*) infile_T->Get("TotalNofCJets");
			inTotalNofLightJets_T = (TH2F*) infile_T->Get("TotalNofLightJets");
}

float BTagWeightTools::getMCEventWeight(vector< TRootJet* >& selectedJets, int syst)
{
      float probMC = 1.;
			float probData = 1.;
			float tagEff = 1.;
			float btagSF = 1.;
			for (unsigned int i=0; i < selectedJets.size(); i++) 
			{
						tagEff = getTagEff(selectedJets[i]->Pt(), selectedJets[i]->Eta(), selectedJets[i]->partonFlavour());											
						btagSF = getSF(selectedJets[i]->Pt(), selectedJets[i]->Eta(), selectedJets[i]->partonFlavour(), _defaultalgo, syst);
						//cout<<"  btagSF = "<<btagSF<<", tagEff = "<<tagEff<<", syst = "<<syst<<endl;	
														
						if (tagEff == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Tag efficiency is zero!" << endl;
								continue;
						}
						if (btagSF == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Btag scalefactor is zero!" << endl;
								continue;
						}
						//else if (btagSF*tagEff >= 1.)
						//{
								//cout << endl << "BTagWeightTools::getMCEventWeight WARNING:	BtagSF*tagEff is larger than 1!" << endl; //but this is not a problem?
						//}	
																			
						float btagValue = -100.;
					  if(_defaultalgo=="CSVL" || _defaultalgo=="CSVM" || _defaultalgo=="CSVT") btagValue = selectedJets[i]->btag_combinedSecondaryVertexBJetTags();
					  else if(_defaultalgo=="CSVV1L" || _defaultalgo=="CSVV1M" || _defaultalgo=="CSVV1T") btagValue = selectedJets[i]->btag_combinedSecondaryVertexRetrainedBJetTags();
					  else if(_defaultalgo=="CSVSLV1L" || _defaultalgo=="CSVSLV1M" || _defaultalgo=="CSVSLV1T") btagValue = selectedJets[i]->btag_combinedCSVSLBJetTags();
					  else if(_defaultalgo=="JPL" || _defaultalgo=="JPM" || _defaultalgo=="JPT") btagValue = selectedJets[i]->btag_jetProbabilityBJetTags();
					  else if(_defaultalgo=="TCHP") btagValue = selectedJets[i]->btag_trackCountingHighPurBJetTags();
					  else std::cout<<" BTagWeightTools::getMCEventWeight WARNING: Working Point for algorithm not found!"<<std::endl;							
						
						if (btagValue > _algoWPcut) //tagged
						{	
						    probMC = probMC*tagEff;
								probData = probData*btagSF*tagEff;
						}
						else
						{	//not tagged
								probMC = probMC*(1.-tagEff);
								probData = probData*(1.-btagSF*tagEff);
						}
			}
			float Weight = probData/probMC;
			return Weight;
}

float BTagWeightTools::getMCEventWeight(vector< TLorentzVector >& selectedJets, vector<int>& selectedJets_partonFlavour, vector<float>& selectedJets_bTagValues, int syst)
{
      float probMC = 1.;
			float probData = 1.;
			float tagEff = 1.;
			float btagSF = 1.;
			for (unsigned int i=0; i < selectedJets.size(); i++) 
			{
					  tagEff = getTagEff(selectedJets[i].Pt(), selectedJets[i].Eta(), selectedJets_partonFlavour[i]);											
						btagSF = getSF(selectedJets[i].Pt(), selectedJets[i].Eta(), selectedJets_partonFlavour[i], _defaultalgo, syst);
						//cout<<"  btagSF = "<<btagSF<<", tagEff = "<<tagEff<<", syst = "<<syst<<endl;
														
						if (tagEff == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Tag efficiency is zero!" << endl;
								continue;
						}
						if (btagSF == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Btag scalefactor is zero!" << endl;
								continue;
						}
						//else if (btagSF*tagEff >= 1.)
						//{
								//cout << endl << "BTagWeightTools::getMCEventWeight WARNING:	BtagSF*tagEff is larger than 1!" << endl; //but this is not a problem?
						//}
												
						if (selectedJets_bTagValues[i] > _algoWPcut) //tagged
						{	
						    probMC = probMC*tagEff;
								probData = probData*btagSF*tagEff;
						}
						else
						{	//not tagged
								probMC = probMC*(1.-tagEff);
								probData = probData*(1.-btagSF*tagEff);
						}
			}
			float Weight = probData/probMC;
			return Weight;
}

float BTagWeightTools::getMCEventWeight_LT(vector< TLorentzVector >& selectedJets, vector<int>& selectedJets_partonFlavour, vector<float>& selectedJets_bTagValues, int syst)
{
      float probMC = 1.;
			float probData = 1.;
			float tagEff_L = 1., tagEff_T = 1.;
			float btagSF_L = 1., btagSF_T = 1.;
			for (unsigned int i=0; i < selectedJets.size(); i++) 
			{
					  tagEff_L = getTagEff(selectedJets[i].Pt(), selectedJets[i].Eta(), selectedJets_partonFlavour[i]);
						tagEff_T = getTagEff_T(selectedJets[i].Pt(), selectedJets[i].Eta(), selectedJets_partonFlavour[i]);				
						btagSF_L = getSF(selectedJets[i].Pt(), selectedJets[i].Eta(), selectedJets_partonFlavour[i], _algo_L, syst);
						btagSF_T = getSF(selectedJets[i].Pt(), selectedJets[i].Eta(), selectedJets_partonFlavour[i], _algo_T, syst);
						//cout<<"  btagSF_L = "<<btagSF_L<<", tagEff_L = "<<tagEff_L<<", syst = "<<syst<<endl;
						//cout<<"  btagSF_T = "<<btagSF_T<<", tagEff_T = "<<tagEff_T<<", syst = "<<syst<<endl;
						
						if (tagEff_L == 0. || tagEff_T == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Tag efficiency is zero!" << endl;
								continue;
						}
						if (btagSF_L == 0. || btagSF_T == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Btag scalefactor is zero!" << endl;
								continue;
						}
						//else if (btagSF*tagEff >= 1.)
						//{
								//cout << endl << "BTagWeightTools::getMCEventWeight WARNING:	BtagSF*tagEff is larger than 1!" << endl; //but this is not a problem?
						//}
																
						if (selectedJets_bTagValues[i] > _algoWPcut_T) //tagged tight
						{	
						    probMC = probMC*tagEff_T;
								probData = probData*btagSF_T*tagEff_T;
						}
						else if(selectedJets_bTagValues[i] < _algoWPcut_L) //not tagged loose
						{
								probMC = probMC*(1.-tagEff_L);
								probData = probData*(1.-btagSF_L*tagEff_L);
						}
						else if(selectedJets_bTagValues[i] > _algoWPcut_L && selectedJets_bTagValues[i] < _algoWPcut_T) //tagged loose but not tagged tight
						{
								probMC = probMC*(tagEff_L - tagEff_T);
								probData = probData*(btagSF_L*tagEff_L - btagSF_T*tagEff_T);
						}
			}
			float Weight = probData/probMC;
			return Weight;
}

float BTagWeightTools::getTagEff(float pt, float eta, int flavor)
{
          float tagEff = 1.;
          int xBin = 0, yBin = 0;
					float PtMax = inTotalNofBJets->GetXaxis()->GetBinUpEdge( inTotalNofBJets->GetXaxis()->GetNbins() );
					if (pt < PtMax)
							xBin = inTotalNofBJets->GetXaxis()->FindBin(pt);		//Histograms have same binning.
					else if (pt >= PtMax)
							xBin = inTotalNofBJets->GetXaxis()->FindBin(PtMax-1);
					if (fabs(eta) < 2.4)
							yBin = inTotalNofBJets->GetYaxis()->FindBin(fabs(eta));
					else if (fabs(eta) == 2.4)
							yBin = inTotalNofBJets->GetYaxis()->FindBin(2.4-0.01);
									
					//cout<<" pt ="<<pt<<",	eta = "<<eta<<endl;		
					if (fabs(flavor) == 5.) 
					{
					    //cout<<" B"<<endl;
							float NofBJets = inTotalNofBJets->GetBinContent(xBin,yBin);
							if (NofBJets == 0.)
							{
									cout << "BTagWeightTools::getMCEventWeight WARNING: No b jets for bin (" << xBin << "," << yBin << ")!" << endl;
									return -1;
							}
							float NofTaggedBJets = inBtaggedBJets->GetBinContent(xBin,yBin);
							tagEff = NofTaggedBJets/NofBJets;
					}
					else if (fabs(flavor) == 4.)
					{
					    //cout<<" C"<<endl;
							float NofCJets = inTotalNofCJets->GetBinContent(xBin,yBin);
							if (NofCJets == 0.)
							{
							    cout << "BTagWeightTools::getMCEventWeight WARNING: No c jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedCJets = inBtaggedCJets->GetBinContent(xBin,yBin);
							tagEff = NofTaggedCJets/NofCJets;
					 }
					 else if (fabs(flavor) == 1. || fabs(flavor) == 2. || fabs(flavor) == 3. || fabs(flavor) == 21.)
					 {
					    //cout<<" DUSG"<<endl;
							float NofLightJets = inTotalNofLightJets->GetBinContent(xBin,yBin);
							if (NofLightJets == 0.)
							{
							    cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedLightJets = inBtaggedLightJets->GetBinContent(xBin,yBin);
							tagEff = NofTaggedLightJets/NofLightJets;
					 }
					 else if (fabs(flavor) == 0.)
					 {
					    //cout<<" PU?"<<endl;
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << flavor << ". Is pileup jet -> consider as light jet." << endl;
							float NofLightJets = inTotalNofLightJets->GetBinContent(xBin,yBin);
							if (NofLightJets == 0.)
							{
									cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedLightJets = inBtaggedLightJets->GetBinContent(xBin,yBin);
							tagEff = NofTaggedLightJets/NofLightJets;
					 }
					 else if (fabs(flavor) > 100.)
					 {
					     //cout<<" hadron?"<<endl;
							 //cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << flavor << ". Is hadron -> consider as light jet." << endl;
							 float NofLightJets = inTotalNofLightJets->GetBinContent(xBin,yBin);
							 if (NofLightJets == 0.)
							 {
								  cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							 }
							 float NofTaggedLightJets = inBtaggedLightJets->GetBinContent(xBin,yBin);
							 tagEff = NofTaggedLightJets/NofLightJets;
						}
						else
						{
							 cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Jet not identified! PdgId: " << flavor << endl;
							 return -1;
						}
						
						return tagEff;

}

float BTagWeightTools::getTagEff_T(float pt, float eta, int flavor)
{
          float tagEff = 1.;
          int xBin = 0, yBin = 0;
					float PtMax = inTotalNofBJets_T->GetXaxis()->GetBinUpEdge( inTotalNofBJets_T->GetXaxis()->GetNbins() );
					if (pt < PtMax)
							xBin = inTotalNofBJets_T->GetXaxis()->FindBin(pt);		//Histograms have same binning.
					else if (pt >= PtMax)
							xBin = inTotalNofBJets_T->GetXaxis()->FindBin(PtMax-1);
					if (fabs(eta) < 2.4)
							yBin = inTotalNofBJets_T->GetYaxis()->FindBin(fabs(eta));
					else if (fabs(eta) == 2.4)
							yBin = inTotalNofBJets_T->GetYaxis()->FindBin(2.4-0.01);
									
					//cout<<" pt ="<<pt<<",	eta = "<<eta<<endl;		
					if (fabs(flavor) == 5.) 
					{
					    //cout<<" B"<<endl;
							float NofBJets = inTotalNofBJets_T->GetBinContent(xBin,yBin);
							if (NofBJets == 0.)
							{
									cout << "BTagWeightTools::getMCEventWeight WARNING: No b jets for bin (" << xBin << "," << yBin << ")!" << endl;
									return -1;
							}
							float NofTaggedBJets = inBtaggedBJets_T->GetBinContent(xBin,yBin);
							tagEff = NofTaggedBJets/NofBJets;
					}
					else if (fabs(flavor) == 4.)
					{
					    //cout<<" C"<<endl;
							float NofCJets = inTotalNofCJets_T->GetBinContent(xBin,yBin);
							if (NofCJets == 0.)
							{
							    cout << "BTagWeightTools::getMCEventWeight WARNING: No c jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedCJets = inBtaggedCJets_T->GetBinContent(xBin,yBin);
							tagEff = NofTaggedCJets/NofCJets;
					 }
					 else if (fabs(flavor) == 1. || fabs(flavor) == 2. || fabs(flavor) == 3. || fabs(flavor) == 21.)
					 {
					    //cout<<" DUSG"<<endl;
							float NofLightJets = inTotalNofLightJets_T->GetBinContent(xBin,yBin);
							if (NofLightJets == 0.)
							{
							    cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedLightJets = inBtaggedLightJets_T->GetBinContent(xBin,yBin);
							tagEff = NofTaggedLightJets/NofLightJets;
					 }
					 else if (fabs(flavor) == 0.)
					 {
					    //cout<<" PU?"<<endl;
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << flavor << ". Is pileup jet -> consider as light jet." << endl;
							float NofLightJets = inTotalNofLightJets_T->GetBinContent(xBin,yBin);
							if (NofLightJets == 0.)
							{
									cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedLightJets = inBtaggedLightJets_T->GetBinContent(xBin,yBin);
							tagEff = NofTaggedLightJets/NofLightJets;
					 }
					 else if (fabs(flavor) > 100.)
					 {
					     //cout<<" hadron?"<<endl;
							 //cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << flavor << ". Is hadron -> consider as light jet." << endl;
							 float NofLightJets = inTotalNofLightJets_T->GetBinContent(xBin,yBin);
							 if (NofLightJets == 0.)
							 {
								  cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							 }
							 float NofTaggedLightJets = inBtaggedLightJets_T->GetBinContent(xBin,yBin);
							 tagEff = NofTaggedLightJets/NofLightJets;
						}
						else
						{
							 cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Jet not identified! PdgId: " << flavor << endl;
							 return -1;
						}
						
						return tagEff;

}

