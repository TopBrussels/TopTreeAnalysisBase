#include "../interface/LeptonTools.h"


// muonSFweight constructor
MuonSFWeight::MuonSFWeight (const string &sfFile, const string &dataOverMC, const bool &debug, const bool &printWarning)
{
  printWarning_ = printWarning;
  debug_ = debug;
  if (debug_)std::cout << "MuonSFWeight constuctor being called!!" << std::endl << endl;
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
  fin->Close ();
  if (debug_) cout << "file closed" << endl;
}


// muonSFweight at method (get SF for the given eta and pt)
double
MuonSFWeight::at(const double &eta, const double &pt, const int &shiftUpDown)
{
  double pt_hist= pt;
  double eta_hist= eta;

  // some cout to help debug
  if (debug_){
    cout << "in 'at' method " << endl;
    cout << "pt hist is " << pt << endl;
    cout << "eta hist is " << eta << endl;
    for (int bin=0; bin <= muonSFWeight_->GetNbinsX(); bin ++) {
      for (int jbin=0; jbin <= muonSFWeight_->GetNbinsY(); jbin++){
	cout << "x bin is " << bin << ", y bin is " << jbin <<  " and bin content is " << muonSFWeight_->GetBinContent(bin,jbin) << endl;
      }
    }
  }


  // to give a non null SF for muons being out of eta and/or pt range of the input histo
  /*
  if (pt > 300 && abs(eta) < muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()) )
    {
      pt_hist =( muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 1) + muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 2))/2;
      if (pt > 300 && abs(eta) < 0.9)
        {
          pt_hist =( muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins()) + muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 1))/2;
        }
    }
  else if (pt < 300 && abs(eta) > muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()))
    {
      eta_hist =(muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()) + muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetNbins() - 1))/2;
    }
  else if (pt > 300 && abs(eta) > muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()))
    {
      pt_hist =( muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 1) + muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetYaxis()->GetNbins() - 2))/2;
      eta_hist =(muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetLast()) + muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetXaxis()->GetNbins() - 1))/2;
    }
  */
  
  Int_t foundBin = muonSFWeight_->FindBin(abs(eta_hist),pt_hist);
  double SF = muonSFWeight_->GetBinContent(foundBin) + shiftUpDown * muonSFWeight_->GetBinError(foundBin);
    Bool_t printWarning_ = false;
  if (SF == 0. && printWarning_) {
    cout << "WARNING. The value of the SF for the muon is equal to 0. This probably means that your are outside the range of the histogram " << endl;
    cout << "range in eta is " << muonSFWeight_->GetXaxis()->GetBinLowEdge(1) << " ---> " << muonSFWeight_->GetXaxis()->GetBinUpEdge(muonSFWeight_->GetNbinsX()) << endl;
    cout << "range in pt is " << muonSFWeight_->GetYaxis()->GetBinLowEdge(1) << " ---> " << muonSFWeight_->GetYaxis()->GetBinUpEdge(muonSFWeight_->GetNbinsY()) << endl;
  }
  return SF;

}

//muonSFweight destructor
MuonSFWeight::~MuonSFWeight ()
{
  delete muonSFWeight_; // close file
}




// electronSFweight constructor
ElectronSFWeight::ElectronSFWeight (const string &sfFile, const string &dataOverMC, const bool &debug, const bool &printWarning)
{
  printWarning_ = printWarning;
  debug_ = debug;
  if (debug_)std::cout << "ElectronSFWeight constuctor being called!!" << std::endl << endl;
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
  fin->Close ();
  if (debug_) cout << "file closed" << endl;
}

// electronSFweight at method (get SF for the given eta and pt)
double
ElectronSFWeight::at(const double &eta, const double &pt, const int &shiftUpDown)
{
  double pt_hist= pt;
  double eta_hist= eta;

  // some cout to help debug
  if (debug_){
    cout << "in 'at' method " << endl;
    cout << "pt hist is " << pt << endl;
    cout << "eta hist is " << eta << endl;
    for (int bin=1; bin <= electronSFWeight_->GetNbinsX(); bin ++) {
      for (int jbin=1; jbin <= electronSFWeight_->GetNbinsY(); jbin++){
	cout << "x bin is " << bin << ", y bin is " << jbin <<  " and bin content is " << electronSFWeight_->GetBinContent(bin,jbin) << endl;
      }
    }
  }


  // to give a non null SF for electrons being out of eta and/or pt range of the input histo
  /*
  if (pt > 300 && abs(eta) < electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetXaxis()->GetLast()) )
    {
      pt_hist =( electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetYaxis()->GetNbins() - 1) + electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetYaxis()->GetNbins() - 2))/2;
      if (pt > 300 && abs(eta) < 0.9)
        {
          pt_hist =( electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetYaxis()->GetNbins()) + electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetYaxis()->GetNbins() - 1))/2;
        }
    }
  else if (pt < 300 && abs(eta) > electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetXaxis()->GetLast()))
    {
      eta_hist =(electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetXaxis()->GetLast()) + electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetXaxis()->GetNbins() - 1))/2;
    }
  else if (pt > 300 && abs(eta) > electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetXaxis()->GetLast()))
    {
      pt_hist =( electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetYaxis()->GetNbins() - 1) + electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetYaxis()->GetNbins() - 2))/2;
      eta_hist =(electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetXaxis()->GetLast()) + electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetXaxis()->GetNbins() - 1))/2;
    }
  */
  
  Int_t foundBin = electronSFWeight_->FindBin(abs(eta_hist),pt_hist);
  double SF = electronSFWeight_->GetBinContent(foundBin) + shiftUpDown * electronSFWeight_->GetBinError(foundBin);
  
  if (SF == 0. && printWarning_) {
    cout << "WARNING. The value of the SF for the electron is equal to 0. This probably means that your are outside the range of the histogram " << endl;
    cout << "range in eta is " << electronSFWeight_->GetXaxis()->GetBinLowEdge(1) << " ---> " << electronSFWeight_->GetXaxis()->GetBinUpEdge(electronSFWeight_->GetNbinsX()) << endl;
    cout << "range in pt is " << electronSFWeight_->GetYaxis()->GetBinLowEdge(1) << " ---> " << electronSFWeight_->GetYaxis()->GetBinUpEdge(electronSFWeight_->GetNbinsY()) << endl;
  }
  return SF;
}


// electronSFweight destructor
ElectronSFWeight::~ElectronSFWeight ()
{
  delete electronSFWeight_; // close file
}


