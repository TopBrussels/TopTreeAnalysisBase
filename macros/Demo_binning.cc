#include <iomanip>
#include <cmath>
#include "TF1.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TBranch.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TObject.h"

//TRootObjects include
#include "TopTreeProducer/interface/TRootGenEvent.h"

//user code
#include "../config/Datasets.cc"
#include "../Selection/interface/SelectionTable.h"
#include "../BkgEstimationMethods/interface/WJetEstimation.h"
#include "../BkgEstimationMethods/interface/TtJetEstimation.h"
#include "../BkgEstimationMethods/interface/ABCDEstimation.h"
#include "../BkgEstimationMethods/interface/QCDShapeEstimation.h"
#include "../BkgEstimationMethods/interface/WJetShapeEstimation.h"
#include "../BkgEstimationMethods/interface/BkgEstimationSummary.h"
#include "../Tools/interface/PlottingTools.h"
#include "../Tools/interface/MultiSamplePlot.h"
#include "../Tools/interface/MultiCutPlot.h"
#include "../Tools/interface/CutImpactEvaluation.h"

#include "../MCInformation/interface/ObservablesMCMatcher.h"
#include "../Tools/interface/Binner.h"
#include "../Content/interface/BinningFirstGuess.h"

#include "../MCInformation/interface/JetPartonMatching.h"

#include "Style.C"

using namespace std;
using namespace TopTree;




