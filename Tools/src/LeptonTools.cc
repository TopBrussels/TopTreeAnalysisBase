#include "../interface/LeptonTools.h"


// muonSFweight constructor
MuonSFWeight::MuonSFWeight (const string &sfFile, const string &dataOverMC, const bool &extendRange, const bool &debug, const bool &printWarning)
{
  printWarning_ = printWarning;
  debug_ = debug;
  extendRange_ = extendRange;
  if (debug_) {
    cout << endl;
    cout << "--------------------------------------" << endl;
    cout << "MuonSFWeight constuctor being called!!" << endl << endl;
  }
  TFile *fin = TFile::Open (sfFile.c_str ());
  if (debug_) cout << "file is " << sfFile.c_str () << endl;
  TH2F* SF_Combined_TOT = (TH2F *) fin->Get(dataOverMC.c_str ());
  if(SF_Combined_TOT == 0 )
    cerr << "error in  MuonSFWeight::MuonSFWeight: Unknown histogram " << dataOverMC << ", not found in file " <<  sfFile << endl;
  if (debug_) cout << "name of histo is " << SF_Combined_TOT->GetName() << endl;
  muonSFWeight_ = (TH2F*)SF_Combined_TOT->Clone("muonSFWeight_");
  muonSFWeight_->SetDirectory(0);
  if (debug_){
    cout << "number of bins: " << muonSFWeight_->GetNbinsX() << ", " << muonSFWeight_->GetNbinsY() << endl;
    cout << "name of cloned histo is " << SF_Combined_TOT->GetName() << endl;
    cout << "Mean of histo is " << muonSFWeight_->GetMean() << endl;
  }
  if (debug_){
    cout << "printing the bin content of the loaded histogram!!" << endl;
    for (int bin=1; bin <= muonSFWeight_->GetNbinsX(); bin ++) {
      for (int jbin=1; jbin <= muonSFWeight_->GetNbinsY(); jbin++){
	cout << "x bin is " << bin << ", y bin is " << jbin <<  " and bin content is " << muonSFWeight_->GetBinContent(bin,jbin) << endl ;
      }
    }
    cout << endl << "End of MuonSFWeight constuctor" << endl ;
    cout << "------------------------------" << endl << endl;
  }
  fin->Close ();
  //  if (extendRange_) cout << "extendRange_ is true" << endl;
}


