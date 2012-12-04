#include "../interface/MVAComputer.h"

MVAComputer::MVAComputer(std::string Method, std::string TrainerOutputRootFile, std::string WeightsPrefix, std::vector<std::string> MVAvars, std::string postfix) {

  Method_ = Method;
	postfix_ = postfix;
  std::cout << std::endl << "==> Start TMVA Computer" << std::endl;
  
  // set the  filenames
  TrainerOutputRootFile_ = TrainerOutputRootFile;
  WeightsPrefix_ = WeightsPrefix ;
  
  //---------------------------------------------------------------
  // default MVA methods to be trained + tested
  Use["Cuts"]            = 0;
  Use["CutsD"]           = 0;
  Use["CutsPCA"]         = 0;
  Use["CutsGA"]          = 0;
  Use["CutsSA"]          = 0;
  // ---
  Use["Likelihood"]      = 0;
  Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
  Use["LikelihoodPCA"]   = 0; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
  Use["LikelihoodKDE"]   = 0;
  Use["LikelihoodMIX"]   = 0;
  // ---
  Use["PDERS"]           = 0;
  Use["PDERSD"]          = 0;
  Use["PDERSPCA"]        = 0;
  Use["PDERSkNN"]        = 0; // depreciated until further notice
  Use["PDEFoam"]         = 0;
  // --
  Use["KNN"]             = 0;
  // ---
  Use["HMatrix"]         = 0;
  Use["Fisher"]          = 0;
  Use["FisherG"]         = 0;
  Use["BoostedFisher"]   = 0;
  Use["LD"]              = 0;
  // ---
  Use["FDA_GA"]          = 0;
  Use["FDA_SA"]          = 0;
  Use["FDA_MC"]          = 0;
  Use["FDA_MT"]          = 0;
  Use["FDA_GAMT"]        = 0;
  Use["FDA_MCMT"]        = 0;
  // ---
  Use["MLP"]             = 0; // this is the recommended ANN
  Use["MLPD"]             = 0; // this is the recommended ANN
  Use["MLPBFGS"]         = 0; // recommended ANN with optional training method
  Use["CFMlpANN"]        = 0; // *** missing
  Use["TMlpANN"]         = 0; 
  // ---
  Use["SVM"]             = 0;
  // ---
  Use["BDT"]             = 0;
  Use["BDTD"]            = 0;
  Use["BDTG"]            = 0;
  Use["BDTB"]            = 0;
  // ---
  Use["RuleFit"]         = 0;
  // ---
  Use["Plugin"]          = 0;
  // ---------------------------------------------------------------
  
  
  // check if the requested method is known
  if (Use.find(Method) == Use.end()) {
    std::cout << "Method \"" << Method << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
     for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
     std::cout << std::endl;
     exit(1);
  }
  
  // enable the requested methods
  Use[Method] = 1;

  // create output root-file (it will contain MVA output and the S and B Trees)
 
  trainingOutput = new TFile(TrainerOutputRootFile_.c_str(),"UPDATE");

  // initialise TMVA Reader

  reader = new TMVA::Reader( "!Color:!Silent" );

  // init variables 
  
  for (std::vector<std::string>::iterator it = MVAvars.begin(); it != MVAvars.end(); ++it) {
    
    //std::cout << *it << std::endl;
    
    bookInputVar((*it).c_str());
    
  }

  // book method(s)
  TString prefix = (TString) WeightsPrefix;
  TString dir = "weights" + postfix_ + "/";

	cout << "postfix_" << postfix_ << endl;
  
	cout << "name of weights dir " << dir << endl;
	
	for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
    if (it->second) {
      TString methodName = it->first + " method";
      TString weightfile = dir + prefix + "_" + TString(it->first) + ".weights.xml";
      reader->BookMVA( methodName, weightfile ); 
    }
  }
   
   // example how to use your own method as plugin
   if (Use["Plugin"]) {
     
      gPluginMgr->AddHandler("TMVA@@MethodBase", "PluginName", "MethodClassName", "UserPackage", "MethodName(DataSet&,TString)");
      // the class is then looked for in libUserPackage.so

      // now the method can be booked like any other
      reader->BookMVA( "User method", dir + prefix + "_User.weights.txt" );
   }

  
}
MVAComputer::~MVAComputer() {

  std::cout << std::endl << "==> Destructing computer, writing root-files" << std::endl;
  
  trainingOutput->Close();

  delete trainingOutput;

}

vector<string> MVAComputer::GetAllMethods()
{
  vector<string> out;
 
  for (map<string, int>::const_iterator it = Use.begin(); it != Use.end(); ++it)
    if(it->second == 1) out.push_back(it->first);
  
  return out;
}

