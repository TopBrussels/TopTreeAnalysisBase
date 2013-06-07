#include "../interface/LeptonTools.h"

LeptonTools::LeptonTools(bool verbose)
{
  verbose_ = verbose;
}

LeptonTools::~LeptonTools()
{

}

void LeptonTools::readMuonSF(string IdIsoFile, string trigFileAB, string trigFileC, string trigFileD)
{
  // fractions should be the same for everybody
  float fracA = 806.594 / 19125.189;
  float fracB = 4375. / 19125.189;
  float fracC = ( 494.051 + 6265 ) / 19125.189;
  float fracD = ( 6678 + 506.544 ) / 19125.189;
  
  TFile* inFileID = new TFile(IdIsoFile.c_str(),"READ");
  TGraphAsymmErrors inIdEtaLow = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_Tight_pt_abseta<0.9_2012ABCD")->Clone();
  TGraphAsymmErrors inIdEtaMed = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_Tight_pt_abseta0.9-1.2_2012ABCD")->Clone();
  TGraphAsymmErrors inIdEtaHigh = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_Tight_pt_abseta1.2-2.1_2012ABCD")->Clone();
  TGraphAsymmErrors inIsoEtaLow = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta<0.9_2012ABCD")->Clone();
  TGraphAsymmErrors inIsoEtaMed = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta0.9-1.2_2012ABCD")->Clone();
  TGraphAsymmErrors inIsoEtaHigh = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta1.2-2.1_2012ABCD")->Clone();
  inFileID->Close();
  delete inFileID;
  
  TFile* inFileTrigAB = new TFile(trigFileAB.c_str(),"READ");
  TGraphAsymmErrors inTrigAEtaLow = *(TGraphAsymmErrors*) inFileTrigAB->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta<0.9_2012A")->Clone();
  TGraphAsymmErrors inTrigAEtaMed = *(TGraphAsymmErrors*) inFileTrigAB->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta0.9-1.2_2012A")->Clone();
  TGraphAsymmErrors inTrigAEtaHigh = *(TGraphAsymmErrors*) inFileTrigAB->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta1.2-2.1_2012A")->Clone();
  TGraphAsymmErrors inTrigBEtaLow = *(TGraphAsymmErrors*) inFileTrigAB->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta<0.9_2012B")->Clone();
  TGraphAsymmErrors inTrigBEtaMed = *(TGraphAsymmErrors*) inFileTrigAB->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta0.9-1.2_2012B")->Clone();
  TGraphAsymmErrors inTrigBEtaHigh = *(TGraphAsymmErrors*) inFileTrigAB->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta1.2-2.1_2012B")->Clone();
  inFileTrigAB->Close();
  delete inFileTrigAB;
  
  TFile* inFileTrigC = new TFile(trigFileC.c_str(),"READ");
  TGraphAsymmErrors inTrigCEtaLow = *(TGraphAsymmErrors*) inFileTrigC->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta<0.9")->Clone();
  TGraphAsymmErrors inTrigCEtaMed = *(TGraphAsymmErrors*) inFileTrigC->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta0.9-1.2")->Clone();
  TGraphAsymmErrors inTrigCEtaHigh = *(TGraphAsymmErrors*) inFileTrigC->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta1.2-2.1")->Clone();
  inFileTrigC->Close();
  delete inFileTrigC;
  
  TFile* inFileTrigD = new TFile(trigFileD.c_str(),"READ");
  TGraphAsymmErrors inTrigDEtaLow = *(TGraphAsymmErrors*) inFileTrigD->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta<0.9_2012D")->Clone();
  TGraphAsymmErrors inTrigDEtaMed = *(TGraphAsymmErrors*) inFileTrigD->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta0.9-1.2_2012D")->Clone();
  TGraphAsymmErrors inTrigDEtaHigh = *(TGraphAsymmErrors*) inFileTrigD->Get("DATA_over_MC_IsoMu24_eta2p1_TightIso_pt_abseta1.2-2.1_2012D")->Clone();
  inFileTrigD->Close();
  delete inFileTrigD;
  
  double* xBinTmp = inTrigDEtaLow.GetX();
  int N = inTrigDEtaLow.GetN();
  
  if(verbose_) cout << "Muon SF!!" << endl;
  
  for(int i=0; i<N; i++)
  {
    double low = xBinTmp[i]-inTrigDEtaLow.GetErrorXlow(i);
    double high = xBinTmp[i]+inTrigDEtaLow.GetErrorXhigh(i);
    muonPt.push_back( (low+high) / 2. );
    muonPtErr.push_back( (high-low) / 2. );
    
    if(verbose_) cout << "bin: " << i << " |  " << low << " <---> " << high << " | " << muonPt[i] << " +- " << muonPtErr[i] << endl;
    
    double trigSFEtaLow = fracA*getSF(&inTrigAEtaLow, low, high) + fracB*getSF(&inTrigBEtaLow, low, high) + fracC*getSF(&inTrigCEtaLow, low, high) + fracD*getSF(&inTrigDEtaLow, low, high) ;
    double trigSFEtaLowUncPlus = sqrt( pow(fracA*inTrigAEtaLow.GetErrorYhigh(i), 2) + pow(fracB*inTrigBEtaLow.GetErrorYhigh(i), 2) + pow(fracC*inTrigCEtaLow.GetErrorYhigh(i), 2) + pow(fracD*inTrigDEtaLow.GetErrorYhigh(i), 2) );
    double trigSFEtaLowUncMinus = sqrt( pow(fracA*inTrigAEtaLow.GetErrorYlow(i), 2) + pow(fracB*inTrigBEtaLow.GetErrorYlow(i), 2) + pow(fracC*inTrigCEtaLow.GetErrorYlow(i), 2) + pow(fracD*inTrigDEtaLow.GetErrorYlow(i), 2) );
    
    double idSFEtaLow = getSF(&inIdEtaLow, low, high);
    double idSFEtaLowUncPlus = inIdEtaLow.GetErrorYhigh(i);
    double idSFEtaLowUncMinus = inIdEtaLow.GetErrorYlow(i);
    
    double isoSFEtaLow = getSF(&inIsoEtaLow, low, high);
    double isoSFEtaLowUncPlus = inIsoEtaLow.GetErrorYhigh(i);
    double isoSFEtaLowUncMinus = inIsoEtaLow.GetErrorYlow(i);
    
    double totalSFEtaLow = trigSFEtaLow*idSFEtaLow*isoSFEtaLow;
    double totalSFEtaLowUncPlus = sqrt( pow(trigSFEtaLowUncPlus*idSFEtaLow*isoSFEtaLow, 2) + pow(idSFEtaLowUncPlus*trigSFEtaLow*isoSFEtaLow, 2) + pow(isoSFEtaLowUncPlus*trigSFEtaLow*idSFEtaLow, 2) );
    double totalSFEtaLowUncMinus = sqrt( pow(trigSFEtaLowUncMinus*idSFEtaLow*isoSFEtaLow, 2) + pow(idSFEtaLowUncMinus*trigSFEtaLow*isoSFEtaLow, 2) + pow(isoSFEtaLowUncMinus*trigSFEtaLow*idSFEtaLow, 2) );
    if(verbose_) cout << "etaLow: " << totalSFEtaLow << " + " << totalSFEtaLowUncPlus << " - " << totalSFEtaLowUncMinus << endl;    
    
    
    double trigSFEtaMed = fracA*getSF(&inTrigAEtaMed, low, high) + fracB*getSF(&inTrigBEtaMed, low, high) + fracC*getSF(&inTrigCEtaMed, low, high) + fracD*getSF(&inTrigDEtaMed, low, high) ;
    double trigSFEtaMedUncPlus = sqrt( pow(fracA*inTrigAEtaMed.GetErrorYhigh(i), 2) + pow(fracB*inTrigBEtaMed.GetErrorYhigh(i), 2) + pow(fracC*inTrigCEtaMed.GetErrorYhigh(i), 2) + pow(fracD*inTrigDEtaMed.GetErrorYhigh(i), 2) );
    double trigSFEtaMedUncMinus = sqrt( pow(fracA*inTrigAEtaMed.GetErrorYlow(i), 2) + pow(fracB*inTrigBEtaMed.GetErrorYlow(i), 2) + pow(fracC*inTrigCEtaMed.GetErrorYlow(i), 2) + pow(fracD*inTrigDEtaMed.GetErrorYlow(i), 2) );
    
    double idSFEtaMed = getSF(&inIdEtaMed, low, high);
    double idSFEtaMedUncPlus = inIdEtaMed.GetErrorYhigh(i);
    double idSFEtaMedUncMinus = inIdEtaMed.GetErrorYlow(i);
    
    double isoSFEtaMed = getSF(&inIsoEtaMed, low, high);
    double isoSFEtaMedUncPlus = inIsoEtaMed.GetErrorYhigh(i);
    double isoSFEtaMedUncMinus = inIsoEtaMed.GetErrorYlow(i);
    
    double totalSFEtaMed = trigSFEtaMed*idSFEtaMed*isoSFEtaMed;
    double totalSFEtaMedUncPlus = sqrt( pow(trigSFEtaMedUncPlus*idSFEtaMed*isoSFEtaMed, 2) + pow(idSFEtaMedUncPlus*trigSFEtaMed*isoSFEtaMed, 2) + pow(isoSFEtaMedUncPlus*trigSFEtaMed*idSFEtaMed, 2) );
    double totalSFEtaMedUncMinus = sqrt( pow(trigSFEtaMedUncMinus*idSFEtaMed*isoSFEtaMed, 2) + pow(idSFEtaMedUncMinus*trigSFEtaMed*isoSFEtaMed, 2) + pow(isoSFEtaMedUncMinus*trigSFEtaMed*idSFEtaMed, 2) );
    if(verbose_) cout << "etaMed: " << totalSFEtaMed << " + " << totalSFEtaMedUncPlus << " - " << totalSFEtaMedUncMinus << endl;    
    
    
    double trigSFEtaHigh = fracA*getSF(&inTrigAEtaHigh, low, high) + fracB*getSF(&inTrigBEtaHigh, low, high) + fracC*getSF(&inTrigCEtaHigh, low, high) + fracD*getSF(&inTrigDEtaHigh, low, high) ;
    double trigSFEtaHighUncPlus = sqrt( pow(fracA*inTrigAEtaHigh.GetErrorYhigh(i), 2) + pow(fracB*inTrigBEtaHigh.GetErrorYhigh(i), 2) + pow(fracC*inTrigCEtaHigh.GetErrorYhigh(i), 2) + pow(fracD*inTrigDEtaHigh.GetErrorYhigh(i), 2) );
    double trigSFEtaHighUncMinus = sqrt( pow(fracA*inTrigAEtaHigh.GetErrorYlow(i), 2) + pow(fracB*inTrigBEtaHigh.GetErrorYlow(i), 2) + pow(fracC*inTrigCEtaHigh.GetErrorYlow(i), 2) + pow(fracD*inTrigDEtaHigh.GetErrorYlow(i), 2) );
    
    double idSFEtaHigh = getSF(&inIdEtaHigh, low, high);
    double idSFEtaHighUncPlus = inIdEtaHigh.GetErrorYhigh(i);
    double idSFEtaHighUncMinus = inIdEtaHigh.GetErrorYlow(i);
    
    double isoSFEtaHigh = getSF(&inIsoEtaHigh, low, high);
    double isoSFEtaHighUncPlus = inIsoEtaHigh.GetErrorYhigh(i);
    double isoSFEtaHighUncMinus = inIsoEtaHigh.GetErrorYlow(i);
    
    double totalSFEtaHigh = trigSFEtaHigh*idSFEtaHigh*isoSFEtaHigh;
    double totalSFEtaHighUncPlus = sqrt( pow(trigSFEtaHighUncPlus*idSFEtaHigh*isoSFEtaHigh, 2) + pow(idSFEtaHighUncPlus*trigSFEtaHigh*isoSFEtaHigh, 2) + pow(isoSFEtaHighUncPlus*trigSFEtaHigh*idSFEtaHigh, 2) );
    double totalSFEtaHighUncMinus = sqrt( pow(trigSFEtaHighUncMinus*idSFEtaHigh*isoSFEtaHigh, 2) + pow(idSFEtaHighUncMinus*trigSFEtaHigh*isoSFEtaHigh, 2) + pow(isoSFEtaHighUncMinus*trigSFEtaHigh*idSFEtaHigh, 2) );
    if(verbose_) cout << "etaHigh: " << totalSFEtaHigh << " + " << totalSFEtaHighUncPlus << " - " << totalSFEtaHighUncMinus << endl;    
    
    muonSFEtaLow.push_back( totalSFEtaLow );
    muonSFEtaLowUncPlus.push_back( totalSFEtaLowUncPlus );
    muonSFEtaLowUncMinus.push_back( totalSFEtaLowUncMinus );
    muonSFEtaMed.push_back( totalSFEtaMed );
    muonSFEtaMedUncPlus.push_back( totalSFEtaMedUncPlus );
    muonSFEtaMedUncMinus.push_back( totalSFEtaMedUncMinus );
    muonSFEtaHigh.push_back( totalSFEtaHigh );
    muonSFEtaHighUncPlus.push_back( totalSFEtaHighUncPlus );
    muonSFEtaHighUncMinus.push_back( totalSFEtaHighUncMinus );
  }
}

