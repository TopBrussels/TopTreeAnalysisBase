#include "../interface/TtSemiMuJetCombination.h"

using namespace std;
using namespace stdcomb;

TtSemiMuJetCombination::TtSemiMuJetCombination(){
    jets.clear();
    muon = TRootMuon();
    matchingAlgo_ = 2 ;
    useMaxDist_ = true ;
    useDeltaR_ = true ;
    maxDist_ = 0.3 ;
    TopMass_ = 172.4 ;
    WMass_ = 80.4 ;
    blMass_ = 91.1 ;
    LepTopMassResol_ = 15.6 ;
    LepblMassResol_ = 36 ;
    HadTopMassResol_ = 12.5 ;
    HadWMassResol_ = 7.6 ;
    NjetsForComb_ = 6 ;
    JetPtThreshold_ = 30 ;
    JetEtaThreshold_ = 2.4 ;
}

TtSemiMuJetCombination::TtSemiMuJetCombination(const TtSemiMuJetCombination & a){
    jets.clear();
    for(unsigned int i=0;i<a.jets.size();i++) jets.push_back(a.jets[i]);
    muon = a.muon;
    matchingAlgo_ = a.matchingAlgo_;
    useMaxDist_ = a.useMaxDist_;
    useDeltaR_ = a.useDeltaR_;
    maxDist_ = a.maxDist_;
    TopMass_ = a.TopMass_;
    WMass_ = a.WMass_;
    blMass_ = a.blMass_;
    LepTopMassResol_ = a.LepTopMassResol_;
    LepblMassResol_ = a.LepblMassResol_;
    HadTopMassResol_ = a.HadTopMassResol_;
    HadWMassResol_ = a.HadWMassResol_;
    NjetsForComb_ = a.NjetsForComb_;
    JetPtThreshold_ = a.JetPtThreshold_;
    JetEtaThreshold_ = a.JetEtaThreshold_;
}

TtSemiMuJetCombination::~TtSemiMuJetCombination(){}


void TtSemiMuJetCombination::SetConfiguration(double TopMass, double WMass, double blMass, double LepTopMassResol, double LepblMassResol, double HadWMassResol, double HadTopMassResol){
	TopMass_ = TopMass;
	WMass_ = WMass;
	blMass_ = blMass;
	LepTopMassResol_ = LepTopMassResol;
	LepblMassResol_ = LepblMassResol;
	HadWMassResol_ = HadWMassResol;
	HadTopMassResol_ = HadTopMassResol;
}

   
void TtSemiMuJetCombination::SetParameters(unsigned int NjetsForComb, double JetPtThreshold, double JetEtaThreshold){
	NjetsForComb_ = NjetsForComb;
	JetPtThreshold_ = JetPtThreshold;
	JetEtaThreshold_ = JetEtaThreshold;
}


void TtSemiMuJetCombination::SetMatchingParameters(int matchingAlgo, bool useMaxDist, bool useDeltaR, double maxDist){
	matchingAlgo_ = matchingAlgo;
	useMaxDist_ = useMaxDist;
	useDeltaR_ = useDeltaR;
	maxDist_ = maxDist;
}

void TtSemiMuJetCombination::SetMuon(const TRootMuon & init_muon){
   muon = init_muon;
}

void TtSemiMuJetCombination::SetJets(const std::vector<TRootJet>& init_jets){
   jets.clear();
   jets.reserve( init_jets.size() );
   for(std::vector<TRootJet>::const_iterator jet_iter = init_jets.begin(); jet_iter != init_jets.end(); ++jet_iter)
   {
 	 if(fabs(jet_iter->Eta())<JetEtaThreshold_ && jet_iter->Pt()>JetPtThreshold_) jets.push_back(*jet_iter);
   }
}

TtSemiMuJetCombination::TtSemiMuJetCombination(const std::vector<TRootJet>& init_jets, const TRootMuon & init_muon,unsigned int NjetsForComb, double JetPtThreshold, double JetEtaThreshold){
   SetParameters(NjetsForComb,JetPtThreshold,JetEtaThreshold);
   int verboselevel_ = 0;
   jets.reserve( init_jets.size() );
   
   if(verboselevel_>0) std::cout<<"- Initial number of jets before the object selection criteria : "<<init_jets.size()<<std::endl;
   
   // PAT jets :
   for(std::vector<TRootJet>::const_iterator jet_iter = init_jets.begin(); jet_iter != init_jets.end(); ++jet_iter)
   {
 	 if(fabs(jet_iter->Eta())<JetEtaThreshold_ && jet_iter->Pt()>JetPtThreshold_) jets.push_back(*jet_iter);
   }

   if(verboselevel_>0) std::cout<<"- Initial number of jets passing the object selection criteria : "<<jets.size()<<std::endl;

   if(NjetsForComb_<jets.size()) jets.resize(NjetsForComb_);

   if(verboselevel_>0) std::cout<<"- Nb of jets considered for the jet selection procedure : "<<jets.size()<<std::endl;
   
   muon = init_muon;
}


