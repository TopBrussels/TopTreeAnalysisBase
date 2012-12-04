#include "../interface/PlotRecoScan.h"

  
PlotRecoScan::PlotRecoScan(){
  NcombMax_ = 10;
  PtJet_.push_back(30);
  EtaJet_.push_back(2.4);
  for(unsigned int i=0;i<PtJet_.size();i++){
    vector<pair<PlotReco,float> > vec;
    for(unsigned int j=0;j<EtaJet_.size();j++){
      vec.push_back(pair<PlotReco,float>(PlotReco(NcombMax_,PtJet_[i],EtaJet_[j]),EtaJet_[j]));
    }
    PlotReco_.push_back(pair<vector<pair<PlotReco,float> >,float >(vec,PtJet_[i]));
    vec.clear();
  }
}

PlotRecoScan::PlotRecoScan(const int& NcombMax, const vector<float>& PtJet, const vector<float>& EtaJet){
  NcombMax_ = 10;
  PtJet_ = PtJet;
  EtaJet_ = EtaJet;
  for(unsigned int i=0;i<PtJet_.size();i++){
    vector<pair<PlotReco,float> > vec;
    for(unsigned int j=0;j<EtaJet_.size();j++){
      PlotReco var_temp(NcombMax_,PtJet_[i],EtaJet_[j]);
      //vec.push_back(pair<PlotReco,float>(PlotReco(NcombMax_,PtJet_[i],EtaJet_[j]),EtaJet_[j]));
      vec.push_back(pair<PlotReco,float>(var_temp,EtaJet_[j]));
    }
    PlotReco_.push_back(pair<vector<pair<PlotReco,float> >,float >(vec,PtJet_[i]));
    vec.clear();
  }
}

PlotRecoScan::PlotRecoScan(const PlotRecoScan& prs):
  NcombMax_(prs.NcombMax_),
  PtJet_(prs.PtJet_),
  EtaJet_(prs.EtaJet_),
  PlotReco_(prs.PlotReco_)
{;}

PlotRecoScan::~PlotRecoScan(){
/*
  PtJet_.clear();
  EtaJet_.clear();
  PlotReco_.clear();
*/
}


void PlotRecoScan::Compute(){
  for(unsigned int i=0;i<PlotReco_.size();i++){
    for(unsigned int j=0;j<PlotReco_[i].first.size();j++){
      (PlotReco_[i].first)[j].first.Compute();
    }
  }
}

void PlotRecoScan::Normalized(){
  for(unsigned int i=0;i<PlotReco_.size();i++){
    for(unsigned int j=0;j<PlotReco_[i].first.size();j++){
      (PlotReco_[i].first)[j].first.Normalized();
    }
  }
}

void PlotRecoScan::Fill(const bool& signal, const double& ChiSquare, const int& Ncomb, const float& Pt, const float& Eta){
  for(unsigned int i=0;i<PlotReco_.size();i++){
    if(Pt == PlotReco_[i].second)
     for(unsigned int j=0;j<PlotReco_[i].first.size();j++){
       if(Eta == (PlotReco_[i].first)[j].second)
          (PlotReco_[i].first)[j].first.Fill(signal,ChiSquare,Ncomb);
     }
  }

}

void PlotRecoScan::FillMC(const bool& signal, const double& ChiSquare, const int& Ncomb, const float& Pt, const float& Eta){
  for(unsigned int i=0;i<PlotReco_.size();i++){
    if(Pt == PlotReco_[i].second)
     for(unsigned int j=0;j<PlotReco_[i].first.size();j++){
       if(Eta == (PlotReco_[i].first)[j].second)
          (PlotReco_[i].first)[j].first.FillMC(signal,ChiSquare,Ncomb);
     }
  }

}
  
void PlotRecoScan::Write(TFile* fout){
  for(unsigned int i=0;i<PlotReco_.size();i++){
    for(unsigned int j=0;j<PlotReco_[i].first.size();j++){
      (PlotReco_[i].first)[j].first.Write(fout);
    }
  }
}

PlotReco PlotRecoScan::GetPlotReco(const float& Pt, const float& Eta)const{
  for(unsigned int i=0;i<PlotReco_.size();i++){
    if(Pt == PlotReco_[i].second)
     for(unsigned int j=0;j<PlotReco_[i].first.size();j++){
       if(Eta == (PlotReco_[i]).first[j].second)
          return (PlotReco_[i].first)[j].first;
     }
  }
  cerr<<"Pt:"<<Pt<<" - Eta:"<<Eta<<" couple not found"<<endl; 
  return PlotReco();
}

