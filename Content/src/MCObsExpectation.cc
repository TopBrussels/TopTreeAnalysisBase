#include "../interface/MCObsExpectation.h"

ClassImp(MCObsExpectation);

MCObsExpectation::MCObsExpectation(){
  Luminosity = 1.;
  Name = string("");
  hstSMProcess = 0;
  hstAll = 0;
  
  hSMProcess = 0;
  hAll = 0;
  
  hSMSubProcesses = 0;
  hNewPhysicsSignal = 0;
  legSMProcess = 0;
  legAll = 0;
}

MCObsExpectation::MCObsExpectation(vector<Dataset*> datasets, TAxis* axis, string VarLabel, float lumi){
	 //SetDatasets(datasets);
   for(unsigned int d=0;d<datasets.size();d++){
      //datasets_.push_back(datasets[d]);
      string dataSetName, dataSetTitle;
      dataSetName = datasets[d]->Name();
      dataSetTitle = datasets[d]->Title();
      datasetNames.push_back(dataSetName);
      datasetTitles.push_back(dataSetTitle);
   }
	 Luminosity = lumi;
 	 Name = VarLabel;
	 legSMProcess = new TLegend(0.6,0.7,0.9,0.9); legSMProcess->SetName("legSMProcess");
	 legAll = new TLegend(0.6,0.7,0.9,0.9); legAll->SetName("legAll");
	 hstSMProcess = new THStack(TString("SMProcess_")+VarLabel, TString("SMProcess"));
	 hstAll = new THStack(TString("All_")+VarLabel,TString("AllProcess"));
	 
	 hSMProcess = new TH1F(TString("SMProcess_")+VarLabel, TString("SMProcess"),axis->GetNbins(), axis->GetXbins()->fArray);	 	 
	hSMProcess->Sumw2(); //(z) added, should be done for all histos??	 
	 hAll = new TH1F(TString("All_")+VarLabel,TString("AllProcess"),axis->GetNbins(), axis->GetXbins()->fArray);	 
	hAll->Sumw2(); //(z) added, should be done for all histos??
	 
	 hSMSubProcesses = new TObjArray();
	 hNewPhysicsSignal = 0;
	 for(unsigned int d=0;d<datasetNames.size();d++){
		if(datasetNames[d].find("SUSY")<=datasetNames[d].size() || datasetNames[d].find("LM")<=datasetNames[d].size() || datasetNames[d].find("Zp")<=datasetNames[d].size() || datasetNames[d].find("NP")<=datasetNames[d].size() || datasetNames[d].find("prime")<=datasetNames[d].size()){
		   TH1F* hdummy = new TH1F(TString(datasetNames[d])+"_"+VarLabel,TString(datasetNames[d]),axis->GetNbins(), axis->GetXbins()->fArray);		   
		   hNewPhysicsSignal = hdummy;                  
		   continue;
		}	
		TH1F* hSMSubProcess = new TH1F(TString(datasetNames[d])+"_"+VarLabel,TString(datasetNames[d]),axis->GetNbins(), axis->GetXbins()->fArray);
		hSMSubProcesses->Add(hSMSubProcess);	
	 }
	 
}

