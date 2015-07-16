//////////////////////////////////////////////
//          Make cutflow table              //
//////////////////////////////////////////////

#include "../interface/HadronicTopReco.h"

HadronicTopReco::HadronicTopReco(TFile *fout, bool isMuon, bool isElectron, bool TrainMVA, vector < Dataset* > datasets, string MVAmethodIn, bool isdebug, float Lumi):
	MSPlot(),
	debug(false),
	leptonChoice(""),
	selectedLeptonTLV_JC(),
	mcParticlesTLV(), 
	selectedJetsTLV(),
	mcMuonsTLV(),
	mcParticlesMatching_(),
	leptonicBJet_(),
	hadronicBJet_(),
	hadronicWJet1_(),
	hadronicWJet2_(), //First index is the JET number(), second one is the parton
	mcParticles_flav(),
	JetPartonPair(),
	TriJetMass(0), DiJetMass(0),
	Topness(0), MultiTopness(0), DiTopness(0), TriTopness(0),
	MVAvals1(), //BDT discriminator value of highest ranked tri jet
	MVAvals2ndPass(), //BDT discriminator value of second highest ranked tri jet
	MVAvals3rdPass(), //BDT discriminator value of third highest ranked tri jet (if possible)
	selectedJets2ndPass(), //remaining jets after removing highest ranked tri jet
	selectedJets3rdPass(), //remaining jets after removing highest & second highest ranked tri jet
	MVASelJets1(), //the selected jets from the highest ranked tri jet
	MVASelJets2(), //the selected jets from the second highest ranked tri jet
	jetCombiner(new JetCombiner(TrainMVA, Lumi, datasets, MVAmethodIn, false)),
	genEvt_flav(0),
	bestTopMass1(0),
	bestTopMass2(0),
	bestTopMass2ndPass(0), 
	bestTopPt(0),
	nMVASuccesses(0),
	scaleFactor(0),
	Luminosity(Lumi),
	MVAmethod(MVAmethodIn),
	wj1(0), wj2(0), bj1(0), wj1_2ndpass(0), wj2_2ndpass(0), bj1_2ndpass(0),
	AngleT1T22ndpass(0),
	AngleT1Lep(0)
	{
	if (isMuon){
		leptonChoice = "Muon";
	}
	else if (isElectron){
		leptonChoice = "Electron";
	}

	if(isdebug)	debug = true;
	
	//fout->cd();
	MSPlot["MVA1TriJetMass"]       = new MultiSamplePlot(datasets, "MVA1TriJetMass", 75, 0, 500, "m_{bjj}");
	MSPlot["MVA1DiJetMass"]        = new MultiSamplePlot(datasets, "MVA1DiJetMass", 75, 0, 500, "m_{bjj}");
	MSPlot["MVA1PtRat"]            = new MultiSamplePlot(datasets, "MVA1PtRat", 25, 0, 2, "P_{t}^{Rat}");
	MSPlot["MVA1BTag"]             = new MultiSamplePlot(datasets, "MVA1BTag", 35, 0, 1, "BTag");
	MSPlot["MVA1AnThBh1"]          = new MultiSamplePlot(datasets, "MVA1AnThBh1", 35, 0, 3.14, "AnThBh");
	MSPlot["MVA1AnThWh1"]          = new MultiSamplePlot(datasets, "MVA1AnThWh1", 35, 0, 3.14, "AnThWh1");
	MSPlot["MVA1TriJet"]           = new MultiSamplePlot(datasets, "MVA1TriJet", 35, -1., 0.5, "BDT Discriminator");
	MSPlot["MultiTopness"]         = new MultiSamplePlot(datasets, "MultiTopness", 35, -1., 0.5, "MultiTopness");
	MSPlot["Topness"]              = new MultiSamplePlot(datasets, "Topness", 35, -1., 0.5, "Topness");
	MSPlot["DiTopness"]            = new MultiSamplePlot(datasets, "DiTopness", 35, -1., 0.5, "DiTopness");
	MSPlot["TriTopness"]           = new MultiSamplePlot(datasets, "TriTopness", 35, -1., 0.5, "TriTopness");
	MSPlot["MVA2ndPassTriJetMass"] = new MultiSamplePlot(datasets, "MVA2ndPassTriJetMass", 30, 0, 1000, "m_{bjj}");
	MSPlot["MVA_AnTop1Top2"]       = new MultiSamplePlot(datasets, "MVAAnTop1Top2", 35, 0, 3.14, "Angle first two hadronic tops");
	MSPlot["MVA_AnTop1Lep"]		   = new MultiSamplePlot(datasets, "MVAAnTop1Lep", 35, 0, 3.14, "Angle first top and lepton");
}

