/*

  An example of how to use this class for e+jets and mu+jets selection (SelV4)

  http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/NewPhysicsBrussels/TopTreeAnalysis/macros/SyncRefSel.cc?view=log&pathrev=CMSSW_38X#rev1.1.2.1

*/

#ifndef MuonSelection_h
#define MuonSelection_h

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

// system include files
#include <memory>
#include <vector>

using namespace std;
using namespace TopTree;


struct HighestMuonPt
{
    bool operator()( TRootMuon* j1, TRootMuon* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
};

class MuonSelection
{

public:
    MuonSelection();
    MuonSelection(const std::vector<TRootMuon*>&);

    MuonSelection(const MuonSelection &);
    ~MuonSelection();

//  void SetConfiguration(float PtThrJets, float EtaThrJets, float EMFThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso, float MuonVetoEM, float MuonVetoHad);

    bool foundZCandidate(TRootMuon* muon, std::vector<TRootMuon*>& vetoMuons, float windowsize = 15 );
    bool foundZCandidate(std::vector<TRootMuon*>& muons1, std::vector<TRootMuon*>& muons2, float windowsize = 15 );

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

private:

	//July 2015 Muon Getters
    std::vector<TRootMuon*> GetSelectedLooseMuons2016(float PtThr, float EtaThr,float MuonRelIso) const;
    std::vector<TRootMuon*> GetSelectedMediumMuons2016(float PtThr, float EtaThr,float MuonRelIso, string ProductionCampaign) const;
    std::vector<TRootMuon*> GetSelectedTightMuons2016(float PtThr, float EtaThr,float MuonRelIso) const;
    std::vector<TRootMuon*> GetSelectedFakeMuons2016(float PtThr, float EtaThr,float MuonRelIso) const;




    std::vector<TRootMuon*> muons;

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

    // displaced lepton functions
    bool identificationDisplacedMuon(const TRootMuon*, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations) const;
    bool isolationDisplacedMuon(const TRootMuon*, float RelIso) const;

    float DRJetMuon_; // for pflow mu-jet cleaning
};

#endif