double LeptonTools::getMuonSF(double eta, double pt, string syst)
{
  if(muonPt.size() == 0) cout << "LeptonTools::getMuonSF :  No SF are loaded!!!" << endl;
  if(pt > 500) return 1.; // No SF available above 500 GeV
  bool foundPt = false;
  for(unsigned int i=0; i<muonPt.size(); i++)
  {
    if( fabs(muonPt[i] - pt) <= muonPtErr[i] )
    {
      foundPt = true;
      if( fabs(eta) < 0.9 )
      {
        if(syst == "Nominal") return muonSFEtaLow[i];
        else if(syst == "Plus") return muonSFEtaLow[i]+muonSFEtaLowUncPlus[i];
        else if(syst == "Minus") return muonSFEtaLow[i]-muonSFEtaLowUncMinus[i];
        else cout << "LeptonTools::getMuonSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 0.9 && fabs(eta) < 1.2 )
      {
        if(syst == "Nominal") return muonSFEtaMed[i];
        else if(syst == "Plus") return muonSFEtaMed[i]+muonSFEtaMedUncPlus[i];
        else if(syst == "Minus") return muonSFEtaMed[i]-muonSFEtaMedUncMinus[i];
        else cout << "LeptonTools::getMuonSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 1.2 && fabs(eta) < 2.1 )
      {
        if(syst == "Nominal") return muonSFEtaHigh[i];
        else if(syst == "Plus") return muonSFEtaHigh[i]+muonSFEtaHighUncPlus[i];
        else if(syst == "Minus") return muonSFEtaHigh[i]-muonSFEtaHighUncMinus[i];
        else cout << "LeptonTools::getMuonSF :  unknown systematic: " << syst << endl;
      }
    }
  }
  if(!foundPt) cout << "Unknown Pt bin: " << pt << endl;
  exit(-1);
  return 1.;
}

void LeptonTools::readElectronSF()
{
  electronPt.push_back(35);
  electronPt.push_back(45);
  electronPt.push_back(125);
  electronPtErr.push_back(5);
  electronPtErr.push_back(5);
  electronPtErr.push_back(75);
  
  // SF taken from AN_2012_429_v4
  
  vector<double> sfId, uncId, sfTrig, uncTrig;
  sfId.push_back(0.939);    sfId.push_back(0.950);    sfId.push_back(0.957);
  uncId.push_back(0.003);   uncId.push_back(0.001);   uncId.push_back(0.001);
  sfTrig.push_back(0.987);  sfTrig.push_back(0.997);  sfTrig.push_back(0.998);
  uncTrig.push_back(0.017); uncTrig.push_back(0.001); uncTrig.push_back(0.002);
  for(unsigned int i=0; i<sfId.size(); i++)
  {
    electronSFEtaLow.push_back(sfId[i]*sfTrig[i]);
    electronSFEtaLowUnc.push_back( sqrt( pow(uncTrig[i]*sfId[i], 2) + pow(uncId[i]*sfTrig[i], 2) ) );
  }
  sfId.clear();   uncId.clear();    sfTrig.clear();   uncTrig.clear();
  
  sfId.push_back(0.920);    sfId.push_back(0.949);    sfId.push_back(0.959);
  uncId.push_back(0.002);   uncId.push_back(0.002);   uncId.push_back(0.003);
  sfTrig.push_back(0.964);  sfTrig.push_back(0.980);  sfTrig.push_back(0.988);
  uncTrig.push_back(0.002); uncTrig.push_back(0.001); uncTrig.push_back(0.002);
  for(unsigned int i=0; i<sfId.size(); i++)
  {
    electronSFEtaMed.push_back(sfId[i]*sfTrig[i]);
    electronSFEtaMedUnc.push_back( sqrt( pow(uncTrig[i]*sfId[i], 2) + pow(uncId[i]*sfTrig[i], 2) ) );
  }
  sfId.clear();   uncId.clear();    sfTrig.clear();   uncTrig.clear();
  
  sfId.push_back(0.907);    sfId.push_back(0.937);    sfId.push_back(0.954);
  uncId.push_back(0.005);   uncId.push_back(0.008);   uncId.push_back(0.011);
  sfTrig.push_back(1.004);  sfTrig.push_back(1.033);  sfTrig.push_back(0.976);
  uncTrig.push_back(0.006); uncTrig.push_back(0.007); uncTrig.push_back(0.015);
  for(unsigned int i=0; i<sfId.size(); i++)
  {
    electronSFEtaHigh.push_back(sfId[i]*sfTrig[i]);
    electronSFEtaHighUnc.push_back( sqrt( pow(uncTrig[i]*sfId[i], 2) + pow(uncId[i]*sfTrig[i], 2) ) );
  }
  
  if(verbose_)
  {
    cout << "Electron SF!!" << endl;
    for(unsigned int i=0; i<electronPt.size(); i++)
    {
      cout << "bin: " << i << " |  " << electronPt[i]-electronPtErr[i] << " <---> " << electronPt[i]+electronPtErr[i] << " | " << electronPt[i] << " +- " << electronPtErr[i] << endl;
      cout << "EtaLow: " << electronSFEtaLow[i] << " +- " << electronSFEtaLowUnc[i] << endl;
      cout << "EtaMed: " << electronSFEtaMed[i] << " +- " << electronSFEtaMedUnc[i] << endl;
      cout << "EtaHigh: " << electronSFEtaHigh[i] << " +- " << electronSFEtaHighUnc[i] << endl;
    }
  }
}

double LeptonTools::getElectronSF(double eta, double pt, string syst)
{
  if(electronPt.size() == 0) cout << "LeptonTools::getElectronSF :  No SF are loaded!!!" << endl;
  if(pt > 200) return 1.; // No SF available above 200 GeV
  bool foundPt = false;
  for(unsigned int i=0; i<electronPt.size(); i++)
  {
    if( fabs(electronPt[i] - pt) <= electronPtErr[i] )
    {
      foundPt = true;
      if( fabs(eta) < 0.8 )
      {
        if(syst == "Nominal") return electronSFEtaLow[i];
        else if(syst == "Plus") return electronSFEtaLow[i]+electronSFEtaLowUnc[i];
        else if(syst == "Minus") return electronSFEtaLow[i]-electronSFEtaLowUnc[i];
        else cout << "LeptonTools::getElectronSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 0.8 && fabs(eta) < 1.478 )
      {
        if(syst == "Nominal") return electronSFEtaMed[i];
        else if(syst == "Plus") return electronSFEtaMed[i]+electronSFEtaMedUnc[i];
        else if(syst == "Minus") return electronSFEtaMed[i]-electronSFEtaMedUnc[i];
        else cout << "LeptonTools::getElectronSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 1.478 && fabs(eta) < 2.5 )
      {
        if(syst == "Nominal") return electronSFEtaHigh[i];
        else if(syst == "Plus") return electronSFEtaHigh[i]+electronSFEtaHighUnc[i];
        else if(syst == "Minus") return electronSFEtaHigh[i]-electronSFEtaHighUnc[i];
        else cout << "LeptonTools::getElectronSF :  unknown systematic: " << syst << endl;
      }
    }
  }
  if(!foundPt) cout << "Unknown Pt bin: " << pt << endl;
  exit(-1);
  return 1.;
}

double LeptonTools::getSF(TGraphAsymmErrors* graph, double low, double high)
{
  int N = graph->GetN();
  for(int i=0; i<N; i++)
  {
    double xTmp = 0, yTmp = 0;
    graph->GetPoint(i, xTmp, yTmp);
    if( xTmp > low && xTmp < high )
      return yTmp;
  }
  return 0;
}
