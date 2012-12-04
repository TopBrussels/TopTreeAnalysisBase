#include "../interface/MCExpectation.h"

ClassImp(MCExpectation);

MCExpectation::MCExpectation(){
  Luminosity = 0;
  Name = string("");
  computed = false;
  EfficienciesGiven = false;
  TriggerEff = pair<float,float>(1,0);
  SkimEff = pair<float,float>(1,0);
  MuonSelEff = pair<float,float>(0,0);
  SecondLeptonVetoEff = pair<float,float>(0,0);
  JetSelEff = pair<float,float>(0,0);
  NAll = pair<float,float>(0,0);
  NTtJets = pair<float,float>(0,0);
  NWlj = pair<float,float>(0,0);
  NWb = pair<float,float>(0,0);
  NWc = pair<float,float>(0,0);
  NWbb = pair<float,float>(0,0);
  NWcc = pair<float,float>(0,0);
  NZlj = pair<float,float>(0,0);
  NZb = pair<float,float>(0,0);
  NZc = pair<float,float>(0,0);
  NZbb = pair<float,float>(0,0);
  NZcc = pair<float,float>(0,0);
  NQCD = pair<float,float>(0,0);
  NSingleTop = pair<float,float>(0,0);
  NVV = pair<float,float>(0,0);
}

MCExpectation::MCExpectation(string Label, float Lumi, pair<float,float> TriggEff_, pair<float,float> SkimEff_){
  Luminosity = Lumi;
  Name = Label;
  computed = false;
  EfficienciesGiven = false;
  TriggerEff = TriggEff_;
  SkimEff = SkimEff_;
  MuonSelEff = pair<float,float>(0,0);
  SecondLeptonVetoEff = pair<float,float>(0,0);
  JetSelEff = pair<float,float>(0,0);
  NAll = pair<float,float>(0,0);
  NTtJets = pair<float,float>(0,0);
  NWlj = pair<float,float>(0,0);
  NWb = pair<float,float>(0,0);
  NWc = pair<float,float>(0,0);
  NWbb = pair<float,float>(0,0);
  NWcc = pair<float,float>(0,0);
  NZlj = pair<float,float>(0,0);
  NZb = pair<float,float>(0,0);
  NZc = pair<float,float>(0,0);
  NZbb = pair<float,float>(0,0);
  NZcc = pair<float,float>(0,0);
  NQCD = pair<float,float>(0,0);
  NSingleTop = pair<float,float>(0,0);
  NVV = pair<float,float>(0,0);
}


MCExpectation::MCExpectation(const MCExpectation& a){
  Luminosity = a.Luminosity;
  Name = a.Name;
  computed = a.computed;
  btag = a.btag;
  Eb = a.Eb;
  Eudsc = a.Eudsc;
  Eudsc_vj = a.Eudsc_vj;
  EfficienciesGiven = a.EfficienciesGiven;
  TriggerEff = a.TriggerEff;
  SkimEff = a.SkimEff;
  MuonSelEff = a.MuonSelEff;
  SecondLeptonVetoEff = a.SecondLeptonVetoEff;
  JetSelEff = a.JetSelEff;
  NAll = a.NAll;
  NTtJets = a.NTtJets;
  NWlj = a.NWlj;
  NWb = a.NWb ;
  NWc = a.NWc;
  NWbb = a.NWbb;
  NWcc = a.NWcc;
  NZlj = a.NZlj;
  NZb = a.NZb;
  NZc = a.NZc;
  NZbb = a.NZbb;
  NZcc = a.NZcc;
  NQCD = a.NQCD;
  NSingleTop = a.NSingleTop;
  NVV = a.NVV;
}

MCExpectation::~MCExpectation(){
}

void MCExpectation::SetEfficiencies(pair<float,float> MuonSelEff_, pair<float,float> SecondLeptonVetoEff_, pair<float,float> JetSelEff_){
 EfficienciesGiven = true;
 MuonSelEff = MuonSelEff_;
 SecondLeptonVetoEff = SecondLeptonVetoEff_;
 JetSelEff = JetSelEff_;
}

