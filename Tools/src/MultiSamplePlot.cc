#include "../interface/MultiSamplePlot.h"

MultiSamplePlot::MultiSamplePlot(
vector<Dataset*> datasets, string PlotName, int Nbins, float Min, float Max, string XaxisLabel, string YaxisLabel){
	Nbins_ = Nbins;
	string histoName = "";
	string histoTitle = "";
	plotName_ = PlotName;
	for(unsigned int i=0;i<datasets.size();i++){
		histoName  = PlotName+"_"+datasets[i]->Name();
		histoTitle = datasets[i]->Title();
		TH1F* h = new TH1F(histoName.c_str(),histoTitle.c_str(),Nbins,Min,Max);
		h->SetStats(false);
		h->Sumw2();
		h->GetXaxis()->SetTitle(XaxisLabel.c_str());
		h->GetYaxis()->SetTitle(YaxisLabel.c_str());
		plots_.push_back(pair<TH1F*,Dataset*>(h,datasets[i]));
    if(datasets[i]->Name().find("data") == 0 || datasets[i]->Name().find("Data") == 0 || datasets[i]->Name().find("DATA") == 0 )
      lumi_ = datasets[i]->EquivalentLumi();
	}
	histoName  = "hRandomPseudoData_"+PlotName;
	histoTitle = "Pseudo-data";
	hRandomPseudoData_ = new TH1F(histoName.c_str(),histoTitle.c_str(),Nbins,Min,Max);
	hData_ = 0;
	hCanvas_ = 0;
	hCanvasStack_ = 0;
	hCanvasStackLogY_ = 0;
	hCanvasStackAreaNorm_ = 0;
	hCanvasStackAreaNormLogY_ = 0;
  hErrorPlus_ = 0;
  hErrorMinus_ = 0;
	hStack_ = 0;
 	hStackAreaNorm_ = 0;
	maxY_ = -1;
	text_ = "";
	showNumberEntries_ = true;
}

MultiSamplePlot::MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float* binsX, string XaxisLabel, string YaxisLabel){
	Nbins_ = Nbins;
	string histoName = "";
	string histoTitle = "";
	plotName_ = PlotName;
	for(unsigned int i=0;i<datasets.size();i++){
		histoName = PlotName+"_"+datasets[i]->Name();
		histoTitle = datasets[i]->Title();
		TH1F* h = new TH1F(histoName.c_str(),histoTitle.c_str(),Nbins,binsX);
		h->SetStats(false);
		h->Sumw2();
		h->GetXaxis()->SetTitle(XaxisLabel.c_str());
		h->GetYaxis()->SetTitle(YaxisLabel.c_str());
		plots_.push_back(pair<TH1F*,Dataset*>(h,datasets[i]));
		if(datasets[i]->Name().find("data") == 0 || datasets[i]->Name().find("Data") == 0 || datasets[i]->Name().find("DATA") == 0 )
      lumi_ = datasets[i]->EquivalentLumi();
	}
	histoName = "hRandomPseudoData_"+PlotName;
	histoTitle = "Pseudo-data";
	hRandomPseudoData_ = new TH1F(histoName.c_str(),histoTitle.c_str(),Nbins,binsX);
	hData_ = 0;
	hCanvas_ = 0;
	hCanvasStack_ = 0;
	hCanvasStackLogY_ = 0;
	hCanvasStackAreaNorm_ = 0;
	hCanvasStackAreaNormLogY_ = 0;
  hErrorPlus_ = 0;
  hErrorMinus_ = 0;
	hStack_ = 0;
	hStackAreaNorm_ = 0;
	maxY_ = -1;
	text_ = "";
	showNumberEntries_ = true;
}

MultiSamplePlot::MultiSamplePlot(vector<pair<TH1F*,Dataset*> > vec){
	plots_ = vec;
	Nbins_ = (vec[0].first)->GetNbinsX();
	string histoName = "";
	string histoTitle = "";
	hRandomPseudoData_ = 0;
	hData_ = 0;
	hCanvas_ = 0;
	hCanvasStack_ = 0;
	hCanvasStackLogY_ = 0;
	hCanvasStackAreaNorm_ = 0;
	hCanvasStackAreaNormLogY_ = 0;
  hErrorPlus_ = 0;
  hErrorMinus_ = 0;
	hStack_ = 0;
	hStackAreaNorm_ = 0;
	maxY_ = -1;
	text_ = "";
	showNumberEntries_ = true;
	for(unsigned int i=0;i<vec.size();i++)
	{
	  if(vec[i].second->Name().find("data") == 0 || vec[i].second->Name().find("Data") == 0 || vec[i].second->Name().find("DATA") == 0 )
	    lumi_ = vec[i].second->EquivalentLumi();
  }	
}

