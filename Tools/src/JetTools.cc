#include "../interface/JetTools.h"

JetTools::JetTools(vector<JetCorrectorParameters> vCorrParam, JetCorrectionUncertainty* jecUnc, bool startFromRaw)
{
  JEC_ = new FactorizedJetCorrector(vCorrParam);
  jecUnc_ = jecUnc;
  startFromRaw_ = startFromRaw;
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
  //cout << "uncorrecting!" << endl;
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
  inJet->SetPxPyPzE(inJet->Px()*corr, inJet->Py()*corr, inJet->Pz()*corr, inJet->E()*corr);
}

void JetTools::correctJets(vector<TRootJet*> inJets, float rhoPU, bool isData)
{
  for(unsigned int j=0; j<inJets.size(); j++)
    correctJet(inJets[j],rhoPU,isData);
}


void JetTools::correctJetJESUnc(TRootJet* inJet, string direction, float nSigma) // direction = plus or minus
{
  if (fabs(inJet->Eta()) < 4.7) {
    jecUnc_->setJetEta(inJet->Eta());
    jecUnc_->setJetPt(inJet->Pt());
//  float unc = jecUnc_->getUncertainty(true);  
//  unc = sqrt((unc*unc) + (0.025*0.025) + ((0.2*0.8*5.62)/inJet->Pt()));
    float corr=1;
    float unc =0;
    if(direction == "plus"){
      unc  = jecUnc_->getUncertainty(true);
      corr = 1 + unc*nSigma;
    }
    else if(direction == "minus"){
      unc  = jecUnc_->getUncertainty(false);
      corr = 1 - unc*nSigma;
    }
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
  inMET->SetPx(inMET->Px() + inJet->Px()); 
  inMET->SetPy(inMET->Py() + inJet->Py());  
  correctJetJESUnc(inJet,direction, nSigma);  
  inMET->SetPx(inMET->Px() - nSigma*inJet->Px());
  inMET->SetPy(inMET->Py() - nSigma*inJet->Py());
  inMET->SetE(sqrt(pow(inMET->Px(),2) + pow(inMET->Py(),2)));
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
  for(unsigned int i=0; i<inJets.size();i++){
    if(fabs(inJets[i]->Eta()) <=4.7)  //EtaValuesAbs already contains the absolute value of the Eta values.
      correctMETTypeOne(inJets[i],inMET, isData);
      
  }
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
