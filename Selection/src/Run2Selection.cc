#include "../interface/Run2Selection.h"

//____CONSTRUCTORS______________________________________________________//

Run2Selection::Run2Selection()
{
    rho_ = 0;
    elecIsoCorrType_ = 2; // 0: no corr, 1: rho corr, 2: dB corr.
    cutHFHadronEnergyFraction_ = false;
}


Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_,  const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_)
{
    rho_ = 0;
    elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
    for(unsigned int i=0; i<jets_.size(); i++) jets.push_back(jets_[i]);
    for(unsigned int i=0; i<muons_.size(); i++) muons.push_back(muons_[i]);
    for(unsigned int i=0; i<electrons_.size(); i++) electrons.push_back(electrons_[i]);
    for(unsigned int i=0; i<mets_.size(); i++) mets.push_back(mets_[i]);
    cutHFHadronEnergyFraction_ = false;
}

Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_,  const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_, float rho)
{
    rho_ = rho;
    elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
    for(unsigned int i=0; i<jets_.size(); i++) jets.push_back(jets_[i]);
    for(unsigned int i=0; i<muons_.size(); i++) muons.push_back(muons_[i]);
    for(unsigned int i=0; i<electrons_.size(); i++) electrons.push_back(electrons_[i]);
    for(unsigned int i=0; i<mets_.size(); i++) mets.push_back(mets_[i]);
    cutHFHadronEnergyFraction_ = false;
}


Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootJet*>& fatjets_,  const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_)
{
    rho_ = 0;
    elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
    for(unsigned int i=0; i<jets_.size(); i++) jets.push_back(jets_[i]);
    for(unsigned int i=0; i<fatjets_.size(); i++) fatjets.push_back(fatjets_[i]);
    for(unsigned int i=0; i<muons_.size(); i++) muons.push_back(muons_[i]);
    for(unsigned int i=0; i<electrons_.size(); i++) electrons.push_back(electrons_[i]);
    for(unsigned int i=0; i<mets_.size(); i++) mets.push_back(mets_[i]);
    cutHFHadronEnergyFraction_ = false;
}

Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootJet*>& fatjets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_, float rho)
{
    rho_ = rho;
    elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
    for(unsigned int i=0; i<jets_.size(); i++) jets.push_back(jets_[i]);
    for(unsigned int i=0; i<fatjets_.size(); i++) fatjets.push_back(fatjets_[i]);
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
    fatjets.clear();
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
                    if(passPFJetID13TEV(PFJet))//This is the 8TeV Recommended Loose PFJet ID.  This should be updated when 13TeV recommendations become available.
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


std::vector<TRootSubstructureJet*> Run2Selection::GetSelectedFatJets(float PtThr, float EtaThr, bool applyJetID) const
{
    std::vector<TRootSubstructureJet*> selectedJets;
    for(unsigned int i=0; i<fatjets.size(); i++)
    {

        TRootSubstructureJet* init_jet = (TRootSubstructureJet*) fatjets[i];

            const TRootSubstructureJet* PFJet = static_cast<const TRootSubstructureJet*>(init_jet);

            if( fabs(PFJet->Eta())<EtaThr && PFJet->Pt()>PtThr )
            {
	      //if ( applyJetID )
		   //{
		  //if(passPFJetID8TEV(PFJet))//This is the 8TeV Recommended Loose PFJet ID.  This should be updated when 13TeV recommendations become available.
		      // {
                        selectedJets.push_back(init_jet);
			// }
		    // }
	      // else selectedJets.push_back(init_jet);
            }
    }
    std::sort(selectedJets.begin(),selectedJets.end(),HighestPt());
    return selectedJets;

}




std::vector<TRootPFJet*> Run2Selection::GetSelectedJets() const
{
    return GetSelectedJets(30,2.5,true);
}


std::vector<TRootSubstructureJet*> Run2Selection::GetSelectedFatJets() const
{
    return GetSelectedFatJets(30,2.5,false);
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
    //Thes quality cuts reflect the Tight Muon ID as provided by the MUON POG.  PT, Eta, and Iso thresholds are not tuned as of July 17, 2015.
    std::vector<TRootMuon*> selectedMuons;
    for(unsigned int i=0; i<muons.size(); i++)
    {

        //float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
		// use cone 4 iso for muons:
        float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected
        if(     muons[i]->isGlobalMuon() && muons[i]->isPFMuon()
                && muons[i]->Pt()>PtThr
                && fabs(muons[i]->Eta())<EtaThr
                && muons[i]->chi2() < NormChi2
                && muons[i]->nofTrackerLayersWithMeasurement() > NTrackerLayersWithMeas
                //&& muons[i]->nofValidMuHits() > NValidMuonHits
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

// displaced muons
std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, float d0, float dZ, int NValidPixelHits, int NMatchedStations, float RelIso) const
{
	// start from 'standard' muons with an extremely loose dz and d0 cut
	std::vector<TRootMuon*> selectedMuons = GetSelectedMuons(PtThr,EtaThr,NormChi2,NTrackerLayersWithMeas,NValidMuonHits,1000,1000,NValidPixelHits,NMatchedStations,RelIso);
	std::vector<TRootMuon*> chosenMuons;
	//	and then loop over the list and copy over good ones to the new array, making selection cuts on the beam spot information instead of the 'standard' d0 and dz info
	for(unsigned int ii=0; ii< selectedMuons.size(); ii++){
		if(fabs(selectedMuons[ii]->d0BeamSpot())<d0)
			continue;
		if(fabs(selectedMuons[ii]->dzBeamSpot())>dZ)
			continue;
			
		chosenMuons.push_back(selectedMuons[ii]);
		
	}
	
	std::sort(chosenMuons.begin(),chosenMuons.end(),HighestPt());
	return chosenMuons;
}
std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr,float EtaThr,float d0, float dz,float MuonRelIso) const
{
	return GetSelectedDisplacedMuons(PtThr,EtaThr,10.,5.,0,d0,dz,0,1,MuonRelIso);
	
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons() const
{
  return GetSelectedDisplacedMuons(30.,2.5,0.02,0.5,0.2);
}


std::vector<TRootMuon*> Run2Selection::GetSelectedDiMuons(float PtThr, float EtaThr,float MuonRelIso) const
{
    std::vector<TRootMuon*> selectedMuons;
    for(unsigned int i=0; i<muons.size(); i++)
    {
        float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected
        if((muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon())
                && muons[i]->isPFMuon()
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
		// use cone 4 iso for muons:
        float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected

        if((muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && muons[i]->isPFMuon() && fabs(muons[i]->Eta())<EtaThr && muons[i]->Pt()>PtThr && reliso < MuonRelIso )
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

// ______________ELECTRONS______________________________________________//

float Run2Selection::GetElectronIsoCorrType(TRootElectron *el) const{
    double EffectiveArea = 0.;
    // Updated to 2015 EA from https://indico.cern.ch/event/370494/contribution/2/attachments/736984/1011061/Rami_update_on_CB_ELE_ID_PHYS14PU20bx25.pdf
    if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 0.8   ) EffectiveArea = 0.1013;
    if (fabs(el->superClusterEta()) >= 0.8   && fabs(el->superClusterEta()) < 1.3 ) EffectiveArea = 0.0988;
    if (fabs(el->superClusterEta()) >= 1.3 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.0572;
    if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.0842;
    if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.5   ) EffectiveArea = 0.1530;
    if (fabs(el->superClusterEta()) >= 2.5) EffectiveArea = -9999;

    double isocorr = 0;
    if(elecIsoCorrType_ == 1) // rho correction (default corr)
        isocorr = rho_*EffectiveArea;
    else if(elecIsoCorrType_ == 2) // dB correction
        isocorr = 0.5*el->puChargedHadronIso(3);
    else if (elecIsoCorrType_ == 0) // no correction
        isocorr = 0.;
    else {
        cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
        cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
        cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
        exit(1);
    }
    return isocorr;
}

float Run2Selection::pfElectronIso(TRootElectron *el) const{
    float isoCorr = (el->neutralHadronIso(3) + el->photonIso(3) - GetElectronIsoCorrType(el));
    float isolation = (el->chargedHadronIso(3) + (isoCorr > 0.0 ? isoCorr : 0.0))/(el->Pt());

    return isolation;

}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(string WorkingPoint, string PHYS14orCSA14, bool CutsBased) const {
    std::vector<TRootElectron* > ElectronCollection;
    if (CutsBased == true){
        if (PHYS14orCSA14 == "PHYS14" && WorkingPoint == "Tight"){
            ElectronCollection = GetSelectedTightElectronsCutsBasedPHYS14(30, 2.5);
            //ElectronCollection.insert(ElectronCollection.end(), GetSelectedTightElectronsCutsBasedPHYS14(30, 2.5).begin(), GetSelectedTightElectronsCutsBasedPHYS14(30, 2.5).end());
        }
        else if (PHYS14orCSA14 == "PHYS14" && WorkingPoint == "Medium"){
            ElectronCollection = GetSelectedMediumElectronsCutsBasedPHYS14(30, 2.5);
        }
        else if (PHYS14orCSA14 == "PHYS14" && WorkingPoint == "Loose"){
            ElectronCollection = GetSelectedLooseElectronsCutsBasedPHYS14(30, 2.5);
        }
        else {
            string printboolval="Cutbased=true";
            if(!CutsBased)
                printboolval="Cutbased=false";

            throw std::invalid_argument( "received incorrect args to GetSelectedElectrons, requested: "+WorkingPoint+", "+PHYS14orCSA14+" "+printboolval);
            //cout<<"Selected electron error"<<endl;
        }
    }
    return ElectronCollection;
}



std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr, float d0, float dz) const {
	
	// use medium electron ID (cut-based) for now, but with cuts on the beam spot d0 dz . This ID can be in flux, and for now is hard-coded here:
	
	//These quality cuts reflect the recommended Medium cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions
	
	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
		if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
			if( fabs(el->superClusterEta()) <= 1.479
			   && fabs(el->deltaEtaIn()) < 0.008925
			   && fabs(el->deltaPhiIn()) < 0.035973
			   && el->sigmaIEtaIEta() < 0.009996
			   && el->hadronicOverEm() < 0.050537
			   && fabs(el->d0BeamSpot()) > d0
			   && fabs(el->dzBeamSpot()) < dz
			   && fabs(1/el->E() - 1/el->P()) < 0.091942
			   && pfElectronIso(el) < 0.107587
			   && el->passConversion()
			   && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
			
			else if (fabs(el->superClusterEta()) < 2.5
					 && fabs(el->deltaEtaIn()) < 0.007429
					 && fabs(el->deltaPhiIn()) < 0.067879
					 && el->sigmaIEtaIEta() <0.030135
					 && el->hadronicOverEm() < 0.086782
					 && fabs(el->d0BeamSpot()) > d0
					 && fabs(el->dzBeamSpot()) < dz
					 && fabs(1/el->E() - 1/el->P()) < 0.100683
					 && pfElectronIso(el) < 0.113254
					 && el->passConversion()
					 && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
		}
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;

	
	return selectedElectrons;
}


std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr) const {
	return GetSelectedDisplacedElectrons(PtThr,EtaThr,0.02,0.2);

}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons() const{
  return GetSelectedDisplacedElectrons(30, 2.5);
}


std::vector<TRootElectron*> Run2Selection::GetSelectedTightElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
    // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
    //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
    // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions

    std::vector<TRootElectron*> selectedElectrons;
    for(unsigned int i=0; i<electrons.size(); i++) {
        TRootElectron* el = (TRootElectron*) electrons[i];
        // Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479
                && fabs(el->deltaEtaIn()) < 0.006046
                && fabs(el->deltaPhiIn()) < 0.028092
                && el->sigmaIEtaIEta() < 0.009947
                && el->hadronicOverEm() < 0.045772
                && fabs(el->d0()) < 0.008790
                && fabs(el->dz()) < 0.021226
                && fabs(1/el->E() - 1/el->P()) < 0.020118
                && pfElectronIso(el) < 0.069537
                && el->passConversion()
                && el->missingHits() <= 1)
            {
                selectedElectrons.push_back(electrons[i]);
            }

            else if (fabs(el->superClusterEta()) < 2.5
                && fabs(el->deltaEtaIn()) < 0.007057
                && fabs(el->deltaPhiIn()) < 0.030159
                && el->sigmaIEtaIEta() < 0.028237
                && (el->hadronicOverEm() < 0.067778)
                && fabs(el->d0()) < 0.027984
                && fabs(el->dz()) < 0.133431
                && fabs(1/el->E() - 1/el->P()) < 0.098919
                && pfElectronIso(el) < 0.078265
                && el->passConversion()
                && el->missingHits() <= 1)
            {
                selectedElectrons.push_back(electrons[i]);
            }
        }
    }
    std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
    return selectedElectrons;
}

std::vector<TRootElectron*> Run2Selection::GetSelectedMediumElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
    // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
    //These quality cuts reflect the recommended Medium cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
    // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions

    std::vector<TRootElectron*> selectedElectrons;
    for(unsigned int i=0; i<electrons.size(); i++) {
        TRootElectron* el = (TRootElectron*) electrons[i];
        // Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479
               && fabs(el->deltaEtaIn()) < 0.008925
               && fabs(el->deltaPhiIn()) < 0.035973
               && el->sigmaIEtaIEta() < 0.009996
               && el->hadronicOverEm() < 0.050537
               && fabs(el->d0()) < 0.012235
               && fabs(el->dz()) < 0.042020
               && fabs(1/el->E() - 1/el->P()) < 0.091942
               && pfElectronIso(el) < 0.107587
               && el->passConversion()
               && el->missingHits() <= 1)
            {
                selectedElectrons.push_back(electrons[i]);
            }
            
            else if (fabs(el->superClusterEta()) < 2.5
                     && fabs(el->deltaEtaIn()) < 0.007429
                     && fabs(el->deltaPhiIn()) < 0.067879
                     && el->sigmaIEtaIEta() <0.030135
                     && el->hadronicOverEm() < 0.086782
                     && fabs(el->d0()) < 0.036719
                     && fabs(el->dz()) < 0.138142
                     && fabs(1/el->E() - 1/el->P()) < 0.100683
                     && pfElectronIso(el) < 0.113254
                     && el->passConversion()
                     && el->missingHits() <= 1)
            {
                selectedElectrons.push_back(electrons[i]);
            }
        }
    }
    std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
    return selectedElectrons;
}

