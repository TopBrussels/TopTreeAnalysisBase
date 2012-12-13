#include "../interface/MCWeighter.h"

MCWeighter::MCWeighter ()
{
}

MCWeighter::~MCWeighter ()
{
}

void 
MCWeighter::Clear ()
{
 variables_.clear();
 hMC_.clear();
 hData_.clear();
 hWeight_.clear();
 fMC_.clear();
 fData_.clear();
 DataMCCanvas_.clear();
}

void
MCWeighter::LoadFile (TFile * fin, int verbosity)
{
  if (!fin) {
    cout << "Cannot load the file: pointer is null" << endl;
    return;
  }
  if (!fin->cd ("ReweightInfos")) {
    cout << "The file doesn't not contain a directory 'ReweightInfos'" << endl;
    return;
  }
  TKey *key = 0;
  TIter nextkey (((TDirectory *) fin->Get ("ReweightPlots"))->GetListOfKeys ());
  vector < string > histovarname;
  vector < string > funcvarname;
  while ( ( key = (TKey *) nextkey() ) ) {
    // read object from first source file
    TObject *obj = key->ReadObj ();
    if (obj->IsA ()->InheritsFrom ("TH1F")) {
      TH1F *h = (TH1F *) obj;
      string hname (h->GetName ());
      if (hname.find ("MCHisto_") == 0)
	histovarname.push_back (hname.substr (8, hname.size ()));
      delete h;
    }
    if (obj->IsA ()->InheritsFrom ("TF1")) {
      TF1 *f = (TF1 *) obj;
      string hname (f->GetName ());
      if (hname.find ("MCFunc_") == 0)
	histovarname.push_back (hname.substr (7, hname.size ()));
      delete f;
    }
  }
  for (unsigned int i = 0; i < histovarname.size (); i++) {
    string hname = "ReweightInfos/MCHisto_" + histovarname[i];
    TH1F *hMC = (TH1F *) fin->Get (hname.c_str ());
    hname = "ReweightInfos/DataHisto_" + histovarname[i];
    TH1F *hData = (TH1F *) fin->Get (hname.c_str ());
    if (hMC && hData) {
      hMC_.push_back ((TH1F *) hMC->Clone ());
      hData_.push_back ((TH1F *) hData->Clone ());
      variables_.push_back (histovarname[i]);
      fMC_.push_back (0);
      fData_.push_back (0);
      TH1F *hWeight = (TH1F *) hData->Clone ();
      hWeight->Divide (hMC);
      unsigned int nbins = hWeight->GetNbinsX ();
      for (unsigned int b = 1; b <= nbins; b++)
	if (hWeight->GetBinContent (b) == 0)
	  hWeight->SetBinContent (b, 1);
      hWeight_.push_back ((TH1F *) hWeight->Clone ());
      delete hWeight;
      if (verbosity > 1)
	cout << "The histograms for " << histovarname[i] << " are loaded" << endl;

    }
    else if (verbosity > 1)
      cout << "The histograms for " << histovarname[i] << " cannot be loaded" << endl;
    delete hMC;
    delete hData;
  }

  for (unsigned int i = 0; i < funcvarname.size (); i++) {
    string hname = "ReweightInfos/MCFunc_" + funcvarname[i];
    TF1 *fMC = (TF1 *) fin->Get (hname.c_str ());
    hname = "ReweightInfos/DataFunc_" + funcvarname[i];
    TF1 *fData = (TF1 *) fin->Get (hname.c_str ());
    if (fMC && fData) {
      fMC_.push_back ((TF1 *) fMC->Clone ());
      fData_.push_back ((TF1 *) fData->Clone ());
      variables_.push_back (histovarname[i]);
      hMC_.push_back (0);
      hData_.push_back (0);
      hWeight_.push_back (0);
      if (verbosity > 1)
	cout << "The functions for " << funcvarname[i] << " are loaded" << endl;
    }
    else if (verbosity > 1)
      cout << "The functions for " << funcvarname[i] << " cannot be loaded" << endl;
    delete fMC;
    delete fData;
  }

  delete key;
}

