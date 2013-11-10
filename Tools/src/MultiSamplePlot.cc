#include "../interface/MultiSamplePlot.h"

MultiSamplePlot::MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float Min, float Max, string XaxisLabel, string YaxisLabel, string Text)
{
	Nbins_ = Nbins;
	string histoName = "";
	string histoTitle = "";
	plotName_ = PlotName;
	text_ = TString(Text);
	lumi_ = 1.;
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
	Initialize();
}

MultiSamplePlot::MultiSamplePlot(vector<Dataset*> datasets, string PlotName, int Nbins, float* binsX, string XaxisLabel, string YaxisLabel, string Text)
{
	Nbins_ = Nbins;
	string histoName = "";
	string histoTitle = "";
	plotName_ = PlotName;
	text_ = TString(Text);
	lumi_ = 1.;
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
	Initialize();
}

MultiSamplePlot::MultiSamplePlot(vector<pair<TH1F*,Dataset*> > vec, string PlotName, string XaxisLabel, string YaxisLabel, string Text)
{
	plots_ = vec;
	Nbins_ = (vec[0].first)->GetNbinsX();
	plotName_ = PlotName;
	text_ = TString(Text);
	lumi_ = 1.;
	for(unsigned int i=0;i<vec.size();i++)
	{
	  if(vec[i].second->Name().find("data") == 0 || vec[i].second->Name().find("Data") == 0 || vec[i].second->Name().find("DATA") == 0 )
	    lumi_ = vec[i].second->EquivalentLumi();
		(vec[i].first)->GetXaxis()->SetTitle(XaxisLabel.c_str());
		(vec[i].first)->GetYaxis()->SetTitle(YaxisLabel.c_str());
  }
	Initialize();
}

void MultiSamplePlot::Initialize()
{
	hData_ = 0;
	hCanvas_ = 0;
	hCanvasStack_ = 0;
	hCanvasStackLogY_ = 0;
	hCanvasStackAreaNorm_ = 0;
	hCanvasStackAreaNormLogY_ = 0;
	hStack_ = 0;
 	hStackAreaNorm_ = 0;
	StackErrorGraph_ = 0;
	RatioErrorGraph_ = 0;
	maxY_ = -1;
	minLogY_ = 1.;
	showNumberEntries_ = true;
	errorbandfile_ = "";
	sqrts_ = 8;
	prelim_=true;
}

MultiSamplePlot::~MultiSamplePlot()
{
	if(hStack_)      delete hStack_;
	if(hStackAreaNorm_)      delete hStackAreaNorm_;
	if(hCanvas_)     delete hCanvas_;
	if(hCanvasStack_)     delete hCanvasStack_;
	if(hCanvasStackAreaNorm_)     delete hCanvasStackAreaNorm_;
	if(hCanvasStackLogY_)     delete hCanvasStackLogY_;
	if(hCanvasStackAreaNormLogY_)     delete hCanvasStackAreaNormLogY_;
	if(hData_) delete hData_;	
	if(StackErrorGraph_) delete StackErrorGraph_;
	if(RatioErrorGraph_) delete RatioErrorGraph_;
}

void MultiSamplePlot::AddDataHisto(TH1F* histo)
{
	hData_ = (TH1F*) histo->Clone();
}

void MultiSamplePlot::Fill(float value, Dataset* data, bool scale, float Lumi)
{
	if(scale)
	{
		for(unsigned int i=0;i<plots_.size();i++)
		{
			if(data->Name()==plots_[i].second->Name()) plots_[i].first->Fill(value, data->NormFactor()*Lumi);
//			if(data->Name()==plots_[i].second->Name()) cout << "NormFactor for " << data->Name() << " = " <<  data->NormFactor()*Lumi << endl;
		}
	}
	else
	{
		for(unsigned int i=0;i<plots_.size();i++)
		{
			if(data->Name()==plots_[i].second->Name()) plots_[i].first->Fill(value);
		}
	}
}

