/*

  An example of how to use this class for e+jets and mu+jets selection (SelV4)

  http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/NewPhysicsBrussels/TopTreeAnalysis/macros/SyncRefSel.cc?view=log&pathrev=CMSSW_38X#rev1.1.2.1

*/

#ifndef JetSelection_h
#define JetSelection_h

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


struct HighestJetPt
{
    bool operator()( TRootJet* j1, TRootJet* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
};

class JetSelection
{

public:
    JetSelection();
    JetSelection(const std::vector<TRootJet*>&);

    JetSelection(const JetSelection &);
    ~JetSelection();

// JET GETTERS ====================================================
    std::vector<TRootPFJet*> GetSelectedBJets(const std::vector<TRootPFJet*>& seljets, Int_t& btagAlgo, Float_t& btagCut) const;
    std::vector<TRootPFJet*> GetSelectedJets() const;
    std::vector<TRootPFJet*> GetSelectedJets(float PtThr, float EtaThr, bool applyJetID = true, std::string TightLoose = "Loose") const;

// SELECTION BOOLEANS =============================================
    bool isThere4Jets() const
    {
        if( GetSelectedJets().size()>3) return true;
        return false;
    };

private:

    std::vector<TRootJet*> jets;

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

};

#endif

