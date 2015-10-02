// -*- C++ -*-
//
// Package:    TopTreeAnalysisBase/Tools
// Class:      BTagWeightTools
// 
/**\class BTagWeightTools TopBrussels/TopTreeAnalysisBase/Tools/interface/BTagWeightTools.cxx TopBrussels/Tools/interface/BTagWeightTools.h

 Description: Container class to parse BTV POG payloads which can be updated from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG

Unfortunately the BTV POG does not give the parameterizations in a parsable format that is consistent even per measurements, so you need to edit the files yourself to the level that they are usable. THe parsing code currently is tuned for the Moriond 2012 parameterizations from ttbar+mujet file


Fake rates: Now also using the Fake Rate parameterizations that are documented (for Moriond 2013) here: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_Moriond2013.C or (for EPS 2013) here: https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_EPS2013.C
As this is a bare Root Macro this has been c++ized into a separate set of functions.

 Implementation:
     Efficiencies: Loads a text file. Accessors return weights as a function of jet eta,pt for multiple options (central value, syst up, syst down, etc.)
     Fake rates: Contains hard-coded functions that are supplied in example code by BTV group
*/
//
// Original Author:  "fblekman"
//         Created:  Fri Feb  1 12:10:50 CET 2013
// $Id: BTagWeightTools.cc,v 1.1.2.7 2013/02/21 10:46:28 fblekman Exp $
//
//

#include "../interface/BTagWeightTools.h"


#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
using namespace TopTree;


// constructor
BTagWeightTools::BTagWeightTools(){
  _ptmax = -999;
  _ptmin = 9999;
  _etamax = 2.4;
  _reader = new BTagCalibrationReader();
  //InitializeMCEfficiencyHistos();
}

BTagWeightTools::BTagWeightTools(const BTagCalibrationReader *reader, float minpt, float maxpt, float maxeta){
  _ptmax = maxpt;
  _ptmin = minpt;
  _etamax = maxeta;
  _reader = reader;
  
  // Try and set ptmax and ptmin for the given selectedJets, to avoid ending up with empty pT bins
  /*for (size_t i = 0; i < selectedJets.size(); i++){
  	if (selectedJets[i]->Pt() > _ptmax){_ptmax = selectedJets[i]->Pt();}
	if (selectedJets[i]->Pt() < _ptmin){_ptmin = selectedJets[i]->Pt();}
  }
  
  // abs(eta) we fix at two bins from 0 to 2.4
  _etamax = 2.4;
  _reader = reader;
  _allSelectedJets = selectedJets;
  InitializeMCEfficiencyHistos(10,_ptmin,_ptmax,2);
  
  FillMCEfficiencyHistos();*/
}


// destructor:
BTagWeightTools::~BTagWeightTools(){

  //_histo2D.clear();
  //_allSelectedJets.clear();
  
}




