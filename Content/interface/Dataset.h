#ifndef Dataset_h
#define Dataset_h

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "TChain.h"

using namespace std;

class Dataset: public TObject
{
public:
	Dataset():Name_(string("")), Title_(string("")), DotIt_(false), Color_(1), LineStyle_(1), LineWidth_(2), NormFactor_(1), Xsection_(-1), PreSelEfficiency_(1.), EqLumi_(-1), eventTree_(NULL), runTree_(NULL), NofEvtsToRunOver_(-999) {;};
	Dataset(string name, string title, bool doIt, int color, int lineStyle, int lineWidth, float normFactor, float xsection): Name_(name), Title_(title), DotIt_(doIt), Color_(color), LineStyle_(lineStyle),LineWidth_(lineWidth), NormFactor_(normFactor), Xsection_(xsection), PreSelEfficiency_(1.), EqLumi_(-1), eventTree_(NULL), runTree_(NULL), NofEvtsToRunOver_(-999) {;};
	Dataset(string name, string title, bool doIt, int color, int lineStyle, int lineWidth, float normFactor, float xsection, vector<string> filenames): Name_(name), Title_(title), DotIt_(doIt), Color_(color), LineStyle_(lineStyle), LineWidth_(lineWidth), NormFactor_(normFactor), Xsection_(xsection), PreSelEfficiency_(1.), EqLumi_(-1), NofEvtsToRunOver_(-999), Filenames_(filenames) {
		eventTree_ = new TChain("eventTree"); 
		runTree_   = new TChain("runTree"); 
		for(unsigned int i=0;i<Filenames_.size();i++) eventTree_->Add(Filenames_[i].c_str());
		for(unsigned int i=0;i<Filenames_.size();i++)   runTree_->Add(Filenames_[i].c_str());
	};
	Dataset(const Dataset &d);
	~Dataset(){
		if(eventTree_) eventTree_->Delete();
		if(runTree_)     runTree_->Delete();
	};
	
	string Name()  const{return Name_;};
	string Title() const{return Title_;};
	bool DoIt() const{return DotIt_;};
	int Color() const{return Color_;};
	int LineStyle() const{return LineStyle_;};
	int LineWidth() const {return LineWidth_;};
	float NormFactor() const {return NormFactor_;};
	float Xsection() const {return Xsection_;};
	float XsectionError() const{return XsectionError_;};
	float Mass() const{return Mass_;};
	float PreSelEfficiency() const {return PreSelEfficiency_;};
	float EquivalentLumi() const {return EqLumi_;};
	int NofEvtsToRunOver() const { if(NofEvtsToRunOver_>0 && eventTree_ && eventTree_->GetEntries()>=NofEvtsToRunOver_ ) return NofEvtsToRunOver_ ; if(eventTree_) return eventTree_->GetEntries(); return 0;};
	TChain* eventTree() const{return eventTree_;};
	TChain* runTree() const{return runTree_;};
	vector<string> Filenames() const {return Filenames_;};
	vector< vector<int> > JSONInfo() const {return JSONInfo_;};

	void SetName(string name) { Name_ = name; }
	void SetTitle(string name) { Title_ = name ;}
	void SetEquivalentLuminosity(float EqLumi){ if(EqLumi>0) { NormFactor_ = 1/EqLumi; EqLumi_ = EqLumi; } };/** will recompute NormFactor = 1/EqLumi*/
	void SetXsection(float Xsection){ Xsection_ = Xsection;};
	void SetMass(float Mass){ Mass_ = Mass;};
	void SetOriginalNumberOfEvents(int NofEvts){ if(NofEvts>0) NormFactor_ = Xsection_/NofEvts; NofEvtsToRunOver_ = NofEvts;};/** will compute NormFactor = Xsection/TNofEvts */
	void SetPreselEffAndNumberOfPreselEvents(float PreselEff, int NofSEvts){ if(NofSEvts>0) NormFactor_ = Xsection_*PreselEff/NofSEvts; NofEvtsToRunOver_ = NofSEvts; PreSelEfficiency_ = PreselEff;}; /** will compute NormFactor = Xsection*PreselEff/NofPreselEvts */
	void SetXsectionError(float xsectionError){XsectionError_ = xsectionError;};
	void ReadJSONFile(string JSONFile);
	void SetColor(int Color){Color_=Color;};

private:	
	string Name_;
	string Title_;
	bool DotIt_;
	int Color_;
	int LineStyle_;
	int LineWidth_;
	float NormFactor_;
	float Xsection_;
	float XsectionError_;
	float Mass_;
	float PreSelEfficiency_;
	float EqLumi_;
	TChain* eventTree_;
	TChain* runTree_;
	int NofEvtsToRunOver_;
	vector<string> Filenames_;
	vector< vector<int> > JSONInfo_;

ClassDef(Dataset,2); //was 1...
};




#endif


