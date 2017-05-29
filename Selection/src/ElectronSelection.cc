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

float ElectronSelection::GetElectronIsoCorrType(TRootElectron *el) const
{
  double EffectiveArea = 0.;
  
  // Updated to Spring 2016 EA from slide7: https://indico.cern.ch/event/482673/contributions/2187022/attachments/1282446/1905912/talk_electron_ID_spring16.pdf
  if (fabs(el->superClusterEta()) >= 0.0   && fabs(el->superClusterEta()) < 1.0   ) EffectiveArea = 0.1703;
  if (fabs(el->superClusterEta()) >= 1.0   && fabs(el->superClusterEta()) < 1.479 ) EffectiveArea = 0.1715;
  if (fabs(el->superClusterEta()) >= 1.479 && fabs(el->superClusterEta()) < 2.0   ) EffectiveArea = 0.1213;
  if (fabs(el->superClusterEta()) >= 2.0   && fabs(el->superClusterEta()) < 2.2   ) EffectiveArea = 0.1230;
  if (fabs(el->superClusterEta()) >= 2.2   && fabs(el->superClusterEta()) < 2.3   ) EffectiveArea = 0.1635;
  if (fabs(el->superClusterEta()) >= 2.3   && fabs(el->superClusterEta()) < 2.4   ) EffectiveArea = 0.1937;
  if (fabs(el->superClusterEta()) >= 2.4   && fabs(el->superClusterEta()) < 5.   ) EffectiveArea = 0.2393;
  if (fabs(el->superClusterEta()) >= 5.) EffectiveArea = -9999;
  
  double isocorr = 0;
  if(elecIsoCorrType_ == 1) // rho correction (default corr)
    isocorr = rho_*EffectiveArea;
  else if(elecIsoCorrType_ == 2) // dB correction
    isocorr = 0.5*el->puChargedHadronIso(3);
  else if (elecIsoCorrType_ == 0) // no correction
    isocorr = 0.;
  else
  {
    cerr << "Please, specify the correction type to be applied for the calculation of the electron relative isolation" << endl;
    cerr << " - Use setElectronIsoCorrType(int) method: " << endl;
    cerr << " -- 0: no correction, 1: rho correction (default), 2: dB correction" << endl;
    exit(1);
  }
  return isocorr;
}

