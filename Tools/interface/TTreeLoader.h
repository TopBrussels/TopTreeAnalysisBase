#ifndef TTreeLoader_h 
#define TTreeLoader_h

#include <vector>
#include "TBranch.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TopTreeAnalysis/tinyxml/tinyxml.h"


//TopTreeAnalysis classes
#include "TopTreeAnalysis/Content/interface/Dataset.h"
#include "TopTreeAnalysis/Content/interface/AnalysisEnvironment.h"

//TopTree classes
#include "TopTreeProducer/interface/TRootVertex.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootCaloJet.h"
#include "TopTreeProducer/interface/TRootGenJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootTrackMET.h"
#include "TopTreeProducer/interface/TRootRun.h"
#include "TopTreeProducer/interface/TRootEvent.h"
#include "TopTreeProducer/interface/TRootGenEvent.h"
#include "TopTreeProducer/interface/TRootNPGenEvent.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"

class TTreeLoader{

public:

TTreeLoader();
~TTreeLoader();

void StringSeparator(string s, string sep, vector<string>& out);

int  LoadDatasets(vector<Dataset*>& v, const char* filename);/** fill the vector of Dataset while parsing a xml file */
void LoadDataset(Dataset* d, AnalysisEnvironment anaEnv);
void UnLoadDataset();
void LoadEvent(int ievent,vector<TRootVertex*>&vertex,vector<TRootMuon*>&vmuons,vector<TRootElectron*>&velectrons,vector<TRootMET*>&vmet,bool verbose=false);
TRootEvent* LoadEvent(int ievent,vector<TRootVertex*>&vertex,vector<TRootMuon*>&vmuons,vector<TRootElectron*>&velectrons,vector<TRootJet*>& vjets, vector<TRootMET*>&vmet,bool verbose=false);


 TRootGenEvent*          LoadGenEvent (int ievt, bool reloadEvent = true);
 vector<TRootGenJet*>     LoadGenJet   (int ievt, bool reloadEvent = true);
 vector<TRootMCParticle*> LoadMCPart   (int ievt, bool reloadEvent = true);
 vector<TRootTrackMET*> LoadTrackMET   (int ievt);
 
 void LoadMCEvent(int ievent, TRootGenEvent *genEvt, TRootNPGenEvent* npgenEvent, bool reloadEvent = true);
 void LoadMCEvent(int ievent, TRootGenEvent *genEvt, TRootNPGenEvent* npgenEvent, vector<TRootMCParticle*>& vmcparticles, bool reloadEvent = true);

bool EventTrigged(int itrigger); /** return true if event is trigged. This method can only be applied if LoadEvent was called first*/
bool EventPassedJSON(int runID, int lumiBlockID);

int iTrigger(string TriggerName, int runID, int iFile=0);/** return the integer corresponding to the trigger name */

private:

  TClonesArray *tcpvertex;/** TCLonesArry with primary vertex*/
  TClonesArray *tcjets;
  TClonesArray *tcmuons;
  TClonesArray *tcelectrons;
  TClonesArray *tcmets;
  TClonesArray *tctrackmets;
  TClonesArray *tcgenjets;
  TClonesArray *tcgenEvt;
  TClonesArray *tcnpgenEvt;
  TClonesArray *tcmcparticles;
  TRootRun     *runInfos;
  TRootEvent   *event;
  Dataset      *d_; /**Pointer to the current dataset */

};

#endif
