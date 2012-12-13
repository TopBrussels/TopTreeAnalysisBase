#include <string>
#include <vector>

#include "../Content/interface/Dataset.h"

using namespace std;


//Output
string FilenameOutputRootFile("test.root");
string FilenameOutputTextFile("test.log");


void LoadJetCollection(vector<string>& jetCollection, vector<string>& jetLabels){
	//jetCollection.push_back("Jets_allLayer1Jets");
	jetCollection.push_back("Jets_allLayer1JetsIC5");
	jetCollection.push_back("Jets_allLayer1JetsSC5");
	jetCollection.push_back("Jets_allLayer1JetsKT4");
	jetCollection.push_back("Jets_allLayer1JetsAntiKT4");
	jetCollection.push_back("Jets_allLayer1JetsCA4");
	jetCollection.push_back("Jets_allLayer1JetsMP5");
	jetCollection.push_back("Jets_allLayer1JetsCDFMP5");
 
	jetLabels.push_back("IC5");
	jetLabels.push_back("SC5");
	jetLabels.push_back("KT4");
	jetLabels.push_back("Anti-KT4");
	jetLabels.push_back("CA4");
	jetLabels.push_back("MP5");
	jetLabels.push_back("CDF-MP5");
 
}

void LoadDatasets(vector<Dataset>& Datasets){

	//boolean to run datasets or no
	bool doTTJets = true;
	bool doWJets = false;
	bool doZJets = false;
	bool doQCD = false;
	bool doSUSY = true;
	bool doSingleTop = false;

  	int color = 2;
  	int linestyle = 1;
  	int linewidth = 2;
  	float normfactor = 1;


	//QCD
	if(doQCD){
 	 	string name("QCD");
		int color_ = 12;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 121650/6315808.;
		float xsection_ = 121650; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/TopTree_QCD.root"));
		//filenames.push_back(string("/user_mnt/user/ghammad/CMSSW_2_2_7/src/TopBrussels/TopTreeProducer/test/InclusiveMuPt15/TopTree.root"));
		Dataset* a = new Dataset(name, doQCD, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	//ZJets
	if(doZJets){
 	 	string name("ZJets");
		int color_ = 28;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 4218./1262816;
		float xsection_ = 4218; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/TopTree_ZJets.root"));
		Dataset* a = new Dataset(name, doZJets, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	//WJets
	if(doWJets){
 	 	string name("WJets");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 45600./9745661.;
		float xsection_ = 45600; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/TopTree_WJets.root"));
		Dataset* a = new Dataset(name, doWJets, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}

	
	//SingleTop 
	if(doSingleTop){
 	 	string name("SingleTop");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 1;
		float xsection_ = 1;
		vector<string> filenames;
		//filenames.push_back(string(""));
		Dataset* a = new Dataset(name, doSingleTop, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}


	//TTJets
	if(doTTJets){
 	 	string name("TTJets");
		int color_ = 4;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ =  414./946644.;
		float xsection_ = 414; 
		vector<string> filenames;
		//filenames.push_back(string("/user_mnt/user/ghammad/CMSSW_2_2_9/src/TopBrussels/TopTreeProducer/test/TTJets/TopTree.root"));
		filenames.push_back(string("/user/echabert/TopTree_Prod229/TopTree_TTJets.root"));
		//filenames.push_back(string("/user/blyweert/PATProduction/CMSSW_2_2_9/src/TopBrussels/TopTreeProducer/TopTrees/TopTree_TTJets-madgraph.root "));
		Dataset* a = new Dataset(name, doTTJets, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}

//SUSY
	if(doSUSY){
	  /*string name("SUSY");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 16.06/208314.;
		float xsection_ = 16.06; 
		vector<string> filenames;*/
		//filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM1.root"));
		//filenames.push_back(string("/user/echabert/TopTree_Prod229/TopTree_TTJets.root"));
		//Dataset* a = new Dataset(name, doSUSY, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		//Datasets.push_back(*a);}
	  /*	  
	bool doLM0 = false;
	bool doLM1 = true;
	bool doLM2 = true;
	bool doLM3 = false;
	bool doLM4 = true;
	bool doLM5 = false;
	bool doLM6 = false;
	bool doLM7 = true;
	bool doLM8 = false;
	bool doLM9 = false;
	bool doLM10 = false;
	  */
	  
	bool doLM0 = false;
	bool doLM1 = false;
	bool doLM2 = false;
	bool doLM3 = true;
	bool doLM4 = false;
	bool doLM5 = false;
	bool doLM6 = false;
	bool doLM7 = false;
	bool doLM8 = false;
	bool doLM9 = false;
	bool doLM10 = true;
	  
  	int linestyle_ = 1;
  	int linewidth_ = 2;
	// string name("SUSY");
	//int color_ = 2;

	
	if(doLM0){
		int color_ = 6;
		float normfactor_ = 110./202686.;
		float xsection_ = 110.; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM0.root"));
		Dataset* a = new Dataset(string("LM0"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM1){
		int color_ = 7;
		float normfactor_ = 16.06/123200.;
		float xsection_ = 16.06; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM1.root"));
		Dataset* a = new Dataset(string("LM1"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM2){
		int color_ = 8;
		float normfactor_ = 2.42/121600.;
		float xsection_ = 2.42; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM2.root"));
		Dataset* a = new Dataset(string("LM2"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM3){
		int color_ = 9;
		float normfactor_ = 6.06/138600.;
		float xsection_ = 6.06; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM3.root"));
		Dataset* a = new Dataset(string("LM3"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM4){
		int color_ = 14;
		float normfactor_ = 6.70/101000.;
		float xsection_ = 6.70; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM4.root"));
		Dataset* a = new Dataset(string("LM4"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM5){
		int color_ = 28;
		//float normfactor_ = 1.94/183800.;
		float normfactor_ = 1.94/93800.;//not full stat !!
		float xsection_ = 1.94; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM5.root"));
		Dataset* a = new Dataset(string("LM5"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM6){
		int color_ = 30;
		float normfactor_ = 1.28/129600.;
		float xsection_ = 1.28; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM6.root"));
		Dataset* a = new Dataset(string("LM6"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM7){
		int color_ = 38;
		float normfactor_ = 2.90/89600.;
		float xsection_ = 2.90; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM7.root"));
		Dataset* a = new Dataset(string("LM7"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM8){
		int color_ = 41;
		float normfactor_ = 2.86/211302.;
		float xsection_ = 2.86; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM8.root"));
		Dataset* a = new Dataset(string("LM8"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM9){
		int color_ = 12;
		float normfactor_ = 11.58/200784.;
		float xsection_ = 11.58; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM9.root"));
		Dataset* a = new Dataset(string("LM9"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM10){
		int color_ = 46;
		float normfactor_ = 0.065/50000;//202860.;
		float xsection_ = 0.065; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM10.root"));
		Dataset* a = new Dataset(string("LM10"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}

}
	
	

}
void LoadDatasetsLMX(vector<Dataset>& Datasets){
	//boolean to run datasets or no
  /*	bool doLM0 = true;
	bool doLM1 = true;
	bool doLM2 = true;
	bool doLM3 = true;
	bool doLM4 = true;
	bool doLM5 = true;
	bool doLM6 = true;
	bool doLM7 = true;
	bool doLM8 = true;
	bool doLM9 = true;
	bool doLM10 = true;
  */

	bool doLM0 = false;
	bool doLM1 = false;
	bool doLM2 = false;
	bool doLM3 = false;
	bool doLM4 = true;
	bool doLM5 = false;
	bool doLM6 = false;
	bool doLM7 = false;
	bool doLM8 = false;
	bool doLM9 = false;
	bool doLM10 = false;

  	int linestyle_ = 1;
  	int linewidth_ = 2;
        string name("SUSY");
	int color_ = 2;

	
	if(doLM0){
		int color_ = 6;
		float normfactor_ = 110./202686.;
		float xsection_ = 110.; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM0.root"));
		Dataset* a = new Dataset(string("LM0"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM1){
		int color_ = 7;
		float normfactor_ = 16.06/123200.;
		float xsection_ = 16.06; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM1.root"));
		Dataset* a = new Dataset(string("LM1"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM2){
		int color_ = 8;
		float normfactor_ = 2.42/121600.;
		float xsection_ = 2.42; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM2.root"));
		Dataset* a = new Dataset(string("LM2"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM3){
		int color_ = 9;
		float normfactor_ = 6.06/138600.;
		float xsection_ = 6.06; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM3.root"));
		Dataset* a = new Dataset(string("LM3"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM4){
		int color_ = 14;
		float normfactor_ = 6.70/101000.;
		float xsection_ = 6.70; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM4.root"));
		Dataset* a = new Dataset(string("LM4"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM5){
		int color_ = 28;
		//float normfactor_ = 1.94/183800.;
		float normfactor_ = 1.94/93800.;//not full stat !!
		float xsection_ = 1.94; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM5.root"));
		Dataset* a = new Dataset(string("LM5"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM6){
		int color_ = 30;
		float normfactor_ = 1.28/129600.;
		float xsection_ = 1.28; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM6.root"));
		Dataset* a = new Dataset(string("LM6"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM7){
		int color_ = 38;
		float normfactor_ = 2.90/89600.;
		float xsection_ = 2.90; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM7.root"));
		Dataset* a = new Dataset(string("LM7"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM8){
		int color_ = 41;
		float normfactor_ = 2.86/211302.;
		float xsection_ = 2.86; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM8.root"));
		Dataset* a = new Dataset(string("LM8"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM9){
		int color_ = 12;
		float normfactor_ = 11.58/200784.;
		float xsection_ = 11.58; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM9.root"));
		Dataset* a = new Dataset(string("LM9"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	if(doLM10){
		int color_ = 46;
		float normfactor_ = 0.065/50000;//202860.;
		float xsection_ = 0.065; 
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/TopTree_Prod229/LMX/TopTree_LM10.root"));
		Dataset* a = new Dataset(string("LM10"), true, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
   }


void LoadDatasetsZprime(vector<Dataset>& Datasets){

	//boolean to run datasets or no
	bool doZp500 = true;
	bool doZp750 = false;
	bool doZp1000 = false;
	bool doZp1250 = false;
	bool doZp1500 = false;
	bool doZp2000 = false;

  	int color = 2;
  	int linestyle = 1;
  	int linewidth = 2;
  	float normfactor = 1;


	if(doZp500){
 	 	string name("Zprime500");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 1;
		float xsection_ = 1;
		vector<string> filenames;
		filenames.push_back(string("/home/echabert/Data/TopTree_Zp500.root"));
		Dataset* a = new Dataset(name, doZp500, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	
	if(doZp750){
 	 	string name("Zprime750");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 1;
		float xsection_ = 1;
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/CMSSW/TopTreeTest/CMSSW_2_2_9/src/TopBrussels/TopTreeProducer/test/CRAB/TopTree_Zp750.root"));
		Dataset* a = new Dataset(name, doZp750, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	
	if(doZp1000){
 	 	string name("Zprime1000");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 1;
		float xsection_ = 1;
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/CMSSW/TopTreeTest/CMSSW_2_2_9/src/TopBrussels/TopTreeProducer/test/CRAB/TopTree_Zp1000.root"));
		Dataset* a = new Dataset(name, doZp1000, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}

	if(doZp1250){
 	 	string name("Zprime1250");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 1;
		float xsection_ = 1;
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/CMSSW/TopTreeTest/CMSSW_2_2_9/src/TopBrussels/TopTreeProducer/test/CRAB/TopTree_Zp1250.root"));
		Dataset* a = new Dataset(name, doZp1250, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}

	if(doZp1500){
 	 	string name("Zprime1500");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 1;
		float xsection_ = 1;
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/CMSSW/TopTreeTest/CMSSW_2_2_9/src/TopBrussels/TopTreeProducer/test/CRAB/TopTree_Zp1500.root"));
		Dataset* a = new Dataset(name, doZp1500, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
	
	if(doZp2000){
 	 	string name("Zprime2000");
		int color_ = 2;
		int linestyle_ = 1;
		int linewidth_ = 1;
		float normfactor_ = 1;
		float xsection_ = 1;
		vector<string> filenames;
		filenames.push_back(string("/user/echabert/CMSSW/TopTreeTest/CMSSW_2_2_9/src/TopBrussels/TopTreeProducer/test/CRAB/TopTree_Zp2000.root"));
		Dataset* a = new Dataset(name, doZp2000, color_, linestyle_, linewidth_, normfactor_, xsection_, filenames);
		Datasets.push_back(*a);
	}
}
