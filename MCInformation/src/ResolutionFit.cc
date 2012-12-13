#include "../interface/ResolutionFit.h"

ResolutionFit::ResolutionFit()
{
}

ResolutionFit::ResolutionFit(string label)
{
  calculatedResolutions_ = false;
  loadedResolutions_ = false;
  nEtaBins_ = 11;
  nPtBins_ = 11;
  label_ = label;
  StijnFlavorJES_ = false;
  if( label_.find("JES") != string::npos ) // The JetMET stuff!
  {
    StijnFlavorJES_ = true;
    cout << "Using Resolutionfit for the FlavorJES stuff!" << endl;
    nEtaBins_ = 10;
    Float_t towerBinning[] = {0.0, 0.5, 1., 1.5, 2., 2.5, 3., 3.5, 4., 4.5, 5.};
    for(unsigned int i=0; i < nEtaBins_+1; i++)
      towerBinning_[i] = towerBinning[i];
    nPtBins_ = 14;
    Float_t jetPtBinning[] = {20.,40.,60.,80.,100.,120.,140.,160.,200.,300.,400.,600.,800.,1000.,1800.};
    for(unsigned int i=0; i < nPtBins_+1; i++)
      jetPtBinning_[i] = jetPtBinning[i];
  }
  else if( label_.find("LightJet") == 0 ){
    Float_t towerBinning[] = {0.0, 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74, 2.5};
    for(unsigned int i=0; i < nEtaBins_+1; i++)
      towerBinning_[i] = towerBinning[i];
    Float_t jetPtBinning[] = {20.,30.,40.,50.,60.,70.,80.,90.,100.,120.,150.,1800.};
    for(unsigned int i=0; i < nPtBins_+1; i++)
      jetPtBinning_[i] = jetPtBinning[i];
  }
  else if( label_.find("BJet") == 0 ){
    Float_t towerBinning[] = {0.0, 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74, 2.5};
    for(unsigned int i=0; i < nEtaBins_+1; i++)
      towerBinning_[i] = towerBinning[i];
    Float_t jetPtBinning[] = {20.,30.,40.,50.,60.,70.,80.,90.,100.,120.,150.,1800.};
    for(unsigned int i=0; i < nPtBins_+1; i++)
      jetPtBinning_[i] = jetPtBinning[i];
  }
  else if( label_.find("QJet") == 0 ){
    Float_t towerBinning[] = {0.0, 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74, 2.5};
    for(unsigned int i=0; i < nEtaBins_+1; i++)
      towerBinning_[i] = towerBinning[i];
    Float_t jetPtBinning[] = {20.,30.,40.,50.,60.,70.,80.,90.,100.,120.,150.,1800.};
    for(unsigned int i=0; i < nPtBins_+1; i++)
      jetPtBinning_[i] = jetPtBinning[i];
  }
  else if( label_.find("Neutrino") == 0){
    nEtaBins_=1;
    Float_t towerBinning[] = {-2.5,2.5,990.,991.,992.,993.,994.,995.,996.,997.,998.,999.};
    for(unsigned int i=0; i < nEtaBins_+1; i++)
      towerBinning_[i] = towerBinning[i];
    Float_t jetPtBinning[] = {0.,20.,25.,30.,35.,40.,45.,50.,60.,70.,80.,100.,800.};
    for(unsigned int i=0; i < nPtBins_+1; i++)
      jetPtBinning_[i] = jetPtBinning[i];
  }
  else if( label_.find("Muon") == 0){

    Float_t towerBinning[] = {0.0,0.1,0.2,0.35,0.42,0.55,0.7,0.82,0.95,1.1,1.5,2.5};
    for(unsigned int i=0; i < nEtaBins_+1; i++)
      towerBinning_[i] = towerBinning[i];
    Float_t jetPtBinning[] = {20.,25.,30.,35.,40.,45.,50.,60.,70.,80.,90.,110.,600.};
    for(unsigned int i=0; i < nPtBins_+1; i++)
      jetPtBinning_[i] = jetPtBinning[i];
  }
  else if( label_.find("Electron") == 0){
    
    Float_t towerBinning[] = {0.0,0.1,0.2,0.35,0.42,0.55,0.7,0.82,0.95,1.1,1.5,2.5};
    for(unsigned int i=0; i < nEtaBins_+1; i++)
      towerBinning_[i] = towerBinning[i];
    Float_t jetPtBinning[] = {20.,35.,40.,45.,50.,55.,60.,70.,80.,95.,110.,150.,600.};
    for(unsigned int i=0; i < nPtBins_+1; i++)
      jetPtBinning_[i] = jetPtBinning[i];
  }
  else cout << "ResolutionFit:  Unknown label_: " << label_ << endl;   

  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++){
    Char_t controlCanName[60], controlCanEtaName[60], controlCanThetaName[60], controlCanPhiName[60], controlCanRelName[60];
    sprintf(controlCanName,"controlCan_%s_%i",label_.c_str(),iEta);
    sprintf(controlCanEtaName,"controlCanEta_%s_%i",label_.c_str(),iEta);
    sprintf(controlCanThetaName,"controlCanTheta_%s_%i",label_.c_str(),iEta);
    sprintf(controlCanPhiName,"controlCanPhi_%s_%i",label_.c_str(),iEta);
    sprintf(controlCanRelName,"controlCanEtRel_%s_%i",label_.c_str(),iEta);
    controlCan_[iEta] = new TCanvas(controlCanName,controlCanName,1000,800);
    controlCan_[iEta]->Divide(4,4);
    controlCanEta_[iEta] = new TCanvas(controlCanEtaName,controlCanEtaName,1000,800);
    controlCanEta_[iEta]->Divide(4,4);
    controlCanTheta_[iEta] = new TCanvas(controlCanThetaName,controlCanThetaName,1000,800);
    controlCanTheta_[iEta]->Divide(4,4);
    controlCanPhi_[iEta] = new TCanvas(controlCanPhiName,controlCanPhiName,1000,800);
    controlCanPhi_[iEta]->Divide(4,4);
    controlCanRel_[iEta] = new TCanvas(controlCanRelName,controlCanRelName,1000,800);
    controlCanRel_[iEta]->Divide(4,4);
    
    if(iEta == 0){
      controlCanRelIncl_ = new TCanvas(("controlCan_"+label_+"_EtaIncl").c_str(),("controlCan_"+label_+"_EtaIncl").c_str(),1000,800);
      controlCanRelIncl_->Divide(4,4);
      bPtEtRelMeanIncl_ = new TCanvas(("bPtEtRelMean_"+label_+"_EtaIncl").c_str(),("bPtEtRelMean_"+label_+"_EtaIncl").c_str(),1000,700);
    }
    
    Char_t lowerUpperSigma[60], lowerUpperEtaSigma[60], lowerUpperThetaSigma[60], lowerUpperPhiSigma[60], lowerUpperRelSigma[60];
    sprintf(lowerUpperSigma,"bPtEtSigma_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperEtaSigma,"bPtEtaSigma_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperThetaSigma,"bPtThetaSigma_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperPhiSigma,"bPtPhiSigma_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperRelSigma,"bPtEtRelSigma_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);

    bPtEtSigma_[iEta] = new TCanvas(lowerUpperSigma,lowerUpperSigma,1000,700);
    bPtEtaSigma_[iEta] = new TCanvas(lowerUpperEtaSigma,lowerUpperEtaSigma,1000,700);
    bPtThetaSigma_[iEta] = new TCanvas(lowerUpperThetaSigma,lowerUpperThetaSigma,1000,700);
    bPtPhiSigma_[iEta] = new TCanvas(lowerUpperPhiSigma,lowerUpperPhiSigma,1000,700);
    bPtEtRelSigma_[iEta] = new TCanvas(lowerUpperRelSigma,lowerUpperRelSigma,1000,700);
    
    Char_t lowerUpperMean[60], lowerUpperEtaMean[60], lowerUpperThetaMean[60], lowerUpperPhiMean[60], lowerUpperRelMean[60];
    sprintf(lowerUpperMean,"bPtEtMean_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperEtaMean,"bPtEtaMean_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperThetaMean,"bPtThetaMean_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperPhiMean,"bPtPhiMean_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperRelMean,"bPtEtRelMean_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    
    bPtEtMean_[iEta] = new TCanvas(lowerUpperMean,lowerUpperMean,1000,700);    
    bPtEtaMean_[iEta] = new TCanvas(lowerUpperEtaMean,lowerUpperEtaMean,1000,700);
    bPtThetaMean_[iEta] = new TCanvas(lowerUpperThetaMean,lowerUpperThetaMean,1000,700);
    bPtPhiMean_[iEta] = new TCanvas(lowerUpperPhiMean,lowerUpperPhiMean,1000,700);
    bPtEtRelMean_[iEta] = new TCanvas(lowerUpperRelMean,lowerUpperRelMean,1000,700);    
    
    for(unsigned int iSel=0; iSel<nPtBins_; iSel++){
      Char_t etaEtNr[10];
      sprintf(etaEtNr,"%i",(iEta*nPtBins_)+iSel);
      TString etaEtNumber = etaEtNr;

      binCenterHisto_[(iEta*nPtBins_)+iSel] = new TH1F("binCenterHisto_"+label_+"_"+etaEtNumber,"binCenterHisto_"+etaEtNumber,150000,jetPtBinning_[iSel],jetPtBinning_[iSel+1]);
      resHisto_[(iEta*nPtBins_)+iSel] = new TH1F("resHisto_"+label_+"_"+etaEtNumber,"resHisto_"+etaEtNumber,200,-50.,50.);
      resHistoEta_[(iEta*nPtBins_)+iSel] = new TH1F("resHistoEta_"+label_+"_"+etaEtNumber,"resHistoEta_"+etaEtNumber,200,-0.4,0.4);
      resHistoTheta_[(iEta*nPtBins_)+iSel] = new TH1F("resHistoTheta_"+label_+"_"+etaEtNumber,"resHistoTheta_"+etaEtNumber,200,-0.4,0.4);
      resHistoPhi_[(iEta*nPtBins_)+iSel] = new TH1F("resHistoPhi_"+label_+"_"+etaEtNumber,"resHistoPhi_"+etaEtNumber,200,-0.4,0.4);
      resRelHisto_[(iEta*nPtBins_)+iSel] = new TH1F("resRelHisto_"+label_+"_"+etaEtNumber,"resRelHisto_"+etaEtNumber,400,-1,3);
      
      if(iEta == 0){
	      binCenterHistoIncl_[iSel] = new TH1F("binCenterHisto_"+label_+"_EtaIncl_"+etaEtNumber,"binCenterHisto_"+label_+"_EtaIncl_"+etaEtNumber,150000,jetPtBinning_[iSel],jetPtBinning_[iSel+1]);
	      resRelHistoIncl_[iSel] = new TH1F("resRelHisto_"+label_+"_EtaIncl_"+etaEtNumber,"resRelHisto_"+label_+"_EtaIncl_"+etaEtNumber,400,-1,3);
      }
    }
    
    Char_t chfHistoName[60], nhfHistoName[60], cefHistoName[60], nefHistoName[60], cmfHistoName[60], ptHistoName[60];
    sprintf(chfHistoName,"chfHisto_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(nhfHistoName,"nhfHisto_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(cefHistoName,"cefHisto_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(nefHistoName,"nefHisto_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(cmfHistoName,"cmfHisto_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(ptHistoName,"ptHisto_%s_%1.3f_%1.3f",label_.c_str(),towerBinning_[iEta],towerBinning_[iEta+1]);
    
    chfHisto_[iEta] = new TH1F(chfHistoName,chfHistoName,nPtBins_,jetPtBinning_);
    chfHisto_[iEta]->Sumw2();
    nhfHisto_[iEta] = new TH1F(nhfHistoName,nhfHistoName,nPtBins_,jetPtBinning_);
    nhfHisto_[iEta]->Sumw2();
    cefHisto_[iEta] = new TH1F(cefHistoName,cefHistoName,nPtBins_,jetPtBinning_);
    cefHisto_[iEta]->Sumw2();
    nefHisto_[iEta] = new TH1F(nefHistoName,nefHistoName,nPtBins_,jetPtBinning_);
    nefHisto_[iEta]->Sumw2();
    cmfHisto_[iEta] = new TH1F(cmfHistoName,cmfHistoName,nPtBins_,jetPtBinning_);
    cmfHisto_[iEta]->Sumw2();
    ptHisto_[iEta] = new TH1F(ptHistoName,ptHistoName,nPtBins_,jetPtBinning_);
    ptHisto_[iEta]->Sumw2();
  }
}

ResolutionFit::ResolutionFit(const ResolutionFit &r)
{
  nEtaBins_ = r.nEtaBins_;
  nPtBins_ = r.nPtBins_;
  loadedResolutions_ = r.loadedResolutions_;
  calculatedResolutions_ = r.calculatedResolutions_;
  label_ = r.label_;
  bPtEtRelMeanIncl_ = r.bPtEtRelMeanIncl_;
  StijnFlavorJES_ = r.StijnFlavorJES_;
  
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++){
    towerBinning_[iEta] = r.towerBinning_[iEta];
    
    bFuncSigma_[iEta] = r.bFuncSigma_[iEta];
    bFuncEtaSigma_[iEta] = r.bFuncEtaSigma_[iEta];
    bFuncThetaSigma_[iEta] = r.bFuncThetaSigma_[iEta];
    bFuncPhiSigma_[iEta] = r.bFuncPhiSigma_[iEta];
    bFuncRelSigma_[iEta] = r.bFuncRelSigma_[iEta];
    
    bFuncMean_[iEta] = r.bFuncMean_[iEta];
    bFuncRelMean_[iEta] = r.bFuncRelMean_[iEta];
    
    controlCan_[iEta] = r.controlCan_[iEta];
    controlCanEta_[iEta] = r.controlCanEta_[iEta];
    controlCanTheta_[iEta] = r.controlCanTheta_[iEta];
    controlCanPhi_[iEta] = r.controlCanPhi_[iEta];
    controlCanRel_[iEta] = r.controlCanRel_[iEta];

    bPtEtSigma_[iEta] = r.bPtEtSigma_[iEta];
    bPtEtaSigma_[iEta] = r.bPtEtaSigma_[iEta];
    bPtThetaSigma_[iEta] = r.bPtThetaSigma_[iEta];
    bPtPhiSigma_[iEta] = r.bPtPhiSigma_[iEta];
    bPtEtRelSigma_[iEta] = r.bPtEtRelSigma_[iEta];
    
    bPtEtMean_[iEta] = r.bPtEtMean_[iEta];
    bPtEtaMean_[iEta] = r.bPtEtaMean_[iEta];
    bPtThetaMean_[iEta] = r.bPtThetaMean_[iEta];
    bPtPhiMean_[iEta] = r.bPtPhiMean_[iEta];
    bPtEtRelMean_[iEta] = r.bPtEtRelMean_[iEta];
    
    for(unsigned int iSel=0; iSel<nPtBins_; iSel++){
      if(iEta == 0){
	      jetPtBinning_[iSel] = r.jetPtBinning_[iSel];
	      binCenterHistoIncl_[iSel] = r.binCenterHistoIncl_[iSel];
	      resRelHistoIncl_[iSel] = r.resRelHistoIncl_[iSel];
      }
      
      binCenterHisto_[(iEta*nPtBins_)+iSel] = (TH1F*) r.binCenterHisto_[(iEta*nPtBins_)+iSel]->Clone();
      resHisto_[(iEta*nPtBins_)+iSel] = (TH1F*) r.resHisto_[(iEta*nPtBins_)+iSel]->Clone();
      resHistoEta_[(iEta*nPtBins_)+iSel] = (TH1F*) r.resHistoEta_[(iEta*nPtBins_)+iSel]->Clone();
      resHistoTheta_[(iEta*nPtBins_)+iSel] = (TH1F*) r.resHistoTheta_[(iEta*nPtBins_)+iSel]->Clone();
      resHistoPhi_[(iEta*nPtBins_)+iSel] = (TH1F*) r.resHistoPhi_[(iEta*nPtBins_)+iSel]->Clone();
      resRelHisto_[(iEta*nPtBins_)+iSel] = (TH1F*) r.resRelHisto_[(iEta*nPtBins_)+iSel]->Clone();
    }
  }
  
  towerBinning_[nEtaBins_+1] = r.towerBinning_[nEtaBins_+1];
  jetPtBinning_[nPtBins_+1] = r.jetPtBinning_[nPtBins_+1];
}

ResolutionFit::~ResolutionFit()
{
  if(bPtEtRelMeanIncl_) delete bPtEtRelMeanIncl_;
  if(bGraphRelMeanIncl_) delete bGraphRelMeanIncl_;
  
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++){
    if(bPtEtSigma_[iEta]) delete bPtEtSigma_[iEta];
    if(bPtEtaSigma_[iEta]) delete bPtEtaSigma_[iEta];
    if(bPtThetaSigma_[iEta]) delete bPtThetaSigma_[iEta];
    if(bPtPhiSigma_[iEta]) delete bPtPhiSigma_[iEta];
    if(bPtEtRelSigma_[iEta]) delete bPtEtRelSigma_[iEta];
    
    if(bPtEtMean_[iEta]) delete bPtEtMean_[iEta];
    if(bPtEtaMean_[iEta]) delete bPtEtaMean_[iEta];
    if(bPtThetaMean_[iEta]) delete bPtThetaMean_[iEta];
    if(bPtPhiMean_[iEta]) delete bPtPhiMean_[iEta];
    if(bPtEtRelMean_[iEta]) delete bPtEtRelMean_[iEta];
    
    if(controlCan_[iEta]) delete controlCan_[iEta];
    if(controlCanEta_[iEta]) delete controlCanEta_[iEta];
    if(controlCanTheta_[iEta]) delete controlCanTheta_[iEta];
    if(controlCanPhi_[iEta]) delete controlCanPhi_[iEta];
    if(controlCanRel_[iEta]) delete controlCanRel_[iEta];
    if(controlCanRelIncl_) delete controlCanRelIncl_;
    
    if(bGraphSigma_[iEta]) delete bGraphSigma_[iEta];
    if(bGraphMean_[iEta]) delete bGraphMean_[iEta];
    if(bGraphEtaSigma_[iEta]) delete bGraphEtaSigma_[iEta];
    if(bGraphEtaMean_[iEta]) delete bGraphEtaMean_[iEta];
    if(bGraphThetaSigma_[iEta]) delete bGraphThetaSigma_[iEta];
    if(bGraphThetaMean_[iEta]) delete bGraphThetaMean_[iEta];
    if(bGraphPhiSigma_[iEta]) delete bGraphPhiSigma_[iEta];
    if(bGraphPhiMean_[iEta]) delete bGraphPhiMean_[iEta];
    if(bGraphRelSigma_[iEta]) delete bGraphRelSigma_[iEta];
    if(bGraphRelMean_[iEta]) delete bGraphRelMean_[iEta];
    
    if(bFuncSigma_[iEta]) delete bFuncSigma_[iEta];
    if(bFuncEtaSigma_[iEta]) delete bFuncEtaSigma_[iEta];
    if(bFuncThetaSigma_[iEta]) delete bFuncThetaSigma_[iEta];
    if(bFuncPhiSigma_[iEta]) delete bFuncPhiSigma_[iEta];
    if(bFuncRelSigma_[iEta]) delete bFuncRelSigma_[iEta];
    
    if(bFuncMean_[iEta]) delete bFuncMean_[iEta];
    if(bFuncRelMean_[iEta]) delete bFuncRelMean_[iEta];
  }
}

void ResolutionFit::FillPFJets(TRootPFJet *jet, TLorentzVector *mcParticle)
{
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++)
  {
    if( fabs(jet->Eta()) >= towerBinning_[iEta] && fabs(jet->Eta()) < towerBinning_[iEta+1] )
    {
      chfHisto_[iEta]->Fill(jet->Pt(), jet->chargedHadronEnergyFraction());
      nhfHisto_[iEta]->Fill(jet->Pt(), jet->neutralHadronEnergyFraction());
      cefHisto_[iEta]->Fill(jet->Pt(), jet->chargedEmEnergyFraction());
      nefHisto_[iEta]->Fill(jet->Pt(), jet->neutralEmEnergyFraction());
      cmfHisto_[iEta]->Fill(jet->Pt(), jet->chargedMuEnergyFraction());
      ptHisto_[iEta]->Fill(jet->Pt());
    }
  }
  if(mcParticle) Fill(jet, mcParticle);
}

void ResolutionFit::Fill(TLorentzVector *lorentzVector, TLorentzVector *mcParticle)
{
  TLorentzVector tmpJet = *lorentzVector;
  float correction = 1;
  if(loadedResolutions_)
    correction *= ( 1 - EtCorrection(&tmpJet) );
  tmpJet = correction * tmpJet;
  
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++)
  {
    for(unsigned int iSel=0; iSel<nPtBins_; iSel++)
    {
      if( (StijnFlavorJES_ && mcParticle->Pt() > jetPtBinning_[iSel] && mcParticle->Pt() < jetPtBinning_[iSel+1]) || 
        (!StijnFlavorJES_ && tmpJet.Pt() > jetPtBinning_[iSel] && tmpJet.Pt() < jetPtBinning_[iSel+1]) )
      {
	      if(iEta == 0)
	      {
	        StijnFlavorJES_ ? binCenterHistoIncl_[iSel]->Fill( mcParticle->Pt() ) : binCenterHistoIncl_[iSel]->Fill( tmpJet.Pt() );
	        StijnFlavorJES_ ? resRelHistoIncl_[iSel]->Fill( tmpJet.Pt() / mcParticle->Pt() ) : resRelHistoIncl_[iSel]->Fill( (tmpJet.Et() - mcParticle->Et()) / tmpJet.Et() );
	      }
	      if( fabs(tmpJet.Eta()) >= towerBinning_[iEta] && fabs(tmpJet.Eta()) < towerBinning_[iEta+1] )
	      {
	        StijnFlavorJES_ ? binCenterHisto_[(iEta*nPtBins_)+iSel]->Fill( mcParticle->Pt() ) : binCenterHisto_[(iEta*nPtBins_)+iSel]->Fill( tmpJet.Pt() );
          StijnFlavorJES_ ? resHisto_[(iEta*nPtBins_)+iSel]->Fill( tmpJet.Pt() - mcParticle->Pt() ) : resHisto_[(iEta*nPtBins_)+iSel]->Fill( tmpJet.Et() - mcParticle->Et() );
	        resHistoEta_[(iEta*nPtBins_)+iSel]->Fill( tmpJet.Eta() - mcParticle->Eta() );
	        resHistoTheta_[(iEta*nPtBins_)+iSel]->Fill( tmpJet.Theta() - mcParticle->Theta() );
	        resHistoPhi_[(iEta*nPtBins_)+iSel]->Fill( tmpJet.DeltaPhi(*mcParticle) );
          StijnFlavorJES_ ? resRelHisto_[(iEta*nPtBins_)+iSel]->Fill( tmpJet.Pt() / mcParticle->Pt() ) : resRelHisto_[(iEta*nPtBins_)+iSel]->Fill( (tmpJet.Et() - mcParticle->Et()) / tmpJet.Et() );
	      }
      }
    } //loop over pTbins
  } //loop over etaBins
}

