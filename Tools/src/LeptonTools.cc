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
  ReReco_ = false;
	
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

void LeptonTools::readMuonSF(string IdFile, string IsoFile, string trigFile)
{
  ReReco_ = true;
	
  TFile* inFileID = new TFile(IdFile.c_str(),"READ");
  TGraphAsymmErrors inIdEtaLow = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_Tight_pt_abseta<0.9")->Clone();
  TGraphAsymmErrors inIdEtaMed = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_Tight_pt_abseta0.9-1.2")->Clone();
  TGraphAsymmErrors inIdEtaHigh = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_Tight_pt_abseta1.2-2.1")->Clone();
  TGraphAsymmErrors inIdEtaVeryHigh = *(TGraphAsymmErrors*) inFileID->Get("DATA_over_MC_Tight_pt_abseta2.1-2.4")->Clone();
  inFileID->Close();
  delete inFileID;

	TFile* inFileIso = new TFile(IsoFile.c_str(),"READ");
  TGraphAsymmErrors inIsoEtaLow = *(TGraphAsymmErrors*) inFileIso->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta<0.9")->Clone();
  TGraphAsymmErrors inIsoEtaMed = *(TGraphAsymmErrors*) inFileIso->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta0.9-1.2")->Clone();
  TGraphAsymmErrors inIsoEtaHigh = *(TGraphAsymmErrors*) inFileIso->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta1.2-2.1")->Clone();  
  TGraphAsymmErrors inIsoEtaVeryHigh = *(TGraphAsymmErrors*) inFileIso->Get("DATA_over_MC_combRelIsoPF04dBeta<012_Tight_pt_abseta2.1-2.4")->Clone();  
  inFileIso->Close();
  delete inFileIso;

  TFile* inFileTrig = new TFile(trigFile.c_str(),"READ");
  TGraphAsymmErrors inTrigEtaLow = *(TGraphAsymmErrors*) inFileTrig->Get("IsoMu24_eta2p1_DATA_over_MC_TightID_IsodB_PT_ABSETA_Barrel_0to0p9_pt25-500_2012ABCD")->Clone();
  TGraphAsymmErrors inTrigEtaMed = *(TGraphAsymmErrors*) inFileTrig->Get("IsoMu24_eta2p1_DATA_over_MC_TightID_IsodB_PT_ABSETA_Transition_0p9to1p2_pt25-500_2012ABCD")->Clone();
  TGraphAsymmErrors inTrigEtaHigh = *(TGraphAsymmErrors*) inFileTrig->Get("IsoMu24_eta2p1_DATA_over_MC_TightID_IsodB_PT_ABSETA_Endcaps_1p2to2p1_pt25-500_2012ABCD")->Clone();
  inFileTrig->Close();
  delete inFileTrig;

  //double* xBinTmp_trig = inTrigEtaLow.GetX(); //graphs in trig file start from 25...
  //int N_trig = inTrigEtaLow.GetN();
  double* xBinTmp = inIdEtaLow.GetX(); //graphs in ID and Iso file start from 10 (note: Iso file seems to have a little bug: bin [40,50] is displayed as [40,45] with the part [45,50] absent...)
  int N = inIdEtaLow.GetN();
	
  if(verbose_) cout << "Muon SF!!" << endl;
  
	ScaleFactor trigSF, idSF, isoSF;
	
	//note that there is a mismatch of the number of bins in the trigger and iso/id files -> therefore the getSF method needed to be changed here to the getSFstruct
  for(int i=0; i<N; i++)
  {
    double low = xBinTmp[i]-inIdEtaLow.GetErrorXlow(i);
    double high = xBinTmp[i]+inIdEtaLow.GetErrorXhigh(i);
    muonPt.push_back( (low+high) / 2. );
    muonPtErr.push_back( (high-low) / 2. );
    
		//note that the trigger SF for the first two bins ([10-20] and [20-25] do not make sense, since the trigger SF is only determined from 25 GeV onwards)
    if(verbose_) cout << "bin: " << i << " |  " << low << " <---> " << high << " | " << muonPt[i] << " +- " << muonPtErr[i] << endl;
    
		
		trigSF = getSFstruct(&inTrigEtaLow, low, high);
    double trigSFEtaLow = trigSF.centralvalue;
    double trigSFEtaLowUncPlus = trigSF.highvalue;
    double trigSFEtaLowUncMinus = trigSF.lowvalue;
    
		idSF = getSFstruct(&inIdEtaLow, low, high);
    double idSFEtaLow = idSF.centralvalue;
    double idSFEtaLowUncPlus = idSF.highvalue;
    double idSFEtaLowUncMinus = idSF.lowvalue;
		
		isoSF = getSFstruct(&inIsoEtaLow, low, high);
    double isoSFEtaLow = isoSF.centralvalue;
    double isoSFEtaLowUncPlus = isoSF.highvalue;
    double isoSFEtaLowUncMinus = isoSF.lowvalue;
    
    double totalSFEtaLow = trigSFEtaLow*idSFEtaLow*isoSFEtaLow;
    double totalSFEtaLowUncPlus = sqrt( pow(trigSFEtaLowUncPlus*idSFEtaLow*isoSFEtaLow, 2) + pow(idSFEtaLowUncPlus*trigSFEtaLow*isoSFEtaLow, 2) + pow(isoSFEtaLowUncPlus*trigSFEtaLow*idSFEtaLow, 2) );
    double totalSFEtaLowUncMinus = sqrt( pow(trigSFEtaLowUncMinus*idSFEtaLow*isoSFEtaLow, 2) + pow(idSFEtaLowUncMinus*trigSFEtaLow*isoSFEtaLow, 2) + pow(isoSFEtaLowUncMinus*trigSFEtaLow*idSFEtaLow, 2) );
    if(verbose_) cout << "trigger*id*iso SF -> etaLow: " << totalSFEtaLow << " + " << totalSFEtaLowUncPlus << " - " << totalSFEtaLowUncMinus << endl;    
    
		
    trigSF = getSFstruct(&inTrigEtaMed, low, high);
    double trigSFEtaMed = trigSF.centralvalue;
    double trigSFEtaMedUncPlus = trigSF.highvalue;
    double trigSFEtaMedUncMinus = trigSF.lowvalue;

    idSF = getSFstruct(&inIdEtaMed, low, high);
    double idSFEtaMed = idSF.centralvalue;
    double idSFEtaMedUncPlus = idSF.highvalue;
    double idSFEtaMedUncMinus = idSF.lowvalue;
    
		isoSF = getSFstruct(&inIsoEtaMed, low, high);
    double isoSFEtaMed = isoSF.centralvalue;
    double isoSFEtaMedUncPlus = isoSF.highvalue;
    double isoSFEtaMedUncMinus = isoSF.lowvalue;
    
    double totalSFEtaMed = trigSFEtaMed*idSFEtaMed*isoSFEtaMed;
    double totalSFEtaMedUncPlus = sqrt( pow(trigSFEtaMedUncPlus*idSFEtaMed*isoSFEtaMed, 2) + pow(idSFEtaMedUncPlus*trigSFEtaMed*isoSFEtaMed, 2) + pow(isoSFEtaMedUncPlus*trigSFEtaMed*idSFEtaMed, 2) );
    double totalSFEtaMedUncMinus = sqrt( pow(trigSFEtaMedUncMinus*idSFEtaMed*isoSFEtaMed, 2) + pow(idSFEtaMedUncMinus*trigSFEtaMed*isoSFEtaMed, 2) + pow(isoSFEtaMedUncMinus*trigSFEtaMed*idSFEtaMed, 2) );
    if(verbose_) cout << "trigger*id*iso SF -> etaMed: " << totalSFEtaMed << " + " << totalSFEtaMedUncPlus << " - " << totalSFEtaMedUncMinus << endl;    
    
    
		trigSF = getSFstruct(&inTrigEtaHigh, low, high);
    double trigSFEtaHigh = trigSF.centralvalue;
    double trigSFEtaHighUncPlus = trigSF.highvalue;
    double trigSFEtaHighUncMinus = trigSF.lowvalue;

    idSF = getSFstruct(&inIdEtaHigh, low, high);
		double idSFEtaHigh = idSF.centralvalue;
    double idSFEtaHighUncPlus = idSF.highvalue;
    double idSFEtaHighUncMinus = idSF.lowvalue;
    
		isoSF = getSFstruct(&inIsoEtaHigh, low, high);
    double isoSFEtaHigh = isoSF.centralvalue;
    double isoSFEtaHighUncPlus = isoSF.highvalue;
    double isoSFEtaHighUncMinus = isoSF.lowvalue;
    
    double totalSFEtaHigh = trigSFEtaHigh*idSFEtaHigh*isoSFEtaHigh;
    double totalSFEtaHighUncPlus = sqrt( pow(trigSFEtaHighUncPlus*idSFEtaHigh*isoSFEtaHigh, 2) + pow(idSFEtaHighUncPlus*trigSFEtaHigh*isoSFEtaHigh, 2) + pow(isoSFEtaHighUncPlus*trigSFEtaHigh*idSFEtaHigh, 2) );
    double totalSFEtaHighUncMinus = sqrt( pow(trigSFEtaHighUncMinus*idSFEtaHigh*isoSFEtaHigh, 2) + pow(idSFEtaHighUncMinus*trigSFEtaHigh*isoSFEtaHigh, 2) + pow(isoSFEtaHighUncMinus*trigSFEtaHigh*idSFEtaHigh, 2) );
    if(verbose_) cout << "trigger*id*iso SF -> etaHigh: " << totalSFEtaHigh << " + " << totalSFEtaHighUncPlus << " - " << totalSFEtaHighUncMinus << endl;    
    								
		//total trigger*id*iso SF
    muonSFEtaLow.push_back( totalSFEtaLow );
    muonSFEtaLowUncPlus.push_back( totalSFEtaLowUncPlus );
    muonSFEtaLowUncMinus.push_back( totalSFEtaLowUncMinus );
    muonSFEtaMed.push_back( totalSFEtaMed );
    muonSFEtaMedUncPlus.push_back( totalSFEtaMedUncPlus );
    muonSFEtaMedUncMinus.push_back( totalSFEtaMedUncMinus );
    muonSFEtaHigh.push_back( totalSFEtaHigh );
    muonSFEtaHighUncPlus.push_back( totalSFEtaHighUncPlus );
    muonSFEtaHighUncMinus.push_back( totalSFEtaHighUncMinus );


    //now only trigger SF (needed for jets with Pt in [300,500], no id or iso isolation is measured for those jets)
    muonTrigSFEtaLow.push_back( trigSFEtaLow );
    muonTrigSFEtaLowUncPlus.push_back( trigSFEtaLowUncPlus );
    muonTrigSFEtaLowUncMinus.push_back( trigSFEtaLowUncMinus );
    muonTrigSFEtaMed.push_back( trigSFEtaMed );
    muonTrigSFEtaMedUncPlus.push_back(trigSFEtaMedUncPlus );
    muonTrigSFEtaMedUncMinus.push_back( trigSFEtaMedUncMinus );
    muonTrigSFEtaHigh.push_back( trigSFEtaHigh );
    muonTrigSFEtaHighUncPlus.push_back( trigSFEtaHighUncPlus );
    muonTrigSFEtaHighUncMinus.push_back( trigSFEtaHighUncMinus );
		

		//now only id*iso SF				
		double idisoSFEtaLow = idSFEtaLow*isoSFEtaLow;
    double idisoSFEtaLowUncPlus = sqrt( pow(idSFEtaLowUncPlus*isoSFEtaLow, 2) + pow(isoSFEtaLowUncPlus*idSFEtaLow, 2) );
    double idisoSFEtaLowUncMinus = sqrt( pow(idSFEtaLowUncMinus*isoSFEtaLow, 2) + pow(isoSFEtaLowUncMinus*idSFEtaLow, 2) );		
    if(verbose_) cout << "id*iso SF -> etaLow: " << idisoSFEtaLow << " + " << idisoSFEtaLowUncPlus << " - " << idisoSFEtaLowUncMinus << endl; 
		
		double idisoSFEtaMed = idSFEtaMed*isoSFEtaMed;
    double idisoSFEtaMedUncPlus = sqrt( pow(idSFEtaMedUncPlus*isoSFEtaMed, 2) + pow(isoSFEtaMedUncPlus*idSFEtaMed, 2) );
    double idisoSFEtaMedUncMinus = sqrt( pow(idSFEtaMedUncMinus*isoSFEtaMed, 2) + pow(isoSFEtaMedUncMinus*idSFEtaMed, 2) );	
		if(verbose_) cout << "id*iso SF -> etaMed: " << idisoSFEtaMed << " + " << idisoSFEtaMedUncPlus << " - " << idisoSFEtaMedUncMinus << endl; 
		
		double idisoSFEtaHigh = idSFEtaHigh*isoSFEtaHigh;
    double idisoSFEtaHighUncPlus = sqrt( pow(idSFEtaHighUncPlus*isoSFEtaHigh, 2) + pow(isoSFEtaHighUncPlus*idSFEtaHigh, 2) );
    double idisoSFEtaHighUncMinus = sqrt( pow(idSFEtaHighUncMinus*isoSFEtaHigh, 2) + pow(isoSFEtaHighUncMinus*idSFEtaHigh, 2) );	
		if(verbose_) cout << "id*iso SF -> etaHigh: " << idisoSFEtaHigh << " + " << idisoSFEtaHighUncPlus << " - " << idisoSFEtaHighUncMinus << endl;    		

    idSF = getSFstruct(&inIdEtaVeryHigh, low, high);
		double idSFEtaVeryHigh = idSF.centralvalue;
    double idSFEtaVeryHighUncPlus = idSF.highvalue;
    double idSFEtaVeryHighUncMinus = idSF.lowvalue;
		
    isoSF = getSFstruct(&inIsoEtaVeryHigh, low, high);
		double isoSFEtaVeryHigh = isoSF.centralvalue;
    double isoSFEtaVeryHighUncPlus = isoSF.highvalue;
    double isoSFEtaVeryHighUncMinus = isoSF.lowvalue; 
		
    double idisoSFEtaVeryHigh = idSFEtaVeryHigh*isoSFEtaVeryHigh;
    double idisoSFEtaVeryHighUncPlus = sqrt( pow(idSFEtaVeryHighUncPlus*isoSFEtaVeryHigh, 2) + pow(isoSFEtaVeryHighUncPlus*idSFEtaVeryHigh, 2) );
    double idisoSFEtaVeryHighUncMinus = sqrt( pow(idSFEtaVeryHighUncMinus*isoSFEtaVeryHigh, 2) + pow(isoSFEtaVeryHighUncMinus*idSFEtaVeryHigh, 2) );		
		if(verbose_) cout << "id*iso SF -> etaVeryHigh: " << idisoSFEtaVeryHigh << " + " << idisoSFEtaVeryHighUncPlus << " - " << idisoSFEtaVeryHighUncMinus << endl;    		
		
		//total id*iso SF (no trigger)
    muonIdIsoSFEtaLow.push_back( idisoSFEtaLow );
    muonIdIsoSFEtaLowUncPlus.push_back( idisoSFEtaLowUncPlus );
    muonIdIsoSFEtaLowUncMinus.push_back( idisoSFEtaLowUncMinus );
    muonIdIsoSFEtaMed.push_back( idisoSFEtaMed );
    muonIdIsoSFEtaMedUncPlus.push_back( idisoSFEtaMedUncPlus );
    muonIdIsoSFEtaMedUncMinus.push_back( idisoSFEtaMedUncMinus );
    muonIdIsoSFEtaHigh.push_back( idisoSFEtaHigh );
    muonIdIsoSFEtaHighUncPlus.push_back( idisoSFEtaHighUncPlus );
    muonIdIsoSFEtaHighUncMinus.push_back( idisoSFEtaHighUncMinus );
		muonIdIsoSFEtaVeryHigh.push_back( idisoSFEtaVeryHigh );
    muonIdIsoSFEtaVeryHighUncPlus.push_back( idisoSFEtaVeryHighUncPlus );
    muonIdIsoSFEtaVeryHighUncMinus.push_back( idisoSFEtaVeryHighUncMinus );
  }

}



