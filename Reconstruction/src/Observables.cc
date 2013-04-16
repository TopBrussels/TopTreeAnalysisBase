#include "../interface/Observables.h"
#include "../interface/PlotObservables.h"
#include "../interface/TTreeObservables.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TMath.h"
#include <TLorentzVector.h>
#include "TClonesArray.h"
#include "TBranch.h"

#include <TTree.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>





Observables::Observables ()
{

  DefaultValue_ = -9999;
  MCavailable_ = false;
  met_ = TRootMET ();
  lepton_ = TRootMuon ();
  quark1_ = TRootJet ();
  quark2_ = TRootJet ();
  hadbquark_ = TRootJet ();
  lepbquark_ = TRootJet ();
  jets_.clear ();
  //hemisphere_ = pat::Hemisphere();
  PzNeutrino_ = 0.;
  ChiSquare_ = DefaultValue_;
  ChiSquareHadTop_ = DefaultValue_;
  string setname = "";
    
cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
ComputeVariables ();


}

Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const TRootMET & MET,float & ChiSq_)
{
 //this constructor used in first version of t' analysis
  MCavailable_ = false;
  met_ = MET;
  lepton_ = lepton;
  quark1_ = qFromW1;
  quark2_ = qFromW2;
  hadbquark_ = bFromHadTop;
  lepbquark_ = bFromLepTop;
  jets_.clear ();
  
  //added the following!
  jets_.push_back(quark1_);
  jets_.push_back(quark2_);
  jets_.push_back(hadbquark_);
  jets_.push_back(lepbquark_);
  
  //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = ChiSq_;
  ChiSquareHadTop_ = 0.;
 

  ComputeVariables();
  
  delete NuPzCalc;
}


Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const TRootMET & MET)
{
  MCavailable_ = false;
  met_ = MET;
  lepton_ = lepton;
  quark1_ = qFromW1;
  quark2_ = qFromW2;
  hadbquark_ = bFromHadTop;
  lepbquark_ = bFromLepTop;
  
  //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
 // ChiSquare_ = 0.;
 // ChiSquareHadTop_ = 0.;
cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
  ComputeVariables();
  
  delete NuPzCalc;
}



Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootMET & MET)
{
  MCavailable_ = false;
  met_ = MET;
  lepton_ = lepton;
  quark1_ = qFromW1;
  quark2_ = qFromW2;
  hadbquark_ = bFromHadTop;
  lepbquark_ = TRootJet ();
//  jets_.clear ();
  //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = 0.;
  ChiSquareHadTop_ = 0.;
  
 cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
  ComputeVariables();
  
  delete NuPzCalc;

}

Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const TRootMET & MET, const vector < TRootJet > &jets)
{
  jets_.clear();
  jets_ = jets;
  (*this) = Observables (lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, MET);
    //  SetJets (jets);
}

Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const TRootMET & MET, const vector < TRootJet* > &jets,vector <float> values_,float & ChiSq_ )
{
  ChiSquare_= ChiSq_;
 // chi2_max=chi2_max_;
  jets_.clear();
  for (unsigned int i=0; i< jets.size();i++) {
	  jets_.push_back(*jets[i]);
//cout<<"  Parsinggggggg values  "<<jets_[i].Pt()<<"  "<<jets[i]->Pt()<<" i  "<<i<<"   "<<jets.size()<<"   qFromW1  "<< qFromW1.Pt()<<"  qFromW2  "<< qFromW2.Pt()<<"  "<<endl;
  }
 // (*this) = Observables (lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, jets_,MET,ChiSq_);
  (*this) = Observables (lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, jets_,values_,MET,ChiSq_);
  

}




Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromTop, const TRootMET & MET, const vector < TRootJet > &jets)
{
  jets_.clear();
  jets_ = jets;
  (*this) = Observables (lepton, qFromW1, qFromW2, bFromTop, MET);
   // SetJets (jets);
}

/*
Observables::Observables(const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromTop,  const TRootMET& MET, const vector<TRootJet>& jets, const TRootHemisphere& hemisphere){
  (*this) = Observables(lepton, qFromW1, qFromW2, bFromTop, MET);
  SetJets(jets);
  SetHemisphere(hemisphere);
}
*/

Observables::Observables (const Observables & obs)
{

 // jets_.clear();
  MCavailable_ = false;
  /*
     met_ = TRootMET();
     lepton_ = TRootMuon();
     quark1_ = TRootJet();
     quark2_ = TRootJet();
     hadbquark_ = TRootJet();
     lepbquark_ = TRootJet();
     jets_.clear(); */

  met_ = obs.met_;
  lepton_ = obs.lepton_;
  quark1_ = obs.quark1_;
  quark2_ = obs.quark2_;
  hadbquark_ = obs.hadbquark_;
  lepbquark_ = obs.lepbquark_;
  jets_ = obs.jets_;
  //hemisphere_ = obs.hemisphere_;
  PzNeutrino_ = obs.PzNeutrino_;
  ChiSquare_ = 999999.;
  ChiSquareHadTop_ = 999999.;

cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
  ComputeVariables();

}






Observables::Observables (const TRootMuon & lepton, const vector < TRootJet > &chijets_, const vector < TRootJet > &jets, const TRootMET & MET, string & setname ,float & ChiSq_ )
{
  //cout<<"  JUST EENTERED THE COMPLEX  "<<endl;
  DefaultValue_ = -9999;  
  met_ = TRootMET ();
  lepton_ = TRootMuon ();
  quark1_ = TRootJet ();
  quark2_ = TRootJet ();
  hadbquark_ = TRootJet ();
  lepbquark_ = TRootJet ();
  jets_.clear ();
  
  ChiSquare_ = ChiSq_;

  //hemisphere_ = pat::Hemisphere();



  quark1_ = chijets_[0];
  quark2_ = chijets_[1];
  hadbquark_ = chijets_[2];
  lepbquark_ = chijets_[3];
  met_ = MET;
  lepton_ = lepton;
  
    //  SetLepton (lepton_);
    //  SetMET (MET);
  
  jets_ = jets;
    //  SetJets(jets);
 
 //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton_);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  //ChiSquare_ = 0.;
  ChiSquareHadTop_ = 0.;
 cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<<chijets_[0].Pt()<< "  "<<chijets_[1].Pt()<<"  "<<chijets_[2].Pt()<<"  "<<chijets_[3].Pt()<<endl; 

  ComputeVariables();
  
  delete NuPzCalc;

}






