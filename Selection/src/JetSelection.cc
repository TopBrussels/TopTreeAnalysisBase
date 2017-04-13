#include "../interface/JetSelection.h"

//____CONSTRUCTORS______________________________________________________//

JetSelection::JetSelection()
{
}


JetSelection::JetSelection(const std::vector<TRootJet*>& jets_)
{
  for(unsigned int i=0; i<jets_.size(); i++) jets.push_back(jets_[i]);
}

JetSelection::JetSelection(const JetSelection& s)
{
  // copy the objects
  jets = s.jets;
}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

JetSelection::~JetSelection()
{
  jets.clear();
}

//____SELECTION GETTERS_________________________________________________//

// ______________JETS_________________________________________________//

// This should be the Standard getter function for Jets in Run-II.  The use of non PF objects should be discouraged.
std::vector<TRootPFJet*> JetSelection::GetSelectedJets(float PtThr, float EtaThr, bool applyJetID, std::string TightLoose) const
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
		  else if(TightLoose == "Tight" && passTightPFJetID13TeV(PFJet))//This is the 13TeV Recommended Tight PFJet ID.  https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
		    {
		      selectedJets.push_back(init_jet);
		    }
		}
	      else selectedJets.push_back(init_jet);
	    }
	}
    }
  std::sort(selectedJets.begin(),selectedJets.end(),HighestJetPt());
  return selectedJets;
}

std::vector<TRootPFJet*> JetSelection::GetSelectedJets() const
{
  return GetSelectedJets(30,2.5,true,"Loose");
}

std::vector<TRootPFJet*> JetSelection::GetSelectedBJets(const std::vector<TRootPFJet*>& seljets, int& btagAlgo, float& btagCut) const
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

//=======================
//Jet Helper Functions===
//=======================

bool JetSelection::passLoosePFJetID13TeV(const TRootPFJet* PFJet) const
{

  if(fabs(fabs(PFJet->Eta()) <= 2.4))
  {
      if(PFJet->chargedHadronEnergyFraction() > 0 && PFJet->chargedEmEnergyFraction() < 0.99 && PFJet->chargedMultiplicity() > 0)
      {
          if(PFJet->neutralHadronEnergyFraction() < 0.99 && PFJet->neutralEmEnergyFraction() < 0.99 && PFJet->nConstituents() > 1) return true;
      }
  }
  else if(fabs(PFJet->Eta()) > 2.4 && fabs(PFJet->Eta()) <= 2.7)
  {
      if(PFJet->neutralHadronEnergyFraction() < 0.99 && PFJet->neutralEmEnergyFraction() < 0.99 && PFJet->nConstituents() > 1) return true;
  }
  else if(fabs(PFJet->Eta()) > 2.7 && fabs(PFJet->Eta()) <= 3.0)
  {
      if(PFJet->neutralEmEnergyFraction() > 0.01 && PFJet->neutralHadronEnergyFraction() < 0.98 && PFJet->neutralMultiplicity() > 2) return true;
  }
  else
  {
      if(PFJet->neutralEmEnergyFraction() < 0.90 && PFJet->neutralMultiplicity() > 10) return true;
  }
  return false;
}

bool JetSelection::passTightPFJetID13TeV(const TRootPFJet* PFJet) const
{
  if(fabs(fabs(PFJet->Eta()) <= 2.4))
  {
      if(PFJet->chargedHadronEnergyFraction() > 0 && PFJet->chargedEmEnergyFraction() < 0.90 && PFJet->chargedMultiplicity() > 0)
      {
          if(PFJet->neutralHadronEnergyFraction() < 0.90 && PFJet->neutralEmEnergyFraction() < 0.90  && PFJet->nConstituents() > 1) return true;
      }
  }
  else if(fabs(PFJet->Eta()) > 2.4 && fabs(PFJet->Eta()) <= 2.7)
  {
      if(PFJet->neutralHadronEnergyFraction() < 0.90 && PFJet->neutralEmEnergyFraction() < 0.90 && PFJet->nConstituents() > 1) return true;
  }
  else if(fabs(PFJet->Eta()) > 2.7 && fabs(PFJet->Eta()) <= 3.0)
  {
      if(PFJet->neutralEmEnergyFraction() > 0.01 && PFJet->neutralHadronEnergyFraction() < 0.98 && PFJet->neutralMultiplicity() > 2) return true;
  }
  else
  {
      if(PFJet->neutralEmEnergyFraction() < 0.90 && PFJet->neutralMultiplicity() > 10) return true;
  }
  return false;
}

//______________________________________________________________________//