void ResolutionFit::CalculateResolutions()
{
  if(loadedResolutions_) cout << "WARNING: Resolutions are already loaded from a file, but will now be calculated" << endl;
  
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++){
    Char_t lowerUpperSigma[40], lowerUpperEtaSigma[40], lowerUpperThetaSigma[40], lowerUpperPhiSigma[40], lowerUpperRelSigma[40];
    sprintf(lowerUpperSigma,"bPtEtSigma_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperEtaSigma,"bPtEtaSigma_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperThetaSigma,"bPtThetaSigma_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperPhiSigma,"bPtPhiSigma_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperRelSigma,"bPtEtRelSigma_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
  
    bGraphSigma_[iEta] = new TGraphErrors(nPtBins_);
    bGraphSigma_[iEta]->SetNameTitle(lowerUpperSigma,lowerUpperSigma);
    bGraphEtaSigma_[iEta] = new TGraphErrors(nPtBins_);
    bGraphEtaSigma_[iEta]->SetNameTitle(lowerUpperEtaSigma,lowerUpperEtaSigma);
    bGraphThetaSigma_[iEta] = new TGraphErrors(nPtBins_);
    bGraphThetaSigma_[iEta]->SetNameTitle(lowerUpperThetaSigma,lowerUpperThetaSigma);
    bGraphPhiSigma_[iEta] = new TGraphErrors(nPtBins_);
    bGraphPhiSigma_[iEta]->SetNameTitle(lowerUpperPhiSigma,lowerUpperPhiSigma);
    bGraphRelSigma_[iEta] = new TGraphErrors(nPtBins_);
    bGraphRelSigma_[iEta]->SetNameTitle(lowerUpperRelSigma,lowerUpperRelSigma);
    
    Char_t lowerUpperMean[40], lowerUpperEtaMean[40], lowerUpperThetaMean[40], lowerUpperPhiMean[40], lowerUpperRelMean[40];
    sprintf(lowerUpperMean,"bPtEtMean_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperEtaMean,"bPtEtaMean_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperThetaMean,"bPtThetaMean_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperPhiMean,"bPtPhiMean_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
    sprintf(lowerUpperRelMean,"bPtEtRelMean_%1.3f_%1.3f",towerBinning_[iEta],towerBinning_[iEta+1]);
  
    bGraphMean_[iEta] = new TGraphErrors(nPtBins_);
    bGraphMean_[iEta]->SetNameTitle(lowerUpperMean,lowerUpperMean);
    bGraphEtaMean_[iEta] = new TGraphErrors(nPtBins_);
    bGraphEtaMean_[iEta]->SetNameTitle(lowerUpperEtaMean,lowerUpperEtaMean);
    bGraphThetaMean_[iEta] = new TGraphErrors(nPtBins_);
    bGraphThetaMean_[iEta]->SetNameTitle(lowerUpperThetaMean,lowerUpperThetaMean);
    bGraphPhiMean_[iEta] = new TGraphErrors(nPtBins_);
    bGraphPhiMean_[iEta]->SetNameTitle(lowerUpperPhiMean,lowerUpperPhiMean);
    bGraphRelMean_[iEta] = new TGraphErrors(nPtBins_);
    bGraphRelMean_[iEta]->SetNameTitle(lowerUpperRelMean,lowerUpperRelMean);
    
    if(iEta == 0){
      bGraphRelMeanIncl_ = new TGraphErrors(nPtBins_);
      bGraphRelMeanIncl_->SetNameTitle("bPtEtRelMean_EtaIncl","bPtEtRelMean_EtaIncl");
    }
  }
  
  TString ethname[14];
  TString ethnameeta[14];
  TString ethnametheta[14];
  TString ethnamephi[14];
  TString ethnamerel[14];
  for(unsigned int iSel = 0; iSel < nPtBins_; iSel++){
    TString lower_et_cut;
    TString upper_et_cut;
    Char_t tmpchar[10];
    sprintf(tmpchar,"%1.0f",jetPtBinning_[iSel]);
    lower_et_cut = tmpchar;
    sprintf(tmpchar,"%1.0f",jetPtBinning_[iSel+1]);
    upper_et_cut = tmpchar;
    
    ethname[iSel] = "h_"+label_+"_"+lower_et_cut+"_"+upper_et_cut;
    ethnameeta[iSel] = "h_eta_"+label_+"_"+lower_et_cut+"_"+upper_et_cut;
    ethnametheta[iSel] = "h_theta_"+label_+"_"+lower_et_cut+"_"+upper_et_cut;
    ethnamephi[iSel] = "h_phi_"+label_+"_"+lower_et_cut+"_"+upper_et_cut;
    ethnamerel[iSel] = "h_rel_"+label_+"_"+lower_et_cut+"_"+upper_et_cut;
  }
  
  Double_t sigma[14];
  Double_t sigmaErr[14];
  Double_t sigmaEta[14];
  Double_t sigmaErrEta[14];
  Double_t sigmaTheta[14];
  Double_t sigmaErrTheta[14];
  Double_t sigmaPhi[14];
  Double_t sigmaErrPhi[14];
  Double_t sigmaRel[14];
  Double_t sigmaErrRel[14];
  Double_t mean[14];
  Double_t meanErr[14];
  Double_t meanEta[14];
  Double_t meanErrEta[14];
  Double_t meanTheta[14];
  Double_t meanErrTheta[14];
  Double_t meanPhi[14];
  Double_t meanErrPhi[14];
  Double_t meanRel[14];
  Double_t meanErrRel[14];
  Double_t meanRelIncl[14];
  Double_t meanErrRelIncl[14];
  TH1F* controlHisto[14];
  TH1F* controlHistoEta[14];
  TH1F* controlHistoTheta[14];
  TH1F* controlHistoPhi[14];
  TH1F* controlHistoRel[14];
  TH1F* controlHistoRelIncl[14];
  
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++){
    for(unsigned int iSel=0; iSel<nPtBins_; iSel++){
      TString lower_et_cut;
      TString upper_et_cut;
      Char_t tmpchar[10];
      sprintf(tmpchar,"%1.0f",jetPtBinning_[iSel]);
      lower_et_cut = tmpchar;
      sprintf(tmpchar,"%1.0f",jetPtBinning_[iSel+1]);
      upper_et_cut = tmpchar;
      
      controlHisto[iSel] = (TH1F*)resHisto_[(iEta*nPtBins_)+iSel]->Clone(ethname[iSel]);
      controlHistoRel[iSel] = (TH1F*)resRelHisto_[(iEta*nPtBins_)+iSel]->Clone(ethnamerel[iSel]);
      controlCan_[iEta]->cd(iSel+1);
      controlHisto[iSel]->Draw();
      controlCanRel_[iEta]->cd(iSel+1);
      controlHistoRel[iSel]->Draw();
      if(!StijnFlavorJES_)
      {
        controlHistoEta[iSel] = (TH1F*)resHistoEta_[(iEta*nPtBins_)+iSel]->Clone(ethnameeta[iSel]);
        controlHistoTheta[iSel] = (TH1F*)resHistoTheta_[(iEta*nPtBins_)+iSel]->Clone(ethnametheta[iSel]);
        controlHistoPhi[iSel] = (TH1F*)resHistoPhi_[(iEta*nPtBins_)+iSel]->Clone(ethnamephi[iSel]);
        controlCanEta_[iEta]->cd(iSel+1);
        controlHistoEta[iSel]->Draw();
        controlCanTheta_[iEta]->cd(iSel+1);
        controlHistoTheta[iSel]->Draw();
        controlCanPhi_[iEta]->cd(iSel+1);
        controlHistoPhi[iSel]->Draw();
      }
      
      vector<double> sigmatemp = ExtractSigmaMean(controlHisto[iSel]);
      sigma[iSel] = sigmatemp[0];
      sigmaErr[iSel] = sigmatemp[1];
      mean[iSel] = sigmatemp[2];
      meanErr[iSel] = sigmatemp[3];
      bGraphSigma_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),sigma[iSel]);
      if(sigmaErr[iSel]!=0.) bGraphSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),sigmaErr[iSel]);
      else bGraphSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
      bGraphMean_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),mean[iSel]);
      if(meanErr[iSel]!=0.) bGraphMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),meanErr[iSel]);
      else bGraphMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
      controlCan_[iEta]->Update();
      
      if(!StijnFlavorJES_)
      {
        sigmatemp = ExtractSigmaMean(controlHistoEta[iSel]);
        sigmaEta[iSel] = sigmatemp[0];
        sigmaErrEta[iSel] = sigmatemp[1];
        meanEta[iSel] = sigmatemp[2];
        meanErrEta[iSel] = sigmatemp[3];
        bGraphEtaSigma_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),sigmaEta[iSel]);
        if(sigmaErrEta[iSel]!=0.)bGraphEtaSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),sigmaErrEta[iSel]);
        else bGraphEtaSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
        bGraphEtaMean_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),meanEta[iSel]);
        if(meanErrEta[iSel]!=0.) bGraphEtaMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),meanErrEta[iSel]);
        else bGraphEtaMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
        controlCanEta_[iEta]->Update();
        
        sigmatemp = ExtractSigmaMean(controlHistoTheta[iSel]);
        sigmaTheta[iSel] = sigmatemp[0];
        sigmaErrTheta[iSel] = sigmatemp[1];
        meanTheta[iSel] = sigmatemp[2];
        meanErrTheta[iSel] = sigmatemp[3];
        bGraphThetaSigma_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),sigmaTheta[iSel]);
        if(sigmaErrTheta[iSel]!=0.)bGraphThetaSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),sigmaErrTheta[iSel]);
        else bGraphThetaSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
        bGraphThetaMean_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),meanTheta[iSel]);
        if(meanErrTheta[iSel]!=0.) bGraphThetaMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),meanErrTheta[iSel]);
        else bGraphThetaMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
        controlCanTheta_[iEta]->Update();
        
        sigmatemp = ExtractSigmaMean(controlHistoPhi[iSel]);
        sigmaPhi[iSel] = sigmatemp[0];
        sigmaErrPhi[iSel] = sigmatemp[1];
        meanPhi[iSel] = sigmatemp[2];
        meanErrPhi[iSel] = sigmatemp[3];
        bGraphPhiSigma_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),sigmaPhi[iSel]);
        if(sigmaErrPhi[iSel]!=0.)bGraphPhiSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),sigmaErrPhi[iSel]);
        else bGraphPhiSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
        bGraphPhiMean_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),meanPhi[iSel]);
        if(meanErrPhi[iSel]!=0.) bGraphPhiMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),meanErrPhi[iSel]);
        else bGraphPhiMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
        controlCanPhi_[iEta]->Update();
      }
      
      sigmatemp = ExtractSigmaMean(controlHistoRel[iSel]);
      sigmaRel[iSel] = sigmatemp[0];
      sigmaErrRel[iSel] = sigmatemp[1];
      meanRel[iSel] = sigmatemp[2];
      meanErrRel[iSel] = sigmatemp[3];
      bGraphRelSigma_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),sigmaRel[iSel]);
      if(sigmaErrRel[iSel]!=0.) bGraphRelSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),sigmaErrRel[iSel]);
      else bGraphRelSigma_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
      bGraphRelMean_[iEta]->SetPoint(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMean(),meanRel[iSel]);
      if(meanErrRel[iSel]!=0.) bGraphRelMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),meanErrRel[iSel]);
      else bGraphRelMean_[iEta]->SetPointError(iSel,binCenterHisto_[(iEta*nPtBins_)+iSel]->GetMeanError(),9999);
      controlCanRel_[iEta]->Update();
      
      if(iEta == 0){
	      controlHistoRelIncl[iSel] = (TH1F*) resRelHistoIncl_[iSel]->Clone(ethnamerel[iSel]);
	      controlCanRelIncl_->cd(iSel+1);
	      controlHistoRelIncl[iSel]->Draw();
        
	      sigmatemp = ExtractSigmaMean(controlHistoRelIncl[iSel]);
	      meanRelIncl[iSel] = sigmatemp[2];
	      meanErrRelIncl[iSel] = sigmatemp[3];
	      bGraphRelMeanIncl_->SetPoint(iSel,binCenterHistoIncl_[iSel]->GetMean(),meanRelIncl[iSel]);
	      if(meanErrRelIncl[iSel]!=0) bGraphRelMeanIncl_->SetPointError(iSel,binCenterHistoIncl_[iSel]->GetMeanError(),meanErrRelIncl[iSel]);
	      else  bGraphRelMeanIncl_->SetPointError(iSel,binCenterHistoIncl_[iSel]->GetMeanError(),9999);
	      controlCanRelIncl_->Update();
      }
    }
  }
  
  TString etahname[13];
  for(UInt_t i_etetaphi = 0; i_etetaphi < 5; i_etetaphi++)
  {
    TF1* myetfunction = 0;
    TF1* mymeanetfunction = 0;
    // define 1D functions in P_T here!
    if(i_etetaphi!=0)
    {
      myetfunction = new TF1("myetfunction","[0]+[1]*sqrt(x)+[2]/x+[3]*x",jetPtBinning_[0],1800.);
      mymeanetfunction = new TF1("mymeanetfunction","[0]+[1]*sqrt(x)+[2]/x+[3]*x",jetPtBinning_[0],1800.);
      if(label_.find("Muon") == 0 && (i_etetaphi == 3 || i_etetaphi == 2 ) )
    	  myetfunction = new TF1("myetfunction","[0]+[1]*sqrt(x)+[2]/x",jetPtBinning_[0],1800.);
    }
    else
    {
      myetfunction = new TF1("myetfunction","[0]+[1]*sqrt(x)+[2]*(x)",jetPtBinning_[0],1800.);
      mymeanetfunction = new TF1("mymeanetfunction","[0]+[1]*sqrt(x)+[2]*(x)",jetPtBinning_[0],1800.);
      //      mymeanetfunction = new TF1("mymeanetfunction","1/([0]*x+[1])+[2]+[3]*x+[4]*log(x)+[5]*log(x)*log(x)",jetPtBinning_[0],1800.);
      if(label_.find("Muon") == 0)
    	  myetfunction = new TF1("myetfunction","[0]+[1]*sqrt(x)+[2]*(x)+[3]*x*x",jetPtBinning_[0],1800.);
    }
    myetfunction->SetLineWidth(2);
    myetfunction->SetLineColor(kRed);
    mymeanetfunction->SetLineWidth(2);
    mymeanetfunction->SetLineColor(kRed);

    for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++)
    {
      TString loweretastrg;
      TString upperetastrg;
      Char_t etachar[10];
      sprintf(etachar,"%1.3f",towerBinning_[iEta]);
      loweretastrg = etachar;
      sprintf(etachar,"%1.3f",towerBinning_[iEta+1]);
      upperetastrg = etachar;

      if(i_etetaphi==0){
	      etahname[iEta] = "mean_et_"+loweretastrg+"_"+upperetastrg;
	      bPtEtMean_[iEta]->cd();
	      bGraphMean_[iEta]->SetMarkerStyle(20);
	      bGraphMean_[iEta]->SetMarkerSize(0.8);
	      if(StijnFlavorJES_)
        {
          bGraphMean_[iEta]->GetYaxis()->SetTitle("Mean(p_{T}^{reco} - p_{T}^{genJet})");
	        bGraphMean_[iEta]->GetXaxis()->SetTitle("p_{T}^{genJet} [GeV]");
	        bGraphMean_[iEta]->GetXaxis()->SetLimits(0.,500);
	        bGraphMean_[iEta]->GetXaxis()->SetRangeUser(-30,30);
        }
        else
        {
  	      bGraphMean_[iEta]->GetYaxis()->SetTitle("Mean(E_{T}^{reco} - E_{T}^{parton})");
	        bGraphMean_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");
	        if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	          bGraphMean_[iEta]->GetXaxis()->SetLimits(0.,250);
	          bGraphMean_[iEta]->GetYaxis()->SetRangeUser(-7,12);
	        }
	        else if(label_.find("Muon") == 0){
	          bGraphMean_[iEta]->GetXaxis()->SetLimits(0.,120.);
	          bGraphMean_[iEta]->GetYaxis()->SetRangeUser(-2,0.5);
	        }
	        else if(label_.find("Neutrino") == 0){
	          bGraphMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	          bGraphMean_[iEta]->GetYaxis()->SetRangeUser(-5,8);
	        }
	      //else if(label_.find("Electron") == 0){
	      //bGraphMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	      //bGraphMean_[iEta]->GetYaxis()->SetRangeUser(-5,8);
	      //}
          
  	      bGraphMean_[iEta]->Fit("mymeanetfunction","+QR");
	        bFuncMean_[iEta] = (TF1*) mymeanetfunction->Clone();
	        bFuncMean_[iEta]->SetNameTitle(etahname[iEta],etahname[iEta]);
        }
        bGraphMean_[iEta]->DrawClone("ape1");
        
	      etahname[iEta] = "reso_et_"+loweretastrg+"_"+upperetastrg;
	      bPtEtSigma_[iEta]->cd();
	      bGraphSigma_[iEta]->SetMarkerStyle(20);
	      bGraphSigma_[iEta]->SetMarkerSize(0.8);
	      bGraphSigma_[iEta]->GetYaxis()->SetTitle("#sigma(E_{T}^{reco} - E_{T}^{parton})");
	      bGraphSigma_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        //bGraphSigma_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        //bGraphSigma_[iEta]->GetYaxis()->SetRangeUser(-7,12);
	      }
	      else if(label_.find("Muon") == 0){
	        //bGraphSigma_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        //bGraphSigma_[iEta]->GetYaxis()->SetRangeUser(0,3);
	      }
	      else if(label_.find("Neutrino") == 0){
	        //bGraphSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphSigma_[iEta]->GetYaxis()->SetRangeUser(10,19);
	      }
	      else if(label_.find("Electron") == 0){
	        //bGraphSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphSigma_[iEta]->GetYaxis()->SetRangeUser(10,19);
	      }

	      myetfunction->SetParLimits(0,0,9999999);
	      myetfunction->SetParLimits(1,0,9999999);
	      //        myetfunction->SetParLimits(2,0,9999999);
	      bGraphSigma_[iEta]->Fit("myetfunction","+QR");
	      bGraphSigma_[iEta]->DrawClone("ape1");
	      bFuncSigma_[iEta] = (TF1*) myetfunction->Clone();
	      bFuncSigma_[iEta]->SetNameTitle(etahname[iEta],etahname[iEta]);
      }
      else if(i_etetaphi==1 && !StijnFlavorJES_){
	      bPtEtaMean_[iEta]->cd();
	      bGraphEtaMean_[iEta]->SetMarkerStyle(20);
	      bGraphEtaMean_[iEta]->SetMarkerSize(0.8);
	      bGraphEtaMean_[iEta]->GetYaxis()->SetTitle("Mean(#eta^{reco} - #eta^{parton})");
	      bGraphEtaMean_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        bGraphEtaMean_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        bGraphEtaMean_[iEta]->GetYaxis()->SetRangeUser(-0.01,0.01);
	      }
	      else if(label_.find("Muon") == 0){
	        bGraphEtaMean_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        bGraphEtaMean_[iEta]->GetYaxis()->SetRangeUser(-0.0025,0.002);
	      }
	      else if(label_.find("Neutrino") == 0){
	        bGraphEtaMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphEtaMean_[iEta]->GetYaxis()->SetRangeUser(,);
	      }
	      else if(label_.find("Electron") == 0){	  
	        //bGraphEtaMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphEtaMean_[iEta]->GetYaxis()->SetRangeUser(,);
	      }

	      bGraphEtaMean_[iEta]->DrawClone("ape1");
              
	      etahname[iEta] = "reso_eta_"+loweretastrg+"_"+upperetastrg;
	      bPtEtaSigma_[iEta]->cd();
	      bGraphEtaSigma_[iEta]->SetMarkerStyle(20);
	      bGraphEtaSigma_[iEta]->SetMarkerSize(0.8);
	      bGraphEtaSigma_[iEta]->GetYaxis()->SetTitle("#sigma(#eta^{reco} - #eta^{parton})");
	      bGraphEtaSigma_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        bGraphEtaSigma_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        bGraphEtaSigma_[iEta]->GetYaxis()->SetRangeUser(0,0.06);
	      }
	      else if(label_.find("Muon") == 0){
	        bGraphEtaSigma_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        bGraphEtaSigma_[iEta]->GetYaxis()->SetRangeUser(0.,0.015);
	      }
	      else if(label_.find("Neutrino") == 0){
	        bGraphEtaSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphEtaSigma_[iEta]->GetYaxis()->SetRangeUser(,);
	      }
	      else if(label_.find("Electron") == 0){
	        //bGraphEtaSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphEtaSigma_[iEta]->GetYaxis()->SetRangeUser(,);
	      }
        
	      bGraphEtaSigma_[iEta]->Fit("myetfunction","+QR");
	      bGraphEtaSigma_[iEta]->SetMinimum(0);
	      bGraphEtaSigma_[iEta]->DrawClone("ape1");
	      bFuncEtaSigma_[iEta] = (TF1*) myetfunction->Clone();
	      bFuncEtaSigma_[iEta]->SetNameTitle(etahname[iEta],etahname[iEta]);
      }
      else if(i_etetaphi==2 && !StijnFlavorJES_){
	      bPtThetaMean_[iEta]->cd();
	      bGraphThetaMean_[iEta]->SetMarkerStyle(20);
	      bGraphThetaMean_[iEta]->SetMarkerSize(0.8);
	      bGraphThetaMean_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");
	      bGraphThetaMean_[iEta]->GetYaxis()->SetTitle("Mean(#theta^{reco} - #theta^{parton})");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        bGraphThetaMean_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        bGraphThetaMean_[iEta]->GetYaxis()->SetRangeUser(-0.01,0.01);
	      }
	      else if(label_.find("Muon") == 0){
	        bGraphThetaMean_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        bGraphThetaMean_[iEta]->GetYaxis()->SetRangeUser(-0.002,0.002);
	      }
	      else if(label_.find("Neutrino") == 0){
	        bGraphThetaMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        bGraphThetaMean_[iEta]->GetYaxis()->SetRangeUser(-0.01,0.01);
	      }
	      else if(label_.find("Electron") == 0){
	        //bGraphThetaMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphThetaMean_[iEta]->GetYaxis()->SetRangeUser(-0.01,0.01);
	      }
	      
	      bGraphThetaMean_[iEta]->DrawClone("ape1");
              
	      etahname[iEta] = "reso_theta_"+loweretastrg+"_"+upperetastrg;
	      bPtThetaSigma_[iEta]->cd();
	      bGraphThetaSigma_[iEta]->SetMarkerStyle(20);
	      bGraphThetaSigma_[iEta]->SetMarkerSize(0.8);
	      bGraphThetaSigma_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");
	      bGraphThetaSigma_[iEta]->GetYaxis()->SetTitle("#sigma(#theta^{reco} - #theta^{parton})");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        bGraphThetaSigma_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        bGraphThetaSigma_[iEta]->GetYaxis()->SetRangeUser(0,0.05);
	      }
	      else if(label_.find("Muon") == 0){
	        bGraphThetaSigma_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        bGraphThetaSigma_[iEta]->GetYaxis()->SetRangeUser(-0.002,0.01);
	      }
	      else if(label_.find("Neutrino") == 0){
	        bGraphThetaSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphThetaSigma_[iEta]->GetYaxis()->SetRangeUser(,);
	      }
	      else if(label_.find("Electron") == 0){
	        //bGraphThetaSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphThetaSigma_[iEta]->GetYaxis()->SetRangeUser(,);
	      }
	        
	      bGraphThetaSigma_[iEta]->Fit("myetfunction","+QR");
	      bGraphThetaSigma_[iEta]->SetMinimum(0);
	      bGraphThetaSigma_[iEta]->DrawClone("ape1");
	      bFuncThetaSigma_[iEta] = (TF1*) myetfunction->Clone();
	      bFuncThetaSigma_[iEta]->SetNameTitle(etahname[iEta],etahname[iEta]);
      }
      else if(i_etetaphi==3 && !StijnFlavorJES_){
	      bPtPhiMean_[iEta]->cd();
	      bGraphPhiMean_[iEta]->SetMarkerStyle(20);
	      bGraphPhiMean_[iEta]->SetMarkerSize(0.8);
	      bGraphPhiMean_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");
	      bGraphPhiMean_[iEta]->GetYaxis()->SetTitle("Mean(#phi^{reco} - #phi^{parton})");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        bGraphPhiMean_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        bGraphPhiMean_[iEta]->GetYaxis()->SetRangeUser(-0.01,0.01);
	      }
	      else if(label_.find("Muon") == 0){
	        bGraphPhiMean_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        bGraphPhiMean_[iEta]->GetYaxis()->SetRangeUser(-0.003,0.003);
	      }
	      else if(label_.find("Neutrino") == 0){
	        bGraphPhiMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphPhiMean_[iEta]->GetYaxis()->SetRangeUser(,);
	      }
	      else if(label_.find("Electron") == 0){
	        //bGraphPhiMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphPhiMean_[iEta]->GetYaxis()->SetRangeUser(,);
	      }	  

	      bGraphPhiMean_[iEta]->DrawClone("ape1");
              
	      etahname[iEta] = "reso_phi_"+loweretastrg+"_"+upperetastrg;
	      bPtPhiSigma_[iEta]->cd();
	      bGraphPhiSigma_[iEta]->SetMarkerStyle(20);
	      bGraphPhiSigma_[iEta]->SetMarkerSize(0.8);
	      bGraphPhiSigma_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");
	      bGraphPhiSigma_[iEta]->GetYaxis()->SetTitle("#sigma(#phi^{reco} - #phi^{parton})");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        bGraphPhiSigma_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        bGraphPhiSigma_[iEta]->GetYaxis()->SetRangeUser(0,0.07);
	      }
	      else if(label_.find("Muon") == 0){
	        bGraphPhiSigma_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        bGraphPhiSigma_[iEta]->GetYaxis()->SetRangeUser(0.,0.014);
	      }
	      else if(label_.find("Neutrino") == 0){
	        bGraphPhiSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphPhiSigma_[iEta]->GetYaxis()->SetRangeUser(,);
	      }
	      
	      bGraphPhiSigma_[iEta]->Fit("myetfunction","+QR");
	      bGraphPhiSigma_[iEta]->SetMinimum(0);
	      bGraphPhiSigma_[iEta]->DrawClone("ape1");
	      bFuncPhiSigma_[iEta] = (TF1*) myetfunction->Clone();
	      bFuncPhiSigma_[iEta]->SetNameTitle(etahname[iEta],etahname[iEta]);
      }
      else if(i_etetaphi==4)
      {
	      if(iEta == 0)
        {
          bPtEtRelMeanIncl_->cd();
          bGraphRelMeanIncl_->SetMarkerStyle(20);
          bGraphRelMeanIncl_->SetMarkerSize(0.8);
          if(StijnFlavorJES_)
          {
            bGraphRelMeanIncl_->GetYaxis()->SetTitle("Mean(#frac{p_{T}^{reco}}{p_{T}^{genJet}})");
            bGraphRelMeanIncl_->GetXaxis()->SetTitle("p_{T}^{genJet} [GeV]");
            bGraphRelMeanIncl_->GetXaxis()->SetLimits(0.,500);        
            bGraphRelMeanIncl_->GetYaxis()->SetRangeUser(0.7,1.3);
          }
          else
          {
            bGraphRelMeanIncl_->GetYaxis()->SetTitle("Mean(#frac{E_{T}^{reco} - E_{T}^{parton}}{E_{T}^{reco}})");
            bGraphRelMeanIncl_->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");

            if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
              bGraphRelMeanIncl_->GetXaxis()->SetLimits(0.,250);        
              bGraphRelMeanIncl_->GetYaxis()->SetRangeUser(-0.3,0.3);
            }
            else if(label_.find("Muon") == 0){
              bGraphRelMeanIncl_->GetXaxis()->SetLimits(0.,120.);
              bGraphRelMeanIncl_->GetYaxis()->SetRangeUser(-0.025,0.01);
            }
            else if(label_.find("Neutrino") == 0){
              bGraphRelMeanIncl_->GetXaxis()->SetLimits(0.,100.);
              bGraphRelMeanIncl_->GetYaxis()->SetRangeUser(-0.2,0.2);
            }
            else if(label_.find("Electron") == 0){
              //bGraphRelMeanIncl_->GetXaxis()->SetLimits(0.,100.);
              //bGraphRelMeanIncl_->GetYaxis()->SetRangeUser(-0.2,0.2);
            }    
          }
          bGraphRelMeanIncl_->DrawClone("ape1");
        }
	      etahname[iEta] = "mean_relative_et_"+loweretastrg+"_"+upperetastrg;
	      bPtEtRelMean_[iEta]->cd();
	      bGraphRelMean_[iEta]->SetMarkerStyle(20);
	      bGraphRelMean_[iEta]->SetMarkerSize(0.8);
	      if(StijnFlavorJES_)
	      {
	        bGraphRelMean_[iEta]->GetYaxis()->SetTitle("<p_{T}^{reco} / p_{T}^{genJet}>");
	        bGraphRelMean_[iEta]->GetXaxis()->SetTitle("p_{T}^{genJet} [GeV]");
	        bGraphRelMean_[iEta]->GetXaxis()->SetLimits(0.,500);        
          bGraphRelMean_[iEta]->GetYaxis()->SetRangeUser(0.7,1.3);
	      }
	      else
	      {
	        bGraphRelMean_[iEta]->GetYaxis()->SetTitle("Mean(#frac{E_{T}^{reco} - E_{T}^{parton}}{E_{T}^{reco}})");
	        bGraphRelMean_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");

	        if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	          bGraphRelMean_[iEta]->GetXaxis()->SetLimits(0.,250);        
	          bGraphRelMean_[iEta]->GetYaxis()->SetRangeUser(-0.3,0.3);
	        }
	        else if(label_.find("Muon") == 0){
	          bGraphRelMean_[iEta]->GetXaxis()->SetLimits(0.,120.);
	          bGraphRelMean_[iEta]->GetYaxis()->SetRangeUser(-0.05,0.05);
	        }
	        else if(label_.find("Neutrino") == 0){
	          bGraphRelMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	          bGraphRelMean_[iEta]->GetYaxis()->SetRangeUser(-0.2,0.2);
	        }
	        else if(label_.find("Electron") == 0){
	          //bGraphRelMean_[iEta]->GetXaxis()->SetLimits(0.,100.);
	          //bGraphRelMean_[iEta]->GetYaxis()->SetRangeUser(-0.2,0.2);
	        }
  	      
	        bGraphRelMean_[iEta]->Fit("mymeanetfunction","+QR");
	        bFuncRelMean_[iEta] = (TF1*) mymeanetfunction->Clone();
	        bFuncRelMean_[iEta]->SetNameTitle(etahname[iEta],etahname[iEta]);
	      }
        bGraphRelMean_[iEta]->DrawClone("ape1");
        
	      etahname[iEta] = "reso_relative_et_"+loweretastrg+"_"+upperetastrg;
	      bPtEtRelSigma_[iEta]->cd();
	      bGraphRelSigma_[iEta]->SetMarkerStyle(20);
	      bGraphRelSigma_[iEta]->SetMarkerSize(0.8);
	      bGraphRelSigma_[iEta]->GetYaxis()->SetTitle("#sigma(#frac{E_{T}^{reco} - E_{T}^{parton}}{E_{T}^{reco}})");
	      bGraphRelSigma_[iEta]->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");

	      if( label_.find("LightJet") == 0 || label_.find("BJet") == 0 ){
	        bGraphRelSigma_[iEta]->GetXaxis()->SetLimits(0.,250);        
	        bGraphRelSigma_[iEta]->GetYaxis()->SetRangeUser(0,0.26);
	      }
	      else if(label_.find("Muon") == 0){
	        bGraphRelSigma_[iEta]->GetXaxis()->SetLimits(0.,120.);
	        bGraphRelSigma_[iEta]->GetYaxis()->SetRangeUser(0.,0.05);
	      }
	      else if(label_.find("Neutrino") == 0){
	        bGraphRelSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphRelSigma_[iEta]->GetYaxis()->SetRangeUser(-0.1,);
	      }
	      else if(label_.find("Electron") == 0){
	        //bGraphRelSigma_[iEta]->GetXaxis()->SetLimits(0.,100.);
	        //bGraphRelSigma_[iEta]->GetYaxis()->SetRangeUser(-0.1,);
	      }	  

	      bGraphRelSigma_[iEta]->Fit("myetfunction","+QR");
	      bFuncRelSigma_[iEta] = (TF1*) myetfunction->Clone();
	      bFuncRelSigma_[iEta]->SetNameTitle(etahname[iEta],etahname[iEta]);
	    }
      bGraphRelSigma_[iEta]->DrawClone("ape1");
      
      if(label_.find("BJet") == 0 || label_.find("LightJet") == 0 || StijnFlavorJES_ )
	    {
	      //      cout << "myetfunction: " << myetfunction->GetExpFormula("p") << ", Prob: " << myetfunction->GetProb() << endl;
	      TLatex l;
	      l.SetTextAlign(11);
	      l.SetTextSize(0.035);
            
	      TString namestring;
	      Char_t namechar[20];
	      sprintf(namechar,"%i",(iEta+1)/2);
	      namestring = namechar;
	      TString loweretastring;
	      TString upperetastring;
	      TString etastring;
	      TString parastring;
	      Char_t parachar[30];
	      sprintf(parachar,"%1.3f",towerBinning_[iEta]);
	      loweretastring = parachar;
	      etastring = loweretastring;
	      etastring += "#leq|#eta|<";
	      sprintf(parachar,"%1.3f",towerBinning_[iEta+1]);
	      upperetastring = parachar;
	      etastring += upperetastring;
	      if(!StijnFlavorJES_) etastring += ": ";
            
	      sprintf(parachar,"%1.5f",myetfunction->GetParameter(0));
	      parastring = parachar;
            
	      if(i_etetaphi==0){
	        sprintf(parachar,"+%1.4f#sqrt{p_{T}}",myetfunction->GetParameter(1));
	        parastring += parachar;
	        sprintf(parachar,"+%1.4f",myetfunction->GetParameter(2));
	        parastring += parachar;
	        l.DrawLatex(10.,10,etastring + parastring +"p_{T}");
	            
	        bPtEtMean_[iEta]->cd();
	        TLatex l2;
	        l2.SetTextAlign(11);
	        l2.SetTextSize(0.035);
	        sprintf(parachar,"%1.5f",mymeanetfunction->GetParameter(0));
	        TString parastring2 = parachar;
	        sprintf(parachar,"+%1.4f#sqrt{p_{T}}",mymeanetfunction->GetParameter(1));
	        parastring2 += parachar;
	        sprintf(parachar,"+%1.4f",mymeanetfunction->GetParameter(2));
	        parastring2 += parachar;
	        l2.DrawLatex(10.,10,etastring + parastring2 +"p_{T}");
	      }
	      else{
	        sprintf(parachar,"+%1.4f#sqrt{p_{T}}",myetfunction->GetParameter(1));
	        parastring += parachar;
	        sprintf(parachar,"+%1.4f/p_{T}",myetfunction->GetParameter(2));
	        parastring += parachar;
	        sprintf(parachar,"+%1.4f",myetfunction->GetParameter(3));
	        parastring += parachar;
	        if(i_etetaphi==2 && !StijnFlavorJES_)
	          l.DrawLatex(10.,0.0025,etastring + parastring +"p_{T}");
	        else if(i_etetaphi==4){
	          //	      [0]+[1]*sqrt(x)+[2]*(x)     [0]+[1]*sqrt(x)+[2]/x+[3]*x    [0]+[1]*sqrt(x)+[2]/x+[3]*x
	          if(StijnFlavorJES_)
	          {
	            bPtEtRelMean_[iEta]->cd();
	            l.DrawLatex(30.,1.15,etastring);
	          }
	          else
	          {
	            l.DrawLatex(10.,0.15,etastring + parastring +"p_{T}");
	            bPtEtRelMean_[iEta]->cd();
	            TLatex l2;
	            l2.SetTextAlign(11);
	            l2.SetTextSize(0.035);
	            sprintf(parachar,"%1.5f",mymeanetfunction->GetParameter(0));
	            TString parastring2 = parachar;
	            sprintf(parachar,"+%1.4f#sqrt{p_{T}}",mymeanetfunction->GetParameter(1));
	            parastring2 += parachar;
	            sprintf(parachar,"+%1.4f/p_{T}",mymeanetfunction->GetParameter(2));
	            parastring2 += parachar;
	            sprintf(parachar,"+%1.4f",mymeanetfunction->GetParameter(3));
	            parastring2 += parachar;
	            l2.DrawLatex(10.,0.15,etastring + parastring2 +"p_{T}");
	          }
	        }
	        else if(!StijnFlavorJES_)
	          l.DrawLatex(10.,0.005,etastring + parastring +"p_{T}");
	      }
	    }
    }
    if(myetfunction) delete myetfunction;
  }
  calculatedResolutions_ = true;
  loadedResolutions_ = false;
}