// muonSFweight at method (get SF for the given eta and pt)
double
MuonSFWeight::at(const double &eta, const double &pt, const int &shiftUpDown)
{
  // insert few useful variables:
  double Xmin = muonSFWeight_->GetXaxis()->GetBinLowEdge(1) ;
  double Xmax = muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetNbinsX());
  double Ymin = muonSFWeight_->GetYaxis()->GetBinLowEdge(1);
  double Ymax = muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetNbinsY()) ;


  // some cout to help debug
  if (debug_){
    cout << "in 'at' method " << endl;
    cout << "pt is " << pt << endl;
    cout << "eta is " << eta << endl;
  }

  double pt_hist= pt;
  double eta_hist= eta;
  bool isOutOfRange = false;
 // double uncertaintyMultiplier = 1;
  // to give a non null SF for muons being out of eta and/or pt range of the input histo
  if (extendRange_){
    if ( pt < Ymin ){
      pt_hist = muonSFWeight_->GetYaxis()->GetBinCenter(1);
     // uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
    if ( Ymax < pt){
      pt_hist = muonSFWeight_->GetYaxis()->GetBinCenter(muonSFWeight_->GetNbinsY());
     // uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
    if (abs(eta) < Xmin){
      eta_hist = muonSFWeight_->GetXaxis()->GetBinCenter(1);
    //  uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
    if (Xmax < abs(eta)){
      eta_hist=muonSFWeight_->GetXaxis()->GetBinCenter(muonSFWeight_->GetNbinsX());
   //   uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
  }
  
 
  Int_t foundBin = muonSFWeight_->FindBin(abs(eta_hist),pt_hist);
  double SF = muonSFWeight_->GetBinContent(foundBin) + shiftUpDown *  muonSFWeight_->GetBinError(foundBin);

  if (isOutOfRange && printWarning_) {
    cout << "The value of pt/and or eta for which you want a SF is outside the range of the histogram you have laoded. The SF of the closest bin will be used and the uncertainty is doubled!" << endl;
    cout << "The pt is " << pt << " and the eta is " << eta << endl;
    cout << "range in eta is " << Xmin << " ---> " << Xmax << endl;
    cout << "range in pt is " << Ymin << " ---> " << Ymax << endl;
    //    cout << "uncertaintyMultiplier is " << uncertaintyMultiplier << endl;
  }

  if (SF==0. && printWarning_) {
    cout << "The SF is equal to 0 because the value of pt/and or eta for which you want a SF is outside the range of the histogram you have laoded. You might want to set 'extendRange_' to true if you want to get the value of the closest bin." << endl;
    cout << "The pt is " << pt << " and the eta is " << eta << endl;
    cout << "range in eta is " << Xmin << " ---> " << Xmax << endl;
    cout << "range in pt is " << Ymin << " ---> " << Ymax << endl;
  }
  return SF;

}

//muonSFweight destructor
MuonSFWeight::~MuonSFWeight ()
{
  delete muonSFWeight_; // close file
}


#include "../interface/LeptonTools.h"


// electronSFweight constructor
ElectronSFWeight::ElectronSFWeight (const string &sfFile, const string &dataOverMC, const bool &extendRange, const bool &debug, const bool &printWarning, const bool &recoSF)
{
  printWarning_ = printWarning;
  recoSF_ = recoSF;
  debug_ = debug;
  extendRange_ = extendRange;
  if (debug_) {
    cout << endl;
    cout << "--------------------------------------" << endl;
    cout << "ElectronSFWeight constuctor being called!!" << endl << endl;
  }
  TFile *fin = TFile::Open (sfFile.c_str ());
  if (debug_) cout << "file is " << sfFile.c_str () << endl;
  TH2F* SF_Combined_TOT = (TH2F *) fin->Get(dataOverMC.c_str ());
  if(SF_Combined_TOT == 0 )
    cerr << "error in  ElectronSFWeight::ElectronSFWeight: Unknown histogram " << dataOverMC << ", not found in file " <<  sfFile << endl;
  if (debug_) cout << "name of histo is " << SF_Combined_TOT->GetName() << endl;
  electronSFWeight_ = (TH2F*)SF_Combined_TOT->Clone("electronSFWeight_");
  electronSFWeight_->SetDirectory(0);
  if (debug_){
    cout << "number of bins: " << electronSFWeight_->GetNbinsX() << ", " << electronSFWeight_->GetNbinsY() << endl;
    cout << "name of cloned histo is " << SF_Combined_TOT->GetName() << endl;
    cout << "Mean of histo is " << electronSFWeight_->GetMean() << endl;
  }
  if (debug_){
    cout << "printing the bin content of the loaded histogram!!" << endl;
    for (int bin=1; bin <= electronSFWeight_->GetNbinsX(); bin ++) {
      for (int jbin=1; jbin <= electronSFWeight_->GetNbinsY(); jbin++){
	cout << "x bin is " << bin << ", y bin is " << jbin <<  " and bin content is " << electronSFWeight_->GetBinContent(bin,jbin) << endl ;
      }
    }
    cout << endl << "End of ElectronSFWeight constuctor" << endl ;
    cout << "------------------------------" << endl << endl;
  }
  fin->Close ();
  //  if (extendRange_) cout << "extendRange_ is true" << endl;
}


// electronSFweight at method (get SF for the given eta and pt)
double
ElectronSFWeight::at(const double &eta, const double &pt, const int &shiftUpDown)
{
  // insert few useful variables:
  double Xmin = electronSFWeight_->GetXaxis()->GetBinLowEdge(1) ;
  double Xmax = electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetNbinsX());
  double Ymin = electronSFWeight_->GetYaxis()->GetBinLowEdge(1);
  //double Ymax = electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetNbinsY()) ;
  double Ymax = 150.;  // see https://twiki.cern.ch/twiki/bin/view/CMS/EgammaIDRecipesRun2#Efficiencies_and_scale_factors

  // some cout to help debug
  if (debug_){
    cout << "in 'at' method " << endl;
    cout << "pt is " << pt << endl;
    cout << "eta is " << eta << endl;
  }

  double pt_hist= pt;
  double eta_hist= eta;
  bool isOutOfRange = false;
  //double uncertaintyMultiplier = 1;
  // to give a non null SF for electrons being out of eta and/or pt range of the input histo
  if (extendRange_){
    // out of pt range
    if ( pt < Ymin ){
      pt_hist = electronSFWeight_->GetYaxis()->GetBinCenter(1);
      //uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
    else  if ( Ymax < pt){
     if(!recoSF_) pt_hist = electronSFWeight_->GetYaxis()->GetBinCenter(electronSFWeight_->GetNbinsY()-1);
     else  pt_hist = electronSFWeight_->GetYaxis()->GetBinCenter(electronSFWeight_->GetNbinsY());
     // uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
    //out of eta range
    if (eta < Xmin){
      eta_hist = electronSFWeight_->GetXaxis()->GetBinCenter(1);
     // uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
    else if (Xmax < eta){
      eta_hist = electronSFWeight_->GetXaxis()->GetBinCenter(electronSFWeight_->GetNbinsX());
     // uncertaintyMultiplier = 2;
      isOutOfRange = true;
    }
  }
  
  
  
  
  Int_t foundBin = electronSFWeight_->FindBin(eta_hist,pt_hist);
  
  double SF = electronSFWeight_->GetBinContent(foundBin) ;
  double totalunc = 0;
  totalunc = electronSFWeight_->GetBinError(foundBin) ;
  if((pt < 20. || pt > 80.) && recoSF_ ){
    totalunc = TMath::Sqrt((electronSFWeight_->GetBinError(foundBin)*electronSFWeight_->GetBinError(foundBin)) + (SF*0.01*SF*0.01));
  }
  SF += shiftUpDown*totalunc;
  
  
  if(debug_ && recoSF_) cout << "extra unc of 1% for pt for RecoSF" << endl;
  if(debug_ && shiftUpDown < 0) cout << "SF down " << endl;
  else if(debug_ && shiftUpDown > 0) cout << "SF up" << endl;
  else if(debug_ && shiftUpDown == 0) cout << "SF nom" << endl;
  if(debug_) cout << "The total uncertainty is " << totalunc << " for as SF of " << SF << endl;
  
  if (isOutOfRange && printWarning_) {
    cout << "The value of pt/and or eta for which you want a SF is outside the range of the histogram you have laoded. The SF of the closest bin will be used and the uncertainty is doubled!" << endl;
    cout << "The pt is " << pt << " and the eta is " << eta << endl;
    cout << "range in eta is " << Xmin << " ---> " << Xmax << endl;
    cout << "range in pt is " << Ymin << " ---> " << Ymax << endl;
    //    cout << "uncertaintyMultiplier is " << uncertaintyMultiplier << endl;
  }

  if (SF==0. && printWarning_) {
    cout << "The SF is equal to 0 because the value of pt/and or eta for which you want a SF is outside the range of the histogram you have laoded. You might want to set 'extendRange_' to true if you want to get the value of the closest bin." << endl;
    cout << "The pt is " << pt << " and the eta is " << eta << endl;
    cout << "range in eta is " << Xmin << " ---> " << Xmax << endl;
    cout << "range in pt is " << Ymin << " ---> " << Ymax << endl;
  }
  return SF;

}

//electronSFweight destructor
ElectronSFWeight::~ElectronSFWeight ()
{
  delete electronSFWeight_; // close file
}