void
MCWeighter::ExportFile (TFile * fout, vector < string > histovarname, vector < TH1F * >MCHistos, vector < TH1F * >DataHistos, vector < string > funcvarname, vector < TF1 * >MCFuncs, vector < TF1 * >DataFuncs, int verbosity)
{
  fout->mkdir ("ReweightInfos");
  fout->cd ("ReweightInfos");
  //check the size of the vector
  if (histovarname.size () == MCHistos.size () && MCHistos.size () == DataHistos.size ()) {
    for (unsigned int i = 0; i < MCHistos.size (); i++) {
      string hname = "MCHisto_" + histovarname[i];
      MCHistos[i]->SetName (hname.c_str ());
      MCHistos[i]->Write ();
      hname = "DataHisto_" + histovarname[i];
      DataHistos[i]->SetName (hname.c_str ());
      DataHistos[i]->Write ();
      if (verbosity > 1)
	cout << "The functions for " << funcvarname[i] << " are exported" << endl;
    }
  }
  else {
    cout << "Cannot export the file: the size of the vector are not the same." << endl;
    return;
  }
  if (funcvarname.size () == MCFuncs.size () && MCFuncs.size () == DataFuncs.size ()) {
    for (unsigned int i = 0; i < MCFuncs.size (); i++) {
      string hname = "MCFunc_" + funcvarname[i];
      MCFuncs[i]->SetName (hname.c_str ());
      MCFuncs[i]->Write ();
      hname = "DataFunc_" + funcvarname[i];
      DataFuncs[i]->SetName (hname.c_str ());
      DataFuncs[i]->Write ();
    }
  }
  else {
    cout << "Cannot export the file: the size of the vector are not the same." << endl;
    return;
  }
}

float
MCWeighter::GetWeight (unsigned int index, float value)
{
  float weight = 9999;
  if (index < variables_.size ()) {
    if (hMC_[index] != 0) {
      unsigned int nbins = hWeight_[index]->GetNbinsX ();
      for (unsigned int i = 0; i <= nbins + 1; i++) {
	if (i == 0 && value < hWeight_[index]->GetBinLowEdge (1))
	  weight = hWeight_[index]->GetBinContent (i);
	else if (i == nbins + 1 && value >= hWeight_[index]->GetBinLowEdge (i))
 	  weight = hWeight_[index]->GetBinContent (i);
	else if (value >= hWeight_[index]->GetBinLowEdge (i) && value < hWeight_[index]->GetBinLowEdge (i + 1))
 	  weight = hWeight_[index]->GetBinContent (i);
      }
    }
    else if (fMC_[index] != 0) {
      weight = fData_[index]->Eval (value) / fMC_[index]->Eval (value);
    }
    else
      cerr << "MCWeighter: hMC_ and fMC_ are both Null pointers!" << endl;
  }

  return weight;
}

float
MCWeighter::GetWeight (string variable, float value)
{
  unsigned int index = GetIndex (variable);
  float weight = GetWeight (index, value);
  return weight;
}

float
MCWeighter::GetWeight (vector < float >values)
{
  float weight = 1;
  for (unsigned int i = 0; i < values.size (); i++) {
    float tempWeight = GetWeight (i, values[i]);;	//read in weight 
    weight *= tempWeight;
  }

  return weight;
}

void
MCWeighter::LoadVariable (string name, bool histoWeights, TH1F * hMC, TH1F * hData, TF1 * fMC, TF1 * fData)
{
  variables_.push_back (name);
  if (histoWeights) {
    hMC_.push_back ((TH1F *) hMC->Clone ());
    fMC_.push_back (0);
    hData_.push_back ((TH1F *) hData->Clone ());
    fData_.push_back (0);

    TH1F *hWeight = (TH1F *) hData->Clone ();
    hWeight->Divide (hMC);
    unsigned int nbins = hWeight->GetNbinsX ();
    for (unsigned int i = 1; i <= nbins; i++)
      if (hWeight->GetBinContent (i) == 0)
	hWeight->SetBinContent (i, 1);

    hWeight_.push_back ((TH1F *) hWeight);
  }
  else {
    fMC_.push_back ((TF1 *) fMC->Clone ());
    hMC_.push_back (0);
    fData_.push_back ((TF1 *) fData->Clone ());
    hData_.push_back (0);
  }
}

