#include "../interface/AnalysisEnvironmentLoader.h"

AnalysisEnvironmentLoader::AnalysisEnvironmentLoader (AnalysisEnvironment& AnaEnv, const char *filename)
{
  TiXmlDocument doc (filename);
  if (!doc.LoadFile ())
    {
      cerr << "Error while loading the xml file in AnalysisEnvionment::Load(" << filename << ") " << endl;
      cerr << " error #" << doc.ErrorId () << " : " << doc.ErrorDesc () << endl;
      return;
    }
  TiXmlHandle hdl (&doc);
  TiXmlNode *node = 0;
  TiXmlElement *elem = 0;
  bool nodeFound = false;
  node = hdl.Node ();
  for( node = node->FirstChild(); node; node = node->NextSibling() )
    {
      if (node->Value () == string ("analysis"))
	{
	  nodeFound = true;
	  break;
	}
    }

  if (nodeFound)
    elem = node->FirstChildElement ();
  else
    {
      cerr << "The node doesn't exist" << endl;
      //delete node;
      //delete elem;
      return;
    }
  if (!elem)
    {
      cerr << "The node doesn't exist" << endl;
      //delete node;
      //delete elem;
      return;
    }
  while (elem)
    {
      string type = elem->Attribute ("type");
      if (type == string ("Collections"))
	    {
	      elem->QueryIntAttribute ("JetType", &AnaEnv.JetType);
	      elem->QueryIntAttribute ("METType", &AnaEnv.METType);
	      AnaEnv.PrimaryVertexCollection = elem->Attribute("PVCollection");
	      AnaEnv.JetCollection = elem->Attribute("JetCollection");
	      AnaEnv.METCollection = elem->Attribute("METCollection");
	      AnaEnv.MuonCollection = elem->Attribute("MuonCollection");
	      AnaEnv.ElectronCollection = elem->Attribute("ElectronCollection");
	      AnaEnv.GenJetCollection   = elem->Attribute("GenJetCollection");
	      AnaEnv.TrackMETCollection = elem->Attribute("TrackMETCollection");
	      AnaEnv.GenEventCollection = elem->Attribute("GenEventCollection");
	      AnaEnv.NPGenEventCollection = elem->Attribute("NPGenEventCollection");
	      AnaEnv.MCParticlesCollection = elem->Attribute("MCParticlesCollection");
	      int loadGenJetCollection_ = 0;
	      elem->QueryIntAttribute ("loadGenJetCollection", &loadGenJetCollection_);
	      (loadGenJetCollection_==0)? AnaEnv.loadGenJetCollection = false: AnaEnv.loadGenJetCollection = true; 
	      int loadGenEventCollection_ = 0;
	      elem->QueryIntAttribute ("loadGenEventCollection", &loadGenEventCollection_);
	      (loadGenEventCollection_==0)? AnaEnv.loadGenEventCollection = false: AnaEnv.loadGenEventCollection = true; 
	      int loadNPGenEventCollection_ = 0;
	      elem->QueryIntAttribute ("loadNPGenEventCollection", &loadNPGenEventCollection_);
	      (loadNPGenEventCollection_==0)? AnaEnv.loadNPGenEventCollection = false: AnaEnv.loadNPGenEventCollection = true;
	      int loadTrackMETCollection_ = 0;
	      elem->QueryIntAttribute ("loadTrackMET", &loadTrackMETCollection_);
	      (loadTrackMETCollection_==0)? AnaEnv.loadTrackMETCollection = false: AnaEnv.loadTrackMETCollection = true;
	      int loadMCParticles_ = 0;
	      elem->QueryIntAttribute ("loadMCParticles", &loadMCParticles_);
	      (loadMCParticles_==0)? AnaEnv.loadMCParticles = false: AnaEnv.loadMCParticles = true;
      }
      if (type == string ("Conditions"))
	{
	  int isMC_ = 0;
	  elem->QueryIntAttribute ("isMC", &isMC_);
	  (isMC_==0)? AnaEnv.isMC = false: AnaEnv.isMC = true;
	  int isSMPseudo_ = 0;
	  elem->QueryIntAttribute ("isSMPseudo", &isSMPseudo_);
          (isSMPseudo_==0)? AnaEnv.isSMPseudo = false: AnaEnv.isSMPseudo = true;
	  elem->QueryIntAttribute ("IntToCut", &AnaEnv.IntToCut);
	  int Vars_ByFile_ = 0;
	  elem->QueryIntAttribute ("Vars_ByFile", &Vars_ByFile_);
          (Vars_ByFile_==0)? AnaEnv.Vars_ByFile = false: AnaEnv.Vars_ByFile = true; 
	  elem->QueryIntAttribute ("Verbose", &AnaEnv.Verbose);
	  elem->QueryFloatAttribute ("Luminosity", &AnaEnv.Luminosity);
	  elem->QueryFloatAttribute ("JES", &AnaEnv.JES);
	  int doABCD_ = 0;
	  elem->QueryIntAttribute ("doABCD", &doABCD_);
	  (doABCD_==0)? AnaEnv.doABCD = false: AnaEnv.doABCD = true; 
	  int doVJEstim_ = 0;
	  elem->QueryIntAttribute ("doVJEstim", &doVJEstim_);
	  (doVJEstim_==0)? AnaEnv.doVJEstim = false: AnaEnv.doVJEstim = true; 
	  int doVJEstPE_ = 0;
	  elem->QueryIntAttribute ("doVJEstPE", &doVJEstPE_);
	  (doVJEstPE_==0)? AnaEnv.doVJEstPE = false: AnaEnv.doVJEstPE = true; 
	  int doTtJEstim_ = 0;
	  elem->QueryIntAttribute ("doTtJEstim", &doTtJEstim_);
	  (doTtJEstim_==0)? AnaEnv.doTtJEstim = false: AnaEnv.doTtJEstim = true; 
	  int doTemplComp_ = 0;
	  elem->QueryIntAttribute ("doTemplComp", &doTemplComp_);
	  (doTemplComp_==0)? AnaEnv.doTemplComp = false: AnaEnv.doTemplComp = true; 
	  elem->QueryIntAttribute ("nPseudoExp", &AnaEnv.nPseudoExp);
	  elem->QueryIntAttribute ("nPseudoSession", &AnaEnv.nPseudoSession);
	  AnaEnv.VarsFile = elem->Attribute("VarsFile");
	  elem->QueryIntAttribute ("MCRound", &AnaEnv.MCRound);
	  int runonTTrees_ = 0;
	  elem->QueryIntAttribute ("runonTTrees", &runonTTrees_);
	  (runonTTrees_==0)? AnaEnv.runonTTrees = false: AnaEnv.runonTTrees = true;
	  elem->QueryIntAttribute ("Systematics", &AnaEnv.Systematics);
	 // elem->QueryStringAttribute("SMPseudo_Point", & AnaEnv.SMPseudo_Point);
	}
      if (type == string ("CrossSection"))
	{
	  AnaEnv.MCExpFilename = elem->Attribute("MCExpFilename");
	  elem->QueryFloatAttribute ("LuminosityError", &AnaEnv.LuminosityError);
	  elem->QueryFloatAttribute ("TriggerEff", &AnaEnv.TriggerEff);
	  elem->QueryFloatAttribute ("SkimEff", &AnaEnv.SkimEff);
	  elem->QueryFloatAttribute ("MuonSelEff", &AnaEnv.MuonSelEff);
	  elem->QueryFloatAttribute ("SecondLeptonVetoEff", &AnaEnv.SecondLeptonVetoEff);
	  elem->QueryFloatAttribute ("JetSelEff", &AnaEnv.JetSelEff);
	  elem->QueryFloatAttribute ("NofSingleTopEvts", &AnaEnv.NofSingleTopEvts);
	  elem->QueryFloatAttribute ("TriggerEffError", &AnaEnv.TriggerEffError);
	  elem->QueryFloatAttribute ("SkimEffError", &AnaEnv.SkimEffError);
	  elem->QueryFloatAttribute ("MuonSelEffError", &AnaEnv.MuonSelEffError);
	  elem->QueryFloatAttribute ("SecondLeptonVetoEffError", &AnaEnv.SecondLeptonVetoEffError);
	  elem->QueryFloatAttribute ("JetSelEffError", &AnaEnv.JetSelEffError);
	  elem->QueryFloatAttribute ("JetSelEffError", &AnaEnv.JetSelEffError);
	  elem->QueryFloatAttribute ("NofSingleTopEvtsError", &AnaEnv.NofSingleTopEvtsError);
        }
      if (type == string ("Search"))
        {
	  int doBkgEstim_ = 0;
	  elem->QueryIntAttribute ("doBkgEstim", &doBkgEstim_);
	  (doBkgEstim_==0)? AnaEnv.doBkgEstim = false: AnaEnv.doBkgEstim = true; 
	  int doDumpPseudoExpInfoInTTree_ = 0;
	  elem->QueryIntAttribute ("doDumpPseudoExpInfoInTTree", &doDumpPseudoExpInfoInTTree_);
	  (doDumpPseudoExpInfoInTTree_==0)? AnaEnv.doDumpPseudoExpInfoInTTree = false: AnaEnv.doDumpPseudoExpInfoInTTree = true; 
	  AnaEnv.DumpTreeName = elem->Attribute("DumpTreeName");
	  string d_string = elem->Attribute("FractionHWEvts"); 
	  StringSeparator(d_string,string(","),AnaEnv.FractionHWEvts);
	}
      if (type == string ("Selection"))
	{
	  elem->QueryIntAttribute ("PVertexNdofCut", &AnaEnv.PVertexNdofCut);
	  elem->QueryFloatAttribute ("PVertexZCut", &AnaEnv.PVertexZCut);
	  elem->QueryFloatAttribute ("PVertexRhoCut", &AnaEnv.PVertexRhoCut);
	  elem->QueryFloatAttribute ("ElectronPtCut", &AnaEnv.ElectronPtCut);
	  elem->QueryFloatAttribute ("ElectronEtaCut", &AnaEnv.ElectronEtaCut);
	  elem->QueryFloatAttribute ("ElectronRelIsoCut", &AnaEnv.ElectronRelIsoCut);
	  elem->QueryFloatAttribute ("MuonPtCutSR", &AnaEnv.MuonPtCutSR);
	  elem->QueryFloatAttribute ("MuonEtaCutSR", &AnaEnv.MuonEtaCutSR);
	  elem->QueryFloatAttribute ("MuonRelIsoCutSR", &AnaEnv.MuonRelIsoCutSR);
	  elem->QueryFloatAttribute ("MuonPtCutVetoSR", &AnaEnv.MuonPtCutVetoSR);
	  elem->QueryFloatAttribute ("MuonEtaCutVetoSR", &AnaEnv.MuonEtaCutVetoSR);
	  elem->QueryFloatAttribute ("MuonRelIsoCutVetoSR", &AnaEnv.MuonRelIsoCutVetoSR);
	  elem->QueryIntAttribute ("MuonNHitsCutSR", &AnaEnv.MuonNHitsCutSR);
	  elem->QueryFloatAttribute ("MuonChi2CutSR", &AnaEnv.MuonChi2CutSR);
	  elem->QueryFloatAttribute ("MuonECALVetoSR", &AnaEnv.MuonECALVetoSR);
	  elem->QueryFloatAttribute ("MuonHCALVetoSR", &AnaEnv.MuonHCALVetoSR);
	  elem->QueryFloatAttribute ("MuonD0CutSR", &AnaEnv.MuonD0CutSR);
	  elem->QueryFloatAttribute ("MuonDRJetsCut", &AnaEnv.MuonDRJetsCut);
	  elem->QueryFloatAttribute ("JetsPtCutSR", &AnaEnv.JetsPtCutSR);
	  elem->QueryFloatAttribute ("JetsEtaCutSR", &AnaEnv.JetsEtaCutSR);
	  elem->QueryIntAttribute ("NofJets", &AnaEnv.NofJets);
	  elem->QueryIntAttribute ("NofJetBins", &AnaEnv.NofJetBins);
	  int applyJetID_ = 0;
	  elem->QueryIntAttribute ("applyJetID", &applyJetID_);
	  (applyJetID_==0)? AnaEnv.applyJetID = false: AnaEnv.applyJetID = true; 
	  elem->QueryFloatAttribute ("JetEMFCut", &AnaEnv.JetEMFCut);
	  elem->QueryIntAttribute ("n90HitsCut", &AnaEnv.n90HitsCut);
	  elem->QueryFloatAttribute ("fHPDCut", &AnaEnv.fHPDCut);
	}
      if (type == string ("CRForTtbarEstimation"))
	{
	  elem->QueryFloatAttribute ("MuonPtCutCR", &AnaEnv.MuonPtCutCR);
	  elem->QueryFloatAttribute ("MuonEtaCutCR", &AnaEnv.MuonEtaCutCR);
	  elem->QueryFloatAttribute ("MuonRelIsoCutCR", &AnaEnv.MuonRelIsoCutCR);
	  elem->QueryFloatAttribute ("JetsPtCutCR", &AnaEnv.JetsPtCutCR);
	  elem->QueryFloatAttribute ("JetsEtaCutCR", &AnaEnv.JetsEtaCutCR);
	  elem->QueryFloatAttribute ("MblCut", &AnaEnv.MblCut);
	  elem->QueryFloatAttribute ("DRBBCut", &AnaEnv.DRBBCut);
	  elem->QueryFloatAttribute ("HTBBCut", &AnaEnv.HTBBCut);
	  elem->QueryIntAttribute   ("BtagAlgo_ttjEst", &AnaEnv.BtagAlgo_ttjEst);
	  elem->QueryFloatAttribute ("BtagDiscriCut_ttjEst", &AnaEnv.BtagDiscriCut_ttjEst);
	  //elem->QueryIntAttribute ("btagAlgo", &AnaEnv.btagAlgo);
	  //elem->QueryFloatAttribute ("btagDiscriCut", &AnaEnv.btagDiscriCut);
	  elem->QueryFloatAttribute ("NREvtFraction", &AnaEnv.NREvtFraction);
	}
      if (type == string ("CRForABCDEstimation"))
	{
	  elem->QueryIntAttribute ("NXbinsABCD", &AnaEnv.NXbinsABCD);
	  elem->QueryIntAttribute ("NYbinsABCD", &AnaEnv.NYbinsABCD);
	  elem->QueryFloatAttribute ("XbinMinABCD", &AnaEnv.XbinMinABCD);
	  elem->QueryFloatAttribute ("XbinMaxABCD", &AnaEnv.XbinMaxABCD);
	  elem->QueryFloatAttribute ("YbinMinABCD", &AnaEnv.YbinMinABCD);
	  elem->QueryFloatAttribute ("YbinMaxABCD", &AnaEnv.YbinMaxABCD);
	  elem->QueryFloatAttribute ("cutXmin", &AnaEnv.cutXmin);
	  elem->QueryFloatAttribute ("cutX0", &AnaEnv.cutX0);
	  elem->QueryFloatAttribute ("cutX1", &AnaEnv.cutX1);
	  elem->QueryFloatAttribute ("cutXmax", &AnaEnv.cutXmax);
	  elem->QueryFloatAttribute ("cutYmin", &AnaEnv.cutYmin);
	  elem->QueryFloatAttribute ("cutY0", &AnaEnv.cutY0);
	  elem->QueryFloatAttribute ("cutY1", &AnaEnv.cutY1);
	  elem->QueryFloatAttribute ("cutYmax", &AnaEnv.cutYmax);
	  elem->QueryIntAttribute ("region", &AnaEnv.region);
	}
      if (type == string ("ParamForVJetEstimation"))
	{
	  elem->QueryIntAttribute ("BtagAlgo_vjEst", &AnaEnv.BtagAlgo_vjEst);
	  elem->QueryIntAttribute ("NofBtagWorkingPoint_vjEst", &AnaEnv.NofBtagWorkingPoint_vjEst);
	  string wp = elem->Attribute("BtagWorkingPoint_vjEst");
    delete AnaEnv.BtagWorkingPoint_vjEst;
	  AnaEnv.BtagWorkingPoint_vjEst = new float[AnaEnv.NofBtagWorkingPoint_vjEst];
	  BtagWPLoader(AnaEnv.NofBtagWorkingPoint_vjEst, wp, string(","), AnaEnv.BtagWorkingPoint_vjEst);
	  //elem->QueryIntAttribute ("NofIterationsVJestEstim", &AnaEnv.NofIterationsVJestEstim);
	  int useMJLE_ = 0;
	  elem->QueryIntAttribute ("useMJLE", &useMJLE_);
	  (useMJLE_==0)? AnaEnv.useMJLE = false: AnaEnv.useMJLE = true; 
	  int useUnBinMLE_ = 0;
	  elem->QueryIntAttribute ("useUnBinMLE", &useUnBinMLE_);
	  (useUnBinMLE_==0)? AnaEnv.useUnBinMLE = false: AnaEnv.useUnBinMLE = true; 
  	  AnaEnv.MinMethod = elem->Attribute("MinMethod");
  	  AnaEnv.MinOption = elem->Attribute("MinOption");
  	  elem->QueryIntAttribute ("NVJetPE",&AnaEnv.NVJetPE);
	  AnaEnv.VJEstFixParam.clear();
	  string sVJEstFixParam = elem->Attribute("VJEstFixParam");
	  StringSeparator(sVJEstFixParam,string(","),AnaEnv.VJEstFixParam);
	  AnaEnv.EffEbsel.clear();
	  string sEffEbsel = elem->Attribute("EffEbsel");
	  vector<string> sEffEbsel_jets;
	  StringSeparator(sEffEbsel,string("/"),sEffEbsel_jets);
	  for(unsigned int se=0;se<sEffEbsel_jets.size();se++){
	  	vector<float> tempv;
		StringSeparator(sEffEbsel_jets[se],string(","),tempv);
	  	AnaEnv.EffEbsel.push_back(tempv);
	  }
	  string sTagEff = elem->Attribute("TagEffInit");
	  vector<string> sTagEff_jets;
	  StringSeparator(sTagEff,string("/"),sTagEff_jets);
	  AnaEnv.TagEffInit.clear();
	  for(unsigned int st=0;st<sTagEff_jets.size();st++){
	  	vector<vector<float> > tempvv;//vector of vector
	        vector<string> sTagEff_jets_wp;
	        StringSeparator(sTagEff_jets[st],string("-"),sTagEff_jets_wp);
	  	for(unsigned int stw=0;stw<sTagEff_jets_wp.size();stw++){
			vector<float> tempv;//vector
	  		StringSeparator(sTagEff_jets_wp[stw],string(","),tempv);
			tempvv.push_back(tempv);
		}
		AnaEnv.TagEffInit.push_back(tempvv);
	  }
	  AnaEnv.NVlikeInit.clear();
	  string sNVlikeInit = elem->Attribute("NVlikeInit");
	  StringSeparator(sNVlikeInit,string("/"),AnaEnv.NVlikeInit);
	  AnaEnv.NTTlikeInit.clear();
	  string sNTTlikeInit = elem->Attribute("NTTlikeInit");
	  StringSeparator(sNTTlikeInit,string("/"),AnaEnv.NTTlikeInit);
	  elem->QueryIntAttribute ("NofIterationsVJestShapeEstim", &AnaEnv.NofIterationsVJestShapeEstim);
	}
      if (type == string ("Observables"))
	{
	  elem->QueryIntAttribute ("runOnObsByString", &AnaEnv.runOnObsByString);
	  string s = elem->Attribute("listOfObsInts");
	  StringSeparator(s,string(","),AnaEnv.listOfObsInts);
	  string ss = elem->Attribute("listOfObsStrings");
	  StringSeparator(ss,string(","),AnaEnv.listOfObsStrings);
          AnaEnv.binningFile = string(elem->Attribute("binning"));
	  elem->QueryIntAttribute ("bins", &AnaEnv.nbins);
	  elem->QueryIntAttribute("eventsperbin", & AnaEnv.eventsperbin);
	  elem->QueryFloatAttribute("EpsilonValue", & AnaEnv.EpsilonValue);
	  elem->QueryFloatAttribute("Correl_cut", & AnaEnv.Correl_cut);
	}
      elem = elem->NextSiblingElement ();	// iteration
    }
  //delete node;
  //delete elem;
}
void AnalysisEnvironmentLoader::StringSeparator(string s, string sep, vector<int>& out)
{
  string::size_type pos, pos_ex = 0; 
  while ( (pos = s.find(sep, pos_ex)) != string::npos)
  {
  	string a = s.substr(pos_ex, pos-pos_ex);
	out.push_back(atoi(a.c_str()));
	pos_ex = pos + 1;
  }
  string a = s.substr(pos_ex);
  out.push_back(atoi(a.c_str()));
}

