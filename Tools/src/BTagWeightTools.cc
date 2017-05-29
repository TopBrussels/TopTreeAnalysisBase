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
BTagWeightTools::BTagWeightTools(string histoFileName, bool verbose):
_ptmin(9999),
_ptmax(-999),
_etamax(2.4),
_reader(new BTagCalibrationReader()),
_histo2D(),
_verbose(verbose),
_histogramsFilled(false)

{
  _f = TFile::Open(histoFileName.c_str(),"READ");
  if (!_f){_f = TFile::Open(histoFileName.c_str(),"RECREATE");}
  else if (!(_f->GetListOfKeys()->FindObject("BtaggedJets"))){_f->ReOpen("UPDATE");}
  InitializeMCEfficiencyHistos();
}

BTagWeightTools::BTagWeightTools(const BTagCalibrationReader *reader, string histoFileName, bool verbose, float minpt, float maxpt, float maxeta):
_ptmin(9999),
_ptmax(-999),
_etamax(2.4),
_reader(new BTagCalibrationReader()),
_histo2D(),
_verbose(verbose),
_histogramsFilled(false)
{
  _ptmax = maxpt;
  _ptmin = minpt;
  _etamax = maxeta;
  _reader = reader;
  
  ifstream filetest(histoFileName.c_str());
  if(verbose && filetest.good()) cout << "BTag weights file exists" << endl; 
  else if (verbose) cout << "BTag weights file doesn't exits" << endl; 
  if (!filetest.good())
  {
    _f = TFile::Open(histoFileName.c_str(),"RECREATE");
    InitializeMCEfficiencyHistos(20,_ptmin,_ptmax,4);
  }
  else if (filetest.good()){_f = TFile::Open(histoFileName.c_str(),"READ");}
}


// destructor:
BTagWeightTools::~BTagWeightTools(){
    
  _f->cd();
  if (_histogramsFilled){
  TString opt = _f->GetOption();
  if (opt == "READ"){
  	_f->ReOpen("UPDATE");
	if (_verbose){cout << "BTagWeightTools::Destructor WARNING: Histograms are being added to already existing file (NOT A NEW FILE)" << endl;}
  }
  _histo2D["BtaggedJets"]->Write();
  _histo2D["BtaggedBJets"]->Write();
  _histo2D["BtaggedCJets"]->Write();
  _histo2D["BtaggedLightJets"]->Write();
  _histo2D["TotalNofBJets"]->Write();
  _histo2D["TotalNofCJets"]->Write();
  _histo2D["TotalNofLightJets"]->Write();
  }
  

  _f->Close();
  delete _f;
  //_histo2D.clear();
  //_allSelectedJets.clear();
  
}




