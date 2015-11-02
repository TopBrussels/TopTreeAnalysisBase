#include "../interface/Selection.h"

//____CONSTRUCTORS______________________________________________________//

Selection::Selection() {
  rho_ = 0;
  elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
  setJetCuts();
  setMuonCuts();
  setDiMuonCuts();
  setElectronCuts();
  setDiElectronCuts();
  setLooseMuonCuts();
  setLooseElectronCuts();
  setLooseDiElectronCuts();
  cutHFHadronEnergyFraction_ = false;
}
/*
 Selection::Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootMET*>& mets_){
 rho_ = 0;
 elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
 setJetCuts();
 setMuonCuts();
 setDiMuonCuts();
 setElectronCuts();
 setDiElectronCuts();
 setLooseMuonCuts();
 setLooseElectronCuts();
 setLooseDiElectronCuts();
 for(unsigned int i=0;i<jets_.size();i++) jets.push_back(jets_[i]);
 for(unsigned int i=0;i<muons_.size();i++) muons.push_back(muons_[i]);
 for(unsigned int i=0;i<mets_.size();i++) mets.push_back(mets_[i]);
 cutHFHadronEnergyFraction_ = false;
 }
 Selection::Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootEelctron*>& electrons_, const std::vector<TRootMET*>& mets_){
 rho_ = 0;
 elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
 setJetCuts();
 setMuonCuts();
 setDiMuonCuts();
 setElectronCuts();
 setDiElectronCuts();
 setLooseMuonCuts();
 setLooseElectronCuts();
 setLooseDiElectronCuts();
 for(unsigned int i=0;i<jets_.size();i++) jets.push_back(jets_[i]);
 for(unsigned int i=0;i<electrons_.size();i++) electrons.push_back(electrons_[i]);
 for(unsigned int i=0;i<mets_.size();i++) mets.push_back(mets_[i]);
 cutHFHadronEnergyFraction_ = false;
 }
*/

Selection::Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_){
  rho_ = 0;
  elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
  setJetCuts();
  setMuonCuts();
  setDiMuonCuts();
  setElectronCuts();
  setDiElectronCuts();
  setLooseMuonCuts();
  setLooseElectronCuts();
  setLooseDiElectronCuts();
  for(unsigned int i=0;i<jets_.size();i++) jets.push_back(jets_[i]);
  for(unsigned int i=0;i<muons_.size();i++) muons.push_back(muons_[i]);
  for(unsigned int i=0;i<electrons_.size();i++) electrons.push_back(electrons_[i]);
  for(unsigned int i=0;i<mets_.size();i++) mets.push_back(mets_[i]);	
	cutHFHadronEnergyFraction_ = false;
}

Selection::Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_, float rho){
  rho_ = rho;
  elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
  setJetCuts();
  setMuonCuts();
  setDiMuonCuts();
  setElectronCuts();
  setDiElectronCuts();
  setLooseMuonCuts();
  setLooseElectronCuts();
  setLooseDiElectronCuts();
  for(unsigned int i=0;i<jets_.size();i++) jets.push_back(jets_[i]);
  for(unsigned int i=0;i<muons_.size();i++) muons.push_back(muons_[i]);
  for(unsigned int i=0;i<electrons_.size();i++) electrons.push_back(electrons_[i]);
  for(unsigned int i=0;i<mets_.size();i++) mets.push_back(mets_[i]);	
	cutHFHadronEnergyFraction_ = false;
}

Selection::Selection(const Selection& s) {
  // copy the objects
  rho_ = s.rho_;
  elecIsoCorrType_ = s.elecIsoCorrType_; // 0: no corr, 1: rho corr, 2: dB corr.
  jets = s.jets;
  electrons = s.electrons;
  muons = s.muons;
  mets = s.mets;
  // copy the cutvalues
  setJetCuts(s.JetPtThreshold_,s.JetEtaThreshold_,s.JetEMFThreshold_,s.n90HitsThreshold_,s.fHPDThreshold_,s.DRJetElectron_,s.DRJetMuon_);
  
  setMuonCuts(s.MuonPtThreshold_,s.MuonEtaThreshold_,s.MuonRelIso_,s.Muond0Cut_,s.MuonDRJetsCut_,s.MuonNMatchedStations_,s.MuonDz_,s.MuonNTrackerLayersWithMeasurement_,s.MuonNValidPixelHits_);
  setDiMuonCuts(s.MuonPtThreshold_,s.MuonEtaThreshold_,s.MuonRelIso_,s.Muond0Cut_);
  
  setLooseMuonCuts(s.JetPtThreshold_,s.JetEtaThreshold_,s.MuonRelIso_);
  
  setElectronCuts(s.ElectronEtThreshold_,s.ElectronEtaThreshold_,s.ElectronRelIso_,s.Electrond0Cut_,s.ElectronMVAId_,s.ElectronDRJetsCut_,s.ElectronMaxMissingHitsCut_);
  setDiElectronCuts(s.ElectronEtThreshold_,s.ElectronEtaThreshold_,s.ElectronRelIso_,s.Electrond0Cut_,s.ElectronMVAId_,9999.,s.ElectronDRJetsCut_,s.ElectronMaxMissingHitsCut_);

  setLooseElectronCuts(s.ElectronLooseEtThreshold_,s.ElectronLooseEtaThreshold_,s.ElectronLooseRelIso_,s.ElectronLooseMVAId_);
  setLooseDiElectronCuts(s.ElectronLooseEtThreshold_,s.ElectronLooseEtaThreshold_,s.ElectronLooseRelIso_,s.ElectronLooseMVAId_);
  
	cutHFHadronEnergyFraction_ = false;
}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

Selection::~Selection() {
  jets.clear();
  electrons.clear();
  muons.clear();
  mets.clear();
}

//______________________________________________________________________//


//____CUT INITIALISATION________________________________________________//

void Selection::setJetCuts(float Pt, float Eta, float EMF, float n90Hits, float fHPD, float dRJetElectron, float dRJetMuon) {
  JetPtThreshold_ = Pt;
  JetEtaThreshold_ = Eta;
  JetEMFThreshold_ = EMF;
  n90HitsThreshold_ = n90Hits;
  fHPDThreshold_ = fHPD;
  DRJetElectron_ = dRJetElectron;
  DRJetMuon_ = dRJetMuon; // only used for PFlow without PF2PAT
}

