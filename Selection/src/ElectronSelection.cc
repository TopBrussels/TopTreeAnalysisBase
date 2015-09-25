#include "../interface/ElectronSelection.h"

//____CONSTRUCTORS______________________________________________________//

ElectronSelection::ElectronSelection()
{
	rho_ = 0;
	elecIsoCorrType_ = 2; // 0: no corr, 1: rho corr, 2: dB corr.
}


ElectronSelection::ElectronSelection(const std::vector<TRootElectron*>& electrons_)
{
	rho_ = 0;
	elecIsoCorrType_ = 2; // 0: no corr, 1: rho corr, 2: dB corr.
	for(unsigned int i=0; i<electrons_.size(); i++) electrons.push_back(electrons_[i]);
}

ElectronSelection::ElectronSelection(const std::vector<TRootElectron*>& electrons_, float rho)
{
	rho_ = rho;
	elecIsoCorrType_ = 1; // 0: no corr, 1: rho corr, 2: dB corr.
	for(unsigned int i=0; i<electrons_.size(); i++) electrons.push_back(electrons_[i]);
}

ElectronSelection::ElectronSelection(const ElectronSelection& s)
{
	// copy the objects
	rho_ = s.rho_;
	elecIsoCorrType_ = s.elecIsoCorrType_; // 0: no corr, 1: rho corr, 2: dB corr.
	electrons = s.electrons;

}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

ElectronSelection::~ElectronSelection()
{
	electrons.clear();
}

//______________________________________________________________________//

//______________________________________________________________________//

//__EXTRA METHODS_______________________________________________________//

//______________________________________________________________________//

//____SELECTION GETTERS_________________________________________________//

// ______________ELECTRONS____________________________________________//
bool ElectronSelection::foundZCandidate(TRootElectron* electron, std::vector<TRootElectron*>& vetoElectrons, float windowsize)
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

bool ElectronSelection::foundZCandidate(std::vector<TRootElectron*>& electrons1, std::vector<TRootElectron*>& electrons2, float windowsize)
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


// ______________ELECTRONS______________________________________________//

float ElectronSelection::GetElectronIsoCorrType(TRootElectron *el) const{
	double EffectiveArea = 0.;
	// Updated to 2015 EA from https://indico.cern.ch/event/370494/contribution/2/attachments/736984/1011061/Rami_update_on_CB_ELE_ID_PHYS14PU20bx25.pdf
	if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 0.8   ) EffectiveArea = 0.0973;
	if (fabs(el->superClusterEta()) >= 0.8   && fabs(el->superClusterEta()) < 1.3   ) EffectiveArea = 0.0954;
	if (fabs(el->superClusterEta()) >= 1.3   && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.0632;
	if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.0727;
	if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.5   ) EffectiveArea = 0.1337;
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

float ElectronSelection::pfElectronIso(TRootElectron *el) const{
	float isoCorr = (el->neutralHadronIso(3) + el->photonIso(3) - GetElectronIsoCorrType(el));
	float isolation = (el->chargedHadronIso(3) + (isoCorr > 0.0 ? isoCorr : 0.0))/(el->Pt());

	return isolation;

}

std::vector<TRootElectron*> ElectronSelection::GetSelectedElectrons() const {
	return GetSelectedElectrons(30,2.5,"Tight","Spring15_25ns",true);
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedElectrons(string WorkingPoint, string ProductionCampaign, bool CutsBased) const {
	return GetSelectedElectrons(30,2.5,WorkingPoint,ProductionCampaign,CutsBased);
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedElectrons(float PtThr, float etaThr, string WorkingPoint, string ProductionCampaign, bool CutsBased) const {
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
		else {
			string printboolval="Cutbased=true";
			if(!CutsBased)
				printboolval="Cutbased=false";

			throw std::invalid_argument( "received incorrect args to GetSelectedElectrons, requested: "+WorkingPoint+", "+ProductionCampaign+" "+printboolval);
		}
	}
	return ElectronCollection;
}



std::vector<TRootElectron*> ElectronSelection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr, float d0, float dz) const {

	// use medium electron ID (cut-based) for now, but with cuts on the beam spot d0 dz . This ID can be in flux, and for now is hard-coded here:

	//These quality cuts reflect the recommended Medium cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#PHYS14_selection_all_conditions

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
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;


	return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr) const {
	return GetSelectedDisplacedElectrons(PtThr,EtaThr,0.02,0.2);

}