int main(int argc, char*argv[]){

  cout<<"**********************************************************************"<<endl;
  cout<<"Begining of the program  !" <<endl;
  cout<<"**********************************************************************"<<endl;

    //setTDRStyle(); 

  /////////////////////
  // Configuration
  /////////////////////

  if (argc!=3)
  {
    printf("Not 2 arguments. Please fill in correctly : first is mode and second is DeltaR for matching.\n");
    return 2;
  }
  int binnerMode = atoi(argv[1]); // binnerMode=1 for BinningFirstGuess ; binnerMode=2 for Binner
  int verbose = 2;
  float Luminosity = 100; // in 1/fb
  bool addLMX = true;
  cout<<"The results will be obtained for a luminosity of "<<Luminosity<<" 1/pb"<<endl;
  
  //Output files
  string rootFileName("BinnerOutput.root");
  if (binnerMode==1)
    rootFileName = string("FirstBinning.root");


  /////////////////////
  // Load Datasets
  /////////////////////
  if(verbose>0) cout<<" - Load datasets ..."<<endl;
  vector<Dataset> datasets;
  LoadDatasets(datasets);
  vector<Dataset> LMX;
  if(addLMX){
  	LoadDatasetsLMX(datasets);
  	LoadDatasetsLMX(LMX);
  }
  /////////////////////

  
  if(verbose>0) cout<<" - Variable declaration ..."<<endl;

  //vector of objects
  vector<TRootMuon> init_muons;
  vector<TRootElectron> init_electrons;
  vector<TRootJet> init_jets;
  vector<TRootMET> mets;
 
  TBranch *jets_br;
  TClonesArray *tcjets;
  TBranch *muons_br;
  TClonesArray *tcmuons;
  TBranch *electrons_br;
  TClonesArray *tcelectrons;
  TBranch *mets_br;
  TClonesArray *tcmets;
  TBranch *genEvt_br;
  TClonesArray *tcgenEvt;
  TBranch *npgenEvt_br;
  TClonesArray *tcnpgenEvt;
  TFile* fout = new TFile(rootFileName.c_str(),"RECREATE");

 ////////////////////////////////////
 /// Plots decleration
 ////////////////////////////////////
  if(verbose>0) cout<<" - Create histos ..."<<endl;
    //////////////////////////////////////////////
  //loop on datasets
 ////////////////////////////////////
  if(verbose>0) cout<<" - Loop over datasets ... "<<datasets.size()<<" datasets !"<<endl;
  /*  Additional inserts : for NPGenEvent loading  */
    //datasets[i].NormFactor()      datasets[i].Xsection()
    //    float lumi = 250.; //luminositu in fb-1
  float intLumiEst = 250./*datasets[d].NormFactor() * lumi*/;
  Binner *binner = NULL;// new Binner(intLumiEst);
  BinningFirstGuess *firstGuess = new BinningFirstGuess(true, intLumiEst);
  if (binnerMode==2) {
    BinningFirstGuess *plouf = NULL;
    TFile f("FirstBinning.root", "READ");
    plouf = (BinningFirstGuess*) f.Get("firstBinning");
    delete binner;
    plouf->Print();
    binner = new Binner(250., plouf->getBinningsFirstGuess(250.));
    binner->Check();
    f.Close();
    fout->cd();
    //delete plouf;
  }
  for(unsigned int d=0;d<datasets.size();d++){
    
    /* Begin Thierry */
    
    /* End Thierry */
    
  	if(verbose>1) cout<<"   Dataset "<<d<<": "<<datasets[d].Name()<<endl;
  	//open files and load
	TChain* eventTree = datasets[d].eventTree();

  	///////////////////////////////////////
  	//Branches and TCLonesArray
  	///////////////////////////////////////
  	//jets
  	jets_br = (TBranch *) eventTree->GetBranch ("Jets");
  	tcjets = new TClonesArray ("TopTree::TRootJet", 0);
  	jets_br->SetAddress (&tcjets);
  	//muons
  	muons_br = (TBranch *) eventTree->GetBranch ("Muons");
  	tcmuons = new TClonesArray ("TopTree::TRootMuon", 0);
  	muons_br->SetAddress (&tcmuons);
  	//electrons
  	electrons_br = (TBranch *) eventTree->GetBranch ("Electrons");
  	tcelectrons = new TClonesArray ("TopTree::TRootElectron", 0);
  	electrons_br->SetAddress (&tcelectrons);
  	//mets
  	mets_br = (TBranch *) eventTree->GetBranch ("MET");
  	tcmets = new TClonesArray ("TopTree::TRootMET", 0);
  	mets_br->SetAddress (&tcmets);
	//GenEvent
	genEvt_br = (TBranch *) eventTree->GetBranch ("GenEvent");
  	tcgenEvt = new TClonesArray ("TopTree::TRootGenEvent", 0);
  	if(genEvt_br) genEvt_br->SetAddress (&tcgenEvt);
      //NPGenEvent
        npgenEvt_br = (TBranch *) eventTree->GetBranch ("NPGenEvent");
  	tcnpgenEvt = new TClonesArray ("TopTree::TRootNPGenEvent", 0);
  	if(npgenEvt_br) npgenEvt_br->SetAddress (&tcnpgenEvt);
      ////////////////////////////////////////

	//loop on events
  	if(verbose>1) cout<<"	Loop over events "<<endl;
	for(unsigned int ievt=0;ievt<eventTree->GetEntries() /*&& ievt<1000*/;ievt++){
		if(verbose>2) cout<<"Event "<<ievt<<endl;
	        eventTree->GetEvent(ievt);
	        
		// clear vectors	
		init_jets.clear();
		init_muons.clear();
		init_electrons.clear();
		mets.clear();
	  	
		TRootGenEvent* genEvt = NULL;
		
		//fill vectors
		for(int i=0;i<tcjets->GetEntriesFast();i++) init_jets.push_back(* (TRootJet*) tcjets->At(i));
		for(int i=0;i<tcmuons->GetEntriesFast();i++) init_muons.push_back(* (TRootMuon*) tcmuons->At(i));
		for(int i=0;i<tcelectrons->GetEntriesFast();i++) init_electrons.push_back(* (TRootElectron*) tcelectrons->At(i));
		for(int i=0;i<tcmets->GetEntriesFast();i++) mets.push_back(* (TRootMET*) tcmets->At(i));
  	        if(tcgenEvt->GetEntriesFast()==1) genEvt = (TRootGenEvent*) tcgenEvt->At(0);
    TRootNPGenEvent *npgenEvt;
		if(tcnpgenEvt->GetEntriesFast()==1) npgenEvt = (TRootNPGenEvent*) tcnpgenEvt->At(0);
    /* Begin Thierry */
    ObservablesMCMatcher obsMC;
      //    float ElectronPtCut = 10;
      //    float ElectronEtaCut = 2.0;
      //    float ElectronRelIsoCut = 0.1;
      //SR
    float MuonPtCutSR = 30;
    float MuonEtaCutSR = 2.1;
    float MuonRelIsoCutSR = 0.1;
    float JetsPtCutSR = 30;
    float JetsEtaCutSR = 2.4;
      //CR: ttjetEstimation
      //    float MuonPtCutCR = 30;
      //    float MuonEtaCutCR = 2.1;
      //    float MuonRelIsoCutCR = 0.1;
      //    float JetsPtCutCR = 30;
      //    float JetsEtaCutCR = 2.4;
    float JetshcalFraction = 0.1;
    Selection selection(init_jets, init_muons, init_electrons, mets);
		vector<TRootJet> jets = selection.GetSelectedJets(JetsPtCutSR,JetsEtaCutSR, JetshcalFraction);
    vector<TRootMuon> muons = selection.GetSelectedMuons(MuonPtCutSR,MuonEtaCutSR,MuonRelIsoCutSR);
    if (muons.size()>0 && jets.size()>3)
    {
      string dsname("MC");
        if (!genEvt->isSemiLeptonic())
          continue;
/*      TRootGenEvent& genevt, const TRootMuon& lepton, const TRootJet& qFromW1, const TRootJet& qFromW2, const TRootJet& bFromHadTop, const TRootJet& bFromLepTop, const TRootMET& MET, const vector<TRootJet>& jets*/
      TRootMET metSum;
      for (vector<TRootMET>::iterator iter = mets.begin(); iter!=mets.end(); iter++)
        metSum += *iter;
      if (muons.size()>0 && jets.size()>3)
      {
	        printf("At least enough for jets and muons.\n");
        if(muons.size()>1)
        printf("WARNING : even with a trick, you are facing problems : several muon candidates !!!");
        
        /* Association from Monte-Carlo. Cheating but only for testing. 
         It's an analysis without bad identification, only systematics from the variables by themselves. Begin */
        vector<TLorentzVector> jets_tlorentz(jets.begin(), jets.end());
        vector<TLorentzVector> gens;
        gens.push_back(genEvt->hadronicDecayQuark());
        gens.push_back(genEvt->hadronicDecayQuarkBar());
        gens.push_back(genEvt->hadronicDecayB());
        gens.push_back(genEvt->leptonicDecayB());
        JetPartonMatching jpm(gens, jets_tlorentz, (int) JetPartonMatching::totalMinDist, true, true, atof(argv[2]));
        vector<int> indices;
        bool skip=false;
        for (size_t i=0; i<gens.size(); i++) {
          int index = jpm.getMatchForParton(i, 0);
          indices.push_back(index);
          if (index < 0)
            skip=true;
        }
        if (skip) continue;
          //        obs->SetMCInfo(leptonMC, qFromW1MC, qFromW2MC, bFromHadTopMC, bFromLepTopMC, METMC, quarks);

        /* End of cheating */
//                  printf("%d\t%d\t%d\t%d\n", indices[0], indices[1], indices[2], indices[3]);
        if (binnerMode==1) {
          firstGuess->Fill(obsMC.getMatchedObservables(*genEvt, muons[0],jets[indices[0]], jets[indices[1]], jets[indices[2]], jets[indices[3]],
						       metSum, jets)/*,  string("MET")*/);
        }
        else if (binnerMode==2)
        {
          obsMC.getMatchedObservables(*genEvt, muons[0],jets[indices[0]], jets[indices[1]], jets[indices[2]], jets[indices[3]], metSum, jets);
          binner->Fill(obsMC.getMatchedObservables(*genEvt, muons[0],jets[indices[0]], jets[indices[1]], jets[indices[2]], jets[indices[3]],
						   metSum, jets)/*,  string("MET")*/);
        /*      binner->Fill(obsMC.getMatchedObservables(*npgenEvt, muons[0], jets, mets[0], dsname),
         string("MET"));
         */
        }
      }
    }
    /* End Thierry */
    
		if(verbose>3) cout<<"Nof muons "<<init_muons.size()<<endl;
		if(verbose>3) cout<<"Nof electrons "<<init_electrons.size()<<endl;
		if(verbose>3) cout<<"Nof jets "<<init_jets.size()<<endl;
		
	}//loop on events
    /* Begin Thierry */
    fout->cd();
    if (binnerMode==1) {
      firstGuess->Write("firstBinning");
    }
    else if (binnerMode==2) {
      binner->Check();
    printf("Before getting profile. After filling\n");
    TProfile* profMET = binner->getProfile("MET");
    for (Int_t i =1; i<=profMET->GetNbinsX(); i++) {
      printf("Bin %d (%f - %f)\t %f entries \t -> mean = %f\n", i, profMET->GetBinLowEdge(i), profMET->GetBinLowEdge(i)+profMET->GetBinWidth(i),
	     profMET->GetBinEntries(i), profMET->GetBinContent(i));
    }
    printf("profMET : %lu \n", (unsigned long) profMET);
    vector<TProfile*> *profs = binner->getProfiles();
    printf("Saving all of the profiles (%lu)\n", profs->size());
    for (vector<TProfile*>::iterator iter=profs->begin(); iter != profs->end(); iter++)
    {    printf(" prof : %lu \n", (unsigned long) *iter);
; (*iter)->Write();}
/*    TCanvas c;
    profMET->Draw();
    printf("save in pdf\n");
    c.Print("ProfileMET.pdf");
*/    printf("Before getting axis. After getting profile\n");
      TFile f("config/Binning.root","RECREATE");
      vector<string> lstVarCpld = binner->listOfVariables();
      TAxis* axe = NULL;
      for (vector<string>::iterator iteri=lstVarCpld.begin(); iteri!=lstVarCpld.end(); iteri++)
      {
        axe = binner->getBins(*iteri, 100., 50, 1.);
        printf("nombre de bins : %d\n", axe->GetNbins());
        axe->SetName(("Binning_"+ *iteri +"_TTb_100fb-1").c_str());
        axe->Write();
      }
      f.Close();
      
      fout->cd();
      TAxis* axis = binner->getBins(string("MET"), 100., 50, 1.);
    printf("Before writing profile. After getting axis\n");
//    profMET->Write();
    printf("Before writing axis. After writing profile\n");
      

    axis->SetName("axis_MET_TTb_100fb-1");
    axis->Write();
    axis = binner->getBins(string("MET"), 250., 50, 1.);
    axis->SetName("axis_MET_TTb_250fb-1");
    axis->Write();
    axis = binner->getBins(string("MET"), 50., 50, 1.);
    axis->SetName("axis_MET_TTb_50fb-1");
    axis->Write();
    axis = binner->getBins(string("MET"), 500., 50, 1.);
    axis->SetName("axis_MET_TTb_500fb-1");
    axis->Write();
    delete profs;
      
    }
    /* End Thierry */
  }//loop on datasets

  //Once everything is filled ...
  //firstGuess->Print();
  if (binnerMode==2) {
      //delete firstGuess;
  }

  ///////////////////
  // Writting
  ///////////////////
  if(verbose>1) cout<<" - Writing outputs on files ..."<<endl;
  //book plots
  if(verbose>1) cout<<" - Writing  the file ..."<<endl;
  fout->Write();
  /* Begin Thierry */
  fout->Close();
  /* End Thierry */
  cout<<"**********************************************************************"<<endl;
  cout<<"           End of the program !!" <<endl;
  cout<<" 		didn't crash ;-) "<<endl;
  cout<<"**********************************************************************"<<endl;

};