void Selection::setJetCuts() {
  //setJetCuts(30.,2.4,0.01,1.,0.98,0.3,0.1); // refSelV4 values
  setJetCuts(35.,2.5,0.01,1.,0.98,0.3,0.1);
}

void Selection::setElectronIsoCorrType(int corrType){
  elecIsoCorrType_ = corrType;
  if (elecIsoCorrType_ == 1 && rho_ <= 0) {
    cerr << "If you plan to use the rho correction for the electron relative isolation, please use the correct constructor:" << endl;
    cerr << " - Selection::Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_, float rho)" << endl;
    cerr << " And/or specify a valid (i.e. positive) rho value." << endl;
    exit(1);
  }
}
void Selection::setElectronCuts(float Et, float Eta, float RelIso, float d0, float MVAId_, float DRJets, int MaxMissingHits) {
  ElectronEtThreshold_ = Et;
  ElectronEtaThreshold_ = Eta;
  ElectronRelIso_ = RelIso;
  Electrond0Cut_ = d0;
  ElectronDRJetsCut_ = DRJets;
  ElectronMVAId_ = MVAId_;
  ElectronMaxMissingHitsCut_ = MaxMissingHits;
}

void Selection::setElectronCuts() {
  setElectronCuts(30,2.5,0.1,0.02,0.,0.3,0); // refSel 2012 values
}


void Selection::setDiElectronCuts(float Et, float Eta, float RelIso, float d0, float MVAId, float DistVzPVz, float DRJets, int MaxMissingHits) {
  ElectronEtThreshold_ = Et;
  ElectronEtaThreshold_ = Eta;
  ElectronRelIso_ = RelIso;
  Electrond0Cut_ = d0;
  ElectronDRJetsCut_ = DRJets;
  ElectronMVAId_ = MVAId;
  ElectronMaxMissingHitsCut_ = MaxMissingHits;
}

void Selection::setDiElectronCuts() {
  setDiElectronCuts(20,2.5,0.15,0.04,0.,1,0.3,1); // refSel 2012 values
}



void Selection::setLooseElectronCuts(float Et, float Eta, float RelIso, float MVAId) {
  ElectronLooseEtThreshold_ = Et;
  ElectronLooseEtaThreshold_ = Eta;
  ElectronLooseRelIso_ = RelIso;
  ElectronLooseMVAId_ = MVAId;
}

void Selection::setLooseElectronCuts() {
  setLooseElectronCuts(20,2.5,0.15,0.); // refSel 2012 values
}


void Selection::setLooseDiElectronCuts(float Et, float Eta, float RelIso, float MVAId) {
  ElectronLooseEtThreshold_ = Et;
  ElectronLooseEtaThreshold_ = Eta;
  ElectronLooseRelIso_ = RelIso;
  ElectronLooseMVAId_ = MVAId;
}

void Selection::setLooseDiElectronCuts() {
  setLooseDiElectronCuts(20,2.5,0.15,0.); // refSel 2012 values
}


void Selection::setMuonCuts(float Pt, float Eta, float RelIso, float d0, float DRJets, int NMatchedStations, float Dz, int NTrackerLayersWithMeas, int NValidPixelHits) {
  MuonPtThreshold_ = Pt;
  MuonEtaThreshold_ = Eta;
  MuonRelIso_ = RelIso;
  Muond0Cut_ = d0;
  MuonDRJetsCut_ = DRJets;
  MuonNMatchedStations_ = NMatchedStations;
  MuonDz_ = Dz;
  MuonNTrackerLayersWithMeasurement_ = NTrackerLayersWithMeas;
  MuonNValidPixelHits_ = NValidPixelHits;
}

void Selection::setMuonCuts() {
  setMuonCuts(26,2.1,0.12,0.2,0.3,1,0.5,5,0); // refSel 2012 values
}

void Selection::setDiMuonCuts(float Pt, float Eta, float RelIso, float d0) {
  MuonPtThreshold_ = Pt;
  MuonEtaThreshold_ = Eta;
  MuonRelIso_ = RelIso;
  Muond0Cut_ = d0;
 
}

void Selection::setDiMuonCuts() {
  setDiMuonCuts(20,2.4,0.20,999.); // 2012 values
}

void Selection::setLooseMuonCuts(float Pt, float Eta, float RelIso) {
  MuonLoosePtThreshold_ = Pt;
  MuonLooseEtaThreshold_ = Eta;
  MuonLooseRelIso_ = RelIso;
}

void Selection::setLooseMuonCuts() {
  setLooseMuonCuts(10,2.5,0.2); // refSelV4 (mu+el) values
}

//______________________________________________________________________//

//__EXTRA METHODS_______________________________________________________//

//______________________________________________________________________//

//____SELECTION GETTERS_________________________________________________//