std::vector<TRootJet> TtSemiMuJetCombination::MCMatchedCombination(const TRootGenEvent& genEvt, double& ChiSquare){
   std::vector<TRootJet> genselectedjets;
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	tljets.push_back((TLorentzVector)jets[i]);
   }

   if(jets.size()<4){
      cout<<"Number of jets in MCMatchedCombination <4"<<endl;
      return genselectedjets;
   }

   if(genEvt.isSemiLeptonic(TRootGenEvent::kMuon))
   {
   	// Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2, Leptonic b  = 3;
   	std::vector<TLorentzVector> quarks;
	quarks.push_back(genEvt.hadronicDecayQuark());
	quarks.push_back(genEvt.hadronicDecayQuarkBar());
	quarks.push_back(genEvt.hadronicDecayB());
	quarks.push_back(genEvt.leptonicDecayB());

	JetPartonMatching GenMatch(quarks, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	TRootJet myjet;
	for(unsigned int i=0;i<quarks.size();i++)
	{
		((GenMatch.getMatchForParton(i,0) < 0) ? genselectedjets.push_back(myjet) : genselectedjets.push_back(jets[GenMatch.getMatchForParton(i,0)]) );
	}
   }
   unsigned int *Permutation = new unsigned int[4];
   for(unsigned int i=0;i<4;i++) Permutation[i]=i;
   ChiSquare = ChiSquareMatching( genselectedjets, muon , Permutation);
   return(genselectedjets);
}

std::vector<TRootJet> TtSemiMuJetCombination::MCMatchedCombination(const TRootNPGenEvent& genEvt, double& ChiSquare){
   std::vector<TRootJet> genselectedjets;
   std::vector<TRootJet> genselectedjets_temp;
   double ChiSquare_temp = 99999999;
  
   if(jets.size()<3){
      cout<<"Number of jets in MCMatchedCombination <3"<<endl;
      return genselectedjets;
   }
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
          tljets.push_back((TLorentzVector)jets[i]);
   }
		
   vector<TRootGenTop> hadronicTops;
   for(unsigned int i=0;i<genEvt.tops().size();i++) if(genEvt.tops()[i].isHadronic()) hadronicTops.push_back(genEvt.tops()[i]);
   std::sort(hadronicTops.begin(),hadronicTops.end(),HighestPtGenTop());
	 
   for(unsigned int i=0;i<hadronicTops.size();i++){
        genselectedjets_temp.clear();
   	// Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2
   	std::vector<TLorentzVector> quarks;
	quarks.push_back(hadronicTops[0].quark());
	quarks.push_back(hadronicTops[0].quarkBar());
	quarks.push_back(hadronicTops[0].bquark());
	JetPartonMatching GenMatch(quarks, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	for(unsigned int i=0;i<quarks.size();i++)
	{
		if(GenMatch.getMatchForParton(i,0) >0) genselectedjets_temp.push_back(jets[GenMatch.getMatchForParton(i,0)]);
	}
   unsigned int *Permutation = new unsigned int[3];
   for(unsigned int i=0;i<3;i++) Permutation[i]=i;
    
   ChiSquare_temp = 99999999;
   if(genselectedjets_temp.size()>2){
      double ChiSquare_tmp = 99999998;
      do{
       // if(verboselevel_>3) std::cout<<"--- ChiSquare matching : Permutations : "<<comb[0]<<"/"<<comb[1]<<"/"<<comb[2]<<"/"<<comb[3]<<std::endl;
       ChiSquare_tmp = ChiSquareMatchingHadTop(genselectedjets_temp,muon,Permutation);                   // Second ChiSq matching (without neutrino involved)
       if(ChiSquare_tmp<ChiSquare_temp){
       	   ChiSquare_temp = ChiSquare_tmp;
       }
      }while(next_permutation(Permutation,Permutation+3));
    }
    if(genselectedjets_temp.size()>genselectedjets.size() || genselectedjets.size()==0){
      ChiSquare = ChiSquare_temp;
      genselectedjets.clear();
      genselectedjets = genselectedjets_temp;
    }
    
   }
   
   return(genselectedjets);
}

