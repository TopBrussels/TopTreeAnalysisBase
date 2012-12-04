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
  /*jets_.push_back(quark1_);
  jets_.push_back(quark2_);
  jets_.push_back(hadbquark_);
  jets_.push_back(lepbquark_);
  */

  
  //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = 0.;
  ChiSquareHadTop_ = 0.;
  
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
  jets_.clear ();
  //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  ChiSquare_ = 0.;
  ChiSquareHadTop_ = 0.;
  
  ComputeVariables();
  
  delete NuPzCalc;

}

Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const TRootMET & MET, const vector < TRootJet > &jets)
{
  jets_.clear();
  (*this) = Observables (lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, MET);
    //  SetJets (jets);
  jets_ = jets;
}

Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const TRootMET & MET, const vector < TRootJet* > &jets,float & ChiSq_)
{
  ChiSquare_ = 0.;
  ChiSquare_=ChiSq_;
  (*this) = Observables (lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, MET);
    //  SetJets (jets);
  jets_.clear();
  for (unsigned int i=0; i< jets.size();i++) {
	  jets_.push_back(*jets[i]);
//cout<<"  Jetsss "<<jets_[i].Pt()<<"  "<<jets[i]->Pt()<<" i  "<<i<<endl;
  }
//   jets_ = jets;
}



Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromTop, const TRootMET & MET, const vector < TRootJet > &jets)
{
  jets_.clear();
  (*this) = Observables (lepton, qFromW1, qFromW2, bFromTop, MET);
    //SetJets (jets);
  jets_ = jets;
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

  jets_.clear();
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
  
  jets_.clear ();
  jets_ = jets;
    //  SetJets(jets);
 
 //hemisphere_ = pat::Hemisphere();
  MEzCalculator *NuPzCalc = new MEzCalculator ();
  NuPzCalc->SetMuon (lepton_);
  NuPzCalc->SetMET (MET);
  PzNeutrino_ = NuPzCalc->Calculate ();
  //ChiSquare_ = 0.;
  ChiSquareHadTop_ = 0.;
  //cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<<chijets_[0].Pt()<< "  "<<chijets_[1].Pt()<<"  "<<chijets_[2].Pt()<<"  "<<chijets_[3].Pt()<<endl; 

  ComputeVariables();
  
  delete NuPzCalc;

}






Observables::Observables (const TRootMuon & lepton, const vector < TRootJet > &jets, const TRootMET & MET, string & setname)
{
  //cout<<"  JUST EENTERED THE COMPLEX  "<<endl;

  //cout<<"  parameters "<<lepton_.Pt()<<"  "<<quark1_.Pt()<<"  "<<quark2_.Pt()<<"    "<<jets_[0].Pt()<<endl; 

  //cout<<"   parameters_should_match_above "<<lepton_.Pt()<<  "  "<<chijets_[0].Pt()<< "  "<<chijets_[1].Pt()<<"  "<<chijets_[2].Pt()<<"  "<<chijets_[3].Pt()<<endl; 

  lepton_ = TRootJet ();
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

 
  ComputeVariables ();


  delete NuPzCalc;
}



Observables::Observables (const TRootMuon & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const vector < TRootJet > &jets, const TRootMET & MET)
{

  lepton_ = TRootJet ();
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
  //  cout << "Start computing variables............................ " << endl;



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
 


  //Search the fourth jets (shouldn't be part of the class Observable)
  /*
     std::vector<TRootJet> remainingJets = jets_;
     std::vector<TRootJet> selectedJets;
     selectedJets.push_back(quark1_);
     selectedJets.push_back(quark2_);
     selectedJets.push_back(bquark_);
     for(unsigned int i=0;i<selectedJets.size();i++){
     for(unsigned int j=0;j<remainingJets.size();j++){
     if(selectedJets[i] == remainingJets[j]) remainingJets.erase(remainingJets.begin()+j);
     }
     }
     if((remainingJets.size()+selectedJets.size())!=jets_.size()) cerr<<"Problem when computing the remaining jets"<<endl;
     std::sort(remainingJets.begin(),remainingJets.end(),HighestPt());
     TRootJet FourthJetPt;
     if(remainingJets.size()>0) FourthJetPt = remainingJets[0];
     std::sort(remainingJets.begin(),remainingJets.end(),HighestBtag());
     TRootJet FourthJetBtag;
     if(remainingJets.size()>0) FourthJetBtag = remainingJets[0];

     bool b4jets = false;
     if(jets_.size()>3) b4jets = true;
   */
 


   HadTop = (quark1_ + quark2_ + hadbquark_);
   HadW = (quark1_ + quark2_);
   LepTopTotal =  (lepbquark_ + lepton_ + met_);

   float LepTopPz = LepTopTotal.Pz ();

  //cout<<" MET  "<<met_<<"  "<<LepTop.Pz()<<"  "<<met_.Pz()<<endl;
 
  LepTopTotal.SetPz (LepTopPz + PzNeutrino_);
  //cout<<LepTopTotal<<endl;

  //cout<<" MET  1  "<<LepTopPz<<"    "<<PzNeutrino_<<"  "<<LepTopTotal.Pz()<<endl;

  
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

 

  Hadp_boosted = quark1_;
  Hadq_boosted = quark2_;
  HadW_boosted = quark1_ + quark2_;
  Hadb_boosted = hadbquark_;
  //TLorentzVector Lepb_boosted =

  Hadp_boosted.Boost (-HadW.BoostVector ());
  Hadq_boosted.Boost (-HadW.BoostVector ());
  Hadb_boosted.Boost (-HadTop.BoostVector ());


  TLorentzVector HadTopMC, HadWMC, LepTopMC, LepTopTotalMC,TtbarMC, LepBlMC;
   HadTopMC.SetPxPyPzE ( 0,0,0,0); HadWMC.SetPxPyPzE ( 0,0,0,0); LepTopMC.SetPxPyPzE ( 0,0,0,0); LepTopTotalMC.SetPxPyPzE ( 0,0,0,0);TtbarMC.SetPxPyPzE ( 0,0,0,0); LepBlMC.SetPxPyPzE ( 0,0,0,0);
  float LepTopPzMC = 0.;
  TLorentzVector Hadp_boostedMC, Hadq_boostedMC, HadW_boostedMC, Hadb_boostedMC ;
  if (MCavailable_)
    {
      HadTopMC = (TLorentzVector) (quark1MC_ + quark2MC_ + hadbquarkMC_);
      HadWMC = (TLorentzVector) (quark1MC_ + quark2MC_);
      // LepTopMC = (TLorentzVector) (lepbquarkMC_ + leptonMC_ + metMC_);
      LepTopTotalMC = (TLorentzVector) (lepbquarkMC_ + leptonMC_ + metMC_);
      LepTopPzMC = LepTopMC.Pz ();
      LepTopTotalMC.SetPz (LepTopPzMC + PzNeutrinoMC_);
      TtbarMC = (TLorentzVector) (HadTopMC + LepTopMC);
      LepBlMC = (TLorentzVector) (leptonMC_ + lepbquarkMC_);
      


      LepTopMC.SetPxPyPzE ( 0,0,0,0);

  float MassCalcMC = lepbquarkMC_.M() + leptonMC_.M();  
  float PxCalcMC = lepbquarkMC_.Px() + leptonMC_.Px() + metMC_.Px();

 float PyCalcMC = lepbquarkMC_.Py() + leptonMC_.Py() + metMC_.Py();
 float PzCalcMC = lepbquarkMC_.Pz() + leptonMC_.Pz() + metMC_.Pz() + PzNeutrinoMC_;

 LepTopTotalMC.SetE( sqrt(MassCalcMC*MassCalcMC + PxCalcMC*PxCalcMC + PyCalcMC*PyCalcMC + PzCalcMC*PzCalcMC ));

 LepTopMC.SetPxPyPzE( double(LepTopTotalMC.Px()), double(LepTopTotalMC.Py()),
			 double(LepTopTotalMC.Pz()), double (LepTopTotalMC.E()));
 
 
      Hadp_boostedMC = quark1MC_;
      Hadq_boostedMC = quark2MC_;
      HadW_boostedMC = quark1MC_ + quark2MC_;
      Hadb_boostedMC = hadbquarkMC_;
      Hadp_boostedMC.Boost (-HadWMC.BoostVector ());
      Hadq_boostedMC.Boost (-HadWMC.BoostVector ());
      Hadb_boostedMC.Boost (-HadTopMC.BoostVector ());
      

    }





// List of variables

    // MCavailable_ = false;
if (ChiSquare_>0){

  variables_.push_back (pair < string, float >(string ("ChiSquare"), ChiSquare_));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));}

