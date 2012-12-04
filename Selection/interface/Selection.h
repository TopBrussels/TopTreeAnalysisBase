/*
 
  An example of how to use this class for e+jets and mu+jets selection (SelV4)

  http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/NewPhysicsBrussels/TopTreeAnalysis/macros/SyncRefSel.cc?view=log&pathrev=CMSSW_38X#rev1.1.2.1

*/

#ifndef Selection_h
#define Selection_h

#include "TopTreeProducer/interface/TRootVertex.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootCaloJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootJPTJet.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"
#include "TopTreeProducer/interface/TRootEvent.h"

#include "TopTreeAnalysis/Reconstruction/interface/Combination.h"
#include "TopTreeAnalysis/Content/interface/AnalysisEnvironment.h"

// system include files
#include <memory>
#include <vector>

using namespace std;
using namespace TopTree;


struct HighestPt{
  bool operator()( TRootJet* j1, TRootJet* j2 ) const{
    return j1->Pt() > j2->Pt() ;
  }
  bool operator()( TRootMuon* j1, TRootMuon* j2 ) const{
    return j1->Pt() > j2->Pt() ;
  }
  bool operator()( TRootElectron* j1, TRootElectron* j2 ) const{
    return j1->Pt() > j2->Pt() ;
  }
  bool operator()( TRootMCParticle* j1, TRootMCParticle* j2 ) const{
    return j1->Pt() > j2->Pt() ;
  }
  bool operator()( TLorentzVector* j1, TLorentzVector* j2 ) const{
    return j1->Pt() > j2->Pt() ;
  }
};				

class Selection{

 public:
  Selection();
  Selection(const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootMET*>&);
  Selection(const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&);
  Selection(const Selection &);
  ~Selection();
     
  void SetConfiguration(float PtThrJets, float EtaThrJets, float EMFThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso, float MuonVetoEM, float MuonVetoHad);
      
  bool isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut);

  //bool passVBTFID(TRootElectron* electron, std::map<std::string,float> cuts);
  bool passConversionRejection(TRootElectron* electron);
  bool foundZCandidate(TRootElectron* electron, std::vector<TRootElectron*>& vetoElectrons, float windowsize = 15 );
  bool foundZCandidate(std::vector<TRootElectron*>& electrons1, std::vector<TRootElectron*>& electrons2, float windowsize = 15 );
  bool foundZCandidate(TRootMuon* muon, std::vector<TRootMuon*>& vetoMuons, float windowsize = 15 );
  bool foundZCandidate(std::vector<TRootMuon*>& muons1, std::vector<TRootMuon*>& muons2, float windowsize = 15 );

// JET SELECTION CUTS ========================================

  void setJetCuts();
  void setJetCuts(float Pt, float Eta, float EMF, float n90Hits, float fHPD, float dRJetElectron, float dRJetMuon);  
  
// ELECTRON SELECTION CUTS ========================================
// -- single-lepton channel
  void setElectronCuts();
  void setElectronCuts(float Pt, float Eta, float RelIso, float d0, float MVAId, float DistVzPVz, float DRJets, int MaxMissingHits);
  void setLooseElectronCuts();
  void setLooseElectronCuts(float Pt, float Eta, float RelIso, float MVAId);
// -- double-lepton channel
  void setDiElectronCuts();
  void setDiElectronCuts(float Et, float Eta, float RelIso, float d0, float MVAId, float DistVzPVz, float DRJets, int MaxMissingHits);
  void setLooseDiElectronCuts();
  void setLooseDiElectronCuts(float Pt, float Eta, float RelIso);

// MUON SELECTION CUTS ========================================
// -- single-lepton channel
  void setMuonCuts();
  void setMuonCuts(float Pt, float Eta, float RelIso, int NValidHits, float d0, float DRJets, int NMatchedStations, float DistVzPVz, int NTrackerLayersWithMeas, int NValidPixelHits);
  void setLooseMuonCuts();
  void setLooseMuonCuts(float Pt, float Eta, float RelIso);
// -- double-lepton channel
  void setDiMuonCuts();
  void setDiMuonCuts(float Pt, float Eta, float RelIso, float d0);

// JET GETTERS ====================================================
  std::vector<TRootJet*> GetSelectedJets(bool applyJetID = false) const;
  std::vector<TRootJet*> GetSelectedJets(vector<TRootMuon*>& muons, bool applyJetID = false) const;
  std::vector<TRootJet*> GetSelectedJets(vector<TRootElectron*>& electrons, bool applyJetID = false) const;
  std::vector<TRootJet*> GetSelectedJets(float PtThr, float EtaThr, bool applyJetID = false) const;
  std::vector<TRootJet*> GetSelectedJets(float PtThr, float EtaThr, vector<TLorentzVector*>& lepton, float dRLeptonJet, bool applyJetID = false) const;
  std::vector<TRootJet*> GetSelectedJets(float PtThr, float EtaThr, vector<TRootMuon*>& muons, float dRLeptonJet, bool applyJetID = false) const;
  std::vector<TRootJet*> GetSelectedJets(float PtThr, float EtaThr, vector<TRootElectron*>& electrons, float dRLeptonJet, bool applyJetID = false) const;
  std::vector<TRootJet*> GetSelectedBJets(const std::vector<TRootJet*>& seljets, Int_t& btagAlgo, Float_t& btagCut) const;