void ResolutionFit::WritePlots(TFile *fout, bool savePNG, string pathPNG)
{
  if(!calculatedResolutions_) CalculateResolutions(); // Otherwise the TCanvasses are not filled or drawn
  cout << "Finished calculating the resolutions. Now plot them!" << endl;
  fout->cd();
  string dirname = "ResolutionFit_"+label_;
  if(fout->Get(dirname.c_str())==0)
    fout->mkdir(dirname.c_str());
  fout->cd(dirname.c_str());
  
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++)
  {
    chfHisto_[iEta]->Write();
    nhfHisto_[iEta]->Write();
    cefHisto_[iEta]->Write();
    nefHisto_[iEta]->Write();
    cmfHisto_[iEta]->Write();
    ptHisto_[iEta]->Write();
    
    TString name;
    Char_t tempName[40];
    sprintf(tempName,"_%i.png",iEta);
    name = label_+tempName;
    
    bGraphSigma_[iEta]->Write();
    bPtEtSigma_[iEta]->Write();
    if(savePNG) bPtEtSigma_[iEta]->SaveAs( pathPNG + "bPtEtSigma_" + name );
    bGraphRelSigma_[iEta]->Write();
    bPtEtRelSigma_[iEta]->Write();
    if(savePNG) bPtEtRelSigma_[iEta]->SaveAs( pathPNG + "bPtEtRelSigma_" + name );
    if(!StijnFlavorJES_)
    {
      bGraphEtaSigma_[iEta]->Write();
      bPtEtaSigma_[iEta]->Write();
      if(savePNG) bPtEtaSigma_[iEta]->SaveAs( pathPNG + "bPtEtaSigma_" + name );
      bGraphThetaSigma_[iEta]->Write();
      bPtThetaSigma_[iEta]->Write();
      if(savePNG) bPtThetaSigma_[iEta]->SaveAs( pathPNG + "bPtThetaSigma_" + name );
      bGraphPhiSigma_[iEta]->Write();
      bPtPhiSigma_[iEta]->Write();
      if(savePNG) bPtPhiSigma_[iEta]->SaveAs( pathPNG + "bPtPhiSigma_" + name );
    }
    
    bGraphMean_[iEta]->Write();
    bPtEtMean_[iEta]->Write();
    if(savePNG) bPtEtMean_[iEta]->SaveAs( pathPNG + "bPtEtMean_" + name );
    bGraphRelMean_[iEta]->Write();
    bPtEtRelMean_[iEta]->Write();
    if(savePNG) bPtEtRelMean_[iEta]->SaveAs( pathPNG + "bPtEtRelMean_" + name );
    if(iEta == 0){
      bGraphRelMeanIncl_->Write();
      bPtEtRelMeanIncl_->Write();
      if(savePNG) bPtEtRelMeanIncl_->SaveAs( (pathPNG + "bPtEtRelMean_EtaIncl.png").c_str() );
      controlCanRelIncl_->Write();
      if(savePNG) controlCanRelIncl_->SaveAs( (pathPNG + "ControlCanRelEt_EtaIncl.png").c_str() );
    }    
    if(!StijnFlavorJES_)
    {
      bGraphEtaMean_[iEta]->Write();
      bPtEtaMean_[iEta]->Write();
      if(savePNG) bPtEtaMean_[iEta]->SaveAs( pathPNG + "bPtEtaMean_" + name );
      bGraphThetaMean_[iEta]->Write();
      bPtThetaMean_[iEta]->Write();
      if(savePNG) bPtThetaMean_[iEta]->SaveAs( pathPNG + "bPtThetaMean_" + name );
      bGraphPhiMean_[iEta]->Write();
      bPtPhiMean_[iEta]->Write();
      if(savePNG) bPtPhiMean_[iEta]->SaveAs( pathPNG + "bPtPhiMean_" + name );
    }
    
    controlCan_[iEta]->Write();
    if(savePNG) controlCan_[iEta]->SaveAs( pathPNG + "ControlCanEt_" + name );
    controlCanRel_[iEta]->Write();
    if(savePNG) controlCanRel_[iEta]->SaveAs( pathPNG + "ControlCanRelEt_" + name );
    if(!StijnFlavorJES_)
    {
      controlCanEta_[iEta]->Write();
      if(savePNG) controlCanEta_[iEta]->SaveAs( pathPNG + "ControlCanEta_" + name );
      controlCanTheta_[iEta]->Write();
      if(savePNG) controlCanTheta_[iEta]->SaveAs( pathPNG + "ControlCanTheta_" + name );
      controlCanPhi_[iEta]->Write();
      if(savePNG) controlCanPhi_[iEta]->SaveAs( pathPNG + "ControlCanPhi_" + name );
    }
  }
}

