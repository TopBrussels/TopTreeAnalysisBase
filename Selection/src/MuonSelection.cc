#include "../interface/MuonSelection.h"

//____CONSTRUCTORS______________________________________________________//

MuonSelection::MuonSelection()
{
}


MuonSelection::MuonSelection(const std::vector<TRootMuon*>& muons_)
{
  for(unsigned int i=0; i<muons_.size(); i++) muons.push_back(muons_[i]);
}

MuonSelection::MuonSelection(const MuonSelection& s)
{
  // copy the objects
  muons = s.muons;
}

//______________________________________________________________________//

//____DESTRUCTOR________________________________________________________//

MuonSelection::~MuonSelection()
{
  muons.clear();
}


//____SELECTION GETTERS_________________________________________________//

// ______________MUONS________________________________________________//


std::vector<TRootMuon*> MuonSelection::GetSelectedMuons() const
{
  //Default Muon selection method.  This method should be updated to the most recent set of recommended cuts regularly.  Currently it points to the MUON POG Medium working point with Pt, Eta, and dBeta RelIso thresholds set as in the TOP PAG selection TWiki for single lepton event ID.
  return GetSelectedMuons(26,2.1,0.15, "Tight", "Summer16");
}

std::vector<TRootMuon*> MuonSelection::GetSelectedMuons(float PtThr, float etaThr, float relIso, string WorkingPoint, string ProductionCampaign) const
{
  std::vector<TRootMuon* > MuonCollection;
  
	if (ProductionCampaign.std::string::find("Summer16") != std::string::npos && WorkingPoint == "Tight"){
    MuonCollection = GetSelectedTightMuons2016(PtThr, etaThr, relIso);
  }
  else if (ProductionCampaign.std::string::find("Summer16") != std::string::npos && WorkingPoint == "Medium"){
    MuonCollection = GetSelectedMediumMuons2016(PtThr, etaThr, relIso, ProductionCampaign);  // production campaign should be Summer16DataBCDEF or Summer16DataGH
  }
  else if (ProductionCampaign.std::string::find("Summer16") != std::string::npos && WorkingPoint == "Loose"){
    MuonCollection = GetSelectedLooseMuons2016(PtThr, etaThr, relIso);
  }
  else if (ProductionCampaign.std::string::find("Summer16") != std::string::npos && WorkingPoint == "Fake"){
    MuonCollection = GetSelectedFakeMuons2016(PtThr, etaThr, relIso);
  }
  else {
    throw std::invalid_argument( "received incorrect args to GetSelectedMuons, requested: "+WorkingPoint+", "+ProductionCampaign);
  }
  
  return MuonCollection;
}

// displaced muons
std::vector<TRootMuon*> MuonSelection::GetSelectedDisplacedMuons (float PtThr, float EtaThr, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations, float RelIso, bool applyIso, bool applyId) const
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
        //      cout << "no id and no iso" << endl;
        saveit = true;
      }
      // apply iso only
      if(applyIso && isolationDisplacedMuon(muon,RelIso) && !applyId){
        //      cout << "iso cut required and passed" <<endl;
        saveit=true;
      }
      // apply id only
      if( !applyIso  && applyId  && identificationDisplacedMuon(muon, NormChi2,  NTrackerLayersWithMeas, NValidMuonHits, NValidPixelHits, NMatchedStations)){
        //      cout << "id cut required and passed" <<endl;
        saveit=true;
      }
      // apply both
      if( applyIso && isolationDisplacedMuon(muon,RelIso) && applyId  &&  identificationDisplacedMuon(muon, NormChi2,  NTrackerLayersWithMeas, NValidMuonHits, NValidPixelHits, NMatchedStations)){
        //      cout << "id and iso cut required and passed" <<endl;
        saveit=true;
      }
      if (saveit) selectedMuons.push_back(muons[i]);
    }
    
  }
  
  //  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestPt());
  return selectedMuons;
}

std::vector<TRootMuon*> MuonSelection::GetSelectedDisplacedMuons(float PtThr,float EtaThr,float MuonRelIso, bool applyIso, bool applyId) const
{
  return GetSelectedDisplacedMuons(PtThr,EtaThr,10.,5.,0,0,1,MuonRelIso,applyIso,applyId);
}

std::vector<TRootMuon*> MuonSelection::GetSelectedDisplacedMuons() const
{
  return GetSelectedDisplacedMuons(35.,2.4,0.15, true, true);
}


