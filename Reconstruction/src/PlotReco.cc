#include "../interface/PlotReco.h"
  
PlotReco::PlotReco(){
  NcombMax_ = 10;
  PtJet_ = 30;
  EtaJet_ = 2.4;
  
  //h_EffvsPur = new TH2F*[NcombMax_-4];
  h_EffvsPur = new TGraphErrors*[NcombMax_-4];
  h_ChiSquare_Signal = new TH1F*[NcombMax_-4];
  h_ChiSquare_Bgk = new TH1F*[NcombMax_-4];
  char* nameHisto = new char[100];
  for(int i=0;i<NcombMax_-4;i++){
    sprintf(nameHisto,"EffVsPur_N%d_NcombMax%d_Pt%f_Eta_%f",i+4,NcombMax_,PtJet_,EtaJet_);
    //h_EffvsPur[i] = new TH2F(nameHisto,nameHisto,100,0,1,100,0,1);
    //h_EffvsPur[i] = new TGraphErrors();
    //h_EffvsPur[i]->SetName(nameHisto);
    //sprintf(nameHisto,"h_ChiSquare_Signal_N%d",i+4);
    sprintf(nameHisto,"ChiSquare_Signal_N%d_NcombMax%d_Pt%f_Eta_%f",i+4,NcombMax_,PtJet_,EtaJet_);
    h_ChiSquare_Signal[i] = new TH1F(nameHisto,nameHisto,200,0,100);
    //sprintf(nameHisto,"h_ChiSquare_Bgk_N%d",i+4);
    sprintf(nameHisto,"ChiSquare_Bgk_N%d_NcombMax%d_Pt%f_Eta_%f",i+4,NcombMax_,PtJet_,EtaJet_);
    h_ChiSquare_Bgk[i] = new TH1F(nameHisto,nameHisto,200,0,100);
  }
  h_PurVsMult = new TH1F("PurVsMult","PurVsMult",NcombMax_-4,4,NcombMax_);
  h_PurVsMultRef = new TH1F("PurVsMultRef","PurVsMultRef",NcombMax_-4,4,NcombMax_);
  h_PurVsMultMC = new TH1F("PurVsMultMC","PurVsMultMC",NcombMax_-4,4,NcombMax_);
  h_PurVsMultMCRef = new TH1F("PurVsMultMCRef","PurVsMultMCRef",NcombMax_-4,4,NcombMax_);

}

PlotReco::PlotReco(const int& NcombMax, const float& Pt, const float& Eta){
  NcombMax_ = NcombMax;
  PtJet_ = Pt;
  EtaJet_ = Eta;
  //h_EffvsPur = new TH2F*[NcombMax_-4];
  h_EffvsPur = new TGraphErrors*[NcombMax_-4];
  h_ChiSquare_Signal = new TH1F*[NcombMax_-4];
  h_ChiSquare_Bgk = new TH1F*[NcombMax_-4];
  char* nameHisto = new char[100];
  //_strtime( timeStr );
  for(int i=0;i<NcombMax_-4;i++){
    sprintf(nameHisto,"EffVsPur_N%d",i+4);
    //sprintf(nameHisto,"EffVsPur_N%d_NcombMax%d_Pt%f_Eta_%f",i+4,NcombMax_,PtJet_,EtaJet_);
    if(h_EffvsPur[i]!=NULL) h_EffvsPur[i] = NULL;
    //h_EffvsPur[i] = new TH2F(nameHisto,nameHisto,100,0,1,100,0,1);
    //h_EffvsPur[i] = new TGraphErrors();
    //h_EffvsPur[i]->SetName(nameHisto);
    sprintf(nameHisto,"h_ChiSquare_Signal_N%d",i+4);
    //sprintf(nameHisto,"ChiSquare_Signal_N%d_NcombMax%d_Pt%f_Eta_%f",i+4,NcombMax_,PtJet_,EtaJet_);
    if(h_ChiSquare_Signal[i]!=NULL) h_ChiSquare_Signal[i] = NULL;
    h_ChiSquare_Signal[i] = new TH1F(nameHisto,nameHisto,100,0,100);
    sprintf(nameHisto,"h_ChiSquare_Bgk_N%d",i+4);
    //sprintf(nameHisto,"ChiSquare_Bgk_N%d_NcombMax%d_Pt%f_Eta_%f",i+4,NcombMax_,PtJet_,EtaJet_);
    if(h_ChiSquare_Bgk[i]!=NULL) h_ChiSquare_Bgk[i] = NULL;
    h_ChiSquare_Bgk[i] = new TH1F(nameHisto,nameHisto,100,0,100);
  }
  if(h_PurVsMult !=NULL) h_PurVsMult = NULL;
  h_PurVsMult = new TH1F("PurVsMult","PurVsMult",NcombMax_-4,4,NcombMax_);
  if(h_PurVsMultRef !=NULL) h_PurVsMultRef = NULL;
  h_PurVsMultRef = new TH1F("PurVsMultRef","PurVsMultRef",NcombMax_-4,4,NcombMax_);
  if(h_PurVsMultMC !=NULL) h_PurVsMultMC = NULL;
  h_PurVsMultMC = new TH1F("PurVsMultMC","PurVsMultMC",NcombMax_-4,4,NcombMax_);
  if(h_PurVsMultMCRef !=NULL) h_PurVsMultMCRef = NULL;
  h_PurVsMultMCRef = new TH1F("PurVsMultMCRef","PurVsMultMCRef",NcombMax_-4,4,NcombMax_);
}