void ResolutionFit::WriteResolutions(string file)
{
  if(!calculatedResolutions_) CalculateResolutions(); // Otherwise the TF1 are empty
  
  TFile *fout = new TFile (file.c_str (), "RECREATE");
  fout->cd();
  
  for(unsigned int iEta=0; iEta<nEtaBins_; iEta++)
    {
      bFuncSigma_[iEta]->Write();
      bFuncEtaSigma_[iEta]->Write();
      bFuncThetaSigma_[iEta]->Write();
      bFuncPhiSigma_[iEta]->Write();
      bFuncRelSigma_[iEta]->Write();
      bFuncMean_[iEta]->Write();
      bFuncRelMean_[iEta]->Write();
    }
  
  fout->Close();
}

void ResolutionFit::LoadResolutions(string file)
{
  if(calculatedResolutions_) cout << "WARNING: Resolutions are already calculated, but will now be loaded by other ones from a file: " << file << endl;
  TFile *fin = new TFile (file.c_str(), "READ");
  fin->cd();
  
  for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++){
    TString loweretastrg;
    TString upperetastrg;
    Char_t etachar[10];
    sprintf(etachar,"%1.3f",towerBinning_[iEta]);
    loweretastrg = etachar;
    sprintf(etachar,"%1.3f",towerBinning_[iEta+1]);
    upperetastrg = etachar;

    bFuncSigma_[iEta] = (TF1*) fin->Get("reso_et_"+loweretastrg+"_"+upperetastrg);
    bFuncEtaSigma_[iEta] = (TF1*) fin->Get("reso_eta_"+loweretastrg+"_"+upperetastrg);
    bFuncThetaSigma_[iEta] = (TF1*) fin->Get("reso_theta_"+loweretastrg+"_"+upperetastrg);
    bFuncPhiSigma_[iEta] = (TF1*) fin->Get("reso_phi_"+loweretastrg+"_"+upperetastrg);
    bFuncRelSigma_[iEta] = (TF1*) fin->Get("reso_relative_et_"+loweretastrg+"_"+upperetastrg);
    bFuncMean_[iEta] = (TF1*) fin->Get("mean_et_"+loweretastrg+"_"+upperetastrg);
    bFuncRelMean_[iEta] = (TF1*) fin->Get("mean_relative_et_"+loweretastrg+"_"+upperetastrg);
  }
  
  loadedResolutions_ = true;
  calculatedResolutions_ = false;
  
  fin->Close();
}