MCObsExpectation::MCObsExpectation(vector<Dataset*> datasets,int nbins, float min, float max, string VarLabel, float lumi){	
   for(unsigned int d=0;d<datasets.size();d++){
      //datasets_.push_back(datasets[d]);
      string dataSetName, dataSetTitle;
      dataSetName = datasets[d]->Name();
      dataSetTitle = datasets[d]->Title();
      datasetNames.push_back(dataSetName);
      datasetTitles.push_back(dataSetTitle);
   }	 
	 Luminosity = lumi;
 	 Name = VarLabel;
	 legSMProcess = new TLegend(0.6,0.7,0.9,0.9); legSMProcess->SetName("legSMProcess");
	 legAll = new TLegend(0.6,0.7,0.9,0.9); legAll->SetName("legAll");
	 hstSMProcess = new THStack(TString("SMProcess_")+VarLabel, TString("SMProcess"));
	 hstAll = new THStack(TString("All_")+VarLabel,TString("AllProcess"));
	 
	 hSMProcess = new TH1F(TString("SMProcess_")+VarLabel, TString("SMProcess"),nbins, min, max);	 	 
	hSMProcess->Sumw2(); //(z) added, should be done for all histos??		 
	 hAll = new TH1F(TString("All_")+VarLabel,TString("AllProcess"),nbins, min, max);
        hAll->Sumw2(); //(z) added, should be done for all histos??

	 hSMSubProcesses = new TObjArray();
	 hNewPhysicsSignal = 0;
	 for(unsigned int d=0;d<datasetNames.size();d++){
		if(datasetNames[d].find("SUSY")<=datasetNames[d].size() || datasetNames[d].find("LM")<=datasetNames[d].size() || datasetNames[d].find("Zp")<=datasetNames[d].size() || datasetNames[d].find("NP")<=datasetNames[d].size() || datasetNames[d].find("prime")<=datasetNames[d].size()){
		   TH1F* hdummy = new TH1F(TString(datasetNames[d])+"_"+VarLabel,TString(datasetNames[d]),nbins, min, max);
		   hNewPhysicsSignal = hdummy;
		   continue;
		}	
		TH1F* hSMSubProcess = new TH1F(TString(datasetNames[d])+"_"+VarLabel,TString(datasetNames[d]),nbins, min, max);
		//hSMSubProcesses.push_back(hSMSubProcess);
		hSMSubProcesses->Add(hSMSubProcess);
	 }
}

/*void MCObsExpectation::SetDatasets(vector<Dataset*> datasets){
   for(unsigned int d=0;d<datasets.size();d++){
      //datasets_.push_back(datasets[d]);
      string dataSetName, dataSetTitle;
      dataSetName = datasets[d]->Name();
      dataSetTitle = datasets[d]->Title();
      datasetNames.push_back(dataSetName);
      datasetTitles.push_back(dataSetTitle);
   }
}*/

void MCObsExpectation::SetColors(vector<Dataset*> datasets){   
   for(unsigned int d=0;d<datasets.size();d++){
      if(datasetNames[d].find("SUSY")<=datasetNames[d].size() || datasetNames[d].find("LM")<=datasetNames[d].size() || datasetNames[d].find("Zp")<=datasetNames[d].size() || datasetNames[d].find("NP")<=datasetNames[d].size() || datasetNames[d].find("prime")<=datasetNames[d].size()){
	   hNewPhysicsSignal->SetFillColor(datasets[d]->Color());
           continue;
      }
      TH1F* hDummy = (TH1F*) hSMSubProcesses->At(d);
      hDummy->SetFillColor(datasets[d]->Color());
   }
}

TH1F* MCObsExpectation::GetHistogram(string hname){
  //the names of the histograms according to the dataset name!
    for(unsigned int d=0;d<datasetNames.size();d++){
       if(datasetNames[d].find("SUSY")<=datasetNames[d].size() || datasetNames[d].find("LM")<=datasetNames[d].size() || datasetNames[d].find("Zp")<=datasetNames[d].size() || datasetNames[d].find("NP")<=datasetNames[d].size() || datasetNames[d].find("prime")<=datasetNames[d].size()){
          if(hname == datasetNames[d]) return hNewPhysicsSignal;
       }
       TH1F* hDummy = (TH1F*) hSMSubProcesses->At(d);
       if(hname == datasetNames[d]) return hDummy;
    }
    cout<<"Input name does NOT match any of the available names"<<endl;
    return 0;
}

