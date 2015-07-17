#include "../interface/EventBDT.h"

EventBDT::EventBDT():
	mvaVars_(),
	eventComputer_(),
	mvaValues_(),
	scoreOfBDT_(0)
{
    mvaVars_.push_back("multitopness");
    mvaVars_.push_back("muonpt");
    mvaVars_.push_back("muoneta");
    mvaVars_.push_back("HTH");
    mvaVars_.push_back("HTRat");
    mvaVars_.push_back("HTb");
    mvaVars_.push_back("nLtags");
    mvaVars_.push_back("nMtags");
    mvaVars_.push_back("nTtags");
    mvaVars_.push_back("nJets");
    mvaVars_.push_back("Jet5Pt");
    mvaVars_.push_back("Jet6Pt");
}

EventBDT::~EventBDT(){

}

void EventBDT::initaliseEventComp(){
	eventComputer_ = new MVAComputer("BDT","MVA/MasterMVA_SingleMuon_24thMarch.root","MasterMVA_SingleMuon_24thMarch",mvaVars_, "_SingleMuon24thMarch2015");

}

void EventBDT::fillVariables(float ditopness, float muonpt, float muoneta, float HTH, float HTRat, float HTb, float nLtags, float nMtags, float nTtags, float nJets, float jet5Pt, float jet6Pt){
	eventComputer_->FillVar("multitopness", ditopness);
	eventComputer_->FillVar("muonpt",muonpt);
	eventComputer_->FillVar("muoneta",muoneta);
	eventComputer_->FillVar("HTH", HTH);
	eventComputer_->FillVar("HTRat",HTRat);
	eventComputer_->FillVar("HTb", HTb);   
	eventComputer_->FillVar("nLtags",nLtags );     
	eventComputer_->FillVar("nMtags",nMtags );     
	eventComputer_->FillVar("nTtags",nTtags );     
	eventComputer_->FillVar("nJets", nJets );
	eventComputer_->FillVar("Jet5Pt", jet5Pt );
	eventComputer_->FillVar("Jet6Pt", jet6Pt );
}

void EventBDT::computeBDTScore(){
	mvaValues_ = eventComputer_->GetMVAValues();  
	for (std::map<std::string,Float_t>::const_iterator it = mvaValues_.begin(); it !=mvaValues_.end(); ++it){
	    //  cout <<"MVA Method : "<< it->first    <<" Score : "<< it->second <<endl;
	    scoreOfBDT_ = it->second;
	}
}

float EventBDT::returnBDTScore(){
	return scoreOfBDT_;
}