void BTagWeightTools::InitializeMCEfficiencyHistos(map<string,TH2F*> &m,int NofPtBins,float PtMin,float PtMax,int NofEtaBins)
{
  m["BtaggedJets"] = new TH2F("BtaggedJets", "Total number of btagged jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  m["BtaggedBJets"] = new TH2F("BtaggedBJets", "Total number of btagged b-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  m["BtaggedCJets"] = new TH2F("BtaggedCJets", "Total number of btagged c-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  m["BtaggedLightJets"] = new TH2F("BtaggedLightJets", "Total number of btagged light jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  m["TotalNofBJets"] = new TH2F("TotalNofBJets", "Total number of b-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  m["TotalNofCJets"] = new TH2F("TotalNofCJets", "Total number of c-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  m["TotalNofLightJets"] = new TH2F("TotalNofLightJets", "Total number of light jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
}

void BTagWeightTools::FillMCEfficiencyHistos(vector< TopTree::TRootPFJet* > allSelectedJets)
{
	for (unsigned int i = 0; i < allSelectedJets.size(); i++){
  		if (allSelectedJets[i]->Pt() > _ptmax){_ptmax = allSelectedJets[i]->Pt();}
		if (allSelectedJets[i]->Pt() < _ptmin){_ptmin = allSelectedJets[i]->Pt();}
  	}
	
	map<string,TH2F*> _histo2D;
	InitializeMCEfficiencyHistos(_histo2D,10,_ptmin,_ptmax,2);
	
	
	TFile* f = (TFile*)TFile::Open("HistosPtEta.root","RECREATE");
  	if (!f){
		std::cerr << "ERROR in BTagWeigtTools::FillMCEfficiencyHistos: Could not open the file for the 2D histogram output." << std::endl;
		throw std::exception();
	}
  
	  for (unsigned int i=0; i < allSelectedJets.size(); i++)
	  {
					float localPt = allSelectedJets[i]->Pt();
					float localEta = fabs(allSelectedJets[i]->Eta());
					//cout << "pT: " << localPt << " eta: " << localEta << endl;
					if (localPt >= _ptmax) localPt = _ptmax-1;
					if (localEta >= 2.4) localEta = 2.4-0.01;
					
					if (fabs(allSelectedJets[i]->hadronFlavour()) == 5.) {	//b-jet
						_histo2D["TotalNofBJets"]->Fill(localPt,localEta);
					}
					else if (fabs(allSelectedJets[i]->hadronFlavour()) == 4.) {	//c-jet
						_histo2D["TotalNofCJets"]->Fill(localPt,localEta);
					}
					else if (fabs(allSelectedJets[i]->hadronFlavour()) == 0.) {
						//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << allSelectedJets[i]->hadronFlavour() << ". Is pileup jet -> consider as light jet." << endl;
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
					else {
						//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << allSelectedJets[i]->hadronFlavour() << ". Is hadron -> consider as light jet." << endl;
						_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
					}
					
					// Get the correct BJetTags value (discriminator value)
					float btagValue = -100.;
					std::string tagger_name = (_reader->calib)->tagger();
					if (tagger_name.find("CSVv2") != std::string::npos || tagger_name.find("csvv2") != std::string::npos){btagValue = allSelectedJets[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();}
  					else if (tagger_name.find("JP") != std::string::npos || tagger_name.find("jp") != std::string::npos){btagValue = allSelectedJets[i]->btag_jetProbabilityBJetTags();}
  					else if (tagger_name.find("SC") != std::string::npos || tagger_name.find("sc") != std::string::npos){
    						std::cerr << "Warning in BTagWeightTools: SC discriminator values not yet been included!!!" << std::endl
	      						  << "Uses CSVv2 instead, please choose another tagger (or update the script)." << std::endl;
    						btagValue = allSelectedJets[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
  					}
  					else{
  						std::cerr << "Warning in BTagCalibration: Tagger " << tagger_name << " is not CSVv2, JP, or SC." << std::endl
						<< "Please define your BTagCalibration object with one of these tagger names." << std::endl
						<< "CSVv2 discriminator is used instead!!!" << std::endl;
						btagValue = allSelectedJets[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
					}
				
					// fill the corresponding histograms for b-tagged jets
					if (btagValue > _reader->op_cutvalue()) {
						_histo2D["BtaggedJets"]->Fill(localPt,localEta);
						
						if (fabs(allSelectedJets[i]->hadronFlavour()) == 5.) {	//b-jet
							_histo2D["BtaggedBJets"]->Fill(localPt,localEta);
						}
						else if (fabs(allSelectedJets[i]->hadronFlavour()) == 4.) {	//c-jet
							_histo2D["BtaggedCJets"]->Fill(localPt,localEta);
						}
						else if (fabs(allSelectedJets[i]->hadronFlavour()) == 0.) {
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << allSelectedJets[i]->hadronFlavour() << ". Is pileup jet -> consider as light jet." << endl;
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
						else {
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << allSelectedJets[i]->hadronFlavour() << ". Is hadron -> consider as light jet." << endl;
							_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
						}
					}
	  }
	  _histo2D["BtaggedJets"]->Write();
	  _histo2D["BtaggedBJets"]->Write();
	  _histo2D["BtaggedCJets"]->Write();
	  _histo2D["BtaggedLightJets"]->Write();
	  _histo2D["TotalNofBJets"]->Write();
	  _histo2D["TotalNofCJets"]->Write();
	  _histo2D["TotalNofLightJets"]->Write();
	  f->Close();

} 



float BTagWeightTools::getMCEventWeight(vector< TopTree::TRootPFJet* > jetsPerEvent, TFile *histo_file) // https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
{
			if (!histo_file){
				std::cerr << "ERROR in BTagWeigtTools::getTagEff: Could not open the file for the 2D histogram input." << std::endl;
				throw std::exception();
			}
			
			float probMC = 1.;
			float probData = 1.;
			float tagEff = 1.;
			float btagSF = 1.;
			for (unsigned int i=0; i < jetsPerEvent.size(); i++) 
			{
						tagEff = getTagEff(jetsPerEvent[i]->Pt(), jetsPerEvent[i]->Eta(), jetsPerEvent[i]->hadronFlavour(),histo_file);											
						btagSF = _reader->eval(jetsPerEvent[i]);
						//cout << "TEST: BTag SF: " << btagSF << std::endl;
						//cout << "TEST: BTag Eff: " << tagEff << std::endl;
						//cout<<"  btagSF = "<<btagSF<<", tagEff = "<<tagEff<<", syst = "<<syst<<endl;	
														
						if (tagEff == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Tag efficiency is zero!" << endl;
								continue;
						}
						if (btagSF == 0.)
						{
								cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Btag scalefactor is zero!" << endl;
								continue;
						}	
						
						// Get BTag discriminator value													
						float btagValue = -100.;
						std::string tagger_name = (_reader->calib)->tagger();
						if (tagger_name.find("CSVv2") != std::string::npos || tagger_name.find("csvv2") != std::string::npos){btagValue = jetsPerEvent[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();}
  						else if (tagger_name.find("JP") != std::string::npos || tagger_name.find("jp") != std::string::npos){btagValue = jetsPerEvent[i]->btag_jetProbabilityBJetTags();}
  						else if (tagger_name.find("SC") != std::string::npos || tagger_name.find("sc") != std::string::npos){
    							std::cerr << "Warning in BTagWeightTools: SC discriminator values not yet been included!!!" << std::endl
	      						  	  << "Uses CSVv2 instead, please choose another tagger (or update the script)." << std::endl;
    							btagValue = jetsPerEvent[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
  						}
  						else{
  							std::cerr << "Warning in BTagCalibration: Tagger " << tagger_name << " is not CSVv2, JP, or SC." << std::endl
							<< "Please define your BTagCalibration object with one of these tagger names." << std::endl
							<< "CSVv2 discriminator is used instead!!!" << std::endl;
							btagValue = jetsPerEvent[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
						}							
						
						// Update the probabilities
						if (btagValue > _reader->op_cutvalue()) //tagged
						{	
						    		probMC = probMC*tagEff;
								probData = probData*btagSF*tagEff;
						}
						else
						{					//not tagged
								probMC = probMC*(1.-tagEff);
								probData = probData*(1.-btagSF*tagEff);
						}
			}
			float Weight = probData/probMC;
			return Weight;
}


float BTagWeightTools::getTagEff(float pt, float eta, int flavor, TFile* histo_file)
{
					if (!histo_file){
						std::cerr << "ERROR in BTagWeigtTools::getTagEff: Could not open the file for the 2D histogram input." << std::endl;
						throw std::exception();
					}
					
					
					float tagEff = 1.;
          				int xBin = 0, yBin = 0;
					if (pt >= _ptmax)
							xBin = ((TH2F*)histo_file->Get("TotalNofBJets"))->GetXaxis()->FindBin(_ptmax-1);
					else if (pt <= _ptmin)
							xBin = ((TH2F*)histo_file->Get("TotalNofBJets"))->GetXaxis()->FindBin(_ptmin+1);		
					else
							xBin = ((TH2F*)histo_file->Get("TotalNofBJets"))->GetXaxis()->FindBin(pt);
					if (fabs(eta) >= _etamax)
							yBin = ((TH2F*)histo_file->Get("TotalNofBJets"))->GetYaxis()->FindBin(_etamax - 0.01);
					else 
							yBin = ((TH2F*)histo_file->Get("TotalNofBJets"))->GetYaxis()->FindBin(fabs(eta));
											
					if (fabs(flavor) == 5.) 
					{
							float NofBJets = ((TH2F*)histo_file->Get("TotalNofBJets"))->GetBinContent(xBin,yBin);
							if (NofBJets == 0.)
							{
									cout << "BTagWeightTools::getMCEventWeight WARNING: No b jets for bin (" << xBin << "," << yBin << ")!" << endl;
									return -1;
							}
							float NofTaggedBJets = ((TH2F*)histo_file->Get("BtaggedBJets"))->GetBinContent(xBin,yBin);
							tagEff = NofTaggedBJets/NofBJets;
					}
					else if (fabs(flavor) == 4.)
					{
					    //cout<<" C"<<endl;
							float NofCJets = ((TH2F*)histo_file->Get("TotalNofCJets"))->GetBinContent(xBin,yBin);
							if (NofCJets == 0.)
							{
							    cout << "BTagWeightTools::getMCEventWeight WARNING: No c jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedCJets = ((TH2F*)histo_file->Get("BtaggedCJets"))->GetBinContent(xBin,yBin);
							tagEff = NofTaggedCJets/NofCJets;
					 }
					 else if (fabs(flavor) == 0.)
					 {
					    //cout<<" PU?"<<endl;
							//cout << "Eventnr.: " << ievt << ", Jetnr.: " << i << ", pdgId: " << flavor << ". Is pileup jet -> consider as light jet." << endl;
							float NofLightJets = ((TH2F*)histo_file->Get("TotalNofLightJets"))->GetBinContent(xBin,yBin);
							if (NofLightJets == 0.)
							{
									cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							}
							float NofTaggedLightJets = ((TH2F*)histo_file->Get("BtaggedLightJets"))->GetBinContent(xBin,yBin);
							tagEff = NofTaggedLightJets/NofLightJets;
					 }
					 else 
					 {
					     		cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Jet not identified! PdgId: " << flavor << endl
					     		     << "Using Light Jet Instead!!!!" << endl;
					     		 float NofLightJets = ((TH2F*)histo_file->Get("TotalNofLightJets"))->GetBinContent(xBin,yBin);
							 if (NofLightJets == 0.)
							 {
								  cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;
									return -1;
							 }
							 float NofTaggedLightJets = ((TH2F*)histo_file->Get("BtaggedLightJets"))->GetBinContent(xBin,yBin);
							 tagEff = NofTaggedLightJets/NofLightJets;
					}
					
						
					return tagEff;

}