double LeptonTools::getMuonSF(double eta, double pt, string syst)
{
  if(muonPt.size() == 0) cout << "LeptonTools::getMuonSF :  No SF are loaded!!!" << endl;
  if(pt > 500) return 1.; // No trigger SF available above 500 GeV
  bool foundPt = false;
	bool VeryHighPt = false;
  for(unsigned int i=0; i<muonPt.size(); i++)
  {
	  //cout<<" muonPt["<<i<<"] = "<<muonPt[i]<<", muonPtErr["<<i<<"] = "<<muonPtErr[i]<<endl;
    if( (fabs(muonPt[i] - pt) <= muonPtErr[i]) || (ReReco_ && i==muonPt.size()-1 && pt > 300))  //300 GeV is the edge of the determination of the Id and Iso SFs with the 2012 ReReco data (not for trigger SF, that goes up to 500)    
    {
		  foundPt = true;
			if(ReReco_ && i==muonPt.size()-1 && pt > 300) VeryHighPt = true;
			
      if( fabs(eta) < 0.9 )
      {
        if(syst == "Nominal")
				{
				   if(VeryHighPt) return muonTrigSFEtaLow[i];
				   else return muonSFEtaLow[i];
				}
        else if(syst == "Plus")
				{
				   if(VeryHighPt) return muonTrigSFEtaLow[i]+muonTrigSFEtaLowUncPlus[i];
				   else return muonSFEtaLow[i]+muonSFEtaLowUncPlus[i];
				}
        else if(syst == "Minus")
				{
				   if(VeryHighPt) return muonTrigSFEtaLow[i]-muonTrigSFEtaLowUncMinus[i];
				   else return muonSFEtaLow[i]-muonSFEtaLowUncMinus[i];
				}
        else cout << "LeptonTools::getMuonSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 0.9 && fabs(eta) < 1.2 )
      {
        if(syst == "Nominal")
				{
				   if(VeryHighPt) return muonTrigSFEtaMed[i];
				   else return muonSFEtaMed[i];
				}
        else if(syst == "Plus")
				{
				   if(VeryHighPt) return muonTrigSFEtaMed[i]+muonTrigSFEtaMedUncPlus[i];
				   else return muonSFEtaMed[i]+muonSFEtaMedUncPlus[i];
				}
        else if(syst == "Minus")
				{
				   if(VeryHighPt) return muonTrigSFEtaMed[i]-muonTrigSFEtaMedUncMinus[i];
				   else return muonSFEtaMed[i]-muonSFEtaMedUncMinus[i];
				}
        else cout << "LeptonTools::getMuonSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 1.2 && fabs(eta) < 2.1 )
      {
        if(syst == "Nominal")
				{
				   if(VeryHighPt) return muonTrigSFEtaHigh[i];
				   else return muonSFEtaHigh[i];
				}
        else if(syst == "Plus")
				{
				   if(VeryHighPt) return muonTrigSFEtaHigh[i]+muonTrigSFEtaHighUncPlus[i];
				   else return muonSFEtaHigh[i]+muonSFEtaHighUncPlus[i];
			  }
        else if(syst == "Minus")
				{
				   if(VeryHighPt) return muonTrigSFEtaHigh[i]-muonTrigSFEtaHighUncMinus[i];
				   else return muonSFEtaHigh[i]-muonSFEtaHighUncMinus[i];
				}
        else cout << "LeptonTools::getMuonSF :  unknown systematic: " << syst << endl;
      }
    }
  }
  if(!foundPt) cout << "LeptonTools::getMuonSF :  Unknown Pt bin: " << pt << endl;
  exit(-1);
  return 1.;
}

double LeptonTools::getMuonIdIsoSF(double eta, double pt, string syst)
{
  if(muonPt.size() == 0) cout << "LeptonTools::getMuonSF :  No SF are loaded!!!" << endl;
  if(pt > 300) return 1.; // No id and iso SF available above 300 GeV
  bool foundPt = false;
  for(unsigned int i=0; i<muonPt.size(); i++)
  {
    if( fabs(muonPt[i] - pt) <= muonPtErr[i] )
    {
      foundPt = true;
      if( fabs(eta) < 0.9 )
      {
        if(syst == "Nominal") return muonIdIsoSFEtaLow[i];
        else if(syst == "Plus") return muonIdIsoSFEtaLow[i]+muonIdIsoSFEtaLowUncPlus[i];
        else if(syst == "Minus") return muonIdIsoSFEtaLow[i]-muonIdIsoSFEtaLowUncMinus[i];
        else cout << "LeptonTools::getMuonIdIsoSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 0.9 && fabs(eta) < 1.2 )
      {
        if(syst == "Nominal") return muonIdIsoSFEtaMed[i];
        else if(syst == "Plus") return muonIdIsoSFEtaMed[i]+muonIdIsoSFEtaMedUncPlus[i];
        else if(syst == "Minus") return muonIdIsoSFEtaMed[i]-muonIdIsoSFEtaMedUncMinus[i];
        else cout << "LeptonTools::getMuonIdIsoSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 1.2 && fabs(eta) < 2.1 )
      {
        if(syst == "Nominal") return muonIdIsoSFEtaHigh[i];
        else if(syst == "Plus") return muonIdIsoSFEtaHigh[i]+muonIdIsoSFEtaHighUncPlus[i];
        else if(syst == "Minus") return muonIdIsoSFEtaHigh[i]-muonIdIsoSFEtaHighUncMinus[i];
        else cout << "LeptonTools::getMuonIdIsoSF :  unknown systematic: " << syst << endl;
      }
			else if( fabs(eta) >= 2.1 && fabs(eta) < 2.4 )
      {
        if(syst == "Nominal") return muonIdIsoSFEtaVeryHigh[i];
        else if(syst == "Plus") return muonIdIsoSFEtaVeryHigh[i]+muonIdIsoSFEtaVeryHighUncPlus[i];
        else if(syst == "Minus") return muonIdIsoSFEtaVeryHigh[i]-muonIdIsoSFEtaVeryHighUncMinus[i];
        else cout << "LeptonTools::getMuonIdIsoSF :  unknown systematic: " << syst << endl;
      }
    }
  }
  if(!foundPt) cout << "LeptonTools::getMuonSF :  Unknown Pt bin: " << pt << endl;
  exit(-1);
  return 1.;
}


void LeptonTools::readElectronSF()
{
  electronPt.push_back(25);
  electronPt.push_back(35);
  electronPt.push_back(45);
  electronPt.push_back(125);
	electronPtErr.push_back(5);
  electronPtErr.push_back(5);
  electronPtErr.push_back(5);
  electronPtErr.push_back(75);
  
  // SF taken from AN_2012_429_v4; table 11 for id&iso efficiencies, and table 12 for HLT_Ele27_WP80 trigger efficiency
  
  vector<double> sfIdIso, uncIdIso, sfTrig, uncTrig;
  sfIdIso.push_back(0.949); sfIdIso.push_back(0.939);    sfIdIso.push_back(0.950);    sfIdIso.push_back(0.957);
  uncIdIso.push_back(0.007);  uncIdIso.push_back(0.003);   uncIdIso.push_back(0.001);   uncIdIso.push_back(0.001); //maximum of uncertainty up and down...
  sfTrig.push_back(0.987);  sfTrig.push_back(0.997);  sfTrig.push_back(0.998);
  uncTrig.push_back(0.017); uncTrig.push_back(0.001); uncTrig.push_back(0.002); //maximum of uncertainty up and down...
  for(unsigned int i=0; i<sfIdIso.size(); i++)
  {
    if(i<sfTrig.size())
		{
		   electronSFEtaLow.push_back(sfIdIso[i+1]*sfTrig[i]);
       electronSFEtaLowUnc.push_back( sqrt( pow(uncTrig[i]*sfIdIso[i+1], 2) + pow(uncIdIso[i+1]*sfTrig[i], 2) ) );
		}
		electronIdIsoSFEtaLow.push_back(sfIdIso[i]);
		electronIdIsoSFEtaLowUnc.push_back(uncIdIso[i]);
  }	
  sfIdIso.clear();   uncIdIso.clear();    sfTrig.clear();   uncTrig.clear();
  
  sfIdIso.push_back(0.900);  sfIdIso.push_back(0.920);    sfIdIso.push_back(0.949);    sfIdIso.push_back(0.959);
  uncIdIso.push_back(0.010);  uncIdIso.push_back(0.002);   uncIdIso.push_back(0.002);   uncIdIso.push_back(0.003);
  sfTrig.push_back(0.964);  sfTrig.push_back(0.980);  sfTrig.push_back(0.988);
  uncTrig.push_back(0.002); uncTrig.push_back(0.001); uncTrig.push_back(0.002);
  for(unsigned int i=0; i<sfIdIso.size(); i++)
  {
    if(i<sfTrig.size())
		{
		   electronSFEtaMed.push_back(sfIdIso[i+1]*sfTrig[i]);
       electronSFEtaMedUnc.push_back( sqrt( pow(uncTrig[i]*sfIdIso[i+1], 2) + pow(uncIdIso[i+1]*sfTrig[i], 2) ) );
		}
		electronIdIsoSFEtaMed.push_back(sfIdIso[i]);
		electronIdIsoSFEtaMedUnc.push_back(uncIdIso[i]);
  }	
  sfIdIso.clear();   uncIdIso.clear();    sfTrig.clear();   uncTrig.clear();
  
  sfIdIso.push_back(0.857);  sfIdIso.push_back(0.907);    sfIdIso.push_back(0.937);    sfIdIso.push_back(0.954);
  uncIdIso.push_back(0.010);  uncIdIso.push_back(0.005);   uncIdIso.push_back(0.008);   uncIdIso.push_back(0.011);
  sfTrig.push_back(1.004);  sfTrig.push_back(1.033);  sfTrig.push_back(0.976);
  uncTrig.push_back(0.006); uncTrig.push_back(0.007); uncTrig.push_back(0.015);
  for(unsigned int i=0; i<sfIdIso.size(); i++)
  {
    if(i<sfTrig.size())
		{
		   electronSFEtaHigh.push_back(sfIdIso[i+1]*sfTrig[i]);
       electronSFEtaHighUnc.push_back( sqrt( pow(uncTrig[i]*sfIdIso[i+1], 2) + pow(uncIdIso[i+1]*sfTrig[i], 2) ) );
		}
		electronIdIsoSFEtaHigh.push_back(sfIdIso[i]);
		electronIdIsoSFEtaHighUnc.push_back(uncIdIso[i]);
  }	
  
  if(verbose_)
  {
    cout << "Electron trigger*idiso SF!!" << endl;
    for(unsigned int i=0; i<electronSFEtaLow.size(); i++)
    {        
		   cout << "bin: " << i << " |  " << electronPt[i+1]-electronPtErr[i+1] << " <---> " << electronPt[i+1]+electronPtErr[i+1] << " | " << electronPt[i+1] << " +- " << electronPtErr[i+1] << endl;
       cout << "EtaLow: " << electronSFEtaLow[i] << " +- " << electronSFEtaLowUnc[i] << endl;
       cout << "EtaMed: " << electronSFEtaMed[i] << " +- " << electronSFEtaMedUnc[i] << endl;
       cout << "EtaHigh: " << electronSFEtaHigh[i] << " +- " << electronSFEtaHighUnc[i] << endl;
    }
		cout << "Electron idiso SF!!" << endl;
    for(unsigned int i=0; i<electronIdIsoSFEtaLow.size(); i++)
    {        
		   cout << "bin: " << i << " |  " << electronPt[i]-electronPtErr[i] << " <---> " << electronPt[i]+electronPtErr[i] << " | " << electronPt[i] << " +- " << electronPtErr[i] << endl;
       cout << "EtaLow: " << electronIdIsoSFEtaLow[i] << " +- " << electronIdIsoSFEtaLowUnc[i] << endl;
       cout << "EtaMed: " << electronIdIsoSFEtaMed[i] << " +- " << electronIdIsoSFEtaMedUnc[i] << endl;
       cout << "EtaHigh: " << electronIdIsoSFEtaHigh[i] << " +- " << electronIdIsoSFEtaHighUnc[i] << endl;
    }
  }
}

double LeptonTools::getElectronSF(double eta, double pt, string syst)
{
  if(electronPt.size() == 0) cout << "LeptonTools::getElectronSF :  No SF are loaded!!!" << endl;
  if(pt > 200) return 1.; // No SF available above 200 GeV
  bool foundPt = false;
  for(unsigned int i=1; i<electronPt.size(); i++)
  {
    if( fabs(electronPt[i] - pt) <= electronPtErr[i] )
    {
      foundPt = true;
      if( fabs(eta) < 0.8 )
      {
        if(syst == "Nominal") return electronSFEtaLow[i-1];
        else if(syst == "Plus") return electronSFEtaLow[i-1]+electronSFEtaLowUnc[i-1];
        else if(syst == "Minus") return electronSFEtaLow[i-1]-electronSFEtaLowUnc[i-1];
        else cout << "LeptonTools::getElectronSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 0.8 && fabs(eta) < 1.478 )
      {
        if(syst == "Nominal") return electronSFEtaMed[i-1];
        else if(syst == "Plus") return electronSFEtaMed[i-1]+electronSFEtaMedUnc[i-1];
        else if(syst == "Minus") return electronSFEtaMed[i-1]-electronSFEtaMedUnc[i-1];
        else cout << "LeptonTools::getElectronSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 1.478 && fabs(eta) < 2.5 )
      {
        if(syst == "Nominal") return electronSFEtaHigh[i-1];
        else if(syst == "Plus") return electronSFEtaHigh[i-1]+electronSFEtaHighUnc[i-1];
        else if(syst == "Minus") return electronSFEtaHigh[i-1]-electronSFEtaHighUnc[i-1];
        else cout << "LeptonTools::getElectronSF :  unknown systematic: " << syst << endl;
      }
    }
  }
  if(!foundPt) cout << "Unknown Pt bin: " << pt << endl;
  exit(-1);
  return 1.;
}

double LeptonTools::getElectronIdIsoSF(double eta, double pt, string syst)
{
  if(electronPt.size() == 0) cout << "LeptonTools::getElectronIdIsoSF :  No SF are loaded!!!" << endl;
  if(pt > 200) return 1.; // No SF available above 200 GeV
  bool foundPt = false;
  for(unsigned int i=0; i<electronPt.size(); i++)
  {
    if( fabs(electronPt[i] - pt) <= electronPtErr[i] )
    {
      foundPt = true;
      if( fabs(eta) < 0.8 )
      {
        if(syst == "Nominal") return electronIdIsoSFEtaLow[i];
        else if(syst == "Plus") return electronIdIsoSFEtaLow[i]+electronIdIsoSFEtaLowUnc[i];
        else if(syst == "Minus") return electronIdIsoSFEtaLow[i]-electronIdIsoSFEtaLowUnc[i];
        else cout << "LeptonTools::getElectronIdIsoSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 0.8 && fabs(eta) < 1.478 )
      {
        if(syst == "Nominal") return electronIdIsoSFEtaMed[i];
        else if(syst == "Plus") return electronIdIsoSFEtaMed[i]+electronIdIsoSFEtaMedUnc[i];
        else if(syst == "Minus") return electronIdIsoSFEtaMed[i]-electronIdIsoSFEtaMedUnc[i];
        else cout << "LeptonTools::getElectronIdIsoSF :  unknown systematic: " << syst << endl;
      }
      else if( fabs(eta) >= 1.478 && fabs(eta) < 2.5 )
      {
        if(syst == "Nominal") return electronIdIsoSFEtaHigh[i];
        else if(syst == "Plus") return electronIdIsoSFEtaHigh[i]+electronIdIsoSFEtaHighUnc[i];
        else if(syst == "Minus") return electronIdIsoSFEtaHigh[i]-electronIdIsoSFEtaHighUnc[i];
        else cout << "LeptonTools::getElectronIdIsoSF :  unknown systematic: " << syst << endl;
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

LeptonTools::ScaleFactor LeptonTools::getSFstruct(TGraphAsymmErrors* graph, double low, double high)
{
  int N = graph->GetN();	
  for(int i=0; i<N; i++)
  {
    double xTmp = 0, yTmp = 0;
    graph->GetPoint(i, xTmp, yTmp);
    if( xTmp > low && xTmp < high )
		{
      SF.centralvalue = yTmp;
			SF.highvalue = graph->GetErrorYhigh(i);
			SF.lowvalue = graph->GetErrorYlow(i);
			//if(verbose_) cout<<"  [LeptonTools::getSF] i = "<<i<<", SF.centralvalue = "<<SF.centralvalue<<", SF.highvalue = "<<SF.highvalue<<", SF.lowvalue = "<<SF.lowvalue<<endl;
		}
  }
  return SF;
}
