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
                            if(nLtags>=1)
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
                            if(nLtags>=1)
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


void CutsTable::Calc_Write(string postfix, string dName, string channelpostfix){
    //(bool mergeTT, bool mergeQCD, bool mergeW, bool mergeZ, bool mergeST)
    selecTable->TableCalculator(  true, true, true, true, true);
    //Options : WithError (false), writeMerged (true), useBookTabs (false), addRawsyNumbers (false), addEfficiencies (false), addTotalEfficiencies (false), writeLandscape (false)
    selecTable->Write(  "FourTop"+postfix+dName+"_Table"+channelpostfix+".tex",    false,true,true,true,false,false,true);
    delete selecTable;
}




