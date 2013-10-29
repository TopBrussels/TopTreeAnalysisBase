#ifdef __CINT__
#include "TopTreeProducer/interface/TRootRun.h"
#include "TopTreeProducer/interface/TRootHLTInfo.h"
#include "TopTreeProducer/interface/TRootEvent.h"
#include "TopTreeProducer/interface/TRootParticle.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootCaloJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootJPTJet.h"
#include "TopTreeProducer/interface/TRootGenJet.h"
#include "TopTreeProducer/interface/TRootLepton.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootCaloMET.h"
#include "TopTreeProducer/interface/TRootPFMET.h"
#include "TopTreeProducer/interface/TRootTrackMET.h"
#include "TopTreeProducer/interface/TRootGenEvent.h"
#include "TopTreeProducer/interface/TRootNPGenEvent.h"
#include "TopTreeProducer/interface/TRootGenTop.h"
#include "TopTreeProducer/interface/TRootSpinCorrGen.h"
#include "TopTreeProducer/interface/TRootVertex.h"
#include "Content/interface/MCExpectation.h"
#include "Content/interface/MCObsExpectation.h"
#include "Content/interface/AnalysisEnvironment.h"
#include "Content/interface/Dataset.h"
#include "Content/interface/BinningFirstGuess.h"
#include "Content/interface/Container.h"
#else
#include "TopTreeProducer/interface/TRootRun.h"
#include "TopTreeProducer/interface/TRootHLTInfo.h"
#include "TopTreeProducer/interface/TRootEvent.h"
#include "TopTreeProducer/interface/TRootParticle.h"
#include "TopTreeProducer/interface/TRootMCParticle.h"
#include "TopTreeProducer/interface/TRootJet.h"
#include "TopTreeProducer/interface/TRootCaloJet.h"
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeProducer/interface/TRootLepton.h"
#include "TopTreeProducer/interface/TRootMuon.h"
#include "TopTreeProducer/interface/TRootElectron.h"
#include "TopTreeProducer/interface/TRootMET.h"
#include "TopTreeProducer/interface/TRootCaloMET.h"
#include "TopTreeProducer/interface/TRootPFMET.h"
#include "TopTreeProducer/interface/TRootTrackMET.h"
#include "TopTreeProducer/interface/TRootGenEvent.h"
#include "TopTreeProducer/interface/TRootNPGenEvent.h"
#include "TopTreeProducer/interface/TRootGenTop.h"
#include "TopTreeProducer/interface/TRootSpinCorrGen.h"
#include "TopTreeProducer/interface/TRootVertex.h"
#include "Content/interface/MCExpectation.h"
#include "Content/interface/MCObsExpectation.h"
#include "Content/interface/AnalysisEnvironment.h"
#include "Content/interface/Dataset.h"
#include "Content/interface/BinningFirstGuess.h"
#include "Content/interface/Container.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TRootRun;
#pragma link C++ class TRootHLTInfo;
#pragma link C++ class TRootEvent;
#pragma link C++ class TRootParticle;
#pragma link C++ class TRootMCParticle;
#pragma link C++ class TRootJet;
#pragma link C++ class TRootCaloJet;
#pragma link C++ class TRootPFJet;
#pragma link C++ class TRootJPTJet;
#pragma link C++ class TRootGenJet;
#pragma link C++ class TRootLepton;
#pragma link C++ class TRootMuon;
#pragma link C++ class TRootElectron;
#pragma link C++ class TRootMET;
#pragma link C++ class TRootCaloMET;
#pragma link C++ class TRootPFMET;
#pragma link C++ class TRootTrackMET;
#pragma link C++ class TRootGenEvent;
#pragma link C++ class TRootNPGenEvent;
#pragma link C++ class TRootGenTop;
#pragma link C++ class TRootSpinCorrGen;
#pragma link C++ class TRootVertex;
#pragma link C++ class MCExpectation;
#pragma link C++ class MCObsExpectation;
#pragma link C++ class Dataset;
#pragma link C++ class AnalysisEnvironment;
#pragma link C++ class BinningFirstGuess;
#pragma link C++ class Container;

#pragma link C++ struct TopTree::triggeredObject;

#endif
