#ifndef _ObservablesMCMatcher_h_
#define _ObservablesMCMatcher_h_

#include<vector>
#include "TopTreeAnalysisBase/Reconstruction/interface/Observables.h"
#include "TopTreeProducer/interface/TRootGenEvent.h"
#include "TopTreeProducer/interface/TRootNPGenEvent.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootMuon.h"

/**
 *  Refactoring ObservablesMCMatcher.h
 *
 *  Created by Thierry Caebergs () on 18/02/10.
 * 
 */
using namespace TopTree;

class ObservablesMCMatcher
{
public:

  ObservablesMCMatcher(){;};

  ObservablesMCMatcher(const ObservablesMCMatcher& rhs){;};

  ~ObservablesMCMatcher(){;};

  const ObservablesMCMatcher& operator=(const ObservablesMCMatcher& rhs){ return rhs;};
/** Semi-leptonic analysis */
  /**
   Load MC from the MCParticles branch, the TRootMCParticles being stored in a vector.
   */
  Observables& getMatchedObservables(vector<TRootMCParticle>& mcParticles, const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets);

  /**
   Load MC info from NPGenEvent.
   Same association of particles as for MCParticles, using the pre-putting in sets by NPGenEvent
   */
  Observables& getMatchedObservables(TRootNPGenEvent& npgenevt, const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets);

  /**
   Load MC info from GenEvent.
   Be aware that TRootGenEvent only provides TLorentzVector.
   TRootMCParticles then only contains the momentum TLorentzVector.
   Moreover, take only into account semi-leptonic events !!!!
   */
  Observables& getMatchedObservables(TRootGenEvent& genevt, const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets);

  /** Generic analysis */
  Observables& getMatchedObservables(TRootNPGenEvent& npgenevt, const TRootMuon & lept, const vector < TRootJet > &jets_, const TRootMET & MET_, string & dsname);

//  ClassDef(ObservablesMCMatcher,1);
};

  //Do I need to use TopMatching for matching ????
  //TopMatching tm(); // xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx xxx

#endif