HadronicTopReco::~HadronicTopReco(){
    for(map<string,MultiSamplePlot*>::const_iterator it = MSPlot.begin(); it != MSPlot.end(); it++)
    {
        string name = it->first;
		MSPlot.erase(name);
    }
}

void HadronicTopReco::SetCollections(vector<TRootPFJet*> selectJets,  vector<TRootMuon*> selectedMuons, vector<TRootElectron*> selectedElectrons, float scaleFac){
	if (leptonChoice == "Muon"){
	    selectedLeptonTLV_JC.push_back(selectedMuons[0]);               
	}
	else if (leptonChoice == "Electron"){
	    selectedLeptonTLV_JC.push_back(selectedElectrons[0]);               
	}
	scaleFactor = scaleFac;
}

void HadronicTopReco::Compute1st(unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets){
	selectedJets2ndPass.clear();
	selectedJets3rdPass.clear();
	MVASelJets1.clear();   

    jetCombiner->ProcessEvent_SingleHadTop(datasets[d], mcParticles_flav, selectedJets, selectedLeptonTLV_JC[0], genEvt_flav, scaleFactor);


	if (debug) cout <<"Processing event with jetcombiner :  "<< endl;

    MVAvals1 = jetCombiner->getMVAValue(MVAmethod, 1); // 1 means the highest MVA value
    Topness = MVAvals1.first;

    MSPlot["Topness"]->Fill(Topness, datasets[d], true, Luminosity*scaleFactor );

    if (debug) cout <<"Processing event with jetcombiner : 1 "<< endl;
}

void HadronicTopReco::Compute2nd(unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets){
	selectedJets2ndPass.clear();
	MVASelJets1.clear();   

    //make vector of jets excluding these selected by 1st pass of mass reco
    for (unsigned int seljet1 =0; seljet1 < selectedJets.size(); seljet1++ ){
        if (seljet1 == MVAvals1.second[0] || seljet1 == MVAvals1.second[1] || seljet1 == MVAvals1.second[2]){ 
            MVASelJets1.push_back(selectedJets[seljet1]);
            continue;
        }
        selectedJets2ndPass.push_back(selectedJets[seljet1]);
    }

    //Perform jet combiner a second time with top tri-jet removed
    jetCombiner->ProcessEvent_SingleHadTop(datasets[d], mcParticles_flav, selectedJets2ndPass, selectedLeptonTLV_JC[0], genEvt_flav, scaleFactor);
 

    MVAvals2ndPass = jetCombiner->getMVAValue(MVAmethod, 1);
    DiTopness = MVAvals2ndPass.first;
    MSPlot["DiTopness"]->Fill(DiTopness,  datasets[d], true, Luminosity*scaleFactor);
    if (debug) cout <<"Processing event with jetcombiner : 2 "<< endl;
}

void HadronicTopReco::Compute3rd(unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets){
	selectedJets3rdPass.clear();

    //make vector of jets excluding these selected by 1st pass of mass reco
    for (unsigned int seljet1 =0; seljet1 < selectedJets2ndPass.size(); seljet1++ ){
        if (seljet1 == MVAvals2ndPass.second[0] || seljet1 == MVAvals2ndPass.second[1] || seljet1 == MVAvals2ndPass.second[2]){ 
            MVASelJets2.push_back(selectedJets2ndPass[seljet1]);
            continue;
        }
        selectedJets3rdPass.push_back(selectedJets2ndPass[seljet1]);
    }

    if (selectedJets3rdPass.size()>=3) {
	     //Perform jet combiner a third time with top tri-jet removed if there are 3 jets remaining, otherwise it's not possible
	    jetCombiner->ProcessEvent_SingleHadTop(datasets[d], mcParticles_flav, selectedJets3rdPass, selectedLeptonTLV_JC[0], genEvt_flav, scaleFactor);

	    MVAvals3rdPass = jetCombiner->getMVAValue(MVAmethod, 1);
	    TriTopness = MVAvals3rdPass.first;
	    MSPlot["TriTopness"]->Fill(TriTopness,  datasets[d], true, Luminosity*scaleFactor);   	
    }

}

