#ifndef _Binner_h_
#define _Binner_h_

#include "TProfile.h"
#include "TAxis.h"
#include <vector>
#include <map>
#include "TopTreeAnalysis/Reconstruction/interface/Observables.h"

/**
 *   Binner.h
 *   
 *
 *  Created by Thierry Caebergs () on 25/01/10.
 * 
 *  Restrictions imposed by the use of Observables : only one information for the binner by event.
 */

#define NBINS_FINE_TPROFILE 400

class Binner
{
public:

  Binner(){;};
  
  
  /**
   Binner for variables.
   It can manage all the variables at the time or only one (see the [Fill] methods)
   */
  
  /**
   * [var] has to comply to the following call : var(refPart) gives the value of the bin to be associated to
   * and var() give the value to fill in the bin obtained previously.
   * Test for no [ref] available/needed for is done on the value of [ref]. If it is NULL, then [var] will be called the same way.
   * All particles present in the TTree are passed to the function. To sort among them, you have to implement it in the [var] function
   *  itself.
   */
  
  /*
   WHAT TO COPY FROM A MAP TO ANOTHER ONE.
   */
  Binner(double intLumi);

  Binner(double intLumi, map<string,TAxis*> firstGuess);

  
  /**
   Copy constructor.
   Also copies all the TProfile's in the way that they will be independent.
   */
  Binner(const Binner& rhs);

void Check();
  
  /**
   Delete the Binner and, more important, all the associated TProfile's
   */
  ~Binner();

  
  const Binner& operator=(const Binner& rhs);
  
  
  TProfile* getProfileCopy(string varname);
  

  TProfile* getProfile(string varname);

  
  vector<TProfile*>* getProfiles();

  
  /**
   Fills all the variables 
   */
  void Fill(Observables& s);

  
/**
 Fills for only one variable in the collection of observables.
 */
  void Fill(Observables& s, string variable);
  

  /**
     To compute for several binners (different datasets), for all the variables.
     Not implemented yet
   */
  vector<TAxis*>* getBins(vector<Binner> vBin, double intLumiEstimated, int minEntries, double numSigma=1.);

  
  /**
    Get the binning for several binners (different datasets), and for a given variable.
   Not implemented yet
  */
  TAxis* getBins(vector<Binner> vBin, string variable, double intLumiEstimated, int minEntries, double numSigma=1.);

  /**
   Returns the binning for a given variable
   */
  TAxis* getBins(string variable, double intLumiEstimated, int minEntries=50, double numSigma=1.);
  
  /**
   Returns the binning for all the variables from the current dataset
   */
  vector<TAxis*>* getBins(double intLumiEstimated, int minEntries=50, double numSigma=1.);

  /**
   Returns the list of binned variables
   */
  vector<string> listOfVariables();

  
private:
  Double_t intLumi;
  map<string,TProfile*> tpf;
  
  /**
   PRIVATE USE
   Returns the binning for a given estimated integrated luminosity (at least ~50 entries by bin, 50 can be modified by setting [minEntries]).
   You can also provide the number of sigma's to take around the value (by setting [numSigma]).
   
   Check for this->profBins->GetBinError(...) because it can return error on the mean instead of sigma
   */
  TAxis* getBins(TProfile *profile, double intLumiEstimated, int minEntries=50, double numSigma=1.);

};

#endif