void MultiSamplePlot::Draw(string label, unsigned int RatioType, bool addRatioErrorBand, bool addErrorBand, bool ErrorBandAroundTotalInput, int scaleNPSignal)
{
	vector<TH1F*> normalizedhistos;
	vector<Dataset*> mergeddatasets;
	
  if(RatioType==0 && addRatioErrorBand)
	{
		cout<<"[MultiSamplePlot::Draw] WARNING: addRatioErrorBand set to true but RatioType to 0 (i.e. no ratio) -> resetting addRatioErrorBand to false now."<<endl;
		addRatioErrorBand = false;
	}	
	
	TFile* tmpInfile = 0;
	if(addRatioErrorBand || addErrorBand)
	{
	   tmpInfile = new TFile(errorbandfile_.c_str(),"READ");
		 if(tmpInfile->IsZombie())
		 {
		    cout<<"[MultiSamplePlot::Draw] WARNING: '"<<errorbandfile_<<"' is no valid error band file -> will not draw error bands. Please set a valid file with the MultiSamplePlot::setErrorBandFile method."<<endl;
		    addErrorBand = false;
		    addRatioErrorBand = false;
		 }
	}
	
	TH1F* hErrorPlus = 0;
	TH1F* hErrorMinus = 0;
	TH1F* hNominal = 0;
  if(addRatioErrorBand || addErrorBand)
	{
		   TDirectory* subdir = (TDirectory*) tmpInfile->Get(("MultiSamplePlot_"+label).c_str());
			 if(subdir)
			 {
			    hErrorPlus = (TH1F*) subdir->Get("Plus")->Clone();
			    hErrorMinus = (TH1F*) subdir->Get("Minus")->Clone();
					hNominal = (TH1F*) subdir->Get("Nominal")->Clone();
					//cout << "Integrals for plot " << label << " ->   hErrorPlus: " << hErrorPlus->Integral(1,hErrorPlus->GetNbinsX()) << ",  hNominal: " << hNominal->Integral(1,hNominal->GetNbinsX()) << ",  hErrorMinus: " << hErrorMinus->Integral(1,hErrorMinus->GetNbinsX()) << endl;
			 }
			 else
			 {
			    cout << "[MultiSamplePlot::Draw] ERROR: errorband file '"<<errorbandfile_<<"' has unknown structure! (Histograms should be in directories with MultiSamplePlot names) -> will not draw error bands." <<endl;
			    addErrorBand = false;
		      addRatioErrorBand = false;
			 }
  }
	
		
  //loop over the processes and create one new histogram per set of processes to merge. Whether or not processes will be merged, will be based on their title in the xml config; same title -> same merged histogram.
	int dataPlotID=-1;
	unsigned int nDataPlots = 0;
	vector<pair<TH1F*,Dataset*> > SetsOfMergedProcesses; //the dataset will correspond to a set of merged processes (e.g. "TTbar+jets", "W+jets", ...)
	for(unsigned int i=0;i<plots_.size();i++)
	{
	  if(plots_[i].second->Name().find("data") == 0 || plots_[i].second->Name().find("Data") == 0 || plots_[i].second->Name().find("DATA") == 0 )
		{
	    dataPlotID = i;
			nDataPlots++;
		}

		string datasetTitle = plots_[i].second->Title();
	  bool histocreated = false;
		//check if the histogram corresponding to this title is already created
		for(unsigned int j=0;j<SetsOfMergedProcesses.size();j++)
	  {
			   if(SetsOfMergedProcesses[j].second->Title() == datasetTitle) histocreated = true;			
		}
		
		(plots_[i].first)->SetLineColor((plots_[i].second)->Color()); //especially relevant for the overlay processes, the stacked histograms get their fill color later on. However, all seperate processes are also saved in MSPlot, so these will get the color specified in the xml too.
    (plots_[i].first)->SetMarkerColor((plots_[i].second)->Color());
		(plots_[i].first)->SetLineStyle((plots_[i].second)->LineStyle());
		(plots_[i].first)->SetLineWidth((plots_[i].second)->LineWidth());
			
		if(!histocreated)
		{
		    TH1F* MergedProcesses = (TH1F*) plots_[i].first->Clone();
				string datasetName = plots_[i].second->Title()+"_Merged";
				if(plots_[i].second->Name().find("NP_overlay_")==0) datasetName = "NP_overlay_"+datasetName;
				else if(plots_[i].second->Name().find("NP_")==0) datasetName = "NP_"+datasetName;
				Dataset* dataSet = new Dataset(datasetName,datasetTitle,true,plots_[i].second->Color(),plots_[i].second->LineStyle(),plots_[i].second->LineWidth(),1.,1.);
				//making sure that the overflow is transferred to the last 'visible' bin; analogously for underflow...
		    MergedProcesses->SetBinContent(Nbins_,MergedProcesses->GetBinContent(Nbins_)+MergedProcesses->GetBinContent(Nbins_+1));
		    MergedProcesses->SetBinContent(Nbins_+1,0);
				MergedProcesses->SetBinContent(1,MergedProcesses->GetBinContent(0)+MergedProcesses->GetBinContent(1));
		    MergedProcesses->SetBinContent(0,0);
				MergedProcesses->GetXaxis()->SetTitle(plots_[i].first->GetXaxis()->GetTitle());
				MergedProcesses->GetYaxis()->SetTitle(plots_[i].first->GetYaxis()->GetTitle());
			  SetsOfMergedProcesses.push_back(pair<TH1F*,Dataset*>(MergedProcesses,dataSet));
		}
		else
		{		
			  //add the process to the histogram corresponding to its title
			  for(unsigned int j=0;j<SetsOfMergedProcesses.size();j++)
	      {
			    if(SetsOfMergedProcesses[j].second->Title() == datasetTitle)
				  {
				    SetsOfMergedProcesses[j].first->Add(plots_[i].first);				 
				  }		
			  }			
		}
	}
	
	if(nDataPlots>1) cout<<"[MultiSamplePlot::Draw] ERROR: you are not allowed to superimpose two data histograms on the same plot!"<<endl;
	else if(nDataPlots==0)
	{
	   //cout<<"[MultiSamplePlot::Draw] WARNING: not running on data, ratio plots will not be drawn
	   RatioType=0;
		 addRatioErrorBand = false;
	}

	//NOTE: in this stage, the SetsOfMergedProcesses vector is basically the same as the plots_ vector, but with the processes merged that have the same title in the xml config.
	for(unsigned int i=0;i<SetsOfMergedProcesses.size();i++)
	{
	  TH1F* histo = (TH1F*) SetsOfMergedProcesses[i].first->Clone();
		Dataset* mergeddataSet = SetsOfMergedProcesses[i].second;		
		TH1F* normalizedclone = (TH1F*) histo->Clone();
		normalizedclone->Scale(1/normalizedclone->Integral(1,normalizedclone->GetNbinsX()+1));
		//normalizedclone->SetFillColor(10);
		normalizedclone->SetFillStyle(1001);
		normalizedclone->GetYaxis()->SetTitle( ( "Normalized " + (string) histo->GetYaxis()->GetTitle() ).c_str() );    
		normalizedhistos.push_back(normalizedclone);
		mergeddatasets.push_back(mergeddataSet);		
	}
	
	//Make the first canvas; to plot distributions normalized to unity
	string name = "Canvas_"+label;
	hCanvas_ = TCanvasCreator(normalizedhistos, mergeddatasets, leg_, string("l"), name);


	//calculating data and MC integrals
	float integralData = 0;
	float integralSM = 0;
	if ( dataPlotID != -1 )
	{
		integralData = plots_[dataPlotID].first->Integral(0,plots_[dataPlotID].first->GetNbinsX()+1); //first argument 0 means you include the underflow bin too
		for(unsigned int i=0;i<plots_.size();i++)
		{
			if ((dataPlotID != (int)i) && (plots_[i].second->Name().find("NP_")!=0))
			   integralSM += plots_[i].first->Integral(0,plots_[i].first->GetNbinsX());
		}
	}
	

  //For histograms to be stacked
	vector<TH1F*> histosForHStack, histosForHStackAreaNorm;
	TH1F* totalSM = 0;
	TH1F* totalSMAreaNorm = 0;
	vector<Dataset*> datasetsForHStack;
  //For histograms (e.g. new-physics signal) to be overlayed
	vector<TH1F*> histosForOverlay, histosForOverlayAreaNorm;
	vector<Dataset*> datasetsForOverlay;	
	
	//Loop from back to front because of the way THStack is used later on (related to order as appearing in xml config compared to order in histogram stack)...
	for(int i=SetsOfMergedProcesses.size()-1;i>=0;i--)
	{
		Dataset* mergeddataSet = SetsOfMergedProcesses[i].second;
	  if(SetsOfMergedProcesses[i].second->Name().find("data") == 0 || SetsOfMergedProcesses[i].second->Name().find("Data") == 0 || SetsOfMergedProcesses[i].second->Name().find("DATA") == 0 )
		{
       AddDataHisto( SetsOfMergedProcesses[i].first );
		}
		else
		{
		   TH1F* clone = (TH1F*) SetsOfMergedProcesses[i].first->Clone();
			 if(mergeddataSet->Name().find("NP_")!=0)
			 {
			    histosForHStack.push_back(clone);
          datasetsForHStack.push_back(mergeddataSet);
          if( ! totalSM ) totalSM = (TH1F*) clone->Clone();			  
          else totalSM->Add( clone );
			 }
			 else if (mergeddatasets[i]->Name().find("NP_overlay_")==0)
			 {
			    histosForOverlay.push_back(clone);
			    datasetsForOverlay.push_back(mergeddatasets[i]);			
			 }
			 
			 TH1F* cloneAreaNorm = (TH1F*) SetsOfMergedProcesses[i].first->Clone();			  
			 if (dataPlotID != -1 && integralData > 0)
			 {
			   cloneAreaNorm->Scale(integralData/integralSM);
			 }			  
			 if(mergeddataSet->Name().find("NP_")!=0)
			 {
				 histosForHStackAreaNorm.push_back(cloneAreaNorm);
				 if( ! totalSMAreaNorm ) totalSMAreaNorm = (TH1F*) cloneAreaNorm->Clone();			  
         else totalSMAreaNorm->Add( cloneAreaNorm );
			 }
			 else if (mergeddataSet->Name().find("NP_overlay_")==0)
			 {
				 histosForOverlayAreaNorm.push_back(cloneAreaNorm);
			 }	 				
		}
	}


	leg_ = new TLegend(0.45,0.63,0.97,0.94);
	leg_->SetFillColor(0);
	leg_->SetTextFont(42);
	leg_->SetLineColor(1);
	leg_->SetLineWidth(1);
	leg_->SetLineStyle(0);
	leg_->SetBorderSize(1);	
	if( ! showNumberEntries_ ) leg_->SetX1(0.76);
	
	//a second legend is needed because otherwise it will add the datasets two times to the same legend...
	legAreaNorm_ = new TLegend(0.45,0.63,0.97,0.94);
	legAreaNorm_->SetFillColor(0);
	legAreaNorm_->SetTextFont(42);
	legAreaNorm_->SetLineColor(1);
	legAreaNorm_->SetLineWidth(1);
	legAreaNorm_->SetLineStyle(0);
	legAreaNorm_->SetBorderSize(1);	
	if( ! showNumberEntries_ ) legAreaNorm_->SetX1(0.76);

  //if running over data, add this one first in the legend
  if(hData_)
	{
		char legDataTitle[100];
		//hData_->SetTitle("Observed");
		hData_->SetTitle("Data");
		if (showNumberEntries_)
			sprintf (legDataTitle, "%s (%.1f entries)", hData_->GetTitle(), hData_->Integral(0,hData_->GetNbinsX()+1) );
		else
			sprintf (legDataTitle, "%s", hData_->GetTitle());
		leg_->AddEntry(hData_, legDataTitle ,"L E P");
		legAreaNorm_->AddEntry(hData_, legDataTitle ,"L E P");
	}


	if( histosForHStack.size() > 0 )
  	  hStack_ = THStackCreator(histosForHStack, datasetsForHStack, leg_, showNumberEntries_);
	name = "Stack_"+label;
	if(hStack_)
	{
		hStack_->SetName(name.c_str());
		hStack_->SetTitle(""); //hStackAreaNorm_->SetTitle(name.c_str()) gives problem: title and the CMS text get mixed on the canvas...
	}

	if( histosForHStackAreaNorm.size() > 0 )
  	  hStackAreaNorm_ = THStackCreator(histosForHStackAreaNorm, datasetsForHStack, legAreaNorm_, showNumberEntries_);
	name = "StackAreaNorm_"+label;
	if(hStackAreaNorm_)
	{
		hStackAreaNorm_->SetName(name.c_str());
		hStackAreaNorm_->SetTitle(""); //hStackAreaNorm_->SetTitle(name.c_str()) gives problem: title and the CMS text get mixed on the canvas...
	}
	
	
	//just adding the overlay histograms to the legend and calculating if we have to change the y-axis range
	float ymaxoverlay = 0, ymaxoverlayAreaNorm = 0;
  for(unsigned int i=0;i<histosForOverlay.size();i++)
	{
		 char legTitle[100];
		 if (showNumberEntries_)
		 {
		    float scalednentries = scaleNPSignal * histosForOverlay[i]->Integral(0,histosForOverlay[i]->GetNbinsX()+1);
		    sprintf (legTitle, "%s (%.1f entries) (X %i)", ( datasetsForOverlay[i]->Title() ).c_str(), scalednentries, scaleNPSignal);
		 }
		 else
		    sprintf (legTitle, "%s (X %i)", ( datasetsForOverlay[i]->Title() ).c_str(), scaleNPSignal);
		 leg_->AddEntry(histosForOverlay[i],legTitle,"F");
		 legAreaNorm_->AddEntry(histosForOverlay[i],legTitle,"F");
		 if(histosForOverlay[i]->GetMaximum()*scaleNPSignal > ymaxoverlay) ymaxoverlay = histosForOverlay[i]->GetMaximum()*scaleNPSignal;
		 if(histosForOverlayAreaNorm[i]->GetMaximum() > ymaxoverlayAreaNorm) ymaxoverlayAreaNorm = histosForOverlayAreaNorm[i]->GetMaximum()*scaleNPSignal;
	}


  //regular 'luminosity normalized'
  name = "CanvasStack_"+label;
	hCanvasStack_ = new TCanvas(name.c_str(), name.c_str(), 1000, 700);
	hCanvasStackLogY_ = new TCanvas((name+"_LogY").c_str(), (name+"_LogY").c_str(), 1000, 700);
	float ymax = 0;
	if(hStack_)
	{
	   if(RatioType>0)
	   {
	     hCanvasStack_->SetCanvasSize(1000, 800);
	     hCanvasStack_->SetBottomMargin(0.3);
	     hCanvasStackLogY_->SetCanvasSize(1000, 800);
	     hCanvasStackLogY_->SetBottomMargin(0.3);
     }
		
		 DrawStackedPlot(hCanvasStack_,hCanvasStackLogY_,hStack_,histosForOverlay,scaleNPSignal,histosForHStack[0]->GetXaxis()->GetTitle(),histosForHStack[0]->GetYaxis()->GetTitle(),RatioType);	
		 ymax = 1.3*hStack_->GetMaximum();
		 if(ymaxoverlay > ymax) ymax = ymaxoverlay;
	}
	
  //'area normalized'
	name = "CanvasStackAreaNorm_"+label;
	hCanvasStackAreaNorm_ = new TCanvas(name.c_str(), name.c_str(), 1000, 700);
	hCanvasStackAreaNormLogY_ = new TCanvas((name+"_LogY").c_str(), (name+"_LogY").c_str(), 1000, 700);
	float ymaxAreaNorm = 0;
	if(hStackAreaNorm_)
	{
	  if(RatioType>0)
	  {	
	    hCanvasStackAreaNorm_->SetCanvasSize(1000, 800);
	    hCanvasStackAreaNorm_->SetBottomMargin(0.3);
	    hCanvasStackAreaNormLogY_->SetCanvasSize(1000, 800);
      hCanvasStackAreaNormLogY_->SetBottomMargin(0.3);
    }
		
	  DrawStackedPlot(hCanvasStackAreaNorm_,hCanvasStackAreaNormLogY_,hStackAreaNorm_,histosForOverlayAreaNorm,scaleNPSignal,histosForHStackAreaNorm[0]->GetXaxis()->GetTitle(),histosForHStackAreaNorm[0]->GetYaxis()->GetTitle(),RatioType);
		ymaxAreaNorm = 1.3*hStackAreaNorm_->GetMaximum();
		if(ymaxoverlayAreaNorm > ymaxAreaNorm) ymaxAreaNorm = ymaxoverlayAreaNorm;
	}


  //Now superimpose data to the plots + draw ratio plots + draw error bands
	TH1F* ratio = 0;
	TPad* pad = 0;
	TPad* padLogY = 0;
	if(hData_)
	{
		hData_->SetLineColor(1);
		hData_->SetMarkerStyle(20);
					
    ratio = (TH1F*) hData_->Clone();
    if(hStack_)
		{
		   if(RatioType==1)
			 {
			    ratio->Divide(totalSM); //ratio = data/MC
			    ratio->GetYaxis()->SetTitle("Data/MC");
					ratio->SetMaximum(1.5);
          ratio->SetMinimum(0.5);
			 }
			 else if(RatioType==2)
			 {
			    ratio->Add(totalSM,-1);
			    ratio->Divide(totalSM); //ratio = (data-MC)/MC
					ratio->GetYaxis()->SetTitle("(Data-MC)/MC");
					ratio->SetMaximum(0.5);
          ratio->SetMinimum(-0.5);					
			 }
		}
    ratio->SetMarkerStyle(20);
    ratio->GetXaxis()->SetLabelSize(0.04);
    ratio->GetYaxis()->SetLabelSize(0.04);
    ratio->GetYaxis()->SetTitleSize(0.045);
    ratio->GetYaxis()->SetTitleOffset(1.15);
    ratio->SetMarkerSize(1.);
    ratio->GetYaxis()->SetNdivisions(5);    
		ratio->SetTitle("");
		
    pad = new TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
    pad->SetTopMargin(0.7);
    pad->SetFillColor(0); // was 1 but could have been related to the problem of black canvases
    pad->SetFillStyle(0); // was 1 but could have been related to the problem of black canvases
    pad->SetGridy(1);
    padLogY = (TPad*) pad->Clone();
  } 
		
	hCanvasStack_->cd();
	if(hStack_)	
	{		  
			if(addErrorBand)
				DrawErrorBand(totalSM,hErrorPlus,hErrorMinus,hNominal,StackErrorGraph_,ErrorBandAroundTotalInput);
			
			if(hData_)
			{
			  hData_->Draw("same E");		
						
	      if(RatioType>0)
	      {   			  
          pad->Draw();
          pad->cd(0);
          ratio->Draw("e");
          if(addRatioErrorBand)
          {           
					   TH1F* totalSMRatio = (TH1F*) totalSM->Clone();
					   TH1F* hErrorPlusRatio = (TH1F*) hErrorPlus->Clone();
					   TH1F* hErrorMinusRatio = (TH1F*) hErrorMinus->Clone();
					   TH1F* hNominalRatio = (TH1F*) hNominal->Clone();
					   if(RatioType==1)
					   {
					      totalSMRatio->Divide(totalSM);
							  hErrorPlusRatio->Divide(totalSM);
							  hErrorMinusRatio->Divide(totalSM);
					      hNominalRatio->Divide(totalSM);
					   }
					   else if(RatioType==2)
			       {
			          totalSMRatio->Add(totalSM,-1);
			          totalSMRatio->Divide(totalSM);
							  hErrorPlusRatio->Add(totalSM,-1);
							  hErrorPlusRatio->Divide(totalSM);
							  hErrorMinusRatio->Add(totalSM,-1);
							  hErrorMinusRatio->Divide(totalSM);
							  hNominalRatio->Add(totalSM,-1);
							  hNominalRatio->Divide(totalSM);
			       }
					   DrawErrorBand(totalSMRatio,hErrorPlusRatio,hErrorMinusRatio,hNominalRatio,RatioErrorGraph_,ErrorBandAroundTotalInput);
          }
				}
      }
  }
  else
	{
	   if(hData_) hData_->Draw("E");
	}
		
		
	//log-scale plots
	hCanvasStackLogY_->cd();
	hCanvasStackLogY_->SetLogy();
	if(hStack_)
	{
		  if(addErrorBand)
				DrawErrorBand(totalSM,hErrorPlus,hErrorMinus,hNominal,StackErrorGraph_,ErrorBandAroundTotalInput);
			
			if(hData_)
			{				
			  hData_->Draw("same E");
		  
			  if(RatioType>0)
		    {		  
          padLogY->Draw();
          padLogY->cd(0);
          ratio->Draw("e");
          if(addRatioErrorBand)
          {
             TH1F* totalSMRatio = (TH1F*) totalSM->Clone();
					   TH1F* hErrorPlusRatio = (TH1F*) hErrorPlus->Clone();
					   TH1F* hErrorMinusRatio = (TH1F*) hErrorMinus->Clone();
					   TH1F* hNominalRatio = (TH1F*) hNominal->Clone();
					   if(RatioType==1)
					   {
					      totalSMRatio->Divide(totalSM);
						  	hErrorPlusRatio->Divide(totalSM);
							  hErrorMinusRatio->Divide(totalSM);
					      hNominalRatio->Divide(totalSM);
					   }
					   else if(RatioType==2)
			       {
			          totalSMRatio->Add(totalSM,-1);
			          totalSMRatio->Divide(totalSM);
							  hErrorPlusRatio->Add(totalSM,-1);
							  hErrorPlusRatio->Divide(totalSM);
							  hErrorMinusRatio->Add(totalSM,-1);
						  	hErrorMinusRatio->Divide(totalSM);
							  hNominalRatio->Add(totalSM,-1);
						  	hNominalRatio->Divide(totalSM);
			       }
					   DrawErrorBand(totalSMRatio,hErrorPlusRatio,hErrorMinusRatio,hNominalRatio,RatioErrorGraph_,ErrorBandAroundTotalInput);
          }
        }
			}			
  }
	else
	{
		  if(hData_) hData_->Draw("E");
	}
			
	//Now area-normalized plots...
	TH1F* ratioAreaNorm = 0;
	TPad* padAreaNorm = 0;
	TPad* padAreaNormLogY = 0;
	if(hData_)
	{
		  ratioAreaNorm = (TH1F*) hData_->Clone();
      if(hStackAreaNorm_)
		  {
		     if(RatioType==1)
			   {
			      ratioAreaNorm->Divide(totalSMAreaNorm); //ratio = data/MC
					  ratioAreaNorm->GetYaxis()->SetTitle("Data/MC");
					  ratioAreaNorm->SetMaximum(1.5);
            ratioAreaNorm->SetMinimum(0.5);
			   }
			   else if(RatioType==2)
			   {
			      ratioAreaNorm->Add(totalSMAreaNorm,-1);
			      ratioAreaNorm->Divide(totalSMAreaNorm); //ratio = (data-MC)/MC
					  ratioAreaNorm->GetYaxis()->SetTitle("(Data-MC)/MC");
				  	ratioAreaNorm->SetMaximum(0.5);
            ratioAreaNorm->SetMinimum(-0.5);
			   }
		  }
      ratioAreaNorm->SetMarkerStyle(20);
      ratioAreaNorm->GetXaxis()->SetLabelSize(0.04);
      ratioAreaNorm->GetYaxis()->SetLabelSize(0.04);
  //    ratioAreaNorm->GetXaxis()->SetTitle(stack->GetXaxis()->GetTitle());
      ratioAreaNorm->GetYaxis()->SetTitleSize(0.045);
      ratioAreaNorm->GetYaxis()->SetTitleOffset(1.15);
      ratioAreaNorm->SetMarkerSize(1.);
      ratioAreaNorm->GetYaxis()->SetNdivisions(5);
	  	ratioAreaNorm->SetTitle("");
    
      padAreaNorm = new TPad("padAreaNorm", "padAreaNorm", 0.0, 0.0, 1.0, 1.0);
      padAreaNorm->SetTopMargin(0.7);
      padAreaNorm->SetFillColor(0);
      padAreaNorm->SetFillStyle(0);
      padAreaNorm->SetGridy(1);
      padAreaNormLogY = (TPad*) padAreaNorm->Clone();
	}
		
	hCanvasStackAreaNorm_->cd();
	if(hStackAreaNorm_)
	{
		  if(addErrorBand)
				DrawErrorBand(totalSMAreaNorm,hErrorPlus,hErrorMinus,hNominal,StackErrorGraph_,ErrorBandAroundTotalInput); //warning: error band around area-normalized plot may not make sense
			
			if(hData_)
			{
			  hData_->Draw("same E");					
			
		    if(RatioType>0)
		    {			
          padAreaNorm->Draw();
          padAreaNorm->cd(0);
          ratioAreaNorm->Draw("e");
          if(addRatioErrorBand)
          {       
				           TH1F* totalSMAreaNormRatio = (TH1F*) totalSM->Clone();
			  		       TH1F* hErrorPlusRatio = (TH1F*) hErrorPlus->Clone();
				  	       TH1F* hErrorMinusRatio = (TH1F*) hErrorMinus->Clone();
				  	       TH1F* hNominalRatio = (TH1F*) hNominal->Clone();
				  	       if(RatioType==1)
				  	       {
				  	         totalSMAreaNormRatio->Divide(totalSMAreaNorm);
				  			     hErrorPlusRatio->Divide(totalSMAreaNorm);
				  			     hErrorMinusRatio->Divide(totalSMAreaNorm);
				  	         hNominalRatio->Divide(totalSMAreaNorm);
				  	       }
				  	       else if(RatioType==2)
			             {
			               totalSMAreaNormRatio->Add(totalSMAreaNorm,-1);
			               totalSMAreaNormRatio->Divide(totalSMAreaNorm);
			  				     hErrorPlusRatio->Add(totalSMAreaNorm,-1);
				  			     hErrorPlusRatio->Divide(totalSMAreaNorm);
				  			     hErrorMinusRatio->Add(totalSMAreaNorm,-1);
				  			     hErrorMinusRatio->Divide(totalSMAreaNorm);
				  			     hNominalRatio->Add(totalSMAreaNorm,-1);
				  			     hNominalRatio->Divide(totalSMAreaNorm);
			            }     
			  		      DrawErrorBand(totalSMAreaNormRatio,hErrorPlusRatio,hErrorMinusRatio,hNominalRatio,RatioErrorGraph_,ErrorBandAroundTotalInput); //warning: error band around area-normalized plot may not make sense
          }
        }
			}
	}
	else 
	{
		   if(hData_) hData_->Draw("E");
	}
	
	//Now area-normalized log-scale plots...
	hCanvasStackAreaNormLogY_->cd();
	hCanvasStackAreaNormLogY_->SetLogy();
	if(hStackAreaNorm_)
	{		  
			if(addErrorBand)
				DrawErrorBand(totalSMAreaNorm,hErrorPlus,hErrorMinus,hNominal,StackErrorGraph_,ErrorBandAroundTotalInput); //warning: error band around area-normalized plot may not make sense	    
			
			if(hData_)
			{
			  hData_->Draw("same E");
			
		    if(RatioType)
		    {	
  		    padAreaNormLogY->Draw();
          padAreaNormLogY->cd(0);
          ratioAreaNorm->Draw("e");
          if(addRatioErrorBand)
          {       
				         TH1F* totalSMAreaNormRatio = (TH1F*) totalSM->Clone();
					       TH1F* hErrorPlusRatio = (TH1F*) hErrorPlus->Clone();
					       TH1F* hErrorMinusRatio = (TH1F*) hErrorMinus->Clone();
					       TH1F* hNominalRatio = (TH1F*) hNominal->Clone();
					       if(RatioType==1)
					       {
					         totalSMAreaNormRatio->Divide(totalSMAreaNorm);
							     hErrorPlusRatio->Divide(totalSMAreaNorm);
							     hErrorMinusRatio->Divide(totalSMAreaNorm);
					         hNominalRatio->Divide(totalSMAreaNorm);
					       }
					       else if(RatioType==2)
			           {
			             totalSMAreaNormRatio->Add(totalSMAreaNorm,-1);
			             totalSMAreaNormRatio->Divide(totalSMAreaNorm);
							     hErrorPlusRatio->Add(totalSMAreaNorm,-1);
							     hErrorPlusRatio->Divide(totalSMAreaNorm);
							     hErrorMinusRatio->Add(totalSMAreaNorm,-1);
							     hErrorMinusRatio->Divide(totalSMAreaNorm);
							     hNominalRatio->Add(totalSMAreaNorm,-1);
							     hNominalRatio->Divide(totalSMAreaNorm);
			          }     
					      DrawErrorBand(totalSMAreaNormRatio,hErrorPlusRatio,hErrorMinusRatio,hNominalRatio,RatioErrorGraph_,ErrorBandAroundTotalInput); //warning: error band around area-normalized plot may not make sense
          }
        }
			}
	}
	else 
	{
		   if(hData_) hData_->Draw("E");
	}
				
	if(addErrorBand)
	{
		   //make dummy graph for legend; dirty because color/style parameters of the error band now configured in 2 places...
		   TGraphAsymmErrors* ErrorGraph = new TGraphAsymmErrors();
		   ErrorGraph->SetFillStyle(3004);//3005 diagonal dashed //3001 ~plain //3013 double diagonal dashed 
			 ErrorGraph->SetFillColor(kRed);
	     ErrorGraph->SetLineColor(kRed);
	     ErrorGraph->SetLineWidth(1);
			 leg_->AddEntry(ErrorGraph,"Systematic Uncertainty","F");
	}
	
	if(hData_)
	{
	  if(hStack_ && ymax < 1.3*(hData_->GetMaximum())) ymax = 1.3*(hData_->GetMaximum());
	  if(hStackAreaNorm_ && ymaxAreaNorm < 1.3*(hData_->GetMaximum())) ymaxAreaNorm = 1.3*(hData_->GetMaximum());
	}

	if(maxY_ > 0)
	{
	  ymax = maxY_;
	  ymaxAreaNorm = maxY_;
	}
	if(hStack_)	hStack_->SetMaximum(ymax);
	if(hStackAreaNorm_) hStackAreaNorm_->SetMaximum(ymaxAreaNorm);

	hCanvasStack_->cd(0);
	leg_->Draw();
	hCanvasStackLogY_->cd(0);
	leg_->Draw();
	hCanvasStackAreaNorm_->cd(0);			
	legAreaNorm_->Draw();
	hCanvasStackAreaNormLogY_->cd(0);			
	legAreaNorm_->Draw();
	
  //if(tmpInfile) tmpInfile->Close(); //causes a crash when trying to write hData_ in the Write method...?
}

