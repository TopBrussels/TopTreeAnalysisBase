/*

  An example of how to use this class for e+jets and mu+jets selection (SelV4)

  http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/NewPhysicsBrussels/TopTreeAnalysis/macros/SyncRefSel.cc?view=log&pathrev=CMSSW_38X#rev1.1.2.1

*/

#ifndef Run2Selection_h
#define Run2Selection_h

#include "TopTreeProducer/interface/TRootVertex.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootCaloJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootSubstructureJet.h"
#include "TopTreeProducer/interface/TRootJPTJet.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"
#include "TopTreeProducer/interface/TRootEvent.h"

#include "TopTreeAnalysisBase/Reconstruction/interface/Combination.h"
#include "TopTreeAnalysisBase/Content/interface/AnalysisEnvironment.h"

// system include files
#include <memory>
#include <vector>

using namespace std;
using namespace TopTree;


struct HighestPt
{
    bool operator()( TRootJet* j1, TRootJet* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
    bool operator()( TRootMuon* j1, TRootMuon* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
    bool operator()( TRootElectron* j1, TRootElectron* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
    bool operator()( TRootMCParticle* j1, TRootMCParticle* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
    bool operator()( TLorentzVector* j1, TLorentzVector* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
};

class Run2Selection
{

public:
    Run2Selection();
    //Selection(const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootMET*>&);
    //Selection(const std::vector<TRootJet*>&, const std::vector<TRootElectron*>&, const std::vector<TRootMET*>&);
    Run2Selection(const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&);
    Run2Selection(const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&, const float rho);

    Run2Selection(const std::vector<TRootJet*>&,const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&);
    Run2Selection(const std::vector<TRootJet*>&,const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&, const float rho);


    Run2Selection(const Run2Selection &);
    ~Run2Selection();

//  void SetConfiguration(float PtThrJets, float EtaThrJets, float EMFThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso, float MuonVetoEM, float MuonVetoHad);

    bool isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut);

    //bool passVBTFID(TRootElectron* electron, std::map<std::string,float> cuts);
//  bool passConversionRejection(TRootElectron* electron);
    bool foundZCandidate(TRootElectron* electron, std::vector<TRootElectron*>& vetoElectrons, float windowsize = 15 );
    bool foundZCandidate(std::vector<TRootElectron*>& electrons1, std::vector<TRootElectron*>& electrons2, float windowsize = 15 );
    bool foundZCandidate(TRootMuon* muon, std::vector<TRootMuon*>& vetoMuons, float windowsize = 15 );
    bool foundZCandidate(std::vector<TRootMuon*>& muons1, std::vector<TRootMuon*>& muons2, float windowsize = 15 );

// JET GETTERS ====================================================
    std::vector<TRootPFJet*> GetSelectedBJets(const std::vector<TRootPFJet*>& seljets, Int_t& btagAlgo, Float_t& btagCut) const;
    std::vector<TRootPFJet*> GetSelectedJets() const;
    std::vector<TRootPFJet*> GetSelectedJets(float PtThr, float EtaThr, bool applyJetID = true, std::string TightLoose = "Loose") const;
    std::vector<TRootSubstructureJet*> GetSelectedFatJets() const;
    std::vector<TRootSubstructureJet*> GetSelectedFatJets(float PtThr, float EtaThr, bool applyJetID = true) const;



    void setElectronIsoCorrType(int corrType = 1);
    float GetElectronIsoCorrType(const TRootElectron* el, bool bx25) const;
    float pfElectronIso(const TRootElectron *el, bool bx25 = true) const;
// ELECTRON GETTERS ===============================================
    std::vector<TRootElectron*> GetSelectedElectrons() const;
	std::vector<TRootElectron*> GetSelectedElectrons(string WorkingPoint, string ProductionCampaign, bool CutsBased) const;
	std::vector<TRootElectron*> GetSelectedElectrons(float PTthr, float EtaThr, string WorkingPoint, string ProductionCampaign, bool CutsBased) const;

    std::vector<TRootElectron*> GetSelectedDisplacedElectrons (float PtThr, float EtaThr, bool noIso=false, bool noId=false) const;
    std::vector<TRootElectron*> GetSelectedDisplacedElectrons () const;
// MUON GETTERS ==================================================
    std::vector<TRootMuon*> GetSelectedMuons() const;
    std::vector<TRootMuon*> GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso, string WorkingPoint, string ProductionCampaign) const;

    // displaced muons
    std::vector<TRootMuon*> GetSelectedDisplacedMuons(float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations, float RelIso, bool noIso=false, bool noId=false) const;
    std::vector<TRootMuon*> GetSelectedDisplacedMuons(float PtThr, float EtaThr, float RelIs, bool noIso=false, bool noId=false) const;
    std::vector<TRootMuon*> GetSelectedDisplacedMuons() const;



// SELECTION BOOLEANS =============================================
    bool isThere1Muon() const
    {
        if( GetSelectedMuons().size()>0) return true;
        return false;
    };
    bool isThere4Jets() const
    {
        if( GetSelectedJets().size()>3) return true;
        return false;
    };

private:

	std::vector<TRootElectron*> GetSelectedTightElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedMediumElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedLooseElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedTightElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedMediumElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedLooseElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedTightElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedMediumElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedLooseElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedVetoElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const;

	//July 2015 Muon Getters
    std::vector<TRootMuon*> GetSelectedLooseMuonsJuly2015(float PtThr, float EtaThr,float MuonRelIso) const;
    std::vector<TRootMuon*> GetSelectedMediumMuonsJuly2015(float PtThr, float EtaThr,float MuonRelIso) const;
    std::vector<TRootMuon*> GetSelectedTightMuonsJuly2015(float PtThr, float EtaThr,float MuonRelIso) const;




    float rho_;
    int elecIsoCorrType_;

    //int JetType;
    std::vector<TRootJet*> jets;
    std::vector<TRootJet*> fatjets;
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
    bool cutHFHadronEnergyFraction_;
    bool applyJetID_;

    //=======================
    //Jet Helper Functions===
    //=======================

    bool passPFJetID8TEV(const TRootPFJet* PFJet) const;
    bool passLoosePFJetID13TeV(const TRootPFJet* PFJet) const;
    bool passTightPFJetID13TeV(const TRootPFJet* PFJet) const;

    //muon
    float MuonPtThreshold_;
    float MuonEtaThreshold_;
    float MuonRelIsoCut_;
    float MuonLoosePtThreshold_;
    float MuonLooseEtaThreshold_;
    float MuonLooseRelIsoCut_;
    float Muond0Cut_;
    float MuondZCut_;
    float MuonNormChi2Cut_;
    int   MuonNMatchedStationsCut_;
    int   MuonNTrackerLayersWithMeasurementCut_;
    int   MuonNValidPixelHitsCut_;
    int   MuonNValidMuonHitsCut_;

    float DRJetMuon_; // for pflow mu-jet cleaning

    //electron
    float ElectronEtThreshold_;
    float ElectronEtaThreshold_;
    float ElectronRelIso_;
    float ElectronLooseEtThreshold_;
    float ElectronLooseEtaThreshold_;
    float ElectronLooseRelIso_;
    float Electrond0Cut_;
    float ElectronDRJetsCut_;
    float ElectronMVAId_;
    float ElectronLooseMVAId_;
    int   ElectronMaxMissingHitsCut_;

    std::map<std::string,float> cutsVBTFWP70;
    std::map<std::string,float> cutsVBTFWP80;
    std::map<std::string,float> cutsVBTFWP95;
    
    // displaced lepton functions
    bool identificationDisplacedElectron(const TRootElectron*) const;
    bool identificationDisplacedMuon(const TRootMuon*, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations) const;
    bool isolationDisplacedElectron(const TRootElectron*) const;
    bool isolationDisplacedMuon(const TRootMuon*, float RelIso) const;
    //met
    float METThreshold_;
};

#endif