MultiSamplePlot::~MultiSamplePlot(){
	if(hStack_)      delete hStack_;
	if(hStackAreaNorm_)      delete hStackAreaNorm_;
	if(hCanvas_)     delete hCanvas_;
	if(hCanvasStack_)     delete hCanvasStack_;
	if(hCanvasStackAreaNorm_)     delete hCanvasStackAreaNorm_;
	if(hCanvasStackLogY_)     delete hCanvasStackLogY_;
	if(hCanvasStackAreaNormLogY_)     delete hCanvasStackAreaNormLogY_;
	if(hRandomPseudoData_) delete hRandomPseudoData_;
	if(hData_) delete hData_;
}

void MultiSamplePlot::AddDataHisto(TH1F* histo){
	hData_ = (TH1F*) histo->Clone();
}

void MultiSamplePlot::Fill(float value, Dataset* data, bool scale, float Lumi){
	if(scale){
		for(unsigned int i=0;i<plots_.size();i++){
			if(data->Name()==plots_[i].second->Name()) plots_[i].first->Fill(value, data->NormFactor()*Lumi);
//			if(data->Name()==plots_[i].second->Name()) cout << "NormFactor for " << data->Name() << " = " <<  data->NormFactor()*Lumi << endl;
		}
	}
	else{
		for(unsigned int i=0;i<plots_.size();i++){
			if(data->Name()==plots_[i].second->Name()) plots_[i].first->Fill(value);
		}
	}
}