void MultiSamplePlot::DrawStackedPlot(TCanvas* canvas, TCanvas* canvasLogY, THStack* hstack, vector<TH1F*> histosForOverlay, int scaleNPSignal, const char* xaxistitle, const char* yaxistitle, unsigned int RatioType)
{ 
	stringstream slumi; slumi.precision(3); slumi << lumi_/1000; //luminosity given in picobarns, but will be displayed in femtobarns, because this in not 2010 anymore.
	stringstream ssqrts; ssqrts << sqrts_; //sqrt(s) given in TeV
	TLatex cmstext;
  cmstext.SetNDC(true);
  cmstext.SetTextAlign(12);
  cmstext.SetTextFont(42);
	cmstext.SetTextSize(0.05);
	
	TLatex text;
	text.SetNDC(true);
	text.SetTextAlign(12);
	text.SetTextFont(42);
	text.SetTextSize(0.04);
	text.SetTextColor(kBlack);

	canvas->cd();
	hstack->Draw("HIST");
	hstack->GetXaxis()->SetTitle(xaxistitle);
	hstack->GetYaxis()->SetTitle(yaxistitle);
	hstack->GetYaxis()->SetTitleOffset(1.1);
	
  if(RatioType>0)
	{
	   hstack->GetXaxis()->SetLabelSize(0);
     hstack->GetXaxis()->SetTitleSize(0);
  }
		 
	if(hData_)
	{
		 if(prelim_) cmstext.DrawLatex(0.16,0.975,("CMS Preliminary, "+slumi.str()+" fb^{-1} at #sqrt{s} = "+ssqrts.str()+" TeV").c_str());
		 else cmstext.DrawLatex(0.16,0.975,("CMS, "+slumi.str()+" fb^{-1} at #sqrt{s} = "+ssqrts.str()+" TeV").c_str());
	}
	else cmstext.DrawLatex(0.16,0.975,("CMS simulation, "+slumi.str()+" fb^{-1} at #sqrt{s} = "+ssqrts.str()+" TeV").c_str());
	
  if(!text_.IsNull()) text.DrawLatex(0.2,0.9,text_);
		
  for(unsigned int i=0;i<histosForOverlay.size();i++)
  {
	   histosForOverlay[i]->Scale(scaleNPSignal);
		 histosForOverlay[i]->SetFillColor(0);
		 histosForOverlay[i]->Draw("HIST SAME");
	}

	canvasLogY->cd();
	canvasLogY->SetLogy();
	hstack->Draw("HIST");
	if(hData_)
	{
		   if(prelim_) cmstext.DrawLatex(0.16,0.975,("CMS Preliminary, "+slumi.str()+" fb^{-1} at #sqrt{s} = "+ssqrts.str()+" TeV").c_str());
			 else cmstext.DrawLatex(0.16,0.975,("CMS, "+slumi.str()+" fb^{-1} at #sqrt{s} = "+ssqrts.str()+" TeV").c_str());
	}
	else cmstext.DrawLatex(0.16,0.975,("CMS simulation, "+slumi.str()+" fb^{-1} at #sqrt{s} = "+ssqrts.str()+" TeV").c_str());
	
  if(!text_.IsNull()) text.DrawLatex(0.5,0.86,text_);
	
  for(unsigned int i=0;i<histosForOverlay.size();i++)
	{
	       histosForOverlay[i]->Draw("HIST SAME");
	}

}