PlotReco::PlotReco(const PlotReco& preco){
  NcombMax_ = preco.NcombMax_;
  PtJet_ = preco.PtJet_;
  EtaJet_ = preco.EtaJet_;
  h_EffvsPur = preco.h_EffvsPur;
  h_ChiSquare_Signal = preco.h_ChiSquare_Signal;
  h_ChiSquare_Bgk = preco.h_ChiSquare_Bgk;
  h_PurVsMult = preco.h_PurVsMult;
  h_PurVsMultRef = preco.h_PurVsMultRef;
  h_PurVsMultMC = preco.h_PurVsMultMC;
  h_PurVsMultMCRef = preco.h_PurVsMultMCRef;
}
  
PlotReco::~PlotReco(){
/*
cerr<<"delete"<<endl;
  //
  for(int i=0;i<NcombMax_-4;i++){
    cout<<"loop delete "<<i<<endl;
    h_EffvsPur->Delete();
    h_ChiSquare_Signal->Delete();
    h_ChiSquare_Bgk->Delete();
    //if(h_EffvsPur[i]!=NULL) delete h_EffvsPur[i];
    //if(h_ChiSquare_Signal[i]!=NULL) delete h_ChiSquare_Signal[i];
    //if(h_ChiSquare_Bgk[i]!=NULL) delete h_ChiSquare_Bgk[i];
    if(h_EffvsPur[i]!=NULL)  h_EffvsPur[i] = NULL;
    if(h_ChiSquare_Signal[i]!=NULL)  h_ChiSquare_Signal[i] = NULL;
    if(h_ChiSquare_Bgk[i]!=NULL)  h_ChiSquare_Bgk[i] = NULL;
  }
  //
  cerr<<"delete"<<endl;
  if(h_EffvsPur!=NULL){ 
	//delete h_EffvsPur;
  	h_EffvsPur = NULL;
  }
  cerr<<"delete"<<endl;
  if(h_ChiSquare_Signal!=NULL){
  	//delete h_ChiSquare_Signal;
  	h_ChiSquare_Signal = NULL;
  }
  cerr<<"delete"<<endl;
  if(h_ChiSquare_Bgk!=NULL){
  	//delete h_ChiSquare_Bgk;
  	h_ChiSquare_Bgk = NULL;
		
  }
  cerr<<"delete"<<endl;
  cout<<h_PurVsMult<<endl;
  cout<<h_PurVsMultRef<<endl;
  cout<<h_PurVsMultMC<<endl;
  cout<<h_PurVsMultMCRef<<endl;
  cout<<"isZombie "<<h_PurVsMult->IsZombie()<<endl;
  //h_PurVsMult->Dump();
  //cout<<h_PurVsMult->IsA ()<<endl;
  //if(h_PurVsMult->IsA () && h_PurVsMult->IsA ()->InheritsFrom ("TH1F"))cout<<"is TH1F"<<endl;
  //else cout<<"pas TH1F"<<endl;
  //cout<<"Info "<<endl;
  //h_PurVsMult->Info();
  if(h_PurVsMult!=NULL){
        cout<<"toto"<<endl;
	//h_PurVsMult->Delete();
  	//delete h_PurVsMult;
        cout<<"tata"<<endl;
  	h_PurVsMult = NULL;
  }
  cerr<<"delete"<<endl;
  if(h_PurVsMultRef!=NULL){
  	//h_PurVsMultRef->Delete();
	//delete h_PurVsMultRef;
  	h_PurVsMultRef = NULL;
  }
  cerr<<"delete"<<endl;
  if(h_PurVsMultMC!=NULL){
  	//h_PurVsMultMC->Delete();
  	//delete h_PurVsMultMC;
  	h_PurVsMultMC = NULL;
  }
  cerr<<"delete"<<endl;
  if(h_PurVsMultMCRef!=NULL){
  	//h_PurVsMultMCRef->Delete();
  	//delete h_PurVsMultMCRef;
  	h_PurVsMultMCRef = NULL;
  }
  cerr<<"end delete"<<endl;
*/
}


