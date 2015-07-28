//////////////////////////////////////////////
//          Make cutflow table              //
//////////////////////////////////////////////

#include "../interface/CutsTable.h"

CutsTable::CutsTable(bool isMuon, bool isElectron){
	if (isMuon){
		leptonChoice = "Muon";
	}
	else if (isElectron){
		leptonChoice = "Electron";
	}
}

CutsTable::~CutsTable(){

}

void CutsTable::AddSelections(){
	CutsselecTable.push_back(string("initial"));
	CutsselecTable.push_back(string("Event cleaning and Trigger"));
	if(leptonChoice == "Muon"){
	    CutsselecTable.push_back(string("Exactly 1 Tight Isolated Muon"));
	    CutsselecTable.push_back(string("Exactly 1 Loose Isolated Muon"));
	    CutsselecTable.push_back(string("Exactly zero electrons"));

	}
	else if(leptonChoice == "Electron"){
	    CutsselecTable.push_back(string("Exactly 1 Tight Electron"));
	    CutsselecTable.push_back(string("Exactly 1 Loose Electron"));
	    CutsselecTable.push_back(string("Exactly zero muons"));
	}
	CutsselecTable.push_back(string("At least 6 Jets"));
	CutsselecTable.push_back(string("At least 1 CSVM Jet"));
	CutsselecTable.push_back(string("At least 2 CSVM Jets"));
	//CutsselecTable->push_back(string("HT $\\geq 100 GeV$"));
	//CutsselecTable->push_back(string("HT $\\geq 200 GeV$"));
	//CutsselecTable->push_back(string("HT $\\geq 300 GeV$"));
	//CutsselecTable->push_back(string("HT $\\geq 400 GeV$"));
}

void CutsTable::AddSelectionsMuons(){
    CutsselecTable.push_back(string("initial"));
    CutsselecTable.push_back(string("Is global"));
    CutsselecTable.push_back(string("Is PFmuon"));    
    CutsselecTable.push_back(string("pt gt 26"));
    CutsselecTable.push_back(string("Eta lt 2.1"));
    CutsselecTable.push_back(string("chi2 lt 10"));
    CutsselecTable.push_back(string("nofTrackerLayersWithMeasurement 5"));
    CutsselecTable.push_back(string("d0 0.2 "));
    CutsselecTable.push_back(string("dz 0.5"));
    CutsselecTable.push_back(string("nofValidPixelHits 0 "));
    CutsselecTable.push_back(string("nmatchedstations 1"));
}

void CutsTable::CreateTable(vector < Dataset* > datasets, float Luminosity){

    selecTable = new SelectionTable(CutsselecTable, datasets);
    selecTable->SetLuminosity(Luminosity);
    selecTable->SetPrecision(1);

}

void CutsTable::FillTable(unsigned int d, bool isGoodPV, bool trigged, float scaleFactor, int nMu, int nLooseMu, int nEl, int nLooseEl, int nJets, int nLtags, int nMtags, int nTtags){

    selecTable->Fill(d,0,scaleFactor);

    if(leptonChoice == "Muon")   //Muon-Electron Selection Table
    {
        if(isGoodPV && trigged)
        {
            selecTable->Fill(d,1,scaleFactor);
            if (nMu==1)
            {
                selecTable->Fill(d,2,scaleFactor);
                if(nLooseMu==1)
                {
                    selecTable->Fill(d,3,scaleFactor);
                    if(nEl==0)
                    {
                        selecTable->Fill(d,4,scaleFactor);
                        if(nJets>=6)
                        {
                            selecTable->Fill(d,5,scaleFactor);
                            if(nMtags>=1)
                            {
                                selecTable->Fill(d,6,scaleFactor);
                                if(nMtags>=2)
                                {
                                    selecTable->Fill(d,7,scaleFactor);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    else if(leptonChoice ==  "Electron")   //Muon-Electron Selection Table
    {
        if(isGoodPV && trigged)
        {
            selecTable->Fill(d,1,scaleFactor);
            if (nEl==1)
            {
                selecTable->Fill(d,2,scaleFactor);
                if(nLooseEl==1)
                {
                    selecTable->Fill(d,3,scaleFactor);
                    if(nMu==0)
                    {
                        selecTable->Fill(d,4,scaleFactor);
                        if(nJets>=6)
                        {
                            selecTable->Fill(d,5,scaleFactor);
                            if(nMtags>=1)
                            {
                                selecTable->Fill(d,6,scaleFactor);
                                if(nMtags>=2)
                                {
                                    selecTable->Fill(d,7,scaleFactor);
                                }
                            }
                        }
                    }
                }
            }
        }
    }         
}


void CutsTable::FillTableMuons(unsigned int d, float scaleFactor, vector < TRootMuon* > init_muons){

    for (unsigned int i=0; i<init_muons.size(); i++)  //Muon-Electron Selection Table
    {
        selecTable->Fill(d,0,scaleFactor);

        if(init_muons[i]->isGlobalMuon())
        {
            selecTable->Fill(d,1,scaleFactor);
            if (init_muons[i]->isPFMuon())
            {
                selecTable->Fill(d,2,scaleFactor);
                if((init_muons[i]->Pt())>26)
                {
                    selecTable->Fill(d,3,scaleFactor);
                    if((init_muons[i]->Eta())<2.1)
                    {
                        selecTable->Fill(d,4,scaleFactor);
                        if((init_muons[i]->chi2())<10)
                        {
                            selecTable->Fill(d,5,scaleFactor);
                            if((init_muons[i]->nofTrackerLayersWithMeasurement())>5)
                            {
                                selecTable->Fill(d,6,scaleFactor);
                                if((init_muons[i]->d0())<0.2)
                                {
                                    selecTable->Fill(d,7,scaleFactor);
                                    if((init_muons[i]->dz())<0.5){

                                        selecTable->Fill(d,8,scaleFactor);
                                        if((init_muons[i]->nofValidPixelHits())>0){

                                            selecTable->Fill(d,9,scaleFactor);
                                            if((init_muons[i]->nofMatchedStations())>1){

                                                selecTable->Fill(d,10,scaleFactor);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void CutsTable::Calc_Write(string postfix, string dName, string channelpostfix){
    //(bool mergeTT, bool mergeQCD, bool mergeW, bool mergeZ, bool mergeST)
    selecTable->TableCalculator(  true, true, true, true, true);
    //Options : WithError (false), writeMerged (true), useBookTabs (false), addRawsyNumbers (false), addEfficiencies (false), addTotalEfficiencies (false), writeLandscape (false)
    selecTable->Write(  "FourTop"+postfix+dName+"_Table"+channelpostfix+".tex",    false,true,true,true,false,false,true);
    delete selecTable;
}




