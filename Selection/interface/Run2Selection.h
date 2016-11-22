/*

  An example of how to use this class for e+jets and mu+jets selection (SelV4)

  http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/NewPhysicsBrussels/TopTreeAnalysis/macros/SyncRefSel.cc?view=log&pathrev=CMSSW_38X#rev1.1.2.1

*/

#ifndef Run2Selection_h
#define Run2Selection_h

#include "TopTreeProducer/interface/TRootVertex.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootSubstructureJet.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"
#include "TopTreeProducer/interface/TRootEvent.h"

#include "TopTreeAnalysisBase/Reconstruction/interface/Combination.h"
#include "TopTreeAnalysisBase/Content/interface/AnalysisEnvironment.h"
#include "TopTreeAnalysisBase/Selection/interface/JetSelection.h"
#include "TopTreeAnalysisBase/Selection/interface/FatJetSelection.h"
#include "TopTreeAnalysisBase/Selection/interface/MuonSelection.h"
#include "TopTreeAnalysisBase/Selection/interface/ElectronSelection.h"


// system include files
#include <memory>
#include <vector>

using namespace std;
using namespace TopTree;


struct HighestPt
{
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
    Run2Selection(const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&);
    Run2Selection(const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&, const float rho);

    Run2Selection(const std::vector<TRootJet*>&,const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&);
    Run2Selection(const std::vector<TRootJet*>&,const std::vector<TRootJet*>&, const std::vector<TRootMuon*>&, const std::vector<TRootElectron*>&,const std::vector<TRootMET*>&, const float rho);


    Run2Selection(const Run2Selection &);
    ~Run2Selection();


    bool isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut);

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
    float GetElectronIsoCorrType(TRootElectron* el) const;
    float pfElectronIso(TRootElectron *el) const;
// ELECTRON GETTERS ===============================================
    std::vector<TRootElectron*> GetSelectedElectrons() const;
    std::vector<TRootElectron*> GetSelectedElectrons(string WorkingPoint, string ProductionCampaign, bool CutsBased) const;
    std::vector<TRootElectron*> GetSelectedElectrons(float PTthr, float EtaThr, string WorkingPoint) const;
    std::vector<TRootElectron*> GetSelectedElectrons(float PTthr, float EtaThr, string WorkingPoint, string ProductionCampaign, bool CutsBased, bool applyVID) const;


    // displaced electrons
    std::vector<TRootElectron*> GetSelectedDisplacedElectrons (float PtThr, float EtaThr, float relIsoB, float relIsoEC, bool applyIso=true, bool applyId=true) const;
    std::vector<TRootElectron*> GetSelectedDisplacedElectrons () const;



// MUON GETTERS ==================================================
    std::vector<TRootMuon*> GetSelectedMuons() const;
    std::vector<TRootMuon*> GetSelectedMuons(float PtThr, float EtaThr, float MuonRelIso, string WorkingPoint, string ProductionCampaign) const;


    // displaced muons
    std::vector<TRootMuon*> GetSelectedDisplacedMuons(float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations, float RelIso, bool applyIso=true, bool applyId=true) const;
    std::vector<TRootMuon*> GetSelectedDisplacedMuons(float PtThr, float EtaThr, float RelIs, bool applyIso=true, bool applyId=true) const;
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


    float rho_;
    int elecIsoCorrType_;

    //int JetType;
    ElectronSelection *electronSelector;
    MuonSelection *muonSelector;
    JetSelection *jetSelector;
    FatJetSelection *fatJetSelector;

    //met
    float METThreshold_;
};

#endif