void MultiSamplePlot::DrawErrorBand(TH1F* totalSM, TH1F* hErrorPlus, TH1F* hErrorMinus, TH1F* hNominal, TGraphAsymmErrors*  ErrorGraph, bool ErrorBandAroundTotalInput)
{
  int nbins = totalSM->GetNbinsX()+1;
	float bins[nbins];
	float bincontents[nbins];
	float dummy[nbins];
	float erroryplus[nbins]; //will be the errors relative to the total histo
	float erroryminus[nbins]; //will be the errors relative to the total histo
  float binwidth = totalSM->GetBinCenter(2) - totalSM->GetBinCenter(1); //assumes a fixed bin width...!
	
  if((totalSM->GetNbinsX() != hErrorPlus->GetNbinsX()) || (totalSM->GetNbinsX() != hErrorMinus->GetNbinsX()))
			cout<<"[MultiSamplePlot::DrawErrorBand] ERROR: error histograms have different binning than total SM histogram!"<<endl;

	for(int iBin=0; iBin < totalSM->GetNbinsX()+1; iBin++)
	{
			bins[iBin] = totalSM->GetBinCenter(iBin);
			if(ErrorBandAroundTotalInput)
			   bincontents[iBin] = totalSM->GetBinContent(iBin); //meaning an error band around whatever is the total MC SM input (can be 'post-fit' (as in the profile likelihood method), the input is then not the 'nominal' expectation anymore w.r.t which the error band is intrinsically defined)
			else 
			   bincontents[iBin] = hNominal->GetBinContent(iBin);
			erroryplus[iBin] = hErrorPlus->GetBinContent(iBin) - hNominal->GetBinContent(iBin);
			erroryminus[iBin] = hNominal->GetBinContent(iBin) - hErrorMinus->GetBinContent(iBin);
      dummy[iBin] = binwidth/2;
	}	
	
	ErrorGraph = new TGraphAsymmErrors(nbins,bins,bincontents,dummy,dummy,erroryplus,erroryminus);					
	ErrorGraph->SetFillStyle(3004);//3005 diagonal dashed //3001 ~plain //3013 double diagonal dashed 
	ErrorGraph->SetFillColor(kRed);
	ErrorGraph->SetLineColor(kRed);
	ErrorGraph->SetLineWidth(1);
	ErrorGraph->Draw("2"); 
}