void HadronicTopReco::FillDiagnosticPlots(TFile *fout, unsigned int d, vector<TRootPFJet*> selectedJets, vector < Dataset* > datasets){

    //check data-mc agreement of kin. reco. variables.
    float mindeltaR =100.;
    float mindeltaR_temp =100.;
 	wj1 = 0, wj2 = 0, bj1 = 0, wj1_2ndpass = 0, wj2_2ndpass = 0, bj1_2ndpass = 0;  //wjet1, wjet2, bjet

    //define the jets from W as the jet pair with smallest deltaR
    for (unsigned int m=0; m<MVASelJets1.size(); m++) {
        for (unsigned int n=0; n<MVASelJets1.size(); n++) {
            if(n==m) continue;
            TLorentzVector lj1  = *MVASelJets1[m];
            TLorentzVector lj2  = *MVASelJets1[n];
            mindeltaR_temp  = lj1.DeltaR(lj2);
            if (mindeltaR_temp < mindeltaR){
                mindeltaR = mindeltaR_temp;
                wj1 = m;
                wj2 = n;
            }
        }
    }

    // find the index of the jet not chosen as a W-jet
    for (unsigned int p=0; p<MVASelJets1.size(); p++) {
        if(p!=wj1 && p!=wj2) bj1 = p;
    }

    if (debug) cout <<"Processing event with jetcombiner : 3 "<< endl;

    //now that putative b and W jets are chosen, calculate the six kin. variables.
    TLorentzVector Wh1 = *MVASelJets1[wj1] + *MVASelJets1[wj2]; //lorentz vector of dijet
    TLorentzVector Bh1 = *MVASelJets1[bj1]; //lorentz vector of b
    TLorentzVector Th1 = Wh1+Bh1; //lorentz vector of top

    TriJetMass = Th1.M();
    DiJetMass = Wh1.M();

    //DeltaR
    float AngleThWh1 = fabs(Th1.DeltaPhi(Wh1)); //angle between top and dijet
    float AngleThBh1 = fabs(Th1.DeltaPhi(Bh1)); //angle between top and remaining jet

    float btag = MVASelJets1[bj1]->btag_combinedSecondaryVertexBJetTags();  //CSV discriminator value of "other jet"

    double PtRat = ( ( *MVASelJets1[0] + *MVASelJets1[1] + *MVASelJets1[2] ).Pt() ) / ( MVASelJets1[0]->Pt() + MVASelJets1[1]->Pt() + MVASelJets1[2]->Pt());  //ratio of vectorial pT over scalar pT (should be larger for tops)

    if (debug) cout <<"Processing event with jetcombiner : 4 "<< endl;    

    bestTopMass1       = ( *selectedJets[MVAvals1.second[0]] + *selectedJets[MVAvals1.second[1]] + *selectedJets[MVAvals1.second[2]]).M();
    bestTopMass2ndPass = ( *selectedJets[MVAvals2ndPass.second[0]] + *selectedJets[MVAvals2ndPass.second[1]] + *selectedJets[MVAvals2ndPass.second[2]]).M(); //top mass of second highest ranked tri jet
    bestTopPt          = ( *selectedJets[MVAvals1.second[0]] + *selectedJets[MVAvals1.second[1]] + *selectedJets[MVAvals1.second[2]]).Pt();

    if(debug) {
        cout <<"Indices of best MVA jets are :  "<< MVAvals1.second[0] <<"  "<< MVAvals1.second[1]  <<" " << MVAvals1.second[2]<<endl;
        cout <<"MVA Mass 1 = "<< bestTopMass1 << " MVA Mass 2 = "<< bestTopMass2ndPass << endl; cout <<"   "<<endl;
    }

    //define the jets from W as the jet pair with smallest deltaR
    for (unsigned int m=0; m<MVASelJets2.size(); m++) {
        for (unsigned int n=0; n<MVASelJets2.size(); n++) {
            if(n==m) continue;
            TLorentzVector lj1  = *MVASelJets2[m];
            TLorentzVector lj2  = *MVASelJets2[n];
            mindeltaR_temp  = lj1.DeltaR(lj2);
            if (mindeltaR_temp < mindeltaR){
                mindeltaR = mindeltaR_temp;
                wj1_2ndpass = m;
                wj2_2ndpass = n;
            }
        }
    }

    // find the index of the jet not chosen as a W-jet
    for (unsigned int p=0; p<MVASelJets2.size(); p++) {
        if(p!=wj1_2ndpass && p!=wj2_2ndpass) bj1_2ndpass = p;
    }

    if (debug) cout <<"Processing event with jetcombiner : 5 "<< endl;
    //now that putative b and W jets are chosen, calculate the six kin. variables.
    TLorentzVector Wh2ndpass = *MVASelJets1[wj1_2ndpass] + *MVASelJets1[wj2_2ndpass]; //lorentz vector of dijet
    TLorentzVector Bh2ndpass = *MVASelJets1[bj1_2ndpass]; //lorentz vector of b
    TLorentzVector Th2ndpass = Wh2ndpass+Bh2ndpass; //lorentz vector of top



    //DeltaR
    // float AngleThWh2ndpass = fabs(Th2ndpass.DeltaPhi(Wh2ndpass)); //angle between top and dijet
    // float AngleThBh2ndpass = fabs(Th2ndpass.DeltaPhi(Bh2ndpass)); //angle between top and remaining jet
    AngleT1T22ndpass = fabs(Th2ndpass.DeltaPhi(Th1)); //angle between had top 1 and had top 2
    AngleT1Lep = fabs(Th1.DeltaPhi(*selectedLeptonTLV_JC[0]));
    fout->cd();

    //cout<<"d: "<<d<<"   Lumi: "<<Luminosity<<"  scaleFactor: "<<scaleFactor<<endl;
    MSPlot["MVA1TriJetMass"]->Fill(TriJetMass,  datasets[d], true, Luminosity*scaleFactor );
    MSPlot["MVA1DiJetMass"]->Fill(DiJetMass,  datasets[d], true, Luminosity*scaleFactor );
    MSPlot["MVA1BTag"]->Fill(btag,  datasets[d], true, Luminosity*scaleFactor );
    MSPlot["MVA1PtRat"]->Fill(PtRat,  datasets[d], true, Luminosity*scaleFactor );
    MSPlot["MVA1AnThWh1"]->Fill(AngleThWh1,  datasets[d], true, Luminosity*scaleFactor );
    MSPlot["MVA1AnThBh1"]->Fill(AngleThBh1,  datasets[d], true, Luminosity*scaleFactor );
    MSPlot["MVA2ndPassTriJetMass"]->Fill(bestTopMass2ndPass,  datasets[d], true, Luminosity*scaleFactor );
    MSPlot["MVA_AnTop1Top2"]->Fill(AngleT1T22ndpass, datasets[d], true, Luminosity*scaleFactor);
    MSPlot["MVA_AnTop1Lep"]->Fill(AngleT1Lep, datasets[d], true, Luminosity*scaleFactor);
    if (debug)  cout <<"MVA Mass 1 = "<< bestTopMass1 << " MVA Mass 2 = "<< bestTopMass2 << endl;
}

