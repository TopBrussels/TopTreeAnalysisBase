#include "../interface/TTreeLoader.h"


TTreeLoader::TTreeLoader ()
{
  tcpvertex = 0;
  tcjets = 0;
  tcmuons = 0;
  tcelectrons = 0;
  tcmets = 0;
  tctrackmets = 0;
  tcgenjets = 0;
  tcgenEvt = 0;
  tcnpgenEvt = 0;
  tcmcparticles = 0;
  runInfos = 0;
  event    = 0;
}

TTreeLoader::~TTreeLoader ()
{
  //TClonesArrays
  if (tcpvertex)
    tcpvertex->Delete();
  if (tcjets)
    tcjets->Delete();
  if (tcmuons)
    tcmuons->Delete();
  if (tcelectrons)
    tcelectrons->Delete();
  if (tcmets)
    tcmets->Delete();
  if (tctrackmets)
    tctrackmets->Delete();
  if (tcgenjets)
    tcgenjets->Delete();
  if (tcgenEvt)
    tcgenEvt->Delete();
  if (tcnpgenEvt)
     tcnpgenEvt->Delete();
  if (tcmcparticles)
     tcmcparticles->Delete();
  if (runInfos)
    runInfos->Delete();
  if (event)
    event->Delete();
}

void TTreeLoader::StringSeparator(string s, string sep, vector<string>& out)
{
  string::size_type pos, pos_ex = 0;
  while ( (pos = s.find(sep, pos_ex)) != string::npos)
  {
    out.push_back(s.substr(pos_ex, pos-pos_ex));
    pos_ex = pos + 1;
  }
  out.push_back(s.substr(pos_ex));
}


int
TTreeLoader::LoadDatasets (vector < Dataset* >& datasets, const char *xmlfilename)
{
  Dataset* d = 0;
  TiXmlDocument doc (xmlfilename);
  if (!doc.LoadFile ())
    {
      cerr << "Error while loading the xml file in LoadDatasets::LoadDatasets(" << xmlfilename << ") " << endl;
      cerr << " error #" << doc.ErrorId () << " : " << doc.ErrorDesc () << endl;
      return 1;
    }
  TiXmlHandle hdl (&doc);
  TiXmlNode *node = 0;
  TiXmlElement *elem = 0;
  bool nodeFound = false;
  node = hdl.Node ();
  for( node = node->FirstChild(); node; node = node->NextSibling() )
    {
      if (node->Value () == string ("datasets"))
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
      delete node;
      delete elem;
      return 2;
    }
  if (!elem)
    {
      cerr << "The node doesn't exist" << endl;
      delete node;
      delete elem;
      return 3;
    }
  while (elem)
    {
	string name = elem->Attribute("name");
	string title = elem->Attribute("title");
	string filename = elem->Attribute("filenames");
	int add = 0;
	int ls = 1;
	int lw = 1;
	int color = 1;	
	float normf = 1.;
	float xsection = 1.;
	float xsectionError = 0.;
	float mass = -1.; 
	elem->QueryIntAttribute ("ls", &ls);
	elem->QueryIntAttribute ("lw", &lw);
	elem->QueryIntAttribute ("color", &color);
	elem->QueryFloatAttribute ("normf", &normf);
	elem->QueryFloatAttribute ("xsection", &xsection);
	elem->QueryFloatAttribute ("xsectionError", &xsectionError);
	elem->QueryFloatAttribute ("mass", &mass);
	elem->QueryIntAttribute ("add", &add);
  if (add)
  {
 	 vector<string> filenames;
	 StringSeparator(filename,string(","),filenames);
	 //create a Dataset and push it back in the vector
	 d = new Dataset(name, title, true, color, ls, lw, normf, xsection, filenames);
	 d->SetXsectionError(xsectionError);
	 d->SetMass(mass);
	 //Norm with Eff and NofEvts 
	 float PreselEff = -1.;
	 int NofPSEvts = -1;
	 elem->QueryFloatAttribute ("PreselEff", &PreselEff);
	 elem->QueryIntAttribute ("NofPSEvts", &NofPSEvts);
	 if(PreselEff>0 && NofPSEvts>0){
	  (NofPSEvts<= d->eventTree()->GetEntries())? d->SetPreselEffAndNumberOfPreselEvents(PreselEff,NofPSEvts):d->SetPreselEffAndNumberOfPreselEvents(PreselEff,d->eventTree()->GetEntries()) ;	
	 }
	 if(PreselEff>0 && NofPSEvts==-1){
	  (d->SetPreselEffAndNumberOfPreselEvents(PreselEff,d->eventTree()->GetEntries())) ;	
	 }
	 //Norm with nof of evts before preselection
	 int NofEvts = -1;
	 elem->QueryIntAttribute ("NofEvts", &NofEvts);
	 if(NofEvts!=-1)
 		(NofEvts>0 && NofEvts<= d->eventTree()->GetEntries())? d->SetOriginalNumberOfEvents(NofEvts):d->SetOriginalNumberOfEvents(d->eventTree()->GetEntries()); 
	
	//Norm with eq lumi
	float EqLumi = -1;
	elem->QueryFloatAttribute ("EqLumi", &EqLumi);
	if(EqLumi>0)
	{
	  d->SetEquivalentLuminosity(EqLumi);
	}
        // JSON File
	int useJSON = 0;
	elem->QueryIntAttribute ("useJSON", &useJSON);
	if(useJSON == 1)
	{
		string JSONFile = elem->Attribute("JSONFile");
		d->ReadJSONFile(JSONFile);
	}

	datasets.push_back(d);
    }

	elem = elem->NextSiblingElement ();	// iteration 
    }
  //delete elem;
  //delete node;
  return 0;
}

