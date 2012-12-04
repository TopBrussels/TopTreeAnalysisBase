#include "../interface/Efficiency.h"

  
Efficiency::Efficiency(){
  subSetEvents_ = 0;
  nofEvents_ = 0;
  histo_ = NULL;
  histoRef_ = NULL;
  label_ = string();
  labelVariable_ = string();
}

Efficiency::Efficiency(string label_, string labelVariable_, char* name, char* title, int nbins, float min, float max, int lineWidth, int lineColor){
  SetLabel(label_);
  SetLabelVariable(labelVariable_);
  ConfigHisto(name,title,nbins,min,max,lineWidth,lineColor);
}

Efficiency::Efficiency(const Efficiency& eff):
	subSetEvents_(eff.subSetEvents_),
	nofEvents_(eff.nofEvents_),
	histo_(eff.histo_),
	histoRef_(eff.histoRef_),
	label_(eff.label_),
	labelVariable_(eff.labelVariable_)
{}

Efficiency::~Efficiency(){
  histo_ = NULL;
  histoRef_ = NULL;
}


void Efficiency::Fill(float variable, bool b){
  subSetEvents_++;
  nofEvents_++;
  if(b) histo_->Fill(variable);
  histoRef_->Fill(variable);
}

void Efficiency::ConfigHisto(char* name, char* title, int nbins, float min, float max, int lineWidth, int lineColor){
  histo_ = new TH1F(name,title,nbins,min,max);
  histo_->SetLineWidth(lineWidth);
  histo_->SetLineColor(lineColor);
  histoRef_ = new TH1F(name,title,nbins,min,max);
  histoRef_->SetLineWidth(lineWidth);
  histoRef_->SetLineColor(lineColor);
}

void Efficiency::Compute(){
  for(int i=1; i<histo_->GetNbinsX()+1;i++){
    float eff = 0;
    float n = histoRef_->GetBinContent(i);
    float error = 0 ;
    if(n>0){
      eff = (float) (histo_->GetBinContent(i)/histoRef_->GetBinContent(i));
      error = sqrt((eff*(1-eff))/n);
    }
    histo_->SetBinContent(i, eff);
    histo_->SetBinError(i, error);
  }
}
  
