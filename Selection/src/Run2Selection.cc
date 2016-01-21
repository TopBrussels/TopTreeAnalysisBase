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



// ______________JETS_________________________________________________//

// This should be the Standard getter function for Jets in Run-II.  The use of non PF objects should be discouraged.
std::vector<TRootPFJet*> Run2Selection::GetSelectedJets(float PtThr, float EtaThr, bool applyJetID, std::string TightLoose) const
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
					if(TightLoose == "Loose" && passLoosePFJetID13TeV(PFJet))//This is the 13TeV Recommended Loose PFJet ID.  https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
					{
						selectedJets.push_back(init_jet);
					}
					else if(TightLoose == "Tight" && passTightPFJetID13TeV(PFJet))//This is the 13TeV Recommended Loose PFJet ID.  https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
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
	return GetSelectedJets(30,2.5,true,"Loose");
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


std::vector<TRootMuon*> Run2Selection::GetSelectedMuons() const
{
        //Default Muon selection method.  This method should be updated to the most recent set of recommended cuts regularly.  Currently it points to the MUON POG Medium working point with Pt, Eta, and dBeta RelIso thresholds set as in the TOP PAG selection TWiki for single lepton event ID.
	return GetSelectedMuons(26,2.1,0.15, "Tight", "Spring15");
}

std::vector<TRootMuon*> Run2Selection::GetSelectedMuons(float PtThr, float etaThr, float relIso, string WorkingPoint, string ProductionCampaign) const
{
	std::vector<TRootMuon* > MuonCollection;

		if (ProductionCampaign == "Spring15" && WorkingPoint == "Tight"){
			MuonCollection = GetSelectedTightMuonsJuly2015(PtThr, etaThr, relIso);
		}
		else if (ProductionCampaign == "Spring15" && WorkingPoint == "Medium"){
			MuonCollection = GetSelectedMediumMuonsJuly2015(PtThr, etaThr, relIso);
		}
		else if (ProductionCampaign == "Spring15" && WorkingPoint == "Loose"){
			MuonCollection = GetSelectedLooseMuonsJuly2015(PtThr, etaThr, relIso);
		}
		else {
			throw std::invalid_argument( "received incorrect args to GetSelectedMuons, requested: "+WorkingPoint+", "+ProductionCampaign);
		}

	return MuonCollection;
}

