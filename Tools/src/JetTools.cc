#include "../interface/JetTools.h"

JetTools::JetTools(vector<JetCorrectorParameters> vCorrParam, JetCorrectionUncertainty* jecUnc, bool startFromRaw)
{
  JEC_ = new FactorizedJetCorrector(vCorrParam);
  jecUnc_ = jecUnc;
  startFromRaw_ = startFromRaw;
  redoFlavorJecUnc_ = false;
}

JetTools::JetTools(vector<JetCorrectorParameters> vCorrParam, string jecUncSourcesFile, string flavorFractionsFile, bool startFromRaw, bool useSubTotalMC)
{
  JEC_ = new FactorizedJetCorrector(vCorrParam);
  startFromRaw_ = startFromRaw;
  redoFlavorJecUnc_ = true;
  
  // Now read in everything for the special JES uncertainty, with alternative Flavor-JES
  cout << "Starting the new Flavor-JES calculation.\n ===> Only implemented for jet |eta| < 2.5 !!!!" << endl;
  
  if(useSubTotalMC) // To be used when only sensitive to Data/MC ratio!!!
    jecUnc_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jecUncSourcesFile, "SubTotalMC")));
  else
    jecUnc_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jecUncSourcesFile, "Total")));
  
  jecUncQCD_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jecUncSourcesFile, "FlavorQCD")));
  jecUncGluon_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jecUncSourcesFile, "FlavorPureGluon")));
  jecUncUDS_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jecUncSourcesFile, "FlavorPureQuark")));
  jecUncC_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jecUncSourcesFile, "FlavorPureCharm")));
  jecUncB_ = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jecUncSourcesFile, "FlavorPureBottom")));
  
  TFile* inFile = new TFile(flavorFractionsFile.c_str(),"READ");
  
  flavFrac_lowEta_Gluon = (TF1*) inFile->Get("Fg_smallEta")->Clone();
  flavFrac_lowEta_UDS = (TF1*) inFile->Get("Fuds_smallEta")->Clone();
  flavFrac_lowEta_C = (TF1*) inFile->Get("Fc_smallEta")->Clone();
  flavFrac_lowEta_B = (TF1*) inFile->Get("Fb_smallEta")->Clone();
  
  flavFrac_medEta_Gluon = (TF1*) inFile->Get("Fg_mediumEta")->Clone();
  flavFrac_medEta_UDS = (TF1*) inFile->Get("Fuds_mediumEta")->Clone();
  flavFrac_medEta_C = (TF1*) inFile->Get("Fc_mediumEta")->Clone();
  flavFrac_medEta_B = (TF1*) inFile->Get("Fb_mediumEta")->Clone();
  
  flavFrac_highEta_Gluon = (TF1*) inFile->Get("Fg_largeEta")->Clone();
  flavFrac_highEta_UDS = (TF1*) inFile->Get("Fuds_largeEta")->Clone();
  flavFrac_highEta_C = (TF1*) inFile->Get("Fc_largeEta")->Clone();
  flavFrac_highEta_B = (TF1*) inFile->Get("Fb_largeEta")->Clone();
  
  inFile->Close();
  delete inFile;
}

JetTools::~JetTools()
{
  if(JEC_) delete JEC_;
}

//_____Jet correctors and uncorrectors_______________________
void JetTools::unCorrectJet(TRootJet* inJet, bool isData)
{
  float corr;
	if(!isData)
	  corr = inJet->getJetCorrFactor("L1FastJetL2L3");
	else
	  corr = inJet->getJetCorrFactor("L1FastJetL2L3L23Residual");
//  cout << "Uncorrecting!  With factor: " << corr << endl;
  inJet->SetPxPyPzE(inJet->Px()/corr, inJet->Py()/corr, inJet->Pz()/corr, inJet->E()/corr);
}

void JetTools::unCorrectJets(vector<TRootJet*> inJets, bool isData)
{
  for(unsigned int i=0; i<inJets.size(); i++)
    unCorrectJet(inJets[i],isData);
}