float ResolutionFit::EtResolution(TLorentzVector* jet)
{
  if( ! ( calculatedResolutions_ || loadedResolutions_ ) ){
    cout << "Resolutions are not loaded and/or calculated. returning -1"<<endl;
    return -1;
  }

  Float_t res = -1;
  for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++)
    if(fabs(jet->Eta()) >= towerBinning_[iEta] && fabs(jet->Eta()) < towerBinning_[iEta+1])
      {
	res = bFuncSigma_[iEta]->Eval( jet->Pt() );
	if( jet->Pt() > bFuncSigma_[iEta]->GetMaximumX() )
	  res = bFuncSigma_[iEta]->GetMaximum();
      }
  return res;
}

float ResolutionFit::EtaResolution(TLorentzVector* jet)
{
  if( ! ( calculatedResolutions_ || loadedResolutions_ ) )
    {
      cout << "Resolutions are not loaded and/or calculated. returning -1"<<endl;
      return -1;
    }
  
  Float_t res = -1;
  for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++)
    if(fabs(jet->Eta()) >= towerBinning_[iEta] && fabs(jet->Eta()) < towerBinning_[iEta+1])
      res = bFuncEtaSigma_[iEta]->Eval( jet->Pt() );
  return res;
}

float ResolutionFit::ThetaResolution(TLorentzVector* jet)
{
  if( ! ( calculatedResolutions_ || loadedResolutions_ ) )
    {
      cout << "Resolutions are not loaded and/or calculated. returning -1"<<endl;
      return -1;
    }

  Float_t res = -1;
  for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++)
    if(fabs(jet->Eta()) >= towerBinning_[iEta] && fabs(jet->Eta()) < towerBinning_[iEta+1])
      res = bFuncThetaSigma_[iEta]->Eval( jet->Pt() );
  return res;
}

