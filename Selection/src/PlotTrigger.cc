#include "../interface/PlotTrigger.h"

using namespace std;

PlotTrigger::PlotTrigger(){}
  
PlotTrigger::PlotTrigger(string labelVariable_, char* name, char* title, int nbins, float min, float max, int lineWidth, int lineColor){
    SetLabel();
    SetLabelVariable(labelVariable_);
    ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
}

PlotTrigger::PlotTrigger(const PlotTrigger& ptrig):
  eff_HLT_QuadJet60(ptrig.eff_HLT_QuadJet60),
  eff_HLT_SumET120(ptrig.eff_HLT_SumET120),
  eff_HLT_MET75(ptrig.eff_HLT_MET75),
  eff_HLT_MET35_HT350(ptrig.eff_HLT_MET35_HT350),
  eff_HLT_TripleJet60_MET60(ptrig.eff_HLT_TripleJet60_MET60),
  eff_HLT_QuadJet35_MET60(ptrig.eff_HLT_QuadJet35_MET60),
  eff_HLT_IsoMu15(ptrig.eff_HLT_IsoMu15),
  eff_HLT_IsoMu7_Jet40(ptrig.eff_HLT_IsoMu7_Jet40),
  eff_HLT_Mu5_TripleJet30(ptrig.eff_HLT_Mu5_TripleJet30),
  labelVariable_(ptrig.labelVariable_)
{;}
 
PlotTrigger::~PlotTrigger(){}

void PlotTrigger::ConfigHisto(char* name, char* title, int nbins, float min, float max, int lineWidth, int lineColor){
  eff_HLT_QuadJet60.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_SumET120.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_MET75.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_MET35_HT350.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_TripleJet60_MET60.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_QuadJet35_MET60.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_IsoMu15.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_IsoMu7_Jet40.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
  eff_HLT_Mu5_TripleJet30.ConfigHisto(name, title, nbins, min, max, lineWidth, lineColor);
}
  
void PlotTrigger::SetLabel(){ 
  eff_HLT_QuadJet60.SetLabel(string("HLT_QuadJet60"));
  eff_HLT_SumET120.SetLabel(string("HLT_SumET120"));
  eff_HLT_MET75.SetLabel(string("HLT_MET75"));
  eff_HLT_MET35_HT350.SetLabel(string("HLT_MET35_HT350"));
  eff_HLT_TripleJet60_MET60.SetLabel(string("HLT_TripleJet60_MET60"));
  eff_HLT_QuadJet35_MET60.SetLabel(string("HLT_QuadJet35_MET60"));
  eff_HLT_IsoMu15.SetLabel(string("HLT_IsoMu15"));
  eff_HLT_IsoMu7_Jet40.SetLabel(string("HLT_IsoMu7_Jet40"));
  eff_HLT_Mu5_TripleJet30.SetLabel(string("HLT_Mu5_TripleJet30"));
}
    
void PlotTrigger::SetLabelVariable(string label){ 
  labelVariable_ = label;
  eff_HLT_QuadJet60.SetLabelVariable(label);
  eff_HLT_SumET120.SetLabelVariable(label);
  eff_HLT_MET75.SetLabelVariable(label);
  eff_HLT_MET35_HT350.SetLabelVariable(label);
  eff_HLT_TripleJet60_MET60.SetLabelVariable(label);
  eff_HLT_QuadJet35_MET60.SetLabelVariable(label);
  eff_HLT_IsoMu15.SetLabelVariable(label);
  eff_HLT_IsoMu7_Jet40.SetLabelVariable(label);
  eff_HLT_Mu5_TripleJet30.SetLabelVariable(label);
}
  
bool PlotTrigger::TriggerAccepted(TRootRun& trig, string trigSearched, int runID){
  UInt_t itrig = trig.getHLTinfo(runID).hltPath(trigSearched);
  if(trig.getHLTinfo(runID).hltAccept(itrig)>0) return true;
  return false;
}

void PlotTrigger::Fill(TRootRun& trig, float var, int runID){
  eff_HLT_QuadJet60.Fill(var, TriggerAccepted(trig,string("HLT_QuadJet60"),runID));
  eff_HLT_SumET120.Fill(var, TriggerAccepted(trig,string("HLT_SumET120"),runID));
  eff_HLT_MET75.Fill(var, TriggerAccepted(trig,string("HLT_MET75"),runID));
  eff_HLT_MET35_HT350.Fill(var, TriggerAccepted(trig,string("HLT_MET35_HT350"),runID));
  eff_HLT_TripleJet60_MET60.Fill(var, TriggerAccepted(trig,string("HLT_TripleJet60_MET60"),runID));
  eff_HLT_QuadJet35_MET60.Fill(var, TriggerAccepted(trig,string("HLT_QuadJet35_MET60"),runID));
  eff_HLT_IsoMu15.Fill(var, TriggerAccepted(trig,string("HLT_IsoMu15"),runID));
  eff_HLT_IsoMu7_Jet40.Fill(var, TriggerAccepted(trig,string("HLT_IsoMu7_Jet40"),runID));
  eff_HLT_Mu5_TripleJet30.Fill(var, TriggerAccepted(trig,string("HLT_Mu5_TripleJet30"),runID));
}

void PlotTrigger::Write(TFile* fout){
  fout->cd();
  char* dirName = new char[100];
  sprintf(dirName,"Eff_Sel_%s",labelVariable_.c_str());
  fout->mkdir(dirName);
  fout->cd();
  eff_HLT_QuadJet60.GetHisto().Write();
  eff_HLT_SumET120.GetHisto().Write();
  eff_HLT_MET75.GetHisto().Write();
  eff_HLT_MET35_HT350.GetHisto().Write();
  eff_HLT_TripleJet60_MET60.GetHisto().Write();
  eff_HLT_QuadJet35_MET60.GetHisto().Write();
  eff_HLT_IsoMu15.GetHisto().Write();
  eff_HLT_IsoMu7_Jet40.GetHisto().Write();
  eff_HLT_Mu5_TripleJet30.GetHisto().Write();
}
		  
 
       