void JetTools::correctJet(TRootJet* inJet, int nPV, bool isData)
{
  if(startFromRaw_)
    unCorrectJet(inJet, isData);
  JEC_->setJetEta(inJet->Eta());
  JEC_->setJetPt(inJet->Pt());
  if(nPV > -1) // do the L1 correction
  {
    JEC_->setJetE(inJet->E());
    JEC_->setNPV(nPV);
  }
  float corr = JEC_->getCorrection();
  inJet->SetPxPyPzE(inJet->Px()*corr, inJet->Py()*corr, inJet->Pz()*corr, inJet->E()*corr);
}

void JetTools::correctJets(vector<TRootJet*> inJets, bool isData)
{
  for(unsigned int i=0; i<inJets.size(); i++)
    correctJet(inJets[i],isData);
}

void JetTools::correctJets(vector<TRootJet*> inJets, vector<TRootVertex*> PVs, bool isData)
{
  int nPV = 0;
  for(unsigned int i=0; i<PVs.size(); i++)
    if(!PVs[i]->isFake() && PVs[i]->ndof()>4)
      nPV++;
  for(unsigned int j=0; j<inJets.size(); j++)
    correctJet(inJets[j],nPV,isData);
}

void JetTools::correctJet(TRootJet* inJet, float rhoPU, bool isData)
{
  if(startFromRaw_)
    unCorrectJet(inJet, isData);
  JEC_->setJetEta(inJet->Eta());
  JEC_->setJetPt(inJet->Pt());
  JEC_->setJetA(inJet->jetArea());
  JEC_->setRho(rhoPU);
	
	//set the correction factors for the type 1 MET correction
	std::vector<float> SubCorrections = JEC_->getSubCorrections(); //0: L1FastJet, 1: L1FastJetL2, 2: L1FastJetL2L3, and if data: 3: L1FastJetL2L3L23Residual  
	/*for(unsigned int c=0; c<SubCorrections.size(); c++)
	  	cout<<"SubCorrections["<<c<<"] = "<<SubCorrections[c]<<endl;	
  */
	inJet->setJetCorrFactor(0,"L1FastJet",SubCorrections[0]);
  inJet->setJetCorrFactor(1,"L1FastJetL2",SubCorrections[1]);
	inJet->setJetCorrFactor(2,"L1FastJetL2L3",SubCorrections[2]);
	if(isData)
	  inJet->setJetCorrFactor(3,"L1FastJetL2L3L23Residual",SubCorrections[3]);
 
  //float corr = JEC_->getCorrection(); //strangely enough, this starts complaining when JEC_->getSubCorrections() is called first
	float corr = SubCorrections[SubCorrections.size()-1]; //this is the correction UP TO the last level; so the complete correction
//	cout << "Apply new JES correction:  " << corr << endl;
  inJet->SetPxPyPzE(inJet->Px()*corr, inJet->Py()*corr, inJet->Pz()*corr, inJet->E()*corr);
}

void JetTools::correctJets(vector<TRootJet*> inJets, float rhoPU, bool isData)
{
  for(unsigned int j=0; j<inJets.size(); j++)
    correctJet(inJets[j],rhoPU,isData);
}

