#ifndef TemplateComparator_h
#define TemplateComparator_h

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "stdio.h"
#include "math.h"
#include "TH1F.h"

using namespace std;

	/**
	//	Aim: Comparing templates for a list of observables coming from 2 differents side,
	//		ex: compare expectations (MC) and estimations (data-driven).
	//
	//	How to use:
	//	- Call the constructor()
	//	- AddNormalisation() with the nof events
	//	- Compute()
	//	- PrintTable() to see the results
	*/


class TemplateComparator{

  
 public:
  
  TemplateComparator(vector<TH1F*> histos_cat1, vector<TH1F*> histos_cat2, vector<string> ObsNames);/** there is a list of histograms for each category corresponding to several observables. Application cat1: expectations, cat2: estimations*/
  TemplateComparator(const TemplateComparator& tc);
  ~TemplateComparator(){;};
  

  void AddNormalisation(pair<float,float> cat1, pair<float,float> cat2);/** Pair of #events and its associated erros for each category*/
  void Compute();/**Compute the momenta of the distributions and execute the tests */
  void PrintTable(bool doLatexMode = false, int nSigmasCompatibility = 1);
  /** Produce a table in a latex style if boolean is equal to true. Check compatiblity of the distribution according errors on the momenta at n sigmas.*/
  bool Compatibility(pair<float,float> v1, pair<float,float> v2, int nsigmas);
  bool AreCompatibleDistributions(int numVar, int nofSigmas = 1);/** Look that all momenta are compatible within n sigmas for a given distribution (numVar) */

private:
	vector<string> MomentNames;
	vector<string> TestNames;
	vector<string> ObsNames;
	//histo
	vector<TH1F*> hcat1;
	vector<TH1F*> hcat2;
  	//#events
	pair<float,float> ncat1;
	pair<float,float> ncat2;
	//moment values
	vector<vector<pair<float,float> > > mcat1;/** momenta and associated errors*/
	vector<vector<pair<float,float> > > mcat2;/** momenta and associated errors*/
	//test values
	vector<vector<float> > tests;
};

#endif
