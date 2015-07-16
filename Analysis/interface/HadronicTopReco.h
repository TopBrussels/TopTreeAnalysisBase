
#ifndef HadronicTopReco1_h
#define HadronicTopReco1_h


#define _USE_MATH_DEFINES
#include "TStyle.h"
#include "TPaveText.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TNtuple.h"
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVectorD.h>
#include <ctime>

#include <cmath>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <errno.h>
#include "TRandom3.h"
#include "TRandom.h"
#include "TProfile.h"
#include <iostream>
#include <map>
#include <cstdlib>

//user code
#include "TopTreeProducer/interface/TRootRun.h"
#include "TopTreeProducer/interface/TRootEvent.h"
#include "TopTreeAnalysisBase/Selection/interface/SelectionTable.h"
//#include "TopTreeAnalysisBase/Selection/interface/FourTopSelectionTable.h"
#include "TopTreeAnalysisBase/Selection/interface/Run2Selection.h"

#include "TopTreeAnalysisBase/Content/interface/AnalysisEnvironment.h"
#include "TopTreeAnalysisBase/Content/interface/Dataset.h"
#include "TopTreeAnalysisBase/Tools/interface/JetTools.h"
#include "TopTreeAnalysisBase/Tools/interface/PlottingTools.h"
#include "TopTreeAnalysisBase/Tools/interface/MultiSamplePlot.h"
#include "TopTreeAnalysisBase/Tools/interface/TTreeLoader.h"
#include "TopTreeAnalysisBase/Tools/interface/AnalysisEnvironmentLoader.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/JetCorrectorParameters.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/JetCorrectionUncertainty.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/MakeBinning.h"
#include "TopTreeAnalysisBase/MCInformation/interface/LumiReWeighting.h"
#include "TopTreeAnalysisBase/MCInformation/interface/JetPartonMatching.h"
#include "TopTreeAnalysisBase/Reconstruction/interface/MEzCalculator.h"
#include "TopTreeAnalysisBase/Tools/interface/LeptonTools.h"
#include "TopTreeAnalysisBase/Tools/interface/SourceDate.h"

#include "TopTreeAnalysisBase/Reconstruction/interface/TTreeObservables.h"

//This header file is taken directly from the BTV wiki. It contains
// to correctly apply an event level Btag SF. It is not yet on CVS
// as I hope to merge the functionality into BTagWeigtTools.h
//#include "TopTreeAnalysisBase/Tools/interface/BTagSFUtil.h"
#include "TopTreeAnalysisBase/Tools/interface/BTagWeightTools.h"
#include "TopTreeAnalysisBase/Tools/interface/JetCombiner.h"
#include "TopTreeAnalysisBase/Tools/interface/MVATrainer.h"
#include "TopTreeAnalysisBase/Tools/interface/MVAComputer.h"
#include "TopTreeAnalysisBase/Tools/interface/JetTools.h"

using namespace std;
using namespace TopTree;
using namespace reweight;
 
class HadronicTopReco{
	public:
		HadronicTopReco(TFile *fout, bool isMuon, bool isElectron, bool TrainMVA,  vector < Dataset* > datasets, string MVAmethodIn, bool isdebug, float Lumi);
		~HadronicTopReco();
		void RecoCheck(bool debug, vector<TRootMuon*> selectedMuons, vector<TRootElectron*> selectedElectrons, vector<TRootPFJet*> selectedJets);
		void Compute1st(unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets);
		void Compute2nd(unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets);
		void Compute3rd(unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets);
		void Train();
		float ReturnTopness();
		float ReturnDiTopness();
		float ReturnTriTopness();
		void ComputeMVASuccesses();
		int ReturnMVASuccesses();
		void FillDiagnosticPlots(TFile *fout, unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets);
		void SetCollections(vector<TRootPFJet*> selectJets,  vector<TRootMuon*> selectedMuons, vector<TRootElectron*> selectedElectrons, float scaleFac);
		void WriteDiagnosticPlots(TFile *fout, string pathPNG);
		float ReturnAnglet1t2();
		float ReturnAngletoplep();

	private:
		map<string,MultiSamplePlot*> MSPlot;
		bool debug;
		string leptonChoice;
        vector<TLorentzVector*> selectedLeptonTLV_JC;
        vector<TLorentzVector> mcParticlesTLV, selectedJetsTLV, mcMuonsTLV;
	    vector<TRootMCParticle*> mcParticlesMatching_;
        pair<unsigned int, unsigned int> leptonicBJet_, hadronicBJet_, hadronicWJet1_, hadronicWJet2_; //First index is the JET number, second one is the parton
        vector<TRootMCParticle*> mcParticles_flav;
        vector< pair<unsigned int, unsigned int> > JetPartonPair;
        double TriJetMass, DiJetMass;
       	float Topness, MultiTopness, DiTopness, TriTopness;
		pair<float, vector<unsigned int> > MVAvals1; //BDT discriminator value of highest ranked tri jet
		pair<float, vector<unsigned int> > MVAvals2ndPass; //BDT discriminator value of second highest ranked tri jet
		pair<float, vector<unsigned int> > MVAvals3rdPass; //BDT discriminator value of third highest ranked tri jet (if possible)
        vector<TRootPFJet*>      selectedJets2ndPass; //remaining jets after removing highest ranked tri jet
        vector<TRootPFJet*>      selectedJets3rdPass; //remaining jets after removing highest & second highest ranked tri jet
        vector<TRootPFJet*>      MVASelJets1; //the selected jets from the highest ranked tri jet
        vector<TRootPFJet*>		 MVASelJets2; //the selected jets from the second highest ranked tri jet
		JetCombiner* jetCombiner;
		TRootGenEvent* genEvt_flav;
		double bestTopMass1;
		double bestTopMass2;
		double bestTopMass2ndPass ;
		double bestTopPt;
		int nMVASuccesses;
		float scaleFactor;
		float Luminosity;
		string MVAmethod;
		unsigned int wj1, wj2, bj1, wj1_2ndpass, wj2_2ndpass, bj1_2ndpass;  //wjet1, wjet2, bjet
		float AngleT1T22ndpass;
		float AngleT1Lep;

		// MSPlot["MVA1TriJetMass"];
		// MSPlot["MVA1DiJetMass"];    
		// MSPlot["MVA1PtRat"];  
		// MSPlot["MVA1BTag"];
		// MSPlot["MVA1AnThBh"];
		// MSPlot["MVA1AnThWh1"];  
		// MSPlot["MVA1TriJet"];
		// MSPlot["MultiTopness"];
		// MSPlot["Topness"];
		// MSPlot["DiTopness"];
		// MSPlot["TriTopness"];
		// MSPlot["MVA2ndPassTriJetMass"];
		// MSPlot["MVA_AnTop1Top2"];
};

#endif

