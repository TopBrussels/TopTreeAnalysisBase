#ifndef SelectionTable_h
#define SelectionTable_h
#include "Selection.h"
#include "TopTreeAnalysis/Tools/interface/Efficiency.h"
#include "TopTreeAnalysis/Content/interface/Dataset.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "TH1F.h"
#include "TFile.h"

using namespace std;

	/**
	//  Aim : producing latex-formatted tables containing the selection efficiencies (and associated errors) 
	//	  and the numbers of events passing a list of pre-defined selection cuts
	//
	//  How to use :
	*/

class SelectionTable {

  
  public:
  	SelectionTable(); /** Default constructor */
  	SelectionTable(vector<string> listOfCuts, vector<Dataset*> listOfDatasets);
  	SelectionTable(const SelectionTable& psel); /** Copy constructor */
  	~SelectionTable(); /** Default destructor */

	void SetLuminosity(float Lumi) {lumi_ = Lumi;};
	void SetPrecision(int Precision) {precision_ = Precision;} /** To specify exactly how many digits to display after the decimal point */

	// Accessors 
	float Luminosity() const {return lumi_;};
	int Precision() const {return precision_;}
	vector<string> ListOfCuts() const {return listOfCuts_;};
	vector<Dataset*> ListOfDatasets() const {return listOfDatasets_;};

	// Statistical errors
	float BinomialeError(float p, float n) const {return (sqrt((p*(1-p)/n)*p));}; /** Calculate the binomial error on the efficiency p */
	float ErrorCalculator( float number, float p, float factor) const { return (sqrt(number*(1-p))*factor); }; /** Calculate the error on the number of selected events, rescaled by factor*/
	float WilsonScoreIntervalLow(float Non, float Ntot); /** Calculate the low limit of the W-S interval*/
	float WilsonScoreIntervalHigh(float Non, float Ntot);/** Calculate the high limit of the W-S interval*/
	float FactorCalculator(float nofevents, float xsection) const { return(lumi_*xsection/nofevents);}; /** Calculate the ratio of the expected number of events to the actual number of events */


  void Clear(){;}; /** Clear the tables */
  void Fill(unsigned int DatasetNumber, vector<float> PassTheCuts); /** Method to fill the tables */
  void Fill(unsigned int DatasetNumber, unsigned int CutNumber, float value); /** Method to fill the tables */
  void TableCalculator(bool mergeTT = false, bool mergeQCD = false, bool mergeW = false, bool mergeZ = false, bool mergeST = false, bool mergeVV = false, bool mergeTTV = false, bool NP_mass = false); /** Calculate the different selection efficiencies and rescaled number of events */
  void Scale(float Lumi);
  void WriteTable(ofstream& fout, float** listTable_, bool writeMerged = true, bool useBookTabs = false, bool writeLandscape = false);
  /**
	Method to display latex-formatted tables
	Options :
	* writeMerged ; if true, display individual contributions for split samples (ex ; st+jets s-ch, t-ch, tW-ch, ...). Set to true by default. 
	* useBookTabs ; if true, use command from the latex package booktabs for table lines. Set to false by default.
  */
  void WriteTable(ofstream& fout, float** listTable_, float** listTableError_, bool writeMerged = true, bool useBookTabs = false, bool writeLandscape = false);
  /**
	Method to display latex-formatted tables into an external file. Display both numbers and their associated errors (binomial approx.)
	Options :
	* writeMerged ; if true, display individual contributions for split samples (ex ; st+jets s-ch, t-ch, tW-ch, ...). Set to true by default. 
	* useBookTabs ; use command from the latex package booktabs for table lines. Set to false by default.
	* writeLandscape ; display tables in landscape mode. Set to false by default.
  */
  void WriteTable(ofstream& fout, float** listTable_, float** listTableErrorHigh_, float** listTableErrorLow_, bool writeMerged = true, bool useBookTabs = false, bool writeLandscape = false);
  /**
	Method to display latex-formatted tables into an external file. Display both numbers and their associated errors (correct error calculation)
	Options :
	* writeMerged ; if true, display individual contributions for split samples (ex ; st+jets s-ch, t-ch, tW-ch, ...). Set to true by default. 
	* useBookTabs ; if true, use command from the latex package booktabs for table lines. Set to false by default.
	* writeLandscape ; if true, display tables in landscape mode. Set to false by default.
  */
  void Write(string filename, bool withError = false, bool writeMerged = true, bool useBookTabs = false, bool addRawNumbers = false, bool addEfficiencies = false, bool addTotalEfficiencies = false, bool writeLandscape = false); 
  /** 
	Write latex-formatted tables into an external file
	Options :
	* addRawNumbers ; if true, add a table containing the raw (unrescaled) numbers of selected events. Set to false by default.
	* addEfficiencies ; if true, add a table containing the individual selection efficiencies. Set to false by default.
	* addTotalEfficiencies ; if true, add a table with the cumulative selection efficiencies after each selection cuts. Set to false by default.
  */
  void Write(ofstream& fout, bool WithError = false, bool writeMerged = true, bool useBookTabs = false, bool addRawNumbers = false, bool addEfficiencies = false, bool addTotalEfficiencies = false, bool writeLandscape = false);  /** Send latex-formatted tables to the specified output stream */

 private:

  float lumi_; // in 1/pb
  int   precision_; // nb of digits to display after the decimal point
  vector<string>   listOfCuts_;
  vector<Dataset*> listOfDatasets_;
  //first dimension: cuts
  //second dimension: datasets
  float**  nofEventsRaw_;	//from Fill method
  float**  nofEventsRawError_;
  float**  nofEventsRawErrorHigh_;
  float**  nofEventsRawErrorLow_;
  float**  nofEvents_;	        //rescaled
  float**  nofEventsError_;
  float**  nofEventsExpErrorHigh_;
  float**  nofEventsExpErrorLow_;
  float**  nofEventsMcErrorHigh_;
  float**  nofEventsMcErrorLow_;
  float**  cutEfficiency_;
  float**  cutEfficiencyErrorHigh_;
  float**  cutEfficiencyErrorLow_;
  float**  cutEfficiencyError_;
  float**  totalCutEfficiency_;
  float**  totalCutEfficiencyErrorHigh_;
  float**  totalCutEfficiencyErrorLow_;
  float**  totalCutEfficiencyError_;
  
  //Merged stuff
  vector<Dataset*> listOfDatasetsMerged_;
  float**  nofEventsRawMerged_;
  float**  nofEventsRawErrorMerged_;
  float**  nofEventsRawErrorHighMerged_;
  float**  nofEventsRawErrorLowMerged_;
  float**  nofEventsMerged_;
  float**  nofEventsErrorMerged_;
  float**  nofEventsExpErrorHighMerged_;
  float**  nofEventsExpErrorLowMerged_;
  float**  nofEventsMcErrorHighMerged_;
  float**  nofEventsMcErrorLowMerged_;
  float**  cutEfficiencyMerged_;
  float**  cutEfficiencyErrorMerged_;
  float**  cutEfficiencyErrorHighMerged_;
  float**  cutEfficiencyErrorLowMerged_;
  float**  totalCutEfficiencyMerged_;
  float**  totalCutEfficiencyErrorMerged_;
  float**  totalCutEfficiencyErrorHighMerged_;
  float**  totalCutEfficiencyErrorLowMerged_;

  bool merge_;
};

#endif