void
TTreeLoader::LoadDataset (Dataset* d, AnalysisEnvironment anaEnv)
{
  d_ = d; 
  char branchStatus[100];

  //d_->runTree()  ->SetBranchStatus ("*",0);
  //d_->eventTree()->SetBranchStatus ("*",0);

  runInfos = new TRootRun();
  d_->runTree()->SetBranchStatus("runInfos*",1);
  d_->runTree()->SetBranchAddress("runInfos",&runInfos);
  //cout<<"SetBranchAddress(runInfos,&runInfos) : "<<d_->runTree()->SetBranchAddress("runInfos",&runInfos)<<endl;

  event = new TRootEvent();
  d_->eventTree()->SetBranchStatus("Event*",1);
  d_->eventTree()->SetBranchAddress("Event",&event);
  //cout<<"SetBranchAddress(Event,&event) : "<<d_->eventTree()->SetBranchAddress("Event",&event)<<endl;

  tcpvertex = new TClonesArray ("TopTree::TRootVertex", 0);
  sprintf(branchStatus,"%s*",anaEnv.PrimaryVertexCollection.c_str());
  d_->eventTree()->SetBranchStatus(branchStatus,1);
  d_->eventTree()->SetBranchAddress(anaEnv.PrimaryVertexCollection.c_str(),&tcpvertex);

  tcmuons = new TClonesArray("TopTree::TRootMuon", 0);
  sprintf(branchStatus,"%s*",anaEnv.MuonCollection.c_str());
  d_->eventTree()->SetBranchStatus(branchStatus,1);
  d_->eventTree()->SetBranchAddress(anaEnv.MuonCollection.c_str(),&tcmuons);

  tcelectrons = new TClonesArray ("TopTree::TRootElectron", 0);
  sprintf(branchStatus,"%s*",anaEnv.ElectronCollection.c_str());
  d_->eventTree()->SetBranchStatus(branchStatus,1);
  d_->eventTree()->SetBranchAddress(anaEnv.ElectronCollection.c_str(),&tcelectrons);

  if(anaEnv.JetType == 1) tcjets = new TClonesArray ("TopTree::TRootCaloJet", 0);
  else if(anaEnv.JetType == 2) tcjets = new TClonesArray ("TopTree::TRootPFJet", 0);
  else if(anaEnv.JetType == 3) tcjets = new TClonesArray ("TopTree::TRootJPTJet", 0);
  else                         tcjets = new TClonesArray ("TopTree::TRootJet", 0);
  sprintf(branchStatus,"%s*",anaEnv.JetCollection.c_str());
  d_->eventTree()->SetBranchStatus(branchStatus,1);
  d_->eventTree()->SetBranchAddress(anaEnv.JetCollection.c_str(),&tcjets);

  if(anaEnv.METType == 1)      tcmets = new TClonesArray ("TopTree::TRootCaloMET", 0);
  else if(anaEnv.METType == 2) tcmets = new TClonesArray ("TopTree::TRootPFMET", 0);
  else if(anaEnv.METType == 3) tcmets = new TClonesArray ("TopTree::TRootMET", 0); // JPT is just TRootMET
  else                         tcmets = new TClonesArray ("TopTree::TRootMET", 0);
  sprintf(branchStatus,"%s*",anaEnv.METCollection.c_str());
  d_->eventTree()->SetBranchStatus(branchStatus,1);
  d_->eventTree()->SetBranchAddress(anaEnv.METCollection.c_str(),&tcmets);

  if (d_->Name().find("Data") < d_->Name().size() || d_->Name().find("data") < d_->Name().size() || d_->Name().find("DATA") < d_->Name().size()) {
    anaEnv.loadGenEventCollection=false;
    anaEnv.loadNPGenEventCollection=false;
    anaEnv.loadMCParticles=false;
  }

  if (anaEnv.loadMCParticles)
  {
      tcmcparticles = new TClonesArray ("TopTree::TRootMCParticle", 0);
      sprintf(branchStatus,"%s*",anaEnv.MCParticlesCollection.c_str());
      d_->eventTree()->SetBranchStatus(branchStatus,1);
      d_->eventTree()->SetBranchAddress(anaEnv.MCParticlesCollection.c_str(),&tcmcparticles);
  }

  if (anaEnv.loadGenJetCollection)
    {
      tcgenjets  = new TClonesArray ("TopTree::TRootGenJet", 0);
      sprintf(branchStatus,"%s*",anaEnv.GenJetCollection.c_str());
      d_->eventTree()->SetBranchStatus(branchStatus,1);
      d_->eventTree()->SetBranchAddress(anaEnv.GenJetCollection.c_str(),&tcgenjets);
    }
  if (anaEnv.loadGenEventCollection)
    {
      tcgenEvt = new TClonesArray ("TopTree::TRootGenEvent", 0);
      sprintf(branchStatus,"%s*",anaEnv.GenEventCollection.c_str());
      d_->eventTree()->SetBranchStatus(branchStatus,1);
      d_->eventTree()->SetBranchAddress(anaEnv.GenEventCollection.c_str(),&tcgenEvt);
    }
  if (anaEnv.loadNPGenEventCollection)
    {
      tcnpgenEvt = new TClonesArray ("TopTree::TRootNPGenEvent", 0);
      sprintf(branchStatus,"%s*",anaEnv.NPGenEventCollection.c_str());
      d_->eventTree()->SetBranchStatus(branchStatus,1);
      d_->eventTree()->SetBranchAddress(anaEnv.NPGenEventCollection.c_str(),&tcnpgenEvt);
    }
    
  if (anaEnv.loadTrackMETCollection)
   { 
      tctrackmets = new TClonesArray ("TopTree::TRootMET", 0);
      sprintf(branchStatus,"%s*",anaEnv.TrackMETCollection.c_str());
      d_->eventTree()->SetBranchStatus(branchStatus,1);
      d_->eventTree()->SetBranchAddress(anaEnv.TrackMETCollection.c_str(),&tctrackmets);
    }
    
    
/*  
  if (anaEnv.loadGenJetCollection)
    {
      genjets_br = (TBranch *) d.eventTree ()->GetBranch (anaEnv.GenJetCollection.c_str());
      tcgenjets  = new TClonesArray ("TopTree::TRootGenJet", 0);
      genjets_br->SetAddress (&tcgenjets);
    }
  if (anaEnv.loadGenEventCollection)
    {
      genEvt_br = (TBranch *) d.eventTree ()->GetBranch (anaEnv.GenEventCollection.c_str());
      tcgenEvt = new TClonesArray ("TopTree::TRootGenEvent", 0);
      genEvt_br->SetAddress (&tcgenEvt);
    }
  if (anaEnv.loadNPGenEventCollection)
    {
      npgenEvt_br = (TBranch *) d.eventTree ()->GetBranch (anaEnv.NPGenEventCollection.c_str());
      tcnpgenEvt = new TClonesArray ("TopTree::TRootNPGenEvent", 0);
      npgenEvt_br->SetAddress (&tcnpgenEvt);
    }
  
  run_br = (TBranch *) d.runTree ()->GetBranch ("runInfos");
  run_br->SetAddress (&runInfos);
  event_br = (TBranch *) d.eventTree ()->GetBranch ("Event");
  event_br->SetAddress (&event);
*/
}

