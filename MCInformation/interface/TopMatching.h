#ifndef TopMatching_h
#define TopMatching_h


// system include files
#include <memory>

// PAT object definition
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootGenEvent.h"
#include "TopTreeProducer/interface/TRootNPGenEvent.h"


#include "JetPartonMatching.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/Combination.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/TtSemiMuJetCombination.h"
		
using namespace TopTree;


class TopMatching{


   public:
      TopMatching();
      TopMatching(float JetPtThreshold_, float JetEtaThreshold_);
      TopMatching(int matchingAlgo_, bool useMaxDist_, bool useDeltaR_, float maxDist_);
      TopMatching(float JetPtThreshold_, float JetEtaThreshold_, int matchingAlgo_, bool useMaxDist_, bool useDeltaR_, float maxDist_);
      TopMatching(const TopMatching &);
      ~TopMatching();
      
      //Matched means 3 jets matched with HadTop + 1 jet matched with LepB
      bool isMCMatched(const std::vector<TRootJet>, const TRootGenEvent&, float& , float& );
      //Matched means 3 jets matched with HadTop
      bool isMCMatchedHadTop(const std::vector<TRootJet>, const TRootGenEvent&, float& , float& );
      bool isMCMatchedHadTop(const std::vector<TRootJet>, const TRootNPGenEvent&, float& , float& );
      bool is3jetsSelected(const std::vector<TRootJet> jets);
      bool is4jetsSelected(const std::vector<TRootJet> jets);

   private:
      
      std::vector<TRootJet> jets;
      
      //acceptance
      float JetPtThreshold_;
      float JetEtaThreshold_;
      //matching
      int matchingAlgo_;
      bool useMaxDist_;
      bool useDeltaR_;
      float maxDist_;

};

#endif
