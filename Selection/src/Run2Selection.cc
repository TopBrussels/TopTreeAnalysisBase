#include "../interface/Run2Selection.h"

//____CONSTRUCTORS______________________________________________________//

Run2Selection::Run2Selection()
{
	rho_ = 0;
	elecIsoCorrType_ = 2; // 0: no corr, 1: rho corr, 2: dB corr.
}


Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_,  const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_)
{
	rho_ = 0;
	elecIsoCorrType_ = 2; // 0: no corr, 1: rho corr, 2: dB corr.
	jetSelector = new JetSelection(jets_);
	electronSelector = new ElectronSelection(electrons_);
	muonSelector = new MuonSelection(muons_);
}

Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_,  const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_, float rho)
{
	rho_ = rho;
	elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
	jetSelector = new JetSelection(jets_);
	electronSelector = new ElectronSelection(electrons_, rho_);
	muonSelector = new MuonSelection(muons_);
}


Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootJet*>& fatjets_,  const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_)
{
	rho_ = 0;
	elecIsoCorrType_ = 2; // 0: no corr, 1: rho corr, 2: dB corr.
	jetSelector = new JetSelection(jets_);
	fatJetSelector = new FatJetSelection(fatjets_);
	electronSelector = new ElectronSelection(electrons_);
	muonSelector = new MuonSelection(muons_);
}

Run2Selection::Run2Selection(const std::vector<TRootJet*>& jets_, const std::vector<TRootJet*>& fatjets_, const std::vector<TRootMuon*>& muons_, const std::vector<TRootElectron*>& electrons_, const std::vector<TRootMET*>& mets_, float rho)
{
	rho_ = rho;
	elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
	jetSelector = new JetSelection(jets_);
	fatJetSelector = new FatJetSelection(fatjets_);
	electronSelector = new ElectronSelection(electrons_, rho_);
	muonSelector = new MuonSelection(muons_);
}

Run2Selection::Run2Selection(const Run2Selection& s)
{
	// copy the objects
	rho_ = s.rho_;
	elecIsoCorrType_ = s.elecIsoCorrType_; // 0: no corr, 1: rho corr, 2: dB corr.
	jetSelector = s.jetSelector;
	fatJetSelector = s.fatJetSelector;
	electronSelector = s.electronSelector;
	muonSelector = s.muonSelector;

}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

Run2Selection::~Run2Selection()
{
	delete jetSelector;
	delete fatJetSelector;
	delete electronSelector;
	delete muonSelector;
}

//__EXTRA METHODS_______________________________________________________//

//______________________________________________________________________//



// ______________JETS_________________________________________________//

// This should be the Standard getter function for Jets in Run-II.  The use of non PF objects should be discouraged.
std::vector<TRootPFJet*> Run2Selection::GetSelectedJets(float PtThr, float EtaThr, bool applyJetID, std::string TightLoose) const
{
	return jetSelector->GetSelectedJets(PtThr, EtaThr, applyJetID, TightLoose);
}


std::vector<TRootSubstructureJet*> Run2Selection::GetSelectedFatJets(float PtThr, float EtaThr, bool applyJetID) const
{

	return fatJetSelector->GetSelectedFatJets(PtThr, EtaThr, applyJetID);

}

std::vector<TRootPFJet*> Run2Selection::GetSelectedJets() const
{
	return jetSelector->GetSelectedJets(30,2.5,true,"Loose");
}

std::vector<TRootSubstructureJet*> Run2Selection::GetSelectedFatJets() const
{
	return fatJetSelector->GetSelectedFatJets(30,2.5,false);
}

std::vector<TRootPFJet*> Run2Selection::GetSelectedBJets(const std::vector<TRootPFJet*>& seljets, int& btagAlgo, float& btagCut) const
{
	std::vector<TRootPFJet*> bjets;
	Float_t btagDisc = 0;
	for(unsigned int i = 0; i<seljets.size(); i++)
	{
		if(btagAlgo == 1) btagDisc = seljets[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
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
	return muonSelector->GetSelectedMuons(PtThr,etaThr,relIso, WorkingPoint, ProductionCampaign);
}

// displaced muons
std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations, float MuonRelIso, bool applyIso, bool applyId) const
{
  return muonSelector->GetSelectedDisplacedMuons(PtThr,EtaThr,NormChi2,NTrackerLayersWithMeas,NValidMuonHits,NValidPixelHits,NMatchedStations,MuonRelIso,applyIso,applyId);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr,float EtaThr,float MuonRelIso, bool applyIso, bool applyId) const
{
  return muonSelector->GetSelectedDisplacedMuons(PtThr,EtaThr,MuonRelIso,applyIso,applyId);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons() const
{
  return muonSelector->GetSelectedDisplacedMuons(35.,2.4,0.12, true, true);
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
	return electronSelector->GetElectronIsoCorrType(el);
}

//This function gets the PF based Isolation for an Electron.  Since the Effective Areas for rho*effArea Isolation
//are different for 25 and 50 ns samples, a boolean must be supplied to indicate which bx scenario.  true -> 25 ns.  false -> 50 ns
float Run2Selection::pfElectronIso(TRootElectron *el) const{
	return electronSelector->pfElectronIso(el);

}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons() const
{
    return GetSelectedElectrons(30,2.5,"Tight","Spring16_80X",true,true);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(string WorkingPoint, string ProductionCampaign, bool CutsBased) const
{
    return GetSelectedElectrons(30,2.5,WorkingPoint,ProductionCampaign,CutsBased,true);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(float PtThr, float etaThr, string WorkingPoint) const
{
    return GetSelectedElectrons(PtThr,etaThr,WorkingPoint,"Spring16_80X",true,true);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedElectrons(float PtThr, float etaThr, string WorkingPoint, string ProductionCampaign, bool CutsBased, bool applyVID) const
{
    return electronSelector->GetSelectedElectrons(PtThr, etaThr, WorkingPoint, ProductionCampaign, CutsBased, applyVID);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr, float relIsoB, float relIsoEC, bool applyIso, bool applyId) const {

  return electronSelector->GetSelectedDisplacedElectrons(PtThr, EtaThr, relIsoB, relIsoEC, applyIso , applyId);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons() const{
  return GetSelectedDisplacedElectrons(40.0, 2.4, 0.0354, 0.0646, true, true);
}

//____________IS SELECTED_______________________________________________//
bool Run2Selection::isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut) {
	if(vertex.size()==0) return false;
	float Rho = sqrt(vertex[0]->x()*vertex[0]->x()+vertex[0]->y()*vertex[0]->y());
	if(!vertex[0]->isFake() && vertex[0]->ndof()>NdofCut && abs(vertex[0]->z())<Zcut && Rho<RhoCut) return true;
	return false;
}

//______________________________________________________________________//