pair<float,float> MCExpectation::GetVJets(){
	float value = NWlj.first+NWlj.first+NWc.first+NWb.first+NWcc.first+NWbb.first;
	value+=NZlj.first+NZlj.first+NZc.first+NZb.first+NZcc.first+NZbb.first;
	float error = pow(NWlj.second,2)+pow(NWlj.second,2)+pow(NWc.second,2)+pow(NWb.second,2)+pow(NWcc.second,2)+pow(NWbb.second,2);
	error+=pow(NZlj.second,2)+pow(NZlj.second,2)+pow(NZc.second,2)+pow(NZb.second,2)+pow(NZcc.second,2)+pow(NZbb.second,2);
	return(pair<float,float>(value,error));
}

void MCExpectation::Compute(){
  if(computed) return;
  computed = true;
  NAll.second = sqrt(NAll.first);
  NTtJets.second = sqrt(NTtJets.first);
  NWlj.second = sqrt(NWlj.first);
  NWb.second = sqrt(NWb.first);
  NWc.second = sqrt(NWc.first);
  NWbb.second = sqrt(NWbb.first);
  NWcc.second = sqrt(NWcc.first);
  NZlj.second = sqrt(NZlj.first);
  NZb.second = sqrt(NZb.first);
  NZc.second = sqrt(NZc.first);
  NZbb.second = sqrt(NZbb.first);
  NZcc.second = sqrt(NZcc.first);
  NQCD.second = sqrt(NQCD.first);
  NSingleTop.second = sqrt(NSingleTop.first);
  NVV.second = sqrt(NVV.first);
  if(!EfficienciesGiven){
  	float num = 0; float denom = 0;
	num = MuonSelEff.first ; denom = MuonSelEff.second; if(denom>0) MuonSelEff.first = num/denom; MuonSelEff.second = sqrt(MuonSelEff.second);
	num = SecondLeptonVetoEff.first ; denom = SecondLeptonVetoEff.second; if(denom>0) SecondLeptonVetoEff.first = num/denom; SecondLeptonVetoEff.second = sqrt(SecondLeptonVetoEff.second);
	num = JetSelEff.first ; denom = JetSelEff.second; if(denom>0) JetSelEff.first = num/denom; JetSelEff.second = sqrt(JetSelEff.second);
  }
}

void MCExpectation::Fill(const Dataset& d, float weight){
   if(d.Name() == string("TTJets")){ NTtJets.first++; NTtJets.second+=weight; }
   if(d.Name() == string("WJets")) { NWlj.first++; NWlj.second+=weight; }
   if(d.Name() == string("Wb")) { NWb.first++; NWb.second+=weight; }
   if(d.Name() == string("Wc")) { NWc.first++; NWc.second+=weight; }
   if(d.Name() == string("Wbb")) { NWbb.first++; NWbb.second+=weight; }
   if(d.Name() == string("Wcc")) { NWcc.first++; NWcc.second+=weight; }
   if(d.Name() == string("ZJets")) { NZlj.first++; NZlj.second+=weight; }
   if(d.Name() == string("Zb")) { NZb.first++; NZb.second+=weight; }
   if(d.Name() == string("Zc")) { NZc.first++; NZc.second+=weight; }
   if(d.Name() == string("Zbb")) { NZbb.first++; NZbb.second+=weight; }
   if(d.Name() == string("Zcc")) { NZcc.first++; NZcc.second+=weight; }
   if(d.Name() == string("QCD")) { NQCD.first++; NQCD.second+=weight; }
   if(d.Name() == string("SingleTop")) { NSingleTop.first++; NSingleTop.second+=weight; }
   if(d.Name() == string("VV")) { NVV.first++; NVV.second+=weight; }
}

void MCExpectation::FillEfficiencies(const Dataset& d, bool isMuonSelected, bool isPassVeto, bool isJetSelected){
   if(d.Name() == string("TTJets")){
   	//order has an importance on the definition of the efficiencies !!
   	if(isMuonSelected) MuonSelEff.first++;
	if(isMuonSelected && isPassVeto) SecondLeptonVetoEff.first++;
	if(isMuonSelected && isPassVeto && isJetSelected) JetSelEff.first++;
	MuonSelEff.second++;
	SecondLeptonVetoEff.second++;
	JetSelEff.second++;
   }
}

void MCExpectation::AddBtagConditions(string algo, float bdisCut, pair<float,float> eb, pair<float,float> eudsc, pair<float,float> eudsc_vj){
	btag.push_back(pair<string,float>(algo,bdisCut));
	Eb.push_back(eb);
	Eudsc.push_back(eudsc);
	Eudsc_vj.push_back(eudsc_vj);
}