void
TTreeLoader::UnLoadDataset (){
  if(tcpvertex)     tcpvertex    ->Delete();
  if(tcmuons)       tcmuons      ->Delete();
  if(tcelectrons)   tcelectrons  ->Delete();
  if(tcjets)        tcjets       ->Delete();
  if(tcmets)        tcmets       ->Delete();
  if(tctrackmets)   tctrackmets  ->Delete();
  if(tcmcparticles) tcmcparticles->Delete();
  if(tcgenEvt)      tcgenEvt     ->Delete();
  if(tcgenjets)     tcgenjets    ->Delete();
  if(tcnpgenEvt)    tcnpgenEvt   ->Delete();
}

void
TTreeLoader::LoadEvent(int ievt, vector<TRootVertex*>& vertex, vector < TRootMuon* >& init_muons, vector < TRootElectron* >& init_electrons, vector < TRootMET* >& mets, bool verbose)
{
//  Long64_t ientry = d_->eventTree ()->LoadTree(ievt);
//  if (ientry < 0)
//    return;
  d_->eventTree ()->GetEntry (ievt);

  //clear vectors   
  vertex.clear();
  init_muons.clear ();
  init_electrons.clear ();
  mets.clear ();

  //fill vectors
  for (int i = 0; i < tcpvertex->GetEntriesFast (); i++)
    vertex.push_back ((TRootVertex *) tcpvertex->At (i));
  for (int i = 0; i < tcmuons->GetEntriesFast (); i++)
    init_muons.push_back ((TRootMuon *) tcmuons->At (i));
  for (int i = 0; i < tcelectrons->GetEntriesFast (); i++)
    init_electrons.push_back ((TRootElectron *) tcelectrons->At (i));
  for (int i = 0; i < tcmets->GetEntriesFast (); i++)
    mets.push_back ((TRootMET *) tcmets->At (i));
 
  if (verbose)
    {
      cout << "In event: " << ievt << endl;
      cout <<" Nof vertices "<<vertex.size() <<endl;
      cout << "Nof muons " << init_muons.size () << endl;
      cout << "Nof electrons " << init_electrons.size () << endl;
    }
}

