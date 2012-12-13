/*
 *  ObservablesMCMatcher.cc
 *  Refactoring
 *
 *  Created by Thierry Caebergs on 18/02/10.
 *  Copyright 2010 Université de Mons. All rights reserved.
 *
 */

#include "../interface/ObservablesMCMatcher.h"

//ClassImp(ObservablesMCMatcher);

#include "TLorentzVector.h"
#include "../interface/JetPartonMatching.h"
#include "../interface/TopMatching.h"


Observables& ObservablesMCMatcher::getMatchedObservables(TRootNPGenEvent& npgenevt, const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets)
{ 
  vector<TRootMCParticle> quarks_unordered=npgenevt.quarks();
  vector<TLorentzVector> quarks_unordered_tlorentz(quarks_unordered.begin(), quarks_unordered.end());
  vector<TRootMCParticle> quarks(quarks_unordered.size());
  vector<TLorentzVector> jets_tlorentz(jets.begin(), jets.end());
  JetPartonMatching jpm (quarks_unordered_tlorentz, jets_tlorentz, (int) JetPartonMatching::totalMinDist, true, true, 0.3);
  for (size_t i=0; i<quarks_unordered.size(); i++) {
    int index = jpm.getMatchForParton(i, 0);
    if (index >= 0)
      quarks[index] = quarks_unordered[i];
  }
    vector<TLorentzVector> internalSet;
    // Only needed if the [jets] are separated from the one given in arguments.
  internalSet.push_back(qFromW1);
  internalSet.push_back(qFromW2);
  internalSet.push_back(bFromHadTop);
  internalSet.push_back(bFromLepTop);
  size_t indexQFromW1, indexQFromW2, indexBFromHadTop, indexBFromLepTop;
  indexQFromW1=indexQFromW2=indexBFromHadTop=indexBFromLepTop= ((size_t) -1); //Set to maximal value
  vector<TLorentzVector> quarks_tlorentz(quarks.begin(), quarks.end());
  JetPartonMatching jpmInternal(quarks_tlorentz, internalSet, (int) JetPartonMatching::totalMinDist, true, true, 0.3);
  for (size_t i=internalSet.size()-1; i>=0; i--) { // This way of proceeding can result in no assignment
    size_t ind = jpmInternal.getMatchForParton(i,0);
    if (ind==internalSet.size()-4)
      indexQFromW1=i;
    if (ind==internalSet.size()-3)
      indexQFromW2=i;
    if (ind==internalSet.size()-2)
      indexBFromHadTop=i;
    if (ind==internalSet.size()-1)
      indexBFromLepTop=i;
  }
  TRootMCParticle qFromW1MC;
  TRootMCParticle qFromW2MC;
  TRootMCParticle bFromHadTopMC;
  TRootMCParticle bFromLepTopMC;
  if (indexQFromW1>=0)
    qFromW1MC = quarks[indexQFromW1];
  if (indexQFromW2>=0)
    qFromW2MC = quarks[indexQFromW2];
  if (indexBFromHadTop>=0)
    bFromHadTopMC = quarks[indexBFromHadTop];
  if (indexBFromLepTop>=0)
    bFromLepTopMC = quarks[indexBFromLepTop];
  
  vector<TRootMCParticle> leptons = npgenevt.leptons();
  TRootMCParticle leptonMC;
  
  vector<TLorentzVector> leptons_tlorentz(leptons.begin(), leptons.end());
  JetPartonMatching leptonsMatching(leptons_tlorentz, vector<TLorentzVector>(1,lepton), (int) JetPartonMatching::totalMinDist, true, true, 0.3);
  for (size_t j=0; j<leptons.size(); j++) {
    if (leptonsMatching.getMatchForParton(j,0)>=0)
    {
      leptonMC = leptons[leptonsMatching.getMatchForParton(j,0)];
      break;
    }
  }
    
  vector<TRootMCParticle> invisiblesEnd;
  for (vector<TRootMCParticle>::iterator iter = npgenevt.neutrinos().begin(); iter != npgenevt.neutrinos().end(); iter++)
    invisiblesEnd.push_back(*iter);
  for (vector<TRootMCParticle>::iterator iter = npgenevt.invisibleParticles().begin(); iter != npgenevt.invisibleParticles().end(); iter++)
    invisiblesEnd.push_back(*iter);
  Double_t px, py, pz, energy, vx, vy, vz;
  px = py = pz = energy = vx = vy = vz = 0.;
  for (vector<TRootMCParticle>::iterator invPart=invisiblesEnd.begin(); invPart!=invisiblesEnd.end(); invPart++) {
    px += invPart->Px();
    py += invPart->Py();
    pz += invPart->Pz();
    energy += invPart->Energy();
    vx += invPart->vx();
    vy += invPart->vy();
    vz += invPart->vz();
  }
  TRootMET METMC(px, py, pz, energy, vx, vy, vz);
  Observables *obs = new Observables(lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, MET, jets);
  obs->SetMCInfo(leptonMC, qFromW1MC, qFromW2MC, bFromHadTopMC, bFromLepTopMC, METMC, quarks);
  return *obs;
};