float JetTools::calculateJESUnc(float eta, float pt, string direction)
{
  jecUnc_->setJetEta(eta);
  jecUnc_->setJetPt(pt);
  float gFrac = 0, udsFrac = 0, cFrac = 0, bFrac = 0;
  if(redoFlavorJecUnc_) // prepare everything...
  {
    jecUncQCD_->setJetEta(eta);
    jecUncQCD_->setJetPt(pt);
    jecUncGluon_->setJetEta(eta);
    jecUncGluon_->setJetPt(pt);
    jecUncUDS_->setJetEta(eta);
    jecUncUDS_->setJetPt(pt);
    jecUncC_->setJetEta(eta);
    jecUncC_->setJetPt(pt);
    jecUncB_->setJetEta(eta);
    jecUncB_->setJetPt(pt);
    
    if(fabs(eta) < 1.305)
    {
      gFrac = flavFrac_lowEta_Gluon->Eval(pt);
      udsFrac = flavFrac_lowEta_UDS->Eval(pt);
      cFrac = flavFrac_lowEta_C->Eval(pt);
      bFrac = flavFrac_lowEta_B->Eval(pt);
    }
    else if(fabs(eta) < 1.93)
    {
      gFrac = flavFrac_medEta_Gluon->Eval(pt);
      udsFrac = flavFrac_medEta_UDS->Eval(pt);
      cFrac = flavFrac_medEta_C->Eval(pt);
      bFrac = flavFrac_medEta_B->Eval(pt);
    }
    else if(fabs(eta) < 2.5)
    {
      gFrac = flavFrac_highEta_Gluon->Eval(pt);
      udsFrac = flavFrac_highEta_UDS->Eval(pt);
      cFrac = flavFrac_highEta_C->Eval(pt);
      bFrac = flavFrac_highEta_B->Eval(pt);
    }
    
    gFrac = gFrac / (gFrac+udsFrac+cFrac+bFrac);
    udsFrac = udsFrac / (gFrac+udsFrac+cFrac+bFrac);
    cFrac = cFrac / (gFrac+udsFrac+cFrac+bFrac);
    bFrac = bFrac / (gFrac+udsFrac+cFrac+bFrac);
  }
  
  float unc =0;
  if(direction == "plus"){
    if(redoFlavorJecUnc_ && fabs(eta) < 2.5)
    {
      float totalUnc = jecUnc_->getUncertainty(true);
      float qcdUnc = jecUncQCD_->getUncertainty(true);
      float gUnc = jecUncGluon_->getUncertainty(true);
      float udsUnc = jecUncUDS_->getUncertainty(true);
      float cUnc = jecUncC_->getUncertainty(true);
      float bUnc = jecUncB_->getUncertainty(true);
      
      unc = totalUnc*totalUnc - qcdUnc*qcdUnc; // first subtract the 'standard' FlavorQCD part
      unc = unc + gFrac*gFrac*gUnc*gUnc + udsFrac*udsFrac*udsUnc*udsUnc + cFrac*cFrac*cUnc*cUnc + bFrac*bFrac*bUnc*bUnc;
      unc = sqrt(unc);
    }
    else unc  = jecUnc_->getUncertainty(true);
  }
  else if(direction == "minus"){
    if(redoFlavorJecUnc_ && fabs(eta) < 2.5)
    {
      float totalUnc = jecUnc_->getUncertainty(false);
      float qcdUnc = jecUncQCD_->getUncertainty(false);
      float gUnc = jecUncGluon_->getUncertainty(false);
      float udsUnc = jecUncUDS_->getUncertainty(false);
      float cUnc = jecUncC_->getUncertainty(false);
      float bUnc = jecUncB_->getUncertainty(false);
      
      unc = totalUnc*totalUnc - qcdUnc*qcdUnc; // first subtract the 'standard' FlavorQCD part
      unc = unc + gFrac*gFrac*gUnc*gUnc + udsFrac*udsFrac*udsUnc*udsUnc + cFrac*cFrac*cUnc*cUnc + bFrac*bFrac*bUnc*bUnc;
      unc = sqrt(unc);
    }
    else unc  = jecUnc_->getUncertainty(false);
  }
  else cout << "JetTools::correctJetJESUnc  unknown direction: " << direction << endl;
  return unc;
}

void JetTools::correctJetJESUnc(TRootJet* inJet, string direction, float nSigma) // direction = plus or minus
{
  if (fabs(inJet->Eta()) < 4.7)
  {
    float corr=0;
    float unc=calculateJESUnc(inJet->Eta(), inJet->Pt(), direction);
    if(direction == "plus") corr = 1 + unc*nSigma;
    else if(direction == "minus") corr = 1 - unc*nSigma;
    else cout << "JetTools::correctJetJESUnc  unknown direction: " << direction << endl;
//  cout << "jet:  Pt: " << inJet->Pt() << "  Eta: " << inJet->Eta() << "  unc: " << unc;
    inJet->SetPxPyPzE(inJet->Px()*corr, inJet->Py()*corr, inJet->Pz()*corr, inJet->E()*corr);
//  cout << "  corrected pt: " << inJet->Pt() << endl;
  }
}