float HadronicTopReco::ReturnAnglet1t2(){
	return AngleT1T22ndpass;
}

float HadronicTopReco::ReturnAngletoplep(){
	return AngleT1Lep;
}
void HadronicTopReco::ComputeMVASuccesses(){
    if(   ( hadronicBJet_.first == MVAvals1.second[0] || hadronicBJet_.first == MVAvals1.second[1] || hadronicBJet_.first == MVAvals1.second[2]   )  && ( hadronicWJet1_.first == MVAvals1.second[0] || hadronicWJet1_.first == MVAvals1.second[1] || hadronicWJet1_.first == MVAvals1.second[2]   )    && ( hadronicWJet2_.first == MVAvals1.second[0] || hadronicWJet2_.first == MVAvals1.second[1] || hadronicWJet2_.first == MVAvals1.second[2]   )      ){
	    nMVASuccesses++;
	}
}

int HadronicTopReco::ReturnMVASuccesses(){
	return nMVASuccesses;
}

float HadronicTopReco::ReturnTopness(){
	return Topness;
}

float HadronicTopReco::ReturnDiTopness(){
	return DiTopness;
}

float HadronicTopReco::ReturnTriTopness(){
	return TriTopness;
}

void HadronicTopReco::WriteDiagnosticPlots(TFile *fout, string pathPNG){
	for(map<string,MultiSamplePlot*>::const_iterator it = MSPlot.begin(); it != MSPlot.end(); it++)
    {
        string name = it->first;
        MultiSamplePlot *temp = it->second;
        temp->Write(fout, name, true, pathPNG, "pdf");
    }
}

