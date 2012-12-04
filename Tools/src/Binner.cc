/*
 *  Binner.cpp
 *  
 *
 *  Created by Thierry Caebergs on 25/01/10.
 *  Copyright 2010 Université de Mons. All rights reserved.
 *
 */

#include "../interface/Binner.h"

//ClassImp(Binner)

Binner::Binner(double intLumi)
{
  this->intLumi = intLumi;
};


Binner::Binner(const Binner& rhs)
{
  for( map<string,TProfile*>::const_iterator iter = rhs.tpf.begin(); iter != rhs.tpf.end(); iter++)
    this->tpf.insert(make_pair(string(iter->first), new TProfile(*(iter->second))));
  this->intLumi = rhs.intLumi;
};

Binner::Binner(double intLumi, map<string,TAxis*> firstGuess)
{
  for (map<string,TAxis*>::iterator iter = firstGuess.begin(); iter!=firstGuess.end(); iter++) {
    TProfile *tprf =  new TProfile(("Binning_"+iter->first).c_str(), ("Binning for "+iter->first).c_str(), iter->second->GetNbins(),
				   iter->second->GetXbins()->fArray);
    tprf->SetDirectory(NULL);
    this->tpf.insert(make_pair(string(iter->first), tprf));
  }
  this->intLumi = intLumi;
};

void Binner::Check()
{
  printf("Binner::Check\n");
  for(map<string,TProfile*>::iterator iter = this->tpf.begin(); iter!=this->tpf.end(); iter++)
    printf("Mean of the histogram : %lf\n", iter->second->GetMean());
  //printf("Name of the histogram : %s\n", /*iter->second->GetName()*/ iter->first.c_str());
}

Binner::~Binner()
{
  for(map<string,TProfile*>::iterator iter = this->tpf.begin(); iter != this->tpf.end(); iter++)
    delete iter->second;
};


const Binner& Binner::operator=(const Binner& rhs)
{
  if (&rhs != this) {
    this->~Binner();
    Binner(rhs);
  }
  return *this;
};


TProfile* Binner::getProfileCopy(string varname)
{ 
  map<string,TProfile*>::iterator iter = this->tpf.find(varname);
  if(iter==tpf.end())
    return NULL;
  return new TProfile(*(iter->second));
};


TProfile* Binner::getProfile(string varname)
{
  map<string,TProfile*>::iterator iter = this->tpf.find(varname);
  if(iter==tpf.end())
    return NULL;
  return iter->second;
};


vector<TProfile*>* Binner::getProfiles()
{
  vector<TProfile*>* vec = new vector<TProfile*>();
  for(map<string,TProfile*>::iterator iter = this->tpf.begin(); iter != this->tpf.end(); iter++)
    vec->push_back(iter->second);
  return vec;
};  


void Binner::Fill(Observables& s)
{
  if (!s.isModeMC())
    return;
  vector<string> listVar = s.ListOfVariablesCoupled();
  vector<pair<string,float> > vars = s.Variables();
  size_t indexRECO, indexMC;
  bool stop = false;
  for (size_t i=0; i<listVar.size(); i++) {
    stop = false;
    indexRECO = 0, indexMC = 0;
    string name = listVar[i];
    while (stop==false && strcmp(name.c_str(), vars[indexRECO].first.c_str()))
    {
      indexRECO++;
      if (indexRECO==vars.size())
      { stop=true; break; }
    }
    if (stop) continue;
    while (stop==false && strcmp(name.c_str(), s.VariablesMC()[indexMC].first.c_str()))
    {
      indexMC++;
      if (indexMC==s.VariablesMC().size())
      { stop=true; break; }
    }
    if (stop) continue;
    float var = vars[indexRECO].second;
    float varMC = s.VariablesMC()[indexMC].second;
  /*  if (!strcmp(name.c_str(),"Aplanarity") || !strcmp(name.c_str(),"Sphericity") ||
        !strcmp(name.c_str(),"Aplanarity_TtbarSyst") || !strcmp(name.c_str(),"Sphericity_TtbarSyst") ||
        !strcmp(name.c_str(),"DeltaPhiMetMuon") || !strcmp(name.c_str(),"HZ4jets"))
      printf("%s\t : RECO = %f\t MC = %f\n", name.c_str(), var, varMC);
 */
    pair<float,float> range = s.RangeVariables()[indexRECO];
    map<string,TProfile*>::iterator iter = this->tpf.find(name);
      //        printf("Binner::Fill -> fill for %s : val[%lu] %f, \t bias[%lu] %f\n", name.c_str(), indexRECO, var, indexMC, varMC-var);
    if(iter==this->tpf.end())
    {
      TProfile *tprf = new TProfile(("Binning_"+name).c_str(), ("Binning for "+name).c_str(), NBINS_FINE_TPROFILE, range.first, range.second);
      tprf->SetDirectory(NULL);
      tprf->Fill(var, varMC-var);
      this->tpf.insert(make_pair(name,tprf));
    }
    else
    {
      if (iter->second==NULL)
      {
        iter->second = new TProfile(("Binning_"+name).c_str(), ("Binning for "+name).c_str(), NBINS_FINE_TPROFILE, range.first, range.second);
        iter->second->SetDirectory(NULL);
      }
      if(iter->second->GetNbinsX()!=0)
        iter->second->Fill(var, varMC-var);
    }
  }
};


