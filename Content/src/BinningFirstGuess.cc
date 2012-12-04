/*
 *  BinningFirstGuess.cc
 *  Refactoring
 *
 *  Created by Thierry Caebergs on 25/02/10.
 *  Copyright 2010 Université de Mons. All rights reserved.
 *
 */

#include "../interface/BinningFirstGuess.h"

#include "TObjString.h"
#include "TMap.h"

  ClassImp(BinningFirstGuess)

BinningFirstGuess::BinningFirstGuess()
{
  this->intLumi_ = 100.;
  this->isOnMC_ = true;
};

BinningFirstGuess::BinningFirstGuess(Double_t intLumi)
{
  this->intLumi_ = intLumi;
  this->isOnMC_ = true;
};

BinningFirstGuess::BinningFirstGuess(Bool_t isOnMC, Double_t intLumi)
{
  this->intLumi_ = intLumi;
  this->isOnMC_ = isOnMC;
};

BinningFirstGuess::BinningFirstGuess(const BinningFirstGuess& rhs)
{
  this->intLumi_ = rhs.intLumi_;
  this->isOnMC_ = true;
  this->fineHistos_.Clear();
  TIterator* iter = this->fineHistos_.MakeIterator();
  TObjString *key;
  while ( (key=(TObjString*) iter->Next()) )
    this->fineHistos_.Add(key, new TH1D(*((TH1D*)this->fineHistos_.GetValue(key))));
  delete iter;
}

BinningFirstGuess::~BinningFirstGuess()
{
  TIterator* iter = this->fineHistos_.MakeIterator();
  TObjString *key;
  while ( (key=(TObjString*) iter->Next()) )
  {
    delete this->fineHistos_.GetValue(key);
    delete key;
  }
  delete iter;
};

const BinningFirstGuess& BinningFirstGuess::operator=(const BinningFirstGuess& rhs)
{
  if (this == &rhs)
    return *this;
  delete this;
  *this = BinningFirstGuess(rhs);
  return *this;
}

map<string,TAxis*> BinningFirstGuess::getBinningsFirstGuess(Double_t intLumiEstim, Int_t minEntries)
{
  map<string,TAxis*> ve;
  TIterator* iter = this->fineHistos_.MakeIterator();
  TObjString *key;
  while ( (key=(TObjString*) iter->Next()) )
    ve.insert(make_pair(string(key->GetString().Data()),this->getBinningFirstGuess((TH1D*) this->fineHistos_.GetValue(key), intLumiEstim, minEntries)));
  delete iter;
  return ve;
}

TAxis* BinningFirstGuess::getBinningFirstGuess(string varname, Double_t intLumiEstim, Int_t minEntries)
{
  TPair* iter = (TPair*) this->fineHistos_.FindObject(varname.c_str());
  if (iter==NULL)
    return NULL;
  else
    return this->getBinningFirstGuess((TH1D*) iter->Value(), intLumiEstim, minEntries);
}
                                   
void BinningFirstGuess::Fill(const Observables& obs)
{
  vector<pair<string,float> > listVars;
  if (isOnMC_)
    listVars = obs.VariablesMC();
  else
    listVars = obs.Variables();
  for (vector<pair<string,float> >::iterator iter=listVars.begin(); iter!=listVars.end(); iter++)
  {
    TPair* iterHist = (TPair*) this->fineHistos_.FindObject(iter->first.c_str());
    if (iterHist==NULL)
    {
      Int_t index = -1;
      vector<pair<string, float> > lVars = obs.Variables();
      for (Int_t i=0; i<(Int_t) lVars.size(); i++)
      {
        if (!strcmp(lVars[i].first.c_str(), iter->first.c_str())) {
          index = i;
          break;
        }
      }
      if (index==-1)
        this->fineHistos_.Add(new TObjString(iter->first.c_str()), new TH1D((""+iter->first).c_str(), "", NBINS_FINE_TH1, 0., 1000.));
      else
      {
        vector<pair <float, float> > ranges = obs.RangeVariables();
        this->fineHistos_.Add(new TObjString(iter->first.c_str()), new TH1D((""+iter->first).c_str(), "", NBINS_FINE_TH1, ranges[index].first, ranges[index].second));
      }
      iterHist = (TPair*) this->fineHistos_.FindObject(iter->first.c_str());
    }
    ((TH1D*) iterHist->Value())->Fill(iter->second);
  }
}

TAxis* BinningFirstGuess::getBinningFirstGuess(TH1D* h, Double_t intLumiEstim, Int_t minEntries)
{
  Int_t nbins = (Int_t) (h->Integral() * (intLumiEstim/this->intLumi_) / minEntries);
  Double_t xq[nbins+1];
  for (Int_t i=0; i<=nbins; i++)
    xq[i] = ((Double_t) i) / nbins;
  Double_t yq[nbins+1];
  h->GetQuantiles(nbins,yq,xq);
  yq[0] = h->GetBinLowEdge(1);
  yq[nbins] = h->GetBinLowEdge(h->GetNbinsX()+1);
  TAxis *axis = new TAxis(nbins,yq);
  return axis;
}

void BinningFirstGuess::Print()
{
  printf("\nisOnMC_ : %d\n", isOnMC_);
  printf("intLumi_ : %lf\n", intLumi_);
  printf("Map of profiles fineHistos_ : \n");
  map<string,TAxis*> ve;
  TIterator* iter = this->fineHistos_.MakeIterator();
  TObjString *key;
  while ( (key=(TObjString*) iter->Next()) )
    printf(" --> %s : Mean is %lf\n", key->GetString().Data(), ((TH1D*) this->fineHistos_.GetValue(key))->GetMean());
  delete iter;
  printf("\n");
}