void MVAComputer::addMethod(std::string Method) {

  Use[Method] = 1;
  
  // book method(s)
  TString prefix = (TString) WeightsPrefix_;
  TString dir = "weights" + postfix_ + "/";
 
  TString methodName = Method + " method";
  TString weightfile = dir + prefix + "_" + TString(Method) + ".weights.xml";
  reader->BookMVA( methodName, weightfile ); 
 
}

void MVAComputer::bookInputVar(std::string var) {

  if (vars.find(var) == vars.end()) {
    
    vars[var] = 0;
    
    reader->AddVariable(var.c_str(),&vars[var]);
  
  }

}

void MVAComputer::FillVar(std::string var, Float_t val) {
  
  vars[var] = val;

}

std::map<std::string,Float_t> MVAComputer::GetMVAValues() {

  std::map<std::string,Float_t> mvaVars;

  if (Use["Likelihood"   ])   mvaVars["Likelihood"] = reader->EvaluateMVA( "Likelihood method"    );
  if (Use["LikelihoodD"  ])   mvaVars["LikelihoodD"] = reader->EvaluateMVA( "LikelihoodD method"   );
  if (Use["LikelihoodPCA"])   mvaVars["LikelihoodPCA"] = reader->EvaluateMVA( "LikelihoodPCA method" );
  if (Use["LikelihoodKDE"])   mvaVars["LikelihoodKDE"] = reader->EvaluateMVA( "LikelihoodKDE method" );
  if (Use["LikelihoodMIX"])   mvaVars["LikelihoodMIX"] = reader->EvaluateMVA( "LikelihoodMIX method" );
  if (Use["PDERS"        ])   mvaVars["PDERS"] = reader->EvaluateMVA( "PDERS method"         );
  if (Use["PDERSD"       ])   mvaVars["PDERSD"] = reader->EvaluateMVA( "PDERSD method"        );
  if (Use["PDERSPCA"     ])   mvaVars["PDERSPCA"] = reader->EvaluateMVA( "PDERSPCA method"      );
  if (Use["KNN"          ])   mvaVars["KNN"] = reader->EvaluateMVA( "KNN method"           );
  if (Use["HMatrix"      ])   mvaVars["HMatrix"] = reader->EvaluateMVA( "HMatrix method"       );
  if (Use["Fisher"       ])   mvaVars["Fisher"] = reader->EvaluateMVA( "Fisher method"        );
  if (Use["FisherG"      ])   mvaVars["FisherG"] = reader->EvaluateMVA( "FisherG method"        );
  if (Use["BoostedFisher"])   mvaVars["BoostedFisher"] = reader->EvaluateMVA( "BoostedFisher method"        );
  if (Use["LD"           ])   mvaVars["LD"] = reader->EvaluateMVA( "LD method"            );
  if (Use["MLP"          ])   mvaVars["MLP"] = reader->EvaluateMVA( "MLP method"           );
  if (Use["MLPD"         ])   mvaVars["MLPD"] = reader->EvaluateMVA( "MLPD method"           );
  if (Use["CFMlpANN"     ])   mvaVars["CFMlpANN"] = reader->EvaluateMVA( "CFMlpANN method"      );
  if (Use["TMlpANN"      ])   mvaVars["TMlpANN"] = reader->EvaluateMVA( "TMlpANN method"       );
  if (Use["BDT"          ])   mvaVars["BDT"] = reader->EvaluateMVA( "BDT method"           );
  if (Use["BDTD"         ])   mvaVars["BDTD"] = reader->EvaluateMVA( "BDTD method"          );
  if (Use["BDTG"         ])   mvaVars["BDTG"] = reader->EvaluateMVA( "BDTG method"          );
  if (Use["RuleFit"      ])   mvaVars["RuleFit"] = reader->EvaluateMVA( "RuleFit method"       );
  if (Use["SVM_Gauss"    ])   mvaVars["SVM_Gauss"] = reader->EvaluateMVA( "SVM_Gauss method"     );
  if (Use["SVM_Poly"     ])   mvaVars["SVM_Poly"] = reader->EvaluateMVA( "SVM_Poly method"      );
  if (Use["SVM_Lin"      ])   mvaVars["SVM_Lin"] = reader->EvaluateMVA( "SVM_Lin method"       );
  if (Use["FDA_MT"       ])   mvaVars["FDA_MT"] = reader->EvaluateMVA( "FDA_MT method"        );
  if (Use["FDA_GA"       ])   mvaVars["FDA_GA"] = reader->EvaluateMVA( "FDA_GA method"        );
  if (Use["Plugin"       ])   mvaVars["Plugin"] = reader->EvaluateMVA( "P_BDT method"         );
  
  return mvaVars;
											    
}