void HadronicTopReco::RecoCheck(bool debug, vector<TRootMuon*> selectedMuons, vector<TRootElectron*> selectedElectrons, vector<TRootPFJet*> selectedJets){
	/////////////////////////////////
	/// Find indices of jets from Tops
	////////////////////////////////

	bool muPlusFromTop = false, muMinusFromTop = false;
	// bool elPlusFromTop = false, elMinusFromTop = false;

	leptonicBJet_ = hadronicBJet_ = hadronicWJet1_ = hadronicWJet2_ = pair<unsigned int,unsigned int>(9999,9999);

	//match jets to partons
	for(unsigned int i=0; i<selectedJets.size(); i++) selectedJetsTLV.push_back(*selectedJets[i]);
	    JetPartonMatching matching = JetPartonMatching(mcParticlesTLV, selectedJetsTLV, 2, true, true, 0.3);

	//loop through mc particles and find matched jets
	for(unsigned int i=0; i<mcParticlesTLV.size(); i++) 
	{
		int matchedJetNumber = matching.getMatchForParton(i, 0);
	    if(matchedJetNumber != -1)
	        JetPartonPair.push_back( pair<unsigned int, unsigned int> (matchedJetNumber, i) );// Jet index, MC Particle index
	}

	if (debug) cout <<"n sel jets  "<<selectedJets.size()  << "   n mc particles tlv : "<< mcParticlesTLV.size() << " jet parton pari size :   "<< JetPartonPair.size()<<"  "<< muPlusFromTop<<muMinusFromTop<<endl;

	for(unsigned int i=0; i<JetPartonPair.size(); i++)//looping through matched jet-parton pairs
	{
	    unsigned int j = JetPartonPair[i].second;     //get index of matched mc particle

	    if( fabs(mcParticlesMatching_[j]->type()) < 5 )
	    {
	        if( ( muPlusFromTop && mcParticlesMatching_[j]->motherType() == -24 && mcParticlesMatching_[j]->grannyType() == -6 )
	            || ( muMinusFromTop && mcParticlesMatching_[j]->motherType() == 24 && mcParticlesMatching_[j]->grannyType() == 6 ) )
	        {
	            if(hadronicWJet1_.first == 9999)
	            {
	                hadronicWJet1_ = JetPartonPair[i];
	                // MCPermutation[0] = JetPartonPair[i].first;
	            }
	            else if(hadronicWJet2_.first == 9999)
	            {
	                hadronicWJet2_ = JetPartonPair[i];
	                //MCPermutation[1] = JetPartonPair[i].first;
	            }
	            //else cerr<<"Found a third jet coming from a W boson Wh1ich comes from a top quark..."<<endl;
	        }
	    }
	    else if( fabs(mcParticlesMatching_[j]->type()) == 5 )
	    {

	        if(  ( muPlusFromTop && mcParticlesMatching_[j]->motherType() == -6) || ( muMinusFromTop && mcParticlesMatching_[j]->motherType() == 6 ) )
	        {
	            hadronicBJet_ = JetPartonPair[i];
	            //MCPermutation[2] = JetPartonPair[i].first;
	        }
	        else if((muPlusFromTop && mcParticlesMatching_[j]->motherType() == 6) ||  ( muMinusFromTop &&mcParticlesMatching_[j]->motherType() == -6) )
	        {
	            leptonicBJet_ = JetPartonPair[i];
	            //MCPermutation[3] = JetPartonPair[i].first;
	        }
	    }
	}

	if (debug) cout <<"Indices of matched jets are :  "<< hadronicBJet_.first<<"  "<< hadronicWJet1_.first  <<" " << hadronicWJet2_.first <<endl;

}
