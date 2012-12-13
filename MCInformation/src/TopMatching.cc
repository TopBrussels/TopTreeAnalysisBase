#include "../interface/TopMatching.h"

using namespace std;
using namespace stdcomb;

TopMatching::TopMatching(){
    matchingAlgo_ = 2 ;
    useMaxDist_ = true ;
    useDeltaR_ = true ;
    JetPtThreshold_ = 30 ;
    JetEtaThreshold_ = 2.4 ;
}

TopMatching::TopMatching(float JetPtThreshold, float JetEtaThreshold){
	(*this) = TopMatching();
	JetPtThreshold_ = JetPtThreshold_;
	JetEtaThreshold_ = JetEtaThreshold_;
}

TopMatching::TopMatching(int matchingAlgo, bool useMaxDist, bool useDeltaR, float maxDist){
	(*this) = TopMatching();
     	matchingAlgo_ = matchingAlgo;
  	useMaxDist_ = useMaxDist;
    	useDeltaR_ = useDeltaR;
}
	      
TopMatching::TopMatching(float JetPtThreshold, float JetEtaThreshold, int matchingAlgo, bool useMaxDist, bool useDeltaR, float maxDist){
	JetPtThreshold_ = JetPtThreshold;
	JetEtaThreshold_ = JetEtaThreshold;
     	matchingAlgo_ = matchingAlgo;
  	useMaxDist_ = useMaxDist;
    	useDeltaR_ = useDeltaR;
}
	      

TopMatching::TopMatching(const TopMatching & a){
    matchingAlgo_ = a.matchingAlgo_;
    useMaxDist_ = a.useMaxDist_;
    useDeltaR_ = a.useDeltaR_;
    maxDist_ = a.maxDist_;
    JetPtThreshold_ = a.JetPtThreshold_;
    JetEtaThreshold_ = a.JetEtaThreshold_;
}

TopMatching::~TopMatching(){}

bool TopMatching::is3jetsSelected(const std::vector<TRootJet> jets){
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	if((jets[i].Eta())<JetEtaThreshold_ && jets[i].Pt()>JetPtThreshold_)
		tljets.push_back((TLorentzVector)jets[i]);
   }
   if(tljets.size()<3) return false;
   return true;
}

bool TopMatching::is4jetsSelected(const std::vector<TRootJet> jets){
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	if((jets[i].Eta())<JetEtaThreshold_ && jets[i].Pt()>JetPtThreshold_)
		tljets.push_back((TLorentzVector)jets[i]);
   }
   if(tljets.size()<4) return false;
   return true;
}

bool TopMatching::isMCMatched(const std::vector<TRootJet> jets,const TRootGenEvent& genEvt, float& WMass, float& TopMass){
   WMass = -999.;
   TopMass = -999.;
   //cout<<"is4jetsSelected "<<is4jetsSelected(jets)<<endl;
   if(!is4jetsSelected(jets)) return false;
   
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	if((jets[i].Eta())<JetEtaThreshold_ && jets[i].Pt()>JetPtThreshold_)
		tljets.push_back((TLorentzVector)jets[i]);
   }

   //cout<<"isSemiLeptonic "<<genEvt.isSemiLeptonic()<<endl;
   if(!genEvt.isSemiLeptonic()) return false;
   //cout<<"pass"<<endl;	
	// Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2, Leptonic b  = 3;
   	std::vector<TLorentzVector> quarks;
	quarks.push_back(genEvt.hadronicDecayQuark());
	quarks.push_back(genEvt.hadronicDecayQuarkBar());
	quarks.push_back(genEvt.hadronicDecayB());
	quarks.push_back(genEvt.leptonicDecayB());
	JetPartonMatching GenMatchHadTop(quarks, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	//cout<<"CombAvailables "<<GenMatchHadTop.getNumberOfAvailableCombinations()<<endl;
	for(unsigned int c=0;c<GenMatchHadTop.getNumberOfAvailableCombinations();c++){
		if(GenMatchHadTop.getNumberOfUnmatchedPartons(c)==0){
			WMass = ((TLorentzVector) (tljets[GenMatchHadTop.getMatchForParton(0,c)]+tljets[GenMatchHadTop.getMatchForParton(0,c)])).M();
			TopMass = ((TLorentzVector) (tljets[GenMatchHadTop.getMatchForParton(0,c)]+tljets[GenMatchHadTop.getMatchForParton(1,c)]+tljets[GenMatchHadTop.getMatchForParton(2,c)])).M();
			return(true);
		}
	}
   return false;
}