std::vector<TRootElectron*> ElectronSelection::GetSelectedDisplacedElectrons() const{
	return GetSelectedDisplacedElectrons(30, 2.5);
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedTightElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 August 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based, BARREL:
		if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
			if( fabs(el->superClusterEta()) <= 1.479
			   && fabs(el->deltaEtaIn()) <  0.00864
			   && fabs(el->deltaPhiIn()) < 0.0286
			   && el->sigmaIEtaIEta_full5x5() < 0.0101
			   && el->hadronicOverEm() < 0.0342
			   && fabs(el->d0()) < 0.0103
			   && fabs(el->dz()) < 0.170
			   && fabs(1/el->E() - 1/el->P()) < 0.0116
			   && pfElectronIso(el) <  0.0591
			   && el->passConversion()
			   && el->missingHits() <= 2)
			{
				selectedElectrons.push_back(electrons[i]);
			}
			// using cut-based, ENDCAP:
			else if (fabs(el->superClusterEta()) < 2.5
					 && fabs(el->deltaEtaIn()) <  0.00762
					 && fabs(el->deltaPhiIn()) <  0.0439
					 && el->sigmaIEtaIEta_full5x5() <  0.0287
					 && (el->hadronicOverEm() < 0.0544)
					 && fabs(el->d0()) < 0.0377
					 && fabs(el->dz()) < 0.571
					 && fabs(1/el->E() - 1/el->P()) < 0.01
					 && pfElectronIso(el) < 0.0759
					 && el->passConversion()
					 && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
		}
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedMediumElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 August 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
		if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
			if( fabs(el->superClusterEta()) <= 1.479
			   && fabs(el->deltaEtaIn()) < 0.0094
			   && fabs(el->deltaPhiIn()) < 0.0296
			   && el->sigmaIEtaIEta_full5x5() < 0.0101
			   && el->hadronicOverEm() < 0.0372
			   && fabs(el->d0()) < 0.0151
			   && fabs(el->dz()) < 0.238
			   && fabs(1/el->E() - 1/el->P()) < 0.118
			   && pfElectronIso(el) < 0.0987
			   && el->passConversion()
			   && el->missingHits() <= 2)
			{
				selectedElectrons.push_back(electrons[i]);
			}

			else if (fabs(el->superClusterEta()) < 2.5
					 && fabs(el->deltaEtaIn()) <  0.00773
					 && fabs(el->deltaPhiIn()) < 0.148
					 && el->sigmaIEtaIEta_full5x5() < 0.0287
					 && (el->hadronicOverEm() < 0.0546)
					 && fabs(el->d0()) < 0.0535
					 && fabs(el->dz()) < 0.572
					 && fabs(1/el->E() - 1/el->P()) < 0.104
					 && pfElectronIso(el) < 0.0902
					 && el->passConversion()
					 && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
		}
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedLooseElectronsCutsBasedSpring15_50ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 August 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
		if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
			if( fabs(el->superClusterEta()) <= 1.479
			   && fabs(el->deltaEtaIn()) < 0.00976
			   && fabs(el->deltaPhiIn()) < 0.0929
			   && el->sigmaIEtaIEta_full5x5() <  0.0105
			   && el->hadronicOverEm() < 0.0765
			   && fabs(el->d0()) <  0.0227
			   && fabs(el->dz()) <  0.379
			   && fabs(1/el->E() - 1/el->P()) <  0.184
			   && pfElectronIso(el) <  0.118
			   && el->passConversion()
			   && el->missingHits() <= 2)
			{
				selectedElectrons.push_back(electrons[i]);
			}

			else if (fabs(el->superClusterEta()) < 2.5
					 && fabs(el->deltaEtaIn()) <  0.00952
					 && fabs(el->deltaPhiIn()) < 0.181
					 && el->sigmaIEtaIEta_full5x5() <  0.0318
					 && (el->hadronicOverEm() < 0.0824)
					 && fabs(el->d0()) <  0.242
					 && fabs(el->dz()) <  0.921
					 && fabs(1/el->E() - 1/el->P()) <  0.125
					 && pfElectronIso(el) <  0.118
					 && el->passConversion()
					 && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
		}
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedTightElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 August 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based, BARREL:
		if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
			if( fabs(el->superClusterEta()) <= 1.479
			   && fabs(el->deltaEtaIn()) <  0.00926
			   && fabs(el->deltaPhiIn()) < 0.0336
			   && el->sigmaIEtaIEta_full5x5() < 0.0101
			   && el->hadronicOverEm() < 0.0597
			   && fabs(el->d0()) < 0.0111
			   && fabs(el->dz()) < 0.0466
			   && fabs(1/el->E() - 1/el->P()) < 0.012
			   && pfElectronIso(el) <  0.0354
			   && el->passConversion()
			   && el->missingHits() <= 2)
			{
				selectedElectrons.push_back(electrons[i]);
			}
			// using cut-based, ENDCAP:
			else if (fabs(el->superClusterEta()) < 2.5
					 && fabs(el->deltaEtaIn()) <  0.00724
					 && fabs(el->deltaPhiIn()) <  0.0918
					 && el->sigmaIEtaIEta_full5x5() <  0.0279
					 && (el->hadronicOverEm() < 0.0615)
					 && fabs(el->d0()) < 0.0351
					 && fabs(el->dz()) < 0.417
					 && fabs(1/el->E() - 1/el->P()) < 0.0898
					 && pfElectronIso(el) < 0.0646
					 && el->passConversion()
					 && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
		}
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedMediumElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 August 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
		if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
			if( fabs(el->superClusterEta()) <= 1.479
			   && fabs(el->deltaEtaIn()) < 0.0103
			   && fabs(el->deltaPhiIn()) < 0.0336
			   && el->sigmaIEtaIEta_full5x5() < 0.0101
			   && el->hadronicOverEm() < 0.0876
			   && fabs(el->d0()) < 0.0118
			   && fabs(el->dz()) < 0.373
			   && fabs(1/el->E() - 1/el->P()) < 0.0174
			   && pfElectronIso(el) < 0.0766
			   && el->passConversion()
			   && el->missingHits() <= 2)
			{
				selectedElectrons.push_back(electrons[i]);
			}

			else if (fabs(el->superClusterEta()) < 2.5
					 && fabs(el->deltaEtaIn()) <  0.00733
					 && fabs(el->deltaPhiIn()) < 0.114
					 && el->sigmaIEtaIEta_full5x5() < 0.0283
					 && (el->hadronicOverEm() < 0.0678)
					 && fabs(el->d0()) < 0.0739
					 && fabs(el->dz()) < 0.602
					 && fabs(1/el->E() - 1/el->P()) < 0.0898
					 && pfElectronIso(el) < 0.0678
					 && el->passConversion()
					 && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
		}
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedLooseElectronsCutsBasedSpring15_25ns(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 August 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#Spring15_selection_50ns

	std::vector<TRootElectron*> selectedElectrons;
	for(unsigned int i=0; i<electrons.size(); i++) {
		TRootElectron* el = (TRootElectron*) electrons[i];
		// Using cut-based
		if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr) {
			if( fabs(el->superClusterEta()) <= 1.479
			   && fabs(el->deltaEtaIn()) < 0.0105
			   && fabs(el->deltaPhiIn()) < 0.115
			   && el->sigmaIEtaIEta_full5x5() <  0.0103
			   && el->hadronicOverEm() < 0.104
			   && fabs(el->d0()) <  0.0261
			   && fabs(el->dz()) <  0.41
			   && fabs(1/el->E() - 1/el->P()) <  0.102
			   && pfElectronIso(el) <  0.0893
			   && el->passConversion()
			   && el->missingHits() <= 2)
			{
				selectedElectrons.push_back(electrons[i]);
			}

			else if (fabs(el->superClusterEta()) < 2.5
					 && fabs(el->deltaEtaIn()) <  0.00814
					 && fabs(el->deltaPhiIn()) < 0.182
					 && el->sigmaIEtaIEta_full5x5() <  0.0301
					 && (el->hadronicOverEm() < 0.0897)
					 && fabs(el->d0()) <  0.118
					 && fabs(el->dz()) <  0.822
					 && fabs(1/el->E() - 1/el->P()) <  0.126
					 && pfElectronIso(el) <  0.121
					 && el->passConversion()
					 && el->missingHits() <= 1)
			{
				selectedElectrons.push_back(electrons[i]);
			}
		}
	}
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}



std::vector<TRootElectron*> ElectronSelection::GetSelectedTightElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#PHYS14_selection_all_conditions

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
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedMediumElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	//These quality cuts reflect the recommended Medium cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#PHYS14_selection_all_conditions

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
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedLooseElectronsCutsBasedPHYS14(float PtThr, float EtaThr) const {
	// (PLEASE UPDATE IF YOU CHANGE THIS CODE)
	// These quality cuts reflect the recommended Loose cut-based electron ID as provided by the EGM POG. Last updated: 25 July 2015
	// as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationElectron#PHYS14_selection_all_conditions
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
	std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
	return selectedElectrons;
}

//____________IS SELECTED_______________________________________________//
bool ElectronSelection::isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut) {
	if(vertex.size()==0) return false;
	float Rho = sqrt(vertex[0]->x()*vertex[0]->x()+vertex[0]->y()*vertex[0]->y());
	if(!vertex[0]->isFake() && vertex[0]->ndof()>NdofCut && abs(vertex[0]->z())<Zcut && Rho<RhoCut) return true;
	return false;
}

//______________________________________________________________________//