void AnalysisEnvironmentLoader::StringSeparator(string s, string sep, vector<float>& out)
{
  string::size_type pos, pos_ex = 0; 
  while ( (pos = s.find(sep, pos_ex)) != string::npos)
  {
  	string a = s.substr(pos_ex, pos-pos_ex);
	out.push_back(atof(a.c_str()));
	pos_ex = pos + 1;
  }
  string a = s.substr(pos_ex);
  out.push_back(atof(a.c_str()));
}

void AnalysisEnvironmentLoader::StringSeparator(string s, string sep, vector<string>& out)
{
  string::size_type pos, pos_ex = 0; 
  while ( (pos = s.find(sep, pos_ex)) != string::npos)
  {
	out.push_back(s.substr(pos_ex, pos-pos_ex));
	pos_ex = pos + 1;
  }
  out.push_back(s.substr(pos_ex));
}
		  
void AnalysisEnvironmentLoader::BtagWPLoader(int nbofbtag, string s, string sep, float* out)
{
  string::size_type pos, pos_ex = 0;
  int loop = 0;
  while ( (pos = s.find(sep, pos_ex)) != string::npos && loop<nbofbtag)
  {
	//cout<<"Btagging wp "<<loop<<" = "<<atof(s.substr(pos_ex, pos-pos_ex).c_str())<<endl;
	out[loop] = atof(s.substr(pos_ex, pos-pos_ex).c_str());
	loop++;
	pos_ex = pos + 1;
  }
  //cout<<"Btagging wp "<<loop<<" = "<<atof(s.substr(pos_ex).c_str())<<endl;
  out[loop] = atof(s.substr(pos_ex).c_str());
}

AnalysisEnvironmentLoader::~AnalysisEnvironmentLoader ()
{;
}