std::vector<TRootElectron*> Run2Selection::GetSelectedLooseElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
    // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
    // These quality cuts reflect the recommended Loose cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
    // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions
    std::vector<TRootElectron*> selectedElectrons;
    for(unsigned int i=0; i<electrons.size(); i++) {
        TRootElectron* el = (TRootElectron*) electrons[i];
        // Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479
                && fabs(el->deltaEtaIn()) < 0.009277
                && fabs(el->deltaPhiIn()) < 0.094739
                && el->sigmaIEtaIEta() < 0.010331
                && el->hadronicOverEm() < 0.093068
                && fabs(el->d0()) < 0.035904
                && fabs(el->dz()) < 0.075496
                && fabs(1/el->E() - 1/el->P()) < 0.189968
                && pfElectronIso(el) < 0.130136
                && el->passConversion()
                && el->missingHits() <= 1)
            {
                selectedElectrons.push_back(electrons[i]);
            }

            else if (fabs(el->superClusterEta()) < 2.5
                && fabs(el->deltaEtaIn()) < 0.009833
                && fabs(el->deltaPhiIn()) < 0.149934
                && el->sigmaIEtaIEta() < 0.031838
                && (el->hadronicOverEm() < 0.115754)
                && fabs(el->d0()) < 0.099266
                && fabs(el->dz()) < 0.197897
                && fabs(1/el->E() - 1/el->P()) < 0.140662
                && pfElectronIso(el) < 0.163368
                && el->passConversion()
                && el->missingHits() <= 1)
            {
                selectedElectrons.push_back(electrons[i]);
            }
        }
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
bool Run2Selection::passPFJetID13TEV(const TRootPFJet* PFJet) const
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
                            if(PFJet->muonEnergyFraction() <= 0.8)
                            {
                                return true;
                            }
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
