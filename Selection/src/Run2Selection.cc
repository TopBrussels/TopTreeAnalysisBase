#include "../interface/Run2Selection.h"

//____CONSTRUCTORS______________________________________________________//

Run2Selection::Run2Selection()
{
    rho_ = 0;
    elecIsoCorrType_ = 2; // 0: no corr, 1: rho corr, 2: dB corr.
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

Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_)
{
    rho_ = 0;
    elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
    for(unsigned int i=0; i<jets_.size(); i++) jets.push_back(jets_[i]);
    for(unsigned int i=0; i<muons_.size(); i++) muons.push_back(muons_[i]);
    for(unsigned int i=0; i<electrons_.size(); i++) electrons.push_back(electrons_[i]);
    for(unsigned int i=0; i<mets_.size(); i++) mets.push_back(mets_[i]);
    cutHFHadronEnergyFraction_ = false;
}

Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_, float rho)
{
    rho_ = rho;
    elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
    for(unsigned int i=0; i<jets_.size(); i++) jets.push_back(jets_[i]);
    for(unsigned int i=0; i<muons_.size(); i++) muons.push_back(muons_[i]);
    for(unsigned int i=0; i<electrons_.size(); i++) electrons.push_back(electrons_[i]);
    for(unsigned int i=0; i<mets_.size(); i++) mets.push_back(mets_[i]);
    cutHFHadronEnergyFraction_ = false;
}

Run2Selection::Run2Selection(const Run2Selection& s)
{
    // copy the objects
    rho_ = s.rho_;
    elecIsoCorrType_ = s.elecIsoCorrType_; // 0: no corr, 1: rho corr, 2: dB corr.
    jets = s.jets;
    electrons = s.electrons;
    muons = s.muons;
    mets = s.mets;

    cutHFHadronEnergyFraction_ = false;
}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

Run2Selection::~Run2Selection()
{
    jets.clear();
    electrons.clear();
    muons.clear();
    mets.clear();
}

//______________________________________________________________________//

//______________________________________________________________________//

//__EXTRA METHODS_______________________________________________________//

//______________________________________________________________________//

//____SELECTION GETTERS_________________________________________________//

// ______________JETS_________________________________________________//

// This should be the Standard getter function for Jets in Run-II.  The use of non PF objects should be discouraged.
std::vector<TRootPFJet*> Run2Selection::GetSelectedJets(float PtThr, float EtaThr, bool applyJetID) const
{
    std::vector<TRootPFJet*> selectedJets;
    for(unsigned int i=0; i<jets.size(); i++)
    {
        TRootPFJet* init_jet = (TRootPFJet*) jets[i];
        if (init_jet->jetType() == 2)
        {
            // PFJets
            const TRootPFJet* PFJet = static_cast<const TRootPFJet*>(init_jet);

            if( fabs(PFJet->Eta())<EtaThr && PFJet->Pt()>PtThr )
            {
                if ( applyJetID )
                {
                    if(passPFJetID8TEV(PFJet))//This is the 8TeV Recommended Loose PFJet ID.  This should be updated when 13TeV recommendations become available.
                    {
                    selectedJets.push_back(init_jet);
                    }
                }
                else selectedJets.push_back(init_jet);
            }
        }
    }
    std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
    return selectedJets;
}

std::vector<TRootPFJet*> Run2Selection::GetSelectedJets() const
{
    return GetSelectedJets(30,2.5,true);
}

