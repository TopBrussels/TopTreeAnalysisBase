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
	return muonSelector->GetSelectedMuons(PtThr,etaThr,relIso, WorkingPoint, ProductionCampaign);
}

// displaced muons
std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, float d0, float dz, int NValidPixelHits, int NMatchedStations, float MuonRelIso) const
{
	return muonSelector->GetSelectedDisplacedMuons(PtThr,EtaThr,NormChi2,NTrackerLayersWithMeas,NValidMuonHits,d0,dz,NValidPixelHits,NMatchedStations,MuonRelIso);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons(float PtThr,float EtaThr,float MuonRelIso) const
{
  return muonSelector->GetSelectedDisplacedMuons(PtThr,EtaThr,0.02,0.5,MuonRelIso);
}

std::vector<TRootMuon*> Run2Selection::GetSelectedDisplacedMuons() const
{
	return GetSelectedDisplacedMuons(35.,2.4,0.12);
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

float Run2Selection::GetElectronIsoCorrType(TRootElectron *el, bool bx25) const{
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
float Run2Selection::pfElectronIso(TRootElectron *el, bool bx25) const{
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

    return electronSelector->GetSelectedElectrons(PtThr, etaThr, WorkingPoint, ProductionCampaign, CutsBased);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr, float d0, float dz) const {

	return electronSelector->GetSelectedDisplacedElectrons(PtThr, EtaThr, d0, dz);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr) const {

	return electronSelector->GetSelectedDisplacedElectrons(PtThr, EtaThr);
}

std::vector<TRootElectron*> Run2Selection::GetSelectedDisplacedElectrons() const{
  return GetSelectedDisplacedElectrons(40.0, 2.4);
}

//____________IS SELECTED_______________________________________________//
bool Run2Selection::isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut) {
	if(vertex.size()==0) return false;
	float Rho = sqrt(vertex[0]->x()*vertex[0]->x()+vertex[0]->y()*vertex[0]->y());
	if(!vertex[0]->isFake() && vertex[0]->ndof()>NdofCut && abs(vertex[0]->z())<Zcut && Rho<RhoCut) return true;
	return false;
}

//______________________________________________________________________//

