/*

  An example of how to use this class for e+jets and mu+jets selection (SelV4)

  http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/NewPhysicsBrussels/TopTreeAnalysis/macros/SyncRefSel.cc?view=log&pathrev=CMSSW_38X#rev1.1.2.1

*/

#ifndef ElectronSelection_h
#define ElectronSelection_h

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


struct HighestElectronPt
{
    bool operator()( TRootElectron* j1, TRootElectron* j2 ) const
    {
        return j1->Pt() > j2->Pt() ;
    }
};

class ElectronSelection
{

public:
    ElectronSelection();
    ElectronSelection(const std::vector<TRootElectron*>&);
    ElectronSelection(const std::vector<TRootElectron*>&, const float rho);


    ElectronSelection(const ElectronSelection &);
    ~ElectronSelection();

//  void SetConfiguration(float PtThrJets, float EtaThrJets, float EMFThrJets, float PtThrMuons, float EtaThrMuons, float MuonRelIso, float MuonVetoEM, float MuonVetoHad);

    bool isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut);

    //bool passVBTFID(TRootElectron* electron, std::map<std::string,float> cuts);
//  bool passConversionRejection(TRootElectron* electron);
    bool foundZCandidate(TRootElectron* electron, std::vector<TRootElectron*>& vetoElectrons, float windowsize = 15 );
    bool foundZCandidate(std::vector<TRootElectron*>& electrons1, std::vector<TRootElectron*>& electrons2, float windowsize = 15 );

    void setElectronIsoCorrType(int corrType = 1);
    float GetElectronIsoCorrType(TRootElectron* el) const;
    float pfElectronIso(TRootElectron *el) const;
// ELECTRON GETTERS ===============================================
    std::vector<TRootElectron*> GetSelectedElectrons() const;
    std::vector<TRootElectron*> GetSelectedElectrons(string WorkingPoint, string ProductionCampaign, bool CutsBased) const;
    std::vector<TRootElectron*> GetSelectedElectrons(float PTthr, float EtaThr, string WorkingPoint) const;
    std::vector<TRootElectron*> GetSelectedElectrons(float PTthr, float EtaThr, string WorkingPoint, string ProductionCampaign, bool CutsBased, bool applyVID) const;
    
    //displaced electrons
    std::vector<TRootElectron*> GetSelectedDisplacedElectrons (float PtThr, float EtaThr, float relIsoB, float relIsoEC, bool applyIso=true, bool applyId=true) const;
    std::vector<TRootElectron*> GetSelectedDisplacedElectrons () const;
    
private:

	std::vector<TRootElectron*> GetSelectedTightElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedMediumElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedLooseElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedVetoElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const;
  std::vector<TRootElectron*> GetSelectedFakeLooseElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const;
  std::vector<TRootElectron*> GetSelectedFakeTightElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const;
  
	std::vector<TRootElectron*> GetSelectedHeepElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedTightElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedMediumElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedLooseElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedVetoElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const;

	std::vector<TRootElectron*> GetSelectedTightElectronsMVABasedSpring16_80X_VID(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedMediumElectronsMVABasedSpring16_80X_VID(float PtThr, float EtaThr) const;
	std::vector<TRootElectron*> GetSelectedLooseElectronsMVABasedSpring16_80X_VID(float PtThr, float EtaThr) const;

    float rho_;
    int elecIsoCorrType_;

    //int JetType;
    std::vector<TRootElectron*> electrons;

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

    // displaced electron functions                                  
    bool identificationDisplacedElectron(const TRootElectron*) const;
    //    bool isolationDisplacedElectron(const TRootElectron*) const;
    bool isolationDisplacedElectron(TRootElectron*, float relIsoB, float relIsoEC) const;

    std::map<std::string,float> cutsVBTFWP70;
    std::map<std::string,float> cutsVBTFWP80;
    std::map<std::string,float> cutsVBTFWP95;

};

#endif