void MultiSamplePlot::Draw(bool addRandomPseudoData, string label, bool mergeTT, bool mergeQCD, bool mergeW, bool mergeZ, bool mergeST, int scaleNPSignal, bool addRatio, bool mergeVV, bool mergeTTV)
{
	vector<TH1F*> histos;
	vector<TH1F*> histos2;
	vector<Dataset*> datasets;
	
	double* SummedBins = new double[Nbins_];
	for(int i=0;i<Nbins_;i++) SummedBins[i]=0;
  
  if(addRatio)
  {
    string filename = "ErrorBands/Error_"+plotName_+".root";
    TFile* tmpInfile = new TFile(filename.c_str(),"READ");
    if( ! tmpInfile->IsZombie() )
    {
		  TH1F* hErrorPlus = (TH1F*) tmpInfile->Get("Plus")->Clone();
		  TH1F* hErrorMinus = (TH1F*) tmpInfile->Get("Minus")->Clone();
	  	hErrorPlus_ = new TGraph(hErrorPlus->GetNbinsX());
	  	hErrorPlus_->SetNameTitle("hErrorPlus","hErrorPlus");
	  	hErrorPlus_->SetLineWidth(2);
	  	hErrorPlus_->SetLineColor(30);
	    hErrorMinus_ = new TGraph(hErrorMinus->GetNbinsX());
	  	hErrorMinus_->SetNameTitle("hErrorMinus","hErrorMinus");
	  	hErrorMinus_->SetLineWidth(2);
	  	hErrorMinus_->SetLineColor(30);
		  for (int iBin=1; iBin < hErrorPlus->GetNbinsX()+1; iBin++)
		  {
        hErrorPlus_->SetPoint(iBin-1, hErrorPlus->GetBinCenter(iBin), hErrorPlus->GetBinContent(iBin));
        hErrorMinus_->SetPoint(iBin-1, hErrorMinus->GetBinCenter(iBin), hErrorMinus->GetBinContent(iBin));
      }
		  tmpInfile->Close();
    }
	  delete tmpInfile;
  }
  //cout<<"So far, so good : 1"<<endl;

	int dataPlotID=-1;
	float integralData = 0;
	float integralMC = 0;

	for(unsigned int i=0;i<plots_.size();i++){
	  if(plots_[i].second->Name().find("data") == 0 || plots_[i].second->Name().find("Data") == 0 || plots_[i].second->Name().find("DATA") == 0 )
	    dataPlotID = i;
	}

	//Dataset tempTTDataset  = Dataset();
	TH1F* ttHisto = 0;
	//Dataset tempQCDDataset = Dataset();
	TH1F* qcdHisto = 0;
	//Dataset tempWDataset   = Dataset();
	TH1F* wHisto = 0;
	//Dataset tempZDataset   = Dataset();
	TH1F* zHisto = 0;
	//Dataset tempSTDataset  = Dataset();
	TH1F* stHisto = 0;
	//Dataset tempVVDataset  = Dataset();
	TH1F* vvHisto = 0;
	//Dataset tempTTVDataset  = Dataset();
	TH1F* ttvHisto = 0;

  if(mergeTT || mergeQCD || mergeW || mergeZ || mergeST || mergeVV || mergeTTV){
	  for(unsigned int i=0; i<plots_.size();i++){
		  string datasetName = plots_[i].second->Name();
        
		  if(mergeTT && ( datasetName.find("TT_") == 0 || datasetName.find("TTbarJets") == 0 ) ){
			  if(!ttHisto) ttHisto = (TH1F*) plots_[i].first->Clone();
			  else ttHisto->Add(plots_[i].first);
			  //tempTTDataset = Dataset("TT_Merged","t#bar{t}+jets, merged",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
		  }
		  if(mergeQCD && datasetName.find("QCD_") == 0){
			  if(!qcdHisto) qcdHisto = (TH1F*) plots_[i].first->Clone();
			  else qcdHisto->Add(plots_[i].first);
			  //tempQCDDataset = Dataset("QCD_Merged","Multi-jets, merged",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
		  }
		  if(mergeW && datasetName.find("W_") == 0){
			  if(!wHisto) wHisto = (TH1F*) plots_[i].first->Clone();
			  else wHisto->Add(plots_[i].first);
			  //tempWDataset = Dataset("WJets_Merged","W+jets, merged",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
		  }
		  if(mergeZ && datasetName.find("Z_") == 0){
			  if(!zHisto) zHisto = (TH1F*) plots_[i].first->Clone();
			  else zHisto->Add(plots_[i].first);
			  //tempZDataset = Dataset("ZJets_Merged","Z+jets, merged",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
		  }
		  if(mergeST && ( datasetName.find("ST_") == 0 || datasetName.find("SingleTop_") == 0 ) ){
			  if(!stHisto) stHisto = (TH1F*) plots_[i].first->Clone();
			  else stHisto->Add(plots_[i].first);
			  //tempSTDataset = Dataset("ST_Merged","st+jets, merged",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
		  }
		  if(mergeVV && ( datasetName.find("WW") == 0 || datasetName.find("WZ") == 0 || datasetName.find("ZZ") == 0) ){
			  if(!vvHisto) vvHisto = (TH1F*) plots_[i].first->Clone();
			  else vvHisto->Add(plots_[i].first);
			  //tempVVDataset = Dataset("VV_Merged","VV, merged",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
		  }
		  if(mergeTTV && ( datasetName.find("ttW") == 0 || datasetName.find("ttZ") == 0 ) ){
			  if(!ttvHisto) ttvHisto = (TH1F*) plots_[i].first->Clone();
			  else ttvHisto->Add(plots_[i].first);
			  //tempTTVDataset = Dataset("ttV_Merged","ttV, merged",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
		  }
	  }
  }
  //cout<<"So far, so good : 2"<<endl;

	for(unsigned int i=0;i<plots_.size();i++){
		TH1F* histo = (TH1F*) plots_[i].first->Clone();
		Dataset* dataSet = plots_[i].second;
		string datasetName = plots_[i].second->Name();
	  
		if(mergeTT && ( datasetName.find("TT_") == 0 || datasetName.find("TTbarJets") == 0 ) ){
			if(ttHisto){
				histo = (TH1F*) ttHisto->Clone();
//				dataSet = new Dataset("TT_Merged","t#bar{t}+jets",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				dataSet = new Dataset("TT_Merged","t#bar{t}",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				ttHisto = 0;
			}
			else continue;
		}
		if(mergeQCD && datasetName.find("QCD_") == 0){
			if(qcdHisto){
				histo = (TH1F*) qcdHisto->Clone();
				dataSet = new Dataset("QCD_Merged","QCD",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				qcdHisto = 0;
			}
			else continue;
		}
		if(mergeW && datasetName.find("W_") == 0){
			if(wHisto){
				histo = (TH1F*) wHisto->Clone();
				dataSet = new Dataset("WJets_Merged","W+jets",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				wHisto = 0;
			}
			else continue;
		}
		if(mergeZ && datasetName.find("Z_") == 0){
			if(zHisto){
				histo = (TH1F*) zHisto->Clone();
				dataSet = new Dataset("ZJets_Merged","Z+jets",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				zHisto = 0;
			}
			else continue;
		}
		if(mergeST && ( datasetName.find("ST_") == 0 || datasetName.find("SingleTop_") == 0 ) )
		{
			if(stHisto){
				histo = (TH1F*) stHisto->Clone();
				dataSet = new Dataset("ST_Merged","Single-Top",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				stHisto = 0;
			}
			else continue;
		}
		if(mergeVV && ( datasetName.find("WW") == 0 || datasetName.find("WZ") == 0 || datasetName.find("ZZ") == 0) )
		{
			if(vvHisto){
				histo = (TH1F*) vvHisto->Clone();
				dataSet = new Dataset("VV_Merged","VV",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				vvHisto = 0;
			}
			else continue;
		}
		if(mergeTTV && ( datasetName.find("ttZ") == 0 || datasetName.find("ttW") == 0 ) )
		{
			if(ttvHisto){
				histo = (TH1F*) ttvHisto->Clone();
				dataSet = new Dataset("ttV_Merged","ttV",true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				ttvHisto = 0;
			}
			else continue;
		}
	  
		int N = histo->GetNbinsX();
		histo->SetBinContent(N,histo->GetBinContent(N)+histo->GetBinContent(N+1));
		histo->SetBinContent(N+1,0);
		
		TH1F* clone = (TH1F*) histo->Clone();
		for(int j=0;j<Nbins_;j++) SummedBins[j] += clone->GetBinContent(j+1);
		clone->Scale(1/clone->Integral(1,clone->GetNbinsX()+1));
		//clone->SetFillColor(10);
		clone->SetFillStyle(1001);
		clone->GetYaxis()->SetTitle( ( "Normalized " + (string) histo->GetYaxis()->GetTitle() ).c_str() );
    
		histos.push_back(clone);
		datasets.push_back(dataSet);
		histos2.push_back(histo);
	}
  //cout<<"So far, so good : 3"<<endl;
  
	if ( dataPlotID != -1 ){
		//integralData = plots_[dataPlotID].first->Integral(1,plots_[dataPlotID].first->GetNbinsX());
		integralData = plots_[dataPlotID].first->Integral(0,plots_[dataPlotID].first->GetNbinsX()+1);
		for(unsigned int i=0;i<plots_.size();i++){
			if (dataPlotID != (int)i) integralMC += plots_[i].first->Integral(1,plots_[i].first->GetNbinsX());
		}
	}
  //cout<<"So far, so good : 4"<<endl;
  
	//Fill the first canvas
	string name;
	name = "Canvas_"+label;
	hCanvas_ = TCanvasCreator(histos, datasets, leg_, string("l"), name);

	//THStack	
	vector<TH1F*> histosForHStack;
	TH1F* addedMC = 0;
	vector<TH1F*> histosForHStackAreaNorm;
	TH1F* addedMCAreaNorm = 0;
	vector<Dataset*> datasetsForHStack;
	//Overlay
	vector<TH1F*> histosForOverlay; //overlaying (new physics) signal
	vector<TH1F*> histosForOverlayAreaNorm;
	vector<Dataset*> datasetsForOverlay;
	
	for(int i=histos2.size()-1;i>=0;i--) {
		if(datasets[i]->Name().find("Data")<datasets[i]->Name().size() || datasets[i]->Name().find("data")<datasets[i]->Name().size() || datasets[i]->Name().find("DATA")<datasets[i]->Name().size()) {
			AddDataHisto( histos2[i] );
		}
		else {
			TH1F* clone = (TH1F*) histos2[i]->Clone();			  
			clone->GetXaxis()->SetTitle(histos2[i]->GetXaxis()->GetTitle());
			clone->GetYaxis()->SetTitle(histos2[i]->GetYaxis()->GetTitle());
			if(datasets[i]->Name().find("NP_")!=0){
			   histosForHStack.push_back(clone);
         datasetsForHStack.push_back(datasets[i]);
         if( ! addedMC ) addedMC = (TH1F*) clone->Clone();			  
        else addedMC->Add( clone );
			}
			else if (datasets[i]->Name().find("NP_overlay_")==0){
			  histosForOverlay.push_back(clone);
			  datasetsForOverlay.push_back(datasets[i]);			
			}
			
			TH1F* cloneNorm = (TH1F*) histos2[i]->Clone();			  
			cloneNorm->GetXaxis()->SetTitle(histos2[i]->GetXaxis()->GetTitle());
			cloneNorm->GetYaxis()->SetTitle(histos2[i]->GetYaxis()->GetTitle());
			if (i != dataPlotID && dataPlotID != -1 && integralData > 0) {
			  cloneNorm->Scale(integralData/integralMC);
			}
			  
			if(datasets[i]->Name().find("NP_")!=0){
				histosForHStackAreaNorm.push_back(cloneNorm);
				if( ! addedMCAreaNorm ) addedMCAreaNorm = (TH1F*) cloneNorm->Clone();			  
        else addedMCAreaNorm->Add( cloneNorm );
			}
			else if (datasets[i]->Name().find("NP_overlay_")==0){
				histosForOverlayAreaNorm.push_back(cloneNorm);
			}
		}
	}
  //cout<<"So far, so good : 5"<<endl;
	
	//Fill the first canvas
	//hStack_ = THStackCreator(histos2, datasets, leg_);
	
//    L[id]->SetBorderSize(0);
    //L[id]->SetFillColor(0);
    //L[id]->SetLineColor(1);
//    L[id]->SetLineStyle(0);
//    L[id]->SetTextFont(42);
//    L[id]->SetFillStyle(0);
	
	leg_ = new TLegend(0.65,0.68,0.99,0.99);
	leg_->SetFillColor(0);
	leg_->SetTextFont(42);
	leg_->SetLineColor(1);
	leg_->SetLineWidth(1);
	leg_->SetLineStyle(0);
	leg_->SetBorderSize(1);
	if( ! showNumberEntries_ ) leg_->SetX1(0.76);
	leg2_ = new TLegend(0.65,0.68,0.99,0.99);
	leg2_->SetFillColor(0);
	leg2_->SetTextFont(42);
	leg2_->SetLineColor(1);
	leg2_->SetLineWidth(1);
	leg2_->SetLineStyle(1);
	leg2_->SetBorderSize(1);
	if( ! showNumberEntries_ ) leg2_->SetX1(0.76);

	if( histosForHStack.size() > 0 )
  	  hStack_ = THStackCreator(histosForHStack, datasetsForHStack, leg_, showNumberEntries_);
	name = "Stack_"+label;
	if(hStack_){
		hStack_->SetName(name.c_str());
		//hStack_->SetTitle(name.c_str());
	}

	if( histosForHStackAreaNorm.size() > 0 )
  	  hStackAreaNorm_ = THStackCreator(histosForHStackAreaNorm, datasetsForHStack, leg2_, showNumberEntries_);
	name = "StackAreaNorm_"+label;
	if(hStackAreaNorm_){
		hStackAreaNorm_->SetName(name.c_str());
		hStackAreaNorm_->SetTitle(name.c_str());
	}
  
	name = "CanvasStack_"+label;
	hCanvasStack_ = new TCanvas(name.c_str(), name.c_str(), 1000, 700);
	hCanvasStackLogY_ = new TCanvas((name+"_LogY").c_str(), (name+"_LogY").c_str(), 1000, 700);
	if(addRatio)
	{
	  hCanvasStack_->SetCanvasSize(1000, 800);
	  hCanvasStack_->SetBottomMargin(0.3);
	  hCanvasStackLogY_->SetCanvasSize(1000, 800);
	  hCanvasStackLogY_->SetBottomMargin(0.3);
  }
  //cout<<"So far, so good : 6"<<endl;
  
	stringstream s; s.precision(2); s << lumi_/1000;
	TLatex text;
  text.SetNDC(true);
  text.SetTextAlign(12);
  text.SetTextFont(42);
	text.SetTextSize(0.05);
	
	TLatex text2;
	text2.SetNDC(true);
	text2.SetTextAlign(12);
	text2.SetTextFont(42);
	text2.SetTextSize(0.075);
	text2.SetTextColor(kBlue);

	if(hStack_){
		hCanvasStack_->cd();
		hStack_->Draw("HIST");
    if(addRatio)
    {
      hStack_->GetXaxis()->SetLabelSize(0);
      hStack_->GetXaxis()->SetTitleSize(0);
    }
    //cout<<"So far, so good : 7"<<endl;

		hStack_->GetXaxis()->SetTitle(histosForHStack[0]->GetXaxis()->GetTitle());
		hStack_->GetYaxis()->SetTitle(histosForHStack[0]->GetYaxis()->GetTitle());
		hStack_->GetYaxis()->SetTitleOffset(1.1);
		if (hData_) text.DrawLatex(0.13,0.967,("CMS, "+s.str()+" fb^{-1} at #sqrt{s} = 8 TeV").c_str());
		if(!text_.IsNull()) text2.DrawLatex(0.5,0.86,text_);
		
		for(unsigned int i=0;i<histosForOverlay.size();i++){
		   char legTitle[100];
		   sprintf (legTitle, "%s (X %i)", ( datasetsForOverlay[i]->Title() ).c_str(), scaleNPSignal);
		   leg_->AddEntry(histosForOverlay[i],legTitle,"F");
		   histosForOverlay[i]->Scale(scaleNPSignal); //scale by factor scaleup...
		   if(i==0){		   
		     histosForOverlay[i]->SetLineStyle(1);
		     histosForOverlay[i]->SetLineColor(1);
		   }
		   else if(i==1){
		      histosForOverlay[i]->SetLineStyle(2);
		      histosForOverlay[i]->SetLineColor(1);
		   }
		   else if(i==2){
		      histosForOverlay[i]->SetLineStyle(1);
		      histosForOverlay[i]->SetLineColor(2);
		   }
		   else if(i==3){
		      histosForOverlay[i]->SetLineStyle(2);
		      histosForOverlay[i]->SetLineColor(2);
		   } 
		   else{
		      histosForOverlay[i]->SetLineStyle(3);
		      histosForOverlay[i]->SetLineColor(i); //before: 4+40 (light pink), but e.g. 4 (blue) should be more visible
		   }
		   histosForOverlay[i]->SetFillColor(0);
		   histosForOverlay[i]->SetLineWidth(3);
		   //histosForNoHStack[i]->GetYaxis()->SetTitle(histosForNoHStack[i]->GetYaxis()->GetTitle());
		   histosForOverlay[i]->Draw("HIST SAME");
		}
		//histo_errors->Draw("E3SAME");
    //cout<<"So far, so good : 8"<<endl;

		hCanvasStackLogY_->cd();
		hCanvasStackLogY_->SetLogy();
		hStack_->Draw("HIST");
		hStack_->GetXaxis()->SetTitle(histosForHStack[0]->GetXaxis()->GetTitle());
		hStack_->GetYaxis()->SetTitle(histosForHStack[0]->GetYaxis()->GetTitle());
  	if (hData_) text.DrawLatex(0.13,0.967,("CMS, "+s.str()+" fb^{-1} at #sqrt{s} = 8 TeV").c_str());
  	if(!text_.IsNull()) text2.DrawLatex(0.5,0.86,text_);
    for(unsigned int i=0;i<histosForOverlay.size();i++){
	    histosForOverlay[i]->Draw("HIST SAME");
		}
  }

	name = "CanvasStackAreaNorm_"+label;
	hCanvasStackAreaNorm_ = new TCanvas(name.c_str(), name.c_str(), 1000, 700);
	hCanvasStackAreaNormLogY_ = new TCanvas((name+"_LogY").c_str(), (name+"_LogY").c_str(), 1000, 700);
	if(addRatio)
	{
	  hCanvasStackAreaNorm_->SetCanvasSize(1000, 800);
	  hCanvasStackAreaNorm_->SetBottomMargin(0.3);
	  hCanvasStackAreaNormLogY_->SetCanvasSize(1000, 800);
    hCanvasStackAreaNormLogY_->SetBottomMargin(0.3);
  }
  //cout<<"So far, so good : 9"<<endl;

	if(hStackAreaNorm_){
		hCanvasStackAreaNorm_->cd();
		hStackAreaNorm_->Draw("HIST");
    if(addRatio)
    {
      hStackAreaNorm_->GetXaxis()->SetTitleSize(0);
      hStackAreaNorm_->GetXaxis()->SetLabelSize(0);
    }
		hStackAreaNorm_->GetXaxis()->SetTitle(histosForHStackAreaNorm[0]->GetXaxis()->GetTitle());
		hStackAreaNorm_->GetYaxis()->SetTitle(histosForHStackAreaNorm[0]->GetYaxis()->GetTitle());
		hStackAreaNorm_->GetYaxis()->SetTitleOffset(1.1);
		if (hData_) text.DrawLatex(0.13,0.967,("CMS, "+s.str()+" fb^{-1} at #sqrt{s} = 8 TeV").c_str());
		if(!text_.IsNull()) text2.DrawLatex(0.5,0.86,text_);
		for(unsigned int i=0;i<histosForOverlayAreaNorm.size();i++){
		   histosForOverlayAreaNorm[i]->Draw("HIST SAME");
		}
			
		hCanvasStackAreaNormLogY_->cd();
		hCanvasStackAreaNormLogY_->SetLogy();
		hStackAreaNorm_->Draw("HIST");
		hStackAreaNorm_->GetXaxis()->SetTitle(histosForHStackAreaNorm[0]->GetXaxis()->GetTitle());
		hStackAreaNorm_->GetYaxis()->SetTitle(histosForHStackAreaNorm[0]->GetYaxis()->GetTitle());
    if (hData_) text.DrawLatex(0.13,0.967,("CMS, "+s.str()+" fb^{-1} at #sqrt{s} = 8 TeV").c_str());
    if(!text_.IsNull()) text2.DrawLatex(0.5,0.86,text_);
		for(unsigned int i=0;i<histosForOverlayAreaNorm.size();i++){
		   histosForOverlayAreaNorm[i]->Draw("HIST SAME");
		}
	}
  //cout<<"So far, so good : 10"<<endl;

	float ymax = 0;
	if(hStack_) ymax = 1.3*hStack_->GetMaximum();
	float ymaxAreaNorm = 0;
	if(hStackAreaNorm_) ymaxAreaNorm = 1.3*hStackAreaNorm_->GetMaximum();

	if(hData_){
		hData_->SetLineColor(1);
//		hData_->SetLineWidth(2);
		hData_->SetMarkerStyle(20);
//		hData_->SetFillStyle(1001);
    TH1F* ratio = (TH1F*) hData_->Clone();
    if(hStack_)	ratio->Divide(addedMC);
//    ratio->SetMarkerStyle(20);
    ratio->SetMaximum( 1.5 );
    ratio->SetMinimum(0.5);
    ratio->GetXaxis()->SetLabelSize(0.04);
    ratio->GetYaxis()->SetLabelSize(0.04);
//    ratio->GetXaxis()->SetTitle(stack->GetXaxis()->GetTitle());
    ratio->GetYaxis()->SetTitle("Data/MC");
    ratio->GetYaxis()->SetTitleSize(0.045);
    ratio->GetYaxis()->SetTitleOffset(1.15);
    ratio->SetMarkerSize(1.);
    ratio->GetYaxis()->SetNdivisions(5);

    //cout<<"So far, so good : 11"<<endl;
    
    TPad* pad = new TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
    pad->SetTopMargin(0.7);
    pad->SetFillColor(1);
    pad->SetFillStyle(1);
    pad->SetGridy(1);
    TPad* padLogY = (TPad*) pad->Clone();
    
		hCanvasStack_->cd();
		if(hStack_)	
	  {
	    hData_->Draw("same E");
//      cout<<"So far, so good : 11-1"<<endl;
	    if(addRatio)
	    {
        pad->Draw();
        pad->cd(0);
        ratio->Draw("e");
//        cout<<"So far, so good : 11-2"<<endl;
        if(hErrorPlus_)
        {
          hErrorPlus_->Draw("C");
          hErrorMinus_->Draw("C");
        }
      }
//      cout<<"So far, so good : 11-3"<<endl;
    }
		else hData_->Draw("E");
		hCanvasStackLogY_->cd();
		hCanvasStackLogY_->SetLogy();
		if(hStack_)
		{
		  hData_->Draw("same E");
		  if(addRatio)
		  {
        padLogY->Draw();
        padLogY->cd(0);
        ratio->Draw("e");
        if(hErrorPlus_)
        {
          hErrorPlus_->Draw("C");
          hErrorMinus_->Draw("C");
        }
      }
    }
		else hData_->Draw("E");
//    cout<<"So far, so good : 12"<<endl;
		
    TH1F* ratioAreaNorm = (TH1F*) hData_->Clone();
    if(hStackAreaNorm_)	ratioAreaNorm->Divide(addedMCAreaNorm);
    ratioAreaNorm->SetMarkerStyle(20);
    ratioAreaNorm->SetMaximum( 1.5 );
    ratioAreaNorm->SetMinimum(0.5);
    ratioAreaNorm->GetXaxis()->SetLabelSize(0.04);
    ratioAreaNorm->GetYaxis()->SetLabelSize(0.04);
//    ratioAreaNorm->GetXaxis()->SetTitle(stack->GetXaxis()->GetTitle());
    ratioAreaNorm->GetYaxis()->SetTitle("Data/MC");
    ratioAreaNorm->GetYaxis()->SetTitleSize(0.045);
    ratioAreaNorm->GetYaxis()->SetTitleOffset(1.15);
    ratioAreaNorm->SetMarkerSize(1.);
    ratioAreaNorm->GetYaxis()->SetNdivisions(5);
    
    TPad* padAreaNorm = new TPad("padAreaNorm", "padAreaNorm", 0.0, 0.0, 1.0, 1.0);
    padAreaNorm->SetTopMargin(0.7);
    padAreaNorm->SetFillColor(0);
    padAreaNorm->SetFillStyle(0);
    padAreaNorm->SetGridy(1);
    TPad* padAreaNormLogY = (TPad*) padAreaNorm->Clone();
		
		hCanvasStackAreaNorm_->cd();
		if(hStackAreaNorm_)
		{
		  hData_->Draw("same E");
		  if(addRatio)
		  {
        padAreaNorm->Draw();
        padAreaNorm->cd(0);
        ratioAreaNorm->Draw("e");
        if(hErrorPlus_)
        {
          hErrorPlus_->Draw("C");
          hErrorMinus_->Draw("C");
        }
      }
		}
		else hData_->Draw("E");
		hCanvasStackAreaNormLogY_->cd();
		hCanvasStackAreaNormLogY_->SetLogy();
		if(hStackAreaNorm_)
		{
		  hData_->Draw("same E");
		  if(addRatio)
		  {
  		  padAreaNormLogY->Draw();
        padAreaNormLogY->cd(0);
        ratioAreaNorm->Draw("e");
        if(hErrorPlus_)
        {
          hErrorPlus_->Draw("C");
          hErrorMinus_->Draw("C");
        }
      }
		}
		else hData_->Draw("E");
		char legDataTitle[100];
		if (showNumberEntries_){
			sprintf (legDataTitle, "%s (%.1f entries)", hData_->GetTitle(), hData_->Integral(0,hData_->GetNbinsX()+1) );
		}
		else
			sprintf (legDataTitle, "%s", hData_->GetTitle());
		leg_->AddEntry(hData_, legDataTitle ,"L E");
		leg2_->AddEntry(hData_, legDataTitle ,"L E");
		if(hStack_ && ymax < 1.3*(hData_->GetMaximum())) ymax = 1.3*(hData_->GetMaximum());
		if(hStackAreaNorm_ && ymaxAreaNorm < 1.3*(hData_->GetMaximum())) ymaxAreaNorm = 1.3*(hData_->GetMaximum());
	}
//  cout<<"So far, so good : 13"<<endl;
	if(addRandomPseudoData){
		ROOT::Math::Random<ROOT::Math::GSLRngMT> *rand = new ROOT::Math::Random<ROOT::Math::GSLRngMT>();
		for(int i=0;i<Nbins_;i++) hRandomPseudoData_->SetBinContent(i+1,rand->Poisson(SummedBins[i]));
		hRandomPseudoData_->SetLineColor(1);
		hRandomPseudoData_->SetLineWidth(2);
		hRandomPseudoData_->SetFillStyle(1001);
		hCanvasStack_->cd(); 	               
		hRandomPseudoData_->Draw("same E"); 	 
		hCanvasStackLogY_->cd(); 	 
		hRandomPseudoData_->Draw("same E");
		leg_->AddEntry(hRandomPseudoData_,"Pseudo-data","l");
		if(ymax < 1.3*(hRandomPseudoData_->GetMaximum())) ymax = 1.3*(hRandomPseudoData_->GetMaximum());
		delete rand;
	}
	if(maxY_ > 0)
	{
	  ymax = maxY_;
	  ymaxAreaNorm = maxY_;
	}
	if(hStack_)	hStack_->SetMaximum(ymax);
	if(hStackAreaNorm_) hStackAreaNorm_->SetMaximum(ymaxAreaNorm);

	hCanvasStack_->cd();
	leg_->Draw();
	hCanvasStackLogY_->cd();
	leg_->Draw();
	hCanvasStackAreaNorm_->cd();			
	leg2_->Draw();
	hCanvasStackAreaNormLogY_->cd();			
	leg2_->Draw();

	delete SummedBins;
}

void MultiSamplePlot::Write(TFile* fout, string label, bool savePNG, string pathPNG, string ext) {
	fout->cd();
	string dirname = "MultiSamplePlot_"+label;
	if(fout->Get(dirname.c_str())==0)
		fout->mkdir(dirname.c_str());
	fout->cd(dirname.c_str());
  
	if(hStack_) hStack_->Write();
	if(hStackAreaNorm_) hStackAreaNorm_->Write();
	if(hRandomPseudoData_) hRandomPseudoData_->Write();
	if(hData_) hData_->Write();
  
	if(hCanvas_) {
	  if(savePNG)
	  {
	    hCanvas_->SaveAs( (pathPNG+label+"_Normalized."+ext).c_str() );
//	    hCanvas_->SaveAs( (pathPNG+label+"_Normalized.pdf").c_str() );
	  }
	  hCanvas_->Write();
	}
  
	if(hCanvasStack_) {
	  if(hStack_) hStack_->SetMinimum(0.001);
	  hCanvasStack_->Write();
	  if(savePNG)
	  {
	    hCanvasStack_->SaveAs( (pathPNG+label+"_Stack."+ext).c_str() );
//	    hCanvasStack_->SaveAs( (pathPNG+label+"_Stack.pdf").c_str() );
	  }
	}
  
	if(hCanvasStackLogY_) {
	  if(hStack_) hStack_->SetMinimum(1.);
	  hCanvasStackLogY_->Write();
	  if(savePNG)
	  {
	    hCanvasStackLogY_->SaveAs( (pathPNG+label+"_StackLogY."+ext).c_str() );
//	    hCanvasStackLogY_->SaveAs( (pathPNG+label+"_StackLogY.pdf").c_str() );
	  }
	}
  
	if(hCanvasStackAreaNorm_) {
	  if(hStackAreaNorm_) hStackAreaNorm_->SetMinimum(0.001);
	  hCanvasStackAreaNorm_->Write();
	  if(savePNG)
	  {
	    hCanvasStackAreaNorm_->SaveAs( (pathPNG+label+"_StackAreaNorm."+ext).c_str() );
//	    hCanvasStackAreaNorm_->SaveAs( (pathPNG+label+"_StackAreaNorm.pdf").c_str() );
	  }
	}
  
	if(hCanvasStackAreaNormLogY_) {
	  if(hStackAreaNorm_) hStackAreaNorm_->SetMinimum(1.);
	  hCanvasStackAreaNormLogY_->Write();
	  if(savePNG)
	  {
	    hCanvasStackAreaNormLogY_->SaveAs( (pathPNG+label+"_StackAreaNormLogY."+ext).c_str() );
//	    hCanvasStackAreaNormLogY_->SaveAs( (pathPNG+label+"_StackAreaNormLogY.pdf").c_str() );
	  }
	}
  
	for(unsigned int i=0; i<plots_.size(); i++)
	  if(plots_[i].second->Name().find("data") != 0 && plots_[i].second->Name().find("Data") != 0 && plots_[i].second->Name().find("DATA") != 0 )
      plots_[i].first->Write();
}