void MultiSamplePlot::Write(TFile* fout, string label, bool savePNG, string pathPNG, string ext) 
{
	fout->cd();
	string dirname = "MultiSamplePlot_"+label;
	if(fout->Get(dirname.c_str())==0)
		fout->mkdir(dirname.c_str());

	fout->cd(dirname.c_str());

	if(hStack_) hStack_->Write(); 
	if(hStackAreaNorm_) hStackAreaNorm_->Write();
	if(hData_) hData_->Write();		 

	if(hCanvas_)
	{
	  if(savePNG)
	    hCanvas_->SaveAs( (pathPNG+label+"_Normalized."+ext).c_str() );
	  hCanvas_->Write();
	}
  
	if(hCanvasStack_)
	{
	  if(hStack_) hStack_->SetMinimum(0.001);
	  hCanvasStack_->Write();
	  if(savePNG)
	    hCanvasStack_->SaveAs( (pathPNG+label+"_Stack."+ext).c_str() );
	}
  
	if(hCanvasStackLogY_)
	{
	  if(hStack_) hStack_->SetMinimum(minLogY_);
	  hCanvasStackLogY_->Write();
	  if(savePNG)
	    hCanvasStackLogY_->SaveAs( (pathPNG+label+"_StackLogY."+ext).c_str() );
	}
  
	if(hCanvasStackAreaNorm_ && hData_)
	{
	  if(hStackAreaNorm_) hStackAreaNorm_->SetMinimum(0.001);
	  hCanvasStackAreaNorm_->Write();
	  if(savePNG)
	    hCanvasStackAreaNorm_->SaveAs( (pathPNG+label+"_StackAreaNorm."+ext).c_str() );
	}
  
	if(hCanvasStackAreaNormLogY_ && hData_)
	{
	  if(hStackAreaNorm_) hStackAreaNorm_->SetMinimum(minLogY_);
	  hCanvasStackAreaNormLogY_->Write();
	  if(savePNG)
	    hCanvasStackAreaNormLogY_->SaveAs( (pathPNG+label+"_StackAreaNormLogY."+ext).c_str() );
	}
  
	for(unsigned int i=0; i<plots_.size(); i++)
	  if(plots_[i].second->Name().find("data") != 0 && plots_[i].second->Name().find("Data") != 0 && plots_[i].second->Name().find("DATA") != 0 )
		{
		  plots_[i].first->SetMarkerSize(0.5);			
      plots_[i].first->Write();
		} 
}
