#include "../interface/FatJetSelection.h"

//____CONSTRUCTORS______________________________________________________//

FatJetSelection::FatJetSelection()
{
}


FatJetSelection::FatJetSelection(const std::vector<TRootJet*>& fatjets_)
{
	for(unsigned int i=0; i<fatjets_.size(); i++) fatjets.push_back(fatjets_[i]);
}

FatJetSelection::FatJetSelection(const FatJetSelection& s)
{
	// copy the objects
	fatjets = s.fatjets;
}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

FatJetSelection::~FatJetSelection()
{
	fatjets.clear();
}

//____SELECTION GETTERS_________________________________________________//

// ______________JETS_________________________________________________//

// This should be the Standard getter function for Jets in Run-II.  The use of non PF objects should be discouraged.
std::vector<TRootSubstructureJet*> FatJetSelection::GetSelectedFatJets(float PtThr, float EtaThr, bool applyJetID) const
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
	std::sort(selectedJets.begin(),selectedJets.end(),HighestFatJetPt());
	return selectedJets;

}

std::vector<TRootSubstructureJet*> FatJetSelection::GetSelectedFatJets() const
{
	return GetSelectedFatJets(30,2.5,false);
}




