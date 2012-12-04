#ifndef MCExpectation_h
#define MCExpectation_h

#include <string>
#include <math.h>
#include "Dataset.h"


using namespace std;

class MCExpectation: public TObject{

 public:
	MCExpectation();
	MCExpectation(string Label, float Luminosity, pair<float,float> TriggEff, pair<float,float> SkimEff);
	MCExpectation(const MCExpectation& a);
	~MCExpectation();
	
	pair<float,float> GetVJets();

	void SetLabel(string Label){ Name = Label;};
	void SetLuminosity(float Lumi){ Luminosity = Lumi;};
	void SetEfficiencies(pair<float,float> MuonSelEff, pair<float,float> SecondLeptonVetoEff, pair<float,float> JetSelEff);
	void Fill(const Dataset& d, float weight);
	void Compute();
	void FillEfficiencies(const Dataset& d, bool isMuonSelected, bool isPassVeto, bool isJetSelected);
	void AddBtagConditions(string algo, float bdisCut, pair<float,float> Eb, pair<float,float> Eusdc, pair<float,float> Eudsc_vj ); 

 public:
	float Luminosity;
	string Name;
	bool computed;
	bool EfficienciesGiven;

	//for V+j method
	vector< pair<string, float> > btag; //algo name & bdisc cut under several conditions
	//for each of them give the efficiencies
	vector< pair<float,float> > Eb;
	vector< pair<float,float> > Eudsc;
	vector< pair<float,float> > Eudsc_vj;
	
	pair<float,float> TriggerEff;
	pair<float,float> SkimEff;
	pair<float,float> MuonSelEff;
	pair<float,float> SecondLeptonVetoEff;
	pair<float,float> JetSelEff;
	
	pair<float,float> NAll;

	pair<float,float> NTtJets;
	pair<float,float> NWlj;
	pair<float,float> NWb;
	pair<float,float> NWc;
	pair<float,float> NWbb;
	pair<float,float> NWcc;
	pair<float,float> NZlj;
	pair<float,float> NZb;
	pair<float,float> NZc;
	pair<float,float> NZbb;
	pair<float,float> NZcc;
	pair<float,float> NQCD;
	pair<float,float> NSingleTop;
	pair<float,float> NVV;
	
  ClassDef(MCExpectation,1);
};

#endif