void JetTools::correctJetJESUnc(vector<TRootJet*> inJets, string direction, float nSigma)
{
  for(unsigned int i=0; i<inJets.size(); i++)
    correctJetJESUnc(inJets[i],direction,nSigma);
}

void JetTools::correctJetJESUnc(TRootJet* inJet, TRootMET* inMET, string direction,float nSigma) // direction = plus or minus
{ 
  if (inJet->Pt() > 10) {
    inMET->SetPx(inMET->Px() + inJet->Px()); 
    inMET->SetPy(inMET->Py() + inJet->Py());  
    correctJetJESUnc(inJet,direction, nSigma);  
    inMET->SetPx(inMET->Px() - nSigma*inJet->Px());
    inMET->SetPy(inMET->Py() - nSigma*inJet->Py());
    inMET->SetE(sqrt(pow(inMET->Px(),2) + pow(inMET->Py(),2)));
  }
}

void JetTools::correctJetJESUnc(vector<TRootJet*> inJets, TRootMET* inMET, string direction, float nSigma)
{
  for(unsigned int i=0; i<inJets.size(); i++)
    correctJetJESUnc(inJets[i],inMET,direction,nSigma);
}

void JetTools::correctJetJER(TRootJet* inJet, TRootGenJet* inGenJet, string direction, bool oldnumbers)
{
  float corrFactor;
  bool JER_minus = false, JER_plus = false; // only one of them true! never both of them!
  if(direction == "minus") JER_minus = true;
  else if(direction == "plus") JER_plus = true;
  else if(direction != "nominal") cout << "Unknown JER direction: " << direction << endl;
  float fabsEta = fabs(inJet->Eta());
	
	if(!oldnumbers)
	{
	  //numbers from https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution
    if(JER_minus)
    {
    	if(fabsEta <= 1.1) corrFactor = -0.006;
    	else if(fabsEta <= 1.7 && fabsEta > 1.1) corrFactor = 0.129;
			else if(fabsEta <= 2.3 && fabsEta > 1.7) corrFactor = 0.011;
			else if(fabsEta <= 5.0 && fabsEta > 2.3) corrFactor = -0.033;
    	else corrFactor = -0.033; //fabsEta > 5.0
  	}
  	else if(JER_plus)
  	{
    	if(fabsEta <= 1.1) corrFactor = 0.136;
    	else if(fabsEta <= 1.7 && fabsEta > 1.1) corrFactor = 0.251;
			else if(fabsEta <= 2.3 && fabsEta > 1.7) corrFactor = 0.176;
			else if(fabsEta <= 5.0 && fabsEta > 2.3) corrFactor = 0.356;
    	else corrFactor = 0.356; //fabsEta > 5.0
  	}
		else
		{
	  	if(fabsEta <= 1.1) corrFactor = 0.066;
    	else if(fabsEta <= 1.7 && fabsEta > 1.1) corrFactor = 0.191;
			else if(fabsEta <= 2.3 && fabsEta > 1.7) corrFactor = 0.096;
			else if(fabsEta <= 5.0 && fabsEta > 2.3) corrFactor = 0.166;
    	else corrFactor = 0.166; //fabsEta > 5.0
		}
	}
	else
	{
	  if(JER_minus)
  	{
    	if(fabsEta <= 1.5) corrFactor = 0.0;
    	else if(fabsEta < 2.0 && fabsEta > 1.5) corrFactor = -0.05;
    	else corrFactor = -0.1;
  	}
  	else if(JER_plus)
  	{
    	if(fabsEta <= 1.5) corrFactor = 0.2;
    	else if(fabsEta < 2.0 && fabsEta > 1.5) corrFactor = 0.25;
    	else corrFactor = 0.3;
  	}
		else 
			corrFactor = 0.1;	
	}
		
  float deltapt = ( inJet->Pt() - inGenJet->Pt() ) * corrFactor;
  float ptscale = max(0.0, ( inJet->Pt() + deltapt) / inJet->Pt() );
  if(ptscale > 0.0)
    inJet->SetPxPyPzE(inJet->Px()*ptscale, inJet->Py()*ptscale, inJet->Pz()*ptscale, inJet->E()*ptscale);
  else inJet->SetPxPyPzE(0.001, 0.001, 0.001, 0.001); // This jet should not pass any event selection
}