void BTagWeightTools::InitializeMCEfficiencyHistos(int NofPtBins,float PtMin,float PtMax,int NofEtaBins)
{
  _histo2D["BtaggedJets"] = new TH2F("BtaggedJets", "Total number of btagged jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  _histo2D["BtaggedBJets"] = new TH2F("BtaggedBJets", "Total number of btagged b-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  _histo2D["BtaggedCJets"] = new TH2F("BtaggedCJets", "Total number of btagged c-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  _histo2D["BtaggedLightJets"] = new TH2F("BtaggedLightJets", "Total number of btagged light jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  _histo2D["TotalNofBJets"] = new TH2F("TotalNofBJets", "Total number of b-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  _histo2D["TotalNofCJets"] = new TH2F("TotalNofCJets", "Total number of c-jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
  _histo2D["TotalNofLightJets"] = new TH2F("TotalNofLightJets", "Total number of light jets", NofPtBins, PtMin, PtMax, NofEtaBins, 0, 2.4);
}

void BTagWeightTools::FillMCEfficiencyHistos(vector< TopTree::TRootPFJet* > allSelectedJets)
{

  	  if (!_f){
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
			_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
		}
		else {
			_histo2D["TotalNofLightJets"]->Fill(localPt,localEta);
		}
		
		// Get the correct BJetTags value (discriminator value)
		float btagValue = -100.;
		std::string tagger_name = (_reader->calib)->tagger();
		if (tagger_name.find("CSVv2") != std::string::npos || tagger_name.find("csvv2") != std::string::npos){btagValue = allSelectedJets[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();}
		else if (tagger_name.find("cMVAv2") != std::string::npos || tagger_name.find("cmvav2") != std::string::npos || tagger_name.find("CMVAv2") != std::string::npos){				
				btagValue = allSelectedJets[i]->btag_PFCombinedMVAV2BJetTags();
		}
		else{
			if (_verbose){std::cerr << "Warning in BTagCalibration: Tagger " << tagger_name << " is not CSVv2 or cMVAv2." << std::endl
						<< "Please define your BTagCalibration object with one of these tagger names." << std::endl
						<< "CSVv2 discriminator is used instead!!!" << std::endl;
			}
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
				_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
			}
			else {
				_histo2D["BtaggedLightJets"]->Fill(localPt,localEta);
			}
		}
	  }
	  
	  _histogramsFilled = true;
	

} 



float BTagWeightTools::getMCEventWeight(vector< TopTree::TRootPFJet* > jetsPerEvent,bool usePartonFlavour) // https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
{

			TString opt = _f->GetOption();
			if (opt == "RECREATE" || opt == "CREATE" || opt == "UPDATE"){ // the file was recreated so it is empty!
				std::cerr << "ERROR in BTagWeigtTools::getMCEventWight --> file does not contain correct histograms, check if the filename exists" << std::endl;
				throw std::exception();
			}

	
			float probMC = 1.;
			float probData = 1.;
			float tagEff = 1.;
			float btagSF = 1.;
			for (unsigned int i=0; i < jetsPerEvent.size(); i++) 
			{
						if (usePartonFlavour){
							int partonFlavour = std::abs(jetsPerEvent[i]->partonFlavour());
							if (partonFlavour!= 5 && partonFlavour!=4){partonFlavour = 0;} // to be consistent with the hadronFlavour numbers
							tagEff = getTagEff(jetsPerEvent[i]->Pt(), jetsPerEvent[i]->Eta(), partonFlavour);
  							BTagEntry::JetFlavor jf;
  							if (partonFlavour == 5){jf = BTagEntry::FLAV_B;}
  							else if (partonFlavour == 4){jf = BTagEntry::FLAV_C;}
  							else {jf = BTagEntry::FLAV_UDSG;}
							btagSF = _reader->eval(jf,jetsPerEvent[i]->Eta(),jetsPerEvent[i]->Pt(),jetsPerEvent[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags());
						}
						else{
							tagEff = getTagEff(jetsPerEvent[i]->Pt(), jetsPerEvent[i]->Eta(), jetsPerEvent[i]->hadronFlavour());											
							btagSF = _reader->eval(jetsPerEvent[i]);
						}
													
						if (tagEff == 0.)
						{
								if (_verbose){cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Tag efficiency is zero!" << endl;}
								continue;
						}
						if (btagSF == 0.)
						{
								if (_verbose){cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Btag scalefactor is zero!" << endl;}
								continue;
						}	
						
						// Get BTag discriminator value													
						float btagValue = -100.;
						std::string tagger_name = (_reader->calib)->tagger();
						if (tagger_name.find("CSVv2") != std::string::npos || tagger_name.find("csvv2") != std::string::npos){btagValue = jetsPerEvent[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();}
  						else if (tagger_name.find("cMVAv2") != std::string::npos || tagger_name.find("cmvav2") != std::string::npos || tagger_name.find("CMVAv2") != std::string::npos){				
							btagValue = jetsPerEvent[i]->btag_PFCombinedMVAV2BJetTags();
						}
  						else{
  							if (_verbose){std::cerr << "Warning in BTagCalibration: Tagger " << tagger_name << " is not CSVv2 or cMVAv2." << std::endl
										<< "Please define your BTagCalibration object with one of these tagger names." << std::endl
										<< "CSVv2 discriminator is used instead!!!" << std::endl;
							}
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




float BTagWeightTools::getTagEff(float pt, float eta, int flavor)
{
				
					float tagEff = 1.;
          				int xBin = 0, yBin = 0;
					if (pt >= _ptmax)
							xBin = ((TH2F*)_f->Get("TotalNofBJets"))->GetXaxis()->FindBin(_ptmax-1);
					else if (pt <= _ptmin)
							xBin = ((TH2F*)_f->Get("TotalNofBJets"))->GetXaxis()->FindBin(_ptmin+1);		
					else
							xBin = ((TH2F*)_f->Get("TotalNofBJets"))->GetXaxis()->FindBin(pt);
					if (fabs(eta) >= _etamax)
							yBin = ((TH2F*)_f->Get("TotalNofBJets"))->GetYaxis()->FindBin(_etamax - 0.01);
					else 
							yBin = ((TH2F*)_f->Get("TotalNofBJets"))->GetYaxis()->FindBin(fabs(eta));
					
					
					if (fabs(flavor) == 5.) 
					{
							float NofBJets = ((TH2F*)_f->Get("TotalNofBJets"))->GetBinContent(xBin,yBin);
							if (NofBJets == 0.)
							{
									if (_verbose){cout << "BTagWeightTools::getMCEventWeight WARNING: No b jets for bin (" << xBin << "," << yBin << ")!" << endl;}
									return -1;
							}
							float NofTaggedBJets = ((TH2F*)_f->Get("BtaggedBJets"))->GetBinContent(xBin,yBin);
							tagEff = NofTaggedBJets/NofBJets;
					}
					else if (fabs(flavor) == 4.)
					{
					    //cout<<" C"<<endl;
							float NofCJets = ((TH2F*)_f->Get("TotalNofCJets"))->GetBinContent(xBin,yBin);
							if (NofCJets == 0.)
							{
							    	if (_verbose){cout << "BTagWeightTools::getMCEventWeight WARNING: No c jets for bin (" << xBin << "," << yBin << ")." << endl;}
									return -1;
							}
							float NofTaggedCJets = ((TH2F*)_f->Get("BtaggedCJets"))->GetBinContent(xBin,yBin);
							tagEff = NofTaggedCJets/NofCJets;
					 }
					 else if (fabs(flavor) == 0.)
					 {
					        float NofLightJets = ((TH2F*)_f->Get("TotalNofLightJets"))->GetBinContent(xBin,yBin);
							if (NofLightJets == 0.)
							{
									if (_verbose){cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," << yBin << ")." << endl;}
									return -1;
							}
							float NofTaggedLightJets = ((TH2F*)_f->Get("BtaggedLightJets"))->GetBinContent(xBin,yBin);
							tagEff = NofTaggedLightJets/NofLightJets;
					 }
					 else 
					 {
					     		 if (_verbose){cout << endl << "BTagWeightTools::getMCEventWeight WARNING: Jet not identified! PdgId: " << flavor << endl
					     		     << "Using Light Jet Instead!!!!" << endl;}
					     		 float NofLightJets = ((TH2F*)_f->Get("TotalNofLightJets"))->GetBinContent(xBin,yBin);
							 if (NofLightJets == 0.)
							 {
								  if (_verbose){cout << "BTagWeightTools::getMCEventWeight WARNING: No light jets for bin (" << xBin << "," <<yBin << ")." << endl;}
									return -1;
							 }
							 float NofTaggedLightJets = ((TH2F*)_f->Get("BtaggedLightJets"))->GetBinContent(xBin,yBin);
							 tagEff = NofTaggedLightJets/NofLightJets;
					}
					
					return tagEff;

}