bool TtSemiMuJetCombination::isMCMatched(const std::vector<TRootJet> jets,const TRootGenEvent& genEvt){
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	tljets.push_back((TLorentzVector)jets[i]);
   }
		 
   if(genEvt.isSemiLeptonic(TRootGenEvent::kMuon) && jets.size()==4)
   {
   	// Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2, Leptonic b  = 3;
   	std::vector<TLorentzVector> quarks;
   	std::vector<TLorentzVector> lepB;
	quarks.push_back(genEvt.hadronicDecayQuark());
	quarks.push_back(genEvt.hadronicDecayQuarkBar());
	quarks.push_back(genEvt.hadronicDecayB());
	quarks.push_back(genEvt.leptonicDecayB());
	JetPartonMatching GenMatchHadTop(quarks, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	JetPartonMatching GenMatchLepB(lepB, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	bool return_ = false;
        (GenMatchHadTop.getNumberOfUnmatchedPartons()==0 && GenMatchLepB.getNumberOfUnmatchedPartons()==0) ? return_ = true : return_ = false;
	return return_;
   }
   return false;
}

bool TtSemiMuJetCombination::isMCMatched(const TRootGenEvent& genEvt){
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
   	tljets.push_back((TLorentzVector)jets[i]);
   }
		 
   if(genEvt.isSemiLeptonic(TRootGenEvent::kMuon) && jets.size()==4)
   {
   	// Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2, Leptonic b  = 3;
   	std::vector<TLorentzVector> quarks;
   	std::vector<TLorentzVector> lepB;
	quarks.push_back(genEvt.hadronicDecayQuark());
	quarks.push_back(genEvt.hadronicDecayQuarkBar());
	quarks.push_back(genEvt.hadronicDecayB());
	quarks.push_back(genEvt.leptonicDecayB());
	JetPartonMatching GenMatchHadTop(quarks, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	JetPartonMatching GenMatchLepB(lepB, tljets, matchingAlgo_, useMaxDist_, useDeltaR_, maxDist_);
	bool return_ = false;
        (GenMatchHadTop.getNumberOfUnmatchedPartons()==0 && GenMatchLepB.getNumberOfUnmatchedPartons()==0) ? return_ = true : return_ = false;
	return return_;
   }
   return false;
}

bool TtSemiMuJetCombination::isMCMatchedHadTop(const std::vector<TRootJet> jets, const TRootNPGenEvent& genEvt){
   if(jets.size()!=3){
     cout<<"Number of jets for the MCMatchedHadTop !=3"<<endl;
     return false;
   }
   std::vector<TLorentzVector> tljets;
   for(unsigned int i=0;i<jets.size();i++){
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
	if(GenMatch.getNumberOfUnmatchedPartons()==0)  return  true ;
   }
   return false;
}



std::vector<TRootJet> TtSemiMuJetCombination::ComputeBestCombination(double& ChiSqOut, int method){

   int verboselevel_ = 0;
   std::vector<TRootJet>  ChiSqMatchSelectedJets;
   std::vector<unsigned int> matchidx;
   
   if(jets.size()<4){
      cout<<"There is not 4 jets available for the combination"<<endl;
      return ChiSqMatchSelectedJets;
   }
   
   unsigned int NofJets = 0 ;
   if(method==0) NofJets = 3;
   if(method==1) NofJets = 4;
   unsigned int *numbers = new unsigned int[jets.size()];
   for(unsigned int i=0;i<jets.size();i++) numbers[i]=i;
   unsigned int *comb = new unsigned int[NofJets];
   for(unsigned int i=0;i <NofJets;i++) comb[i]=i;
   unsigned int *Permutation = new unsigned int[NofJets];
   for(unsigned int i=0;i<NofJets;i++) Permutation[i]=i;
   
   double ChiSquare = 99999999.,ChiSquare_tmp = 99999998.;
   
   do
   {
	if(verboselevel_>1)
	{
		std::cout<<"-- Enter loop for jet combination "<<std::endl;
		if(NofJets == 3 ) std::cout<<"-- Jet combination considered : "<<comb[0]<<"/"<<comb[1]<<"/"<<comb[2]<<std::endl;
		if(NofJets == 4 ) std::cout<<"-- Jet combination considered : "<<comb[0]<<"/"<<comb[1]<<"/"<<comb[2]<<"/"<<comb[3]<<std::endl;
	}

   	ChiSquare_tmp = 99999998.;

	do
	{
		if(verboselevel_>3){
		  if(NofJets == 3) std::cout<<"--- ChiSquare matching : Permutations : "<<comb[0]<<"/"<<comb[1]<<"/"<<comb[2]<<std::endl;
		  if(NofJets == 4) std::cout<<"--- ChiSquare matching : Permutations : "<<comb[0]<<"/"<<comb[1]<<"/"<<comb[2]<<"/"<<comb[3]<<std::endl;
		}
		if(method == 0)  ChiSquare_tmp = ChiSquareMatchingHadTop(jets,muon,comb); 
		if(method == 1)  ChiSquare_tmp = ChiSquareMatching(jets,muon,comb); 
		if(ChiSquare_tmp<ChiSquare)
		{
			for(unsigned int i=0;i<NofJets;i++) Permutation[i] = comb[i];
			ChiSquare = ChiSquare_tmp;
		}
	}
	while(next_permutation(comb,comb+NofJets));
   }
   while(next_combination(numbers,numbers+jets.size(),comb,comb+NofJets));
   
   if(verboselevel_>2){
     if(NofJets == 3) std::cout<<"--- Min ChiSquare : "<<ChiSquare<<" / Permutations : "<<Permutation[0]<<"/"<<Permutation[1]<<"/"<<Permutation[2]<<std::endl;
     if(NofJets == 4) std::cout<<"--- Min ChiSquare : "<<ChiSquare<<" / Permutations : "<<Permutation[0]<<"/"<<Permutation[1]<<"/"<<Permutation[2]<<"/"<<Permutation[3]<<std::endl;
   }
   if(Permutation[0]<jets.size()) ChiSqMatchSelectedJets.push_back(jets[Permutation[0]]);
   if(Permutation[1]<jets.size()) ChiSqMatchSelectedJets.push_back(jets[Permutation[1]]); 
   if(Permutation[2]<jets.size()) ChiSqMatchSelectedJets.push_back(jets[Permutation[2]]);
   if( NofJets == 4 &&  Permutation[3]<jets.size()) ChiSqMatchSelectedJets.push_back(jets[Permutation[3]]);

   //delete
   delete numbers;
   delete comb;
   delete Permutation;


   // Matching index : Hadronic Q  = 0, Hadronic Q' = 1, Hadronic b  = 2, Leptonic b  = 3;
   //->check if it's ordered
   ChiSqOut = ChiSquare; // value modified
   return (ChiSqMatchSelectedJets);
}



double TtSemiMuJetCombination::ChiSquareMatchingHadTop( std::vector<TRootJet> &jets, const TRootMuon &muon, unsigned int *Permutation) {
	
        int verboselevel_ = 0;
	double ChiSquare=0, HadWChiSquare=0, HadTopChiSquare=0;
	HadWChiSquare   = pow(((jets[Permutation[0]]+jets[Permutation[1]]).M()-WMass_),2)/pow(HadWMassResol_,2);
	HadTopChiSquare = pow(((jets[Permutation[0]]+jets[Permutation[1]]+jets[Permutation[2]]).M()-TopMass_),2)/pow(HadTopMassResol_,2);
	if(isnan(HadWChiSquare+HadTopChiSquare) || isinf(HadWChiSquare+HadTopChiSquare)) ChiSquare = 99999;
	else ChiSquare = HadWChiSquare+HadTopChiSquare;
	if(verboselevel_>2)
	{
		std::cout<<"ChiSquare = "<<ChiSquare<<std::endl;
		std::cout<<"Associated had W mass = "<<(jets[Permutation[0]]+jets[Permutation[1]]).M()<<std::endl;
		std::cout<<"Associated had top mass = "<<(jets[Permutation[0]]+jets[Permutation[1]]+jets[Permutation[2]]).M()<<std::endl;
	}
	return ChiSquare;
}


double TtSemiMuJetCombination::ChiSquareMatching( std::vector<TRootJet> &jets, const TRootMuon &muon, unsigned int *Permutation) {
	
        int verboselevel_ = 0;
	double ChiSquare=0, HadWChiSquare=0, HadTopChiSquare=0, LepblChiSquare=0;
	
	HadWChiSquare   = pow(((jets[Permutation[0]]+jets[Permutation[1]]).M()-WMass_),2)/pow(HadWMassResol_,2);
	HadTopChiSquare = pow(((jets[Permutation[0]]+jets[Permutation[1]]+jets[Permutation[2]]).M()-TopMass_),2)/pow(HadTopMassResol_,2);
	LepblChiSquare  = pow(((jets[Permutation[3]]+muon).M()-blMass_),2)/pow(LepblMassResol_,2);
	
	if(isnan(HadWChiSquare+HadTopChiSquare+LepblChiSquare) || isinf(HadWChiSquare+HadTopChiSquare+LepblChiSquare)) ChiSquare = 99999;
	else ChiSquare = HadWChiSquare+HadTopChiSquare+LepblChiSquare;
	
	if(verboselevel_>2)
	{
		std::cout<<"ChiSquare = "<<ChiSquare<<std::endl;
		std::cout<<"Associated had W mass = "<<(jets[Permutation[0]]+jets[Permutation[1]]).M()<<std::endl;
		std::cout<<"Associated had top mass = "<<(jets[Permutation[0]]+jets[Permutation[1]]+jets[Permutation[2]]).M()<<std::endl;
		std::cout<<"Associated lep b+l mass = "<<(jets[Permutation[3]]+muon).M()<<std::endl;
	}
	return ChiSquare;
}