float ElectronSelection::pfElectronIso(TRootElectron *el) const
{
  float isoCorr = (el->neutralHadronIso(3) + el->photonIso(3) - GetElectronIsoCorrType(el));
  float isolation = (el->chargedHadronIso(3) + (isoCorr > 0.0 ? isoCorr : 0.0))/(el->Pt());
  
  return isolation;
  
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedElectrons() const
{
  return GetSelectedElectrons(30,2.5,"Tight","Spring16_80X",true,true);
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedElectrons(string WorkingPoint, string ProductionCampaign, bool CutsBased) const
{
  return GetSelectedElectrons(30,2.5,WorkingPoint,ProductionCampaign,CutsBased,true);
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedElectrons(float PtThr, float etaThr, string WorkingPoint) const
{
  return GetSelectedElectrons(PtThr,etaThr,WorkingPoint,"Spring16_80X",true,true);
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedElectrons(float PtThr, float etaThr, string WorkingPoint, string ProductionCampaign, bool CutsBased, bool applyVID) const
{
 // cout << "Get selected electrons " << endl;
 // cout <<  "PtThr " << PtThr << " etaThr " << etaThr << " WorkingPoint " << WorkingPoint << " ProductionCampaign " << ProductionCampaign << " CutsBased " << CutsBased << " applyVID " << applyVID << endl;
  std::vector<TRootElectron* > ElectronCollection;
  if (CutsBased == true && applyVID ==  false)
  {
    if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Tight")
    {
      ElectronCollection = GetSelectedTightElectronsCutsBasedSpring16_80X(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Medium")
    {
      ElectronCollection = GetSelectedMediumElectronsCutsBasedSpring16_80X(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Loose")
    {
      ElectronCollection = GetSelectedLooseElectronsCutsBasedSpring16_80X(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Veto")
    {
      ElectronCollection = GetSelectedVetoElectronsCutsBasedSpring16_80X(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "FakeLoose")
    {
      ElectronCollection = GetSelectedFakeLooseElectronsCutsBasedSpring16_80X(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "FakeTight")
    {
      ElectronCollection = GetSelectedFakeTightElectronsCutsBasedSpring16_80X(PtThr, etaThr);
    }
    else
    {
      string printboolval="Cutbased=true, applyVID=false";
      
      throw std::invalid_argument( "received incorrect args to GetSelectedElectrons, requested: "+WorkingPoint+", "+ProductionCampaign+" "+printboolval);
    }
  }
  else if (CutsBased == true && applyVID ==  true)
  {
    if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Heep")
    {
      ElectronCollection = GetSelectedHeepElectronsCutsBasedSpring16_80X_VID(PtThr, etaThr);
    }
    if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Tight")
    {
      ElectronCollection = GetSelectedTightElectronsCutsBasedSpring16_80X_VID(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Medium")
    {
      ElectronCollection = GetSelectedMediumElectronsCutsBasedSpring16_80X_VID(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Loose")
    {
      ElectronCollection = GetSelectedLooseElectronsCutsBasedSpring16_80X_VID(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Veto")
    {
      ElectronCollection = GetSelectedVetoElectronsCutsBasedSpring16_80X_VID(PtThr, etaThr);
    }
    else
    {
      string printboolval="Cutbased=true, applyVID=true";
      
      throw std::invalid_argument( "received incorrect args to GetSelectedElectrons, requested: "+WorkingPoint+", "+ProductionCampaign+" "+printboolval);
    }
  }
  else if (CutsBased == false && applyVID ==  true)
  {
    if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Tight")
    {
      ElectronCollection = GetSelectedTightElectronsMVABasedSpring16_80X_VID(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Medium")
    {
      ElectronCollection = GetSelectedMediumElectronsMVABasedSpring16_80X_VID(PtThr, etaThr);
    }
    else if (ProductionCampaign == "Spring16_80X" && WorkingPoint == "Loose")
    {
      ElectronCollection = GetSelectedLooseElectronsMVABasedSpring16_80X_VID(PtThr, etaThr);
    }
    else
    {
      string printboolval="Cutbased=false, applyVID=true";
      
      throw std::invalid_argument( "received incorrect args to GetSelectedElectrons, requested: "+WorkingPoint+", "+ProductionCampaign+" "+printboolval);
    }
  }
  else
  {
    string printboolval="Cutbased=false, applyVID=false";
    
    throw std::invalid_argument( "received incorrect args to GetSelectedElectrons, requested: "+WorkingPoint+", "+ProductionCampaign+" "+printboolval);
  }
  return ElectronCollection;
}

//VID electrons
std::vector<TRootElectron*> ElectronSelection::GetSelectedHeepElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->isCB_HeepID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr) selectedElectrons.push_back(electrons[i]);
  }
  
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedTightElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
 // cout << "GetSelectedTightElectronsCutsBasedSpring16_80X_VID " << PtThr << " " << EtaThr << endl;
 // cout << "initial collection size " << electrons.size() << endl;
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
   // cout << "electron " << i << " isCB_TightID() " << el->isCB_TightID() << " pt " << el->Pt() << " eta " << el->Eta() << endl;
    
    if(el->isCB_TightID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr){
      selectedElectrons.push_back(electrons[i]);
      //cout << "pushing back " << endl;
    }
    
  }
  
  //cout << "selected collection size " << selectedElectrons.size() << endl;
  return selectedElectrons;
}
std::vector<TRootElectron*> ElectronSelection::GetSelectedMediumElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->isCB_MediumID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr) selectedElectrons.push_back(electrons[i]);
  }
  
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedLooseElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->isCB_LooseID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr) selectedElectrons.push_back(electrons[i]);
  }
  
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedVetoElectronsCutsBasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->isCB_VetoID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr) selectedElectrons.push_back(electrons[i]);
  }
  
  return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedTightElectronsMVABasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->isMVA_TightID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr) selectedElectrons.push_back(electrons[i]);
  }
  
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedMediumElectronsMVABasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->isMVA_MediumID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr) selectedElectrons.push_back(electrons[i]);
  }
  
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedLooseElectronsMVABasedSpring16_80X_VID(float PtThr, float EtaThr) const
{
  std::vector<TRootElectron*> selectedElectrons;
  
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->isMVA_LooseID() && el->Pt() > PtThr && fabs(el->Eta()) < EtaThr) selectedElectrons.push_back(electrons[i]);
  }
  
  return selectedElectrons;
}





// displaced electrons
std::vector<TRootElectron*> ElectronSelection::GetSelectedDisplacedElectrons(float PtThr, float EtaThr, float relIsoB, float relIsoEC, bool applyIso, bool applyId) const
{
  // use tight electron ID (cut-based) for now, but without cuts on  d0 dz . This ID can be in flux, and for now is hard-coded here:
  //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 2 december 2015
  // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Spring15_selection_25ns (revision 27)
  std::vector<TRootElectron*> selectedElectrons;
  bool saveit=false;
  
  for(unsigned int i=0; i<electrons.size(); i++) {
    saveit=false;
    TRootElectron* el = (TRootElectron*) electrons[i];
    
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr && !el->isEBEEGap()) {
      // no id no iso
      if (!applyIso && !applyId) {
        //      cout << "no id and no iso" << endl;
        saveit = true;
      }
      // apply iso only
      if(applyIso && isolationDisplacedElectron(el, relIsoB, relIsoEC) && !applyId){
        //      cout << "iso cut required and passed" <<endl;
        saveit=true;
      }
      // apply id only
      if( !applyIso  && applyId  && identificationDisplacedElectron(el)){
        //      cout << "id cut required and passed" <<endl
        saveit=true;
      }
      // apply both
      if( applyIso && isolationDisplacedElectron(el, relIsoB, relIsoEC) && applyId && identificationDisplacedElectron(el)){
        //      cout << "id and iso cut required and passed" <<endl
        saveit=true;
      }
      if(saveit) selectedElectrons.push_back(electrons[i]);
    }
  }
  
  //  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedDisplacedElectrons() const{
  return GetSelectedDisplacedElectrons(42.0, 2.4, 0.0354, 0.0646, true, true);
}



std::vector<TRootElectron*> ElectronSelection::GetSelectedTightElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const
{
  // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
  //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 26 October 2016
  // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for (revision 37)
  
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    //cout << "electron " << i << " fabs(el->deltaEtaIn()) " << fabs(el->deltaEtaIn()) << "< 0.00308 "<<    " fabs(el->deltaPhiIn()) " << fabs(el->deltaPhiIn()) <<  "< 0.0816"    << " el->sigmaIEtaIEta_full5x5()  " << el->sigmaIEtaIEta_full5x5() << "<0.00998 "    << " el->hadronicOverEm() " << el->hadronicOverEm() << "<0.0414 "    << " el->ioEmIoP() " << el->ioEmIoP() << "<0.0129 "    << " pfElectronIso(el) " <<  pfElectronIso(el) << "<0.0588 "    << " el->missingHits() " << el->missingHits() << "<=1 "    << " el->passConversion() "  << endl;
   
    // Using cut-based, BARREL:
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
    {
      if( fabs(el->superClusterEta()) <= 1.479)
      {
        if( fabs(el->deltaEtaIn()) <  0.00308
           && fabs(el->deltaPhiIn()) < 0.0816
           && el->sigmaIEtaIEta_full5x5() < 0.00998
           && el->hadronicOverEm() < 0.0414
           && el->ioEmIoP() < 0.0129
           && pfElectronIso(el) <  0.0588
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
      // using cut-based, ENDCAP:
      else
      {
        if( fabs(el->deltaEtaIn()) <  0.00605
           && fabs(el->deltaPhiIn()) <  0.0394
           && el->sigmaIEtaIEta_full5x5() <  0.0292
           && (el->hadronicOverEm() < 0.0641)
           && el->ioEmIoP() < 0.0129
           && pfElectronIso(el) < 0.0571
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
  return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedMediumElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const
{
  // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
  //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 7 October 2016
  // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for (revision 37)
  
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    // Using cut-based, BARREL:
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
    {
      if( fabs(el->superClusterEta()) <= 1.479)
      {
        if( fabs(el->deltaEtaIn()) <  0.00311
           && fabs(el->deltaPhiIn()) < 0.103
           && el->sigmaIEtaIEta_full5x5() < 0.00998
           && el->hadronicOverEm() < 0.253
           && el->ioEmIoP() < 0.134
           && pfElectronIso(el) <  0.0695
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
      // using cut-based, ENDCAP:
      else
      {
        if( fabs(el->deltaEtaIn()) <  0.00609
           && fabs(el->deltaPhiIn()) <  0.045
           && el->sigmaIEtaIEta_full5x5() <  0.0298
           && (el->hadronicOverEm() < 0.0878)
           && el->ioEmIoP() < 0.13
           && pfElectronIso(el) < 0.0821
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
  return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedLooseElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const
{
  // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
  //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 7 October 2016
  // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for (revision 37)
  
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    // Using cut-based, BARREL:
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
    {
      if( fabs(el->superClusterEta()) <= 1.479)
      {
        if( fabs(el->deltaEtaIn()) <  0.00477
           && fabs(el->deltaPhiIn()) < 0.222
           && el->sigmaIEtaIEta_full5x5() < 0.011
           && el->hadronicOverEm() < 0.298
           && el->ioEmIoP() < 0.241
           && pfElectronIso(el) <  0.0994
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
      // using cut-based, ENDCAP:
      else
      {
        if( fabs(el->deltaEtaIn()) <  0.00868
           && fabs(el->deltaPhiIn()) <  0.213
           && el->sigmaIEtaIEta_full5x5() <  0.0314
           && (el->hadronicOverEm() < 0.101)
           && el->ioEmIoP() < 0.14
           && pfElectronIso(el) < 0.107
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedFakeLooseElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const
{
  // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
  //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 7 October 2016
  // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for (revision 37)
  
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    // Using cut-based, BARREL:
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
    {
      if( fabs(el->superClusterEta()) <= 1.479)
      {
        if( fabs(el->deltaEtaIn()) <  0.00477
           && fabs(el->deltaPhiIn()) < 0.222
           && el->sigmaIEtaIEta_full5x5() < 0.011
           && el->hadronicOverEm() < 0.298
           && el->ioEmIoP() < 0.0129 // like tight id
           && pfElectronIso(el) >=  0.0994 // invert iso
           && pfElectronIso(el) < 1 // extra cut on iso
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
      // using cut-based, ENDCAP:
      else
      {
        if( fabs(el->deltaEtaIn()) <  0.00868
           && fabs(el->deltaPhiIn()) <  0.213
           && el->sigmaIEtaIEta_full5x5() <  0.0314
           && (el->hadronicOverEm() < 0.101)
           && el->ioEmIoP() < 0.0129 // like tight
           && pfElectronIso(el) >= 0.107 // invert iso
           && pfElectronIso(el) < 1
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
  return selectedElectrons;
}

std::vector<TRootElectron*> ElectronSelection::GetSelectedFakeTightElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const
{
  // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
  //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 7 October 2016
  // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for (revision 37)
  
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    // Using cut-based, BARREL:
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
    {
      if( fabs(el->superClusterEta()) <= 1.479)
      {
        if( fabs(el->deltaEtaIn()) <  0.00477
           && fabs(el->deltaPhiIn()) < 0.222
           && el->sigmaIEtaIEta_full5x5() < 0.011
           && el->hadronicOverEm() < 0.298
           && el->ioEmIoP() < 0.0129 // like tight id
           && pfElectronIso(el) >=  0.0588 // invert iso
           && pfElectronIso(el) < 1 // extra cut on iso
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
      // using cut-based, ENDCAP:
      else
      {
        if( fabs(el->deltaEtaIn()) <  0.00868
           && fabs(el->deltaPhiIn()) <  0.213
           && el->sigmaIEtaIEta_full5x5() <  0.0314
           && (el->hadronicOverEm() < 0.101)
           && el->ioEmIoP() < 0.0129 // like tight
           && pfElectronIso(el) >= 0.0571// invert iso
           && pfElectronIso(el) < 1
           && el->missingHits() <= 1
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
  return selectedElectrons;
}


std::vector<TRootElectron*> ElectronSelection::GetSelectedVetoElectronsCutsBasedSpring16_80X(float PtThr, float EtaThr) const
{
  // (PLEASE UPDATE IF YOU CHANGE THIS CODE)
  //These quality cuts reflect the recommended Tight cut-based electron ID as provided by the EGM POG. Last updated: 7 October 2016
  // as these are still in flux, it is probably useful to check them here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2#Working_points_for_2016_data_for (revision 37)
  
  std::vector<TRootElectron*> selectedElectrons;
  for(unsigned int i=0; i<electrons.size(); i++)
  {
    TRootElectron* el = (TRootElectron*) electrons[i];
    // Using cut-based, BARREL:
    if(el->Pt() > PtThr && fabs(el->Eta())< EtaThr)
    {
      if( fabs(el->superClusterEta()) <= 1.479)
      {
        if( fabs(el->deltaEtaIn()) <  0.00749
           && fabs(el->deltaPhiIn()) < 0.228
           && el->sigmaIEtaIEta_full5x5() < 0.0115
           && el->hadronicOverEm() < 0.356
           && el->ioEmIoP() < 0.299
           && pfElectronIso(el) <  0.175
           && el->missingHits() <= 2
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
      // using cut-based, ENDCAP:
      else
      {
        if( fabs(el->deltaEtaIn()) <  0.00895
           && fabs(el->deltaPhiIn()) <  0.213
           && el->sigmaIEtaIEta_full5x5() <  0.037
           && (el->hadronicOverEm() < 0.211)
           && el->ioEmIoP() < 0.15
           && pfElectronIso(el) < 0.159
           && el->missingHits() <= 3
           && el->passConversion())
        {
          selectedElectrons.push_back(electrons[i]);
        }
      }
    }
  }
  std::sort(selectedElectrons.begin(),selectedElectrons.end(),HighestElectronPt());
  return selectedElectrons;
}


//____________IS SELECTED_______________________________________________//
bool ElectronSelection::isPVSelected(const std::vector<TRootVertex*>& vertex, int NdofCut, float Zcut, float RhoCut)
{
  if(vertex.size()==0) return false;
  float Rho = sqrt(vertex[0]->x()*vertex[0]->x()+vertex[0]->y()*vertex[0]->y());
  if(!vertex[0]->isFake() && vertex[0]->ndof()>NdofCut && abs(vertex[0]->z())<Zcut && Rho<RhoCut) return true;
  return false;
}

//______________________________________________________________________//

//---- selection functions for displaced electrons and muons. factorising ID and isolation.
bool ElectronSelection::isolationDisplacedElectron(TRootElectron* el, float relIsoB, float relIsoEC) const{
  if( fabs(el->superClusterEta()) <= 1.479){
    //    if(pfElectronIso(el) < 0.0354)
    if(pfElectronIso(el) < relIsoB) // using the 25ns Effective Area
      return true;
    else
      return false;
  }
  // For the endcap
  else if (fabs(el->superClusterEta()) < 2.5){
    //    if(pfElectronIso(el) < 0.0646)
    if(pfElectronIso(el) < relIsoEC) // using the 25ns Effective Area
      return true;
    else
      return false;
  }
  return false;
}
bool ElectronSelection::identificationDisplacedElectron(const TRootElectron* el) const{
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
      //      cout << "the displaced Id electron is true" << endl;
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