void Binner::Fill(Observables& s, string variable)
{
  if (!s.isModeMC())
    return;
  vector<pair<string,float> > vars = s.Variables();
  size_t indexRECO, indexMC;
  indexRECO = 0, indexMC = 0;
  bool stop = false;
  if (vars.size()==0 || s.VariablesMC().size()==0)
    return;
  while (true) {
    string name = variable;
    while (stop==false && strcmp(name.c_str(), vars[indexRECO].first.c_str()))
    {
      indexRECO++;
      if (indexRECO==vars.size())
      { stop=true; break; }
    }
    while (stop==false && strcmp(name.c_str(), s.VariablesMC()[indexMC].first.c_str()))
    {
      indexMC++;
      if (indexMC==s.VariablesMC().size())
      { stop=true; break; }
    }
    if (stop) break;
    //    printf("RECO (%lu - %f) %s \t MC (%lu - %f) %s\n", indexRECO, vars[indexRECO].second, vars[indexRECO].first.c_str(), indexMC, s.VariablesMC()[indexMC].second, s.VariablesMC()[indexMC].first.c_str());
    float var = vars[indexRECO].second;
    float varMC = s.VariablesMC()[indexMC].second;
    pair<float,float> range = s.RangeVariables()[indexRECO];
    map<string,TProfile*>::iterator iter = this->tpf.find(name);
    if(iter==this->tpf.end())
    {
      TProfile *tprf = new TProfile(("Binning_"+name).c_str(), ("Binning for "+name).c_str(), NBINS_FINE_TPROFILE, range.first, range.second);
      tprf->SetDirectory(NULL);
      tprf->Fill(var, varMC-var);
      this->tpf.insert(make_pair(name,tprf));
    }
    else
    {
      if (iter->second==NULL)
      {
        iter->second = new TProfile(("Binning_"+name).c_str(), ("Binning for "+name).c_str(), NBINS_FINE_TPROFILE, range.first, range.second);
        iter->second->SetDirectory(NULL);
      }
      iter->second->Fill(var, varMC-var);
    }
    indexMC++;
    indexRECO++;
    if (vars.size()==indexRECO || s.VariablesMC().size()==indexMC)
      return;
  }
};


vector<TAxis*>* Binner::getBins(vector<Binner> vBin, double intLumiEstimated, int minEntries, double numSigma)
{
  return NULL;
};


TAxis* Binner::getBins(vector<Binner> vBin, string variable, double intLumiEstimated, int minEntries, double numSigma)
{
  return NULL;
};


TAxis* Binner::getBins(string variable, double intLumiEstimated, int minEntries, double numSigma)
{
  map<string,TProfile*>::iterator iter = this->tpf.find(variable);
  if(iter==this->tpf.end())
    return NULL;
  return getBins(iter->second, intLumiEstimated, minEntries, numSigma);
};


vector<TAxis*>* Binner::getBins(double intLumiEstimated, int minEntries, double numSigma)
{
  vector<TAxis*> *v = new vector<TAxis*>();
  for (map<string,TProfile*>::iterator iter = this->tpf.begin(); iter!=this->tpf.end(); iter++) {
    v->push_back(getBins(iter->second, intLumiEstimated, minEntries, numSigma));
  }
  return v;
};

