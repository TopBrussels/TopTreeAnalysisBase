#ifndef Container_h
#define Container_h

#include <string>
#include <iostream>
#include <vector>
#include "TChain.h"

using namespace std;

/**
	This is class is designed to store vector of std values like int,float,double or string
	As it inherit from TObject, it can be used in a TList or a TClonesArray
*/

class Container: public TObject
{
public:
	Container(){};
	Container(const Container &d);
	~Container(){}; 
	
	void Clear();

	vector<Int_t> GetInt(){ return int_;};
	vector<Float_t> GetFloat(){ return float_;};
	vector<Double_t> GetDouble(){ return double_;};
	vector<string> GetString(){ return string_;};

	void SetInt(vector<Int_t> v) {int_ = v;};
	void SetFloat(vector<Float_t> v) {float_ = v;};
	void SetDouble(vector<Double_t> v) {double_ = v;};
	void SetString(vector<string> v) {string_ = v;};

	void Add(Int_t a){int_.push_back(a);};
	void Add(Float_t a){float_.push_back(a);};
	void Add(Double_t a){double_.push_back(a);};
	void Add(string a){string_.push_back(a);};

private:	
	vector<Int_t> int_;
	vector<Float_t> float_;
	vector<Double_t> double_;
	vector<string> string_;

ClassDef(Container,1);
};




#endif