std::vector<TRootMuon*> MuonSelection::GetSelectedFakeMuons2016(float PtThr, float EtaThr,float MuonRelIso) const
{
  // These quality cuts reflect the LooseMuon ID as provided by the MUON POG, but the isolation is reversed.  PT, Eta, and Iso thresholds are not tuned as of April 13, 2017.
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0; i<muons.size(); i++)
  {
    
    //float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
    // use cone 4 iso for muons:
    float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected
    if(  (muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && muons[i]->isPFMuon()
       && muons[i]->Pt()>PtThr
       && fabs(muons[i]->Eta())<EtaThr
       && reliso >= MuonRelIso  ) // reverse isolation to get fake
    {
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestMuonPt());
  return selectedMuons;
}




std::vector<TRootMuon*> MuonSelection::GetSelectedLooseMuons2016(float PtThr, float EtaThr,float MuonRelIso) const
{
  // These quality cuts reflect the LooseMuon ID as provided by the MUON POG.  PT, Eta, and Iso thresholds are not tuned as of April 13, 2017.
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0; i<muons.size(); i++)
  {
    //float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
    // use cone 4 iso for muons:
    float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected
    if(  (muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && muons[i]->isPFMuon()
       && muons[i]->Pt()>PtThr
       && fabs(muons[i]->Eta())<EtaThr
       && reliso < MuonRelIso  )  // Should be 0.25 for loose ID
    {
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestMuonPt());
  return selectedMuons;
}

std::vector<TRootMuon*> MuonSelection::GetSelectedMediumMuons2016(float PtThr, float EtaThr,float MuonRelIso, string ProductionCampaign) const
{
  // These quality cuts reflect the Medium Muon ID as provided by the MUON POG.  PT, Eta, and Iso thresholds are not tuned as of April 13, 2017.
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
    float validFrac = 0.8;
    if( ProductionCampaign.std::string::find("BCDEF") != std::string::npos ) validFrac = 0.49;
    
    if(  (muons[i]->isGlobalMuon() || muons[i]->isTrackerMuon()) && muons[i]->isPFMuon()
       && muons[i]->Pt()>PtThr
       && fabs(muons[i]->Eta())<EtaThr
       && reliso < MuonRelIso
       && muons[i]->validFraction() > validFrac
       && muons[i]->segmentCompatibility() > (goodGlob ? 0.303 : 0.451)  )
    {
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestMuonPt());
  return selectedMuons;
}

std::vector<TRootMuon*> MuonSelection::GetSelectedTightMuons2016(float PtThr, float EtaThr,float MuonRelIso) const
{
		// These quality cuts reflect the Tight Muon ID as provided by the MUON POG.  PT, Eta, and Iso thresholds are not tuned as of April 13, 2017.
  std::vector<TRootMuon*> selectedMuons;
  for(unsigned int i=0; i<muons.size(); i++)
  {
    //float reliso = (muons[i]->chargedHadronIso()+muons[i]->neutralHadronIso()+muons[i]->photonIso())/muons[i]->Pt();
    // use cone 4 iso for muons:
    float reliso = (muons[i]->chargedHadronIso(4) + max( 0.0, muons[i]->neutralHadronIso(4) + muons[i]->photonIso(4) - 0.5*muons[i]->puChargedHadronIso(4) ) ) / muons[i]->Pt(); // dBeta corrected
    if(   muons[i]->isGlobalMuon() && muons[i]->isPFMuon()
       && muons[i]->Pt()>PtThr
       && fabs(muons[i]->Eta())<EtaThr
       && muons[i]->chi2() < 10.
       && muons[i]->nofTrackerLayersWithMeasurement() > 5
       && muons[i]->nofValidMuHits() > 0
       && fabs(muons[i]->d0()) < 0.2
       && fabs(muons[i]->dz()) < 0.5
       && muons[i]->nofValidPixelHits() > 0
       && muons[i]->nofMatchedStations()> 1
       && reliso < MuonRelIso  )  // Should be 0.15 for tight ID
    {
      selectedMuons.push_back(muons[i]);
    }
  }
  std::sort(selectedMuons.begin(),selectedMuons.end(),HighestMuonPt());
  return selectedMuons;
}


bool MuonSelection::foundZCandidate(TRootMuon* muon, std::vector<TRootMuon*>& vetoMuons, float windowsize)
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

bool MuonSelection::foundZCandidate(std::vector<TRootMuon*>& muons1, std::vector<TRootMuon*>& muons2, float windowsize)
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


bool MuonSelection::identificationDisplacedMuon(const TRootMuon* muon, float NormChi2, int NTrackerLayersWithMeas, int NValidMuonHits, int NValidPixelHits, int NMatchedStations) const
{
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
bool MuonSelection::isolationDisplacedMuon(const TRootMuon* muon, float RelIso) const
{
  if( muon->Pt()<0.0001) // protecting against the strange case that the pT is zero... just to be sure
    return false;
  float reliso = (muon->chargedHadronIso(4) + max( 0.0, muon->neutralHadronIso(4) + muon->photonIso(4) - 0.5*muon->puChargedHadronIso(4) ) ) / muon->Pt(); // dBeta corrected                                                    
  
  if(reliso< RelIso)
    return true;
  return false;
}