TRootEvent*
TTreeLoader::LoadEvent (int ievt, vector<TRootVertex*>& vertex, vector < TRootMuon* >& init_muons, vector < TRootElectron* >& init_electrons, vector < TRootJet* >& init_jets, vector < TRootMET* >& mets, bool verbose){
	LoadEvent(ievt, vertex, init_muons, init_electrons, mets, verbose);
  	init_jets.clear ();

  	for (int i = 0; i < tcjets->GetEntriesFast (); i++) {
	  //const TRootCaloJet* CaloJet = static_cast<const TRootCaloJet*>((TRootJet*)(tcjets->At(0)));
	
	  //if (i==0)
	  //  cout << "EMF " << CaloJet->ecalEnergyFraction() << " fHPD" << CaloJet->fHPD() << " n90Hits " << CaloJet->n90Hits() <<endl;

	  init_jets.push_back( (TRootJet *) tcjets->At (i));

	  //const TRootCaloJet* CaloJet2 = static_cast<const TRootCaloJet*>((TRootJet*) init_jets[0]);
	  
	  //if (i==0)
	  //  cout << "NOPOINTER EMF " << CaloJet2->ecalEnergyFraction() << " fHPD" << CaloJet2->fHPD() << " n90Hits " << CaloJet2->n90Hits() <<endl;
	}

        if(verbose) cout << "Nof jets " << init_jets.size () << endl;
	return event;
}