void JetTools::correctJetJER(vector<TRootJet*> inJets, vector<TRootGenJet*> inGenJets, string direction, bool oldnumbers)
{
  // Match RecoJets with GenJets
  vector< pair<size_t, size_t> > indexVector; //first index = RecoJet, second index = GenJet
  vector<bool> mLock(inGenJets.size(),false);   // when locked, genJet is already matched to a recoJet
  for(size_t i=0; i<inJets.size(); i++)
  {
    pair<size_t, size_t> tmpIndex;
    float minDR = 9999.;
    for(size_t j=0; j<inGenJets.size(); j++)
    {
      if( ! mLock[j] )
      {
        if( inJets[i]->DeltaR(*inGenJets[j]) < 0.4 && inJets[i]->DeltaR(*inGenJets[j]) < minDR )
        {
          minDR = inJets[i]->DeltaR(*inGenJets[j]);
          tmpIndex = pair<size_t, size_t>(i,j);
        }
      }
    }
    if(minDR < 999.)
    {
      mLock[tmpIndex.second] = true;
      indexVector.push_back(tmpIndex);
    }
  }

  // Apply correction for jet energy resolution on-the-fly, only for recoJets matched with a genJet
  for(size_t i=0; i<indexVector.size(); i++)
    if( inJets[indexVector[i].first]->Pt() >= 10. )
      correctJetJER(inJets[indexVector[i].first], inGenJets[indexVector[i].second], direction, oldnumbers);
}

void JetTools::correctJetJER(vector<TRootJet*> inJets, vector<TRootGenJet*> inGenJets, TRootMET* inMET, string direction, bool oldnumbers)
{
  // Match RecoJets with GenJets
  vector< pair<size_t, size_t> > indexVector; //first index = RecoJet, second index = GenJet
  vector<bool> mLock(inGenJets.size(),false);   // when locked, genJet is already matched to a recoJet
  for(size_t i=0; i<inJets.size(); i++)
  {
    pair<size_t, size_t> tmpIndex;
    float minDR = 9999.;
    for(size_t j=0; j<inGenJets.size(); j++)
    {
      if( ! mLock[j] )
      {
        if( inJets[i]->DeltaR(*inGenJets[j]) < 0.4 && inJets[i]->DeltaR(*inGenJets[j]) < minDR )
        {
          minDR = inJets[i]->DeltaR(*inGenJets[j]);
          tmpIndex = pair<size_t, size_t>(i,j);
        }
      }
    }
    if(minDR < 999.)
    {
      mLock[tmpIndex.second] = true;
      indexVector.push_back(tmpIndex);
    }
  }

  // Apply correction for jet energy resolution on-the-fly, only for recoJets matched with a genJet
  for(size_t i=0; i<indexVector.size(); i++)
    if( inJets[indexVector[i].first]->Pt() >= 10. )
    {
      inMET->SetPx(inMET->Px() + inJets[indexVector[i].first]->Px());
      inMET->SetPy(inMET->Py() + inJets[indexVector[i].first]->Py());
      correctJetJER(inJets[indexVector[i].first], inGenJets[indexVector[i].second], direction, oldnumbers);
      inMET->SetPx(inMET->Px() - inJets[indexVector[i].first]->Px());
      inMET->SetPy(inMET->Py() - inJets[indexVector[i].first]->Py());
      inMET->SetE(sqrt(pow(inMET->Px(),2) + pow(inMET->Py(),2)));
    }
}