Observables::Observables (const TRootMuon & lepton, const vector < TRootJet > &jets, const TRootMET & MET, string & setname)
{
  //cout<<"  JUST EENTERED THE COMPLEX  "<<endl;

  //cout<<"  parameters "<<lepton_.Pt()<<"  "<<quark1_.Pt()<<"  "<<quark2_.Pt()<<"    "<<jets_[0].Pt()<<endl; 

  //cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<<chijets_[0].Pt()<< "  "<<chijets_[1].Pt()<<"  "<<chijets_[2].Pt()<<"  "<<chijets_[3].Pt()<<endl; 

  lepton_ = TRootMuon ();
  quark1_ = TRootJet ();
  quark2_ = TRootJet ();
  hadbquark_ = TRootJet ();
  lepbquark_ = TRootJet ();
  met_ = MET;
    //  SetLepton (lepton);
    //  SetMET (MET);
  lepton_ = lepton;

  jets_.clear ();
  jets_ = jets;
  //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = 0.;
  ChiSquareHadTop_ = 0.;
  vector < TRootJet > chijets_;

  for (unsigned int i = 0; i < jets.size (); i++)
    {
      chijets_.push_back (jets[i]);
    }

 
cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
  ComputeVariables ();


  delete NuPzCalc;
}



Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const vector < TRootJet > &jets, const TRootMET & MET)
{

  lepton_ = TRootMuon ();
  quark1_ = TRootJet ();
  quark2_ = TRootJet ();
  hadbquark_ = TRootJet ();
  lepbquark_ = TRootJet ();
  jets_.clear ();
  met_ = MET;
  lepton_ = lepton;
  quark1_ = qFromW1;
  quark2_ = qFromW2;
  hadbquark_ = bFromHadTop;
  lepbquark_ = bFromLepTop;

  jets_ = jets;

//hemisphere_ = pat::Hemisphere();
  // jets_= jets;  
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = 0.;
  ChiSquareHadTop_ = 0.;

cout<<"   ppppparameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
  ComputeVariables();

  delete NuPzCalc;
}


Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const vector < TRootJet > &jets,vector<float> values_,const TRootMET & MET, float &ChiSq){

  lepton_ = TRootMuon ();
  quark1_ = TRootJet ();
  quark2_ = TRootJet ();
  hadbquark_ = TRootJet ();
  lepbquark_ = TRootJet ();
  jets_.clear ();
  met_ = MET;
  lepton_ = lepton;
  quark1_ = qFromW1;
  quark2_ = qFromW2;
  hadbquark_ = bFromHadTop;
  lepbquark_ = bFromLepTop;



  jets_ = jets;

//hemisphere_ = pat::Hemisphere();
  // jets_= jets;  
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = ChiSq;
  ChiSquareHadTop_ = 0.;
  btaggerHadB = values_[0];
  btaggerLepB = values_[1];
  //CombBtagInfo = values_[2];
  //cout<<"  "<<values_[0]<<"  "<<values_[1]<<endl;
//cout<<"   PPPparameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
  ComputeVariables();

  delete NuPzCalc;
}

Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const vector < TRootJet > &jets, const TRootMET & MET, float &ChiSq, bool GoodChi2){

  lepton_ = TRootMuon ();
  quark1_ = TRootJet ();
  quark2_ = TRootJet ();
  hadbquark_ = TRootJet ();
  lepbquark_ = TRootJet ();
  jets_.clear ();
  met_ = MET;
  lepton_ = lepton;
  if (GoodChi2){
  quark1_ = qFromW1;
  quark2_ = qFromW2;
  hadbquark_ = bFromHadTop;
  lepbquark_ = bFromLepTop;
  }

  jets_ = jets;

//hemisphere_ = pat::Hemisphere();
  // jets_= jets;  
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = ChiSq;
  ChiSquareHadTop_ = 0.;

//cout<<"   PPPparameters_should_match_above "<<lepton_.Pt()<<  "  "<< quark1_.Pt()<< "  "<< quark2_.Pt()<<"  "<<hadbquark_.Pt()<<"  "<<lepbquark_.Pt()<<endl; 
  ComputeVariables();

  delete NuPzCalc;
}


Observables::~Observables ()
{
}



void Observables::SetMCInfo(const TRootParticle& lepton, const TRootMCParticle& qFromW1, const TRootMCParticle& qFromW2, const TRootJet& bFromHadTop, const TRootMCParticle& bFromLepTop, const TRootMET& MET, const vector<TRootMCParticle>& jets)
{
  MCavailable_ = true;
  metMC_ = MET;
  leptonMC_ = lepton;
  quark1MC_ = qFromW1;
  quark2MC_ = qFromW2;
  hadbquarkMC_ = bFromHadTop;
  jetsMC_ = jets;
  variables_.clear();
  RangeVariables_.clear();
  string name("");
  ComputeVariables ();
}


/* Variables for RECO and MC have to be filled in the same order and a MC can only be present if a RECO is present. */

