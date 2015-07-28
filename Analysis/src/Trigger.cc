#include "../interface/Trigger.h"

Trigger::Trigger(bool isMuon, bool isElectron):
muon(false), electron(false), trigged(false), redotrigmap(false), triggerList(), currentRun(0), previousRun(-1), currentFilename(""),
 previousFilename(""), iFile(0), triggermap()
{
	if(isMuon){
		muon = true;
	}
	else if(isElectron){
		electron = true;
	}
	else{
		cout<<"neither lepton selection"<<endl;
	}
}

Trigger::~Trigger(){

}

void Trigger::bookTriggers(){
	if(muon){
	    triggerList.push_back("HLT_IsoMu20_eta2p1_v2");
	    triggerList.push_back("HLT_IsoMu20_eta2p1_TriCentralPFJet30_v2");
	    triggerList.push_back("HLT_IsoMu20_eta2p1_TriCentralPFJet50_40_30_v2");
	    triggerList.push_back("HLT_IsoMu20_eta2p1_v2");
	    triggerList.push_back("HLT_IsoMu20_eta2p1_TriCentralPFJet30_v2");
	    triggerList.push_back("HLT_IsoMu20_eta2p1_TriCentralPFJet50_40_30_v2");		
	}

    if (electron){
	    triggerList.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf_v1");
	    triggerList.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf_TriCentralPFJet30_v1");
	    triggerList.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf_TriCentralPFJet50_40_30_v1");
	    triggerList.push_back("HLT_Ele27_eta2p1_WP75_Gsf_v1");
	    triggerList.push_back("HLT_Ele27_eta2p1_WP75_Gsf_TriCentralPFJet30_v1");
	    triggerList.push_back("HLT_Ele27_eta2p1_WP75_Gsf_TriCentralPFJet50_40_30_v1");    	
    }

    for(UInt_t itrig=0; itrig<triggerList.size(); itrig++){
        triggermap[triggerList[itrig]]=std::pair<int,bool>(-999,false);
    }

    // for(std::map<std::string,std::pair<int,bool> >::iterator trigiter = triggermap.begin(); trigiter != triggermap.end(); trigiter++){
    //     std::pair<int,bool> bla = trigiter->second;
    //     std::string bla2 = trigiter->first; 
    // }    
}

void Trigger::checkAvail(int currentRun, vector < Dataset* > datasets, unsigned int d, TTreeLoader treeLoader, TRootEvent* event){

	redotrigmap=false;
	currentFilename = datasets[d]->eventTree()->GetFile()->GetName();
	if(previousFilename != currentFilename){
	    previousFilename = currentFilename;
	    iFile++;
	    redotrigmap=true;
	    cout<<"File changed!!! => iFile = "<<iFile << " new file is " << datasets[d]->eventTree()->GetFile()->GetName() << " in sample " << datasets[d]->Name() << endl;
	}
	if(previousRun != currentRun){
	    previousRun = currentRun;
	    redotrigmap=true;
	}

	// // get trigger info:
	// for(std::map<std::string,std::pair<int,bool> >::iterator iter = triggermap.begin(); iter != triggermap.end(); iter++){
	//     if(redotrigmap){
	//         Int_t loc= treeLoader.iTrigger(iter->first, currentRun);
	//         iter->second.first=loc;
	//     }
	//     // and check if it fired:
	//     if(iter->second.first>=0 && iter->second.first!=9999) // trigger exists
	//         iter->second.second=event->trigHLT(iter->second.first);
	//     else
	//         iter->second.second=false;
	// }
            

}

int Trigger::checkIfFired(){
	// now check if the appropriate triggers fired for each analysis:
	if(muon){
		for(UInt_t itrig=0; itrig<triggerList.size() && trigged==0; itrig++){
		    if(triggermap[triggerList[itrig]].second)    trigged=1;
		}		
	}

	else if (electron){
		for(UInt_t itrig=0; itrig<triggerList.size() && trigged==0; itrig++){
		    if(triggermap[triggerList[itrig]].second)   trigged=1;
		}
	}

	return trigged;
}