void JetTools::scaleJet(TRootJet* inJet, float scale)
{
  inJet->SetPxPyPzE(inJet->Px()*scale, inJet->Py()*scale, inJet->Pz()*scale, inJet->E()*scale);
}

void JetTools::scaleJets(vector<TRootJet*> inJets, float scale)
{
  for(unsigned int i=0; i<inJets.size(); i++)
    scaleJet(inJets[i],scale);
}

void JetTools::scaleJets(vector<TRootJet*> inJets, TRootMET* inMET, float scale)
{
  for(unsigned int i=0; i<inJets.size(); i++) {
        
    inMET->SetPx(inMET->Px() + inJets[i]->Px());
    inMET->SetPy(inMET->Py() + inJets[i]->Py());

    scaleJet(inJets[i],scale);

    inMET->SetPx(inMET->Px() - inJets[i]->Px());
    inMET->SetPy(inMET->Py() - inJets[i]->Py());
    inMET->SetE(sqrt(pow(inMET->Px(),2) + pow(inMET->Py(),2)));

  }
}

void JetTools::unCorrectMETTypeOne(TRootJet* inJet, TRootMET* inMET, bool isData)
{
  float corr = -9999;
  if(!isData) corr = inJet->getJetCorrFactor("L1FastJetL2L3");
  else corr = inJet->getJetCorrFactor("L1FastJetL2L3L23Residual"); //see JetAnalyzer.cc in TopTreeProducer
  float L1corr = inJet->getJetCorrFactor("L1FastJet");
  inMET->SetPxPyPzE(inMET->Px()+(inJet->Px()-inJet->Px()*L1corr/corr), inMET->Py()+(inJet->Py()-inJet->Py()*L1corr/corr), 0, sqrt(pow(inMET->Px()+(inJet->Px()-inJet->Px()*L1corr/corr),2) + pow(inMET->Py()+(inJet->Py()-inJet->Py()*L1corr/corr),2)) ); //METx_raw = METx_raw + Px_raw - Px_corr    
}

void JetTools::unCorrectMETTypeOne(vector<TRootJet*> inJets, TRootMET* inMET, bool isData)
{
  for(unsigned int i=0; i<inJets.size();i++)
    if(inJets[i]->Pt() > 10)
      unCorrectMETTypeOne(inJets[i], inMET, isData);
}

void JetTools::correctMETTypeOne(TRootJet* inJet, TRootMET* inMET, bool isData)  //Do not apply for |eta| > 4.7
{
  float corr = -9999;
  if(!isData) corr = inJet->getJetCorrFactor("L1FastJetL2L3");
  else corr = inJet->getJetCorrFactor("L1FastJetL2L3L23Residual"); //see JetAnalyzer.cc in TopTreeProducer
  float L1corr = inJet->getJetCorrFactor("L1FastJet");
  inMET->SetPxPyPzE(inMET->Px()-(inJet->Px()-inJet->Px()*L1corr/corr), inMET->Py()-(inJet->Py()-inJet->Py()*L1corr/corr), 0, sqrt(pow(inMET->Px()-(inJet->Px()-inJet->Px()*L1corr/corr),2) + pow(inMET->Py()-(inJet->Py()-inJet->Py()*L1corr/corr),2)) ); //METx_raw = METx_raw + Px_raw - Px_corr    
}

void JetTools::correctMETTypeOne(vector<TRootJet*> inJets, TRootMET* inMET, bool isData)
{
  for(unsigned int i=0; i<inJets.size();i++)
    if(inJets[i]->Pt() > 10)
      correctMETTypeOne(inJets[i], inMET, isData);
}