void
Observables::ComputeVariables ()
{
DefaultValue_ = -9999;
//cout << "Start computing variables............................Jets Size " <<jets_.size()<< "  "<<quark1_.Pt()<<"  "<<quark2_.Pt()<<hadbquark_.Pt()<<"  "<<lepton_.Pt()<<"  "<<met_.Et()<<"   "<< ChiSquare_<<"   "<<endl;
  TLorentzVector HadTop ; 
  TLorentzVector HadW ;
  TLorentzVector LepTopTotal ; 
  TLorentzVector LepTop;
  TLorentzVector Ttbar;
  TLorentzVector LepBl;
  TLorentzVector Hadp_boosted ;
  TLorentzVector Hadq_boosted ;
  TLorentzVector HadW_boosted ;
  TLorentzVector Hadb_boosted ;
  TLorentzVector HadTop_boosted ;
  HadTop_boosted.SetPxPyPzE ( 0,0,0,0);
  Hadp_boosted.SetPxPyPzE ( 0,0,0,0);
  Hadq_boosted.SetPxPyPzE ( 0,0,0,0);  
  HadW_boosted.SetPxPyPzE ( 0,0,0,0);  
  Hadb_boosted.SetPxPyPzE ( 0,0,0,0);
  HadTop.SetPxPyPzE ( 0,0,0,0);
  HadW.SetPxPyPzE ( 0,0,0,0);

  LepTop.SetPxPyPzE ( 0,0,0,0);
  LepTopTotal.SetPxPyPzE ( 0,0,0,0);
  Ttbar.SetPxPyPzE ( 0,0,0,0);
  LepBl.SetPxPyPzE ( 0,0,0,0);
 



if (quark1_.Pt()>0 && quark2_.Pt()>0 && hadbquark_.Pt()>0 && lepbquark_.Pt()>0){

  Hadp_boosted = quark1_;
  Hadq_boosted = quark2_;
  HadW_boosted = quark1_ + quark2_;
  Hadb_boosted = hadbquark_;
  HadTop_boosted = quark1_+quark2_+hadbquark_;
  Hadp_boosted.Boost (-HadW.BoostVector ());
  Hadq_boosted.Boost (-HadW.BoostVector ());
  Hadb_boosted.Boost (-HadTop.BoostVector ());

   HadTop = (quark1_ + quark2_ + hadbquark_);
   HadW = (quark1_ + quark2_);
   LepTopTotal =  (lepbquark_ + lepton_ + met_);

   float LepTopPz = LepTopTotal.Pz ();

 // cout<<" MET  "<<met_<<"  "<<LepTop.Pz()<<"  "<<met_.Pz()<<endl;
 
  LepTopTotal.SetPz (LepTopPz + PzNeutrino_);
  
  float MassCalc = lepbquark_.M() + lepton_.M(); 
  float PxCalc = lepbquark_.Px() + lepton_.Px() + met_.Px();

  float PyCalc = lepbquark_.Py() + lepton_.Py() + met_.Py();
  float PzCalc = lepbquark_.Pz() + lepton_.Pz() + met_.Pz() + PzNeutrino_;

  LepTopTotal.SetE( sqrt(MassCalc*MassCalc + PxCalc*PxCalc + PyCalc*PyCalc + PzCalc*PzCalc ));

  LepTop.SetPxPyPzE( double(LepTopTotal.Px()), double(LepTopTotal.Py()),
			 double(LepTopTotal.Pz()), double (LepTopTotal.E()));
 
  Ttbar = (HadTop + LepTop);
  LepBl = (lepton_ + lepbquark_);
  //TLorentzVector lbPt =  (TLorentzVector) (lepton_+ FourthJetPt);
  //TLorentzVector lbBtag = (TLorentzVector) (lepton_+ FourthJetBtag);
  // if (Ttbar.Mt()<0)
  //cout<<"  Mt "<<Ttbar.Mt()<<" Mt2  "<<Ttbar.Mt2()<<" E2 "<<Ttbar.E()*Ttbar.E()<<" Z2 "<<Ttbar.Z()*Ttbar.Z()<<" E  "<<Ttbar.E()<<" Z  "<<Ttbar.Z()<<endl;


  // cout <<"  HADTOP  complex variables  "<<HadTop.M()<<" LepTop  "<<LepTop.M()<<"  "<<quark1_.Pt()<<"   "<<lepton_.Pt()<<"   "<<lepbquark_.Pt()<<"  "<<met_.Et()<<endl;
  //cout<<" MET  2  "<<LepTopPz<<"   "<<PzNeutrino_<<"  "<<LepTopTotal.Pz()<<"  "<<LepTop.Pz()<<endl;
  // Boosted jet candidates  

 
HadTop_boosted.Boost( -HadTop_boosted.BoostVector() );

//cout << HadTop_boosted.Px() << " " << HadTop_boosted.Py() << " " << HadTop_boosted.Pz() << " " << HadTop_boosted.E() <<"  "<<HadTop_boosted.Pt()<<"  "<<HadTop_boosted.M()<<"   "<<HadTop.M()<<"  "<<fabs(HadTop.M()-HadTop_boosted.M())<< endl;

}



// List of variables

/*
  if (ChiSquare_>0){

  variables_.push_back (pair < string, float >(string ("ChiSquare"), ChiSquare_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));}

else{
  variables_.push_back (pair < string, float >(string ("ChiSquare"), DefaultValue_)); 
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));}

  //ChiSquareHadTop
  //  variables_.push_back (pair < string, float >(string ("ChiSquareHadTop"), ChiSquareHadTop_));
  //RangeVariables_.push_back (pair < float, float >(0.0, 500.));
*/

  //MET
  variables_.push_back (pair < string, float >(string ("MET"), met_.Et ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  //cout<<" MET  -------------> "<<met_.Et()<<endl;
  


  //MET divived by HadTopPt
if ( HadTop.Pt()>0 ){

  variables_.push_back (pair < string, float >(string ("METOverHadTopPt"), met_.Et () / HadTop.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 50.));
  
  variables_.push_back (pair < string, float >(string ("HadTopPtOverLepBlPt"), HadTop.Pt ()/LepBl.Pt()));
  RangeVariables_.push_back (pair < float, float >(0.0, 50.));

  variables_.push_back (pair < string, float >(string ("MassBoostedTop"),  HadTop_boosted.M()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
   
  variables_.push_back (pair < string, float >(string ("BTaggerLepB"), btaggerLepB));
  RangeVariables_.push_back (pair < float, float >(0, 100.));
  
  variables_.push_back (pair < string, float >(string ("BTaggerHadB"), btaggerHadB));
  RangeVariables_.push_back (pair < float, float >(0, 100.));

 // variables_.push_back (pair < string, float >(string ("BTaggerCombInfo"), CombBtagInfo));
//  RangeVariables_.push_back (pair < float, float >(0, 100.));

}
else
{
  variables_.push_back (pair < string, float >(string ("METOverHadTopPt"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 50.));
  
  variables_.push_back (pair < string, float >(string ("HadTopPtOverLepBlPt"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 50.));
  
  variables_.push_back (pair < string, float >(string ("MassBoostedTop"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000));
  
   variables_.push_back (pair < string, float >(string ("BTaggerLepB"),  DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 50.));
   variables_.push_back (pair < string, float >(string ("BTaggerHadB"),  DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 50.));
//  variables_.push_back (pair < string, float >(string ("BTaggerCombInfo"), DefaultValue_));
//  RangeVariables_.push_back (pair < float, float >(0, 100.));

}  
  
  //HT
  //variables_.push_back(pair<string,float>(string("MyHT"),quark1_.Pt() + quark2_.Pt() + hadbquark_.Pt() + FourthJetPt.Pt()));
  
float quarkspt = quark1_.Pt () + quark2_.Pt () + hadbquark_.Pt() + lepbquark_.Pt ();
if (quarkspt>0){
  variables_.push_back (pair < string, float >(string ("Chi2JetsPt"), quark1_.Pt () + quark2_.Pt () + hadbquark_.Pt () + lepbquark_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));

  variables_.push_back (pair < string, float >(string ("Chi2JetsMETLeptPt"), quark1_.Pt () + quark2_.Pt () + hadbquark_.Pt () + lepbquark_.Pt () + met_.Et () + lepton_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));


}
else{ 
  variables_.push_back (pair < string, float >(string ("Chi2JetsPt"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
  variables_.push_back (pair < string, float >(string ("Chi2JetsMETLeptPt"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
}


  float HT = DefaultValue_;
  float HZ = DefaultValue_;
  float HTMET = DefaultValue_;
  float HT4jetsMuonMET = DefaultValue_;

  if (jets_.size () > 3)
    {
HT = 0;
      HT = jets_[0].Pt () + jets_[1].Pt () + jets_[2].Pt () + jets_[3].Pt ();
      variables_.push_back (pair < string, float >(string ("HT4jets"), HT));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
 
      HZ = 0;
      HZ = jets_[0].Pz () + jets_[1].Pz () + jets_[2].Pz () + jets_[3].Pz ();
      variables_.push_back (pair < string, float >(string ("HZ4jets"), HZ));
      RangeVariables_.push_back (pair < float, float >(-5000., 5000.));
 
      HTMET = 0;
      HTMET = jets_[0].Pt () + jets_[1].Pt () + jets_[2].Pt () + jets_[3].Pt () + met_.Et ();
      variables_.push_back (pair < string, float >(string ("HT4jetsMET"), HTMET));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      

      //variables_.push_back (pair < string, float >(string ("HOver(HT+HZ)"), HT / ( (HZ+HT)* (HZ+HT)) ));
      //RangeVariables_.push_back (pair < float, float >(-1000., 1000.));
      
      HT4jetsMuonMET = 0;
      HT4jetsMuonMET = jets_[0].Pt () + jets_[1].Pt () + jets_[2].Pt () + jets_[3].Pt () + met_.Et () + lepton_.Pt ();
      variables_.push_back (pair < string, float >(string ("HT4jetsMuonMET"), HT4jetsMuonMET));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("HT4jets"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      variables_.push_back (pair < string, float >(string ("HZ4jets"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5000., 5000.));
      
      variables_.push_back (pair < string, float >(string ("HT4jetsMET"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      //variables_.push_back (pair < string, float >(string ("HOver(HT+HZ)"), DefaultValue_));
      //RangeVariables_.push_back (pair < float, float >(-1000., 1000.));
      
      variables_.push_back (pair < string, float >(string ("HT4jetsMuonMET"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
    }



  
    

  float AllJetsPt_ = DefaultValue_;


  if (jets_.size () > 3)
    {
      AllJetsPt_ = 0;
      for (unsigned int i = 0; i < jets_.size (); i++)
	{
	  AllJetsPt_ = AllJetsPt_ + jets_[i].Pt ();
	}
 //if (AllJetsPt_>5000. cout<<" AlljetsPt is bigger than "<<AllJetsPt_<<endl;  
    variables_.push_back (pair < string, float >(string ("AllJetsPt"), AllJetsPt_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
    
      variables_.push_back (pair < string, float >(string ("NofJets"), jets_.size()));
      RangeVariables_.push_back (pair < float, float >(0.0, 30.));
      
      variables_.push_back (pair < string, float >(string ("AvrgHT"), float(AllJetsPt_/jets_.size())));
      RangeVariables_.push_back (pair < float, float >(0.0, 500.));
      
      variables_.push_back (pair < string, float >(string ("AllJetsPtMET"), AllJetsPt_ + met_.Et ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  //cout<<"  AllJetsPt  "<<AllJetsPt_<<"   "<<jets_.size()<<"  "<<float(AllJetsPt_/jets_.size())<<endl;
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("AllJetsPt"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      variables_.push_back (pair < string, float >(string ("NofJets"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 30.));
      
      variables_.push_back (pair < string, float >(string ("AvrgHT"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 500.));
      
      variables_.push_back (pair < string, float >(string ("AllJetsPtMET"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
    }




/////////4 Leading JetsPt 
  float ET12_ = DefaultValue_;
  float ET23_ = DefaultValue_;
  float ET34_ = DefaultValue_;
  float ET1oET2_ = DefaultValue_;
  float ET1oET3_ = DefaultValue_;
  float ET1oET4_ = DefaultValue_;
  float ET2oET3_ = DefaultValue_;
  
 if (jets_.size () > 3)
    {
      variables_.push_back (pair < string, float >(string ("JetsPt1"), jets_[0].Pt ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      variables_.push_back (pair < string, float >(string ("JetsPt2"), jets_[1].Pt ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      variables_.push_back (pair < string, float >(string ("JetsPt3"), jets_[2].Pt ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      variables_.push_back (pair < string, float >(string ("JetsPt4"), jets_[3].Pt ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      
///////////ET of 1rst+2nd
      ET12_ = 0;
      ET12_ = jets_[0].Pt () + jets_[1].Pt ();


      variables_.push_back (pair < string, float >(string ("ET12"), ET12_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      ET23_ = 0;
      ET23_ = jets_[1].Pt () + jets_[2].Pt ();
      variables_.push_back (pair < string, float >(string ("ET23"), ET23_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      ET34_ = 0;
      ET34_ = jets_[2].Pt () + jets_[3].Pt ();
      variables_.push_back (pair < string, float >(string ("ET34"), ET34_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      ET1oET2_ = 0;
      ET1oET2_ = jets_[0].Pt () / jets_[1].Pt ();
      variables_.push_back (pair < string, float >(string ("ET1oET2"), ET1oET2_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
      
      ET1oET3_ = 0;
      ET1oET3_ = jets_[0].Pt () / jets_[2].Pt ();
      variables_.push_back (pair < string, float >(string ("ET1oET3"), ET1oET3_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
      
      ET1oET4_ = 0;
      ET1oET4_ = jets_[0].Pt () / jets_[3].Pt ();
      variables_.push_back (pair < string, float >(string ("ET1oET4"), ET1oET4_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
      
      ET2oET3_ = 0;
      ET2oET3_ = jets_[1].Pt () / jets_[2].Pt ();
      variables_.push_back (pair < string, float >(string ("ET2oET3"), ET2oET3_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("JetsPt1"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      variables_.push_back (pair < string, float >(string ("JetsPt2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      variables_.push_back (pair < string, float >(string ("JetsPt3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      variables_.push_back (pair < string, float >(string ("JetsPt4"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      variables_.push_back (pair < string, float >(string ("ET12"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      variables_.push_back (pair < string, float >(string ("ET23"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      variables_.push_back (pair < string, float >(string ("ET34"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
      
      variables_.push_back (pair < string, float >(string ("ET1oET2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
      
      variables_.push_back (pair < string, float >(string ("ET1oET3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
      
      variables_.push_back (pair < string, float >(string ("ET1oET4"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
      
      variables_.push_back (pair < string, float >(string ("ET2oET3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 200.));
    }





  //Mass transverse
  if (Ttbar.Mt() >0){
  variables_.push_back (pair < string, float >(string ("Ttbar.Mt"), Ttbar.Mt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
 // if (Ttbar.Mt()<0)
//  cout<<Ttbar.Mt()<<endl;
}
  else
    {

 variables_.push_back (pair < string, float >(string ("Ttbar.Mt"),DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  }
  
   //float dphi = Ttbar.Phi() - met_.Phi();
   // cout<<dphi<<endl;
   float numb=0;numb= 1- TMath::Cos(   ROOT::Math::VectorUtil::DeltaPhi (Ttbar, met_));
   //cout<<sqrt(2*Ttbar.Pt()*met_.Et()* (1-TMath::Cos(numb)))<<endl;;
    float trans =0;trans= sqrt(2* Ttbar.Pt() * met_.Et() *(numb));

   float numb2=0;numb2= 1- TMath::Cos(   ROOT::Math::VectorUtil::DeltaPhi (lepton_, met_));
    float trans2 = 0;trans2=sqrt(2* lepton_.Pt() * met_.Et() * numb2);

 //if (trans<0)
   //cout<<"    Wtr "<<trans2<<" 1-cos  "<<numb2<<" l_pt  "<<lepton_.Pt()<<" met "<<met_.Et()<<" cos "<<  ROOT::Math::VectorUtil::DeltaPhi (lepton_, met_)<<endl;
  if (trans>0){
    variables_.push_back (pair < string, float >(string ("TransMassTtbar"), trans));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  }
  else
    {

 variables_.push_back (pair < string, float >(string ("TransMassTtbar"),DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  }
  if (trans2>0){
    variables_.push_back (pair < string, float >(string ("Wtrans"), trans2));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
  }
  else
    {

 variables_.push_back (pair < string, float >(string ("Wtrans"),DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
  }
  
  //Mtt
  if (Ttbar.M()>0 &&  LepTop.M () && HadTop.M()>0){
// if (Ttbar.M()>0)
  //cout<<"  Ttbar.M "<<Ttbar.M()<<endl;
  variables_.push_back (pair < string, float >(string ("MassTtbar"), Ttbar.M ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
  variables_.push_back (pair < string, float >(string ("MassLepTop"), LepTop.M ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
  variables_.push_back (pair < string, float >(string ("TransMassLepTop"), LepTop.Mt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
  variables_.push_back (pair < string, float >(string ("TransMassHadTop"), HadTop.Mt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));

  variables_.push_back (pair < string, float >(string ("EtaHadTop"), HadTop.Eta ()));
  RangeVariables_.push_back (pair < float, float >(-5., 5.));
  
  }

  else {
  variables_.push_back (pair < string, float >(string ("MassTtbar"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
    variables_.push_back (pair < string, float >(string ("MassLepTop"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
  variables_.push_back (pair < string, float >(string ("TransMassLepTop"),  DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
 variables_.push_back (pair < string, float >(string ("TransMassHadTop"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
  
  variables_.push_back (pair < string, float >(string ("EtaHadTop"),DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(-5., 5.));
  
  }


  

  //PtMuon 
 // if (lepton_.Pt ()>10)
  variables_.push_back (pair < string, float >(string ("PtMuon"), lepton_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 500.));


  //EtaMuon


    float eta =-log(tan(lepton_.Theta()/2.));

  //if ( lepton_.Pt()>0 )
  variables_.push_back (pair < string, float >(string ("EtaMuon"), eta));
  //variables_.push_back (pair < string, float >(string ("EtaMuon"), lepton_.Eta()));
  RangeVariables_.push_back (pair < float, float >(-5., 5.));
  // cout<<eta<<endl;
  


/*
  //Pt of jet (!) of quark 1 (from W decay)
  if (quark1_.Pt ()>0){
  variables_.push_back (pair < string, float >(string ("PtQuark1"), quark1_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
  } 
else {
  variables_.push_back (pair < string, float >(string ("PtQuark1"), -1));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
  } 
if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("PtQuark1"), quark1MC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0.0, 1000.));
    }
  //Pt of jet (!) of quark 2 (from W decay)
  if (quark2_.Pt ()>0){
  variables_.push_back (pair < string, float >(string ("PtQuark2"), quark2_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
  }
else {
  variables_.push_back (pair < string, float >(string ("PtQuark2"), -1));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
}
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("PtQuark2"), quark2MC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0.0, 1000.));
    }
  if (hadbquark_.Pt ()>10){
  //Pt of jet (!) of hadronic b quark (from had top)
  variables_.push_back (pair < string, float >(string ("PtHadbquark"), hadbquark_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
  }
	else{
  variables_.push_back (pair < string, float >(string ("PtHadbquark"),  DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
	}
  
  //Pt of jet (!) of leptonic b quark (from lep top)
  if (lepbquark_.Pt ()>10){
  variables_.push_back (pair < string, float >(string ("PtLepbquark"), lepbquark_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0.0, 1000.));
  }
else{
  variables_.push_back (pair < string, float >(string ("PtLepbquark"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(10., 1000.));
  }

*/

  //DeltaPhiMetJet1
  
    
   if (jets_.size()>3  )
    {
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetMuon"),  ROOT::Math::VectorUtil::DeltaPhi (met_, lepton_)));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetJet1"), ROOT::Math::VectorUtil::DeltaPhi (met_, jets_[0])));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRMetJet1"), ROOT::Math::VectorUtil::DeltaR (met_, jets_[0])));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetJet2"), ROOT::Math::VectorUtil::DeltaPhi (met_, jets_[1])));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRMetJet2"), ROOT::Math::VectorUtil::DeltaR (met_, jets_[1])));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRLepJet1"), ROOT::Math::VectorUtil::DeltaR (lepton_, jets_[0])));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiLepJet1"), ROOT::Math::VectorUtil::DeltaPhi (lepton_, jets_[0])));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      
      variables_.push_back (pair < string, float >(string ("DeltaRLepJet2"), ROOT::Math::VectorUtil::DeltaR (lepton_, jets_[1])));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiLepJet2"), ROOT::Math::VectorUtil::DeltaPhi (lepton_, jets_[1])));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
    
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet2"),  ROOT::Math::VectorUtil::DeltaR (jets_[0], jets_[1])));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiJet1Jet2"),  ROOT::Math::VectorUtil::DeltaPhi (jets_[0], jets_[1])));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet3"), ROOT::Math::VectorUtil::DeltaR (jets_[0], jets_[1])));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiJet1Jet3"), ROOT::Math::VectorUtil::DeltaPhi (jets_[0], jets_[1])));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      variables_.push_back (pair < string, float >(string ("DeltaRHadtopLepTop"), ROOT::Math::VectorUtil::DeltaR (HadTop, LepTop)));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiHadtopLepTop"), ROOT::Math::VectorUtil::DeltaPhi (HadTop, LepTop)));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMuHadtop"), ROOT::Math::VectorUtil::DeltaPhi (lepton_, HadTop)));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRMuHadtop"), ROOT::Math::VectorUtil::DeltaR (lepton_, HadTop)));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMETHadtop"), ROOT::Math::VectorUtil::DeltaPhi (met_, HadTop)));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRMETHadtop"), ROOT::Math::VectorUtil::DeltaR (met_, HadTop)));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
      
   //   variables_.push_back (pair < string, float >(string ("DeltaRChi2BTagJets"), ROOT::Math::VectorUtil::DeltaR (hadbquark_, lepbquark_)));
  //    RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("MLepBl"), LepBl.M ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 500.));

      }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetMuon"),  DefaultValue_));//(met_.Phi () - lepton_.Phi ())));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetJet1"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRMetJet1"),  DefaultValue_));// (met_.Phi () - jets_[0].Phi ())));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetJet2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
      
      
      variables_.push_back (pair < string, float >(string ("DeltaRMetJet2"),  DefaultValue_));// (met_.Phi () - jets_[0].Phi ())));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRLepJet1"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiLepJet1"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      
      variables_.push_back (pair < string, float >(string ("DeltaRLepJet2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiLepJet2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiJet1Jet2"),   DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiJet1Jet3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      variables_.push_back (pair < string, float >(string ("DeltaRHadtopLepTop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiHadtopLepTop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
  //    variables_.push_back (pair < string, float >(string ("DeltaRChi2BTagJets"), DefaultValue_));
 //     RangeVariables_.push_back (pair < float, float >(0.0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMuHadtop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRMuHadtop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaPhiMETHadtop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5, 5.));
      
      variables_.push_back (pair < string, float >(string ("DeltaRMETHadtop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0, 5.));
  
      variables_.push_back (pair < string, float >(string ("MLepBl"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 500.));
      }




  
  float SumDeltaPhiMuJet12_ = DefaultValue_;
  if ( jets_.size()>1 && jets_[0].Pt()>0 &&  jets_[1].Pt()>0 && lepton_.Pt()>0)   SumDeltaPhiMuJet12_=ROOT::Math::VectorUtil::DeltaPhi (lepton_, jets_[0])+   ROOT::Math::VectorUtil::DeltaPhi (lepton_, jets_[1] );
// if ( jets_.size()>2 && jets_[0].Pt()>0 &&  jets_[1].Pt()>0 && lepton_.Pt()>0)   var= jets_[0].Pt()+jets_[1].Pt();
//cout<<var<<"   "<<SumDeltaPhiMuJet12_<<endl;
  
    if ( SumDeltaPhiMuJet12_> DefaultValue_)
    {
      variables_.push_back (pair < string, float >(string ("SumDeltaPhiMuJet12"),  SumDeltaPhiMuJet12_  ));// (lepton_.Phi () - jets_[0].Phi ()) + (lepton_.Phi () - jets_[1].Phi ())));

      // cout<<SumDeltaPhiMuJet12_<<endl;
      RangeVariables_.push_back (pair < float, float >(-10., 10.));
    }


  else
    {
      variables_.push_back (pair < string, float >(string ("SumDeltaPhiMuJet12"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-10., 10.));
    }



  //M3
  float Max = 0.;
  float M3 = 0.;
	//    TLorentzVector v ;
	  //  v.SetPxPyPzE ( 0,0,0,0);
  if (jets_.size () > 2)
    for (unsigned int i = 0; i < jets_.size (); i++)
      for (unsigned int j = i + 1; j < jets_.size (); j++)
	for (unsigned int k = j + 1; k < jets_.size (); k++)
	  {
	     TLorentzVector v = jets_[i] + jets_[j] + jets_[k];
	    if (v.Pt () > Max)
	      {
		Max = v.Pt ();
		M3 = v.M ();
	      }
	  }
if (M3>0){
  variables_.push_back (pair < string, float >(string ("M3"), M3));
 //cout<<" M3  "<<M3<<endl;
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
}
else{

  variables_.push_back (pair < string, float >(string ("M3"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
}
/*
  

  //Var is of muon
  //variables_.push_back(pair<string,float>(string("d0Sig"),(lepton_.d0error() > 0 ? fabs(lepton_.d0())/lepton_.d0error() : -1 )));
  //RangeVariables_.push_back(pair<float,float>(0.0,30.));
  variables_.push_back (pair < string, float >(string ("RelCaloIso"), (lepton_.Pt () > 0 ? (lepton_.isoR03_emEt () + lepton_.isoR03_hadEt ()) / lepton_.Pt () : -1)));
  RangeVariables_.push_back (pair < float, float >(0.0, 10.));

  variables_.push_back (pair < string, float >(string ("RelTrackIso"), (lepton_.Pt () > 0 ? (lepton_.isoR03_sumPt ()) / lepton_.Pt () : -1)));
  RangeVariables_.push_back (pair < float, float >(0.0, 10.));
  //hemisphere


if (Hadp_boosted.Pt ()>0){
      variables_.push_back (pair < string, float >(string ("HadqPtWRF"), Hadp_boosted.Pt ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
 }
else{
      variables_.push_back (pair < string, float >(string ("HadqPtWRF"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
 }




      // Hadronic b jet pt in hadronic top candidate rest frame
     if ( Hadb_boosted.Pt ()>0){
      variables_.push_back (pair < string, float >(string ("HadbPtHadTopRF"), Hadb_boosted.Pt ()));
      RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
     }
else{
      variables_.push_back (pair < string, float >(string ("HadbPtHadTopRF"), DefaultValue_));
            RangeVariables_.push_back (pair < float, float >(0.0, 5000.));
     }


*/ 
// Build-up a vector with all jets and the selected lepton
  vector < TLorentzVector > AllJets_Lepton_noMet_;
  for (unsigned int i = 0; i < jets_.size (); i++){
    AllJets_Lepton_noMet_.push_back (jets_[i]);}
    AllJets_Lepton_noMet_.push_back (lepton_);

  // Centrality
  float Centrality = 0, Centrality_N = 0, Centrality_D = 0;
  for (unsigned int i = 0; i < AllJets_Lepton_noMet_.size (); i++)
    {
      Centrality_N += AllJets_Lepton_noMet_[i].Pt ();
      Centrality_D += AllJets_Lepton_noMet_[i].P ();
    }
  (Centrality_D > 0 ? Centrality = Centrality_N / Centrality_D : -1);
  variables_.push_back (pair < string, float >(string ("Centrality"), Centrality));
  RangeVariables_.push_back (pair < float, float >(0.0, 2.));


  // Build-up a vector with top jet candidates, the selected lepton and the missing transverse energy. The pz of the latter is calculated.
  /*
  if (quark1_.Pt()>0 && quark2_.Pt()>0 && hadbquark_.Pt()>0 && lepbquark_.Pt()>0){
  vector < TLorentzVector > TtbarSystem_;
  TtbarSystem_.push_back (quark1_);
  TtbarSystem_.push_back (quark2_);
  TtbarSystem_.push_back (hadbquark_);
  TtbarSystem_.push_back (lepbquark_);
  TtbarSystem_.push_back (lepton_);
  TtbarSystem_.push_back (met_);
  TtbarSystem_[5].SetPz (PzNeutrino_);

  for (unsigned int i = 0; i < TtbarSystem_.size (); i++)
    {
      TtbarSystem_[i].Boost (-Ttbar.BoostVector ());
    }


float  CentralityTT = 0, Centrality_NTT = 0, Centrality_DTT = 0;
  for (unsigned int i = 0; i < TtbarSystem_.size (); i++)
    {
      Centrality_NTT += TtbarSystem_[i].Pt ();
      Centrality_DTT += TtbarSystem_[i].P ();
    }
  (Centrality_DTT > 0 ? CentralityTT = Centrality_NTT / Centrality_DTT : -1);
  
  variables_.push_back (pair < string, float >(string ("CentralityTtbar"), CentralityTT));
  RangeVariables_.push_back (pair < float, float >(0.0, 2.));

  }
  */


  //Compute the matrice for
  Hadp_boosted.SetPxPyPzE ( 0,0,0,0);
  Hadq_boosted.SetPxPyPzE ( 0,0,0,0);  
  HadW_boosted.SetPxPyPzE ( 0,0,0,0);  
  Hadb_boosted.SetPxPyPzE ( 0,0,0,0);
  HadTop.SetPxPyPzE ( 0,0,0,0);
  HadW.SetPxPyPzE ( 0,0,0,0);

  LepTop.SetPxPyPzE ( 0,0,0,0);
  LepTopTotal.SetPxPyPzE ( 0,0,0,0);
  Ttbar.SetPxPyPzE ( 0,0,0,0);
  LepBl.SetPxPyPzE ( 0,0,0,0);

}




void Observables::setEventscaleFactor(float scaleFactor){
//not an observable, but a scale factor (weight) for an event... Is put in the Observables class for convenience code-wise... (but is not very logical from physics point of view)
//!!!Take care of fact that ObservablesRanker might be screwed up by adding this!!!!
 variables_.push_back (pair < string, float >(string ("scaleFactor"), scaleFactor));
 RangeVariables_.push_back (pair < float, float >(-1., 10.)); //doesn't matter, will not be used as a real observable...
}


void
Observables::unsetMC ()
{
  this->MCavailable_ = false;
}

bool
Observables::isModeMC ()
{
  return this->MCavailable_;
}





void
Observables::ComputeVariables (const TRootMuon & lepton, const TRootJet & quark1, const TRootJet & quark2, const TRootJet & hadbquark, const TRootJet & lepbquark, const vector < TRootJet > &Seljets, const TRootMET & selmets, string & dss,float &ChiSq_)
{
  // cout<<"Start computing variables............................PSEUDO EXPERIMENT for ."<<setname<<endl;


  lepton_ = lepton;
  quark1_ = quark1;
  quark2_ = quark2;
  hadbquark_ = hadbquark;
  lepbquark_ = lepbquark;


  jets_.clear ();
  jets_ = Seljets;
    //  SetJets (jets_);
    //  SetMET (selmets);
  met_ = selmets;
  SetChiSquare (ChiSq_);


 ComputeVariables();




}




pair < float, float >
Observables::Range (const string label)
{
  for (unsigned int i = 0; i < variables_.size (); i++)
    {
      if (label == variables_[i].first)
	{
	  return RangeVariables_[i];
	}
    }
  return pair < float, float >(-66666., -66666.);
}

pair < float, float >
Observables::Range (const char *label)
{
  for (unsigned int i = 0; i < variables_.size (); i++)
    {
      if (!strcmp (label, variables_[i].first.c_str ()))
	{
	  return RangeVariables_[i];
	}
    }
  return pair < float, float >(-66666., -66666.);
}

float
Observables::Variable (const string label)
{
  for (unsigned int i = 0; i < variables_.size (); i++)
    {
      if (label == variables_[i].first)
	{
	  return variables_[i].second;
	}
    }
  return -66666.;
}


float
Observables::Variable (const char *label)
{
  for (unsigned int i = 0; i < variables_.size (); i++)
    if (!strcmp (label, variables_[i].first.c_str ()))
      return variables_[i].second;

  return -66666.;
}



float
Observables::VariableMC (const string label)
{
  for (unsigned int i = 0; i < variablesMC_.size (); i++)
    {
      if (label == variablesMC_[i].first)
	{
	  return variablesMC_[i].second;
	}
    }
  return -66666.;
}

float
Observables::VariableMC (const char *label)
{
  for (unsigned int i = 0; i < variablesMC_.size (); i++)
    if (!strcmp (label, variablesMC_[i].first.c_str ()))
      return variablesMC_[i].second;

  return -66666.;
}



vector < string > Observables::ListOfVariables ()
{
  vector < string > out;
  for (unsigned int i = 0; i < variables_.size (); i++)
    out.push_back (variables_[i].first);
  return out;
}

vector < string > Observables::ListOfVariablesMC ()
{
  vector < string > out;
  for (unsigned int i = 0; i < variablesMC_.size (); i++)
    out.push_back (variablesMC_[i].first);
  return out;
}

/* Observables for MC need to be filled in the same order as the RECO observables (even if not available for all) */
vector < string > Observables::ListOfVariablesCoupled ()
{
  vector < string > out;
  bool
    stop = false;
  size_t
    indexMC = 0;
  for (vector < pair < string, float > >::iterator iter = variables_.begin (); iter != variables_.end (); iter++)
    {
      while (stop == false && strcmp (iter->first.c_str (), variablesMC_[indexMC].first.c_str ()))
	{
	  indexMC++;
	  if (indexMC == variablesMC_.size ())
	    {
	      stop = true;
	    }
	}
      if (stop)
	{
	  indexMC = 0;
	  stop = false;
	  continue;
	}
      out.push_back (iter->first);
    }
  return out;
}




