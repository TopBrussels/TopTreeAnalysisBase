#include "../interface/Resolution.h"
  
Resolution::Resolution(){
  lquark_.clear();
  MassW_ = -9999;
  MassTop_ = -9999;
}


Resolution::Resolution(pair<TLorentzVector,TLorentzVector> qFromW1, pair<TLorentzVector,TLorentzVector> qFromW2, pair<TLorentzVector,TLorentzVector> bQuark){
  lquark_.push_back(qFromW1);
  lquark_.push_back(qFromW2);
  bquark_ = bQuark;
  MassW_ = -9999;
  MassTop_ = -9999;
}

Resolution::Resolution(const Resolution& res):
  lquark_(res.lquark_),
  bquark_(res.bquark_),
  MassW_(res.MassW_),
  MassTop_(res.MassTop_)
{;}

Resolution::~Resolution(){
  lquark_.clear();
}

void Resolution::ComputeVariables(){
  TLorentzVector W;
  TLorentzVector Top;
  if(lquark_.size()==2){
    W = lquark_[0].first+lquark_[1].first;
    Top = lquark_[0].first+lquark_[1].first+bquark_.first;
  }
  MassW_ = W.M(); 
  MassTop_ = Top.M();
}

//0: all - 1: l-jets - 2: b-jets
//return reso & Pt
  
std::vector<std::pair<float,float> > Resolution::PtResolution(int jet){
  std::vector<std::pair<float,float> > res;
  if(jet<2)
    for(unsigned int i=0;i<lquark_.size();i++)
      if(lquark_[i].second.Pt()>0) res.push_back(pair<float,float> ((lquark_[i].first.Pt() - lquark_[i].second.Pt())/lquark_[i].second.Pt(),lquark_[i].first.Pt()));
  if(jet ==0 || jet == 1)
    if(bquark_.second.Pt()>0) res.push_back(pair<float,float> ((bquark_.first.Pt() - bquark_.second.Pt())/bquark_.second.Pt(),bquark_.first.Pt()));
  return res;
}


std::vector<std::pair<float,float> > Resolution::EtaResolution(int jet){
  std::vector<std::pair<float,float> > res;
  if(jet<2)
    for(unsigned int i=0;i<lquark_.size();i++)
      if(lquark_[i].second.Eta()>0) res.push_back(pair<float,float> ((lquark_[i].first.Eta() - lquark_[i].second.Eta())/lquark_[i].second.Eta(),lquark_[i].first.Pt()));
  if(jet ==0 || jet == 1)
    if(bquark_.second.Eta()>0) res.push_back(pair<float,float> ((bquark_.first.Eta() - bquark_.second.Eta())/bquark_.second.Eta(),bquark_.first.Pt()));
  return res;
}

std::vector<std::pair<float,float> > Resolution::PhiResolution(int jet){
  std::vector<std::pair<float,float> > res;
  if(jet<2)
    for(unsigned int i=0;i<lquark_.size();i++)
      if(lquark_[i].second.Phi()>0) res.push_back(pair<float,float> ((lquark_[i].first.Phi() - lquark_[i].second.Phi())/lquark_[i].second.Phi(),lquark_[i].first.Pt()));
  if(jet ==0 || jet == 1)
    if(bquark_.second.Phi()>0) res.push_back(pair<float,float> ((bquark_.first.Phi() - bquark_.second.Phi())/bquark_.second.Phi(),bquark_.first.Pt()));
  return res;
}