void JetTools::correctMETUnclusteredEnergy(TRootMET* inMET, vector<TRootJet*> inJets, vector<TRootMuon*> inMuons, vector<TRootElectron*> inElectrons, string direction) {
  
  double factor=1.;

  //Use all jets stored in the event, with no additional pt (>10 GeV standard) or eta or Id requirement
  double met_x = inMET->Px();
  double met_y = inMET->Py();
  for (unsigned int i=0; i<inJets.size(); i++){
    if(inJets[i]->Pt() > 10.){
      met_x += inJets[i]->Px();
      met_y += inJets[i]->Py();
    }
  }
  for(unsigned int i=0; i<inMuons.size(); i++){
    met_x += inMuons[i]->Px();
    met_y += inMuons[i]->Py();
  }
  for(unsigned int i=0; i<inElectrons.size(); i++){
    met_x += inElectrons[i]->Px();
    met_y += inElectrons[i]->Py();
  }
  
  if (direction == "minus")
    factor = 0.9;
  else if (direction == "plus")
    factor = 1.1;
  
  met_x *= factor;
  met_y *= factor;
  
  //Go back to original MET, but with unclustered energy scaled up and down:
  for (unsigned int i=0; i<inJets.size(); i++){
    if(inJets[i]->Pt() > 10.){
      met_x -= inJets[i]->Px();
      met_y -= inJets[i]->Py();
    }
  }
  for(unsigned int i=0; i<inMuons.size(); i++){
    met_x -= inMuons[i]->Px();
    met_y -= inMuons[i]->Py();
  }
  for(unsigned int i=0; i<inElectrons.size(); i++){
    met_x -= inElectrons[i]->Px();
    met_y -= inElectrons[i]->Py();
  }
  inMET->SetPx(met_x);
  inMET->SetPy(met_y);

}
    
//_____Jet convertors________________________________________
vector<TRootCaloJet*> JetTools::convertToCaloJets(vector<TRootJet*> init_jets) {
  vector<TRootCaloJet*> jets;
  for (unsigned i=0; i<init_jets.size(); i++)
    if (init_jets[i]->jetType() == 1)
      jets.push_back(static_cast<TRootCaloJet*>(init_jets[i]));
    else
      cout << "ERROR - JetTools: Attempting to convert a TRootJet to TRootCaloJet that was not created from a CaloJet!";
  return jets;
}

vector<TRootPFJet*> JetTools::convertToPFJets(vector<TRootJet*> init_jets) {
  vector<TRootPFJet*> jets;
  for (unsigned i=0; i<init_jets.size(); i++)
    if (init_jets[i]->jetType() == 2)
      jets.push_back(static_cast<TRootPFJet*>(init_jets[i]));
    else
      cout << "ERROR - JetTools: Attempting to convert a TRootJet to TRootPFJet that was not created from a PFJet!";
  return jets;
}

vector<TRootJPTJet*> JetTools::convertToJPTJets(vector<TRootJet*> init_jets) {
  vector<TRootJPTJet*> jets;
  for (unsigned i=0; i<init_jets.size(); i++)
    if (init_jets[i]->jetType() == 3)
      jets.push_back(static_cast<TRootJPTJet*>(init_jets[i]));
    else
      cout << "ERROR - JetTools: Attempting to convert a TRootJet to TRootJPTJet that was not created from a JPTJet!";
  return jets;
}

TRootCaloJet* JetTools::convertToCaloJets(TRootJet* init_jet) {
  std::vector<TRootJet*> init_jets; init_jets.push_back(init_jet);
  std::vector<TRootCaloJet*> jets = convertToCaloJets(init_jets);
  if (jets.size() > 0) return jets[0];
  return NULL;
}

TRootPFJet* JetTools::convertToPFJets(TRootJet* init_jet) {
  std::vector<TRootJet*> init_jets; init_jets.push_back(init_jet);
  std::vector<TRootPFJet*> jets = convertToPFJets(init_jets);
  if (jets.size() > 0) return jets[0];
  return NULL;
}

TRootJPTJet* JetTools::convertToJPTJets(TRootJet* init_jet) {
  std::vector<TRootJet*> init_jets; init_jets.push_back(init_jet);
  std::vector<TRootJPTJet*> jets = convertToJPTJets(init_jets);
  if (jets.size() > 0) return jets[0];
  return NULL;
}
//___________________________________________________________