// displaced muons
std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations, float RelIso, bool applyIso, bool applyId) const
{
  // start from 'standard' muons with an extremely loose dz and d0 cut
  std::vector<TRootMuon*> selectedMuons;
  bool saveit=false;
  for(unsigned int i=0; i<muons.size(); i++)
    {
      saveit=false;
      TRootMuon *muon = (TRootMuon*) muons[i]; // type conversion, not very clear why necessary... also used in electrons
      
      if( muons[i]->Pt()>PtThr && fabs(muon->Eta())<EtaThr){
	
	// no id no iso
	if (!applyIso && !applyId) {
	  //	  cout << "no id and no iso" << endl;
	  saveit = true;
	}
	// apply iso only
        if(applyIso && isolationDisplacedMuon(muon,RelIso) && !applyId){
	  //	  cout << "iso cut required and passed" <<endl;
	  saveit=true;
	}
	// apply id only
        if( !applyIso  && applyId  && identificationDisplacedMuon(muon, NormChi2,  NTrackerLayersWithMeas, NValidMuonHits, NValidPixelHits, NMatchedStations)){
	  //	  cout << "id cut required and passed" <<endl;
	  saveit=true;
	}
	// apply both
        if( applyIso && isolationDisplacedMuon(muon,RelIso) && applyId  &&  identificationDisplacedMuon(muon, NormChi2,  NTrackerLayersWithMeas, NValidMuonHits, NValidPixelHits, NMatchedStations)){
	  //	  cout << "id and iso cut required and passed" <<endl;
	  saveit=true;
	}
	
	if (saveit) selectedMuons.push_back(muons[i]);
	
      }
      
    }
  
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr,float EtaThr,float MuonRelIso, bool applyIso, bool applyId) const
{
  return GetSelectedDisplacedMuons(PtThr,EtaThr,10.,5.,0,0,1,MuonRelIso,applyIso,applyId);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons() const
{
	return GetSelectedDisplacedMuons(35.,2.4,0.15,false,false);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedLooseMuonsJuly2015(float PtThr, float EtaThr,float MuonRelIso) const
{
	//Thes quality cuts reflect the LooseMuon ID as provided by the MUON POG.  PT, Eta, and Iso thresholds are not tuned as of July 17, 2015.
	std::vector<TRootMuon*> selectedMuons;
	for(unsigned int i=0; i<muons.size(); i++)
	{

		//float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
		// use cone 4 iso for muons:
		float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected
		if(     (muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && muons[i]->isPFMuon()
		   && muons[i]->Pt()>PtThr
		   && fabs(muons[i]->Eta())<EtaThr
		   && reliso < MuonRelIso)
		{
			selectedMuons.push_back(muons[i]);
		}
	}
	std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
	return selectedMuons;
}

std::vector<TRootMuon*> Run2Selection::GetSelectedMediumMuonsJuly2015(float PtThr, float EtaThr,float MuonRelIso) const
{
    //These quality cuts reflect the Medium Muon ID as provided by the MUON POG.  PT, Eta, and Iso thresholds are not tuned as of July 17, 2015.
	std::vector<TRootMuon*> selectedMuons;
	for(unsigned int i=0; i<muons.size(); i++)
	{

		//float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
		// use cone 4 iso for muons:
		float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected
                bool goodGlob = (muons[i]->isGlobalMuon()
                              && muons[i]->chi2() < 3
                              && muons[i]->chi2LocalPosition() < 12
                              && muons[i]->trkKink() < 20);

		if(     (muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && muons[i]->isPFMuon()
		   && muons[i]->Pt()>PtThr
		   && fabs(muons[i]->Eta())<EtaThr
		   && reliso < MuonRelIso
		   && muons[i]->validFraction() > 0.8
                   && muons[i]->segmentCompatibility() > (goodGlob ? 0.303 : 0.451))
		{
			selectedMuons.push_back(muons[i]);
		}
	}
	std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
	return selectedMuons;
}

std::vector<TRootMuon*> Run2Selection::GetSelectedTightMuonsJuly2015(float PtThr, float EtaThr,float MuonRelIso) const
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
		   && muons[i]->chi2() < 10
		   && muons[i]->nofTrackerLayersWithMeasurement() > 5
		   && muons[i]->nofValidMuHits() > 0
		   && fabs(muons[i]->d0()) < 0.2
		   && fabs(muons[i]->dz()) < 0.5
		   && muons[i]->nofValidPixelHits() > 0
		   && muons[i]->nofMatchedStations()> 1
		   && reliso < MuonRelIso)
		{
			selectedMuons.push_back(muons[i]);
		}
	}
	std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
	return selectedMuons;

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

float Run2Selection::GetElectronIsoCorrType(const TRootElectron *el, bool bx25) const{
	double EffectiveArea = 0.;
	if(bx25)
    {
        // Updated to Spring 2015 EA from https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_14/RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt#L8
        if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 1.0   ) EffectiveArea = 0.1752;
        if (fabs(el->superClusterEta()) >= 1.0   && fabs(el->superClusterEta()) < 1.479 ) EffectiveArea = 0.1862;
        if (fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.1411;
        if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.1534;
        if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.3   ) EffectiveArea = 0.1903;
        if (fabs(el->superClusterEta()) >= 2.3   && fabs(el->superClusterEta()) < 2.4   ) EffectiveArea = 0.2243;
        if (fabs(el->superClusterEta()) >= 2.4   && fabs(el->superClusterEta()) < 5.0   ) EffectiveArea = 0.2687;
    }
    else
    {
        // Updated to Spring 2015 EA from https://github.com/cms-sw/cmssw/blob/CMSSW_7_4_14/RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_50ns.txt
        if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 0.8   ) EffectiveArea = 0.0973;
        if (fabs(el->superClusterEta()) >= 0.8   && fabs(el->superClusterEta()) < 1.3   ) EffectiveArea = 0.0954;
        if (fabs(el->superClusterEta()) >= 1.3   && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.0632;
        if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.0727;
        if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 5.0   ) EffectiveArea = 0.1337;
    }

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

//This function gets the PF based Isolation for an Electron.  Since the Effective Areas for rho*effArea Isolation
//are different for 25 and 50 ns samples, a boolean must be supplied to indicate which bx scenario.  true -> 25 ns.  false -> 50 ns
float Run2Selection::pfElectronIso(const TRootElectron *el, bool bx25) const{
	float isoCorr = (el->neutralHadronIso(3) + el->photonIso(3) - GetElectronIsoCorrType(el, bx25));
	float isolation = (el->chargedHadronIso(3) + (isoCorr > 0.0 ? isoCorr : 0.0))/(el->Pt());

	return isolation;

}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons() const {
	return GetSelectedElectrons(30,2.5,"Tight","Spring15_25ns",true);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(string WorkingPoint, string ProductionCampaign, bool CutsBased) const {
	return GetSelectedElectrons(30,2.5,WorkingPoint,ProductionCampaign,CutsBased);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(float PtThr, float etaThr, string WorkingPoint, string ProductionCampaign, bool CutsBased) const {
	std::vector<TRootElectron* > ElectronCollection;
	if (CutsBased == true){
		if (ProductionCampaign == "PHYS14" && WorkingPoint == "Tight"){
			ElectronCollection = GetSelectedTightElectronsCutsBasedPHYS14(PtThr, etaThr);
		}
		else if (ProductionCampaign == "PHYS14" && WorkingPoint == "Medium"){
			ElectronCollection = GetSelectedMediumElectronsCutsBasedPHYS14(PtThr, etaThr);
		}
		else if (ProductionCampaign == "PHYS14" && WorkingPoint == "Loose"){
			ElectronCollection = GetSelectedLooseElectronsCutsBasedPHYS14(PtThr, etaThr);
		}
		else if (ProductionCampaign == "Spring15_50ns" && WorkingPoint == "Tight"){
			ElectronCollection = GetSelectedTightElectronsCutsBasedSpring15_50ns(PtThr, etaThr);
		}
		else if (ProductionCampaign == "Spring15_50ns" && WorkingPoint == "Medium"){
			ElectronCollection = GetSelectedMediumElectronsCutsBasedSpring15_50ns(PtThr, etaThr);
		}
		else if (ProductionCampaign == "Spring15_50ns" && WorkingPoint == "Loose"){
			ElectronCollection = GetSelectedLooseElectronsCutsBasedSpring15_50ns(PtThr, etaThr);
		}
		else if (ProductionCampaign == "Spring15_25ns" && WorkingPoint == "Tight"){
			ElectronCollection = GetSelectedTightElectronsCutsBasedSpring15_25ns(PtThr, etaThr);
		}
		else if (ProductionCampaign == "Spring15_25ns" && WorkingPoint == "Medium"){
			ElectronCollection = GetSelectedMediumElectronsCutsBasedSpring15_25ns(PtThr, etaThr);
		}
		else if (ProductionCampaign == "Spring15_25ns" && WorkingPoint == "Loose"){
			ElectronCollection = GetSelectedLooseElectronsCutsBasedSpring15_25ns(PtThr, etaThr);
		}
		else if (ProductionCampaign == "Spring15_25ns" && WorkingPoint == "Veto"){
			ElectronCollection = GetSelectedVetoElectronsCutsBasedSpring15_25ns(PtThr, etaThr);
		}
		else {
			string printboolval="Cutbased=true";
			if(!CutsBased)
				printboolval="Cutbased=false";

			throw std::invalid_argument( "received incorrect args to GetSelectedElectrons, requested: "+WorkingPoint+", "+ProductionCampaign+" "+printboolval);
		}
	}
	return ElectronCollection;
}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr,bool applyIso, bool applyId) const {
    // use tight electron ID (cut-based) for now, but without cuts on  d0 dz . This ID can be in flux, and for now is hard-coded here:

    //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 2 december 2015
    // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_25ns (revision 27)
    std::vector<TRootElectron*> selectedElectrons;
    bool saveit=false;

    for(unsigned int i=0; i<electrons.size(); i++) {
      saveit=false;
        TRootElectron* el = (TRootElectron*) electrons[i];
	
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {

	  // no id no iso
	  if (!applyIso && !applyId) {
	    //	    cout << "no id and no iso" << endl;
	    saveit = true;
	  }
	  // apply iso only                                   
	  if(applyIso && isolationDisplacedElectron(el) && !applyId){
	    //	    cout << "iso cut required and passed" <<endl;
	    saveit=true;
	  }
	  // apply id only
	  if( !applyIso  && applyId  && identificationDisplacedElectron(el)){
	    //	    cout << "id cut required and passed" <<endl;
	    saveit=true;
	  }
	  // apply both
	  if( applyIso && isolationDisplacedElectron(el) && applyId && identificationDisplacedElectron(el)){
	    //	    cout << "id and iso cut required and passed" <<endl;
	    saveit=true;
	  }
	  if(saveit) selectedElectrons.push_back(electrons[i]);
        }
    }

    std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
    return selectedElectrons;
}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons() const{
  return GetSelectedDisplacedElectrons(42.0, 2.4,false,false);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedTightElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 11 Jan 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
    for(unsigned int i=0; i<electrons.size(); i++) {
        TRootElectron* el = (TRootElectron*) electrons[i];
        
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr){
            // Using cut-based, BARREL:
            if( fabs(el->superClusterEta()) <= 1.479){
                if(fabs(el->deltaEtaIn()) <  0.00864
                   && fabs(el->deltaPhiIn()) < 0.0286
                   && el->sigmaIEtaIEta_full5x5() < 0.0101
                   && el->hadronicOverEm() < 0.0342
                   && el->ioEmIoP() < 0.0116
                   && pfElectronIso(el, false) <  0.0591
                   && el->passConversion()
                   && el->missingHits() <= 2)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            // using cut-based, ENDCAP:
            else if (fabs(el->superClusterEta()) < 2.5){
                if(fabs(el->deltaEtaIn()) <  0.00762
                   && fabs(el->deltaPhiIn()) <  0.0439
                   && el->sigmaIEtaIEta_full5x5() <  0.0287
                   && (el->hadronicOverEm() < 0.0544)
                   && el->ioEmIoP() < 0.01
                   && pfElectronIso(el, false) < 0.0759
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
    }
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> Run2Selection::GetSelectedMediumElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 August 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479){
                if( fabs(el->deltaEtaIn()) < 0.0094
                   && fabs(el->deltaPhiIn()) < 0.0296
                   && el->sigmaIEtaIEta_full5x5() < 0.0101
                   && el->hadronicOverEm() < 0.0372
                   && fabs(el->d0()) < 0.0151
                   && fabs(el->dz()) < 0.238
                   && el->ioEmIoP() < 0.118
                   && pfElectronIso(el, false) < 0.0987
                   && el->passConversion()
                   && el->missingHits() <= 2)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5){
                if( fabs(el->deltaEtaIn()) <  0.00773
                   && fabs(el->deltaPhiIn()) < 0.148
                   && el->sigmaIEtaIEta_full5x5() < 0.0287
                   && (el->hadronicOverEm() < 0.0546)
                   && fabs(el->d0()) < 0.0535
                   && fabs(el->dz()) < 0.572
                   && el->ioEmIoP() < 0.104
                   && pfElectronIso(el, false) < 0.0902
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> Run2Selection::GetSelectedLooseElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479){
                if( fabs(el->deltaEtaIn()) < 0.00976
                   && fabs(el->deltaPhiIn()) < 0.0929
                   && el->sigmaIEtaIEta_full5x5() <  0.0105
                   && el->hadronicOverEm() < 0.0765
                   && fabs(el->d0()) <  0.0227
                   && fabs(el->dz()) <  0.379
                   && el->ioEmIoP() <  0.184
                   && pfElectronIso(el, false) <  0.118
                   && el->passConversion()
                   && el->missingHits() <= 2)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5){
                if( fabs(el->deltaEtaIn()) <  0.00952
                   && fabs(el->deltaPhiIn()) < 0.181
                   && el->sigmaIEtaIEta_full5x5() <  0.0318
                   && (el->hadronicOverEm() < 0.0824)
                   && fabs(el->d0()) <  0.242
                   && fabs(el->dz()) <  0.921
                   && el->ioEmIoP() <  0.125
                   && pfElectronIso(el, false) <  0.118
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}

std::vector<TRootElectron*> Run2Selection::GetSelectedTightElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based, BARREL:
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479){
                if( fabs(el->deltaEtaIn()) <  0.00926
                   && fabs(el->deltaPhiIn()) < 0.0336
                   && el->sigmaIEtaIEta_full5x5() < 0.0101
                   && el->hadronicOverEm() < 0.0597
                   && fabs(el->d0()) < 0.0111
                   && fabs(el->dz()) < 0.0466
                   && el->ioEmIoP() < 0.012
                   && pfElectronIso(el, true) <  0.0354
                   && el->passConversion()
                   && el->missingHits() <= 2)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            // using cut-based, ENDCAP:
            else if (fabs(el->superClusterEta()) < 2.5){
                if( fabs(el->deltaEtaIn()) <  0.00724
                   && fabs(el->deltaPhiIn()) <  0.0918
                   && el->sigmaIEtaIEta_full5x5() <  0.0279
                   && (el->hadronicOverEm() < 0.0615)
                   && fabs(el->d0()) < 0.0351
                   && fabs(el->dz()) < 0.417
                   && el->ioEmIoP() < 0.00999
                   && pfElectronIso(el, true) < 0.0646
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> Run2Selection::GetSelectedMediumElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479 ){
                if( fabs(el->deltaEtaIn()) < 0.0103
                   && fabs(el->deltaPhiIn()) < 0.0336
                   && el->sigmaIEtaIEta_full5x5() < 0.0101
                   && el->hadronicOverEm() < 0.0876
                   && fabs(el->d0()) < 0.0118
                   && fabs(el->dz()) < 0.373
                   && el->ioEmIoP() < 0.0174
                   && pfElectronIso(el, true) < 0.0766
                   && el->passConversion()
                   && el->missingHits() <= 2)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5){
                if( fabs(el->deltaEtaIn()) <  0.00733
                   && fabs(el->deltaPhiIn()) < 0.114
                   && el->sigmaIEtaIEta_full5x5() < 0.0283
                   && (el->hadronicOverEm() < 0.0678)
                   && fabs(el->d0()) < 0.0739
                   && fabs(el->dz()) < 0.602
                   && el->ioEmIoP() < 0.0898
                   && pfElectronIso(el, true) < 0.0678
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> Run2Selection::GetSelectedLooseElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479 ){
                if( fabs(el->deltaEtaIn()) < 0.0105
                   && fabs(el->deltaPhiIn()) < 0.115
                   && el->sigmaIEtaIEta_full5x5() <  0.0103
                   && el->hadronicOverEm() < 0.104
                   && fabs(el->d0()) <  0.0261
                   && fabs(el->dz()) <  0.41
                   && el->ioEmIoP() <  0.102
                   && pfElectronIso(el, true) <  0.0893
                   && el->passConversion()
                   && el->missingHits() <= 2)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5 ){
                if( fabs(el->deltaEtaIn()) <  0.00814
                   && fabs(el->deltaPhiIn()) < 0.182
                   && el->sigmaIEtaIEta_full5x5() <  0.0301
                   && (el->hadronicOverEm() < 0.0897)
                   && fabs(el->d0()) <  0.118
                   && fabs(el->dz()) <  0.822
                   && el->ioEmIoP() <  0.126
                   && pfElectronIso(el, true) <  0.121
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> Run2Selection::GetSelectedVetoElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479 ){
                if( fabs(el->deltaEtaIn()) < 0.0152
                   && fabs(el->deltaPhiIn()) < 0.216
                   && el->sigmaIEtaIEta_full5x5() <  0.0114
                   && el->hadronicOverEm() < 0.181
                   && fabs(el->d0()) <  0.0564
                   && fabs(el->dz()) <  0.472
                   && el->ioEmIoP() <  0.207
                   && pfElectronIso(el, true) <  0.126
                   && el->passConversion()
                   && el->missingHits() <= 2)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5){
                if( fabs(el->deltaEtaIn()) <  0.0113
                   && fabs(el->deltaPhiIn()) < 0.237
                   && el->sigmaIEtaIEta_full5x5() <  0.0352
                   && (el->hadronicOverEm() < 0.116)
                   && fabs(el->d0()) <  0.222
                   && fabs(el->dz()) <  0.921
                   && el->ioEmIoP() <  0.174
                   && pfElectronIso(el, true) <  0.144
                   && el->passConversion()
                   && el->missingHits() <= 3)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}



std::vector<TRootElectron*> Run2Selection::GetSelectedTightElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479 ){
                if( fabs(el->deltaEtaIn()) < 0.006046
                   && fabs(el->deltaPhiIn()) < 0.028092
                   && el->sigmaIEtaIEta() < 0.009947
                   && el->hadronicOverEm() < 0.045772
                   && fabs(el->d0()) < 0.008790
                   && fabs(el->dz()) < 0.021226
                   && el->ioEmIoP() < 0.020118
                   && pfElectronIso(el, true) < 0.069537
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5) {
                if( fabs(el->deltaEtaIn()) < 0.007057
                   && fabs(el->deltaPhiIn()) < 0.030159
                   && el->sigmaIEtaIEta() < 0.028237
                   && (el->hadronicOverEm() < 0.067778)
                   && fabs(el->d0()) < 0.027984
                   && fabs(el->dz()) < 0.133431
                   && el->ioEmIoP() < 0.098919
                   && pfElectronIso(el, true) < 0.078265
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}

std::vector<TRootElectron*> Run2Selection::GetSelectedMediumElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Medium cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
    // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479 ){
                if( fabs(el->deltaEtaIn()) < 0.008925
                   && fabs(el->deltaPhiIn()) < 0.035973
                   && el->sigmaIEtaIEta() < 0.009996
                   && el->hadronicOverEm() < 0.050537
                   && fabs(el->d0()) < 0.012235
                   && fabs(el->dz()) < 0.042020
                   && el->ioEmIoP() < 0.091942
                   && pfElectronIso(el, true) < 0.107587
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5 ){
                if( fabs(el->deltaEtaIn()) < 0.007429
                   && fabs(el->deltaPhiIn()) < 0.067879
                   && el->sigmaIEtaIEta() <0.030135
                   && el->hadronicOverEm() < 0.086782
                   && fabs(el->d0()) < 0.036719
                   && fabs(el->dz()) < 0.138142
                   && el->ioEmIoP() < 0.100683
                   && pfElectronIso(el, true) < 0.113254
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
        }
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
	return selectedElectrons;
}

std::vector<TRootElectron*> Run2Selection::GetSelectedLooseElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	// These quality cuts reflect the recommended Loose cut-based electron ID as provided by the EGM POG. Last updated: 11 January 2016
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#PHYS14_selection_all_conditions
	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
        if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
            if( fabs(el->superClusterEta()) <= 1.479 ){
                if( fabs(el->deltaEtaIn()) < 0.009277
                   && fabs(el->deltaPhiIn()) < 0.094739
                   && el->sigmaIEtaIEta() < 0.010331
                   && el->hadronicOverEm() < 0.093068
                   && fabs(el->d0()) < 0.035904
                   && fabs(el->dz()) < 0.075496
                   && el->ioEmIoP() < 0.189968
                   && pfElectronIso(el, true) < 0.130136
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
            }
            else if (fabs(el->superClusterEta()) < 2.5 ){
                if( fabs(el->deltaEtaIn()) < 0.009833
                   && fabs(el->deltaPhiIn()) < 0.149934
                   && el->sigmaIEtaIEta() < 0.031838
                   && (el->hadronicOverEm() < 0.115754)
                   && fabs(el->d0()) < 0.099266
                   && fabs(el->dz()) < 0.197897
                   && el->ioEmIoP() < 0.140662
                   && pfElectronIso(el, true) < 0.163368
                   && el->passConversion()
                   && el->missingHits() <= 1)
                {
                    selectedElectrons.push_back(electrons[i]);
                }
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
bool Run2Selection::passLoosePFJetID13TeV(const TRootPFJet* PFJet) const
{
	if (fabs(PFJet->Eta()) <= 3.0)
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
	}
	else if (PFJet->neutralEmEnergyFraction() < 0.9 )
	{
		if(PFJet->neutralMultiplicity() > 10)
		{
			return true;
		}
	}
	return false;
}
bool Run2Selection::passTightPFJetID13TeV(const TRootPFJet* PFJet) const
{
	if(fabs(PFJet->Eta()) <= 3.0)
	{
		if (PFJet->nConstituents() > 1 )
		{
			if (PFJet->neutralHadronEnergyFraction() < 0.9)
			{
				if (PFJet->neutralEmEnergyFraction() < 0.9 )
				{
					if (fabs(PFJet->Eta()) >= 2.4 || PFJet->chargedEmEnergyFraction() < 0.9 )
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
	}
	else if (PFJet->neutralEmEnergyFraction() < 0.9 )
	{
		if(PFJet->neutralMultiplicity() > 10)
		{
			return true;
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

//---- selection functions for displaced electrons and muons. factorising ID and isolation.
bool Run2Selection::isolationDisplacedElectron(const TRootElectron* el) const{
    if( fabs(el->superClusterEta()) <= 1.479){
        if(pfElectronIso(el) < 0.0354)
            return true;
        else
            return false;
    }
    // For the endcap
    else if (fabs(el->superClusterEta()) < 2.5){
            if(pfElectronIso(el) < 0.0646)
                return true;
            else
                return false;
    }
    return false;
}
bool Run2Selection::identificationDisplacedElectron(const TRootElectron* el) const{
  //  cout << "entering the displaced Id electron" << endl;
  if( fabs(el->superClusterEta()) <= 1.479){
    if ( el->sigmaIEtaIEta() < 0.0101
	 && fabs(el->deltaEtaIn()) < 0.00926
	 && fabs(el->deltaPhiIn()) < 0.0336
	 && el->hadronicOverEm() < 0.0597
	 && el->ioEmIoP() < 0.012
	 && el->missingHits() <= 2 // check wrt to expectedMissingInnerHits
	 && el->passConversion())
      {
	//	cout << "the displaced Id electron is true" << endl;
        return true;
      }
  }
  // For the endcap
  else if (fabs(el->superClusterEta()) < 2.5)
    {
      if ( el->sigmaIEtaIEta() < 0.0279
	   && fabs(el->deltaEtaIn()) < 0.00724
	   && fabs(el->deltaPhiIn()) < 0.0918
	   && el->hadronicOverEm() < 0.0615
	   && el->ioEmIoP() < 0.00999
	   && el->missingHits() <= 1 // check wrt to expectedMissingInnerHits
	   && el->passConversion())
	{
	  //      cout << "the displaced Id electron is true" << endl;
	  return true;
	}
    }
  return false;

}
bool Run2Selection::identificationDisplacedMuon(const TRootMuon* muon, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations) const{
    
    // use cone 4 iso for muons:

    if( muon->isGlobalMuon() && muon->isPFMuon()
	      && muon->chi2() < NormChi2
	      && muon->nofTrackerLayersWithMeasurement() > NTrackerLayersWithMeas
	      && muon->nofValidPixelHits() > NValidPixelHits
	      && muon->nofMatchedStations()> NMatchedStations)
    {
        return true;
    }
    return false;
}
bool Run2Selection::isolationDisplacedMuon(const TRootMuon* muon, float RelIso) const{
    if( muon->Pt()<0.0001) // protecting against the strange case that the pT is zero... just to be sure
        return false;
    float reliso = (muon->chargedHadronIso(4) + max( 0.0, muon->neutralHadronIso(4) + muon->photonIso(4) - 0.5*muon->puChargedHadronIso(4) ) ) / muon->Pt(); // dBeta corrected

    if(reliso< RelIso)
        return true;
    return false;
}