float ResolutionFit::PhiResolution(TLorentzVector* jet)
{
  if( ! ( calculatedResolutions_ || loadedResolutions_ ) )
    {
      cout << "Resolutions are not loaded and/or calculated. returning -1"<<endl;
      return -1;
    }

  Float_t res = -1;
  for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++)
    if(fabs(jet->Eta()) >= towerBinning_[iEta] && fabs(jet->Eta()) < towerBinning_[iEta+1])
      res = bFuncPhiSigma_[iEta]->Eval( jet->Pt() );
  return res;
}

float ResolutionFit::EtCorrection(TLorentzVector* jet)
{
  if( ! ( calculatedResolutions_ || loadedResolutions_ ) )
    {
      cout << "Resolutions are not loaded and/or calculated. returning -1"<<endl;
      return -1;
    }
  
  Float_t res = -1;
  for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++)
    if(fabs(jet->Eta()) >= towerBinning_[iEta] && fabs(jet->Eta()) < towerBinning_[iEta+1])
      res = bFuncRelMean_[iEta]->Eval( jet->Pt() );
  return res;
}

vector<double> ResolutionFit::ExtractSigmaMean(TH1* theHisto)
{
  double lowLim = theHisto->GetMean()-theHisto->GetRMS(); 
  double upLim = theHisto->GetMean()+theHisto->GetRMS();
  
  //  cout<<"Fitting!\nHistoTitle = "<<theHisto->GetTitle()<<endl;
  //  cout<<"Mean = "<<theHisto->GetMean()<<"  RMS = "<<theHisto->GetRMS()<<endl;
  //  cout<<"lowLim = "<<lowLim<<"  upLim = "<<upLim<<endl;

  TF1 *f1 = new TF1("f1","gaus");
  f1->SetRange(lowLim,upLim);
  f1->SetParLimits(1,lowLim,upLim);
  theHisto->Fit("f1","RLQ");

  double mean =  f1->GetParameter(1);
  double deviation = f1->GetParameter(2);
  
  vector<double> output;

  lowLim = mean - (1.5*deviation);
  upLim  = mean + (1.5*deviation);

  //  cout<<"After first fit:  Mean = "<<mean<<"  Deviation = "<<deviation<<endl;
  //  cout<<"lowLim = "<<lowLim<<"  upLim = "<<upLim<<endl;
  
  f1->SetRange(lowLim,upLim);
  f1->SetParLimits(1,lowLim,upLim);
  theHisto->Fit("f1","RLQ");

  //  cout << "quality is Chi^2/NDF = " << f1->GetChisquare() << "/" << f1->GetNDF() << " = " <<  f1->GetChisquare()/f1->GetNDF() << endl;
  //  cout << "mean value: " << f1->GetParameter(1) << " +- " << f1->GetParError(1) << endl;
  //  cout << "sigma: " << f1->GetParameter(2) << " +- " << f1->GetParError(2) << endl;

  output.push_back(f1->GetParameter(2));
  output.push_back(f1->GetParError(2));
  output.push_back(f1->GetParameter(1));
  output.push_back(f1->GetParError(1));
  delete f1;
  return output;
}