std::vector<TRootPFJet*> Run2Selection::GetSelectedBJets(const std::vector<TRootPFJet*>& seljets, int& btagAlgo, float& btagCut) const
{
    std::vector<TRootPFJet*> bjets;
    Float_t btagDisc = 0;
    for(unsigned int i = 0; i<seljets.size(); i++)
    {
        if(btagAlgo == 1) btagDisc = seljets[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
        else if(btagAlgo == 2) btagDisc = seljets[i]->btag_simpleSecondaryVertexHighEffBJetTags();
        else if(btagAlgo == 3) btagDisc = seljets[i]->btag_simpleSecondaryVertexHighPurBJetTags();
        else if(btagAlgo == 4) btagDisc = seljets[i]->btag_combinedSecondaryVertexMVABJetTags();
        else                   btagDisc = -999999;
        if(btagDisc>btagCut) bjets.push_back(seljets[i]);
    }
    return bjets;
}

// ______________MUONS________________________________________________//

std::vector<TRootMuon*> Run2Selection::GetSelectedMuons(float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, float d0, float dZ, int NValidPixelHits, int NMatchedStations, float RelIso) const
{
    std::vector<TRootMuon*> selectedMuons;
    for(unsigned int i=0; i<muons.size(); i++)
    {

        //float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
        float reliso = (muons[i]->chargedHadronIso() + max( 0.0, muons[i]->neutralHadronIso() + muons[i]->photonIso() - 0.5*muons[i]->puChargedHadronIso() ) ) / muons[i]->Pt(); // dBeta corrected
        if(     muons[i]->idGlobalMuonPromptTight() //&& muons[i]->isPFMuon()
                && muons[i]->Pt()>PtThr
                && fabs(muons[i]->Eta())<EtaThr
                && muons[i]->chi2() < NormChi2
                && muons[i]->nofTrackerLayersWithMeasurement() > NTrackerLayersWithMeas
                && muons[i]->nofValidMuHits() > NValidMuonHits
                && fabs(muons[i]->d0()) < d0
                && fabs(muons[i]->dz()) < dZ
                && muons[i]->nofValidPixelHits() > NValidPixelHits
                && muons[i]->nofMatchedStations()> NMatchedStations
                && reliso < RelIso)
        {
            selectedMuons.push_back(muons[i]);
        }
    }
    std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
    return selectedMuons;
}

std::vector<TRootMuon*> Run2Selection::GetSelectedMuons() const
{
    return GetSelectedMuons(26,2.1,0.12);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedMuons(float PtThr, float EtaThr,float MuonRelIso) const
{
    return GetSelectedMuons(PtThr,EtaThr,10,5,0,0.2,0.5,0,1,MuonRelIso);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDiMuons(float PtThr, float EtaThr,float MuonRelIso) const
{
    std::vector<TRootMuon*> selectedMuons;
    for(unsigned int i=0; i<muons.size(); i++)
    {
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

std::vector<TRootMuon*> Run2Selection::GetSelectedDiMuons() const
{
    return GetSelectedDiMuons(20,2.4,0.20);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedLooseMuons(float PtThr, float EtaThr,float MuonRelIso) const
{
    std::vector<TRootMuon*> selectedMuons;
    for(unsigned int i=0; i<muons.size(); i++)
    {
        float reliso = (muons[i]->chargedHadronIso() + max( 0.0, muons[i]->neutralHadronIso() + muons[i]->photonIso() - 0.5*muons[i]->puChargedHadronIso() ) ) / muons[i]->Pt(); // dBeta corrected

        if((muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && fabs(muons[i]->Eta())<EtaThr && muons[i]->Pt()>PtThr && reliso < MuonRelIso )
        {
            selectedMuons.push_back(muons[i]);
        }
    }
    std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
    return selectedMuons;
}

std::vector<TRootMuon*> Run2Selection::GetSelectedLooseMuons() const
{
    return GetSelectedLooseMuons(10, 2.5, 0.20);
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
bool Run2Selection::foundZCandidate(TRootElectron* electron, std::vector<TRootElectron*>& vetoElectrons, float windowsize)
{
    bool foundZ = false;
    for(unsigned int i=0; i<vetoElectrons.size(); i++)
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

bool Run2Selection::foundZCandidate(TRootMuon* muon, std::vector<TRootMuon*>& vetoMuons, float windowsize)
{
    bool foundZ = false;
    for(unsigned int i=0; i<vetoMuons.size(); i++)
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

bool Run2Selection::foundZCandidate(std::vector<TRootElectron*>& electrons1, std::vector<TRootElectron*>& electrons2, float windowsize)
{
    bool foundZ = false;
    for(unsigned int j=0; j<electrons1.size(); j++)
    {
        for(unsigned int i=0; i<electrons2.size(); i++)
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

bool Run2Selection::foundZCandidate(std::vector<TRootMuon*>& muons1, std::vector<TRootMuon*>& muons2, float windowsize)
{
    bool foundZ = false;
    for(unsigned int j=0; j<muons1.size(); j++)
    {
        for(unsigned int i=0; i<muons2.size(); i++)
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

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(float PtThr, float EtaThr, float ElectronRelIso, bool mvaID, bool bx25, bool invertIdCut) const
{
    std::vector<TRootElectron*> selectedElectrons;
    //cout << ElectronRelIso << endl;
    for(unsigned int i=0; i<electrons.size(); i++)
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
        else
        {
            cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
            cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
            cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
            exit(1);
        }
        // Using 2012 MVA ID
        if(mvaID)
        {
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
        //Medium Cut-Based Electron Selection: Current cuts taken from EGM recommendations for CSA14 Studies
        else
        {
            if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
            {
                if(!bx25)   //Cuts for the 50ns Bunch Spacings Scenario for CSA14
                {
                    if(fabs(el->superClusterEta()) <= 1.479)
                    {
                        if(fabs(el->deltaEtaIn() < 0.015))
                            if(fabs(el->deltaPhiIn() < 0.051))
                                //if(el->e5x5() < 0.01)
                                if(el->hadronicOverEm() < 0.1)
                                    if(fabs(el->d0()) < 0.012)
                                        if(fabs(el->dz()) < 0.03)
                                            if(fabs(1/el->E() - 1/el->P()) < 0.53)
                                                if(el->relPfIso(3, 0.5) < 0.14)
                                                    if(el->passConversion())
                                                        if(el->missingHits() <= 1)
                                                            selectedElectrons.push_back(electrons[i]);
                    }
                    else if (fabs(el->superClusterEta()) < 2.5)
                    {
                        if(fabs(el->deltaEtaIn() < 0.023))
                            if(fabs(el->deltaPhiIn() < 0.056))
                                //if(el->e5x5() < 0.03)
                                if(el->hadronicOverEm() < 0.099)
                                    if(fabs(el->d0()) < 0.068)
                                        if(fabs(el->dz()) < 0.78)
                                            if(fabs(1/el->E() - 1/el->P()) < 0.11)
                                                if(el->relPfIso(3, 0.5) < 0.15)
                                                    if(el->passConversion())
                                                        if(el->missingHits() <= 1)
                                                            selectedElectrons.push_back(electrons[i]);

                    }
                }
                if(bx25)   //Cuts for the 25ns Bunch Spacings Scenario for PHYS14
                {
                    if(fabs(el->superClusterEta()) <= 1.479)
                    {
                        if(fabs(el->deltaEtaIn() < 0.007641))
                            if(fabs(el->deltaPhiIn() < 0.032643))
                                //if(el->e5x5() < 0.01)
                                if(el->hadronicOverEm() < 0.060662)
                                    if(fabs(el->d0()) < 0.011811)
                                        if(fabs(el->dz()) < 0.070775)
                                            if(fabs(1/el->E() - 1/el->P()) < 0.153897)
                                                if(el->relPfIso(3, 0.5) < 0.097213)
                                                    if(el->passConversion())
                                                        if(el->missingHits() <= 1)
                                                            selectedElectrons.push_back(electrons[i]);
                    }
                    else if (fabs(el->superClusterEta()) < 2.5)
                    {
                        if(fabs(el->deltaEtaIn() < 0.009285))
                            if(fabs(el->deltaPhiIn() < 0.042447))
                                //if(el->e5x5() < 0.032)
                                if(el->hadronicOverEm() < 0.104263)
                                    if(fabs(el->d0()) < 0.051682)
                                        if(fabs(el->dz()) < 0.180720)
                                            if(fabs(1/el->E() - 1/el->P()) < 0.137468)
                                                if(el->relPfIso(3, 0.5) < 0.116708)
                                                    if(el->passConversion())
                                                        if(el->missingHits() <= 1)
                                                            selectedElectrons.push_back(electrons[i]);

                    }
                }
            }
        }
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
std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons() const
{
    return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_);
}

/*
//Method including rho correction
std::vector<TRootElectron*> Selection::GetSelectedElectrons(float rho) const{
  return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_, rho);
}
*/
std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(vector<TRootJet*>& selJets) const
{
    return GetSelectedElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_, selJets);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(float EtThr, float EtaThr, float ElectronRelIso, vector<TRootJet*>& selJets) const
{
    std::vector<TRootElectron*> init_electrons = GetSelectedElectrons(EtThr,EtaThr,ElectronRelIso);
    std::vector<TRootElectron*> selectedElectrons;
    for(unsigned int i=0; i<init_electrons.size(); i++)
    {
        float mindRElJet = 999999.;
        TRootJet* jet;
        for(unsigned int j=0; j<selJets.size(); j++)
        {
            jet = selJets.at(j);
            float dRElJet = init_electrons[i]->DeltaR(*jet);
            if(dRElJet < mindRElJet) mindRElJet = dRElJet;
        }
        if(mindRElJet > ElectronDRJetsCut_)
        {
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

std::vector<TRootElectron*> Run2Selection::GetSelectedDiElectrons(float PtThr, float EtaThr, float ElectronRelIso) const
{
    std::vector<TRootElectron*> selectedElectrons;
    //cout << ElectronRelIso << endl;
    for(unsigned int i=0; i<electrons.size(); i++)
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
        else
        {
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

std::vector<TRootElectron*> Run2Selection::GetSelectedDiElectrons() const
{
    return GetSelectedDiElectrons(ElectronEtThreshold_, ElectronEtaThreshold_, ElectronRelIso_);
}




//std::vector<TRootElectron*> Selection::GetSelectedLooseElectrons(float PtThr, float EtaThr, float ElectronRelIso, bool vbtfid) const {
std::vector<TRootElectron*> Run2Selection::GetSelectedLooseElectrons(float PtThr, float EtaThr, float ElectronRelIso) const
{
    std::vector<TRootElectron*> selectedElectrons;
    for(unsigned int i=0; i<electrons.size(); i++)
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
        else
        {
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

std::vector<TRootElectron*> Run2Selection::GetSelectedLooseDiElectrons(float PtThr, float EtaThr, float ElectronRelIso) const
{
    std::vector<TRootElectron*> selectedElectrons;
    for(unsigned int i=0; i<electrons.size(); i++)
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
        else
        {
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

std::vector<TRootElectron*> Run2Selection::GetSelectedLooseDiElectrons() const
{
    return GetSelectedLooseDiElectrons(ElectronLooseEtThreshold_, ElectronLooseEtaThreshold_, ElectronLooseRelIso_);
}


std::vector<TRootElectron*> Run2Selection::GetSelectedElectronsInvIso(float ElectronRelIso) const
{
    std::vector<TRootJet*> empty;
    return GetSelectedElectronsInvIso(ElectronRelIso, empty);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectronsInvIso(float ElectronRelIso, vector<TRootJet*>& selJets) const
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
        else
        {
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

//=======================
//Jet Helper Functions===
//=======================

bool Run2Selection::passPFJetID8TEV(const TRootPFJet* PFJet) const
{
    if (PFJet->nConstituents() > 1 )
    {
        if (PFJet->neutralHadronEnergyFraction() < 0.99)
        {
            if (PFJet->neutralEmEnergyFraction() < 0.99 )
            {
                if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedEmEnergyFraction() < 0.99 )
                {
                    if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedHadronEnergyFraction() > 0)
                    {
                        if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedMultiplicity() > 0)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

//____________IS SELECTED_______________________________________________//
bool Run2Selection::isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut) {
    if(vertex.size()==0) return false;
    float Rho = sqrt(vertex[0]->x()*vertex[0]->x()+vertex[0]->y()*vertex[0]->y());
    if(!vertex[0]->isFake() && vertex[0]->ndof()>NdofCut && abs(vertex[0]->z())<Zcut && Rho<RhoCut) return true;
    return false;
    }

//______________________________________________________________________//