// ______________JETS_________________________________________________//

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, bool applyJetID) const{
  std::vector<TRootJet*> selectedJets;
  for(unsigned int i=0;i<jets.size();i++)
    {
      TRootJet* init_jet = (TRootJet*) jets[i];
      if (init_jet->jetType() == 1)
	{ // CaloJets
	  const TRootCaloJet* CaloJet = static_cast<const TRootCaloJet*>(init_jet);
	  if(fabs(CaloJet->Eta())<EtaThr && CaloJet->Pt()>PtThr)
	    {
	      if( applyJetID )
		{
		  if ( CaloJet->ecalEnergyFraction() > JetEMFThreshold_)
		    {
		      if (CaloJet->fHPD() < fHPDThreshold_ )
			{
			  if (CaloJet->n90Hits()> n90HitsThreshold_ )
			    {
			      selectedJets.push_back(init_jet);
			    }
			}
		    }
		}
	      else selectedJets.push_back(init_jet);
	    }
	}
      else if (init_jet->jetType() == 2)
	{ // PFJets
	  const TRootPFJet* PFJet = static_cast<const TRootPFJet*>(init_jet);
      
	  //cout << "jet pT:  " << PFJet->Pt() << "  cutValue: " << PtThr << endl;
	  //cout << "jet Eta:  " << fabs(PFJet->Eta()) << "  cutValue: " << EtaThr << endl;
	  //cout << "jet nConstituents:  " << PFJet->nConstituents() << endl;
	  //cout << "jet chargedEmEnergyFraction:  " << PFJet->chargedEmEnergyFraction() << endl;
	  //cout << "jet neutralHadronEnergyFraction:  " << PFJet->neutralHadronEnergyFraction() << endl;
          //if(cutHFHadronEnergyFraction_) cout << "jet HFHadronEnergyFraction:  " << PFJet->HFHadronEnergyFraction() << endl;
	  //cout << "jet neutralEmEnergyFraction:  " << PFJet->neutralEmEnergyFraction() << endl;
	  //cout << "jet chargedHadronEnergyFraction:  " << PFJet->chargedHadronEnergyFraction() << endl;
	  //cout << "jet chargedMultiplicity:  " << PFJet->chargedMultiplicity() << endl;
      	  //if(cutHFHadronEnergyFraction_) cout << "sum of jet neutralHadronEnergyFraction and HFHadronEnergyFraction: " << (PFJet->neutralHadronEnergyFraction() + PFJet->HFHadronEnergyFraction()) << endl;
      
	  if( fabs(PFJet->Eta())<EtaThr && PFJet->Pt()>PtThr )
	    {
	      if ( applyJetID )
		{
		  if (PFJet->nConstituents() > 1 )
		    if ((!cutHFHadronEnergyFraction_ && (PFJet->neutralHadronEnergyFraction() < 0.99))  || (cutHFHadronEnergyFraction_ && ((PFJet->neutralHadronEnergyFraction() + PFJet->HFHadronEnergyFraction()) < 0.99 )))
		      if (PFJet->neutralEmEnergyFraction() < 0.99 )
			if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedEmEnergyFraction() < 0.99 )
			  if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedHadronEnergyFraction() > 0) 
			    if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedMultiplicity() > 0)
			      {
				selectedJets.push_back(init_jet);
			      }
		}
	      else selectedJets.push_back(init_jet);
	    }
	}
      else if (init_jet->jetType() == 3)
	{ // JPTJets
	  const TRootJPTJet* JPTJet = static_cast<const TRootJPTJet*>(init_jet);
	  if(fabs(JPTJet->Eta())<JetEtaThreshold_ && JPTJet->Pt()>JetPtThreshold_)
	    {
	      if( applyJetID )
		{
		  if ( JPTJet->ecalEnergyFraction() > JetEMFThreshold_)
		    {
		      if (JPTJet->fHPD() < fHPDThreshold_ )
			{
			  if (JPTJet->n90Hits()> n90HitsThreshold_ )
			    {
			      selectedJets.push_back(init_jet);
			    }
			}
		    }
		}
	      else selectedJets.push_back(init_jet);
	    }
	}
    }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, vector<TLorentzVector*>& lepton, float dRLeptonJet, bool applyJetID) const {
  std::vector<TRootJet*> init_jets = GetSelectedJets(PtThr,EtaThr,applyJetID);
  std::vector<TRootJet*> selectedJets;
  for (unsigned int i=0; i<init_jets.size(); i++) {
    float minDrElJet = 9999999999.;
    for (unsigned int j=0; j<lepton.size(); j++) {
      TLorentzVector* l = (TLorentzVector*) lepton[j];
      float dr = l->DeltaR(*init_jets[i]);
      if ( dr < minDrElJet ) minDrElJet = dr;
    }
  
    if (minDrElJet >= dRLeptonJet){
      selectedJets.push_back(init_jets[i]);
      std::cout << " Jet not surviving the lepton-jet deltaR veto .... " << std::endl;
    }
  }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
  return selectedJets;
}

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, vector<TRootMuon*>& muons, float dRLeptonJet, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<muons.size(); i++)
    leptons.push_back((TLorentzVector*)muons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetMuon_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedJets(float PtThr, float EtaThr, vector<TRootElectron*>& electrons, float dRLeptonJet, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<electrons.size(); i++)
    leptons.push_back((TLorentzVector*)electrons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetElectron_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedJets(vector<TRootMuon*>& muons, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<muons.size(); i++)
    leptons.push_back((TLorentzVector*)muons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetMuon_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedJets(vector<TRootElectron*>& electrons, bool applyJetID) const {
  std::vector<TLorentzVector*> leptons;
  for (unsigned int i=0; i<electrons.size(); i++)
    leptons.push_back((TLorentzVector*)electrons[i]);
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,leptons,DRJetElectron_,applyJetID);
}


std::vector<TRootJet*> Selection::GetSelectedJets(bool applyJetID) const{
  return GetSelectedJets(JetPtThreshold_,JetEtaThreshold_,applyJetID);
}

std::vector<TRootJet*> Selection::GetSelectedBJets(const std::vector<TRootJet*>& seljets, int& btagAlgo, float& btagCut) const{
  std::vector<TRootJet*> bjets;
  Float_t btagDisc = 0;
  for(unsigned int i = 0; i<seljets.size(); i++) {
    if(     btagAlgo == 0) btagDisc = seljets[i]->btag_trackCountingHighEffBJetTags();
    else if(btagAlgo == 1) btagDisc = seljets[i]->btag_trackCountingHighPurBJetTags();
    else if(btagAlgo == 2) btagDisc = seljets[i]->btag_jetProbabilityBJetTags();
    else if(btagAlgo == 3) btagDisc = seljets[i]->btag_jetBProbabilityBJetTags();
    else if(btagAlgo == 4) btagDisc = seljets[i]->btag_simpleSecondaryVertexHighEffBJetTags();
    else if(btagAlgo == 5) btagDisc = seljets[i]->btag_simpleSecondaryVertexHighPurBJetTags();
    else if(btagAlgo == 6) btagDisc = seljets[i]->btag_combinedSecondaryVertexBJetTags();
    else if(btagAlgo == 7) btagDisc = seljets[i]->btag_combinedSecondaryVertexMVABJetTags();
    else                   btagDisc = -999999;
    if(btagDisc>btagCut) bjets.push_back(seljets[i]);
  }
  return bjets;
}

// ______________MUONS________________________________________________//

std::vector<TRootMuon*> Selection::GetSelectedMuons(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<muons.size();i++){
    
    //float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
    float reliso = (muons[i]->chargedHadronIso() + max( 0.0, muons[i]->neutralHadronIso() + muons[i]->photonIso() - 0.5*muons[i]->puChargedHadronIso() ) ) / muons[i]->Pt(); // dBeta corrected

    //    cout << "mu global:  " << muons[i]->isGlobalMuon() << endl;
    //    cout << "mu globalMuPromptTight:  " << muons[i]->idGlobalMuonPromptTight() << endl;
    //    cout << "mu pT:  " << muons[i]->Pt() << "  cutValue: " << PtThr << endl;
    //    cout << "mu Eta:  " << fabs(muons[i]->Eta()) << "  cutValue: " << EtaThr << endl;
    //    cout << "mu reliso:  " << reliso << "  cutValue: " << MuonRelIso << endl;
    //    cout << "mu puChargedHad Iso: "<< muons[i]->puChargedHadronIso() << endl;
    //    cout << "mu d0:  " << fabs(muons[i]->d0()) << "  cutValue: " << Muond0Cut_ << endl;
    //    cout << "mu nMatches:  " << muons[i]->nofMatches() << "  cutValue: " << MuonNMatches_ << endl;
    //    cout << "mu nPixLayers:  " << muons[i]->nofValidPixelHits() << "  cutValue: " << MuonNTrackerLayersWithMeasurement_ << endl;
    if(muons[i]->idGlobalMuonPromptTight() //&& muons[i]->isPFMuon()
       && muons[i]->Pt()>PtThr 
       && fabs(muons[i]->Eta())<EtaThr 
       && reliso < MuonRelIso 
       && fabs(muons[i]->d0()) <Muond0Cut_
       && muons[i]->nofMatchedStations()>MuonNMatchedStations_ 
       && muons[i]->nofTrackerLayersWithMeasurement() > MuonNTrackerLayersWithMeasurement_
       && muons[i]->nofValidPixelHits() > MuonNValidPixelHits_) {
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuons() const{
  return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_);
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(float PtThr, float EtaThr,float MuonRelIso, vector<TRootJet*> selJets) const{
  std::vector<TRootMuon*> selectedMuons;
  std::vector<TRootMuon*> init_muons=GetSelectedMuons(PtThr,EtaThr,MuonRelIso);
  for(unsigned int i=0;i<init_muons.size();i++){
    float mindRMuJet = 999999.;
    TRootJet* jet;
    for(unsigned int j=0;j<selJets.size();j++) {
      jet = selJets.at(j);
      float dRMuJet = init_muons[i]->DeltaR(*jet);
      if(dRMuJet < mindRMuJet) mindRMuJet = dRMuJet;
    }
    //    cout << "mu mindRMuJet:  " << mindRMuJet << "  cutValue: " << MuonDRJetsCut_ << "   Pt: " << init_muons[i]->Pt() << endl;
    if(mindRMuJet > MuonDRJetsCut_){
      selectedMuons.push_back(init_muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso, TRootVertex* vertex, vector<TRootJet*>& selJets) const {
  std::vector<TRootMuon*> selectedMuons;
  std::vector<TRootMuon*> init_muons=GetSelectedMuons(PtThr,EtaThr,MuonRelIso,selJets);
  for(unsigned int i=0;i<init_muons.size();i++){
    TRootMuon* mu = (TRootMuon*) init_muons[i];
    //    cout << "mu dz:  " << fabs(mu->dz()) << "  cutValue: " << MuonDz_ << endl;
    if ( fabs(mu->dz()) < MuonDz_) {
      //      cout << "OK" << endl;
      selectedMuons.push_back(init_muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(TRootVertex* vertex) const {
  std::vector<TRootJet*> empty;
  return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_,vertex,empty);
}

std::vector<TRootMuon*> Selection::GetSelectedMuons(TRootVertex* vertex, vector<TRootJet*>& selJets) const {
  return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_,vertex,selJets);
}


std::vector<TRootMuon*> Selection::GetSelectedDiMuons(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<muons.size();i++){
    float reliso = (muons[i]->chargedHadronIso() + max( 0.0, muons[i]->neutralHadronIso() + muons[i]->photonIso() - 0.5*muons[i]->puChargedHadronIso() ) ) / muons[i]->Pt(); // dBeta corrected
    if((muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon())
       && muons[i]->Pt()>PtThr 
       && fabs(muons[i]->Eta())<EtaThr 
       && reliso < MuonRelIso )
    {
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedDiMuons() const{
  return GetSelectedDiMuons(MuonPtThreshold_,MuonEtaThreshold_,MuonRelIso_);
}

std::vector<TRootMuon*> Selection::GetSelectedLooseMuons(float PtThr, float EtaThr,float MuonRelIso) const{
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<muons.size();i++){
    //float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
    float reliso = (muons[i]->chargedHadronIso() + max( 0.0, muons[i]->neutralHadronIso() + muons[i]->photonIso() - 0.5*muons[i]->puChargedHadronIso() ) ) / muons[i]->Pt(); // dBeta corrected

    //    float reliso = muons[i]->relativeIso03();
    //    cout << "mu global:  " << muons[i]->isGlobalMuon() << endl;
    //    cout << "mu pT:  " << muons[i]->Pt() << "  cutValue: " << PtThr << endl;
    //    cout << "mu Eta:  " << fabs(muons[i]->Eta()) << "  cutValue: " << EtaThr << endl;
    //    cout << "mu reliso:  " << reliso << "  cutValue: " << MuonRelIso << endl;
    if((muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && fabs(muons[i]->Eta())<EtaThr && muons[i]->Pt()>PtThr && reliso < MuonRelIso ) {
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedLooseMuons() const{
  return GetSelectedLooseMuons(MuonLoosePtThreshold_, MuonLooseEtaThreshold_, MuonLooseRelIso_);
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsInvIso(float MuonRelIso, TRootVertex* vertex) const
{
  std::vector<TRootJet*> empty;
  return GetSelectedMuonsInvIso(MuonRelIso, vertex, empty);
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsInvIso(float MuonRelIso, TRootVertex* vertex, vector<TRootJet*>& selJets) const
{
  std::vector<TRootMuon*> init_muons = GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,99999.);
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<init_muons.size();i++)
  {
    float mindRMuJet = 999.;
    for(unsigned int j=0;j<selJets.size();j++) {
      float dRMuJet = init_muons[i]->DeltaR(*selJets.at(j));
      if(dRMuJet < mindRMuJet) mindRMuJet = dRMuJet;
    }
    float reliso = (init_muons[i]->chargedHadronIso() + max( 0.0, init_muons[i]->neutralHadronIso() + init_muons[i]->photonIso() - 0.5*init_muons[i]->puChargedHadronIso() ) ) / init_muons[i]->Pt(); // dBeta corrected
//    float reliso = (init_muons[i]->chargedHadronIso()+init_muons[i]->neutralHadronIso()+init_muons[i]->photonIso())/init_muons[i]->Pt();
    if ( fabs(init_muons[i]->dz()) < MuonDz_ && mindRMuJet > MuonDRJetsCut_ && reliso > MuonRelIso )
      selectedMuons.push_back(init_muons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsNonIso(float PtThr, float EtaThr, vector<TRootJet*>& selJets) const{
  return GetSelectedMuons(PtThr,EtaThr,9999.,selJets);
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsNonIso() const{
  return GetSelectedMuons(MuonPtThreshold_,MuonEtaThreshold_,9999.);
}

std::vector<TRootMuon*> Selection::GetSelectedMuonsIsoRange(float PtThr, float EtaThr,float MuonRelIsoMin, float MuonRelIsoMax, vector<TRootJet*>& selJets) const{
  std::vector<TRootMuon*> init_muons = GetSelectedMuons(PtThr,EtaThr,9999.,selJets);
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0;i<init_muons.size();i++){
    //Compute isolation
    float reliso = (init_muons[i]->chargedHadronIso() + max( 0.0, init_muons[i]->neutralHadronIso() + init_muons[i]->photonIso() - 0.5*init_muons[i]->puChargedHadronIso() ) ) / init_muons[i]->Pt(); // dBeta corrected
    if( MuonRelIsoMin < reliso && reliso < MuonRelIsoMax )
      selectedMuons.push_back(muons[i]);
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}
  
// ______________ELECTRONS____________________________________________//
/*
bool Selection::passConversionRejection(TRootElectron* electron)
{
  bool passed = false;
  if( ( fabs(electron->Dist()) >= 0.02 || fabs(electron->DCot()) >= 0.02 ) && electron->missingHits() == 0)
    {
      //    cout << "Found non-conversion electron with  Dist() = " << electron->Dist() << "  DCot() = " << electron->DCot() << "  missingHits() = " << electron->missingHits() << endl;
      passed = true;
    }
  return passed;
}
*/
bool Selection::foundZCandidate(TRootElectron* electron, std::vector<TRootElectron*>& vetoElectrons, float windowsize)
{
  bool foundZ = false;
  for(unsigned int i=0;i<vetoElectrons.size();i++)
    {
      TRootElectron* el = (TRootElectron*) vetoElectrons[i];
      if( fabs(el->Pt() - electron->Pt()) > 0.001 && fabs(el->Eta() - electron->Eta()) > 0.001 )
	{
	  double zMass = (*electron + *el).M();
	  if( zMass >= (91.-windowsize) && zMass <= (91.+windowsize) )
	    {
	      foundZ = true;
	      //        cout << "Found Z with mass: " << zMass << "  El1: " << el->Pt() << " " << el->Eta() << " | El2: " << electron->Pt() << " " << electron->Eta() << endl;
	    }
	}
    }
  return foundZ;
}

bool Selection::foundZCandidate(TRootMuon* muon, std::vector<TRootMuon*>& vetoMuons, float windowsize)
{
  bool foundZ = false;
  for(unsigned int i=0;i<vetoMuons.size();i++)
    {
      TRootMuon* mu = (TRootMuon*) vetoMuons[i];
      if( fabs(mu->Pt() - muon->Pt()) > 0.001 && fabs(mu->Eta() - muon->Eta()) > 0.001 )
	{
	  double zMass = (*muon + *mu).M();
	  if( zMass >= (91.-windowsize) && zMass <= (91.+windowsize) )
	    {
	      foundZ = true;
	      //        cout << "Found Z with mass: " << zMass << "  mu1: " << mu->Pt() << " " << mu->Eta() << " | mu2: " << muon->Pt() << " " << muon->Eta() << endl;
	    }
	}
    }
  return foundZ;
}

bool Selection::foundZCandidate(std::vector<TRootElectron*>& electrons1, std::vector<TRootElectron*>& electrons2, float windowsize)
{
  bool foundZ = false;
  for(unsigned int j=0;j<electrons1.size();j++)
    {
      for(unsigned int i=0;i<electrons2.size();i++)
  	{
	  TRootElectron* el1 = (TRootElectron*) electrons1[j];
	  TRootElectron* el2 = (TRootElectron*) electrons2[i];
	  if( fabs(el2->Pt() - el1->Pt()) > 0.001 && fabs(el2->Eta() - el1->Eta()) > 0.001 )
	    {	
	      double zMass = (*el1 + *el2).M();
	      if( zMass >= (91.-windowsize) && zMass <= (91.+windowsize) )
		{
		  foundZ = true;
		  //        cout << "Found Z with mass: " << zMass << "  El1: " << el->Pt() << " " << el->Eta() << " | El2: " << electron->Pt() << " " << electron->Eta() << endl;
		}
	    }
	}
    }
  return foundZ;
}

bool Selection::foundZCandidate(std::vector<TRootMuon*>& muons1, std::vector<TRootMuon*>& muons2, float windowsize)
{
  bool foundZ = false;
  for(unsigned int j=0;j<muons1.size();j++)
    {
      for(unsigned int i=0;i<muons2.size();i++)
  	{
	  TRootMuon* mu1 = (TRootMuon*) muons1[j];
	  TRootMuon* mu2 = (TRootMuon*) muons2[i];
	  if( fabs(mu2->Pt() - mu1->Pt()) > 0.001 && fabs(mu2->Eta() - mu1->Eta()) > 0.001 )
	    {	
	      double zMass = (*mu1 + *mu2).M();
	      if( zMass >= (91.-windowsize) && zMass <= (91.+windowsize) )
		{
		  foundZ = true;
		  //        cout << "Found Z with mass: " << zMass << "  mu1: " << mu->Pt() << " " << mu->Eta() << " | mu2: " << muectron->Pt() << " " << muectron->Eta() << endl;
		}
	    }
	}
    }
  return foundZ;
}

std::vector<TRootElectron*> Selection::GetSelectedElectrons(float PtThr, float EtaThr, float ElectronRelIso, bool invertIdCut) const {
  std::vector<TRootElectron*> selectedElectrons;
  //cout << ElectronRelIso << endl;
  for(unsigned int i=0;i<electrons.size();i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];

    // Detector-based relative isolation
    //  float RelIso = (el->caloIso(3)+el->trackerIso(3)) / el->Et();
    // PF relative isolation
    //  float RelIso = (el->chargedHadronIso()+el->neutralHadronIso()+el->photonIso())/el->Pt();
    // PF relative isolation with dBeta correction:
    //  float RelIso = (el->chargedHadronIso()+max(0.0,el->neutralHadronIso()+el->photonIso()-0.5*el->puChargedHadronIso()))/el->Pt();

    double EffectiveArea = 0.;
    
    // HCP 2012 updated for electron conesize = 0.3, taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.4&view=markup
    if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 1.0   ) EffectiveArea = 0.130;
    if (fabs(el->superClusterEta()) >= 1.0   && fabs(el->superClusterEta()) < 1.479 ) EffectiveArea = 0.137;
    if (fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.067;
    if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.089;
    if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.3   ) EffectiveArea = 0.107;
    if (fabs(el->superClusterEta()) >= 2.3   && fabs(el->superClusterEta()) < 2.4   ) EffectiveArea = 0.110;
    if (fabs(el->superClusterEta()) >= 2.4) EffectiveArea = 0.138;

    double isocorr = 0;
    if(elecIsoCorrType_ == 1) // rho correction (default corr)
      isocorr = rho_*EffectiveArea;
    else if(elecIsoCorrType_ == 2) // dB correction
      isocorr = 0.5*el->puChargedHadronIso();
    else if (elecIsoCorrType_ == 0) // no correction
      isocorr = 0.;
    else{
      cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
      cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
      cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
      exit(1);
    }
    // Compute the relative isolation
    float RelIso = (el->chargedHadronIso() + max(0.0 , el->neutralHadronIso() + el->photonIso() - isocorr) )/ el->Pt();
    
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
      if(fabs(el->superClusterEta()) < 1.4442 || fabs(el->superClusterEta()) > 1.5660)
        if(fabs(el->d0()) < Electrond0Cut_)
          if(el->passConversion())
            if( (!invertIdCut && el->mvaTrigId() > ElectronMVAId_) || (invertIdCut && el->mvaTrigId() < ElectronMVAId_) )
              if(el->missingHits() <=  ElectronMaxMissingHitsCut_)
                if(RelIso < ElectronRelIso)
                  selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

/*
//Method including rho correction
std::vector<TRootElectron*> Selection::GetSelectedElectrons(float PtThr, float EtaThr, float ElectronRelIso, float rho) const {
  std::vector<TRootElectron*> selectedElectrons;
  //cout << ElectronRelIso << endl;
  for(unsigned int i=0;i<electrons.size();i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    double EffectiveArea = 0.;
    
    // HCP 2012 updated for electron conesize = 0.3, taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.4&view=markup
    if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 1.0   ) EffectiveArea = 0.130;
    if (fabs(el->superClusterEta()) >= 1.0   && fabs(el->superClusterEta()) < 1.479 ) EffectiveArea = 0.137;
    if (fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.067;
    if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.089;
    if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.3   ) EffectiveArea = 0.107;
    if (fabs(el->superClusterEta()) >= 2.3   && fabs(el->superClusterEta()) < 2.4   ) EffectiveArea = 0.110;
    if (fabs(el->superClusterEta()) >= 2.4) EffectiveArea = 0.138;
    
    
    float RelIso = (el->chargedHadronIso() + max( el->neutralHadronIso() + el->photonIso()  - rho*EffectiveArea, 0.) )/ el->Pt();
    
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
      if(fabs(el->superClusterEta()) < 1.4442 || fabs(el->superClusterEta()) > 1.5660)
        if(fabs(el->d0()) < Electrond0Cut_)
          if(el->passConversion())
            if(el->mvaTrigId() > ElectronMVAId_)
              if(el->missingHits() <=  ElectronMaxMissingHitsCut_)
                if(RelIso < ElectronRelIso_)
                  selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}
*/
std::vector<TRootElectron*> Selection::GetSelectedElectrons() const{
  return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_);
}

/*
//Method including rho correction
std::vector<TRootElectron*> Selection::GetSelectedElectrons(float rho) const{
  return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_, rho);
}
*/
std::vector<TRootElectron*> Selection::GetSelectedElectrons(vector<TRootJet*>& selJets) const{
  return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_, selJets);
}

std::vector<TRootElectron*> Selection::GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso, vector<TRootJet*>& selJets) const{
  std::vector<TRootElectron*> init_electrons = GetSelectedElectrons(EtThr,EtaThr,ElectronRelIso);
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0;i<init_electrons.size();i++){
    float mindRElJet = 999999.;
    TRootJet* jet;
    for(unsigned int j=0;j<selJets.size();j++) {
      jet = selJets.at(j);
      float dRElJet = init_electrons[i]->DeltaR(*jet);
      if(dRElJet < mindRElJet) mindRElJet = dRElJet;
    }
    if(mindRElJet > ElectronDRJetsCut_){
      selectedElectrons.push_back(init_electrons[i]);
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

/*
//Method including rho correction
std::vector<TRootElectron*> Selection::GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso, float rho, vector<TRootJet*>& selJets) const{
  std::vector<TRootElectron*> init_electrons = GetSelectedElectrons(EtThr,EtaThr,ElectronRelIso,rho);
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0;i<init_electrons.size();i++){
    float mindRElJet = 999999.;
    TRootJet* jet;
    for(unsigned int j=0;j<selJets.size();j++) {
      jet = selJets.at(j);
      float dRElJet = init_electrons[i]->DeltaR(*jet);
      if(dRElJet < mindRElJet) mindRElJet = dRElJet;
    }
    if(mindRElJet > ElectronDRJetsCut_){
      selectedElectrons.push_back(init_electrons[i]);
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}
*/

std::vector<TRootElectron*> Selection::GetSelectedDiElectrons(float PtThr, float EtaThr, float ElectronRelIso) const {
  std::vector<TRootElectron*> selectedElectrons;
  //cout << ElectronRelIso << endl;
  for(unsigned int i=0;i<electrons.size();i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];

    // Detector-based relative isolation
    //  float RelIso = (el->caloIso(3)+el->trackerIso(3)) / el->Et();
    // PF relative isolation
    //  float RelIso = (el->chargedHadronIso()+el->neutralHadronIso()+el->photonIso())/el->Pt();
    // PF relative isolation with dBeta correction:
    //  float RelIso = (el->chargedHadronIso()+max(0.0,el->neutralHadronIso()+el->photonIso()-0.5*el->puChargedHadronIso()))/el->Pt();
    
    double EffectiveArea = 0.;
    
    // HCP 2012 updated for electron conesize = 0.3, taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.4&view=markup
    if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 1.0   ) EffectiveArea = 0.130;
    if (fabs(el->superClusterEta()) >= 1.0   && fabs(el->superClusterEta()) < 1.479 ) EffectiveArea = 0.137;
    if (fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.067;
    if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.089;
    if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.3   ) EffectiveArea = 0.107;
    if (fabs(el->superClusterEta()) >= 2.3   && fabs(el->superClusterEta()) < 2.4   ) EffectiveArea = 0.110;
    if (fabs(el->superClusterEta()) >= 2.4) EffectiveArea = 0.138;
    
    double isocorr = 0;
    if(elecIsoCorrType_ == 1) // rho correction (default corr)
      isocorr = rho_*EffectiveArea;
    else if(elecIsoCorrType_ == 2) // dB correction
      isocorr = 0.5*el->puChargedHadronIso();
    else if (elecIsoCorrType_ == 0) // no correction
      isocorr = 0.;
    else{
      cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
      cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
      cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
      exit(1);
    }
    // Compute the relative isolation
    float RelIso = (el->chargedHadronIso() + max( el->neutralHadronIso() + el->photonIso()  - isocorr, 0.) )/ el->Pt();

    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
      if (fabs(el->d0()) < Electrond0Cut_)
        if (el->passConversion())
          if (el->mvaTrigId() > ElectronMVAId_)
            if(el->missingHits() <=  ElectronMaxMissingHitsCut_)
              if(RelIso < ElectronRelIso)
                selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> Selection::GetSelectedDiElectrons() const{
  return GetSelectedDiElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_);
}




//std::vector<TRootElectron*> Selection::GetSelectedLooseElectrons(float PtThr, float EtaThr, float ElectronRelIso, bool vbtfid) const {
std::vector<TRootElectron*> Selection::GetSelectedLooseElectrons(float PtThr, float EtaThr, float ElectronRelIso) const {
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0;i<electrons.size();i++){
    TRootElectron* el = (TRootElectron*) electrons[i];

    // Detector-based relative isolation
    //  float RelIso = (el->caloIso(3)+el->trackerIso(3)) / el->Et();
    // PF relative isolation
    //  float RelIso = (el->chargedHadronIso()+el->neutralHadronIso()+el->photonIso())/el->Pt();
    // PF relative isolation with dBeta correction:
    //  float RelIso = (el->chargedHadronIso()+max(0.0,el->neutralHadronIso()+el->photonIso()-0.5*el->puChargedHadronIso()))/el->Pt();
    
    double EffectiveArea = 0.;
    
    // HCP 2012 updated for electron conesize = 0.3, taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.4&view=markup
    if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 1.0   ) EffectiveArea = 0.130;
    if (fabs(el->superClusterEta()) >= 1.0   && fabs(el->superClusterEta()) < 1.479 ) EffectiveArea = 0.137;
    if (fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.067;
    if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.089;
    if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.3   ) EffectiveArea = 0.107;
    if (fabs(el->superClusterEta()) >= 2.3   && fabs(el->superClusterEta()) < 2.4   ) EffectiveArea = 0.110;
    if (fabs(el->superClusterEta()) >= 2.4) EffectiveArea = 0.138;
    
    double isocorr = 0;
    if(elecIsoCorrType_ == 1) // rho correction (default corr)
      isocorr = rho_*EffectiveArea;
    else if(elecIsoCorrType_ == 2) // dB correction
      isocorr = 0.5*el->puChargedHadronIso();
    else if (elecIsoCorrType_ == 0) // no correction
      isocorr = 0.;
    else{
      cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
      cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
      cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
      exit(1);
    }
    // Compute the relative isolation
    float RelIso = (el->chargedHadronIso() + max( el->neutralHadronIso() + el->photonIso()  - isocorr, 0.) )/ el->Pt();

    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
//      if (fabs(el->superClusterEta()) < 1.4442 || fabs(el->superClusterEta()) > 1.5660)
        if (el->mvaTrigId()>ElectronLooseMVAId_)
          if ( RelIso < ElectronRelIso )
            selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> Selection::GetSelectedLooseDiElectrons(float PtThr, float EtaThr, float ElectronRelIso) const {
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0;i<electrons.size();i++){
    TRootElectron* el = (TRootElectron*) electrons[i];

    // Detector-based relative isolation
    //  float RelIso = (el->caloIso(3)+el->trackerIso(3)) / el->Et();
    // PF relative isolation
    //  float RelIso = (el->chargedHadronIso()+el->neutralHadronIso()+el->photonIso())/el->Pt();
    // PF relative isolation with dBeta correction:
    //  float RelIso = (el->chargedHadronIso()+max(0.0,el->neutralHadronIso()+el->photonIso()-0.5*el->puChargedHadronIso()))/el->Pt();
    
    double EffectiveArea = 0.;
    
    // HCP 2012 updated for electron conesize = 0.3, taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.4&view=markup
    if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 1.0   ) EffectiveArea = 0.130;
    if (fabs(el->superClusterEta()) >= 1.0   && fabs(el->superClusterEta()) < 1.479 ) EffectiveArea = 0.137;
    if (fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.067;
    if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.089;
    if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.3   ) EffectiveArea = 0.107;
    if (fabs(el->superClusterEta()) >= 2.3   && fabs(el->superClusterEta()) < 2.4   ) EffectiveArea = 0.110;
    if (fabs(el->superClusterEta()) >= 2.4) EffectiveArea = 0.138;
    
    double isocorr = 0;
    if(elecIsoCorrType_ == 1) // rho correction (default corr)
      isocorr = rho_*EffectiveArea;
    else if(elecIsoCorrType_ == 2) // dB correction
      isocorr = 0.5*el->puChargedHadronIso();
    else if (elecIsoCorrType_ == 0) // no correction
      isocorr = 0.;
    else{
      cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
      cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
      cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
      exit(1);
    }
    // Compute the relative isolation
    float RelIso = (el->chargedHadronIso() + max( el->neutralHadronIso() + el->photonIso()  - isocorr, 0.) )/ el->Pt();

    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
    	if ( RelIso < ElectronRelIso )
        if (el->mvaTrigId() > ElectronLooseMVAId_)
          selectedElectrons.push_back(electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> Selection::GetSelectedLooseDiElectrons() const{
  return GetSelectedLooseDiElectrons(ElectronLooseEtThreshold_, ElectronLooseEtaThreshold_, ElectronLooseRelIso_);
}


std::vector<TRootElectron*> Selection::GetSelectedElectronsInvIso(float ElectronRelIso) const
{
  std::vector<TRootJet*> empty;
  return GetSelectedElectronsInvIso(ElectronRelIso, empty);
}

std::vector<TRootElectron*> Selection::GetSelectedElectronsInvIso(float ElectronRelIso, vector<TRootJet*>& selJets) const
{
  std::vector<TRootElectron*> init_electrons = GetSelectedElectrons(ElectronEtThreshold_,ElectronEtaThreshold_,99999.,true);
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0; i<init_electrons.size(); i++)
  {
    if(init_electrons[i]->mvaTrigId() > 0.5) continue;
    
    float mindRElJet = 999.;
    for(unsigned int j=0; j<selJets.size(); j++)
    {
      float dRElJet = init_electrons[i]->DeltaR(*selJets.at(j));
      if(dRElJet < mindRElJet) mindRElJet = dRElJet;
    }
    // Detector-based relative isolation
    //  float RelIso = (el->caloIso(3)+el->trackerIso(3)) / el->Et();
    // PF relative isolation
    //  float RelIso = (el->chargedHadronIso()+el->neutralHadronIso()+el->photonIso())/el->Pt();
    // PF relative isolation with dBeta correction:
    //  float RelIso = (el->chargedHadronIso()+max(0.0,el->neutralHadronIso()+el->photonIso()-0.5*el->puChargedHadronIso()))/el->Pt();
    
    double EffectiveArea = 0.;
    
    // HCP 2012 updated for electron conesize = 0.3, taken from http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/interface/ElectronEffectiveArea.h?revision=1.4&view=markup
    if (fabs(init_electrons[i]->superClusterEta()) >= 0.0   && fabs(init_electrons[i]->superClusterEta()) < 1.0   ) EffectiveArea = 0.130;
    if (fabs(init_electrons[i]->superClusterEta()) >= 1.0   && fabs(init_electrons[i]->superClusterEta()) < 1.479 ) EffectiveArea = 0.137;
    if (fabs(init_electrons[i]->superClusterEta()) >= 1.479 && fabs(init_electrons[i]->superClusterEta()) < 2.0   ) EffectiveArea = 0.067;
    if (fabs(init_electrons[i]->superClusterEta()) >= 2.0   && fabs(init_electrons[i]->superClusterEta()) < 2.2   ) EffectiveArea = 0.089;
    if (fabs(init_electrons[i]->superClusterEta()) >= 2.2   && fabs(init_electrons[i]->superClusterEta()) < 2.3   ) EffectiveArea = 0.107;
    if (fabs(init_electrons[i]->superClusterEta()) >= 2.3   && fabs(init_electrons[i]->superClusterEta()) < 2.4   ) EffectiveArea = 0.110;
    if (fabs(init_electrons[i]->superClusterEta()) >= 2.4) EffectiveArea = 0.138;
    
    double isocorr = 0;
    if(elecIsoCorrType_ == 1) // rho correction (default corr)
      isocorr = rho_*EffectiveArea;
    else if(elecIsoCorrType_ == 2) // dB correction
      isocorr = 0.5*init_electrons[i]->puChargedHadronIso();
    else if (elecIsoCorrType_ == 0) // no correction
      isocorr = 0.;
    else{
      cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
      cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
      cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
      exit(1);
    }
    // Compute the relative isolation
    float RelIso = (init_electrons[i]->chargedHadronIso() + max( init_electrons[i]->neutralHadronIso() + init_electrons[i]->photonIso()  - isocorr, 0.) )/ init_electrons[i]->Pt();
    
//    cout << "reliso = " << RelIso << "  minDR = " << mindRElJet << endl;
    
    if( mindRElJet > ElectronDRJetsCut_ && RelIso > ElectronRelIso )
      selectedElectrons.push_back(init_electrons[i]);
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

//____________IS SELECTED_______________________________________________//

bool Selection::isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut){
  if(vertex.size()==0) return false;
  float Rho = sqrt(vertex[0]->x()*vertex[0]->x()+vertex[0]->y()*vertex[0]->y());
  if(!vertex[0]->isFake() && vertex[0]->ndof()>NdofCut && abs(vertex[0]->z())<Zcut && Rho<RhoCut) return true;
  return false;
}

bool Selection::isSelected(unsigned int Njets, bool Veto2ndLepton) const{
  if(Veto2ndLepton){
    if(GetSelectedMuons().size()==1 && GetSelectedElectrons().size()==0 && GetSelectedJets().size()>= Njets) return true;
  }
  else{
    if(GetSelectedMuons().size()>0 && GetSelectedJets().size()>= Njets) return true;
  }
  return false;
}

bool Selection::isSelected(float JetPtThr, unsigned int Njets, bool Veto2ndLepton) const{
  if(Veto2ndLepton){
    if(GetSelectedMuons().size()==1 && GetSelectedElectrons().size()==0 && GetSelectedJets(JetPtThr, JetEtaThreshold_, JetEMFThreshold_).size()>= Njets) return true;
  }
  else{
    if(GetSelectedMuons().size()>0 && GetSelectedJets(JetPtThr, JetEtaThreshold_, JetEMFThreshold_).size()>= Njets) return true;
  }
  return false;
}

bool Selection::isSelected(float PtThrJets, float EtaThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso, float PtThrElectrons, float EtaThrElectrons, float ElectronRelIso, unsigned int Njets, bool Veto2ndLepton) const{
  if(Veto2ndLepton){
    if(GetSelectedMuons(PtThrMuons, EtaThrMuons, MuonRelIso, GetSelectedJets(PtThrJets, EtaThrJets, Njets)).size()==1 && GetSelectedElectrons(PtThrElectrons, EtaThrElectrons, ElectronRelIso).size()==0 && GetSelectedJets(PtThrJets, EtaThrJets, Njets).size()>=Njets) return true;
  }
  else{
    if(GetSelectedMuons(PtThrMuons, EtaThrMuons, MuonRelIso, GetSelectedJets(PtThrJets, EtaThrJets, Njets)).size()>0 && GetSelectedJets(PtThrJets, EtaThrJets, Njets).size()>=Njets) return true;
  }
  return false;
}

//______________________________________________________________________//