void PlotReco::Compute(){
  float s = 0;
  float b = 0;
  float n = 0;
  float ntot = 0;
  float eff = 0;
  float effError = 0;
  float pur = 0;
  float purError = 0;
  char* nameHisto = new char[100];
 for(int i=0;i<NcombMax_-4;i++){
  sprintf(nameHisto,"EffVsPur_N%d",i+4);
  
  int N = h_ChiSquare_Signal[i]->GetNbinsX();
  float* PointX  = new float[N];
  float* PointY  = new float[N];
  float* PointXError  = new float[N];
  float* PointYError  = new float[N];
  ntot = h_ChiSquare_Signal[i]->Integral() + h_ChiSquare_Bgk[i]->Integral();
  for(int j=1;j<h_ChiSquare_Signal[i]->GetNbinsX();j++){
    s = h_ChiSquare_Signal[i]->Integral(1,j+1);
    b = h_ChiSquare_Bgk[i]->Integral(1,j+1);
    n = s+b;
    if(ntot>0){
     eff = n/ntot;
     effError = sqrt(eff*(1-eff)/ntot);
    }
    if(n>0){
      pur = s/n;
      purError = sqrt(pur*(1-pur)/n);
    }
    //h_EffvsPur[i]->SetPoint(i+1,eff,pur);
    //h_EffvsPur[i]->SetPointError(i+1,effError,purError);
    PointX[j] = eff;
    PointY[j] = pur;
    PointXError[j] = effError;
    PointYError[j] = purError;
  }
  h_EffvsPur[i] = new TGraphErrors(N,PointX,PointY,PointXError,PointYError);
  h_EffvsPur[i]->SetName(nameHisto);
 }
}

void PlotReco::Normalized(){
 for(int i=0;i<NcombMax_-4;i++){
  if(h_ChiSquare_Signal[i]->Integral()>0) h_ChiSquare_Signal[i]->Scale(1/h_ChiSquare_Signal[i]->Integral());
  if(h_ChiSquare_Bgk[i]->Integral()>0) h_ChiSquare_Bgk[i]->Scale(1/h_ChiSquare_Bgk[i]->Integral());
 }
}

void PlotReco::Fill(const bool& signal, const double& ChiSquare, const int& Ncomb){
   if(Ncomb<NcombMax_ && Ncomb>3){
     if(signal) h_ChiSquare_Signal[Ncomb-4]->Fill(ChiSquare);
     else h_ChiSquare_Bgk[Ncomb-4]->Fill(ChiSquare); 
   }
   if(signal) h_PurVsMult->Fill(Ncomb);
   h_PurVsMultRef->Fill(Ncomb);
}

void PlotReco::FillMC(const bool& signal, const double& ChiSquare, const int& Ncomb){
   if(signal) h_PurVsMultMC->Fill(Ncomb);
   h_PurVsMultMCRef->Fill(Ncomb);
}

void PlotReco::Write(TFile* fout){
  fout->cd();
  char* dirName = new char[100];
  sprintf(dirName,"Reco_Pt%f_Eta%f",PtJet_,EtaJet_);
  fout->mkdir(dirName);
  fout->cd(dirName);
  for(int i=0;i<NcombMax_-4;i++){
    h_EffvsPur[i]->Write();
    h_ChiSquare_Signal[i]->Write();
    h_ChiSquare_Bgk[i]->Write();
  }
  for(int i=1;i<h_PurVsMult->GetNbinsX()+1;i++) 
    if(h_PurVsMultRef->GetBinContent(i)>0){
    	float pur = (float)(h_PurVsMult->GetBinContent(i)/h_PurVsMultRef->GetBinContent(i)) ;
	float error = sqrt(pur*(1-pur)/h_PurVsMultRef->GetBinContent(i));
	h_PurVsMult->SetBinContent(i,pur);
	h_PurVsMult->SetBinError(i,pur*error);
    }
  for(int i=1;i<h_PurVsMultMC->GetNbinsX()+1;i++) 
    if(h_PurVsMultMCRef->GetBinContent(i)>0){
    	float pur = (float)(h_PurVsMultMC->GetBinContent(i)/h_PurVsMultMCRef->GetBinContent(i)) ;
	float error = sqrt(pur*(1-pur)/h_PurVsMultMCRef->GetBinContent(i));
	h_PurVsMultMC->SetBinContent(i,pur);
	h_PurVsMultMC->SetBinError(i,pur*error);
    }
  h_PurVsMult->Write();
  h_PurVsMultRef->Write();
  h_PurVsMultMC->Write();
  h_PurVsMultMCRef->Write();
}