void MCObsExpectation::Compute(){
    int dNP = -999;
    for(unsigned int d=0;d<datasetNames.size();d++){
       if(datasetNames[d].find("SUSY")<=datasetNames[d].size() || datasetNames[d].find("LM")<=datasetNames[d].size() || datasetNames[d].find("Zp")<=datasetNames[d].size() || datasetNames[d].find("NP")<=datasetNames[d].size() || datasetNames[d].find("prime")<=datasetNames[d].size()){
           dNP = d;
	   continue;
       }
       TH1F* hDummy = (TH1F*) hSMSubProcesses->At(d);
       if(hDummy->Integral()>0){
         hstSMProcess->Add(hDummy);
         legSMProcess->AddEntry(hDummy,TString(datasetTitles[d]),"F"); //GetTitle, not GetName...
	 hstAll->Add(hDummy);
	 legAll->AddEntry(hDummy,TString(datasetTitles[d]),"F");
       }
    }
    if(dNP>-999){
       hstAll->Add(hNewPhysicsSignal);
       legAll->AddEntry(hNewPhysicsSignal,TString(datasetTitles[dNP]),"F");
    }   
}

MCObsExpectation::MCObsExpectation(const MCObsExpectation& a){
  Luminosity = a.Luminosity;
  Name = a.Name;
  legSMProcess = a.legSMProcess;
  legAll = a.legAll;
  hstSMProcess = a.hstSMProcess;
  hstAll = a.hstAll;
  hSMProcess = a.hSMProcess;
  hAll = a.hAll;
  
  datasetNames = a.datasetNames;
  datasetTitles = a.datasetTitles;
  
  hSMSubProcesses = a.hSMSubProcesses;  
  hNewPhysicsSignal = a.hNewPhysicsSignal;
}

MCObsExpectation::~MCObsExpectation(){
  delete  legSMProcess;
  delete  legAll;
  delete  hstSMProcess;
  delete  hstAll;
  delete  hSMProcess;
  delete  hAll;
  
  delete hSMSubProcesses; //maybe this should be done in a different way...
  
  delete  hNewPhysicsSignal;
}

void MCObsExpectation::Fill(Dataset *dataset, float variable, float weight){
   //updated: same procedure to deal with overflow bin as in MultiSamplePlot //tbc
   for(unsigned int d=0;d<datasetNames.size();d++){       
       if(dataset->Name() == datasetNames[d]){          
	  hAll->Fill(variable, weight);
	  int N = hAll->GetNbinsX();
	  hAll->SetBinContent(N,hAll->GetBinContent(N)+hAll->GetBinContent(N+1));
	  hAll->SetBinContent(N+1,0);
	  if(datasetNames[d].find("SUSY")<=datasetNames[d].size() || datasetNames[d].find("LM")<=datasetNames[d].size() || datasetNames[d].find("Zp")<=datasetNames[d].size() || datasetNames[d].find("NP")<=datasetNames[d].size() || datasetNames[d].find("prime")<=datasetNames[d].size()){
	    hNewPhysicsSignal->Fill(variable, weight);
	    N = hNewPhysicsSignal->GetNbinsX();
	    hNewPhysicsSignal->SetBinContent(N,hNewPhysicsSignal->GetBinContent(N)+hNewPhysicsSignal->GetBinContent(N+1));
	    hNewPhysicsSignal->SetBinContent(N+1,0);
	    continue;
	  }
	  TH1F* hDummy = (TH1F*) hSMSubProcesses->At(d);
	  hDummy->Fill(variable, weight);
	  N = hDummy->GetNbinsX();
	  hDummy->SetBinContent(N,hDummy->GetBinContent(N)+hDummy->GetBinContent(N+1));
	  hDummy->SetBinContent(N+1,0);
	  hSMProcess->Fill(variable, weight);
	  N = hSMProcess->GetNbinsX();
	  hSMProcess->SetBinContent(N,hSMProcess->GetBinContent(N)+hSMProcess->GetBinContent(N+1));
	  hSMProcess->SetBinContent(N+1,0);
       }        
   }  
}
