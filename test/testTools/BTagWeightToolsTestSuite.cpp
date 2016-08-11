/* 
 * File:   TestToolsSuite
 * Author: Denys Lontkovskyi
 * email: denys.lontkovskyi@vub.ac.be/denys.lontkovskyi@cern.ch
 *
 * Created on Aug 10, 2016, 7:38:46 PM
 */

#include <string>
#include <vector>

//GoogleTest includes
#include <gtest/gtest.h>

//ROOT includes
#include "TFile.h"

//TopTreeAnalysisBase includes
#include "TopTreeProducer/interface/TRootPFJet.h"
#include "TopTreeAnalysisBase/Tools/interface/BTagWeightTools.h"
#include "TopTreeAnalysisBase/Tools/interface/BTagCalibrationStandalone.h"

// TEST GLOBALS
const char* cstrRootFileName = nullptr;     // efficiency histograms ROOT file name
const char* cstrCSVFileName = nullptr;      // reweighting functions CSV file name

class BTagWeightToolsTestSuite : public testing::Test {
protected:
    
    BTagCalibration * bTagCalib_ {nullptr};
    // BTag CSV file readers
    BTagCalibrationReader * bTagReader_ {nullptr};
    BTagCalibrationReader * bTagReaderUp_ {nullptr};
    BTagCalibrationReader * bTagReaderDown_ {nullptr};
    
    // multiple instances of BTagWeightTools
    BTagWeightTools * btwt_ {nullptr};
    BTagWeightTools * btwtUp_ {nullptr};
    BTagWeightTools * btwtDown_ {nullptr};
    
    // Efficiency histograms ROOT file handle
    TFile * rootFile_ {nullptr};
public:
    BTagWeightToolsTestSuite() {
        
        const char *cstrBTagAlgorithm = "CSVv2";
        bTagCalib_ = new BTagCalibration(cstrBTagAlgorithm,cstrCSVFileName);
        
        // Instantiate different btag systematics variations readers
        bTagReader_     = new BTagCalibrationReader(bTagCalib_,BTagEntry::OP_MEDIUM,"mujets","central");
        bTagReaderUp_   = new BTagCalibrationReader(bTagCalib_,BTagEntry::OP_MEDIUM,"mujets","up");
        bTagReaderDown_ = new BTagCalibrationReader(bTagCalib_,BTagEntry::OP_MEDIUM,"mujets","down");
        
        // Open efficiency histogram file
        rootFile_ = TFile::Open(cstrRootFileName);
        assert(rootFile_ != nullptr); //"CAN'T OPEN FILE: HistosPtEta_JECon_76X_v1.root"
        
        btwt_     = new BTagWeightTools(bTagReader_,    rootFile_,30,510,2.4,true); 
        btwtUp_   = new BTagWeightTools(bTagReaderUp_,  rootFile_,30,510,2.4,true); 
        btwtDown_ = new BTagWeightTools(bTagReaderDown_,rootFile_,30,510,2.4,true); 
    }
    
    virtual ~BTagWeightToolsTestSuite() {
        delete btwt_;
        delete btwtUp_;
        delete btwtDown_;
        
        rootFile_->Close();
        
        delete bTagReader_;
        delete bTagReaderUp_;
        delete bTagReaderDown_;
        
        delete bTagCalib_;
    }
};

TEST_F(BTagWeightToolsTestSuite, testMultBTagWeightToolsNoClash) {
    
    std::vector<TRootPFJet*>    selectedJetsLight;
    selectedJetsLight.reserve(2);
    
    // fill selectedJets with predefined values
    // light flavour jets
    // jet1
    auto jet = new TRootPFJet;
    jet->SetPtEtaPhiM (68.164, -0.860713, 0., 0.);
    jet->setHadronFlavour(0);
    jet->setBtag_combinedInclusiveSecondaryVertexV2BJetTags(0.615488);
    selectedJetsLight.push_back(jet);
    
    // jet2
    jet = new TRootPFJet;
    jet->SetPtEtaPhiM (33.492, 0.346383, 0., 0.);
    jet->setHadronFlavour(0);
    jet->setBtag_combinedInclusiveSecondaryVertexV2BJetTags(0.149428);
    selectedJetsLight.push_back(jet);

    auto btagWeight     =  btwt_->getMCEventWeight(selectedJetsLight, false);
    auto btagWeightUp   =  btwtUp_->getMCEventWeight(selectedJetsLight, false);
    auto btagWeightDown =  btwtDown_->getMCEventWeight(selectedJetsLight, false);
    
    for(auto jet: selectedJetsLight) delete jet;
    
    EXPECT_FLOAT_EQ(btagWeight, 0.99958768204);
    EXPECT_FLOAT_EQ(btagWeightUp, 0.998158081725);
    EXPECT_FLOAT_EQ(btagWeightDown, 1.00101846336);
    
    
    
    
    // fill selectedJets with predefined values
    // light flavour jets
    std::vector<TRootPFJet*>    selectedJetsHeavy;
    selectedJetsHeavy.reserve(3);
    
    //jet1
    jet = new TRootPFJet;
    jet->SetPtEtaPhiM (108.471, -1.41093, 0., 0.);
    jet->setHadronFlavour(5);
    jet->setBtag_combinedInclusiveSecondaryVertexV2BJetTags(0.991957);
    selectedJetsHeavy.push_back(jet);
    //jet2
    jet = new TRootPFJet;
    jet->SetPtEtaPhiM (66.4699, -2.09457, 0., 0.);
    jet->setHadronFlavour(4);
    jet->setBtag_combinedInclusiveSecondaryVertexV2BJetTags(0.171892);
    selectedJetsHeavy.push_back(jet);
    //jet3
    jet = new TRootPFJet;
    jet->SetPtEtaPhiM (65.2957, -1.78993, 0., 0.);
    jet->setHadronFlavour(5);
    jet->setBtag_combinedInclusiveSecondaryVertexV2BJetTags(0.165272);
    selectedJetsHeavy.push_back(jet);
    
    btagWeight     =  btwt_->getMCEventWeight(selectedJetsHeavy, false);
    btagWeightUp   =  btwtUp_->getMCEventWeight(selectedJetsHeavy, false);
    btagWeightDown =  btwtDown_->getMCEventWeight(selectedJetsHeavy, false);
    
    for(auto jet: selectedJetsHeavy) delete jet;

    EXPECT_FLOAT_EQ(btagWeight, 1.05782040166);
    EXPECT_FLOAT_EQ(btagWeightUp, 1.02176376076);
    EXPECT_FLOAT_EQ(btagWeightDown, 1.09223583994);
    
//    FAIL();
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  
  assert(argc == 3);
  
  cstrRootFileName = argv[1];   // efficiency histograms ROOT file name
  
  cstrCSVFileName = argv[2];    // reweighting functions CSV file name
  
  return RUN_ALL_TESTS();
}