Observables& ObservablesMCMatcher::getMatchedObservables(TRootGenEvent& genevt, const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets)
{
  TRootMCParticle leptonMC(genevt.lepton());
  TRootMCParticle qFromW1MC(genevt.hadronicDecayQuark());
  TRootMCParticle qFromW2MC(genevt.hadronicDecayQuarkBar());
  TRootMCParticle bFromHadTopMC(genevt.hadronicDecayB());
  TRootMCParticle bFromLepTopMC(genevt.leptonicDecayB());
  TRootMCParticle METMC(genevt.neutrino());
  vector<TRootMCParticle> quarks;
  vector<TLorentzVector> lep = genevt.leptonicDecayTopRadiation();
  for (vector<TLorentzVector>::const_iterator iter = lep.begin(); iter != lep.end(); iter++)
    quarks.push_back(TRootMCParticle(*iter));
  vector<TLorentzVector> had = genevt.hadronicDecayTopRadiation();
  for (vector<TLorentzVector>::const_iterator iter = had.begin(); iter != had.end(); iter++)
    quarks.push_back(TRootMCParticle(*iter));
  vector<TLorentzVector> isr = genevt.ISR();
  for (vector<TLorentzVector>::const_iterator iter = isr.begin(); iter != isr.end(); iter++)
    quarks.push_back(TRootMCParticle(*iter));
  quarks.push_back(qFromW1MC);
  quarks.push_back(qFromW2MC);
  quarks.push_back(bFromHadTopMC);
  quarks.push_back(bFromLepTopMC);
  
    //  Observables *obs = new Observables(lepton, jets[], jets[], jets[], jets[], MET, jets);
    Observables *obs = new Observables(lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, MET, jets);
  obs->SetMCInfo(leptonMC, qFromW1MC, qFromW2MC, bFromHadTopMC, bFromLepTopMC, METMC, quarks);
  return *obs;
};

Observables& ObservablesMCMatcher::getMatchedObservables(vector<TRootMCParticle>& mcParticles, const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets)
{
  vector<TRootMCParticle> quarks_unordered;
  vector<TRootMCParticle> leptons;
  vector<TRootMCParticle> invisiblesEnd; //vector of end of daughterisation neutral particles. (to compute MET)
                                         // Sorting particles in several categories.
  for (vector<TRootMCParticle>::iterator iter=mcParticles.begin(); iter != mcParticles.end(); iter++)
  {
    if (abs(iter->type()) < 6 || abs(iter->type()) == 21)
    { //complying with TopTreeProducer : excluding top quark
      quarks_unordered.push_back(*iter);
      continue;
    }
    else if (abs(iter->type())==11 || abs(iter->type())==12 || abs(iter->type())==13 ) // Generalizing lepton(TRootMuon) to electron or muon or tau (maybe taus should be excluded, but inserted for consistency with NPGenEvent)
    {
      leptons.push_back(*iter);
      continue;
    }
    else if (iter->nDau()==0 && iter->charge()==0.)
    {
      invisiblesEnd.push_back(*iter);
      continue;
    }
  }
  
    // Filtering
  vector<TRootMCParticle> quarks(quarks_unordered.size());
  vector<TLorentzVector> quarks_unordered_tlorentz(quarks_unordered.begin(), quarks_unordered.end());
  vector<TLorentzVector> jets_tlorentz(jets.begin(), jets.end());
  JetPartonMatching jpm (quarks_unordered_tlorentz, jets_tlorentz, (int) JetPartonMatching::totalMinDist, true, true, 0.3);
  for (size_t i=0; i<quarks_unordered.size(); i++) {
    int index = jpm.getMatchForParton(i, 0);
    if (index >= 0)
      quarks[index] = quarks_unordered[i];
  }
  vector<TLorentzVector> internalSet(jets.begin(), jets.end());
    // Only needed if the [jets] are separated from the one given in arguments.
  internalSet.push_back(qFromW1);
  internalSet.push_back(qFromW2);
  internalSet.push_back(bFromHadTop);
  internalSet.push_back(bFromLepTop);
  size_t indexQFromW1, indexQFromW2, indexBFromHadTop, indexBFromLepTop;
  indexQFromW1=indexQFromW2=indexBFromHadTop=indexBFromLepTop= ((size_t) -1); //Set to maximal value
  vector<TLorentzVector> quarks_tlorentz(quarks.begin(), quarks.end());
  JetPartonMatching jpmInternal(quarks_tlorentz, internalSet, (int) JetPartonMatching::totalMinDist, true, true, 0.3);
  for (size_t i=internalSet.size()-1; i>=0; i--) {
    size_t ind = jpmInternal.getMatchForParton(i,0);
    if (ind==internalSet.size()-4)
      indexQFromW1=i;
    if (ind==internalSet.size()-3)
      indexQFromW2=i;
    if (ind==internalSet.size()-2)
      indexBFromHadTop=i;
    if (ind==internalSet.size()-1)
      indexBFromLepTop=i;
  }
  TRootMCParticle qFromW1MC;
  TRootMCParticle qFromW2MC;
  TRootMCParticle bFromHadTopMC;
  TRootMCParticle bFromLepTopMC;
  if (indexQFromW1>=0)
    qFromW1MC = quarks[indexQFromW1];
  if (indexQFromW2>=0)
    qFromW2MC = quarks[indexQFromW2];
  if (indexBFromHadTop>=0)
    bFromHadTopMC = quarks[indexBFromHadTop];
  if (indexBFromLepTop>=0)
    bFromLepTopMC = quarks[indexBFromLepTop];
  
  TRootMCParticle leptonMC;
  vector<TLorentzVector> leptons_tlorentz(leptons.begin(), leptons.end());
  JetPartonMatching leptonsMatching(leptons_tlorentz, vector<TLorentzVector>(1,lepton), (int) JetPartonMatching::totalMinDist, true, true, 0.3);
  for (size_t j=0; j<leptons.size(); j++) {
    if (leptonsMatching.getMatchForParton(j,0)>=0)
    {
      leptonMC = leptons[leptonsMatching.getMatchForParton(j,0)];
      break;
    }
  }
  
  Double_t px, py, pz, energy, vx, vy, vz;
  px = py = pz = energy = vx = vy = vz = 0.;
  for (vector<TRootMCParticle>::iterator invPart=invisiblesEnd.begin(); invPart!=invisiblesEnd.end(); invPart++) {
    px += invPart->Px();
    py += invPart->Py();
    pz += invPart->Pz();
    energy += invPart->Energy();
    vx += invPart->vx();
    vy += invPart->vy();
    vz += invPart->vz();
  }
  TRootMET METMC(px, py, pz, energy, vx, vy, vz);
  
    // Create the Observables instance to manage all the stuff
  Observables *obs = new Observables(lepton, qFromW1, qFromW2, bFromHadTop, bFromLepTop, MET, jets);
  obs->SetMCInfo(leptonMC, qFromW1MC, qFromW2MC, bFromHadTopMC, bFromLepTopMC, METMC, quarks);
  return *obs;
};

