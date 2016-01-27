#ifndef ObservablesEl_h
#define ObservablesEl_h
//#include "../../../../TopBrussels/TopTreeProducer/interface/TRootHemisphere.h"

#include "../../../TopTreeProducer/interface/TRootJet.h"
#include "../../../TopTreeProducer/interface/TRootElectron.h"
#include "../../../TopTreeProducer/interface/TRootNPGenEvent.h"
#include "../../Reconstruction/interface/MEzCalculator.h"


#include <iostream>
#include <iomanip>
#include <Math/VectorUtil.h>


#include "TClonesArray.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompSVD.h"


using namespace std;
using namespace TopTree;

struct highest{
   bool operator()( float a1, float a2 ) const{
     return a1 > a2;
   }
};
/*
struct HighestPt{
    bool operator()( TRootJet j1, TRootJet j2 ) const{
     return j1.Pt() > j2.Pt() ;
    }
    bool operator()( TRootElectron j1, TRootElectron j2 ) const{
     return j1.Pt() > j2.Pt() ;
    }
};
struct HighestBtag{
    bool operator()( TRootJet j1, TRootJet j2 ) const{
      return j1.btag_trackCountingHighEffBJetTags > j2.btag_trackCountingHighEffBJetTags ;
    }
};
*/

class ObservablesEl {
  
 public:
  
  ObservablesEl();

  ObservablesEl(const ObservablesEl& obs);
  
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootMET& MET);
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET);
  
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET,float & ChiSquare_);
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootMET& MET, const vector<TRootJet>& jets);

 
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets);
  

  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet*>& jets,float & ChiSquare_,int & chi2_max_);
  
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet*>& jets,float & ChiSquare_);
  
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet*>& jets,vector <float> values_,float & ChiSquare_); 


 

  ObservablesEl(const TRootElectron& lepton, const vector <TRootJet> & chijets,const vector <TRootJet> & jets_,const TRootMET& MET, string & dsname , float & ChiSquare_);


  ObservablesEl (const TRootElectron & lepton, const TRootJet & qFromW1, const TRootJet & qFromW2, const TRootJet & bFromHadTop, const TRootJet & bFromLepTop, const vector < TRootJet > &jets,vector<float> values_,const TRootMET & MET, float &ChiSq);

  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const vector <TRootJet> & jets_,const TRootMET& MET);

  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const vector <TRootJet> & jets_,const TRootMET& MET,float & ChiSquare_);
  
  ObservablesEl(const TRootElectron& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const vector <TRootJet> & jets_,const TRootMET& MET,float & ChiSquare_,bool GoodChi2_);
  ObservablesEl (const TRootElectron & lept, const vector < TRootJet > &jets_, const TRootMET & MET_, string & dsname);

  ~ObservablesEl();



/**
   Different ways to load MC information to compute variables for MC.
   This is the only way to set the MC flag has "on".
  */
  void SetMCInfo(const TRootParticle& lepton, const TRootMCParticle& qFromW1, const TRootMCParticle& qFromW2, const TRootJet& bFromHadTop, const TRootMCParticle& bFromLepTop, const TRootMET& MET, const vector<TRootMCParticle>& jets);
  
  /**
   When loading individual parts, you reset the MC usage. If you want to use it, you have to go through SetMCInfo
   */
  void SetMET(TRootMET met){ MCavailable_ = false; met_ = met;};
  void SetLepton(TRootElectron lepton){ MCavailable_ = false; lepton_ = lepton;};
  void SetQuarksFromW(TRootJet q1, TRootJet q2){ MCavailable_ = false; quark1_ = q1; quark2_ = q2;};
  void SetBFromHadTop(TRootJet b){ MCavailable_ = false; hadbquark_ = b;};
  void SetJets(vector<TRootJet> jets){ MCavailable_ = false; jets_ = jets; };
  //void SetHemisphere(TRootHemisphere hemisphere){ hemisphere_ = hemisphere;};
  void SetChiSquare(float a){ChiSquare_ = a;};
  void SetChiSquareHadTop(float a){ChiSquareHadTop_ = a;};

  /**
   Different ways to load MC information to compute variables for MC.
   This is the only way to set the MC flag has "on".
  */



  void ComputeVariables();
 
  
void ComputeVariables (const TRootElectron &selmuons_,  const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop , const vector <TRootJet> &seljets_,const TRootMET & selmets_, string & dsname);


void ComputeVariables (const TRootElectron &selmuons_,  const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop , const vector <TRootJet> &seljets_,const TRootMET & selmets_, string & dsname , float & ChiSquare_);


 void ComputeVariables (const TRootElectron &selmuons_, const vector <TRootJet> &seljets_,const TRootMET & selmets_ , string & dsname);
  


  float ChiSquare() const{return ChiSquare_;};
  float ChiSquareHadTop() const{return ChiSquareHadTop_;}; 

  void setEventscaleFactor(float scaleFactor);
  float Variable(const char*) ;
  float Variable(const string) ;
  float VariableMC(const char*) ;
  float VariableMC(const string) ;
  bool isModeMC();
  void unsetMC();
  pair<float,float> Range(const char*);
  pair<float,float> Range(const string);
  vector< pair<string,float> > Variables() const {return variables_;};
  vector< pair<string,float> > VariablesMC() const {return variablesMC_;};
  vector< pair<float,float> > RangeVariables() const {return RangeVariables_;};
  vector< pair<float,float> > RangeVariablesMC() const {return RangeVariablesMC_;};

  vector<string> ListOfVariables() ;
  
  vector<string> ListOfVariablesMC() ;
  //map<string,TH1F*> Histo1D;
  //map<string,TH2F*> Histo2D;
  /**
  Returns the variables for which the information for MC and RECO is available, at the same time. 
  Result should be the same as ListOfVariablesMC().
  */
  vector<string> ListOfVariablesCoupled();
 
 private:
 
   TRootMET met_;
   TRootMET  metMC_;
   TRootElectron lepton_;
   TRootMCParticle leptonMC_;
   TRootJet quark1_;
   TRootJet quark2_;
   TRootJet hadbquark_;
   TRootJet lepbquark_;
   TRootJet Fourthquark_;
   TRootMCParticle quark1MC_, quark2MC_, hadbquarkMC_, lepbquarkMC_, FourthquarkMC_;
   vector<TRootJet> jets_;
   vector<TRootMCParticle> jetsMC_;
   float DefaultValue_; 
  //pat::Hemisphere hemisphere_;
   float PzNeutrino_, PzNeutrinoMC_;
  
   bool MCavailable_;
   std::vector<pair<string,float> > variables_;
   std::vector<pair<string,float> > variablesMC_;
   std::vector<pair<float,float> > RangeVariables_;
   std::vector<pair<float,float> > RangeVariablesMC_;

   float ChiSquare_;
   float ChiSquareHadTop_;
   float btaggerLepB;
   float btaggerHadB;
   float  CombBtagInfo ;
   int chi2_max;
};

#endif
