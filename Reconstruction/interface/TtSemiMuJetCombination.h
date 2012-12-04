#ifndef TtSemiMuJetCombination_h
#define TtSemiMuJetCombination_h


// system include files
#include <memory>

// PAT object definition
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootGenEvent.h"
#include "TopTreeProducer/interface/TRootNPGenEvent.h"

#include "TopTreeProducer/interface/TRootGenTop.h"

#include "TopTreeAnalysis/MCInformation/interface/JetPartonMatching.h"
#include "Combination.h"

using namespace TopTree;


struct HighestPtGenTop{
	bool operator()( TRootGenTop j1, TRootGenTop j2 ) const{
		return j1.Pt() > j2.Pt() ;
	}
};
		

class TtSemiMuJetCombination{


   public:
      TtSemiMuJetCombination();
      TtSemiMuJetCombination(const std::vector<TRootJet>&, const TRootMuon &,unsigned int NjetsForComb_, double JetPtThreshold_, double JetEtaThreshold_);
      TtSemiMuJetCombination(const TtSemiMuJetCombination &);
      ~TtSemiMuJetCombination();
      
      void SetConfiguration(double TopMass_, double WMass_, double blMass_, double LepTopMassResol_, double LepblMassResol_, double HadWMassResol_, double HadTopMassResol_);
      void SetParameters(unsigned int NjetsForComb_, double JetPtThreshold_, double JetEtaThreshold_);
      void SetMatchingParameters(int matchingAlgo_ = 2 , bool useMaxDist_ = true, bool useDeltaR_ = true, double maxDist_ = 0.3);
      void SetMuon(const TRootMuon & init_muon);
      void SetJets(const std::vector<TRootJet>& init_jets);
      double ChiSquareMatching( std::vector<TRootJet> &, const TRootMuon &, unsigned int *);
      double ChiSquareMatchingHadTop( std::vector<TRootJet> &, const TRootMuon &, unsigned int *);
      
      // Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2, Leptonic b  = 3;
      //method 0 = HadTop - method 1 = 4 quarks (+MassLb)
      std::vector<TRootJet> ComputeBestCombination(double& ChiSquare, int method = 0);//modify the value of ChiSquare
      //std::vector<TRootJet> ComputeBestCombination(int method = 0);// float& ChiSquare);//modify the value of ChiSquare
      //std::vector<std::vector<TRootJet> > ComputeCombinations(int method = 0, std::vector<float>& ChiSquareVector );//modify the vector of ChiSquare
      //->both vector ordered by increasing ChiSquare
      //not yet implemented
      std::vector<TRootJet> MCMatchedCombination(const TRootGenEvent&, double& ChiSquare);
      std::vector<TRootJet> MCMatchedCombination(const TRootNPGenEvent&, double& ChiSquare);
      //Matched means 3 jets matched with HadTop + 1 jet matched with LepB
      bool isMCMatched(const TRootGenEvent& genEvt);
      bool isMCMatched(const std::vector<TRootJet>, const TRootGenEvent&);
      //Matched means 3 jets matched with HadTop
      bool isMCMatchedHadTop(const std::vector<TRootJet>, const TRootNPGenEvent&);

   private:
      
      std::vector<TRootJet> jets;
      TRootMuon muon;
      
      //matching
      int matchingAlgo_;
      bool useMaxDist_;
      bool useDeltaR_;
      double maxDist_;
      //ChiSquare
      double TopMass_;
      double WMass_;
      double blMass_;
      double LepTopMassResol_;
      double LepblMassResol_;
      double HadTopMassResol_;
      double HadWMassResol_;
      //jets 
      unsigned int NjetsForComb_;
      double JetPtThreshold_;
      double JetEtaThreshold_;

};

#endif
