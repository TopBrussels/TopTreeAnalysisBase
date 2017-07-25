#include "../interface/JetCombiner.h"

bool sortMVAValues (MVAValues f1, MVAValues f2) {
    return f1.MVAValue > f2.MVAValue;
}

struct HighestCVSBtag {
    bool operator()( TRootPFJet* j1, TRootPFJet* j2 ) const {
        return j1->btag_combinedInclusiveSecondaryVertexV2BJetTags() > j2->btag_combinedInclusiveSecondaryVertexV2BJetTags();
    }
};

struct HighestPt {
    bool operator()( TRootPFJet* j1, TRootPFJet* j2 ) const {
        return j1->Pt() > j2->Pt();
    }
};



bool sortFloat (float f1, float f2) {
    return f1 > f2;
}

JetCombiner::JetCombiner(bool trainMVA, float Luminosity, const vector<Dataset*>& datasets, string MVAMethod, bool Tprime, string MVAfilePostfix, string postfix) {
    Tprime_ = Tprime;
    MVAfilePostfix_ = MVAfilePostfix;
    postfix_ = postfix;

    trainMVA_ = trainMVA;
    trainer_ = 0;
    computer_ = 0;

    EndJobWasRun_ = false;

    Luminosity_ = Luminosity;
    datasets_ = datasets;

    string MVAPrefix = "JetCombTrainer";

    string MVAOut = "MVAOutput";

    if(!Tprime_) {
        MVAOut = MVAOut+"_TTbarJES.root";
    }
    else {
        MVAOut = MVAOut+"_Tprime"+postfix_+".root";
    }

    if(trainMVA_) {
        cout << "The MVA stuff will be trained!" << endl;
        trainer_ = new MVATrainer(MVAMethod,MVAPrefix,MVAOut);
        trainer_->addMethod("BDT");
        trainer_->addMethod("Likelihood");
        trainer_->addMethod("LikelihoodD");
        // trainer_->addMethod("LikelihoodPCA");
        // trainer_->addMethod("MLP");
        // trainer_->addMethod("MLPD");
        // trainer_->addMethod("RuleFit");
        trainer_->bookInputVar("btag");
        // trainer_->bookInputVar("btag2");
        // trainer_->bookInputVar("btag3");
        trainer_->bookInputVar("ThPtOverSumPt");
        trainer_->bookInputVar("AngleThWh");
        // trainer_->bookInputVar("AngleBlMu");
        // trainer_->bookInputVar("AngleThBl");
        trainer_->bookInputVar("AngleThBh");
        // trainer_->bookInputVar("AngleThMu");
        // if(Tprime_)
        trainer_->bookInputVar("HadrWmass");
        trainer_->bookInputVar("TopMass");
        // trainer_->bookInputVar("delR");
        // trainer_->bookInputVar("sumE");
    }

    else {
        cout << "The MVA stuff will be computed!" << endl;

        // First prepare some plots
        MSPlot_["MVA_input_btag"] = new MultiSamplePlot(datasets, "MVA_input_btag", 20, -0.0001, 1, "btag");
        MSPlot_["MVA_input_ThPtOverSumPt"] = new MultiSamplePlot(datasets, "MVA_input_ThPtOverSumPt", 20, -0.0001, 1, "ThPtOverSumPt");
        MSPlot_["MVA_input_AngleThWh"] = new MultiSamplePlot(datasets, "MVA_input_AngleThWh", 16, -0.0001, 3.2, "AngleThWh");
        MSPlot_["MVA_input_AngleBlMu"] = new MultiSamplePlot(datasets, "MVA_input_AngleBlMu", 16, -0.0001, 3.2, "AngleBlMu");
        MSPlot_["MVA_input_AngleThBl"] = new MultiSamplePlot(datasets, "MVA_input_AngleThBl", 16, -0.0001, 3.2, "AngleThBl");
        MSPlot_["MVA_input_AngleThBh"] = new MultiSamplePlot(datasets, "MVA_input_AngleThBh", 16, -0.0001, 3.2, "AngleThBh");
        MSPlot_["MVA_input_AngleThMu"] = new MultiSamplePlot(datasets, "MVA_input_AngleThMu", 16, -0.0001, 3.2, "AngleThMu");
        MSPlot_["MVA_input_HadrWmass"] = new MultiSamplePlot(datasets, "MVA_input_HadrWmass", 50, -0.0001, 200, "HadrWmass");

        MSPlot_["maxMVA_btag"] = new MultiSamplePlot(datasets, "MVA_btag", 10, -0.0001, 1, "btag");
        MSPlot_["maxMVA_ThPtOverSumPt"] = new MultiSamplePlot(datasets, "MVA_ThPtOverSumPt", 10, -0.0001, 1, "ThPtOverSumPt");
        MSPlot_["maxMVA_AngleThWh"] = new MultiSamplePlot(datasets, "MVA_AngleThWh", 8, -0.0001, 3.2, "AngleThWh");
        MSPlot_["maxMVA_AngleThBl"] = new MultiSamplePlot(datasets, "MVA_AngleThBl", 8, -0.0001, 3.2, "AngleThBl");
        MSPlot_["maxMVA_AngleThBh"] = new MultiSamplePlot(datasets, "MVA_AngleThBh", 8, -0.0001, 3.2, "AngleThBh");
        MSPlot_["maxMVA_HadrWmass"] = new MultiSamplePlot(datasets, "MVA_HadrWmass", 50, -0.0001, 200, "HadrWmass");
        MSPlot_["maxMVA_HadrWMassCalc"] = new MultiSamplePlot(datasets, "maxMVA_HadrWMassCalc", 20, -0.0001, 600, "Hadronic W Mass");
        MSPlot_["maxMVA_HadrTopMass"] = new MultiSamplePlot(datasets, "maxMVA_HadrTopMass", 20, -0.0001, 1000, "Hadronic Top Mass");


        cout << "plots prepared..." << endl;

        vector<string> MVAvars;
        MVAvars.push_back("btag");
        // MVAvars.push_back("btag2");
        // MVAvars.push_back("btag3");
        MVAvars.push_back("ThPtOverSumPt");
        MVAvars.push_back("AngleThWh");
        // MVAvars.push_back("AngleBlMu");
        // MVAvars.push_back("AngleThBl");
        MVAvars.push_back("AngleThBh");
        // MVAvars.push_back("AngleThMu");
        // if(Tprime_)
        MVAvars.push_back("HadrWmass");
        MVAvars.push_back("TopMass");

        cout << "vector filled..." << endl;

        // MVAvars.push_back("sumE");
        // MVAvars.push_back("delR");


        cout <<"Instantiating computer..."<< endl;

        computer_ = new MVAComputer(MVAMethod,MVAOut,MVAPrefix,MVAvars,postfix_);
        // computer_->addMethod("LikelihoodD");
        // computer_->addMethod("LikelihoodPCA");
        // computer_->addMethod("MLP");
        // computer_->addMethod("MLPD");
    }

    histo1D_["PtJetCut_nEventsBefore"] = new TH1F("PtJetCut_nEventsBefore","PtJetCut_nEventsBefore",17,17.5,102.5);
    histo1D_["PtJetCut_nEvents4JetsMatchedBefore"] = new TH1F("PtJetCut_nEvents4JetsMatchedBefore","PtJetCut_nEvents4JetsMatchedBefore",17,17.5,102.5);
    histo1D_["PtJetCut_nEvents4PartonsMatchedBefore"] = new TH1F("PtJetCut_nEvents4PartonsMatchedBefore","PtJetCut_nEvents4PartonsMatchedBefore",17,17.5,102.5);

    histo1D_["PtJetCut_nEventsAfterBadISR"] = new TH1F("PtJetCut_nEventsAfterBadISR","PtJetCut_nEventsAfter >= 1 non-TTbar jet in 4 hardest jets, ISR",17,17.5,102.5);
    histo1D_["PtJetCut_nEventsAfterBadNoISR"] = new TH1F("PtJetCut_nEventsAfterBadNoISR","PtJetCut_nEventsAfter >= 1 non-TTbar jet in 4 hardest jets, no ISR",17,17.5,102.5);

    histo1D_["PtJetCut_nEventsAfter"] = new TH1F("PtJetCut_nEventsAfter","PtJetCut_nEventsAfter",17,17.5,102.5);
    histo1D_["PtJetCut_nEvents4JetsMatchedAfter"] = new TH1F("PtJetCut_nEvents4JetsMatchedAfter","PtJetCut_nEvents4JetsMatchedAfter",17,17.5,102.5);
    histo1D_["PtJetCut_nEvents4PartonsMatchedAfter"] = new TH1F("PtJetCut_nEvents4PartonsMatchedAfter","PtJetCut_nEvents4PartonsMatchedAfter",17,17.5,102.5);

    histo1D_["PtQuarksTTbar"] = new TH1F("PtQuarksTTbar","p_{T} of the quarks in TTbar semi-lep",100,0,200);
    histo1D_["Pt4thQuarkTTbar"] = new TH1F("Pt4thQuarkTTbar","p_{T} of the 4th quark in TTbar semi-lep",100,0,200);
    histo1D_["PtPartonsISR"] = new TH1F("PtPartonsISR","p_{T} of the ISR partons",100,0,200);

    histo1D_["PtMatchedQuarksTTbar"] = new TH1F("PtMatchedQuarksTTbar","p_{T} of the matched quarks in TTbar semi-lep",100,0,200);
    histo1D_["PtMatchedPartonsISR"] = new TH1F("PtMatchedPartonsISR","p_{T} of the matched ISR partons",100,0,200);

    histo1D_["hadronicTopMass"] = new TH1F("hadronicTopMass","Hadronic Top Mass", 100, 0, 400);
    histo1D_["hadronicWMass"] = new TH1F("hadronicWMass","Hadronic W Mass",100,0,200);
    histo2D_["hadronicTopMassVSHadronicWMass"] = new TH2F("hadronicTopMassVSHadronicWMass","Hadronic Top Mass VS Hadronic W Mass",100,0,400,100,0,200);

    histo1D_["hadronicPartonTopMass"] = new TH1F("hadronicPartonTopMass","Hadronic Top Mass, using the Partons", 80, 172, 173);
    histo1D_["hadronicPartonWMass"] = new TH1F("hadronicPartonWMass","Hadronic W Mass, using the Partons", 80, 60, 100);

    histo1D_["hadronicTopQuarkMass"] = new TH1F("hadronicTopQuarkMass","Hadronic Top Quark Mass", 80, 172.25, 172.75);
    histo1D_["leptonicTopQuarkMass"] = new TH1F("leptonicTopQuarkMass","Leptonic Top Quark Mass", 80, 172.25, 172.75);
    histo1D_["TopQuarkMassDiff"] = new TH1F("TopQuarkMassDiff","Top Quark Mass difference", 80, -0.4, 0.4);




    // Plots for the correlation
    histo2D_["hadronicWMassVSbtag"] = new TH2F("hadronicWMassVSbtag","Hadronic W Mass VS btag product",100,0,150,100,-0.0001,1);
    histo2D_["hadronicWMassVSAngleThWh"] = new TH2F("hadronicWMassVSAngleThWh","Hadronic W Mass VS Angle Th Wh",100,0,150,100,-0.0001,3.2);
    histo2D_["hadronicWMassVSAngleBlMu"] = new TH2F("hadronicWMassVSAngleBlMu","Hadronic W Mass VS Angle Bl Mu",100,0,150,100,-0.0001,3.2);
    histo2D_["hadronicWMassVSAngleThBl"] = new TH2F("hadronicWMassVSAngleThBl","Hadronic W Mass VS Angle Th Bl",100,0,150,100,-0.0001,3.2);
    histo2D_["hadronicWMassVSAngleThBh"] = new TH2F("hadronicWMassVSAngleThBh","Hadronic W Mass VS Angle Th Bh",100,0,150,100,-0.0001,3.2);
    histo2D_["hadronicWMassVSAngleThMu"] = new TH2F("hadronicWMassVSAngleThMu","Hadronic W Mass VS Angle Th Mu",100,0,150,100,-0.0001,3.2);
    histo2D_["hadronicWMassVSThPtOverSumPt"] = new TH2F("hadronicWMassVSThPtOverSumPt","Hadronic W Mass VS ThPtOverSumPt",100,0,150,100,-0.0001,1);

    histo2D_["hadronicTopMassVSbtag"] = new TH2F("hadronicTopMassVSbtag","Hadronic Top Mass VS btag product",100,100,250,100,-0.0001,1);
    histo2D_["hadronicTopMassVSAngleThWh"] = new TH2F("hadronicTopMassVSAngleThWh","Hadronic Top Mass VS Angle Th Wh",100,100,250,100,-0.0001,3.2);
    histo2D_["hadronicTopMassVSAngleBlMu"] = new TH2F("hadronicTopMassVSAngleBlMu","Hadronic Top Mass VS Angle Bl Mu",100,100,250,100,-0.0001,3.2);
    histo2D_["hadronicTopMassVSAngleThBl"] = new TH2F("hadronicTopMassVSAngleThBl","Hadronic Top Mass VS Angle Th Bl",100,100,250,100,-0.0001,3.2);
    histo2D_["hadronicTopMassVSAngleThBh"] = new TH2F("hadronicTopMassVSAngleThBh","Hadronic Top Mass VS Angle Th Bh",100,100,250,100,-0.0001,3.2);
    histo2D_["hadronicTopMassVSAngleThMu"] = new TH2F("hadronicTopMassVSAngleThMu","Hadronic Top Mass VS Angle Th Mu",100,100,250,100,-0.0001,3.2);
    histo2D_["hadronicTopMassVSThPtOverSumPt"] = new TH2F("hadronicTopMassVSThPtOverSumPt","Hadronic Top Mass VS ThPtOverSumPt",100,100,250,100,-0.0001,1);
}

JetCombiner::~JetCombiner() {
    if(trainer_) delete trainer_;
    if(computer_) delete computer_;
}

