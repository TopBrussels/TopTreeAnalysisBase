#ifndef _BinningFirstGuess_h_
#define _BinningFirstGuess_h_

#include "TopTreeAnalysis/Reconstruction/interface/Observables.h"
#include "TH1D.h"
#include "TObject.h"
#include "TMap.h"
#include "TBuffer.h"
#include "TClass.h"

/**
 *  Refactoring BinningFirstGuess.h
 *
 *  Created by Thierry Caebergs () on 25/02/10.
 * 
 */

#define NBINS_FINE_TH1 400

class BinningFirstGuess : public TObject
{
public:

  BinningFirstGuess();

  /**
   intLumi is the integrated luminosity corresponding to the whole set of data.
   minRange and maxRange are boundaries of the data.
   */
  BinningFirstGuess(Double_t intLumi);
  
  BinningFirstGuess(Bool_t isOnMC, Double_t intLumi);

  BinningFirstGuess(const BinningFirstGuess& rhs);

  ~BinningFirstGuess();

  const BinningFirstGuess& operator=(const BinningFirstGuess& rhs);

  /**
   Returns the binning estimated of a given variable ([varname]) and for a given integrated luminosity, and the given number of entries by bin.
   */
  TAxis* getBinningFirstGuess(string varname, Double_t intLumiEstim, Int_t minEntries=10);

  /**
   Returns the binnings estimated for a given integrated luminosity, and the given number of entries by bin.
   */
  map<string,TAxis*> getBinningsFirstGuess(Double_t intLumiEstim, Int_t minEntries=10);

  void Fill(const Observables& obs);
  
  void Print();
  
/*  string nameOf(Int_t i); */
  
/* virtual void Streamer(TBuffer& b);

  virtual TClass* IsA()
  { return NULL; };
  static TClass* Class()
  { return NULL; };
*/
     ClassDef(BinningFirstGuess,1);

private:
  Bool_t isOnMC_;
  Double_t intLumi_;
  TMap fineHistos_;
  
  TAxis* getBinningFirstGuess(TH1D* h, Double_t intLumiEstim, Int_t minEntries=10);

};

#endif