bool TopMatching::isMCMatchedHadTop(const std::vector<TRootJet> jets, const TRootNPGenEvent& genEvt, float& WMass, float& TopMass){
   WMass = -999.;
   TopMass = -999.;
   //cout<<"is3jetsSelected "<<is3jetsSelected(jets)<<endl;
   if(!is3jetsSelected(jets)) return false;
   //cout<<"passe"<<endl;
   
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	if((jets[i].Eta())<JetEtaThreshold_ && jets[i].Pt()>JetPtThreshold_)
   		tljets.push_back((TLorentzVector)jets[i]);
   }
		
   vector<TRootGenTop> hadronicTops;
   for(unsigned int i=0;i<genEvt.tops().size();i++) if(genEvt.tops()[i].isHadronic()) hadronicTops.push_back(genEvt.tops()[i]);
   std::sort(hadronicTops.begin(),hadronicTops.end(),HighestPtGenTop());
   for(unsigned int i=0;i<hadronicTops.size();i++){
   	// Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2
   	std::vector<TLorentzVector> quarks;
	quarks.push_back(hadronicTops[i].quark());
	quarks.push_back(hadronicTops[i].quarkBar());
	quarks.push_back(hadronicTops[i].bquark());
	JetPartonMatching GenMatch(quarks, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	for(unsigned int c=0;c<GenMatch.getNumberOfAvailableCombinations();c++){
		if(GenMatch.getNumberOfUnmatchedPartons(c)==0){
			WMass = ((TLorentzVector) (tljets[GenMatch.getMatchForParton(0,c)]+tljets[GenMatch.getMatchForParton(1,c)])).M();
			TopMass = ((TLorentzVector) (tljets[GenMatch.getMatchForParton(0,c)]+tljets[GenMatch.getMatchForParton(1,c)]+tljets[GenMatch.getMatchForParton(2,c)])).M();
			return(true);
		}
	}
   }
   return false;
}

bool TopMatching::isMCMatchedHadTop(const std::vector<TRootJet> jets, const TRootGenEvent& genEvt, float& WMass, float& TopMass){
   WMass = -999.;
   TopMass = -999.;
   if(!is3jetsSelected(jets)) return false;
   
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	if((jets[i].Eta())<JetEtaThreshold_ && jets[i].Pt()>JetPtThreshold_)
   		tljets.push_back((TLorentzVector)jets[i]);
   }
		 
   if(!genEvt.isSemiLeptonic()) return false;

   // Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2
   std::vector<TLorentzVector> quarks;
   quarks.push_back(genEvt.hadronicDecayQuark());
   quarks.push_back(genEvt.hadronicDecayQuarkBar());
   quarks.push_back(genEvt.hadronicDecayB());
   JetPartonMatching GenMatch(quarks, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
   for(unsigned int c=0;c<GenMatch.getNumberOfAvailableCombinations();c++){
   	if(GenMatch.getNumberOfUnmatchedPartons(c)==0){
		WMass = ((TLorentzVector) (tljets[GenMatch.getMatchForParton(0,c)]+tljets[GenMatch.getMatchForParton(0,c)])).M();
		TopMass = ((TLorentzVector) (tljets[GenMatch.getMatchForParton(0,c)]+tljets[GenMatch.getMatchForParton(1,c)]+tljets[GenMatch.getMatchForParton(2,c)])).M();
		return(true);
	}
    }
    return(false);
}