vector < string > MCWeighter::GetListofVariables ()
{
  return variables_;
}

string MCWeighter::GetVarName (unsigned int index)
{
  if (index < variables_.size ())
    return variables_[index];
  else
    return "";
}

unsigned int
MCWeighter::GetIndex (string variable)
{
  int index = 9999;
  for (unsigned int i = 0; i < variables_.size (); i++)
    if (variables_[i] == variable)
      index = i;

  return index;
}

TH1F *
MCWeighter::GetHistoData (unsigned int index)
{
  if (index < hData_.size ())
    return hData_[index];
  else
    return 0;
}

TH1F *
MCWeighter::GetHistoData (string variable)
{
  unsigned int index = GetIndex (variable);
  if (index < hData_.size ())
    return hData_[index];
  else
    return 0;
}

TH1F *
MCWeighter::GetHistoMC (unsigned int index)
{
  if (index < hMC_.size ())
    return hMC_[index];
  else
    return 0;
}

TH1F *
MCWeighter::GetHistoMC (string variable)
{
  unsigned int index = GetIndex (variable);
  if (index < hMC_.size ())
    return hMC_[index];
  else
    return 0;
}

TH1F *
MCWeighter::GetHistoWeight (unsigned int index)
{
  if (index < hWeight_.size ())
    return hWeight_[index];
  else
    return 0;
}

TH1F *
MCWeighter::GetHistoWeight (string variable)
{
  unsigned int index = GetIndex (variable);
  if (index < hWeight_.size ())
    return hWeight_[index];
  else
    return 0;
}

TF1 *
MCWeighter::GetFuncData (unsigned int index)
{
  if (index < fData_.size ())
    return fData_[index];
  else
    return 0;
}

TF1 *
MCWeighter::GetFuncData (string variable)
{
  unsigned int index = GetIndex (variable);
  if (index < fData_.size ())
    return fData_[index];
  else
    return 0;
}

TF1 *
MCWeighter::GetFuncMC (unsigned int index)
{
  if (index < fMC_.size ())
    return fMC_[index];
  else
    return 0;
}

TF1 *
MCWeighter::GetFuncMC (string variable)
{
  unsigned int index = GetIndex (variable);
  if (index < fMC_.size ())
    return fMC_[index];
  else
    return 0;
}

void
MCWeighter::Draw ()
{
//      for(unsigned int i=0; i<variables_.size(); i++)
//      {
//              TCanvas* canvas = new TCanvas();
//      }
}

void
MCWeighter::Write (TFile * fout)
{
  fout->cd ();
  TDirectory *mainDir = fout->mkdir ("MCWeighter");

  TDirectory *hMCDir = mainDir->mkdir ("hMC");
  hMCDir->cd ();
  for (unsigned int i = 0; i < hMC_.size (); i++)
    if(hMC_[i]) hMC_[i]->Write ();

  TDirectory *hDataDir = mainDir->mkdir ("hData");
  hDataDir->cd ();
  for (unsigned int i = 0; i < hData_.size (); i++)
    if(hData_[i]) hData_[i]->Write ();

  TDirectory *hWeightDir = mainDir->mkdir ("hWeight");
  hWeightDir->cd ();
  for (unsigned int i = 0; i < hWeight_.size (); i++)
    if(hWeight_[i]) hWeight_[i]->Write ();

  TDirectory *fMCDir = mainDir->mkdir ("fMC");
  fMCDir->cd ();
  for (unsigned int i = 0; i < fMC_.size (); i++)
    if(fMC_[i]) fMC_[i]->Write ();

  TDirectory *fDataDir = mainDir->mkdir ("fData");
  fDataDir->cd ();
  for (unsigned int i = 0; i < fData_.size (); i++)
    if(fData_[i]) fData_[i]->Write ();
}