vector<string> Binner::listOfVariables()
{
  vector<string> names;
  for (map<string,TProfile*>::iterator iter=this->tpf.begin(); iter!=this->tpf.end(); iter++)
    names.push_back(iter->first);
  return names;
};






TAxis* Binner::getBins(TProfile *profile, double intLumiEstimated, int minEntries, double numSigma)
{
  /*    if (index >= varname.size())
   return NULL;
   TProfile *profile = this->profBins->at(index);*/
  if (profile==NULL)
    return NULL;
  bool checkForMiddle = true; // check that center of bin + diff +/- numsigma * sigma does not go out of the bin.
  std::vector<double> v;
  if (profile == NULL)
    return NULL;
  if (profile->GetNbinsX() > 0)
  {
    double accStat = 0.;
    bool lastSet = false;
    double xToPush = 0.;
    double lastSaved = profile->GetBinLowEdge(1);
    double foreseenNewToSave = profile->GetBinLowEdge(1)+profile->GetBinContent(1)+numSigma*profile->GetBinError(1);
    int iLastSaved = 0;
    v.push_back(lastSaved);
    for (int32_t i=1; i<=profile->GetNbinsX(); i++)
    {
        // One directional (only forward ; if difference is negative, then ) ---> should be ok now
        // Smearing on not more than the neighbours.
      accStat += profile->GetBinEntries(i);
      
      xToPush = profile->GetBinLowEdge(i);
      
        // To be checked !!!!, should be wrong (ok if *TMath::Sqrt(profile->GetBinEntries(i)) )
      
      if (((profile->GetBinContent(i)<0. &&
            lastSaved<=xToPush+profile->GetBinContent(i)-numSigma*profile->GetBinError(i))
           || (profile->GetBinContent(iLastSaved)>0. &&
               foreseenNewToSave <= xToPush))
          // && (accStat >= minEntries)
          )
      {
        int bin = profile->FindBin((profile->GetBinLowEdge(iLastSaved)+profile->GetBinLowEdge(i)+profile->GetBinWidth(i))/2.);
        double gt0 = 0.;
        double lt0 = 0.;
        if (profile->GetBinContent(bin)-numSigma*profile->GetBinError(bin)<0.)
          lt0 = profile->GetBinContent(bin)-numSigma*profile->GetBinError(bin);
        if (profile->GetBinContent(bin)+numSigma*profile->GetBinError(bin)>0.)
          gt0 = profile->GetBinContent(bin)+numSigma*profile->GetBinError(bin);
        
          // A VERIFIER
        if (checkForMiddle && 
            (accStat/this->intLumi > minEntries/intLumiEstimated) &&
            (profile->GetBinLowEdge(iLastSaved) < profile->GetBinLowEdge(bin)+profile->GetBinWidth(bin)/2 + lt0
             || profile->GetBinLowEdge(bin)+profile->GetBinWidth(bin)/2 + gt0 < profile->GetBinLowEdge(i)+profile->GetBinWidth(i)))
            // To check the middle of the bin allows the diff
        {
          if (i==profile->GetNbinsX())
            lastSet = true;
          v.push_back(xToPush);
          accStat = 0.;
          lastSaved = xToPush;
          iLastSaved = i;
          foreseenNewToSave = xToPush+profile->GetBinContent(i)+2*numSigma*profile->GetBinError(i);
        }
      }
    }
    if (!lastSet)
    {
      int lastIndex = profile->GetNbinsX();
      /*        v.push_back(profile->GetBinLowEdge(lastIndex) + profile->GetBinWidth(lastIndex));*/
        //Code inserted to merge the last computed bin to the upper edge one; to avoid fake last bin
      v[v.size()-1] = profile->GetBinLowEdge(lastIndex) + profile->GetBinWidth(lastIndex);
    }
  }
  Double_t *vvv = (Double_t*) malloc(v.size() * sizeof(Double_t));
  int j=0;
  for (vector<Double_t>::iterator it=v.begin(); it!=v.end(); it++) {
    vvv[j]=(*it);
    /*      printf("vvv[%d]=%f\n", j, *it);*/
    j++;
  }
  TAxis *a = new TAxis(j-1, vvv);
  free(vvv);
  return a;
};