else{
  variables_.push_back (pair < string, float >(string ("ChiSquare"), DefaultValue_)); 
  RangeVariables_.push_back (pair < float, float >(0., 5000.));}

  //ChiSquareHadTop
  //  variables_.push_back (pair < string, float >(string ("ChiSquareHadTop"), ChiSquareHadTop_));
  //RangeVariables_.push_back (pair < float, float >(0., 500.));


  //MET
  variables_.push_back (pair < string, float >(string ("MET"), met_.Et ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));
  //cout<<" MET  -------------> "<<met_.Et()<<endl;
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("MET"), metMC_.Et ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }
  

  //MET divived by HadTopPt
if ( met_.Et()/HadTop.Pt()>0 && met_.Et()/HadTop.Pt()<200){
  variables_.push_back (pair < string, float >(string ("METDivHadTopPt"), met_.Et () / HadTop.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 200.));}
else
{
  variables_.push_back (pair < string, float >(string ("METDivHadTopPt"), DefaultValue_));
  
  RangeVariables_.push_back (pair < float, float >(0., 200.));}
  
if (MCavailable_ && HadTopMC.Pt()!=0)
    {
      variablesMC_.push_back (pair < string, float >(string ("METDivHadTopPt"), metMC_.Et () / HadTopMC.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 200.));
    }
  
  
  //HT
  //variables_.push_back(pair<string,float>(string("MyHT"),quark1_.Pt() + quark2_.Pt() + hadbquark_.Pt() + FourthJetPt.Pt()));
  variables_.push_back (pair < string, float >(string ("4QuarksPt"), quark1_.Pt () + quark2_.Pt () + hadbquark_.Pt () + lepbquark_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));
 

 
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("4QuarksPt"), quark1MC_.Pt () + quark2MC_.Pt () + hadbquarkMC_.Pt () + lepbquarkMC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }

  variables_.push_back (pair < string, float >(string ("4QuarksPtMET"), quark1_.Pt () + quark2_.Pt () + hadbquark_.Pt () + lepbquark_.Pt () + met_.Et () + lepton_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));



  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("4QuarksPtMET"), quark1MC_.Pt () + quark2MC_.Pt () + hadbquarkMC_.Pt () + lepbquarkMC_.Pt () + metMC_.Et () + leptonMC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }

  float HT = DefaultValue_;
  float HZ = DefaultValue_;
  float HTMET = DefaultValue_;
  float HTMC = DefaultValue_;
  float HZMC = DefaultValue_;
  float HTMETMC = DefaultValue_;

  if (jets_.size () > 3)
    {
      HT = 0;
      HT = jets_[0].Pt () + jets_[1].Pt () + jets_[2].Pt () + jets_[3].Pt ();
      variables_.push_back (pair < string, float >(string ("HT4jets"), HT));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("HT4jets"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  HTMC = 0;
	  HTMC = jetsMC_[0].Pt () + jetsMC_[1].Pt () + jetsMC_[2].Pt () + jetsMC_[3].Pt ();
	  variablesMC_.push_back (pair < string, float >(string ("HT4jets"), HTMC));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("HT4jets"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

    }


  if (jets_.size () > 3)
    {
      
      HZ = 0;
      HZ = jets_[0].Pz () + jets_[1].Pz () + jets_[2].Pz () + jets_[3].Pz ();
      variables_.push_back (pair < string, float >(string ("HZ4jets"), HZ));
      RangeVariables_.push_back (pair < float, float >(-5000., 5000.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("HZ4jets"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5000., 5000.));
    }

  if (MCavailable_)
    {
      if (jetsMC_.size () > 3)
	{
	  HZMC = 0;
	  HZMC = jetsMC_[0].Pz () + jetsMC_[1].Pz () + jetsMC_[2].Pz () + jetsMC_[3].Pz ();
	  variablesMC_.push_back (pair < string, float >(string ("HZ4jets"), HZMC));
    RangeVariablesMC_.push_back (pair < float, float >(-5000., 5000.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("HZ4jets"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(-5000., 5000.));
	}

    }


  if (jets_.size () > 3 && HZ!=0 && HT!=0)
    {
      variables_.push_back (pair < string, float >(string ("HTo(HT+HZ)"), HT / ( (HZ+HT)* (HZ+HT)) ));
      RangeVariables_.push_back (pair < float, float >(-1000., 1000.));
    }
else
    {
      variables_.push_back (pair < string, float >(string ("HTo(HT+HZ)"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-1000., 1000.));
    }

  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  variablesMC_.push_back (pair < string, float >(string ("HTo(HT+HZ)"), HTMC / (HZMC+HTMC)));
    RangeVariablesMC_.push_back (pair < float, float >(-100., 100.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("HTo(HT+HZ)"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(-100., 100.));
	}
    }


  
  if (jets_.size () > 3)
    {
      HTMET = 0;
      HTMET = jets_[0].Pt () + jets_[1].Pt () + jets_[2].Pt () + jets_[3].Pt () + met_.Et ();
      variables_.push_back (pair < string, float >(string ("HT4jetsMET"), HTMET));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

  else
    {

      variables_.push_back (pair < string, float >(string ("HT4jetsMET"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }


  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  HTMETMC = 0;
	  HTMETMC = jetsMC_[0].Pt () + jetsMC_[1].Pt () + jetsMC_[2].Pt () + jetsMC_[3].Pt () + metMC_.Et ();
	  variablesMC_.push_back (pair < string, float >(string ("HT4jetsMET"), HTMETMC));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

      else
	{

	  variablesMC_.push_back (pair < string, float >(string ("HT4jetsMET"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

    }
   
  float HT4jetsMuonMET = DefaultValue_;
  if (jets_.size () > 3)
    {
      HT4jetsMuonMET = 0;
      HT4jetsMuonMET = jets_[0].Pt () + jets_[1].Pt () + jets_[2].Pt () + jets_[3].Pt () + met_.Et () + lepton_.Pt ();
      variables_.push_back (pair < string, float >(string ("HT4jetsMuonMET"), HT4jetsMuonMET));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

  else
    {

      variables_.push_back (pair < string, float >(string ("HT4jetsMuonMET"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }
    

  float AllJetsPt_ = DefaultValue_;
  float AllJetsPtMC_ = DefaultValue_;


  if (jets_.size () > 3)
    {
      AllJetsPt_ = 0;
      for (unsigned int i = 0; i < jets_.size (); i++)
	{
	  AllJetsPt_ = AllJetsPt_ + jets_[i].Pt ();
	}

 //if (AllJetsPt_>5000. cout<<" AlljetsPt is bigger than "<<AllJetsPt_<<endl;  
    variables_.push_back (pair < string, float >(string ("AllJetsPt"), AllJetsPt_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("AllJetsPt"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

  if (MCavailable_)
    {


      if (jetsMC_.size () > 3)
	{
	  AllJetsPtMC_ = 0;
	  for (unsigned int i = 0; i < jetsMC_.size (); i++)
	    {
	      AllJetsPtMC_ = AllJetsPtMC_ + jetsMC_[i].Pt ();
	    }
	  variablesMC_.push_back (pair < string, float >(string ("AllJetsPt"), AllJetsPtMC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("AllJetsPt"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}
    }




  float AllJetsPtMET_ = DefaultValue_;
  float AllJetsPtMETMC_ = DefaultValue_;


  if (jets_.size () > 3)
    {
      AllJetsPtMET_ = 0;
      for (unsigned int i = 0; i < jets_.size (); i++)
	{
	  AllJetsPtMET_ = AllJetsPtMET_ + jets_[i].Pt ();
	}
      variables_.push_back (pair < string, float >(string ("AllJetsPtMET"), AllJetsPtMET_ + met_.Et ()));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("AllJetsPtMET"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }


  if (MCavailable_)
    {


      if (jetsMC_.size () > 3)
	{
	  AllJetsPtMETMC_ = 0;
	  for (unsigned int i = 0; i < jetsMC_.size (); i++)
	    {
	      AllJetsPtMETMC_ = AllJetsPtMETMC_ + jetsMC_[i].Pt ();
	    }
	  variablesMC_.push_back (pair < string, float >(string ("AllJetsPtMET"), AllJetsPtMETMC_ + metMC_.Et ()));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("AllJetsPtMET"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

    }


  float ET12_ = DefaultValue_;
  float ET12MC_ = DefaultValue_;


  if (jets_.size () > 3)
    {
      ET12_ = 0;
      ET12_ = jets_[0].Pt () + jets_[1].Pt ();


      variables_.push_back (pair < string, float >(string ("ET12"), ET12_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("ET12"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }
  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  ET12MC_ = 0;
	  ET12MC_ = jetsMC_[0].Pt () + jetsMC_[1].Pt ();


	  variablesMC_.push_back (pair < string, float >(string ("ET12"), ET12MC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("ET12"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

    }


  float ET23_ = DefaultValue_;
  float ET23MC_ = DefaultValue_;

  
  if (jets_.size () > 3)
    {
      ET23_ = 0;
      ET23_ = jets_[1].Pt () + jets_[2].Pt ();
      variables_.push_back (pair < string, float >(string ("ET23"), ET23_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("ET23"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }


  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  ET23MC_ = 0;
	  ET23MC_ = jetsMC_[1].Pt () + jetsMC_[2].Pt ();


	  variablesMC_.push_back (pair < string, float >(string ("ET23"), ET23MC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("ET23"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

    }



  float ET34_ = DefaultValue_;
  float ET34MC_ = DefaultValue_;

  if (jets_.size () > 3)
    {
      ET34_ = 0;
      ET34_ = jets_[2].Pt () + jets_[3].Pt ();
      variables_.push_back (pair < string, float >(string ("ET34"), ET34_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("ET34"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }


  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  ET34MC_ = 0;
	  ET34MC_ = jetsMC_[2].Pt () + jetsMC_[3].Pt ();


	  variablesMC_.push_back (pair < string, float >(string ("ET34"), ET34MC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("ET34"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
	}

    }


  float ET1oET2_ = DefaultValue_;
  float ET1oET2MC_ = DefaultValue_;

  if (jets_.size () > 3 && jets_[0].Pt () / jets_[1].Pt ()>0 && jets_[0].Pt () / jets_[1].Pt ()<200)
    {
      ET1oET2_ = 0;
      ET1oET2_ = jets_[0].Pt () / jets_[1].Pt ();
      variables_.push_back (pair < string, float >(string ("ET1oET2"), ET1oET2_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("ET1oET2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }


  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  ET1oET2MC_ = 0;
	  ET1oET2MC_ = jetsMC_[0].Pt () / jetsMC_[1].Pt ();
	  variablesMC_.push_back (pair < string, float >(string ("ET1oET2"), ET1oET2MC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}

      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("ET1oET2"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}
    }


  float ET1oET3_ = DefaultValue_;
  float ET1oET3MC_ = DefaultValue_;

  if (jets_.size () > 3 && jets_[0].Pt () / jets_[2].Pt ()>0 && jets_[0].Pt () / jets_[2].Pt ()<200)
    {
      ET1oET3_ = 0;
      ET1oET3_ = jets_[0].Pt () / jets_[2].Pt ();
      variables_.push_back (pair < string, float >(string ("ET1oET3"), ET1oET3_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("ET1oET3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }

  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  ET1oET3MC_ = 0;
	  ET1oET3MC_ = jetsMC_[0].Pt () / jetsMC_[2].Pt ();
	  variablesMC_.push_back (pair < string, float >(string ("ET1oET3"), ET1oET3MC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}

      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("ET1oET3"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}
    }



  float ET1oET4_ = DefaultValue_;
  float ET1oET4MC_ = DefaultValue_;

  if (jets_.size () > 3 && jets_[0].Pt () / jets_[3].Pt ()>0 && jets_[0].Pt () / jets_[3].Pt ()<200)
    {
      ET1oET4_ = 0;
      ET1oET4_ = jets_[0].Pt () / jets_[3].Pt ();
      variables_.push_back (pair < string, float >(string ("ET1oET4"), ET1oET4_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("ET1oET4"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }

  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  ET1oET4MC_ = 0;
	  ET1oET4MC_ = jetsMC_[0].Pt () / jetsMC_[3].Pt ();
	  variablesMC_.push_back (pair < string, float >(string ("ET1oET4"), ET1oET4MC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}

      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("ET1oET4"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}
    }


  float ET2oET3_ = DefaultValue_;
  float ET2oET3MC_ = DefaultValue_;
  if (jets_.size () > 3 && jets_[1].Pt () / jets_[2].Pt ()>0 && jets_[1].Pt () / jets_[2].Pt ()<200)
    {
      ET2oET3_ = 0;
      ET2oET3_ = jets_[1].Pt () / jets_[2].Pt ();
      variables_.push_back (pair < string, float >(string ("ET2oET3"), ET2oET3_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("ET2oET3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 200.));
    }

  if (MCavailable_)
    {

      if (jetsMC_.size () > 3)
	{
	  ET2oET3MC_ = 0;
	  ET2oET3MC_ = jetsMC_[1].Pt () / jetsMC_[2].Pt ();
	  variablesMC_.push_back (pair < string, float >(string ("ET2oET3"), ET2oET3MC_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}

      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("ET2oET3"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 20.));
	}
    }

  
  //Mass transverse
  if (Ttbar.Mt() !=0){
  variables_.push_back (pair < string, float >(string ("Ttbar.Mt"), Ttbar.Mt ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));
  if (Ttbar.Mt()<0)
  cout<<Ttbar.Mt()<<endl;
}
  else
    {

 variables_.push_back (pair < string, float >(string ("Ttbar.Mt"),DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0., 5000.));
  }
  
  
 if (Ttbar.Pt() !=0){
   //float dphi = Ttbar.Phi() - met_.Phi();
   // cout<<dphi<<endl;
   float numb= 1- TMath::Cos(   ROOT::Math::VectorUtil::DeltaPhi (Ttbar, met_));//Ttbar.Phi() - met_.Phi());
   //cout<<sqrt(2*Ttbar.Pt()*met_.Et()* (1-TMath::Cos(numb)))<<endl;;
    float trans = sqrt(2* Ttbar.Pt() * met_.Et() * ( TMath::Cos(numb)));

 if (trans<0)
   cout<<"  TransMassTtbar is negative "<<trans<<endl;
  variables_.push_back (pair < string, float >(string ("TransMassTtbar"), trans));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));}
  else
    {

 variables_.push_back (pair < string, float >(string ("TransMassTtbar"),DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0., 5000.));
  }

  
  //Mtt
  if (Ttbar.M()!=0){
 if (Ttbar.M()<0)
  cout<<"  Ttbar.M "<<Ttbar.M()<<endl;
  variables_.push_back (pair < string, float >(string ("MassTtbar"), Ttbar.M ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));}

  else {
  variables_.push_back (pair < string, float >(string ("MassTtbar"), DefaultValue_));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));}


  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("TransMassTtbar"), TtbarMC.Mt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 200.));
      //Mtt
      variablesMC_.push_back (pair < string, float >(string ("MassTtbar"), TtbarMC.M ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));


    }


  //Mass transverse
 
	//if ( LepTop.Mt()>-9999){
 if (LepTop.Mt()<0)
   cout<<"  LepTopTransMass is negative  "<<LepTop.Mt()<<endl;
  variables_.push_back (pair < string, float >(string ("TransMassLepTop"), LepTop.Mt ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));//}
  //Mtt
  //
  /*else {
 variables_.push_back (pair < string, float >(string ("TransMassLepTop"), DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0., 5000.));

  }*/

if (LepTop.Mt()>5000)cout<<" TransMassLepTop is more than 5000... "<<endl;

  //  if (LepTop.M()!=0){
  if (LepTop.M()<0 && LepTop.P() == LepTop.E() ){

    variables_.push_back (pair < string, float >(string ("MassLepTop"), DefaultValue_));

    //  cout<<"  LepTopTrans is negative  "<<LepTop.M()<<" P  "<<LepTop.P()<<" Energy  "<<LepTop.E()<<" will put DefaultValue... "<<endl;

			    }
     else if (LepTop.M()<0 && LepTop.P() != LepTop.E() ) {

    variables_.push_back (pair < string, float >(string ("MassLepTop"), DefaultValue_));

    //    cout<<"  LepTopTrans is negative  "<<LepTop.M()<<" P  "<<LepTop.P()<<" Energy  "<<LepTop.E()<<" are not equal ...will put DefaultValue... "<<endl;

    }


   else if (LepTop.M()>0){

  variables_.push_back (pair < string, float >(string ("MassLepTop"), LepTop.M ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));}
  else {

 variables_.push_back (pair < string, float >(string ("MassLepTop"), DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0., 5000.));}
  
  
if (LepTop.M()>5000) cout<<" LEPTOP.M() IS MORE THAN 200 "<<LepTop.M()<<endl;
  //Mass transverse
  if (HadTop.Mt()!=0){
if (HadTop.Mt()<0)
  cout<<"  HadTopTransMass is negative  "<<HadTop.Mt()<<endl;
  variables_.push_back (pair < string, float >(string ("TransMassHadTop"), HadTop.Mt ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));}
  //Mtt

  else{
 variables_.push_back (pair < string, float >(string ("TransMassHadTop"), DefaultValue_));
 RangeVariables_.push_back (pair < float, float >(0., 5000.));}

  

if (HadTop.M()<0)
   cout<<"  HadTopMass is negative  "<<HadTop.M()<<endl;
  variables_.push_back (pair < string, float >(string ("MassHadTop"), HadTop.M ()));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));


  if (MCavailable_)
    {

      //Mass transverse
      variablesMC_.push_back (pair < string, float >(string ("TransMassLepTop"), LepTopMC.Mt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
      //Mtt

      variablesMC_.push_back (pair < string, float >(string ("MassLepTop"), LepTopMC.M ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));


      //Mass transverse
      variablesMC_.push_back (pair < string, float >(string ("TransMassHadTop"), HadTopMC.Mt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
      //Mtt

      variablesMC_.push_back (pair < string, float >(string ("MassHadTop"), HadTopMC.M ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));

    }
    
  //Eta of HadTop
  variables_.push_back (pair < string, float >(string ("EtaHadTop"), HadTop.Eta ()));
  RangeVariables_.push_back (pair < float, float >(-5., 5.));

  
  //Angle between HadTop & Lepton
  variables_.push_back (pair < string, float >(string ("AngleHadTopLepton"), ROOT::Math::VectorUtil::Angle (HadTop, lepton_)));
  RangeVariables_.push_back (pair < float, float >(0., 4.));
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("AngleHadTopLepton"), ROOT::Math::VectorUtil::Angle (HadTopMC, leptonMC_)));
      RangeVariablesMC_.push_back (pair < float, float >(0., 4.));
    }
  


  
  //Angle between HadTop & LetTop
  variables_.push_back (pair < string, float >(string ("AngleHadTopLepTop"), ROOT::Math::VectorUtil::Angle (HadTop, LepTop)));
  RangeVariables_.push_back (pair < float, float >(0., 4.));
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("AngleHadTopLepton"), ROOT::Math::VectorUtil::Angle (HadTopMC, LepTopMC)));
      RangeVariablesMC_.push_back (pair < float, float >(0., 4.));
    }
  
  

  //PtMuon 
  variables_.push_back (pair < string, float >(string ("PtMuon"), lepton_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 500.));
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("PtMuon"), leptonMC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 500.));
    }


  //EtaMuon

  if ( lepton_.Pt()!=0 ){

    float eta =-log(tan(lepton_.Theta()/2.));

  variables_.push_back (pair < string, float >(string ("EtaMuon"), eta));
  RangeVariables_.push_back (pair < float, float >(-5., 5.));
  // cout<<eta<<endl;
  }
  else {
  	variables_.push_back (pair < string, float >(string ("EtaMuon"), DefaultValue_));
  	RangeVariables_.push_back (pair < float, float >(-5., 5.));
  }

  //  variables_.push_back (pair < string, float >(string ("EtaMuon"), lepton_.Eta ()));
  //RangeVariables_.push_back (pair < float, float >(-3., 3.));

  if (MCavailable_)
    {

  if ( leptonMC_.Pt()!=0 ){

    float etaMC =-log(tan(leptonMC_.Theta()/2.));

  variablesMC_.push_back (pair < string, float >(string ("EtaMuon"), etaMC));
  RangeVariablesMC_.push_back (pair < float, float >(-5., 5.));
  // cout<<eta<<endl;
  }

      //variablesMC_.push_back (pair < string, float >(string ("EtaMuon"), leptonMC_.Eta ()));
      //RangeVariablesMC_.push_back (pair < float, float >(-3., 3.));
   }

  //Pt of jet (!) of quark 1 (from W decay)
  variables_.push_back (pair < string, float >(string ("PtQuark1"), quark1_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 1000.));
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("PtQuark1"), quark1MC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 1000.));
    }
    
  //Pt of jet (!) of quark 2 (from W decay)
  variables_.push_back (pair < string, float >(string ("PtQuark2"), quark2_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 1000.));
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("PtQuark2"), quark2MC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 1000.));
    }
    
  //Pt of jet (!) of hadronic b quark (from had top)
  variables_.push_back (pair < string, float >(string ("PtHadbquark"), hadbquark_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 1000.));
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("PtHadbquark"), hadbquarkMC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 1000.));
    }
  
  //Pt of jet (!) of leptonic b quark (from lep top)
  variables_.push_back (pair < string, float >(string ("PtLepbquark"), lepbquark_.Pt ()));
  RangeVariables_.push_back (pair < float, float >(0., 1000.));
  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("PtLepbquark"), lepbquarkMC_.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 1000.));
    }   

  //DeltaPhiMetMuon
  variables_.push_back (pair < string, float >(string ("DeltaPhiMetMuon"),  ROOT::Math::VectorUtil::DeltaPhi (met_, lepton_)));//(met_.Phi () - lepton_.Phi ())));
  RangeVariables_.push_back (pair < float, float >(-5., 5.));

  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("DeltaPhiMetMuon"),  ROOT::Math::VectorUtil::DeltaPhi (metMC_, leptonMC_)));// (metMC_.Phi () - leptonMC_.Phi ())));
      RangeVariablesMC_.push_back (pair < float, float >(-5., 5.));
    }


  //DeltaPhiMetLeadingJet
  if (jets_.size () > 0)
    {
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetLeadingJet"), ROOT::Math::VectorUtil::DeltaPhi (met_, jets_[0])));// (met_.Phi () - jets_[0].Phi ())));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaPhiMetLeadingJet"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
    }

  if (MCavailable_)
    {
      if (jetsMC_.size () > 0)
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaPhiMetLeadingJet"), ROOT::Math::VectorUtil::DeltaPhi (metMC_, jetsMC_[0]))); //(metMC_.Phi () - jetsMC_[0].Phi ())));
    RangeVariablesMC_.push_back (pair < float, float >(-5., 5.));
	}

      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaPhiMetLeadingJet"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(-5., 5.));
	}

    }



  
  if (jets_.size () > 3 && HadTop.Pt()!=0 && LepTop.Pt()!=0)
    {
      variables_.push_back (pair < string, float >(string ("DeltaPhiHadtopLepTop"),  ROOT::Math::VectorUtil::DeltaPhi (HadTop, LepTop)));//HadTop.Phi()-LepTop.Phi()));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaPhiHadtopLepTop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-5., 5.));
    }


  if (MCavailable_)
    {
      if (jetsMC_.size () > 3)
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaPhiHadtopLepTop"), ROOT::Math::VectorUtil::DeltaPhi (HadTopMC, LepTopMC)));// HadTopMC.Phi()-LepTopMC.Phi()));
   //	  RangeVariablesMC_.push_back (pair < float, float >(-5., 5.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaPhiHadtopLepTop"), DefaultValue_));

   //	  RangeVariablesMC_.push_back (pair < float, float >(-5., 5.));
	}
    }

  
  
  
  if (jets_.size () > 3)
    {
      variables_.push_back (pair < string, float >(string ("DeltaRLeptonLeadingJet"), ROOT::Math::VectorUtil::DeltaR (lepton_, jets_[0])));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaRLeptonLeadingJet"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }


  if (MCavailable_)
    {
      if (jetsMC_.size () > 0)
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRLeptonLeadingJet"), ROOT::Math::VectorUtil::DeltaR (leptonMC_, jetsMC_[0])));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRLeptonLeadingJet"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}
    }

  

  

  if (jets_.size () > 3 && HadTop.Pt()!=0 && LepTop.Pt()!=0)
    {
      variables_.push_back (pair < string, float >(string ("DeltaRHadtopLepTop"), ROOT::Math::VectorUtil::DeltaR (HadTop, LepTop)));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaRHadtopLepTop"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }


  if (MCavailable_)
    {
      if (jetsMC_.size () > 0)
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRHadtopLepTop"), ROOT::Math::VectorUtil::DeltaR (HadTopMC, LepTopMC)));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRHadtopLepTop"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}
    }
  


  if (jets_.size () > 3)
    {
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet2"), ROOT::Math::VectorUtil::DeltaR (jets_[0], jets_[1])));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet2"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }

  if (MCavailable_)
    {
      if (jetsMC_.size () > 1)
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRJet1Jet2"), ROOT::Math::VectorUtil::DeltaR (jetsMC_[0], jetsMC_[1])));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRJet1Jet2"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}

    }


  if (jets_.size () > 2)
    {
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet3"), ROOT::Math::VectorUtil::DeltaR (jets_[0], jets_[2])));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaRJet1Jet3"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }


  if (MCavailable_)
    {
      if (jetsMC_.size () > 1)
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRJet1Jet3"), ROOT::Math::VectorUtil::DeltaR (jetsMC_[0], jetsMC_[2])));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRJet1Jet3"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}

    }



  if (hadbquark_.Pt () != 0 && lepbquark_.Pt () != 0)
    {
      variables_.push_back (pair < string, float >(string ("DeltaRBTagJets"), ROOT::Math::VectorUtil::DeltaR (hadbquark_, lepbquark_)));
      RangeVariables_.push_back (pair < float, float >(0., 5.));

    }
  else
    {
      variables_.push_back (pair < string, float >(string ("DeltaRBTagJets"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(0., 5.));
    }

  

  if (MCavailable_)
    {
      if (jetsMC_.size () > 1)
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRBTagJets"), ROOT::Math::VectorUtil::DeltaR (hadbquark_, lepbquark_)));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("DeltaRBTagJets"), DefaultValue_));
    RangeVariablesMC_.push_back (pair < float, float >(0., 5.));
	}

    }



  //Mass of System lepton+b ..

  variables_.push_back (pair < string, float >(string ("MLepBl"), LepBl.M ()));
  //variables_.push_back(pair<string,float>(string("MblPt"),lbPt.M()));
  RangeVariables_.push_back (pair < float, float >(0., 500.));

  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("MLepBl"), LepBlMC.M ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 500.));
    }

  //variables_.push_back(pair<string,float>(string("MblBtag"),lbBtag.M()));
  //RangeVariables_.push_back(pair<float,float>(0.,500.));
  //compute with highest b-tag jet

  //we should have all the jets as input !!

  

  float SumDeltaPhiMuJet12_ = DefaultValue_;
  if (jets_.size () > 1)
    {
      SumDeltaPhiMuJet12_ = (lepton_.Phi () - jets_[0].Phi ()) + (lepton_.Phi () - jets_[1].Phi ());
      variables_.push_back (pair < string, float >(string ("SumDeltaPhiMuJet12"),  ROOT::Math::VectorUtil::DeltaPhi (lepton_, jets_[0])+   ROOT::Math::VectorUtil::DeltaPhi (lepton_, jets_[1] )   ));// (lepton_.Phi () - jets_[0].Phi ()) + (lepton_.Phi () - jets_[1].Phi ())));

      // cout<<SumDeltaPhiMuJet12_<<endl;
      RangeVariables_.push_back (pair < float, float >(-10., 10.));
    }

  else
    {
      variables_.push_back (pair < string, float >(string ("SumDeltaPhiMuJet12"), DefaultValue_));
      RangeVariables_.push_back (pair < float, float >(-10., 10.));
    }



  float SumDeltaPhiMuJet12MC = DefaultValue_;
  if (MCavailable_)
    {
      if (jetsMC_.size () > 1)
	{
	  SumDeltaPhiMuJet12MC = (leptonMC_.Phi () - jetsMC_[0].Phi ()) + (leptonMC_.Phi () - jetsMC_[1].Phi ());
	  variablesMC_.push_back (pair < string, float >(string ("SumDeltaPhiMuJet12"),  ROOT::Math::VectorUtil::DeltaPhi (leptonMC_, jetsMC_[0])+   ROOT::Math::VectorUtil::DeltaPhi (leptonMC_, jetsMC_[1] )   ));// SumDeltaPhiMuJet12MC));
    RangeVariablesMC_.push_back (pair < float, float >(0., 12.));
	}
      else
	{
	  variablesMC_.push_back (pair < string, float >(string ("SumDeltaPhiMuJet12"), DefaultValue_));
	  RangeVariablesMC_.push_back (pair < float, float >(0., 12.));
	}
    }

  //M3
  float Max = 0.;
  float M3 = 0.;
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
  variables_.push_back (pair < string, float >(string ("M3"), M3));
  RangeVariables_.push_back (pair < float, float >(0., 5000.));


  if (MCavailable_)
    {
      float MaxMC = DefaultValue_;
      float M3MC = DefaultValue_;
      if (jetsMC_.size () > 2)
	for (unsigned int i = 0; i < jetsMC_.size (); i++)
	  for (unsigned int j = i + 1; j < jetsMC_.size (); j++)
	    for (unsigned int k = j + 1; k < jetsMC_.size (); k++)
	      {
		TLorentzVector v = jetsMC_[i] + jetsMC_[j] + jetsMC_[k];
		//            printf("Pt of sum3 jets : %f\n", v.Pt());
		if (v.Pt () > MaxMC)
		  {
		    MaxMC = v.Pt ();
		    M3MC = v.M ();
		  }
	      }
      //          printf("M3 RECO : %f\t MC : %f\n", M3, M3MC);
      variablesMC_.push_back (pair < string, float >(string ("M3"), M3MC));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }




  

  //Var is of muon
  //variables_.push_back(pair<string,float>(string("d0Sig"),(lepton_.d0error() > 0 ? fabs(lepton_.d0())/lepton_.d0error() : -1 )));
  //RangeVariables_.push_back(pair<float,float>(0.,30.));
/*
  variables_.push_back (pair < string, float >(string ("RelCaloIso"), (lepton_.Pt () > 0 ? (lepton_.isoR03_emEt () + lepton_.isoR03_hadEt ()) / lepton_.Pt () : -1)));
  RangeVariables_.push_back (pair < float, float >(0., 10.));

  variables_.push_back (pair < string, float >(string ("RelTrackIso"), (lepton_.Pt () > 0 ? (lepton_.isoR03_sumPt ()) / lepton_.Pt () : -1)));
  RangeVariables_.push_back (pair < float, float >(0., 10.));
*/
  //hemisphere

  // Hadronic light jet pt in W candidate rest frame

  //cout<<"  HADDDDDD "<<Hadp_boosted.Pt()<<endl;
 
      variables_.push_back (pair < string, float >(string ("HadqPtWRF"), Hadp_boosted.Pt ()));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
    


  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("HadqPtWRF"), Hadp_boostedMC.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }



      // Hadronic b jet pt in hadronic top candidate rest frame
      variables_.push_back (pair < string, float >(string ("HadbPtHadTopRF"), Hadb_boosted.Pt ()));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
  


  if (MCavailable_)
    {
      variablesMC_.push_back (pair < string, float >(string ("HadbPtHadTopRF"), Hadb_boostedMC.Pt ()));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }



  // Build-up a vector with all jets and the selected lepton
  vector < TLorentzVector > AllJets_Lepton_noMet_;
  for (unsigned int i = 0; i < jets_.size (); i++){
    AllJets_Lepton_noMet_.push_back (jets_[i]);}
  AllJets_Lepton_noMet_.push_back (lepton_);


  // Build-up a vector with top jet candidates, the selected lepton and the missing transverse energy. The pz of the latter is calculated.
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

  vector < TLorentzVector > AllJets_Lepton_noMetMC_;
  vector < TLorentzVector > TtbarSystemMC_;
  if (MCavailable_)
    {
      for (unsigned int i = 0; i < jetsMC_.size (); i++){
	AllJets_Lepton_noMetMC_.push_back (jetsMC_[i]);}
      AllJets_Lepton_noMetMC_.push_back (lepton_);
      TtbarSystemMC_.push_back (quark1MC_);
      TtbarSystemMC_.push_back (quark2MC_);
      TtbarSystemMC_.push_back (hadbquarkMC_);
      TtbarSystemMC_.push_back (lepbquarkMC_);
      TtbarSystemMC_.push_back (leptonMC_);
      TtbarSystemMC_.push_back (metMC_);
      TtbarSystemMC_[5].SetPz (PzNeutrinoMC_);
      for (unsigned int i = 0; i < TtbarSystemMC_.size (); i++)
	{
	  TtbarSystemMC_[i].Boost (-TtbarMC.BoostVector ());
	}
    }


/*
  // Centrality

  float Centrality = 0, Centrality_N = 0, Centrality_D = 0;
  for (unsigned int i = 0; i < AllJets_Lepton_noMet_.size (); i++)
    {
      Centrality_N += AllJets_Lepton_noMet_[i].Pt ();
      Centrality_D += AllJets_Lepton_noMet_[i].P ();
    }
  (Centrality_D > 0 ? Centrality = Centrality_N / Centrality_D : -1);
  variables_.push_back (pair < string, float >(string ("Centrality"), Centrality));
  RangeVariables_.push_back (pair < float, float >(0., 2.));

  float CentralityMC = 0, Centrality_NMC = 0, Centrality_DMC = 0;
  if (MCavailable_)
    {
      for (unsigned int i = 0; i < AllJets_Lepton_noMetMC_.size (); i++)
	{
	  Centrality_NMC += AllJets_Lepton_noMetMC_[i].Pt ();
	  Centrality_DMC += AllJets_Lepton_noMetMC_[i].P ();
	}
      (Centrality_DMC > 0 ? CentralityMC = Centrality_NMC / Centrality_DMC : -1);
     if (CentralityMC!=0){ variablesMC_.push_back (pair < string, float >(string ("Centrality"), CentralityMC));
       RangeVariablesMC_.push_back (pair < float, float >(0., 2.));
     }

    }

  Centrality = 0, Centrality_N = 0, Centrality_D = 0;
  for (unsigned int i = 0; i < TtbarSystem_.size (); i++)
    {
      Centrality_N += TtbarSystem_[i].Pt ();
      Centrality_D += TtbarSystem_[i].P ();
    }
  (Centrality_D > 0 ? Centrality = Centrality_N / Centrality_D : -1);
  
  if (Centrality !=0){variables_.push_back (pair < string, float >(string ("Centrality_TtbarSyst"), Centrality));
  RangeVariables_.push_back (pair < float, float >(0., 5.));}


  if (MCavailable_)
    {
      CentralityMC = 0, Centrality_NMC = 0, Centrality_DMC = 0;
      for (unsigned int i = 0; i < TtbarSystemMC_.size (); i++)
	{
	  Centrality_NMC += TtbarSystemMC_[i].Pt ();
	  Centrality_DMC += TtbarSystemMC_[i].P ();
	}
      (Centrality_DMC > 0 ? CentralityMC = Centrality_NMC / Centrality_DMC : -1);
      if (CentralityMC !=0){ variablesMC_.push_back (pair < string, float >(string ("Centrality_TtbarSyst"), CentralityMC));
        RangeVariablesMC_.push_back (pair < float, float >(0., 1.));
      }

    }

  //Compute the matrice for
  //Sphericity - Aplanarity - Circularity
*/

/*    
   vector<TLorentzVector> objects_;

   if(jets_.size()>3){
     for(unsigned int i=0;i<4;i++)
      objects_.push_back(jets_[i]);
   objects_.push_back(lepton_);
   objects_.push_back(met_);
   objects_[5].SetPz(PzNeutrino_);
*/

/*
  TMatrixD matrix_alljets (3, 3), matrix_alljetsMC (3, 3);
  float val = 0, valMC = 0;
  float P = 0, PMC = 0;
  float a = 0, aMC = 0;
  float b = 0, bMC = 0;
  for (int i = 0; i < 3; i++)
	{
    for (int j = 0; j < 3; j++)
      {
	val = 0;
	P = 0;
	a = 0;
	b = 0;
	for (unsigned int k = 0; k < AllJets_Lepton_noMet_.size (); k++)
	  {
	    P += AllJets_Lepton_noMet_[k].P () * AllJets_Lepton_noMet_[k].P ();
	    if (i == 0)
	      a = AllJets_Lepton_noMet_[k].Px ();
	    if (i == 1)
	      a = AllJets_Lepton_noMet_[k].Py ();
	    if (i == 2)
	      a = AllJets_Lepton_noMet_[k].Pz ();
	    if (j == 0)
	      b = AllJets_Lepton_noMet_[k].Px ();
	    if (j == 1)
	      b = AllJets_Lepton_noMet_[k].Py ();
	    if (j == 2)
	      b = AllJets_Lepton_noMet_[k].Pz ();
	    val += a * b;
	  }
	matrix_alljets (i, j) = val;
	}
	}
TDecompSVD decomp_alljets (matrix_alljets);
   



  if (MCavailable_)
    {

for (int i = 0; i < 3; i++)
	{
    for (int j = 0; j < 3; j++)
      {
	 valMC = 0;
	 PMC = 0;
	 aMC = 0;
	 bMC = 0;
	for (unsigned int k = 0; k < AllJets_Lepton_noMetMC_.size (); k++)
	  {
	    PMC += AllJets_Lepton_noMetMC_[k].P () * AllJets_Lepton_noMetMC_[k].P ();
	    if (i == 0)
	      aMC = AllJets_Lepton_noMetMC_[k].Px ();
	    if (i == 1)
	      aMC = AllJets_Lepton_noMetMC_[k].Py ();
	    if (i == 2)
	      aMC = AllJets_Lepton_noMetMC_[k].Pz ();
	    if (j == 0)
	      bMC = AllJets_Lepton_noMetMC_[k].Px ();
	    if (j == 1)
	      bMC = AllJets_Lepton_noMetMC_[k].Py ();
	    if (j == 2)
	      bMC = AllJets_Lepton_noMetMC_[k].Pz ();
	    valMC += aMC * bMC;
	}
	matrix_alljetsMC (i, j) = valMC;
	}
	}
TDecompSVD decomp_alljetsMC (matrix_alljetsMC);
}
  vector < float >EigenValues;
  bool notVeto = decomp_alljets.Decompose ();
  if (notVeto)
    {
      TVectorD sig_alljets = decomp_alljets.GetSig ();
      EigenValues.push_back (sig_alljets (0));
      EigenValues.push_back (sig_alljets (1));
      EigenValues.push_back (sig_alljets (2));
      sort (EigenValues.begin (), EigenValues.end (), Highest ());

      variables_.push_back (pair < string, float >(string ("Aplanarity"), float (3. / 2. * EigenValues[2])));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));
      variables_.push_back (pair < string, float >(string ("Sphericity"), float (3. / 2. * (EigenValues[2] + EigenValues[1]))));
      RangeVariables_.push_back (pair < float, float >(0., 5000.));

 //      variablesMC_.push_back (pair < string, float >(string ("Aplanarity"), float (3. / 2. * EigenValues[2])));
 //     RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
 //     variablesMC_.push_back (pair < string, float >(string ("Sphericity"), float (3. / 2. * (EigenValues[2] + EigenValues[1]))));
 //     RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));


    }
  else
    {
      variables_.push_back (pair < string, float >(string ("Aplanarity"), -1.));
       RangeVariables_.push_back (pair < float, float >(0., 5000.));
      variables_.push_back (pair < string, float >(string ("Sphericity"), -1.));
       RangeVariables_.push_back (pair < float, float >(0., 5000.));
    }

 

  TMatrixD matrix (3, 3);
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++)
      {
	val = 0, valMC = 0;
	P = 0;
	a = 0;
	b = 0;
	for (unsigned int k = 0; k < TtbarSystem_.size (); k++)
	  {
	    P += TtbarSystem_[k].P () * TtbarSystem_[k].P ();
	    if (i == 0)
	      a = TtbarSystem_[k].Px ();
	    if (i == 1)
	      a = TtbarSystem_[k].Py ();
	    if (i == 2)
	      a = TtbarSystem_[k].Pz ();
	    if (j == 0)
	      b = TtbarSystem_[k].Px ();
	    if (j == 1)
	      b = TtbarSystem_[k].Py ();
	    if (j == 2)
	      b = TtbarSystem_[k].Pz ();
	    val += a * b;
	  }
	matrix (i, j) = val;
  }}

  TDecompSVD decomp (matrix);
  EigenValues.clear ();
  notVeto = decomp.Decompose ();
  if (notVeto)
    {
      TVectorD sig = decomp.GetSig ();
      EigenValues.push_back (sig (0));
      EigenValues.push_back (sig (1));
      EigenValues.push_back (sig (2));
      sort (EigenValues.begin (), EigenValues.end (), Highest ());
      variables_.push_back (pair < string, float >(string ("Aplanarity_TtbarSyst"), float (3. / 2. * EigenValues[2])));
      variables_.push_back (pair < string, float >(string ("Sphericity_TtbarSyst"), float (3. / 2. * (EigenValues[1] + EigenValues[2]))));
    }
  else
    {
      variables_.push_back (pair < string, float >(string ("Aplanarity_TtbarSyst"), -1.));
      variables_.push_back (pair < string, float >(string ("Sphericity_TtbarSyst"), -1.));
    }
  
if (MCavailable_)
    {
	
TMatrixD  matrixMC (3, 3);
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++)
      {
	 valMC = 0;
	PMC = 0;
	 aMC = 0;
	 bMC = 0;
	for (unsigned int k = 0; k < TtbarSystemMC_.size (); k++)
	  {
	    PMC += TtbarSystemMC_[k].P () * TtbarSystemMC_[k].P ();
	    if (i == 0)
	      aMC = TtbarSystemMC_[k].Px ();
	    if (i == 1)
	      aMC = TtbarSystemMC_[k].Py ();
	    if (i == 2)
	      aMC = TtbarSystemMC_[k].Pz ();
	    if (j == 0)
	      bMC = TtbarSystemMC_[k].Px ();
	    if (j == 1)
	      bMC = TtbarSystemMC_[k].Py ();
	    if (j == 2)
	      bMC = TtbarSystemMC_[k].Pz ();
	    valMC += aMC * bMC;
	  }
	matrixMC (i, j) = valMC;
   }}
  vector < float >EigenValuesMC;
  bool notVetoMC = decomp_alljets.Decompose ();
  TDecompSVD decompMC (matrixMC);
  EigenValuesMC.clear ();
  if (notVetoMC && decompMC.Decompose ())
    {
      TVectorD sigMC = decompMC.GetSig ();
      EigenValuesMC.push_back (sigMC (0));
      EigenValuesMC.push_back (sigMC (1));
      EigenValuesMC.push_back (sigMC (2));
      sort (EigenValuesMC.begin (), EigenValuesMC.end (), Highest ());

      variablesMC_.push_back (pair < string, float >(string ("Aplanarity_TtbarSyst"), float (3. / 2. * EigenValuesMC[2])));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
      variablesMC_.push_back (pair < string, float >(string ("Sphericity_TtbarSyst"), float (3. / 2. * (EigenValuesMC[1] + EigenValuesMC[2]))));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }
  else
    {
      variablesMC_.push_back (pair < string, float >(string ("Aplanarity_TtbarSyst"), -1.));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
      variablesMC_.push_back (pair < string, float >(string ("Sphericity_TtbarSyst"), -1.));
      RangeVariablesMC_.push_back (pair < float, float >(0., 5000.));
    }
  
}

*/

  // Circularity of the event

  double N = 0, D = 0, C_tmp = 0, C = 5000.;
  double nx = 0, ny = 0, nz = 0;
  double DMC = 0, CMC = 5000.;

  // C = 2min(E(pt.n)^2/E(pt)^2) = 2*N/D but it is theorically preferable to use C'=PI/2*min(E|pt.n|/E|pt|), sum over all jets+lepton+MET (cf PhysRevD 48 R3953(Nov 1993))

  for (unsigned int i = 0; i < AllJets_Lepton_noMet_.size (); i++)
	{
    D += fabs (AllJets_Lepton_noMet_[i].Pt ());}
    
  if ((D > 0))
    {
      // Loop over all the unit vectors in the transverse plane in order to find the miminum : 
      for (unsigned int i = 0; i < 720; i++)
	{
	  nx = cos ((2 * TMath::Pi () / 720) * i);
	  ny = sin ((2 * TMath::Pi () / 720) * i);
	  nz = 0;
	  N = 0;
	  for (unsigned int j = 0; j < AllJets_Lepton_noMet_.size (); j++)
	    {
	      N += fabs (AllJets_Lepton_noMet_[j].Px () * nx + AllJets_Lepton_noMet_[j].Py () * ny + AllJets_Lepton_noMet_[j].Pz () * nz);
	    }
	  C_tmp = 2 * N / D;
	  if (C_tmp < C)
	    C = C_tmp;
	}
    
  C_tmp = 0;
	}
  variables_.push_back (pair < string, float >(string ("Circularity"), C));
  RangeVariables_.push_back (pair < float, float >(0., 1.));

if (MCavailable_)
    {

  for (unsigned int i = 0; i < AllJets_Lepton_noMetMC_.size (); i++)
	{
    DMC += fabs (AllJets_Lepton_noMetMC_[i].Pt ());}
	
  if ((DMC > 0))
    {
      // Loop over all the unit vectors in the transverse plane in order to find the miminum : 
      for (unsigned int i = 0; i < 720; i++)
	{
	  nx = cos ((2 * TMath::Pi () / 720) * i);
	  ny = sin ((2 * TMath::Pi () / 720) * i);
	  nz = 0;
	  N = 0;
	  for (unsigned int j = 0; j < AllJets_Lepton_noMetMC_.size (); j++)
	    {
	      N += fabs (AllJets_Lepton_noMetMC_[j].Px () * nx + AllJets_Lepton_noMetMC_[j].Py () * ny + AllJets_Lepton_noMetMC_[j].Pz () * nz);
	    }
	  C_tmp = 2 * N / DMC;
	  if (C_tmp < CMC)
	    CMC = C_tmp;
	}
    
 

  variablesMC_.push_back (pair < string, float >(string ("Circularity"), CMC));
  RangeVariablesMC_.push_back (pair < float, float >(0., 1.));
	}

}


  N = 0, D = 0, C_tmp = 0, C = 5000.;
  nx = 0, ny = 0, nz = 0;
  DMC = 0, CMC = 5000.;

  for (unsigned int i = 0; i < TtbarSystem_.size (); i++)
{
    D += fabs (TtbarSystem_[i].Pt ());}

  for (unsigned int i = 0; i < TtbarSystemMC_.size (); i++){
    DMC += fabs (TtbarSystemMC_[i].Pt ());}

  if ((D > 0))
    {
      // Loop over all the unit vectors in the transverse plane in order to find the miminum : 
      for (unsigned int i = 0; i < 720; i++)
	{
	  nx = cos ((2 * TMath::Pi () / 720) * i);
	  ny = sin ((2 * TMath::Pi () / 720) * i);
	  nz = 0;
	  N = 0;
	  for (unsigned int j = 0; j < TtbarSystem_.size (); j++)
	    {
	      N += fabs (TtbarSystem_[j].Px () * nx + TtbarSystem_[j].Py () * ny + TtbarSystem_[j].Pz () * nz);
	    }
	  C_tmp = 2 * N / D;
	  if (C_tmp < C)
	    C = C_tmp;
	}
    }
	
  else
    {
      C = -1;
    }
  C_tmp = 0;

  if (MCavailable_)
    {
  if ((DMC > 0))
    {
      // Loop over all the unit vectors in the transverse plane in order to find the miminum : 
      for (unsigned int i = 0; i < 720; i++)
	{
	  nx = cos ((2 * TMath::Pi () / 720) * i);
	  ny = sin ((2 * TMath::Pi () / 720) * i);
	  nz = 0;
	  N = 0;
	  for (unsigned int j = 0; j < TtbarSystemMC_.size (); j++)
	    {
	      N += fabs (TtbarSystemMC_[j].Px () * nx + TtbarSystemMC_[j].Py () * ny + TtbarSystemMC_[j].Pz () * nz);
	    }
	  C_tmp = 2 * N / DMC;
	  if (C_tmp < CMC)
	    CMC = C_tmp;
	}
    }
  else
    {
      CMC = -1;
    }
  variablesMC_.push_back (pair < string, float >(string ("Circularity_TtbarSyst"), CMC));
  RangeVariablesMC_.push_back (pair < float, float >(0., 1.));

    }
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