/** Generic analysis */

Observables& ObservablesMCMatcher::getMatchedObservables(TRootNPGenEvent& npgenevt, const TRootMuon & lepton, const vector < TRootJet > &jets, const TRootMET & MET, string & dsname)
{
    // I am almost not sure about that stuff.
  TRootMCParticle qFromW1MC;
  TRootMCParticle qFromW2MC;
  TRootMCParticle bFromHadTopMC;
  TRootMCParticle bFromLepTopMC;
  TRootMCParticle METMC;
  TRootMCParticle leptonMC;
  if (npgenevt.tops().size()>1) {
    if (npgenevt.tops()[0].isHadronic() && !npgenevt.tops()[1].isLeptonic())
    {
      qFromW1MC = npgenevt.tops()[0].quark();
      qFromW2MC = npgenevt.tops()[0].quarkBar();
      bFromHadTopMC = npgenevt.tops()[0].bquark();
      bFromLepTopMC = npgenevt.tops()[1].bquark();
      leptonMC = npgenevt.tops()[1].lepton();
    }
      else if (npgenevt.tops()[1].isHadronic() && !npgenevt.tops()[0].isLeptonic())
      {
        qFromW1MC = npgenevt.tops()[1].quark();
        qFromW2MC = npgenevt.tops()[1].quarkBar();
        bFromHadTopMC = npgenevt.tops()[1].bquark();
        bFromLepTopMC = npgenevt.tops()[0].bquark();
        leptonMC = npgenevt.tops()[0].lepton();
      }
    else {
      ;
    }

  }
  for (vector<TRootGenTop>::iterator iter=npgenevt.tops().begin(); iter!=npgenevt.tops().end(); iter++) {
    if(iter->isLeptonic())
      if (METMC.type()==0)
        METMC = iter->neutrino();
      else
        METMC += iter->neutrino();
  }
  for (vector<TRootMCParticle>::iterator iter= npgenevt.invisibleParticles().begin(); iter!= npgenevt.invisibleParticles().end(); iter++)
    METMC += *iter;
  for (vector<TRootMCParticle>::iterator iter= npgenevt.neutrinos().begin(); iter!= npgenevt.neutrinos().end(); iter++)
    METMC += *iter;

  vector<TRootMCParticle> quarks;
  quarks.push_back(qFromW1MC);
  quarks.push_back(qFromW2MC);
  quarks.push_back(bFromHadTopMC);
  quarks.push_back(bFromLepTopMC);
  
  Observables *obs = new Observables(lepton, jets, MET, dsname);
  if (jets.size()>3)
   obs->SetMCInfo(leptonMC, qFromW1MC, qFromW2MC, bFromHadTopMC, bFromLepTopMC, METMC, quarks);
  return *obs;
  
}