// ELECTRON GETTERS ===============================================
  std::vector<TRootElectron*> GetSelectedElectrons() const;
  std::vector<TRootElectron*> GetSelectedElectrons(TRootVertex* vertex) const;
  std::vector<TRootElectron*> GetSelectedElectrons(TRootVertex* vertex, vector<TRootJet*>& selJets) const;
  std::vector<TRootElectron*> GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso) const;
  std::vector<TRootElectron*> GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso, TRootVertex* vertex) const;
  std::vector<TRootElectron*> GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso, TRootVertex* vertex, vector<TRootJet*>& selJets) const;
  std::vector<TRootElectron*> GetSelectedDiElectrons() const;
  std::vector<TRootElectron*> GetSelectedDiElectrons(float EtThr, float EtaThr, float ElectronRelIso) const;
  std::vector<TRootElectron*> GetSelectedLooseElectrons() const;
  std::vector<TRootElectron*> GetSelectedLooseElectrons(float EtThr, float EtaThr, float ElectronRelIso) const;
  std::vector<TRootElectron*> GetSelectedLooseDiElectrons() const;
  std::vector<TRootElectron*> GetSelectedLooseDiElectrons(float EtThr, float EtaThr, float ElectronRelIso) const;
  std::vector<TRootElectron*> GetSelectedElectronsInvIso(float ElectronRelIso, TRootVertex* vertex) const;
  std::vector<TRootElectron*> GetSelectedElectronsInvIso(float ElectronRelIso, TRootVertex* vertex, vector<TRootJet*>& selJets) const;

// MUON GETTERS ==================================================
  std::vector<TRootMuon*> GetSelectedMuons() const;
  std::vector<TRootMuon*> GetSelectedMuons(TRootVertex* vertex) const;
  std::vector<TRootMuon*> GetSelectedMuons(TRootVertex* vertex, vector<TRootJet*>& selJets) const;
  std::vector<TRootMuon*> GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso) const;
  std::vector<TRootMuon*> GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso, vector<TRootJet*> selJets) const;
  std::vector<TRootMuon*> GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso, TRootVertex* vertex, vector<TRootJet*>& selJets) const;
  std::vector<TRootMuon*> GetSelectedDiMuons() const;
  std::vector<TRootMuon*> GetSelectedDiMuons(float PtThr, float EtaThr, float MuonRelIso) const;
  std::vector<TRootMuon*> GetSelectedLooseMuons() const;
  std::vector<TRootMuon*> GetSelectedLooseMuons(float PtThr, float EtaThr, float MuonRelIso) const;

  std::vector<TRootMuon*> GetSelectedMuonsNonIso() const;
  std::vector<TRootMuon*> GetSelectedMuonsNonIso(float PtThr, float EtaThr, vector<TRootJet*>& selJets) const;
  std::vector<TRootMuon*> GetSelectedMuonsInvIso(float MuonRelIso, TRootVertex* vertex) const;
  std::vector<TRootMuon*> GetSelectedMuonsInvIso(float MuonRelIso, TRootVertex* vertex, vector<TRootJet*>& selJets) const;
  std::vector<TRootMuon*> GetSelectedMuonsIsoRange(float PtThr, float EtaThr, float MuonRelIsoMin, float MuonRelIsoMax, vector<TRootJet*>& selJets) const;


// SELECTION BOOLEANS =============================================
  bool isThere1Muon() const { if( GetSelectedMuons().size()>0) return true; return false; };
  bool isThere1MuonIso() const { if( GetSelectedMuonsNonIso().size()>0) return true; return false; };
  bool isThere4Jets() const { if( GetSelectedJets().size()>3) return true; return false; };
  bool isSelected(unsigned int Njets = 4, bool Veto2ndLepton = false) const;
  bool isSelected(float JetPtThr, unsigned int Njets = 4, bool Veto2ndLepton = false) const;
  //bool isSelected (float PtThrJets, float EtaThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso, float MuonVetoEM, float MuonVetoHad) const;
  bool isSelected (float PtThrJets, float EtaThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso,float PtThrElectrons, float EtaThrElectrons, float ElectronRelIso, unsigned int Njets = 4, bool Veto2ndLepton = false) const;

 private:
  //int JetType;
  std::vector<TRootJet*> jets;
  std::vector<TRootElectron*> electrons;
  std::vector<TRootMuon*> muons;
  std::vector<TRootMET*> mets;
	
  //jets
  int Njets_;
  float JetPtThreshold_;
  float JetEtaThreshold_;
  float JetEMFThreshold_;
  float n90HitsThreshold_;
  float fHPDThreshold_;
  float DRJetElectron_;

  //muon
  float MuonPtThreshold_;
  float MuonEtaThreshold_;
  float MuonRelIso_;
  float MuonLoosePtThreshold_;
  float MuonLooseEtaThreshold_;
  float MuonLooseRelIso_;
  int   MuonNofValidHits_;
  float Muond0Cut_;
  float MuonDRJetsCut_;
  float MuonNormChi2_;
  int   MuonNMatchedStations_;
  float MuonDistVzPVz_;
  int   MuonNTrackerLayersWithMeasurement_;
  int   MuonNValidPixelHits_;

  float DRJetMuon_; // for pflow mu-jet cleaning

  //electron
  float ElectronEtThreshold_;
  float ElectronEtaThreshold_;
  float ElectronRelIso_;
  float ElectronLooseEtThreshold_;
  float ElectronLooseEtaThreshold_;
  float ElectronLooseRelIso_;
  float Electrond0Cut_;
  float ElectronDistVzPVz_;
  float ElectronDRJetsCut_;
  float ElectronMVAId_;
  float ElectronLooseMVAId_;
  int   ElectronMaxMissingHitsCut_;

  std::map<std::string,float> cutsVBTFWP70;
  std::map<std::string,float> cutsVBTFWP80;
  std::map<std::string,float> cutsVBTFWP95;

  //met
  float METThreshold_;
};

#endif