TRootGenEvent*
TTreeLoader::LoadGenEvent (int ievt, bool reloadEvent)
{
  if (reloadEvent)
    d_->eventTree ()->GetEntry (ievt);
  if (tcgenEvt && tcgenEvt->GetEntriesFast () == 1)
    return (TRootGenEvent *) tcgenEvt->At (0);
  return 0;
}

vector<TRootTrackMET*>
TTreeLoader::LoadTrackMET(int ievt)
{
  vector<TRootTrackMET*> vtrackmets;
  if (tctrackmets) {
    for(int i = 0; i < tctrackmets->GetEntriesFast (); i++)
      vtrackmets.push_back( (TRootTrackMET *) tctrackmets->At (i));
  }
  return vtrackmets;
}

 
    
vector<TRootGenJet*>
TTreeLoader::LoadGenJet(int ievt, bool reloadEvent)
{
  if (reloadEvent)
    d_->eventTree ()->GetEntry (ievt);
  vector<TRootGenJet*> vgenjets;
  if (tcgenjets) {
    for(int i = 0; i < tcgenjets->GetEntriesFast (); i++)
      vgenjets.push_back( (TRootGenJet *) tcgenjets->At (i));
  }
  return vgenjets;
}

vector<TRootMCParticle*>
TTreeLoader::LoadMCPart(int ievt, bool reloadEvent)
{
  if (reloadEvent)
    d_->eventTree ()->GetEntry (ievt);
  vector<TRootMCParticle*> vmcpart;
  if (tcmcparticles) {
    for(int i = 0; i < tcmcparticles->GetEntriesFast (); i++)
      vmcpart.push_back( (TRootMCParticle *) tcmcparticles->At (i));
  }
  return vmcpart;
}

void
TTreeLoader::LoadMCEvent (int ievt, TRootGenEvent * genEvt, TRootNPGenEvent * npgenEvt, bool reloadEvent)
{
  if (reloadEvent)
    d_->eventTree ()->GetEntry (ievt);
  if (tcgenEvt && tcgenEvt->GetEntriesFast () == 1)
    genEvt = (TRootGenEvent *) tcgenEvt->At (0);
  if (tcnpgenEvt && tcnpgenEvt->GetEntriesFast () == 1)
    npgenEvt = (TRootNPGenEvent *) tcnpgenEvt->At (0);
}

void
TTreeLoader::LoadMCEvent (int ievt, TRootGenEvent * genEvt, TRootNPGenEvent * npgenEvt, vector < TRootMCParticle* >& vmcparticles, bool reloadEvent)
{ 
  if (reloadEvent)
    d_->eventTree ()->GetEntry (ievt);
  vmcparticles.clear ();
  if (tcgenEvt && tcgenEvt->GetEntriesFast () == 1)
    genEvt = (TRootGenEvent *) tcgenEvt->At (0);
  if (tcnpgenEvt && tcnpgenEvt->GetEntriesFast () == 1)
    npgenEvt = (TRootNPGenEvent *) tcnpgenEvt->At (0);
  if (tcmcparticles) {
    for(int i = 0; i < tcmcparticles->GetEntriesFast (); i++)
      vmcparticles.push_back( (TRootMCParticle *) tcmcparticles->At (i));
  }
}

bool
TTreeLoader::EventPassedJSON(int runID, int lumiBlockID)
{
  bool eventPassed = false;
  if(d_->Name() == "data" || d_->Name() == "DATA" || d_->Name() == "Data")
  {
    vector< vector<int> > JSONInfo = d_->JSONInfo();
    for(unsigned int k=0; k<JSONInfo.size(); k++)
    {
      if(runID == JSONInfo[k][0] && lumiBlockID >= JSONInfo[k][1] && lumiBlockID <= JSONInfo[k][2])
        eventPassed = true;
    }
  }
  else eventPassed = true;
  return eventPassed;
}

int
TTreeLoader::iTrigger (string triggerName, int runID, int iFile)
{ 
  d_->runTree()->GetEntry(iFile);
  
  if (runInfos == 0) return -9999;
  return runInfos->getHLTinfo(runID).hltPath(triggerName);
}

bool TTreeLoader::EventTrigged (int itrigger)
{
  return (event->trigHLT (itrigger));
}
