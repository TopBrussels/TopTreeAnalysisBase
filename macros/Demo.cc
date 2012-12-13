#include <iomanip>
#include <cmath>
#include "TF1.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TBranch.h"
#include "TLegend.h"
#include "TStyle.h"

//TRootObjects include
#include "TopTreeProducer/interface/TRootGenEvent.h"

//user code
#include "../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../Content/interface/AnalysisEnvironment.h"
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
#include "../Tools/interface/TTreeLoader.h"

#include "Style.C"

using namespace std;


int
main (int argc, char *argv[])
{


  cout << "**********************************************************************" << endl;
  cout << "Begining of the program  !" << endl;
  cout << "**********************************************************************" << endl;
  TTree* configTree = new TTree("configTree","configuration Tree");
  TClonesArray* tcdatasets = new TClonesArray("Dataset",1000);
  configTree->Branch("Datasets","TClonesArray",&tcdatasets);

  setTDRStyle ();

  /////////////////////
  // Configuration
  /////////////////////
  int verbose = 2;
  float Luminosity = 100;	// in 1/fb
  cout << "The results will be obtained for a luminosity of " << Luminosity << " 1/pb" << endl;

  //XML file
  string xmlFileName ="../config/myconfig.xml";
  if (argc >= 2)
    xmlFileName=string(argv[1]);
  const char *xmlfile = xmlFileName.c_str();
  //Output files
  string rootFileName ("demo.root");

  /////////////////////
  TTreeLoader treeLoader;
  /////////////////////
  // Load Datasets
  /////////////////////
  if (verbose > 0)
    cout << " - Load datasets ..." << endl;
  vector < Dataset > datasets;
  treeLoader.LoadDatasets (datasets, xmlfile);
  for(unsigned int i=0;i<datasets.size();i++) new ((*tcdatasets)[i]) Dataset(datasets[i]);
  /////////////////////
  
  ////////////////////
  // AnaEnv
  ///////////////////
  AnalysisEnvironment anaEnv;
  cout<<"Loading environment ..."<<endl;
  AnalysisEnvironmentLoader anaLoad(anaEnv,xmlfile);

  if (verbose > 0)
    cout << " - Variable decleration ..." << endl;

  //vector of objects
  vector < TRootVertex > vertex;
  vector < TRootMuon > init_muons;
  vector < TRootElectron > init_electrons;
  vector < TRootJet > init_jets;
  vector < TRootMET > mets;

  ////////////////////////////////////
  /// Plots decleration
  ////////////////////////////////////
  if (verbose > 0)
    cout << " - Create histos ..." << endl;

  //Global variables
  int JetType = 0;
  TRootEvent* event = 0;

  //////////////////////////////////////////////
  //loop on datasets
  ////////////////////////////////////
  if (verbose > 0)
    cout << " - Loop over datasets ... " << datasets.size () << " datasets !" << endl;
  for (unsigned int d = 0; d < datasets.size (); d++)
    {
      if (verbose > 1)
	cout << "   Dataset " << d << ": " << datasets[d].Name () << endl;

      //open files and load
      ///////////////////////////////////////
      treeLoader.LoadDataset (datasets[d], anaEnv);
      ////////////////////////////////////////

      //loop on events
      if (verbose > 1)
	cout << "	Loop over events " << endl;
      for (unsigned int ievt = 0; ievt < datasets[d].eventTree()->GetEntries (); ievt++)
	{
	  if (verbose > 2)
	    cout << "Event " << ievt << endl;

	  ///////////////////////////////////////
	  event = treeLoader.LoadEvent (ievt, datasets[d], vertex, init_muons, init_electrons, init_jets, mets);
	  //////////////////////////////////////
          int itrigger = itrigger = treeLoader.iTrigger (datasets[d], string ("HLT_Mu9"), event->runId() );
 
	  //do what you want with objects here ...

	}			//loop on events
    }				//loop on datasets

  //Once everything is filled ...


  ///////////////////
  // Writting
  ///////////////////
  if (verbose > 1)
    cout << " - Writting outputs on files ..." << endl;
  TFile *fout = new TFile (rootFileName.c_str (), "RECREATE");
  //book plots
  if (verbose > 1)
    cout << " - Writting  the file ..." << endl;
  
  //write configuration
  fout->cd();
  configTree->Fill();
  configTree->Write();
  fout->Write ();
  fout->Close();

  //delete
  delete fout;
  delete configTree;
  delete tcdatasets;

  cout << "**********************************************************************" << endl;
  cout << "           End of the program !!" << endl;
  cout << " 		doesn't crashed ;-) " << endl;
  cout << "**********************************************************************" << endl;
}