void JetCombiner::ProcessEvent_SingleHadTop(Dataset* dataSet, const vector<TRootMCParticle*> mcParticles, const vector<TRootPFJet*> selectedJets, const TLorentzVector* selectedLepton, float scaleFactor) {
    //initialize stuff for each event
    bool all4PartonsMatched = false; // True if the 4 ttbar semi-lep partons are matched to 4 jets (not necessarily the 4 highest pt jets)
    all4JetsMatched_MCdef_ = false; // True if the 4 highest pt jets are matched to the 4 ttbar semi-lep partons
    hadronictopJetsMatched_MCdef_ = false;

    bool debug = false;

    vectorMVA_.clear();
    leptonicBJet_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicBJet_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicWJet1_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicWJet2_ = pair<unsigned int,unsigned int>(9999,9999);
    mcParticlesMatching_.clear();
    selectedJets_.clear();
    selectedJets_ = selectedJets;

    vector<TRootPFJet*> tempselectedJets;


    relDiffEJetParton_b_ = -9999;
    relDiffEJetParton_l1_ = -9999;
    relDiffEJetParton_l2_ = -9999;

    string dataSetName = dataSet->Name();

    if(debug)   cout <<"jetcombiner: in jet combiner"<< endl;
    if(debug)   cout <<"jetcombiner: got gen event"<< endl;
    vector<TLorentzVector> mcParticlesTLV, selectedJetsTLV;
    TLorentzVector topQuark, antiTopQuark;

    bool muPlusFromTop = false, muMinusFromTop = false, elPlusFromTop = false, elMinusFromTop = false;
    int nTTbarQuarks = 0;
    int pdgID_top = 6; //top quark

    for(unsigned int i=0; i<mcParticles.size(); i++) {
        if(debug)     cout <<"jetcombiner: looping mc parts"<< endl;
        // cout << i << ":  status: " << mcParticles[i]->status() << "  pdgId: " << mcParticles[i]->type()
        //  << "  motherPdgId: " << mcParticles[i]->motherType() << "  grannyPdgId: " << mcParticles[i]->grannyType() << endl;
        if( mcParticles[i]->status() != 3) continue;

        if( mcParticles[i]->type() == pdgID_top )
            topQuark = *mcParticles[i];
        else if( mcParticles[i]->type() == -pdgID_top )
            antiTopQuark = *mcParticles[i];

        if( mcParticles[i]->type() == 13 && mcParticles[i]->motherType() == -24 && mcParticles[i]->grannyType() == -pdgID_top )
            muMinusFromTop = true;
        if( mcParticles[i]->type() == -13 && mcParticles[i]->motherType() == 24 && mcParticles[i]->grannyType() == pdgID_top )
            muPlusFromTop = true;
        if( mcParticles[i]->type() == 11 && mcParticles[i]->motherType() == -24 && mcParticles[i]->grannyType() == -pdgID_top )
            elMinusFromTop = true;
        if( mcParticles[i]->type() == -11 && mcParticles[i]->motherType() == 24 && mcParticles[i]->grannyType() == pdgID_top )
            elPlusFromTop = true;
    }

    if( muMinusFromTop || muPlusFromTop || elMinusFromTop || elPlusFromTop) {


        for(unsigned int i=0; i<mcParticles.size(); i++) {


            // if( (fabs(mcParticles[i]->type()) < 6 && fabs(mcParticles[i]->motherType()) == 24 && fabs(mcParticles[i]->grannyType()) == pdgID_top) || (fabs(mcParticles[i]->type()) == 5 && fabs(mcParticles[i]->motherType()) == pdgID_top)) //this if statement to be in sync with other processevent method
            if( abs(mcParticles[i]->type()) < 6 || abs(mcParticles[i]->type()) == 21 ) { //light/b quarks, 6 should stay hardcoded
                mcParticlesTLV.push_back(*mcParticles[i]);
                mcParticlesMatching_.push_back(mcParticles[i]);

                if( fabs(mcParticles[i]->motherType()) == pdgID_top || fabs(mcParticles[i]->grannyType()) == pdgID_top ) {
                    histo1D_["PtQuarksTTbar"]->Fill(mcParticles[i]->Pt());
                    nTTbarQuarks++;
                    if(nTTbarQuarks == 4)
                        histo1D_["Pt4thQuarkTTbar"]->Fill(mcParticles[i]->Pt());
                }
                else
                    histo1D_["PtPartonsISR"]->Fill(mcParticles[i]->Pt());
            }
        }

        // Fill some top Quark plots
        if(muPlusFromTop || elPlusFromTop) {
            histo1D_["hadronicTopQuarkMass"]->Fill( antiTopQuark.M() );
            histo1D_["leptonicTopQuarkMass"]->Fill( topQuark.M() );
        }
        else if(muMinusFromTop || elMinusFromTop) {
            histo1D_["hadronicTopQuarkMass"]->Fill( topQuark.M() );
            histo1D_["leptonicTopQuarkMass"]->Fill( antiTopQuark.M() );
        }
        histo1D_["TopQuarkMassDiff"]->Fill( topQuark.M() - antiTopQuark.M() );

        // take all the selectedJets_ to study the radiation stuff, selectedJets_ are already ordened in decreasing Pt()
        for(unsigned int i=0; i<selectedJets_.size(); i++)
            selectedJetsTLV.push_back(*selectedJets_[i]);

        JetPartonMatching matching = JetPartonMatching(mcParticlesTLV, selectedJetsTLV, 2, true, true, 0.3);

        if(matching.getNumberOfAvailableCombinations() != 1)
            cerr << "matching.getNumberOfAvailableCombinations() = "<<matching.getNumberOfAvailableCombinations()<<"  This should be equal to 1 !!!"<<endl;

        vector< pair<unsigned int, unsigned int> > JetPartonPair, ISRJetPartonPair; // First one is jet number, second one is mcParticle number

        for(unsigned int i=0; i<mcParticlesTLV.size(); i++) {
            int matchedJetNumber = matching.getMatchForParton(i, 0);
            //Refining the Jet-parton association: now the simple jet-parton matching scheme must agree with the more sophisticated diagnosis of jet flavour from PAT.
            if((matchedJetNumber != -1) && (   selectedJets_[matchedJetNumber]->partonFlavour() == mcParticlesMatching_[i]->type()   ))
                JetPartonPair.push_back( pair<unsigned int, unsigned int> (matchedJetNumber, i) );
        }

        for(unsigned int i=0; i<JetPartonPair.size(); i++) {
            unsigned int j = JetPartonPair[i].second;

            // cout <<"Matching Flavour = =  "<<mcParticlesMatching_[JetPartonPair[i].second]->type()   <<"     PAT Flavour = ="  << selectedJets_[JetPartonPair[i].first]->partonFlavour()<<endl;

            //      cout<<"JetPartonPair["<<i<<"]: "<<endl;
            //      cout<<"Jet Pt: "<<selectedJets_[JetPartonPair[i].first]->Pt()<<"  Eta: "<<selectedJets_[JetPartonPair[i].first]->Eta()<<"  Phi: "<<selectedJets_[JetPartonPair[i].first]->Phi()<<endl;
            //      cout<<"MCParticle Pt: "<<mcParticlesMatching_[JetPartonPair[i].second]->Pt()<<"  Eta: "<<mcParticlesMatching_[JetPartonPair[i].second]->Eta()<<"  Phi: "<<mcParticlesMatching_[JetPartonPair[i].second]->Phi()<<endl;
            //      cout<<"\ttype: "<<mcParticlesMatching_[JetPartonPair[i].second]->type()<<"  motherType: "<<mcParticlesMatching_[JetPartonPair[i].second]->motherType()<<"  grannyType: "<<mcParticlesMatching_[JetPartonPair[i].second]->grannyType()<<endl;
            //      cout<<"DR(MCParticle, jet) = "<<selectedJets_[JetPartonPair[i].first]->DeltaR(*mcParticlesMatching_[JetPartonPair[i].second])<<endl;

            if( fabs(mcParticlesMatching_[j]->type()) < 6 ) { //light/b quarks, 6 should stay hardcoded

                //dividing bad combinations into sub-categories to understand structure in BDT discriminant
                // Class 1: exactly 1 incorrect jet
                //          1A: the b-jet is not selected
                //          1B: one of the jets from the W is not selected.
                // Class 2: exactly 2 incorrect jets
                //          2A: both jets from the W are incorrect
                //          2B: 1 jet from the b and one jet from the W are incorrect
                // Class3:  All jets are incorrect.


                if( ( ( muPlusFromTop || elPlusFromTop ) && mcParticlesMatching_[j]->motherType() == -24 && mcParticlesMatching_[j]->grannyType() == -pdgID_top )
                 || ( ( muMinusFromTop || elMinusFromTop ) && mcParticlesMatching_[j]->motherType() == 24 && mcParticlesMatching_[j]->grannyType() == pdgID_top ) ) {
                    if(hadronicWJet1_.first == 9999)
                        hadronicWJet1_ = JetPartonPair[i];
                    else if(hadronicWJet2_.first == 9999)
                        hadronicWJet2_ = JetPartonPair[i];
                    else cerr<<"Found a third jet coming from a W boson which comes from a top quark..."<<endl;
                }
            }
            if( fabs(mcParticlesMatching_[j]->type()) == 5 ) {
                if( ( ( muPlusFromTop || elPlusFromTop ) && mcParticlesMatching_[j]->motherType() == -pdgID_top )
                    || ( ( muMinusFromTop || elMinusFromTop ) && mcParticlesMatching_[j]->motherType() == pdgID_top ) )
                    hadronicBJet_ = JetPartonPair[i];
                else if( ( ( muPlusFromTop || elPlusFromTop ) && mcParticlesMatching_[j]->motherType() == pdgID_top )
                     || ( ( muMinusFromTop || elMinusFromTop ) && mcParticlesMatching_[j]->motherType() == -pdgID_top ) )
                    leptonicBJet_ = JetPartonPair[i];
            }

            // look for ISR stuff
            if( fabs(mcParticlesMatching_[j]->type()) != pdgID_top && fabs(mcParticlesMatching_[j]->motherType()) != 24 && fabs(mcParticlesMatching_[j]->motherType()) != pdgID_top &&
             fabs(mcParticlesMatching_[j]->grannyType()) != 24 && fabs(mcParticlesMatching_[j]->grannyType()) != pdgID_top ) {
                ISRJetPartonPair.push_back(JetPartonPair[i]);
            }
        }

        if(hadronicWJet1_.first != 9999 && hadronicWJet2_.first != 9999 && hadronicBJet_.first != 9999 && leptonicBJet_.first != 9999) {
            histo1D_["hadronicPartonTopMass"]->Fill((*mcParticlesMatching_[hadronicWJet1_.second]+*mcParticlesMatching_[hadronicWJet2_.second]+*mcParticlesMatching_[hadronicBJet_.second]).M());
            histo1D_["hadronicPartonWMass"]->Fill((*mcParticlesMatching_[hadronicWJet1_.second]+*mcParticlesMatching_[hadronicWJet2_.second]).M());

            all4PartonsMatched = true;
            // if(hadronicWJet1_.first < 4 && hadronicWJet2_.first < 4 && hadronicBJet_.first < 4 )
            all4JetsMatched_MCdef_ = true;
        }
        ////cout<<"   ------> according to JetCombiner: hadronicWJet1_.first = "<<hadronicWJet1_.first<<", hadronicWJet2_.first = "<<hadronicWJet2_.first<<", hadronicBJet_.first = "<<hadronicBJet_.first<<endl;
        if(hadronicWJet1_.first < 4 && hadronicWJet2_.first < 4 && hadronicBJet_.first < 4)
            hadronictopJetsMatched_MCdef_ = true;

        // fill ISR plots
        for(unsigned int i=0; i<ISRJetPartonPair.size(); i++)
            histo1D_["PtMatchedPartonsISR"]->Fill(mcParticlesMatching_[ISRJetPartonPair[i].second]->Pt());

        if(hadronicWJet1_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[hadronicWJet1_.second]->Pt());
        if(hadronicWJet2_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[hadronicWJet2_.second]->Pt());
        if(hadronicBJet_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[hadronicBJet_.second]->Pt());
        if(leptonicBJet_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[leptonicBJet_.second]->Pt());

        for(int i=1; i<=histo1D_["PtJetCut_nEventsBefore"]->GetNbinsX(); i++) {
            float binCenter = histo1D_["PtJetCut_nEventsBefore"]->GetBinCenter(i);
            histo1D_["PtJetCut_nEventsBefore"]->Fill(binCenter);
            if(selectedJets_.size() > 3 && selectedJets_[3]->Pt() > binCenter) {
                histo1D_["PtJetCut_nEventsAfter"]->Fill(binCenter);
                if( hadronicWJet1_.first > 3 || hadronicWJet2_.first > 3 || hadronicBJet_.first > 3 || leptonicBJet_.first > 3 ) {
                    bool foundISR = false;
                    for(unsigned int j=0; j<ISRJetPartonPair.size(); j++)
                        if(ISRJetPartonPair[j].first < 4)
                            foundISR = true;

                    if(foundISR)
                        histo1D_["PtJetCut_nEventsAfterBadISR"]->Fill(binCenter);
                    else
                        histo1D_["PtJetCut_nEventsAfterBadNoISR"]->Fill(binCenter);
                }
            }
        }

        if(all4PartonsMatched) {
            for(int i=1; i<=histo1D_["PtJetCut_nEventsBefore"]->GetNbinsX(); i++) {
                float binCenter = histo1D_["PtJetCut_nEventsBefore"]->GetBinCenter(i);
                histo1D_["PtJetCut_nEvents4PartonsMatchedBefore"]->Fill(binCenter);
                if(selectedJets_.size() > 3 && selectedJets_[leptonicBJet_.first]->Pt() > binCenter && selectedJets_[hadronicBJet_.first]->Pt() > binCenter &&
                        selectedJets_[hadronicWJet1_.first]->Pt() > binCenter && selectedJets_[hadronicWJet2_.first]->Pt() > binCenter)
                    histo1D_["PtJetCut_nEvents4PartonsMatchedAfter"]->Fill(binCenter);
            }

            if(all4JetsMatched_MCdef_) {
                // all 4 jets found and matched, now do something with them!

                for(int i=1; i<=histo1D_["PtJetCut_nEventsBefore"]->GetNbinsX(); i++) {
                    float binCenter = histo1D_["PtJetCut_nEventsBefore"]->GetBinCenter(i);
                    histo1D_["PtJetCut_nEvents4JetsMatchedBefore"]->Fill(binCenter);
                    if(selectedJets_.size() > 3 && selectedJets_[3]->Pt() > binCenter)
                        histo1D_["PtJetCut_nEvents4JetsMatchedAfter"]->Fill(binCenter);
                }
            }
        }
    } //if Semi mu or semi el ttbar


    ///////////////////////
    // Train/Compute MVA //
    ///////////////////////

    if(debug)  cout <<"jetcombiner: train/compute MVA" << endl;

    float previousMaxMVA = -9999;

    float maxMVA_btag = -9999;
    float maxMVA_AngleThWh = -9999;
    float maxMVA_AngleThBl = -9999;
    float maxMVA_AngleThBh = -9999;
    float maxMVA_ThPtOverSumPt = -9999;
    float maxMVA_HadrWmass = -9999;

    // index convention -> i,j: jets from Hadronic W  k: Hadronic b
    for (unsigned int i=0; i<selectedJets.size(); i++) {
        for (unsigned int j=0; j<selectedJets.size(); j++) {
            for (unsigned int k=0; k<selectedJets.size(); k++) {
                //for (unsigned int l=0; l<4; l++) {
                if (i != j && i != k && j != k ) {
                    // cout<<"Jet comb :  "<<endl;
                    // cout << "ijk = > " << i << j << k <<endl;
                    // cout <<" "<<endl;

                    //calculate the vars
                    // btag
                    float btag_i = selectedJets_[i]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
                    if(btag_i < -90) btag_i = 0;
                    float btag_j = selectedJets_[j]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
                    if(btag_j < -90) btag_j = 0;
                    float btag_k = selectedJets_[k]->btag_combinedInclusiveSecondaryVertexV2BJetTags();
                    if(btag_k < -90) btag_k = 0;

                    tempselectedJets.clear();
                    tempselectedJets.push_back(selectedJets_[i]);
                    tempselectedJets.push_back(selectedJets_[j]);
                    tempselectedJets.push_back(selectedJets_[k]);
                    float mindeltaR =100.;
                    float mindeltaR_temp =100.;
                    unsigned int wj1=0;
                    unsigned int wj2=0;
                    unsigned int bj1=0;

                    //define the jets from W as the jet pair with smallest deltaR
                    for (unsigned int m=0; m<tempselectedJets.size(); m++) {
                        for (unsigned int n=0; n<tempselectedJets.size(); n++) {
                            if(n==m) continue;
                            TLorentzVector lj1  = *tempselectedJets[m];
                            TLorentzVector lj2  = *tempselectedJets[n];
                            mindeltaR_temp  = lj1.DeltaR(lj2);
                            if (mindeltaR_temp < mindeltaR) {
                                mindeltaR = mindeltaR_temp;
                                wj1 = m;
                                wj2 = n;
                            }
                        }
                    }

                    // find the index of the jet not chosen as a W-jet
                    for (unsigned int p=0; p<tempselectedJets.size(); p++) {
                        if(p!=wj1 && p!=wj2) bj1 = p;
                    }
                    float btag = -9999;
                    if(tempselectedJets.size()>0)
                        btag = tempselectedJets[bj1]->btag_combinedInclusiveSecondaryVertexV2BJetTags();

                    // float  delR =  sqrt(   pow(tempselectedJets[wj1]->Eta() - tempselectedJets[wj2]->Eta(),2) + pow(tempselectedJets[wj1]->Phi() - tempselectedJets[wj2]->Phi(),2 )   ) ;
                    // float  delR =  sqrt(   pow(selectedJets[0]->Eta() - selectedJets[1]->Eta(),2) + pow(selectedJets[0]->Phi() - selectedJets[1]->Phi(),2 )   )  +  sqrt(   pow(selectedJets[1]->Eta() - selectedJets[2]->Eta(),2) + pow(selectedJets[1]->Phi() - selectedJets[2]->Phi(),2 )   )  + sqrt(   pow(selectedJets[2]->Eta() - selectedJets[0]->Eta(),2) + pow(selectedJets[2]->Phi() - selectedJets[0]->Phi(),2 )) ;

                    // build the lorentz-vectors
                    TLorentzVector Wh = *tempselectedJets[wj1]+*tempselectedJets[wj2];
                    TLorentzVector Bh = *tempselectedJets[bj1];
                    TLorentzVector Th = Wh+Bh;

                    //double TopMass = Th.M(); // currently not used anywhere!

                    // TLorentzVector Bl = *selectedJets_[l];
                    // TLorentzVector Mu = *selectedLepton;

                    //Angular separation
                    // float AngleThWh = Th.Angle(Wh.Vect());

                    //DeltaR
                    float AngleThWh = fabs(Th.DeltaPhi(Wh));


                    // cout <<" AngleThWh    "<<  AngleThWh <<endl;
                    // float AngleBlMu = Bl.Angle(Mu.Vect());
                    // float AngleThBl = Th.Angle(Bl.Vect());

                    // Angular separation
                    // float AngleThBh = Th.Angle(Bh.Vect());

                    // DeltaR
                    float AngleThBh = fabs(Th.DeltaPhi(Bh));



                    //trying the summed energy of the jets...
                    // currently not used                    double sumE = (tempselectedJets[wj1]->E())+ (tempselectedJets[wj2]->E()) + (tempselectedJets[bj1]->E()) ;

                    // float AngleThMu = Th.Angle(Mu.Vect());

                    // pt(Th)/SumPt 3j - possible 3 jet combinations are 012 013 023 and 123

                    vector<float> PTJetComb;
                    float sumPt = 0;
                    sumPt = selectedJets_[i]->Pt() + selectedJets_[j]->Pt() + selectedJets_[k]->Pt();

                    // sumPt += (*selectedJets_[0]+*selectedJets_[1]+*selectedJets_[2]).Pt();
                    // sumPt += (*selectedJets_[0]+*selectedJets_[1]+*selectedJets_[3]).Pt();
                    // sumPt += (*selectedJets_[0]+*selectedJets_[2]+*selectedJets_[3]).Pt();
                    // sumPt += (*selectedJets_[1]+*selectedJets_[2]+*selectedJets_[3]).Pt();

                    float ThPtOverSumPt = Th.Pt()/sumPt;

                    if( muMinusFromTop || muPlusFromTop || elMinusFromTop || elPlusFromTop) {

                        if ( (i!=j)&&(i == hadronicWJet1_.first || i ==  hadronicWJet2_.first) && (j == hadronicWJet2_.first || j == hadronicWJet1_.first) && k == hadronicBJet_.first) {
                            // Do some things only for the good jet combination on the hadronic side
                            histo1D_["hadronicTopMass"]->Fill( Th.M() );
                            histo1D_["hadronicWMass"]->Fill( Wh.M() );
                            histo2D_["hadronicTopMassVSHadronicWMass"]->Fill( Th.M() , Wh.M() );

                            // if( l == leptonicBJet_.first ) // fully matched event
                            // {
                            histo2D_["hadronicWMassVSbtag"]->Fill(Wh.M(), btag);
                            histo2D_["hadronicWMassVSAngleThWh"]->Fill(Wh.M(), AngleThWh);
                            //  histo2D_["hadronicWMassVSAngleBlMu"]->Fill(Wh.M(), AngleBlMu);
                            //         histo2D_["hadronicWMassVSAngleThBl"]->Fill(Wh.M(), AngleThBl);
                            histo2D_["hadronicWMassVSAngleThBh"]->Fill(Wh.M(), AngleThBh);
                            //   histo2D_["hadronicWMassVSAngleThMu"]->Fill(Wh.M(), AngleThMu);
                            histo2D_["hadronicWMassVSThPtOverSumPt"]->Fill(Wh.M(), ThPtOverSumPt);

                            histo2D_["hadronicTopMassVSbtag"]->Fill(Th.M(), btag);
                            histo2D_["hadronicTopMassVSAngleThWh"]->Fill(Th.M(), AngleThWh);
                            //   histo2D_["hadronicTopMassVSAngleBlMu"]->Fill(Th.M(), AngleBlMu);
                            //       histo2D_["hadronicTopMassVSAngleThBl"]->Fill(Th.M(), AngleThBl);
                            histo2D_["hadronicTopMassVSAngleThBh"]->Fill(Th.M(), AngleThBh);
                            //   histo2D_["hadronicTopMassVSAngleThMu"]->Fill(Th.M(), AngleThMu);
                            histo2D_["hadronicTopMassVSThPtOverSumPt"]->Fill(Th.M(), ThPtOverSumPt);
                            // }
                        }


                        if( trainMVA_ && all4JetsMatched_MCdef_ ) { // Only train for events where you are sure what is S and B
                            // fill the MVA trainer variables and check what should be the good combination
                            if ( (i == hadronicWJet1_.first || i ==  hadronicWJet2_.first) && (j == hadronicWJet2_.first || j == hadronicWJet1_.first) && k == hadronicBJet_.first) {


                                trainer_->Fill("S","btag",btag);
                                // trainer_->Fill("S","btag2",btag2);
                                //trainer_->Fill("S","btag3",btag3);
                                // trainer_->Fill("S","TopMass", Th.M());

                                trainer_->Fill("S","ThPtOverSumPt",ThPtOverSumPt);
                                trainer_->Fill("S","AngleThWh",AngleThWh);
                                // trainer_->Fill("S","AngleBlMu",AngleBlMu);
                                // trainer_->Fill("S","AngleThBl",AngleThBl);
                                trainer_->Fill("S","AngleThBh",AngleThBh);
                                // trainer_->Fill("S","delR",delR);
                                //     trainer_->Fill("S","AngleThMu",AngleThMu);
                                trainer_->Fill("S","HadrWmass",Wh.M());
                                trainer_->Fill("S","TopMass",Th.M());
                                // trainer_->Fill("S","AngleThLep",AngleThLep);


                                //  trainer_->Fill("S","sumE",sumE);

                            }
                            else {
                                trainer_->Fill("B","btag",btag);
                                // trainer_->Fill("B","btag2",btag2);
                                //trainer_->Fill("B","btag3",btag3);
                                //   trainer_->Fill("B","TopMass", Th.M());
                                // trainer_->Fill("B","delR",delR);


                                trainer_->Fill("B","ThPtOverSumPt",ThPtOverSumPt);
                                trainer_->Fill("B","AngleThWh",AngleThWh);
                                // trainer_->Fill("B","AngleBlMu",AngleBlMu);
                                // trainer_->Fill("B","AngleThBl",AngleThBl);
                                trainer_->Fill("B","AngleThBh",AngleThBh);
                                //trainer_->Fill("B","AngleThMu",AngleThMu);
                                trainer_->Fill("B","HadrWmass",Wh.M());
                                trainer_->Fill("B","TopMass",Th.M());
                                // trainer_->Fill("B","AngleThLep",AngleThLep);

                                //trainer_->Fill("B","sumE",sumE);

                            }
                        } //end trainMVA_ && all4JetsMatched_MCdef_
                    } //end (muMinusFromTop || muPlusFromTop || elMinusFromTop || elPlusFromTop)


                    if( !trainMVA_ ) {
                        // compute MVA stuff
                        MSPlot_["MVA_input_btag"]->Fill(btag, dataSet, true, Luminosity_*scaleFactor);
                        MSPlot_["MVA_input_ThPtOverSumPt"]->Fill(ThPtOverSumPt, dataSet, true, Luminosity_*scaleFactor);
                        MSPlot_["MVA_input_AngleThWh"]->Fill(AngleThWh, dataSet, true, Luminosity_*scaleFactor);
                        // MSPlot_["MVA_input_AngleBlMu"]->Fill(AngleBlMu, dataSet, true, Luminosity_*scaleFactor);
                        // MSPlot_["MVA_input_AngleThBl"]->Fill(AngleThBl, dataSet, true, Luminosity_*scaleFactor);
                        MSPlot_["MVA_input_AngleThBh"]->Fill(AngleThBh, dataSet, true, Luminosity_*scaleFactor);
                        // MSPlot_["MVA_input_AngleThMu"]->Fill(AngleThMu, dataSet, true, Luminosity_*scaleFactor);
                        MSPlot_["MVA_input_HadrWmass"]->Fill(Wh.M(), dataSet, true, Luminosity_*scaleFactor);

                        computer_->FillVar("btag",btag);
                        // computer_->FillVar("btag2",btag2);
                        // computer_->FillVar("btag3",btag3);
                        computer_->FillVar("ThPtOverSumPt",ThPtOverSumPt);
                        computer_->FillVar("AngleThWh",AngleThWh);
                        // computer_->FillVar("AngleBlMu",AngleBlMu);
                        // computer_->FillVar("AngleThBl",AngleThBl);
                        computer_->FillVar("AngleThBh",AngleThBh);
                        // computer_->FillVar("AngleThMu",AngleThMu);

                        computer_->FillVar("TopMass",Th.M());

                        computer_->FillVar("HadrWmass",Wh.M());
                        // computer_->FillVar("sumE",sumE);


                        //only the jets from the hadronic top should be matched
                        if ( (i == hadronicWJet1_.first || i ==  hadronicWJet2_.first) && (j == hadronicWJet2_.first || j == hadronicWJet1_.first) && k == hadronicBJet_.first)
                            //if ( ((i == hadronicWJet1_.first && j ==  hadronicWJet2_.first) || (i == hadronicWJet2_.first && j == hadronicWJet1_.first)) && k == hadronicBJet_.first)
                        {
                            relDiffEJetParton_b_ = (mcParticlesMatching_[hadronicBJet_.second]->Energy()-selectedJets_[k]->Energy())/selectedJets_[k]->Energy();
                            relDiffEJetParton_l1_ = (mcParticlesMatching_[hadronicWJet1_.second]->Energy()-selectedJets_[hadronicWJet1_.first]->Energy())/selectedJets_[hadronicWJet1_.first]->Energy();
                            relDiffEJetParton_l2_ = (mcParticlesMatching_[hadronicWJet2_.second]->Energy()-selectedJets_[hadronicWJet2_.first]->Energy())/selectedJets_[hadronicWJet2_.first]->Energy();
                        }

                        // get map of  MVAValues
                        std::map<std::string,Float_t> MVAVals = computer_->GetMVAValues();

                        for (std::map<std::string,Float_t>::const_iterator it = MVAVals.begin(); it != MVAVals.end(); ++it) {

                            MVAValues tempMVA;
                            tempMVA.MVAAlgo = it->first;
                            tempMVA.MVAValue = it->second;
                            tempMVA.WJet1 = i;
                            tempMVA.WJet2 = j;
                            tempMVA.HadrBJet = k;
                            // tempMVA.LeptBJet = l;
                            // cout <<"In process event, MVA val =   "<< it->second << endl;

                            vectorMVA_.push_back(tempMVA);

                            if(tempMVA.MVAValue > previousMaxMVA) {
                                previousMaxMVA = tempMVA.MVAValue;

                                maxMVA_btag = btag;
                                maxMVA_AngleThWh = AngleThWh;
                                //   maxMVA_AngleThBl = AngleThBl;
                                maxMVA_AngleThBh = AngleThBh;
                                maxMVA_ThPtOverSumPt = ThPtOverSumPt;
                                maxMVA_HadrWmass = Wh.M();
                            }
                        }
                    }
                }
            //}
            }
        }
    } // end of jetcomb loop

    if(debug)  cout <<"jetcombiner: end of jet comb loop"<< endl;

    //  cout <<"    "<< endl;
    //  cout <<"    "<< endl;
    //  cout <<"    "<< endl;

    if( !trainMVA_ ) {
        vector<string> MVAnames = computer_->GetAllMethods();
        for(unsigned int i=0; i<MVAnames.size(); i++) {

            if(debug) cout <<"jetcombiner: not training mva..."<< endl;
            pair<float, vector<unsigned int> > MVAResult = getMVAValue(MVAnames[i], 1); // 1 means the highest MVA value
            if(MVAResult.first != previousMaxMVA)
                cout << "MVAResult.first != previousMaxMVA ???" << endl;

            //only the jets from the hadronic top should be matched
            ////cout<<"   ------> according to JetCombiner MVA: MVAResult.second[0] = "<<MVAResult.second[0]<<", MVAResult.second[1] = "<<MVAResult.second[1]<<", MVAResult.second[2] = "<<MVAResult.second[2]<<endl;
            hadronictopJetsMatched_MVAdef_ = false;
            if ( ((MVAResult.second[0] == hadronicWJet1_.first && MVAResult.second[1] ==  hadronicWJet2_.first)
                    || (MVAResult.second[0] == hadronicWJet2_.first && MVAResult.second[1] == hadronicWJet1_.first))
                    && MVAResult.second[2] == hadronicBJet_.first)
                hadronictopJetsMatched_MVAdef_ = true;

            float mW_maxMVA = (*selectedJets_[MVAResult.second[0]] + *selectedJets_[MVAResult.second[1]]).M();
            float mTop_maxMVA = (*selectedJets_[MVAResult.second[0]] + *selectedJets_[MVAResult.second[1]] + *selectedJets_[MVAResult.second[2]]).M();

            MSPlot_["maxMVA_btag"]->Fill(maxMVA_btag, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_ThPtOverSumPt"]->Fill(maxMVA_ThPtOverSumPt, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThWh"]->Fill(maxMVA_AngleThWh, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThBl"]->Fill(maxMVA_AngleThBl, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThBh"]->Fill(maxMVA_AngleThBh, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrWmass"]->Fill(maxMVA_HadrWmass, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrWMassCalc"]->Fill(mW_maxMVA, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrTopMass"]->Fill(mTop_maxMVA, dataSet, true, Luminosity_*scaleFactor);

            if(debug) cout <<"jetcombiner: msplots filled..."<< endl;


            string title = "MaxMVA_"+MVAnames[i]+"_allcomb";
            string titleSemiMuBG = "MaxMVA_"+MVAnames[i]+"_allSemiMuBGcomb"; //alSemiMulBGcomb contains badcomb events and also events where the mcParticle matching was not successful
            string axisTitle = "MVA "+MVAnames[i]+" response";


            // check if plots exist, else create them
            if (MSPlot_.find(title) == MSPlot_.end())
                MSPlot_[title] = new MultiSamplePlot(datasets_, title.c_str(), 20, 0, 1, axisTitle.c_str());
            if(histo1D_.find(titleSemiMuBG) == histo1D_.end())
                histo1D_[titleSemiMuBG] = new TH1F(titleSemiMuBG.c_str(),axisTitle.c_str(),50,0,1);
            if(debug) cout <<"jetcombiner: making 1 d histos 0 ..."<< endl;

            // MSPlot_[title]->Fill(MVAResult.first, dataSet, true, Luminosity_*scaleFactor);
            if( hadronictopJetsMatched_MVAdef_ == false && dataSetName.find("TTbarJets_SemiMu") == 0 )
                histo1D_[titleSemiMuBG]->Fill(MVAResult.first);
            if(debug) cout <<"jetcombiner: making 1 d histos 1"<< endl;


            if( muMinusFromTop || muPlusFromTop || elMinusFromTop || elPlusFromTop) {
                std::string titleGood = "MaxMVA_"+MVAnames[i]+"_goodcomb";
                std::string titleBad = "MaxMVA_"+MVAnames[i]+"_badcomb";
                std::string axisTitle = "MVA "+MVAnames[i]+" response";
                std::string titleGoodmWVSmtop = "MaxMVA_mWVSmtop_"+MVAnames[i]+"_goodcomb";
                std::string titleBadmWVSmtop = "MaxMVA_mWVSmtop_"+MVAnames[i]+"_badcomb";

                if(debug) cout <<"jetcombiner: making 1 d histos..."<< endl;


                // check if goodcomb badcomb histo exists, else create it
                /* 1 d histos not needed
                                           if (histo1D_.find(titleGood) == histo1D_.end())
                                           {
                                               histo1D_[titleGood] = new TH1F(titleGood.c_str(),axisTitle.c_str(),50,0,1);
                                               histo1D_[titleBad] = new TH1F(titleBad.c_str(),axisTitle.c_str(),50,0,1);
                                               histo2D_[titleGoodmWVSmtop] = new TH2F(titleGoodmWVSmtop.c_str(),axisTitle.c_str(),100,0,200,200,0,400);
                                               histo2D_[titleBadmWVSmtop] = new TH2F(titleBadmWVSmtop.c_str(),axisTitle.c_str(),100,0,200,200,0,400);
                                           }

                                           //cout << MVAnames[i] << " " << MVAResult->first << " " << hadronictopJetsMatched_MVAdef_ << endl;
                                           if ( hadronictopJetsMatched_MVAdef_ )
                                           {
                                               histo1D_[titleGood]->Fill( MVAResult.first );
                                               histo2D_[titleGoodmWVSmtop]->Fill(mW_maxMVA,mTop_maxMVA);
                                           }
                                           else if ( hadronictopJetsMatched_MCdef_ )
                                           {
                                               histo1D_[titleBad]->Fill( MVAResult.first );
                                               histo2D_[titleBadmWVSmtop]->Fill(mW_maxMVA,mTop_maxMVA);
                                           }
                */

            }
        }
    }


    if(debug) cout <<"jetcombiner: end of single had top method..."<< endl;
}

void JetCombiner::ProcessEvent(Dataset* dataSet, const vector<TRootMCParticle*> mcParticles, const vector<TRootPFJet*> selectedJets, const TLorentzVector* selectedLepton, vector<TRootElectron*> vectEl, vector<TRootMuon*> vectMu, float scaleFactor, bool TprimeEvaluation) {
    //initialize stuff for each event
    bool all4PartonsMatched = false; // True if the 4 ttbar semi-lep partons are matched to 4 jets (not necessarily the 4 highest pt jets)    
    all4JetsMatched_MCdef_ = false; // True if the 4 highest pt jets are matched to the 4 ttbar semi-lep partons
    hadronictopJetsMatched_MCdef_ = false;

    vectorMVA_.clear();
    leptonicBJet_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicBJet_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicWJet1_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicWJet2_ = pair<unsigned int,unsigned int>(9999,9999);
    mcParticlesMatching_.clear();
    selectedJets_.clear();
    selectedJets_ = selectedJets;

    relDiffEJetParton_b_ = -9999;
    relDiffEJetParton_l1_ = -9999;
    relDiffEJetParton_l2_ = -9999;

    vector<TLorentzVector> mcParticlesTLV, selectedJetsTLV;
    TLorentzVector topQuark, antiTopQuark;

    bool muPlusFromTop = false, muMinusFromTop = false, elPlusFromTop = false, elMinusFromTop = false;
    int nTTbarQuarks = 0;
    int pdgID_top = 6; //top quark
    if(TprimeEvaluation)
        pdgID_top = 8; //4th generation t' quark
    for(unsigned int i=0; i<mcParticles.size(); i++) {
        //cout << i << ":  status: " << mcParticles[i]->status() << "  pdgId: " << mcParticles[i]->type()
        //  << "  motherPdgId: " << mcParticles[i]->motherType() << "  grannyPdgId: " << mcParticles[i]->grannyType() << endl;
        if( mcParticles[i]->status() != 3) continue;

        if( mcParticles[i]->type() == pdgID_top )
            topQuark = *mcParticles[i];
        else if( mcParticles[i]->type() == -pdgID_top )
            antiTopQuark = *mcParticles[i];

        if( mcParticles[i]->type() == 13 && mcParticles[i]->motherType() == -24 && mcParticles[i]->grannyType() == -pdgID_top )
            muMinusFromTop = true;
        if( mcParticles[i]->type() == -13 && mcParticles[i]->motherType() == 24 && mcParticles[i]->grannyType() == pdgID_top )
            muPlusFromTop = true;
        if( mcParticles[i]->type() == 11 && mcParticles[i]->motherType() == -24 && mcParticles[i]->grannyType() == -pdgID_top )
            elMinusFromTop = true;
        if( mcParticles[i]->type() == -11 && mcParticles[i]->motherType() == 24 && mcParticles[i]->grannyType() == pdgID_top )
            elPlusFromTop = true;
    }    

    string dataSetName = dataSet->Name();

    if( muMinusFromTop || muPlusFromTop || elMinusFromTop || elPlusFromTop || TprimeEvaluation) {

        for(unsigned int i=0; i<mcParticles.size(); i++) {
            //if( (fabs(mcParticles[i]->type()) < 6 && fabs(mcParticles[i]->motherType()) == 24 && fabs(mcParticles[i]->grannyType()) == pdgID_top) || (fabs(mcParticles[i]->type()) == 5 && fabs(mcParticles[i]->motherType()) == pdgID_top)) //this if statement to be in sync with other processevent method
            if( abs(mcParticles[i]->type()) < 6 || abs(mcParticles[i]->type()) == 21 ) { //light/b quarks, 6 should stay hardcoded
                mcParticlesTLV.push_back(*mcParticles[i]);
                mcParticlesMatching_.push_back(mcParticles[i]);

                if( fabs(mcParticles[i]->motherType()) == pdgID_top || fabs(mcParticles[i]->grannyType()) == pdgID_top ) {
                    histo1D_["PtQuarksTTbar"]->Fill(mcParticles[i]->Pt());
                    nTTbarQuarks++;
                    if(nTTbarQuarks == 4)
                        histo1D_["Pt4thQuarkTTbar"]->Fill(mcParticles[i]->Pt());
                }
                else
                    histo1D_["PtPartonsISR"]->Fill(mcParticles[i]->Pt());
            }
        }

        // Fill some top Quark plots
        if(muPlusFromTop || elPlusFromTop) {
            histo1D_["hadronicTopQuarkMass"]->Fill( antiTopQuark.M() );
            histo1D_["leptonicTopQuarkMass"]->Fill( topQuark.M() );
        }
        else if(muMinusFromTop || elMinusFromTop) {
            histo1D_["hadronicTopQuarkMass"]->Fill( topQuark.M() );
            histo1D_["leptonicTopQuarkMass"]->Fill( antiTopQuark.M() );
        }
        histo1D_["TopQuarkMassDiff"]->Fill( topQuark.M() - antiTopQuark.M() );

        // take all the selectedJets_ to study the radiation stuff, selectedJets_ are already ordened in decreasing Pt()
        for(unsigned int i=0; i<selectedJets_.size(); i++)
            selectedJetsTLV.push_back(*selectedJets_[i]);

        JetPartonMatching matching = JetPartonMatching(mcParticlesTLV, selectedJetsTLV, 2, true, true, 0.3);

        if(matching.getNumberOfAvailableCombinations() != 1)
            cerr << "matching.getNumberOfAvailableCombinations() = "<<matching.getNumberOfAvailableCombinations()<<"  This should be equal to 1 !!!"<<endl;

        vector< pair<unsigned int, unsigned int> > JetPartonPair, ISRJetPartonPair; // First one is jet number, second one is mcParticle number

        for(unsigned int i=0; i<mcParticlesTLV.size(); i++) {
            int matchedJetNumber = matching.getMatchForParton(i, 0);
            if(matchedJetNumber != -1)
                JetPartonPair.push_back( pair<unsigned int, unsigned int> (matchedJetNumber, i) );
        }

        for(unsigned int i=0; i<JetPartonPair.size(); i++) {
            unsigned int j = JetPartonPair[i].second;

            //      cout<<"JetPartonPair["<<i<<"]: "<<endl;
            //      cout<<"Jet Pt: "<<selectedJets_[JetPartonPair[i].first]->Pt()<<"  Eta: "<<selectedJets_[JetPartonPair[i].first]->Eta()<<"  Phi: "<<selectedJets_[JetPartonPair[i].first]->Phi()<<endl;
            //      cout<<"MCParticle Pt: "<<mcParticlesMatching_[JetPartonPair[i].second]->Pt()<<"  Eta: "<<mcParticlesMatching_[JetPartonPair[i].second]->Eta()<<"  Phi: "<<mcParticlesMatching_[JetPartonPair[i].second]->Phi()<<endl;
            //      cout<<"\ttype: "<<mcParticlesMatching_[JetPartonPair[i].second]->type()<<"  motherType: "<<mcParticlesMatching_[JetPartonPair[i].second]->motherType()<<"  grannyType: "<<mcParticlesMatching_[JetPartonPair[i].second]->grannyType()<<endl;
            //      cout<<"DR(MCParticle, jet) = "<<selectedJets_[JetPartonPair[i].first]->DeltaR(*mcParticlesMatching_[JetPartonPair[i].second])<<endl;

            if( fabs(mcParticlesMatching_[j]->type()) < 6 ) { //light/b quarks, 6 should stay hardcoded
                if( ( ( muPlusFromTop || elPlusFromTop ) && mcParticlesMatching_[j]->motherType() == -24 && mcParticlesMatching_[j]->grannyType() == -pdgID_top )
                    || ( ( muMinusFromTop || elMinusFromTop ) && mcParticlesMatching_[j]->motherType() == 24 && mcParticlesMatching_[j]->grannyType() == pdgID_top ) ) {
                    if(hadronicWJet1_.first == 9999)
                        hadronicWJet1_ = JetPartonPair[i];
                    else if(hadronicWJet2_.first == 9999)
                        hadronicWJet2_ = JetPartonPair[i];
                    else cerr<<"Found a third jet coming from a W boson which comes from a top quark..."<<endl;
                }
            }
            if( fabs(mcParticlesMatching_[j]->type()) == 5 ) {
                if( ( ( muPlusFromTop || elPlusFromTop ) && mcParticlesMatching_[j]->motherType() == -pdgID_top )
                    || ( ( muMinusFromTop || elMinusFromTop ) && mcParticlesMatching_[j]->motherType() == pdgID_top ) )
                    hadronicBJet_ = JetPartonPair[i];
                else if( ( ( muPlusFromTop || elPlusFromTop ) && mcParticlesMatching_[j]->motherType() == pdgID_top )
                   || ( ( muMinusFromTop || elMinusFromTop ) && mcParticlesMatching_[j]->motherType() == -pdgID_top ) )
                    leptonicBJet_ = JetPartonPair[i];
            }

            // look for ISR stuff
            if( fabs(mcParticlesMatching_[j]->type()) != pdgID_top && fabs(mcParticlesMatching_[j]->motherType()) != 24 && fabs(mcParticlesMatching_[j]->motherType()) != pdgID_top &&
                fabs(mcParticlesMatching_[j]->grannyType()) != 24 && fabs(mcParticlesMatching_[j]->grannyType()) != pdgID_top ) {
                ISRJetPartonPair.push_back(JetPartonPair[i]);
            }
        }

        if(hadronicWJet1_.first != 9999 && hadronicWJet2_.first != 9999 && hadronicBJet_.first != 9999 && leptonicBJet_.first != 9999) {
            histo1D_["hadronicPartonTopMass"]->Fill((*mcParticlesMatching_[hadronicWJet1_.second]+*mcParticlesMatching_[hadronicWJet2_.second]+*mcParticlesMatching_[hadronicBJet_.second]).M());
            histo1D_["hadronicPartonWMass"]->Fill((*mcParticlesMatching_[hadronicWJet1_.second]+*mcParticlesMatching_[hadronicWJet2_.second]).M());

            all4PartonsMatched = true;
            if(hadronicWJet1_.first < 4 && hadronicWJet2_.first < 4 && hadronicBJet_.first < 4 && leptonicBJet_.first < 4)
                all4JetsMatched_MCdef_ = true;
        }
            ////cout<<"   ------> according to JetCombiner: hadronicWJet1_.first = "<<hadronicWJet1_.first<<", hadronicWJet2_.first = "<<hadronicWJet2_.first<<", hadronicBJet_.first = "<<hadronicBJet_.first<<endl;
        if(hadronicWJet1_.first < 4 && hadronicWJet2_.first < 4 && hadronicBJet_.first < 4)
            hadronictopJetsMatched_MCdef_ = true;

            // fill ISR plots
        for(unsigned int i=0; i<ISRJetPartonPair.size(); i++)
            histo1D_["PtMatchedPartonsISR"]->Fill(mcParticlesMatching_[ISRJetPartonPair[i].second]->Pt());

        if(hadronicWJet1_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[hadronicWJet1_.second]->Pt());
        if(hadronicWJet2_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[hadronicWJet2_.second]->Pt());
        if(hadronicBJet_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[hadronicBJet_.second]->Pt());
        if(leptonicBJet_.first != 9999)
            histo1D_["PtMatchedQuarksTTbar"]->Fill(mcParticlesMatching_[leptonicBJet_.second]->Pt());

        for(int i=1; i<=histo1D_["PtJetCut_nEventsBefore"]->GetNbinsX(); i++) {
            float binCenter = histo1D_["PtJetCut_nEventsBefore"]->GetBinCenter(i);
            histo1D_["PtJetCut_nEventsBefore"]->Fill(binCenter);
            if(selectedJets_.size() > 3 && selectedJets_[3]->Pt() > binCenter) {
                histo1D_["PtJetCut_nEventsAfter"]->Fill(binCenter);
                if( hadronicWJet1_.first > 3 || hadronicWJet2_.first > 3 || hadronicBJet_.first > 3 || leptonicBJet_.first > 3 ) {
                    bool foundISR = false;
                    for(unsigned int j=0; j<ISRJetPartonPair.size(); j++)
                        if(ISRJetPartonPair[j].first < 4)
                            foundISR = true;

                        if(foundISR)
                            histo1D_["PtJetCut_nEventsAfterBadISR"]->Fill(binCenter);
                        else
                            histo1D_["PtJetCut_nEventsAfterBadNoISR"]->Fill(binCenter);
                }
            }
        }

        if(all4PartonsMatched) {
            for(int i=1; i<=histo1D_["PtJetCut_nEventsBefore"]->GetNbinsX(); i++) {
                float binCenter = histo1D_["PtJetCut_nEventsBefore"]->GetBinCenter(i);
                histo1D_["PtJetCut_nEvents4PartonsMatchedBefore"]->Fill(binCenter);
                if(selectedJets_.size() > 3 && selectedJets_[leptonicBJet_.first]->Pt() > binCenter && selectedJets_[hadronicBJet_.first]->Pt() > binCenter &&
                    selectedJets_[hadronicWJet1_.first]->Pt() > binCenter && selectedJets_[hadronicWJet2_.first]->Pt() > binCenter)
                    histo1D_["PtJetCut_nEvents4PartonsMatchedAfter"]->Fill(binCenter);
            }

            if(all4JetsMatched_MCdef_) {
                // all 4 jets found and matched, now do something with them!
                for(int i=1; i<=histo1D_["PtJetCut_nEventsBefore"]->GetNbinsX(); i++) {
                    float binCenter = histo1D_["PtJetCut_nEventsBefore"]->GetBinCenter(i);
                    histo1D_["PtJetCut_nEvents4JetsMatchedBefore"]->Fill(binCenter);
                    if(selectedJets_.size() > 3 && selectedJets_[3]->Pt() > binCenter)
                        histo1D_["PtJetCut_nEvents4JetsMatchedAfter"]->Fill(binCenter);
                }
            }
        }
    } //if Semi mu or semi el ttbar
    // string dataSetName = dataSet->Name();

    ///////////////////////
    // Train/Compute MVA //
    ///////////////////////

    float previousMaxMVA = -9999;

    float maxMVA_btag = -9999;
    float maxMVA_AngleThWh = -9999;
    float maxMVA_AngleThBl = -9999;
    float maxMVA_AngleThBh = -9999;
    float maxMVA_ThPtOverSumPt = -9999;
    float maxMVA_HadrWmass = -9999;

    // index convention -> i,j: jets from Hadronic W  k: Hadronic b and l: leptonic b
    for (unsigned int i=0; i<4; i++) {
        for (unsigned int j=0; j<4; j++) {
            for (unsigned int k=0; k<4; k++) {
                for (unsigned int l=0; l<4; l++) {
                    if (i < j && i != j && i != k && i != l && j != k && j != l && k != l) {
                        //calculate the vars
                        // btag
                        float btag_i = 0;//selectedJets_[i]->btag_trackCountingHighEffBJetTags();
                        if(btag_i < -90) btag_i = 0;
                        float btag_j = 0;//selectedJets_[j]->btag_trackCountingHighEffBJetTags();
                        if(btag_j < -90) btag_j = 0;
                        float btag_k = 0;//selectedJets_[k]->btag_trackCountingHighEffBJetTags();
                        if(btag_k < -90) btag_k = 0;
                        float btag_l = 0;//selectedJets_[l]->btag_trackCountingHighEffBJetTags();
                        if(btag_l < -90) btag_l = 0;

                        float btag = pow(btag_k,2) + pow(btag_l,2);
                        btag = btag / ( pow(btag_i,2) + pow(btag_j,2) + pow(btag_k,2) + pow(btag_l,2) );

                        // build the lorentz-vectors
                        TLorentzVector Wh = *selectedJets_[i]+*selectedJets_[j];
                        TLorentzVector Bh = *selectedJets_[k];
                        TLorentzVector Th = Wh+Bh;
                        TLorentzVector Bl = *selectedJets_[l];
                        TLorentzVector Mu = *selectedLepton;

                        float AngleThWh = Th.Angle(Wh.Vect());
                        float AngleBlMu = Bl.Angle(Mu.Vect());
                        float AngleThBl = Th.Angle(Bl.Vect());
                        float AngleThBh = Th.Angle(Bh.Vect());
                        float AngleThMu = Th.Angle(Mu.Vect());

                        // pt(Th)/SumPt 3j - possible 3 jet combinations are 012 013 023 and 123

                        vector<float> PTJetComb;
                        float sumPt = 0;
                        sumPt += (*selectedJets_[0]+*selectedJets_[1]+*selectedJets_[2]).Pt();
                        sumPt += (*selectedJets_[0]+*selectedJets_[1]+*selectedJets_[3]).Pt();
                        sumPt += (*selectedJets_[0]+*selectedJets_[2]+*selectedJets_[3]).Pt();
                        sumPt += (*selectedJets_[1]+*selectedJets_[2]+*selectedJets_[3]).Pt();

                        float ThPtOverSumPt = Th.Pt()/sumPt;

                        if( muMinusFromTop || muPlusFromTop || elMinusFromTop || elPlusFromTop || TprimeEvaluation) {
                            if ( (i == hadronicWJet1_.first || i ==  hadronicWJet2_.first) && (j == hadronicWJet2_.first || j == hadronicWJet1_.first) && k == hadronicBJet_.first) {
                                // Do some things only for the good jet combination on the hadronic side
                                    histo1D_["hadronicTopMass"]->Fill( Th.M() );
                                    histo1D_["hadronicWMass"]->Fill( Wh.M() );
                                    histo2D_["hadronicTopMassVSHadronicWMass"]->Fill( Th.M() , Wh.M() );

                                if( l == leptonicBJet_.first ) { // fully matched event
                                    histo2D_["hadronicWMassVSbtag"]->Fill(Wh.M(), btag);
                                    histo2D_["hadronicWMassVSAngleThWh"]->Fill(Wh.M(), AngleThWh);
                                    histo2D_["hadronicWMassVSAngleBlMu"]->Fill(Wh.M(), AngleBlMu);
                                    histo2D_["hadronicWMassVSAngleThBl"]->Fill(Wh.M(), AngleThBl);
                                    histo2D_["hadronicWMassVSAngleThBh"]->Fill(Wh.M(), AngleThBh);
                                    histo2D_["hadronicWMassVSAngleThMu"]->Fill(Wh.M(), AngleThMu);
                                    histo2D_["hadronicWMassVSThPtOverSumPt"]->Fill(Wh.M(), ThPtOverSumPt);

                                    histo2D_["hadronicTopMassVSbtag"]->Fill(Th.M(), btag);
                                    histo2D_["hadronicTopMassVSAngleThWh"]->Fill(Th.M(), AngleThWh);
                                    histo2D_["hadronicTopMassVSAngleBlMu"]->Fill(Th.M(), AngleBlMu);
                                    histo2D_["hadronicTopMassVSAngleThBl"]->Fill(Th.M(), AngleThBl);
                                    histo2D_["hadronicTopMassVSAngleThBh"]->Fill(Th.M(), AngleThBh);
                                    histo2D_["hadronicTopMassVSAngleThMu"]->Fill(Th.M(), AngleThMu);
                                    histo2D_["hadronicTopMassVSThPtOverSumPt"]->Fill(Th.M(), ThPtOverSumPt);
                                }
                            }

                            if( trainMVA_ && all4JetsMatched_MCdef_ ) { // Only train for events where you are sure what is S and B
                                // fill the MVA trainer variables and check what should be the good combination
                                if ( (i == hadronicWJet1_.first || i ==  hadronicWJet2_.first) && (j == hadronicWJet2_.first || j == hadronicWJet1_.first) && k == hadronicBJet_.first && l == leptonicBJet_.first) {
                                    trainer_->Fill("S","btag",btag);
                                    trainer_->Fill("S","ThPtOverSumPt",ThPtOverSumPt);
                                    //    	              trainer_->Fill("S","AngleThWh",AngleThWh);
                                    trainer_->Fill("S","AngleBlMu",AngleBlMu);
                                    trainer_->Fill("S","AngleThBl",AngleThBl);
                                    //    	              trainer_->Fill("S","AngleThBh",AngleThBh);
                                    trainer_->Fill("S","AngleThMu",AngleThMu);
                                    if(Tprime_)
                                        trainer_->Fill("S","HadrWmass",Wh.M());
                                }
                                else {
                                    trainer_->Fill("B","btag",btag);
                                    trainer_->Fill("B","ThPtOverSumPt",ThPtOverSumPt);
                                    //   		              trainer_->Fill("B","AngleThWh",AngleThWh);
                                    trainer_->Fill("B","AngleBlMu",AngleBlMu);
                                    trainer_->Fill("B","AngleThBl",AngleThBl);
                                    //   		              trainer_->Fill("B","AngleThBh",AngleThBh);
                                    trainer_->Fill("B","AngleThMu",AngleThMu);
                                    if(Tprime_)
                                        trainer_->Fill("B","HadrWmass",Wh.M());
                                }
                            }
                        }

                        if( !trainMVA_ ) {
                            // compute MVA stuff
                            MSPlot_["MVA_input_btag"]->Fill(btag, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_ThPtOverSumPt"]->Fill(ThPtOverSumPt, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThWh"]->Fill(AngleThWh, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleBlMu"]->Fill(AngleBlMu, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThBl"]->Fill(AngleThBl, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThBh"]->Fill(AngleThBh, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThMu"]->Fill(AngleThMu, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_HadrWmass"]->Fill(Wh.M(), dataSet, true, Luminosity_*scaleFactor);

                            computer_->FillVar("btag",btag);
                            computer_->FillVar("ThPtOverSumPt",ThPtOverSumPt);
                            //                computer_->FillVar("AngleThWh",AngleThWh);
                            computer_->FillVar("AngleBlMu",AngleBlMu);
                            computer_->FillVar("AngleThBl",AngleThBl);
                            //                computer_->FillVar("AngleThBh",AngleThBh);
                            computer_->FillVar("AngleThMu",AngleThMu);
                            if(Tprime_)
                                computer_->FillVar("HadrWmass",Wh.M());

                            //only the jets from the hadronic top should be matched
                            if ( (i == hadronicWJet1_.first || i ==  hadronicWJet2_.first) && (j == hadronicWJet2_.first || j == hadronicWJet1_.first) && k == hadronicBJet_.first)
                                //if ( ((i == hadronicWJet1_.first && j ==  hadronicWJet2_.first) || (i == hadronicWJet2_.first && j == hadronicWJet1_.first)) && k == hadronicBJet_.first)
                            {
                                relDiffEJetParton_b_ = (mcParticlesMatching_[hadronicBJet_.second]->Energy()-selectedJets_[k]->Energy())/selectedJets_[k]->Energy();
                                relDiffEJetParton_l1_ = (mcParticlesMatching_[hadronicWJet1_.second]->Energy()-selectedJets_[hadronicWJet1_.first]->Energy())/selectedJets_[hadronicWJet1_.first]->Energy();
                                relDiffEJetParton_l2_ = (mcParticlesMatching_[hadronicWJet2_.second]->Energy()-selectedJets_[hadronicWJet2_.first]->Energy())/selectedJets_[hadronicWJet2_.first]->Energy();
                            }

                            // get map of  MVAValues
                            std::map<std::string,Float_t> MVAVals = computer_->GetMVAValues();

                            // vectorMVA_
                            for (std::map<std::string,Float_t>::const_iterator it = MVAVals.begin(); it != MVAVals.end(); ++it) {
                                MVAValues tempMVA;
                                tempMVA.MVAAlgo = it->first;
                                tempMVA.MVAValue = it->second;
                                tempMVA.WJet1 = i;
                                tempMVA.WJet2 = j;
                                tempMVA.HadrBJet = k;
                                tempMVA.LeptBJet = l;

                                vectorMVA_.push_back(tempMVA);

                                if(tempMVA.MVAValue > previousMaxMVA) {
                                    previousMaxMVA = tempMVA.MVAValue;

                                    maxMVA_btag = btag;
                                    maxMVA_AngleThWh = AngleThWh;
                                    maxMVA_AngleThBl = AngleThBl;
                                    maxMVA_AngleThBh = AngleThBh;
                                    maxMVA_ThPtOverSumPt = ThPtOverSumPt;
                                    maxMVA_HadrWmass = Wh.M();
                                }
                            }
                        } //if train MVA
                    } //if jets aren't the same
                }//for
            }//for
        }//for
    } // end of jetcomb loop

    if( !trainMVA_ ) {
        vector<string> MVAnames = computer_->GetAllMethods();
        for(unsigned int i=0; i<MVAnames.size(); i++) {
            pair<float, vector<unsigned int> > MVAResult = getMVAValue(MVAnames[i], 1); // 1 means the highest MVA value
            if(MVAResult.first != previousMaxMVA)
                cout << "MVAResult.first != previousMaxMVA ???" << endl;

            //only the jets from the hadronic top should be matched
            ////cout<<"   ------> according to JetCombiner MVA: MVAResult.second[0] = "<<MVAResult.second[0]<<", MVAResult.second[1] = "<<MVAResult.second[1]<<", MVAResult.second[2] = "<<MVAResult.second[2]<<endl;
            hadronictopJetsMatched_MVAdef_ = false;
            if ( ((MVAResult.second[0] == hadronicWJet1_.first && MVAResult.second[1] ==  hadronicWJet2_.first)
                || (MVAResult.second[0] == hadronicWJet2_.first && MVAResult.second[1] == hadronicWJet1_.first))
                && MVAResult.second[2] == hadronicBJet_.first)
                hadronictopJetsMatched_MVAdef_ = true;

            float mW_maxMVA = (*selectedJets_[MVAResult.second[0]] + *selectedJets_[MVAResult.second[1]]).M();
            float mTop_maxMVA = (*selectedJets_[MVAResult.second[0]] + *selectedJets_[MVAResult.second[1]] + *selectedJets_[MVAResult.second[2]]).M();

            MSPlot_["maxMVA_btag"]->Fill(maxMVA_btag, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_ThPtOverSumPt"]->Fill(maxMVA_ThPtOverSumPt, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThWh"]->Fill(maxMVA_AngleThWh, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThBl"]->Fill(maxMVA_AngleThBl, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThBh"]->Fill(maxMVA_AngleThBh, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrWmass"]->Fill(maxMVA_HadrWmass, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrWMassCalc"]->Fill(mW_maxMVA, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrTopMass"]->Fill(mTop_maxMVA, dataSet, true, Luminosity_*scaleFactor);

            string title = "MaxMVA_"+MVAnames[i]+"_allcomb";
            string titleSemiMuBG = "MaxMVA_"+MVAnames[i]+"_allSemiMuBGcomb"; //alSemiMulBGcomb contains badcomb events and also events where the mcParticle matching was not successful
            string axisTitle = "MVA "+MVAnames[i]+" response";

            // check if plots exist, else create them
            if (MSPlot_.find(title) == MSPlot_.end())
                MSPlot_[title] = new MultiSamplePlot(datasets_, title.c_str(), 20, 0, 1, axisTitle.c_str());
            if(histo1D_.find(titleSemiMuBG) == histo1D_.end())
                histo1D_[titleSemiMuBG] = new TH1F(titleSemiMuBG.c_str(),axisTitle.c_str(),50,0,1);

            MSPlot_[title]->Fill(MVAResult.first, dataSet, true, Luminosity_*scaleFactor);
            if( hadronictopJetsMatched_MVAdef_ == false && dataSetName.find("TTbarJets_SemiMu") == 0 )
                histo1D_[titleSemiMuBG]->Fill(MVAResult.first);

            if( muMinusFromTop || muPlusFromTop || elMinusFromTop || elPlusFromTop || TprimeEvaluation) {
                std::string titleGood = "MaxMVA_"+MVAnames[i]+"_goodcomb";
                std::string titleBad = "MaxMVA_"+MVAnames[i]+"_badcomb";
                std::string axisTitle = "MVA "+MVAnames[i]+" response";
                std::string titleGoodmWVSmtop = "MaxMVA_mWVSmtop_"+MVAnames[i]+"_goodcomb";
                std::string titleBadmWVSmtop = "MaxMVA_mWVSmtop_"+MVAnames[i]+"_badcomb";

                // check if goodcomb badcomb histo exists, else create it
                if (histo1D_.find(titleGood) == histo1D_.end()) {
                    histo1D_[titleGood] = new TH1F(titleGood.c_str(),axisTitle.c_str(),50,0,1);
                    histo1D_[titleBad] = new TH1F(titleBad.c_str(),axisTitle.c_str(),50,0,1);
                    histo2D_[titleGoodmWVSmtop] = new TH2F(titleGoodmWVSmtop.c_str(),axisTitle.c_str(),100,0,200,200,0,400);
                    histo2D_[titleBadmWVSmtop] = new TH2F(titleBadmWVSmtop.c_str(),axisTitle.c_str(),100,0,200,200,0,400);
                }

                //cout << MVAnames[i] << " " << MVAResult->first << " " << hadronictopJetsMatched_MVAdef_ << endl;
                if ( hadronictopJetsMatched_MVAdef_ ) {
                    histo1D_[titleGood]->Fill( MVAResult.first );
                    histo2D_[titleGoodmWVSmtop]->Fill(mW_maxMVA,mTop_maxMVA);
                }
                else if ( hadronictopJetsMatched_MCdef_ ) {
                    histo1D_[titleBad]->Fill( MVAResult.first );
                    histo2D_[titleBadmWVSmtop]->Fill(mW_maxMVA,mTop_maxMVA);
                }
            }
        }//for loop
    }//if !train MVA
}

void JetCombiner::ProcessEvent(Dataset* dataSet, const vector<TLorentzVector> mcParticlesForMatching, const vector<TLorentzVector> selectedJets, const vector<float> bTagValues, const TLorentzVector selectedLepton, bool isSemiLep, float scaleFactor, bool TprimeEvaluation) {
    //initialize stuff for each event
    all4JetsMatched_MCdef_ = false; // True if the 4 highest pt jets are matched to the 4 ttbar semi-lep partons
    hadronictopJetsMatched_MCdef_ = false;

    vectorMVA_.clear();
    leptonicBJet_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicBJet_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicWJet1_ = pair<unsigned int,unsigned int>(9999,9999);
    hadronicWJet2_ = pair<unsigned int,unsigned int>(9999,9999);

    string dataSetName = dataSet->Name();
    if( isSemiLep  || (TprimeEvaluation && mcParticlesForMatching.size()==4) ) { //requiring that there are 4 quarks stored means that it is semilep! (see treecreator)
        TLorentzVector topQuark, antiTopQuark;

        //for(unsigned int i=0; i<selectedJets.size(); i++)
        //	cout<<"selectedJets["<<i<<"].Pt() = "<<selectedJets[i].Pt()<<endl;

        JetPartonMatching matching = JetPartonMatching(mcParticlesForMatching, selectedJets, 2, true, true, 0.3);

        if(matching.getNumberOfAvailableCombinations() != 1)
            cerr << "matching.getNumberOfAvailableCombinations() = "<<matching.getNumberOfAvailableCombinations()<<"  This should be equal to 1 !!!"<<endl;

        vector< pair<unsigned int, unsigned int> > JetPartonPair; // First one is jet number, second one is mcParticle number

        for(unsigned int i=0; i<mcParticlesForMatching.size(); i++) {
            int matchedJetNumber = matching.getMatchForParton(i, 0);
            //cout<<"matchedJetNumber = "<<matchedJetNumber<<endl;
            if(matchedJetNumber != -1) {
                //cout<<"mcParticlesForMatching["<<i<<"].Pt() = "<<mcParticlesForMatching[i].Pt()<<endl;
                JetPartonPair.push_back( pair<unsigned int, unsigned int> (matchedJetNumber, i) );
                }
            else
                JetPartonPair.push_back( pair<unsigned int, unsigned int> (9999,9999) ); //a bit different than the other processEvent function
            }

        //ordering was defined in InclFourthGen_TreeCreator
        if(JetPartonPair.size() == 4) {
            hadronicWJet1_ = JetPartonPair[0];
            hadronicWJet2_ = JetPartonPair[1];
            hadronicBJet_ = JetPartonPair[2];
            leptonicBJet_ = JetPartonPair[3];
            //cout<<"hadronicWJet1_.first = "<<hadronicWJet1_.first<<endl;
            //cout<<"hadronicWJet2_.first = "<<hadronicWJet2_.first<<endl;
            //cout<<"hadronicBJet_.first = "<<hadronicBJet_.first<<endl;
            //cout<<"leptonicBJet_.first = "<<leptonicBJet_.first<<endl;
            }
        else
            cout<<"Problem!! JetPartonPair.size() != 4"<<endl;

        if(hadronicWJet1_.first != 9999 && hadronicWJet2_.first != 9999 && hadronicBJet_.first != 9999 && leptonicBJet_.first != 9999) {
            if(hadronicWJet1_.first < 4 && hadronicWJet2_.first < 4 && hadronicBJet_.first < 4 && leptonicBJet_.first < 4)
                all4JetsMatched_MCdef_ = true;
            }
        //cout<<"   ------> according to JetCombiner: hadronicWJet1_.first = "<<hadronicWJet1_.first<<", hadronicWJet2_.first = "<<hadronicWJet2_.first<<", hadronicBJet_.first = "<<hadronicBJet_.first<<endl;
        if(hadronicWJet1_.first < 4 && hadronicWJet2_.first < 4 && hadronicBJet_.first < 4)
            hadronictopJetsMatched_MCdef_ = true;

        } //if Semi mu or semi el ttbar

    ///////////////////////
    // Train/Compute MVA //
    ///////////////////////

    float previousMaxMVA = -9999;

    float maxMVA_btag = -9999;
    float maxMVA_AngleThWh = -9999;
    float maxMVA_AngleThBl = -9999;
    float maxMVA_AngleThBh = -9999;
    float maxMVA_ThPtOverSumPt = -9999;
    float maxMVA_HadrWmass = -9999;

    // index convention -> i,j: jets from Hadronic W  k: Hadronic b and l: leptonic b
    for (unsigned int i=0; i<4; i++) {
        for (unsigned int j=0; j<4; j++) {
            for (unsigned int k=0; k<4; k++) {
                for (unsigned int l=0; l<4; l++) {
                    if (i < j && i != j && i != k && i != l && j != k && j != l && k != l) {
                        //calculate the vars
                        // btag
                        float btag_i = bTagValues[i];
                        if(btag_i < -90) btag_i = 0;
                        float btag_j = bTagValues[j];
                        if(btag_j < -90) btag_j = 0;
                        float btag_k = bTagValues[k];
                        if(btag_k < -90) btag_k = 0;
                        float btag_l = bTagValues[l];
                        if(btag_l < -90) btag_l = 0;

                        float btag = pow(btag_k,2) + pow(btag_l,2);
                        btag = btag / ( pow(btag_i,2) + pow(btag_j,2) + pow(btag_k,2) + pow(btag_l,2) );

                        // build the lorentz-vectors
                        TLorentzVector Wh = selectedJets[i]+selectedJets[j];
                        TLorentzVector Bh = selectedJets[k];
                        TLorentzVector Th = Wh+Bh;
                        TLorentzVector Bl = selectedJets[l];
                        TLorentzVector Mu = selectedLepton;

                        float AngleThWh = Th.Angle(Wh.Vect());
                        float AngleBlMu = Bl.Angle(Mu.Vect());
                        float AngleThBl = Th.Angle(Bl.Vect());
                        float AngleThBh = Th.Angle(Bh.Vect());
                        float AngleThMu = Th.Angle(Mu.Vect());

                        // pt(Th)/SumPt 3j - possible 3 jet combinations are 012 013 023 and 123

                        vector<float> PTJetComb;
                        float sumPt = 0;
                        sumPt += (selectedJets[0]+selectedJets[1]+selectedJets[2]).Pt();
                        sumPt += (selectedJets[0]+selectedJets[1]+selectedJets[3]).Pt();
                        sumPt += (selectedJets[0]+selectedJets[2]+selectedJets[3]).Pt();
                        sumPt += (selectedJets[1]+selectedJets[2]+selectedJets[3]).Pt();

                        float ThPtOverSumPt = Th.Pt()/sumPt;

                        if(isSemiLep  || (TprimeEvaluation && mcParticlesForMatching.size()==4)) {
                            if( trainMVA_ && all4JetsMatched_MCdef_ ) { // Only train for events where you are sure what is S and B
                                // fill the MVA trainer variables and check what should be the good combination
                                if ( (i == hadronicWJet1_.first || i ==  hadronicWJet2_.first) && (j == hadronicWJet2_.first || j == hadronicWJet1_.first) && k == hadronicBJet_.first && l == leptonicBJet_.first) {
                                    trainer_->Fill("S","btag",btag);
                                    trainer_->Fill("S","ThPtOverSumPt",ThPtOverSumPt);
                                    //    	              trainer_->Fill("S","AngleThWh",AngleThWh);
                                    trainer_->Fill("S","AngleBlMu",AngleBlMu);
                                    trainer_->Fill("S","AngleThBl",AngleThBl);
                                    //    	              trainer_->Fill("S","AngleThBh",AngleThBh);
                                    trainer_->Fill("S","AngleThMu",AngleThMu);
                                    if(Tprime_)
                                        trainer_->Fill("S","HadrWmass",Wh.M());
                                    }
                                else {
                                    trainer_->Fill("B","btag",btag);
                                    trainer_->Fill("B","ThPtOverSumPt",ThPtOverSumPt);
                                    //   		              trainer_->Fill("B","AngleThWh",AngleThWh);
                                    trainer_->Fill("B","AngleBlMu",AngleBlMu);
                                    trainer_->Fill("B","AngleThBl",AngleThBl);
                                    //   		              trainer_->Fill("B","AngleThBh",AngleThBh);
                                    trainer_->Fill("B","AngleThMu",AngleThMu);
                                    if(Tprime_)
                                        trainer_->Fill("B","HadrWmass",Wh.M());
                                    }
                                }
                            }

                        if( !trainMVA_ ) {
                            // compute MVA stuff
                            MSPlot_["MVA_input_btag"]->Fill(btag, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_ThPtOverSumPt"]->Fill(ThPtOverSumPt, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThWh"]->Fill(AngleThWh, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleBlMu"]->Fill(AngleBlMu, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThBl"]->Fill(AngleThBl, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThBh"]->Fill(AngleThBh, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_AngleThMu"]->Fill(AngleThMu, dataSet, true, Luminosity_*scaleFactor);
                            MSPlot_["MVA_input_HadrWmass"]->Fill(Wh.M(), dataSet, true, Luminosity_*scaleFactor);

                            computer_->FillVar("btag",btag);
                            computer_->FillVar("ThPtOverSumPt",ThPtOverSumPt);
                            //                computer_->FillVar("AngleThWh",AngleThWh);
                            computer_->FillVar("AngleBlMu",AngleBlMu);
                            computer_->FillVar("AngleThBl",AngleThBl);
                            //                computer_->FillVar("AngleThBh",AngleThBh);
                            computer_->FillVar("AngleThMu",AngleThMu);
                            if(Tprime_)
                                computer_->FillVar("HadrWmass",Wh.M());

                            // get map of  MVAValues
                            std::map<std::string,Float_t> MVAVals = computer_->GetMVAValues();

                            // vectorMVA_
                            for (std::map<std::string,Float_t>::const_iterator it = MVAVals.begin(); it != MVAVals.end(); ++it) {
                                MVAValues tempMVA;
                                tempMVA.MVAAlgo = it->first;
                                tempMVA.MVAValue = it->second;
                                tempMVA.WJet1 = i;
                                tempMVA.WJet2 = j;
                                tempMVA.HadrBJet = k;
                                tempMVA.LeptBJet = l;

                                vectorMVA_.push_back(tempMVA);

                                if(tempMVA.MVAValue > previousMaxMVA) {
                                    previousMaxMVA = tempMVA.MVAValue;

                                    maxMVA_btag = btag;
                                    maxMVA_AngleThWh = AngleThWh;
                                    maxMVA_AngleThBl = AngleThBl;
                                    maxMVA_AngleThBh = AngleThBh;
                                    maxMVA_ThPtOverSumPt = ThPtOverSumPt;
                                    maxMVA_HadrWmass = Wh.M();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } // end of jetcomb loop
    if( !trainMVA_ ) {
        vector<string> MVAnames = computer_->GetAllMethods();
        for(unsigned int i=0; i<MVAnames.size(); i++) {
            pair<float, vector<unsigned int> > MVAResult = getMVAValue(MVAnames[i], 1); // 1 means the highest MVA value
            if(MVAResult.first != previousMaxMVA)
                cout << "MVAResult.first != previousMaxMVA ???" << endl;

            //only the jets from the hadronic top should be matched
            ////cout<<"   ------> according to JetCombiner MVA: MVAResult.second[0] = "<<MVAResult.second[0]<<", MVAResult.second[1] = "<<MVAResult.second[1]<<", MVAResult.second[2] = "<<MVAResult.second[2]<<endl;
            hadronictopJetsMatched_MVAdef_ = false;
            if ( ((MVAResult.second[0] == hadronicWJet1_.first && MVAResult.second[1] ==  hadronicWJet2_.first)
                    || (MVAResult.second[0] == hadronicWJet2_.first && MVAResult.second[1] == hadronicWJet1_.first))
                    && MVAResult.second[2] == hadronicBJet_.first)
                hadronictopJetsMatched_MVAdef_ = true;

            float mW_maxMVA = (selectedJets[MVAResult.second[0]] + selectedJets[MVAResult.second[1]]).M();
            float mTop_maxMVA = (selectedJets[MVAResult.second[0]] + selectedJets[MVAResult.second[1]] + selectedJets[MVAResult.second[2]]).M();

            MSPlot_["maxMVA_btag"]->Fill(maxMVA_btag, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_ThPtOverSumPt"]->Fill(maxMVA_ThPtOverSumPt, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThWh"]->Fill(maxMVA_AngleThWh, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThBl"]->Fill(maxMVA_AngleThBl, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_AngleThBh"]->Fill(maxMVA_AngleThBh, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrWmass"]->Fill(maxMVA_HadrWmass, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrWMassCalc"]->Fill(mW_maxMVA, dataSet, true, Luminosity_*scaleFactor);
            MSPlot_["maxMVA_HadrTopMass"]->Fill(mTop_maxMVA, dataSet, true, Luminosity_*scaleFactor);

            string title = "MaxMVA_"+MVAnames[i]+"_allcomb";
            string titleSemiMuBG = "MaxMVA_"+MVAnames[i]+"_allSemiMuBGcomb"; //alSemiMulBGcomb contains badcomb events and also events where the mcParticle matching was not successful
            string axisTitle = "MVA "+MVAnames[i]+" response";

            // check if plots exist, else create them
            if (MSPlot_.find(title) == MSPlot_.end())
                MSPlot_[title] = new MultiSamplePlot(datasets_, title.c_str(), 20, 0, 1, axisTitle.c_str());
            if(histo1D_.find(titleSemiMuBG) == histo1D_.end())
                histo1D_[titleSemiMuBG] = new TH1F(titleSemiMuBG.c_str(),axisTitle.c_str(),50,0,1);

            MSPlot_[title]->Fill(MVAResult.first, dataSet, true, Luminosity_*scaleFactor);
            if( hadronictopJetsMatched_MVAdef_ == false && dataSetName.find("TTbarJets_SemiMu") == 0 )
                histo1D_[titleSemiMuBG]->Fill(MVAResult.first);

            if(isSemiLep || (TprimeEvaluation && mcParticlesForMatching.size()==4)) {
                std::string titleGood = "MaxMVA_"+MVAnames[i]+"_goodcomb";
                std::string titleBad = "MaxMVA_"+MVAnames[i]+"_badcomb";
                std::string axisTitle = "MVA "+MVAnames[i]+" response";
                std::string titleMtopGood = "Mtop_forMaxMVA_"+MVAnames[i]+"_goodcomb";
                std::string titleMtopBad = "Mtop_forMaxMVA_"+MVAnames[i]+"_badcomb";

                // check if goodcomb badcomb histo exists, else create it
                if (histo1D_.find(titleGood) == histo1D_.end()) {
                    histo1D_[titleGood] = new TH1F(titleGood.c_str(),axisTitle.c_str(),50,0,1);
                    histo1D_[titleBad] = new TH1F(titleBad.c_str(),axisTitle.c_str(),50,0,1);
                    histo1D_[titleMtopGood] = new TH1F(titleGood.c_str(),axisTitle.c_str(),50,0,500);
                    histo1D_[titleMtopBad] = new TH1F(titleBad.c_str(),axisTitle.c_str(),50,0,500);
                    }

                //cout << MVAnames[i] << " " << MVAResult->first << " " << hadronictopJetsMatched_MVAdef_ << endl;
                if ( hadronictopJetsMatched_MVAdef_ ) {
                    histo1D_[titleGood]->Fill( MVAResult.first );
                    histo1D_[titleMtopGood]->Fill( mTop_maxMVA );
                    }
                else if ( hadronictopJetsMatched_MCdef_ ) {
                    histo1D_[titleBad]->Fill( MVAResult.first );
                    histo1D_[titleMtopBad]->Fill( mTop_maxMVA );
                    }
                }
            }
        }
    }


void JetCombiner::FillResolutions(ResolutionFit* resFitLightJets, ResolutionFit* resFitBJets, ResolutionFit* resFitBJets_B, ResolutionFit* resFitBJets_Bbar) {
    // Fill the resolution-stuff6for events where the 4 ttbar semi-lep partons are all matched to jets
    if(hadronicWJet1_.first < 9999 && hadronicWJet2_.first < 9999 && hadronicBJet_.first < 9999 && leptonicBJet_.first < 9999) {
        resFitLightJets->Fill(selectedJets_[hadronicWJet1_.first], mcParticlesMatching_[hadronicWJet1_.second]);
        resFitLightJets->Fill(selectedJets_[hadronicWJet2_.first], mcParticlesMatching_[hadronicWJet2_.second]);
        resFitBJets->Fill(selectedJets_[hadronicBJet_.first], mcParticlesMatching_[hadronicBJet_.second]);
        resFitBJets->Fill(selectedJets_[leptonicBJet_.first], mcParticlesMatching_[leptonicBJet_.second]);
        if( resFitBJets_B && resFitBJets_Bbar ) {
            if( mcParticlesMatching_[hadronicBJet_.second]->type() == 5 ) {
                resFitBJets_B->Fill(selectedJets_[hadronicBJet_.first], mcParticlesMatching_[hadronicBJet_.second]);
                resFitBJets_Bbar->Fill(selectedJets_[leptonicBJet_.first], mcParticlesMatching_[leptonicBJet_.second]);
                }
            else {
                resFitBJets_B->Fill(selectedJets_[leptonicBJet_.first], mcParticlesMatching_[leptonicBJet_.second]);
                resFitBJets_Bbar->Fill(selectedJets_[hadronicBJet_.first], mcParticlesMatching_[hadronicBJet_.second]);
                }
            }
        }
    }

pair<float, vector<unsigned int> > JetCombiner::getMVAValue(string MVAMethod, int rank) {
    if(vectorMVA_.size() == 0)
        cout << "LightJetCombiner:  vectorMVA_.size() == 0   Was this event processed by the LightJetCombiner??" << endl;
    if(rank < 1 && rank > 12)
        cout << "LightJetCombiner: Wrong rank!!   rank = " << rank << endl;

    vector<MVAValues> MVAvalues;


//  cout <<"size of mva vals vector "<< vectorMVA_.size() << "1, 2, 3 "<< vectorMVA_[0].MVAValue <<"  "<< vectorMVA_[1].MVAValue <<"  "<< vectorMVA_[2].MVAValue <<endl;
    for(unsigned int i=0; i<vectorMVA_.size(); i++) {
        if( vectorMVA_[i].MVAAlgo.find(MVAMethod) == 0 ) {
            MVAvalues.push_back(vectorMVA_[i]);
            // cout <<"pushing back mva vals"<< endl;
            }
        }
    sort( MVAvalues.begin(), MVAvalues.end(), sortMVAValues );

    // cout <<"MVA value 0  "<<MVAvalues[0].MVAValue  <<  " MVA value 1  "<<MVAvalues[1].MVAValue   <<  " MVA value 2  "<<MVAvalues[2].MVAValue <<  " MVA value 3  "<<MVAvalues[3].MVAValue   << " MVA value 4  "<<MVAvalues[4].MVAValue <<  " MVA value 5  "<<MVAvalues[5].MVAValue  << " MVA value 6  "<<MVAvalues[6].MVAValue <<  " MVA value 7  "<<MVAvalues[7].MVAValue <<endl;
    //cout <<" "<<endl;

    pair<float, vector<unsigned int> > out;
    vector<unsigned int> tempVec;
    tempVec.push_back(MVAvalues[rank-1].WJet1);
    tempVec.push_back(MVAvalues[rank-1].WJet2);
    tempVec.push_back(MVAvalues[rank-1].HadrBJet);
    tempVec.push_back(MVAvalues[rank-1].LeptBJet);
    out.second = tempVec;
    out.first = MVAvalues[rank-1].MVAValue;
    return out;
    }

bool JetCombiner::isGoodJetCombination(string MVAMethod, int rank, bool fullcombination) {
    bool isGoodJetComb_ = false;

    vector<float> MVAvalues;

    for(unsigned int i=0; i<vectorMVA_.size(); i++) {
        if( vectorMVA_[i].MVAAlgo.find(MVAMethod) == 0 )
            MVAvalues.push_back(vectorMVA_[i].MVAValue);
        }

    sort( MVAvalues.begin(), MVAvalues.end(), sortFloat );
    for(unsigned int i=0; i<vectorMVA_.size(); i++) {
        if( vectorMVA_[i].MVAAlgo.find(MVAMethod) == 0 && vectorMVA_[i].MVAValue == MVAvalues[rank-1] ) {
            if((( (unsigned int) vectorMVA_[i].WJet1 == hadronicWJet1_.first &&  (unsigned int) vectorMVA_[i].WJet2 == hadronicWJet2_.first)
                    || ( (unsigned int) vectorMVA_[i].WJet1 == hadronicWJet2_.first &&  (unsigned int) vectorMVA_[i].WJet2 ==	hadronicWJet1_.first))
                    &&  (unsigned int) vectorMVA_[i].HadrBJet == hadronicBJet_.first) {
                if(fullcombination == false)
                    isGoodJetComb_ = true;
                else if( (unsigned int) vectorMVA_[i].LeptBJet == leptonicBJet_.first)
                    isGoodJetComb_ = true;
                }
            }
        }
    return isGoodJetComb_;
    }

vector<unsigned int> JetCombiner::GetGoodJetCombination() {
    vector<unsigned int> outCombi;
    outCombi.push_back(hadronicWJet1_.first);
    outCombi.push_back(hadronicWJet2_.first);
    outCombi.push_back(hadronicBJet_.first);
    outCombi.push_back(leptonicBJet_.first);

    return outCombi;
    }

TLorentzVector JetCombiner::GetHadrBQuark() {
    if(hadronicBJet_.second < 9999) return *mcParticlesMatching_[hadronicBJet_.second];
    else return TLorentzVector(0,0,0,0);
    }

TLorentzVector JetCombiner::GetLightQuark1() {
    if(hadronicWJet1_.second < 9999) return *mcParticlesMatching_[hadronicWJet1_.second];
    else return TLorentzVector(0,0,0,0);
    }

TLorentzVector JetCombiner::GetLightQuark2() {
    if(hadronicWJet2_.second < 9999) return *mcParticlesMatching_[hadronicWJet2_.second];
    else return TLorentzVector(0,0,0,0);
    }

TLorentzVector JetCombiner::GetLeptBQuark() {
    if(leptonicBJet_.second < 9999) return *mcParticlesMatching_[leptonicBJet_.second];
    else return TLorentzVector(0,0,0,0);
    }

void JetCombiner::EndJob() {
    cerr << " in end job ..." << endl;
    EndJobWasRun_ = true;

    if(trainMVA_)
        trainer_->TrainMVA("Block","",0,0,"",0,0,postfix_, false);

    graphAsymmErr_["PtJetCutEfficiency"] = GraphRatio(histo1D_["PtJetCut_nEventsAfter"],histo1D_["PtJetCut_nEventsBefore"],"PtJetCutEfficiency","cut on p_{T}^{4 leading jets}","Fraction with >= 4 jets");
    graphAsymmErr_["PtJetCut4PartonsMatchingEfficiency"] = GraphRatio(histo1D_["PtJetCut_nEvents4PartonsMatchedAfter"],histo1D_["PtJetCut_nEventsAfter"],"PtJetCut4PartonsMatchingEfficiency","cut on p_{T}^{jets}","Fraction with >= 4 matched jets");
    graphAsymmErr_["PtJetCut4PartonsMatchedSelectionEfficiency"] = GraphRatio(histo1D_["PtJetCut_nEvents4PartonsMatchedAfter"],histo1D_["PtJetCut_nEvents4PartonsMatchedBefore"],"PtJetCut4PartonsMatchedSelectionEfficiency","cut on p_{T}^{jets}","Event selection efficiency");
    graphAsymmErr_["PtJetCut4JetsMatchingEfficiency"] = GraphRatio(histo1D_["PtJetCut_nEvents4JetsMatchedAfter"],histo1D_["PtJetCut_nEventsAfter"],"PtJetCut4JetsMatchingEfficiency","cut on p_{T}^{4 leading jets}","Fraction with >= 4 matched jets");
    graphAsymmErr_["PtJetCut4JetsMatchedSelectionEfficiency"] = GraphRatio(histo1D_["PtJetCut_nEvents4JetsMatchedAfter"],histo1D_["PtJetCut_nEvents4JetsMatchedBefore"],"PtJetCut4JetsMatchedSelectionEfficiency","cut on p_{T}^{4 leading jets}","Event selection efficiency");
    graphAsymmErr_["PtJetCutBadMatchingISRFraction"] = GraphRatio(histo1D_["PtJetCut_nEventsAfterBadISR"],histo1D_["PtJetCut_nEventsAfter"],"PtJetCutBadMatchingISRFraction","cut on p_{T}^{4 leading jets}","Fraction with unmatched (ISR) in 4 leading jets");
    graphAsymmErr_["PtJetCutBadMatchingNoISRFraction"] = GraphRatio(histo1D_["PtJetCut_nEventsAfterBadNoISR"],histo1D_["PtJetCut_nEventsAfter"],"PtJetCutBadMatchingNoISRFraction","cut on p_{T}^{4 leading jets}","Fraction with unmatched (no ISR) in 4 leading jets");

    }

void JetCombiner::Write(TFile* fout, bool savePNG, string pathPNG, bool plotMVAstuff) {
    cerr << " - Writing out the JetCombiner stuff ..." << endl;

    if( !EndJobWasRun_ )
        EndJob();

    cout <<" post end job./"<<endl;

    //  string pathPNGExpCorr = pathPNG + "../ExpCorr/";

    fout->cd();
    string dirname = "1D_histograms_JetCombiner" + MVAfilePostfix_;
    if(fout->Get(dirname.c_str())==0)
        fout->mkdir(dirname.c_str());
    fout->cd(dirname.c_str());

    // Write MVA distributions
    if( !trainMVA_ && plotMVAstuff ) {

        cerr <<"in write MVA stuff..."<<endl;
        map<string,Float_t> MVAVals = computer_->GetMVAValues();
        vector< pair<TGraph*, string> > effpurplots;
        vector< pair<TGraph*, string> > effpurplotsAllSemiMuBG;

        for (map<string,Float_t>::const_iterator it = MVAVals.begin(); it != MVAVals.end(); ++it) {
            vector< pair<TH1F*,string> > histos;
            string titleGood = "MaxMVA_"+it->first+"_goodcomb";
            string titleBad = "MaxMVA_"+it->first+"_badcomb";
            string titleSemiMuBG = "MaxMVA_"+it->first+"_allSemiMuBGcomb";
            string titleMtopGood = "Mtop_forMaxMVA_"+it->first+"_goodcomb";
            string titleMtopBad = "Mtop_forMaxMVA_"+it->first+"_badcomb";

            if (histo1D_.find(titleGood) != histo1D_.end()) {
                string label = "Maximal MVA value using method: "+it->first;
                string label_Mtop = "m_{bW} for the maximal MVA value";

                /////////////
                // S/(S+B) //
                /////////////
                TH1F* newB = (TH1F*)histo1D_[titleBad]->Clone();
                newB->Add(histo1D_[titleGood]);
                TGraphAsymmErrors* SoverSB = new TGraphAsymmErrors(histo1D_[titleGood],newB);

                SoverSB->SetTitle((it->first+" response: S/(S+B)").c_str());
                SoverSB->GetXaxis()->SetTitle((it->first+" response").c_str());
                SoverSB->GetYaxis()->SetTitle("S/(S+B)");
                cerr << " pre canvas write"   << endl;


                TCanvas* tempCanvas2 = TCanvasCreator(SoverSB, "c1");
                cerr << " pre canvas write 1"   << endl;

                if(savePNG) tempCanvas2->SaveAs( (pathPNG+"SoverSB_"+it->first+".png").c_str() );
                cerr << " pre canvas write  2"   << endl;

                tempCanvas2->Write();

                cerr << " post canvas write"   << endl;

                ///////////////////////////////////
                // make efficiency vs purity plot
                ///////////////////////////////////
                int nVals = 0;

                cerr << " post canvas write 2"   << endl;


                // calculate efficiency and purity
                for (double cut=0; cut < 1; cut+=0.01) {
                    cerr << " n eff loop"   << endl;


                    //cerr << " - eff and puirty integral" << histo1D_[titleGood]->Integral()   << " cut   "<<  cut<<endl;

                    Int_t cutBin = histo1D_[titleGood]->GetXaxis()->FindBin(cut);

                    Double_t nSignalAbove = histo1D_[titleGood]->Integral(cutBin,histo1D_[titleGood]->GetNbinsX());
                    Double_t nSignalBelow = histo1D_[titleGood]->Integral(0,cutBin);

                    //   Double_t nSignalAbove = 1.;
                    //   Double_t nSignalBelow = 1.;
                    // Double_t nBadCombAbove =1.;
                    // Double_t nAllSemiMuBGCombAbove = 1.;

                    //cout <<"Cut " << cut << " -> " <<  cutBin << " " << nSignalBelow << " " << nSignalAbove << " " << nBadCombAbove <<" "<< nAllSemiMuBGCombAbove<<endl;




                    nVals++;
                    cout <<"Cut " << cut << " -> " <<  cutBin << " " << nSignalBelow << " " << nSignalAbove << " " << endl;
                    }

                //removing refs to problematic arrays...
                //    TGraph* plot = new TGraph(nVals,x,yBadComb);
                TGraph* plot = new TGraph();

                effpurplots.push_back(pair<TGraph*,string>(plot,it->first));
                //TGraph* plotallSemiMuBG = new TGraph(nVals,x,yAllSemiMuBGComb);
                TGraph* plotallSemiMuBG = new TGraph();
                effpurplotsAllSemiMuBG.push_back(pair<TGraph*,string>(plotallSemiMuBG,it->first));

                //  cout << "   - MVA method: " << it->first << " ||  Eff for SemiLep-Matched: " << yBadComb[0] << " ||  Eff for All SemiLep: " << yAllSemiMuBGComb[0] << endl;
                // cout << "                 nGoodCombi: " << histo1D_[titleGood]->GetEffectiveEntries() << " || nBadCombi: " << histo1D_[titleBad]->GetEffectiveEntries() << endl;


                //////////////////////////////////////
                // good overlayed with bad jet combi
                //////////////////////////////////////
                histo1D_[titleGood]->GetXaxis()->SetTitle((it->first+" Response").c_str());
                histo1D_[titleBad]->GetXaxis()->SetTitle((it->first+" Response").c_str());
                histo1D_[titleGood]->GetYaxis()->SetTitle("a.u.");
                histo1D_[titleBad]->GetYaxis()->SetTitle("a.u.");
                histo1D_[titleGood]->Scale(1/histo1D_[titleGood]->Integral()); //normalize to unity
                histo1D_[titleBad]->Scale(1/histo1D_[titleBad]->Integral()); //normalize to unity

                histos.push_back(std::pair<TH1F*,std::string>(histo1D_[titleGood],"Good Jet Combinations"));
                histos.push_back(std::pair<TH1F*,std::string>(histo1D_[titleBad],"Bad Jet Combinations"));

                TCanvas* tempCanvas = TCanvasCreator(histos, "LF",label,false);
                if(savePNG) tempCanvas->SaveAs( (pathPNG+"MaxMVA_"+it->first+".png").c_str() );

                tempCanvas->Write();

                cerr << " pre histos clear"   << endl;

                //same procedure for top mass
                histos.clear();
                cerr << " post histos clear"   <<titleMtopGood <<endl;

                if (histo1D_[titleMtopGood]) {
                    histo1D_[titleMtopGood]->GetXaxis()->SetTitle("m_{bW} (GeV)");
                    histo1D_[titleMtopBad]->GetXaxis()->SetTitle("m_{bW} (GeV)");
                    histo1D_[titleMtopGood]->GetYaxis()->SetTitle("a.u.");
                    histo1D_[titleMtopBad]->GetYaxis()->SetTitle("a.u.");

                    cerr << " mtop good integral "   << histo1D_[titleMtopGood]->Integral() <<" "<< histo1D_[titleMtopBad]->Integral()<< endl;


                    histo1D_[titleMtopGood]->Scale(1/histo1D_[titleMtopGood]->Integral()); //normalize to unity
                    histo1D_[titleMtopBad]->Scale(1/histo1D_[titleMtopBad]->Integral()); //normalize to unity

                    histos.push_back(std::pair<TH1F*,std::string>(histo1D_[titleMtopGood],"Good Jet Combinations"));
                    histos.push_back(std::pair<TH1F*,std::string>(histo1D_[titleMtopBad],"Bad Jet Combinations"));

                    }

                TCanvas* tempCanvas_Mtop = TCanvasCreator(histos, "LF",label_Mtop,false);
                if(savePNG) tempCanvas_Mtop->SaveAs( (pathPNG+"Mtop_forMaxMVA_"+it->first+".png").c_str() );
                cerr << " pre mtop write"   << endl;

                tempCanvas_Mtop->Write();


                delete tempCanvas;
                delete tempCanvas2;
                delete tempCanvas_Mtop;

                histos.clear();
                }
            }

        ///////////////////////////////
        // WRITE EFF PUR plots canvas
        ///////////////////////////////
        string effpurtitle= "Eff_Pur_MVA";

        if (effpurplots.size() > 0) {
            effpurplots[0].first->GetXaxis()->SetTitle("Efficiency");
            effpurplots[0].first->GetYaxis()->SetTitle("Purity");
            effpurplots[0].first->SetTitle("Efficiency VS purity MVA discriminator cut, only successfull mc-matched hadronic tops");
            effpurplots[0].first->SetLineWidth(3);
            }
        TCanvas* tempCanvas3 = TCanvasCreator(effpurplots, effpurtitle.c_str());
        if(savePNG) tempCanvas3->SaveAs( (pathPNG+effpurtitle+".png").c_str() );
        tempCanvas3->Write();

        effpurtitle= "Eff_Pur_MVA_AllSemiMuBG";
        if (effpurplotsAllSemiMuBG.size() > 0) {
            effpurplotsAllSemiMuBG[0].first->GetXaxis()->SetTitle("Efficiency");
            effpurplotsAllSemiMuBG[0].first->GetYaxis()->SetTitle("Purity");
            effpurplotsAllSemiMuBG[0].first->SetTitle("Efficiency VS purity MVA discriminator cut, all selected TTbar semi-lep events");
            effpurplotsAllSemiMuBG[0].first->SetLineWidth(3);
            }
        TCanvas* tempCanvas5 = TCanvasCreator(effpurplotsAllSemiMuBG, effpurtitle.c_str());
        if(savePNG) tempCanvas5->SaveAs( (pathPNG+effpurtitle+".png").c_str() );
        tempCanvas5->Write();
        cout << "Done with complicated MVA plotting" << endl;
        } // done with MVA plotting

    if(savePNG) mkdir( (pathPNG+"MSPlot/").c_str(),0777);
    // Write out the MSPlot
    for(map<string,MultiSamplePlot*>::const_iterator it = MSPlot_.begin(); it != MSPlot_.end(); it++) {

        cout << "writing ms plots: " << it->first + MVAfilePostfix_<<endl;

        MultiSamplePlot *temp = it->second;
        string name = it->first + MVAfilePostfix_;
        temp->Draw(name, 0, false, false, false, 1);
        temp->Write(fout, name, savePNG, pathPNG+"MSPlot/");
        }

    for(std::map<std::string,TH1F*>::const_iterator it = histo1D_.begin(); it != histo1D_.end(); it++) {
        cout << "writing 1d histos" << endl;


        TH1F *temp = it->second;
        if (temp) {
            int N = temp->GetNbinsX();
            temp->SetBinContent(N,temp->GetBinContent(N)+temp->GetBinContent(N+1));
            temp->SetBinContent(N+1,0);
            temp->SetEntries(temp->GetEntries()-2); // necessary since each SetBinContent adds +1 to the number of entries...
            temp->Write();
            TCanvas* tempCanvas = TCanvasCreator(temp, it->first);
            if(savePNG) tempCanvas->SaveAs( (pathPNG+it->first+".png").c_str() );

            }
        }

    cout << "out of 1d loop" << endl;


    // 2D
    string dirname2D = "2D_histograms_graphs_JetCombiner" + MVAfilePostfix_;
    if(fout->Get(dirname.c_str())==0)
        fout->mkdir(dirname.c_str());
    fout->cd(dirname.c_str());

    for(std::map<std::string,TH2F*>::const_iterator it = histo2D_.begin(); it != histo2D_.end(); it++) {
        cout << "writing 2d histos" << endl;

        TH2F *temp = it->second;
        temp->Write();
        TCanvas* tempCanvas = TCanvasCreator(temp, it->first);
        if(savePNG) tempCanvas->SaveAs( (pathPNG+it->first+".png").c_str() );
        }

    //Write TGraphAsymmErrors
    for(map<string,TGraphAsymmErrors*>::const_iterator it = graphAsymmErr_.begin(); it != graphAsymmErr_.end(); it++) {
        cout << "writing TGraphs..." << endl;

        TGraphAsymmErrors *temp = it->second;
        temp->Write();
        TCanvas* tempCanvas = TCanvasCreator(temp, it->first);
        if(savePNG) tempCanvas->SaveAs( (pathPNG+it->first+".png").c_str() );
        }
    }
