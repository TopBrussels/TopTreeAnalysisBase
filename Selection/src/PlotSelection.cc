#include "../interface/PlotSelection.h"

using namespace std;

PlotSelection::PlotSelection(){}
  
PlotSelection::PlotSelection(string labelVariable_, char* name, char* title, int nbins, float min, float max, int lineWidth, int lineColor){
    SetLabelVariable(labelVariable_);
    ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
}

PlotSelection::PlotSelection(const PlotSelection& psel):
  eff1Muon(psel.eff1Muon),
  eff1MuonIso(psel.eff1MuonIso),
  eff4Jets(psel.eff4Jets),
  effAll(psel.effAll),
  labelVariable_(psel.labelVariable_)
{;}
 
PlotSelection::~PlotSelection(){}

void PlotSelection::ConfigHisto(char* name, char* title, int nbins, float min, float max, int lineWidth, int lineColor){
   eff1Muon.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
   eff1MuonIso.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
   eff4Jets.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
   effAll.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
}
  
void PlotSelection::SetLabel(){ 
   eff1Muon.SetLabel(string("1Muon"));
   eff1MuonIso.SetLabel(string("1MuonIso"));
   eff4Jets.SetLabel(string("4Jets"));
   effAll.SetLabel(string("All"));
}
    
void PlotSelection::SetLabelVariable(string label){ 
   labelVariable_ = label;
   eff1Muon.SetLabelVariable(label);
   eff1MuonIso.SetLabelVariable(label);
   eff4Jets.SetLabelVariable(label);
   effAll.SetLabelVariable(label);
}
    
void PlotSelection::Fill(const Selection& sel, float var){
  eff1Muon.Fill(var, sel.isThere1Muon());
  eff1MuonIso.Fill(var, sel.isThere1MuonIso());
  eff4Jets.Fill(var, sel.isThere4Jets());
  effAll.Fill(var, sel.isSelected());
}

void PlotSelection::Write(TFile* fout){
  fout->cd();
  char* dirName = new char[100];
  sprintf(dirName,"Eff_Sel_%s",labelVariable_.c_str());
  fout->mkdir(dirName);
  fout->cd();
  eff1Muon.GetHisto().Write();
  eff1MuonIso.GetHisto().Write();
  eff4Jets.GetHisto().Write();
  effAll.GetHisto().Write();
}

 
       
