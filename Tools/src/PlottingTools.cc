#include "../interface/PlottingTools.h"

using namespace std;


void CopyHisto(TH1F* original, TH1F* copy){
	double* binsX = new double[original->GetNbinsX()+1];
	if(original->GetXaxis()->GetXbins()->GetSize()>0){
		for(int i=0;i<original->GetXaxis()->GetXbins()->GetSize();i++) binsX[i] = original->GetXaxis()->GetXbins()->At(i);
		copy->SetBins(original->GetNbinsX(),binsX);
	}
	else copy->SetBins(original->GetNbinsX(),original->GetXaxis()->GetXmin(),original->GetXaxis()->GetXmax());
	for(int i=0;i<copy->GetNbinsX()+1;i++){
		copy->SetBinContent(i,original->GetBinContent(i));
		copy->SetBinError(i,original->GetBinError(i));
	}
}

float DiffEstimation(TH1F* h1, TH1F* h2){
        if(h1==0 || h2==0) return -9999.;
        if(h1->Integral()==0 || h2->Integral()==0) return -9999.;
        //clone
        TH1F* temp_1 = (TH1F*) h1->Clone();
        TH1F* temp_2 = (TH1F*) h2->Clone();
        //scale
	if(temp_1->GetSumw2N()==0) temp_1->Sumw2();
	if(temp_2->GetSumw2N()==0) temp_2->Sumw2();
        temp_1->Scale(1/temp_1->Integral());
        temp_2->Scale(1/temp_2->Integral());
        //compute
        float diff = 0;
        for(int i=1;i<temp_1->GetNbinsX()+1;i++) diff+=pow(temp_1->GetBinContent(i)-temp_2->GetBinContent(i),2.);
        delete temp_1;
        delete temp_2;
        return diff;
}


float Chi2Normalized(TH1F* h1, TH1F* h2, bool scaled){
        if(h1==0 || h2==0) return -9999.;
        if(h1->Integral()==0 || h2->Integral()==0) return -9999.;
        if(scaled){
                //clone
                TH1F* temp_1 = (TH1F*) h1->Clone();
                TH1F* temp_2 = (TH1F*) h2->Clone();
                //scale
		if(temp_1->GetSumw2N()==0) temp_1->Sumw2();
		if(temp_2->GetSumw2N()==0) temp_2->Sumw2();
                temp_1->Scale(1/temp_1->Integral());
                temp_2->Scale(1/temp_2->Integral());
                //compute
                float chi2 = 0;
                float biggestError = 0;
                for(int i=1;i<temp_1->GetNbinsX()+1;i++){
                        temp_1->GetBinError(i)> temp_2->GetBinError(i) ? biggestError = temp_1->GetBinError(i): biggestError = temp_2->GetBinError(i);
                        if(biggestError!=0) chi2+=(pow(temp_1->GetBinContent(i)-temp_2->GetBinContent(i),2.)/(biggestError*biggestError));
                }
                float nof = temp_1->GetNbinsX();
                delete temp_1;
                delete temp_2;
                return (chi2/nof);
        }
        else{
                float chi2 = 0;
                float biggestError = 0;
                for(int i=1;i<h1->GetNbinsX()+1;i++){
                        h1->GetBinError(i)> h2->GetBinError(i) ? biggestError = h1->GetBinError(i): biggestError = h2->GetBinError(i);
                        if(biggestError!=0) chi2+=(pow(h1->GetBinContent(i)-h2->GetBinContent(i),2.)/(biggestError*biggestError));
                }
                return (chi2/h1->GetNbinsX());
        }
}

TH1F* PlotCumulDiffEstimation(TH1F* h1, TH1F* h2, TString hName){
        if(h1==0 || h2==0) return NULL;
        if(h1->Integral()==0 || h2->Integral()==0) return NULL;
	if(h1->GetNbinsX() != h2->GetNbinsX()) return NULL;
	
	TH1F* CumulHist = (TH1F*) h1->Clone();
	CumulHist->SetNameTitle(hName,"Cumulative difference");
	float CumulEff = 0;
        //compute
        for(int i=1;i<h1->GetNbinsX()+1;i++){
                
		if(h1->Integral(i,h1->GetNbinsX()+1)>0) CumulEff = fabs(h1->Integral(i,h1->GetNbinsX()+1)-h2->Integral(i,h1->GetNbinsX()+1))/h1->Integral(i,h1->GetNbinsX()+1);
		else CumulEff = 0;
		CumulHist->SetBinContent(i,CumulEff);
	}
	CumulHist->GetYaxis()->SetTitle("Rel. Error");
        return CumulHist;
}

//Compute ratio of 2 histograms
TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator){	
	string hname;
	if(hnumerator==0 || hdenominator==0) hname = string (hnumerator->GetName())+"_"+string (hdenominator->GetName())+"_Ratio";
	return PlotRatio(hnumerator,hdenominator,hname);
}

TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator, string histoName){	
	if(hnumerator==0 || hdenominator==0){
		cout<<"PlotRatio:: one of the plots doesn't exist !"<<endl;
		return 0;
	}
	if(hnumerator->GetNbinsX()!= hdenominator->GetNbinsX()){
		cout<<"PlotRatio:: plots doesn't have the same nof bins !"<<endl;
		return 0;
	}
	TH1F* histoRatio = (TH1F*) hnumerator->Clone();
	histoRatio->SetName(histoName.c_str());
	histoRatio->Divide(hdenominator);
	return histoRatio; 
}

TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator){	
	string hname;
	if(hnumerator==0 || hdenominator==0) hname = string (hnumerator->GetName())+"_"+string (hdenominator->GetName())+"_Ratio";
	return PlotRatio(hnumerator,hdenominator,hname);
}


TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator, string histoName){	
	if(hnumerator==0 || hdenominator==0){
		cout<<"PlotRatio:: one of the plots doesn't exist !"<<endl;
		return 0;
	}
	if(hnumerator->GetNbinsX()!= hdenominator->GetNbinsX()){
		cout<<"PlotRatio:: plots doesn't have the same nof bins !"<<endl;
		return 0;
	}
	TH1D* histoRatio = (TH1D*) hnumerator->Clone();
	histoRatio->SetName(histoName.c_str());
	histoRatio->Divide(hdenominator);
	return histoRatio; 
}

TH2F* PlotRatio(TH2F* hnumerator, TF2* fdenominator, string histoName)
{
  TH2F* histoRatio = (TH2F*) hnumerator->Clone();
  histoRatio->SetName(histoName.c_str());
  for(int xBin=1; xBin<histoRatio->GetNbinsX(); xBin++)
  {
    for(int yBin=1; yBin<histoRatio->GetNbinsY(); yBin++)
    {
      float xValue = histoRatio->GetXaxis()->GetBinCenter(xBin);
      if(fabs(xValue - hnumerator->GetXaxis()->GetBinCenter(xBin)) > 1.) cout << "Weird binning problem..." << endl;
      float yValue = histoRatio->GetYaxis()->GetBinCenter(yBin);
      if(fabs(yValue - hnumerator->GetYaxis()->GetBinCenter(yBin)) > 0.01) cout << "Weird binning problem..." << endl;
      float funcValue = fdenominator->Eval(xValue, yValue);
//      cout << "xBin = " << xBin << " xValue = " << xValue << " xOldValue = " << hnumerator->GetXaxis()->GetBinCenter(xBin) << "  yBin = " << yBin << " yValue = " << yValue << " yOldValue = " << hnumerator->GetYaxis()->GetBinCenter(yBin) << "  histoValue = " << hnumerator->GetBinContent(xBin, yBin) << "  funcValue = " << funcValue << endl;
      histoRatio->SetBinContent(xBin, yBin, hnumerator->GetBinContent(xBin, yBin) / funcValue );
    }
  }
  return histoRatio;
}

TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator, TF1* func){	
	string hname;
	if(hnumerator==0 || hdenominator==0) hname = string (hnumerator->GetName())+"_"+string (hdenominator->GetName())+"_Ratio";
	return PlotRatio(hnumerator,hdenominator,func,hname);
}

TH1F* PlotRatio(TH1F* hnumerator, TH1F* hdenominator, TF1* func, string histoname, bool doFit){	
	if(hnumerator==0 || hdenominator==0){
		cout<<"PlotRatio:: one of the plots doesn't exist !"<<endl;
		return 0;
	}
	if(hnumerator->GetNbinsX()!= hdenominator->GetNbinsX()){
		cout<<"PlotRatio:: plots doesn't have the same nof bins !"<<endl;
		return 0;
	}
	if(func == 0 ){
		cout<<"TF1 function is empty"<<endl;
		return 0 ;
	}
	TH1F* histoRatio = (TH1F*) hnumerator->Clone();
	histoRatio->SetName(histoname.c_str());
	histoRatio->Divide(hdenominator);
	if(doFit) histoRatio->Fit(func);
	return histoRatio; 
}

TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator, TF1* func){	
	string hname;
	if(hnumerator==0 || hdenominator==0) hname = string (hnumerator->GetName())+"_"+string (hdenominator->GetName())+"_Ratio";
	return PlotRatio(hnumerator,hdenominator,func,hname);
}

TH1D* PlotRatio(TH1D* hnumerator, TH1D* hdenominator, TF1* func, string histoname, bool doFit){	
	if(hnumerator==0 || hdenominator==0){
		cout<<"PlotRatio:: one of the plots doesn't exist !"<<endl;
		return 0;
	}
	if(hnumerator->GetNbinsX()!= hdenominator->GetNbinsX()){
		cout<<"PlotRatio:: plots doesn't have the same nof bins !"<<endl;
		return 0;
	}
	if(func == 0 ){
		cout<<"TF1 function is empty"<<endl;
		return 0 ;
	}
	TH1D* histoRatio = (TH1D*) hnumerator->Clone();
	histoRatio->SetName(histoname.c_str());
	histoRatio->Divide(hdenominator);
	if(doFit) histoRatio->Fit(func);
	return histoRatio; 
}

//Compute a THstack starting with a ordered collection of histograms
THStack* THStackCreator(vector<TH1F*> listOfHistos){
	THStack* stack = new THStack();
	for(unsigned int i=0;i<listOfHistos.size();i++){
		stack->Add(listOfHistos[i]);
	}
	return stack;
}

THStack* THStackCreator(vector<TH1D*> listOfHistos){
	THStack* stack = new THStack();
	for(unsigned int i=0;i<listOfHistos.size();i++){
		stack->Add(listOfHistos[i]);
	}
	return stack;
}

THStack* THStackCreator(vector<TH1F*> listOfHistos, vector<int> listOfColor){
	if(listOfColor.size()!=listOfHistos.size()) cout<<"listOfColor size is different that listOfHistos size !!"<<endl;
	THStack* stack = new THStack();
	for(unsigned int i=0;i<listOfHistos.size();i++){
		if(i<listOfColor.size()){
			listOfHistos[i]->SetLineColor(listOfColor[i]); 
			listOfHistos[i]->SetFillColor(listOfColor[i]); 
		}
		stack->Add(listOfHistos[i]);
	}
	return stack;
}

THStack* THStackCreator(vector<TH1D*> listOfHistos, vector<int> listOfColor){
	if(listOfColor.size()!=listOfHistos.size()) cout<<"listOfColor size is different that listOfHistos size !!"<<endl;
	THStack* stack = new THStack();
	for(unsigned int i=0;i<listOfHistos.size();i++){
		if(i<listOfColor.size()){
			listOfHistos[i]->SetLineColor(listOfColor[i]); 
			listOfHistos[i]->SetFillColor(listOfColor[i]); 
		}
		stack->Add(listOfHistos[i]);
	}
	return stack;
}

THStack* THStackCreator(vector<TH1F*> listOfHistos, vector<Dataset*> datasets, TLegend* leg, bool showNbEntries){
	if(listOfHistos.size()!=datasets.size()) cout<<"datasets size is different that listOfHistos size !!"<<endl;
	THStack* stack = new THStack();
	for(int i=(listOfHistos.size()-1);i>=0;i--){
		listOfHistos[i]->SetLineColor(datasets[i]->Color()); 
		listOfHistos[i]->SetFillColor(datasets[i]->Color()); 
		stack->Add(listOfHistos[i]);
	}
//	for(int i=(listOfHistos.size()-1);i>=0;i--){
	for(unsigned int i=0;i<listOfHistos.size();i++){
		if(!showNbEntries){
			if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], datasets[i]->Title().c_str(), "F");
		}
		else{
			char legDataTitle[100];
			sprintf (legDataTitle, "%s (%.1f entries)", ( datasets[i]->Title() ).c_str(), listOfHistos[i]->Integral(1, listOfHistos[i]->GetNbinsX() ) );
			if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], legDataTitle, "F");
		}
	}
	return stack;
}

THStack* THStackCreator(vector<TH1D*> listOfHistos, vector<Dataset*> datasets, TLegend* leg, bool showNbEntries){
	if(listOfHistos.size()!=datasets.size()) cout<<"datasets size is different that listOfHistos size !!"<<endl;
	THStack* stack = new THStack();
	for(int i=(listOfHistos.size()-1);i>=0;i--){
		listOfHistos[i]->SetLineColor(datasets[i]->Color()); 
		listOfHistos[i]->SetFillColor(datasets[i]->Color()); 
		stack->Add(listOfHistos[i]);
	}
//	for(int i=(listOfHistos.size()-1);i>=0;i--){
	for(unsigned int i=0;i<listOfHistos.size();i++){
		if(!showNbEntries){
			if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], datasets[i]->Title().c_str(), "F");
		}
		else{
			char legDataTitle[100];
			sprintf (legDataTitle, "%s (%.1f entries)", ( datasets[i]->Title() ).c_str(), listOfHistos[i]->Integral(1, listOfHistos[i]->GetNbinsX() ) );
			if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], legDataTitle, "F");
		}
	}
	return stack;
}

void ConfigureHisto(TH1F* histo, Dataset* dataset, bool fillMode){
	histo->SetLineStyle(dataset->LineStyle());
	histo->SetLineWidth(dataset->LineWidth());
	histo->SetLineColor(dataset->Color());
	if(fillMode) histo->SetFillColor(dataset->Color());
	else histo->SetFillColor(0);
}

float YMax(vector<TH1F*> listOfHistos){
	float YMax = 0;
	for(unsigned int i=0;i<listOfHistos.size();i++){
	  float tempMax = listOfHistos[i]->GetBinContent(listOfHistos[i]->GetMaximumBin());
		if(tempMax > YMax) YMax = tempMax;
	}
	return YMax;
}

float YMax(vector<std::pair<TH1F*,std::string> > listOfHistos){
	float YMax = 0;
	for(unsigned int i=0;i<listOfHistos.size();i++){
	  float tempMax = listOfHistos[i].first->GetBinContent(listOfHistos[i].first->GetMaximumBin());
	  if(tempMax > YMax) YMax = tempMax;
	}
	return YMax;
}

float WilsonErrorHigh(float Non, float Ntot)
{
	float T = (Ntot>0 ? 1/Ntot : 0);
	float p_hat = (Ntot>0 && Non>=0 && Ntot>=Non ? Non/Ntot : 0);
	float Int_High = ((p_hat+(T/2))/(1+T))+(sqrt(p_hat*(1-p_hat)*T+pow(T/2,2))/(1+T));
	return (Int_High-p_hat);
}

float WilsonErrorLow(float Non, float Ntot)
{
	float T = (Ntot>0 ? 1/Ntot : 0);
	float p_hat = (Ntot>0 && Non>=0 && Ntot>=Non ? Non/Ntot : 0);
	float Int_Low = ((p_hat+(T/2))/(1+T))-(sqrt(p_hat*(1-p_hat)*T+pow(T/2,2))/(1+T));
	return (p_hat-Int_Low);
}

TGraphAsymmErrors* BGScaling(TH1F* hJetMultiplicity){
  const int Nbins = hJetMultiplicity->GetNbinsX();
  float X[Nbins],Xl[Nbins],Xr[Nbins];
  float Y[Nbins],Yl[Nbins],Yh[Nbins];
  //X[0]=0; Xl[0]=0; Xr[0]=0; 
  //Y[0]=0; Yl[0]=0; Yh[0]=0; 
  for(int i=0;i<Nbins;i++){
	X[i] = hJetMultiplicity->GetBinCenter(i+1);
	Xl[i]= hJetMultiplicity->GetBinWidth(i+1)/2;
	Xr[i]= hJetMultiplicity->GetBinWidth(i+1)/2;
	if(hJetMultiplicity->Integral(i,Nbins+1)>0){
		Y[i] = hJetMultiplicity->Integral(i+2,Nbins+1)/hJetMultiplicity->Integral(i+1,Nbins+1);
		Yh[i]= WilsonErrorHigh(hJetMultiplicity->Integral(i+2,Nbins+1),hJetMultiplicity->Integral(i+1,Nbins+1));
		Yl[i]= WilsonErrorLow( hJetMultiplicity->Integral(i+2,Nbins+1),hJetMultiplicity->Integral(i+1,Nbins+1));
	}
	else{
		Y[i] = 0;
		Yh[i]= 0;
		Yl[i]= 0;
	}
  }
  TGraphAsymmErrors* tg = new TGraphAsymmErrors(Nbins,X,Y,Xl,Xr,Yl,Yh);
  return tg;
}

TGraphAsymmErrors* GraphRatio(TH1F* hnumerator, TH1F* hdenominator, string label, string xAxisLabel, string yAxisLabel){
  TGraphAsymmErrors* graph = new TGraphAsymmErrors(hnumerator, hdenominator);
  graph->SetNameTitle(label.c_str(),label.c_str());
  graph->GetXaxis()->SetTitle(xAxisLabel.c_str());
  graph->GetYaxis()->SetTitle(yAxisLabel.c_str());
  return graph;
}

TCanvas* TCanvasCreator(TH1F* histo, string label){
  histo->GetXaxis()->SetTitle(label.c_str());
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  histo->Draw();
  return c1;
}

TCanvas* TCanvasCreator(TH2F* histo, string label, string drawOption){
  if(label.find("VS") >= 0)
  {
    size_t beforeVS = label.find("VS");
    histo->GetXaxis()->SetTitle( ( label.substr(0, beforeVS) ).c_str() );
    histo->GetYaxis()->SetTitle( ( label.substr(beforeVS+2, label.size()) ).c_str() );
  }
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  histo->Draw(drawOption.c_str());
  return c1;
}

TCanvas* TCanvasCreator(TGraphAsymmErrors* graph, string label)
{
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  graph->Draw("AP");
  return c1;
}
TCanvas* TCanvasCreator(TGraphErrors* graph, string label, string options)
{
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  graph->Draw(options.c_str());
  return c1;
}

TCanvas* TCanvasCreator(TGraph* graph, string label, string options){
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  graph->Draw(options.c_str());
  return c1;
}

TCanvas* TCanvasCreator(TProfile* profile, string label)
{
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  profile->Draw();
  return c1;
}

TCanvas* TCanvasCreator(TH1* histo, TF1* fit, string label) {
  histo->SetTitle(label.c_str());
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  histo->Draw();
  fit->SetLineColor(4);
  fit->Draw("same");
  return c1;
}

TCanvas* TCanvasCreator(TH2F* histo, TF2* fit, string label, string drawOption)
{
  if(label.find("VS") >= 0)
  {
    size_t beforeVS = label.find("VS");
    histo->GetXaxis()->SetTitle( ( label.substr(0, beforeVS) ).c_str() );
    histo->GetYaxis()->SetTitle( ( label.substr(beforeVS+2, label.size()) ).c_str() );
  }
  TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
  c1->cd();
  histo->Draw(drawOption.c_str());
  fit->SetLineColor(4);
  fit->Draw("surf same");
  return c1;
}

TCanvas* TCanvasCreator(vector<std::pair<TGraph*,std::string> > listOfHistos, string label){
	//// standard method 
	TLegend* leg = new TLegend(0.58,0.72,0.92,0.92);//0.75,0.8,0.99,1.
	leg->SetFillColor(0);
	leg->SetShadowColor(0);
	TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
	c1->cd();
	gPad->SetGrid();
	for(unsigned int i=0;i<listOfHistos.size();i++){
	  if (i == 3)
	    listOfHistos[i].first->SetLineColor(1);
	  else
	    listOfHistos[i].first->SetLineColor(i+2);
	  listOfHistos[i].first->SetTitle(label.c_str());

	  if(i==0){
	    listOfHistos[i].first->GetYaxis()->SetRangeUser(0.,1.);
	    listOfHistos[i].first->Draw("AC");
	  }
	  else listOfHistos[i].first->Draw("sameC");

	  if(listOfHistos[i].first->GetN()>0) leg->AddEntry(listOfHistos[i].first, listOfHistos[i].second.c_str(), "L");
	}
	leg->Draw();
	return c1;
}

TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset> datasets, TLegend* leg, string option, string label){
	if(datasets.size()!=listOfHistos.size()){
		cout<< "Nof Histo and Nof datasets are not the same"<<endl;
		return NULL;
	}

	/** if there is dataset called data||Data||DATA call a other method to return a TCanvas with MC+data superimposed*/
	unsigned int it_Data = 9999;
	for(unsigned int i=0;i<datasets.size();i++){
		if(datasets[i].Name().find("Data")<datasets[i].Name().size() || datasets[i].Name().find("data")<datasets[i].Name().size() || datasets[i].Name().find("DATA")<datasets[i].Name().size()) it_Data = i;
	}

	if(it_Data < 9999){
		TH1F* hData = (TH1F*) listOfHistos[it_Data]->Clone();
		vector<TH1F*> histosNoData;
		vector<Dataset> datasetsNoData;
		for(unsigned int i=0; i<datasets.size();i++) {
			if(i != it_Data) {
				TH1F* clone = (TH1F*) listOfHistos[i]->Clone();
				histosNoData.push_back(clone);
				datasetsNoData.push_back(datasets[i]);
			}
		}
		return TCanvasCreator(histosNoData, datasetsNoData, hData, datasets[it_Data].Title(), leg, option, label);
	}

	//// standard method
	leg = new TLegend(0.58,0.72,0.92,0.92);//0.75,0.8,0.99,1.
	leg->SetFillColor(0);
	leg->SetShadowColor(0);
	TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
	c1->cd();
	float ymax = 1.2*YMax(listOfHistos);
	for(unsigned int i=0;i<listOfHistos.size();i++){
		ConfigureHisto(listOfHistos[i], &datasets[i]);
		if(i==0){
			listOfHistos[i]->GetYaxis()->SetRangeUser(0.,ymax);
			listOfHistos[i]->Draw("HIST");
		}
		else	listOfHistos[i]->Draw("same HIST");
		if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], datasets[i].Title().c_str(), option.c_str());
	}
	leg->Draw("same");
	return c1;
}

TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset*> datasets, TLegend* leg, string option, string label){
	if(datasets.size()!=listOfHistos.size()){
		cout<< "Nof Histo and Nof datasets are not the same"<<endl;
		return NULL;
	}

	/** if there is dataset called data||Data||DATA call a other method to return a TCanvas with MC+data superimposed*/
	unsigned int it_Data = 9999;
	for(unsigned int i=0;i<datasets.size();i++){
		if(datasets[i]->Name().find("Data")<datasets[i]->Name().size() || datasets[i]->Name().find("data")<datasets[i]->Name().size() || datasets[i]->Name().find("DATA")<datasets[i]->Name().size()) it_Data = i;
	}

	if(it_Data < 9999){
		TH1F* hData = (TH1F*) listOfHistos[it_Data]->Clone();
		vector<TH1F*> histosNoData;
		vector<Dataset*> datasetsNoData;
		for(unsigned int i=0; i<datasets.size();i++) {
			if(i != it_Data) {
				TH1F* clone = (TH1F*) listOfHistos[i]->Clone();
				histosNoData.push_back(clone);
				datasetsNoData.push_back(datasets[i]);
			}
		}
		return TCanvasCreator(histosNoData, datasetsNoData, hData, datasets[it_Data]->Title(), leg, option, label);
	}

	//// standard method
	leg = new TLegend(0.58,0.72,0.92,0.92);//0.75,0.8,0.99,1.
	leg->SetFillColor(0);
	leg->SetShadowColor(0);
	TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
	c1->cd();
	float ymax = 1.2*YMax(listOfHistos);
	for(unsigned int i=0;i<listOfHistos.size();i++){
		ConfigureHisto(listOfHistos[i], datasets[i]);
		if(i==0){
			listOfHistos[i]->GetYaxis()->SetRangeUser(0.,ymax);
			listOfHistos[i]->Draw("HIST");
		}
		else	listOfHistos[i]->Draw("same HIST");
		if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], datasets[i]->Title().c_str(), option.c_str());
	}
	leg->Draw("same");
	return c1;
}

TCanvas* TCanvasCreator(vector<std::pair<TH1F*,std::string> > listOfHistos, vector<int> listOfColor, string option, string label,bool norm, bool doStack){

	if(norm)   
	  for(unsigned int i=0;i<listOfHistos.size();i++)
	    listOfHistos[i].first->Scale(1./listOfHistos[i].first->Integral(1,listOfHistos[i].first->GetNbinsX()+1));
	//// standard method 
	TLegend* leg = new TLegend(0.58,0.72,0.92,0.92);//0.75,0.8,0.99,1.
	leg->SetFillColor(0);
	leg->SetShadowColor(0);
	TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
	c1->cd();
	float ymax = 1.5*YMax(listOfHistos);
	THStack* stack = 0;
	if(doStack) stack = new THStack();
	for(unsigned int i=0;i<listOfHistos.size();i++){

	  if(doStack){
		listOfHistos[i].first->SetFillColor(listOfColor[i]);
		listOfHistos[i].first->SetLineColor(kBlack);
	  }
	  else  listOfHistos[i].first->SetLineColor(listOfColor[i]);
	  listOfHistos[i].first->SetTitle(label.c_str());
	  if(doStack){
	    stack->Add(listOfHistos[i].first);
	  }
	  else{
	    if(i==0){
	      listOfHistos[i].first->GetYaxis()->SetRangeUser(0.,ymax);
	      listOfHistos[i].first->Draw();
	    }
	    else listOfHistos[i].first->Draw("same");
	  }
	  if(listOfHistos[i].first->Integral()>0) leg->AddEntry(listOfHistos[i].first, listOfHistos[i].second.c_str(), option.c_str());
	}
	if(doStack){
	  stack->Draw("HIST");
	  if(listOfHistos.size()>0){
	    stack->GetXaxis()->SetTitle(listOfHistos[0].first->GetXaxis()->GetTitle());
	    stack->GetYaxis()->SetTitle(listOfHistos[0].first->GetYaxis()->GetTitle());
	  }
	}
	leg->Draw("same");
	return c1;
}

TCanvas* TCanvasCreator(vector<std::pair<TH1F*,std::string> > listOfHistos, string option, string label,bool norm, bool doStack){

	if(norm)   
	  for(unsigned int i=0;i<listOfHistos.size();i++)
	    listOfHistos[i].first->Scale(1./listOfHistos[i].first->Integral(1,listOfHistos[i].first->GetNbinsX()+1));
	//// standard method 
	TLegend* leg = new TLegend(0.58,0.72,0.92,0.92);//0.75,0.8,0.99,1.
	leg->SetFillColor(0);
	leg->SetShadowColor(0);
	TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
	c1->cd();
	float ymax = 1.5*YMax(listOfHistos);
	THStack* stack = 0;
	if(doStack) stack = new THStack();
	for(unsigned int i=0;i<listOfHistos.size();i++){

	  if(doStack){
		listOfHistos[i].first->SetFillColor(i+2);
		listOfHistos[i].first->SetLineColor(kBlack);
	  }else	listOfHistos[i].first->SetLineColor(i+1);
	  listOfHistos[i].first->SetTitle(label.c_str());
	  if(doStack){
	    stack->Add(listOfHistos[i].first);
	  }
	  else{
	    if(i==0){
	      listOfHistos[i].first->GetYaxis()->SetRangeUser(0.,ymax);
	      listOfHistos[i].first->Draw();
	    }
	    else listOfHistos[i].first->Draw("same");
	  }
	  if(listOfHistos[i].first->Integral()>0) leg->AddEntry(listOfHistos[i].first, listOfHistos[i].second.c_str(), option.c_str());
	}
	if(doStack){
	  stack->Draw("HIST");
	  if(listOfHistos.size()>0){
	    stack->GetXaxis()->SetTitle(listOfHistos[0].first->GetXaxis()->GetTitle());
	    stack->GetYaxis()->SetTitle(listOfHistos[0].first->GetYaxis()->GetTitle());
	  }
	}
	leg->Draw("same");
	return c1;
}

TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset> datasets, TH1F* hData, string dataLabel, TLegend* leg, string option, string label){
	leg = new TLegend(0.58,0.72,0.92,0.92);//0.75,0.8,0.99,1.
	leg->SetFillColor(0);
	leg->SetShadowColor(0);
	if(datasets.size()!=listOfHistos.size()){
		cout<< "Nof Histo and Nof datasets are not the same"<<endl;
		return NULL;
	}
	TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
	c1->cd();
	float ymax = 1.2*YMax(listOfHistos);
	if(ymax < 1.2*(hData->GetMaximum())) ymax = 1.2*(hData->GetMaximum());
	for(unsigned int i=0;i<listOfHistos.size();i++){
		ConfigureHisto(listOfHistos[i], &datasets[i]);
		if(i==0){
			listOfHistos[i]->GetYaxis()->SetRangeUser(0.,ymax);
			listOfHistos[i]->Draw("HIST");
		}
		else	listOfHistos[i]->Draw("HIST same");
		if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], datasets[i].Title().c_str(), option.c_str());
	}
	hData->SetLineColor(1);
	hData->SetLineWidth(2);
	hData->SetFillStyle(1001);
	hData->Draw("same HIST");
	leg->AddEntry(hData,dataLabel.c_str(),"l");
	leg->Draw("same");
	return c1;
}
TCanvas* TCanvasCreator(vector<TH1F*> listOfHistos, vector<Dataset*> datasets, TH1F* hData, string dataLabel, TLegend* leg, string option, string label){
	leg = new TLegend(0.58,0.72,0.92,0.92);//0.75,0.8,0.99,1.
	leg->SetFillColor(0);
	leg->SetShadowColor(0);
	if(datasets.size()!=listOfHistos.size()){
		cout<< "Nof Histo and Nof datasets are not the same"<<endl;
		return NULL;
	}
	TCanvas *c1 = new TCanvas(label.c_str(), label.c_str());
	c1->cd();
	float ymax = 1.2*YMax(listOfHistos);
	if(ymax < 1.2*(hData->GetMaximum())) ymax = 1.2*(hData->GetMaximum());
	for(unsigned int i=0;i<listOfHistos.size();i++){
		ConfigureHisto(listOfHistos[i], datasets[i]);
		if(i==0){
			listOfHistos[i]->GetYaxis()->SetRangeUser(0.,ymax);
			listOfHistos[i]->Draw("HIST");
		}
		else	listOfHistos[i]->Draw("same HIST");
		if(listOfHistos[i]->Integral()>0) leg->AddEntry(listOfHistos[i], datasets[i]->Title().c_str(), option.c_str());
	}
	hData->SetLineColor(1);
	hData->SetLineWidth(2);
	hData->SetFillStyle(1001);
	hData->Draw("same HIST");
	leg->AddEntry(hData,dataLabel.c_str(),"l");
	leg->Draw("same");
	return c1;
}

//Return a TCanvas with plots on top and bottom
TCanvas* TCanvasCreator(TH1F* histoTop, TH1F* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit){
	vector<TH1F*> hTop;
	vector<TH1F*> hBottom;
	hBottom.push_back(histoBottom);
	hTop.push_back(histoTop);
	return TCanvasCreator(hTop, hBottom, line, leg, label, canvasName, MaxY);
}

TCanvas* TCanvasCreator(vector<TH1F*> histoTop, TH1F* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit){
	vector<TH1F*> hBottom;
	hBottom.push_back(histoBottom);
	return TCanvasCreator(histoTop, hBottom, line, leg, label, canvasName, MaxY);
}

TCanvas* TCanvasCreator(vector<TH1F*> histoTop, vector<TH1F*> histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit){
    TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
    if(canvasName!=string("")) c1->SetName(canvasName.c_str());
    //c1->Range(0,0,1,1);
    c1->Divide(1,2);
    TVirtualPad *pad1 = c1->GetPad(1);
    pad1->SetPad(0.0,0.35,1.0,1.0);
    pad1->SetMargin(0.13,0.05,0.00,0.05);
    pad1->SetFillStyle(0);
    TVirtualPad *pad2 = c1->GetPad(2);
    pad2->SetPad(0.0,0.0,1.0,0.35);
    pad2->SetMargin(0.13,0.05,0.20,0.00);
    pad2->SetFillStyle(0);

    float defaultRatioYmin = 0.;
    float defaultRatioYmax = MaxY;
    if(line!=0){
	line->SetLineColor(kRed);
	line->SetLineWidth(2);
    }
   
    //Top
    c1->cd(1);
    float Ymax = 1.2*YMax(histoTop);
    if(histoTop.size()>0) {
    	histoTop[0]->Draw("");
    	histoTop[0]->SetTitle(0);
	histoTop[0]->GetYaxis()->SetRangeUser(0,Ymax);
	histoTop[0]->GetYaxis()->SetLabelFont(43);
	histoTop[0]->GetYaxis()->SetLabelSize(18);
	histoTop[0]->GetYaxis()->SetTitleSize(histoTop[0]->GetYaxis()->GetTitleSize()/0.65);
	histoTop[0]->GetYaxis()->SetTitleOffset(histoTop[0]->GetYaxis()->GetTitleOffset()*0.65);
	if(label!=string("")) histoTop[0]->GetXaxis()->SetTitle(label.c_str());
    }
    for(unsigned int i=1;i<histoTop.size();i++){
    	//histoTop[i]->Draw("esame");
    	histoTop[i]->Draw("same");
    }
    if(leg!=0) leg->Draw("same");
    //Bottom
    c1->cd(2);
    //c1->cd(2)->SetLogy();
    if(histoBottom.size()>0){
   	histoBottom[0]->SetTitle(0);
   	//histoBottom[0]->GetXaxis()->Clear();
   	//histoBottom[0]->GetXaxis()->SetNdivisions(0);
   	//histoBottom[0]->GetYaxis()->SetNdivisions(5);
	histoBottom[0]->GetXaxis()->SetLabelFont(43);
	histoBottom[0]->GetXaxis()->SetLabelSize(18);
	histoBottom[0]->GetXaxis()->SetTitleOffset(histoBottom[0]->GetXaxis()->GetTitleOffset()*0.65);
	histoBottom[0]->GetXaxis()->SetTitleSize(histoBottom[0]->GetXaxis()->GetTitleSize()/0.45);
   	histoBottom[0]->GetYaxis()->SetRangeUser(defaultRatioYmin,defaultRatioYmax);
	histoBottom[0]->GetYaxis()->SetLabelFont(43);
	histoBottom[0]->GetYaxis()->SetLabelSize(18);
	histoBottom[0]->GetYaxis()->SetTitleSize(histoBottom[0]->GetYaxis()->GetTitleSize()/0.45);
	histoBottom[0]->GetYaxis()->SetTitleOffset(histoBottom[0]->GetYaxis()->GetTitleOffset()*0.45);
    	histoBottom[0]->Draw("E1");
    	if(line!=0)line->SetX1(histoBottom[0]->GetXaxis()->GetXmin());
    	if(line!=0)line->SetX2(histoBottom[0]->GetXaxis()->GetXmax());
    	if(line!=0)line->Draw("esame");
	if(doFit) histoBottom[0]->Fit("pol0","","same");
    }
    for(unsigned int i=1;i<histoBottom.size();i++){
    	histoBottom[i]->Draw("esame");
    }


    return c1;
}

TCanvas* TCanvasCreator(THStack* histoTop, TH1F* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit){
    TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
    if(canvasName!=string("")) c1->SetName(canvasName.c_str());
    //c1->Range(0,0,1,1);
    c1->Divide(1,2);
    TVirtualPad *pad1 = c1->GetPad(1);
    pad1->SetPad(0.0,0.35,1.0,1.0);
    pad1->SetMargin(0.13,0.05,0.00,0.05);
    pad1->SetFillStyle(0);
    TVirtualPad *pad2 = c1->GetPad(2);
    pad2->SetPad(0.0,0.0,1.0,0.35);
    pad2->SetMargin(0.13,0.05,0.20,0.00);
    pad2->SetFillStyle(0);

    float defaultRatioYmin = 0.;
    float defaultRatioYmax = MaxY;
    if(line!=0){
	line->SetLineColor(kRed);
	line->SetLineWidth(2);
    }
   
    //Top
    c1->cd(1);
    float Ymax = 1.2*histoTop->GetMaximum();
    if(histoTop!=0) {
    	histoTop->Draw("");
    	histoTop->SetTitle(0);
	histoTop->GetYaxis()->SetRangeUser(0,Ymax);
	histoTop->GetYaxis()->SetLabelFont(43);
	histoTop->GetYaxis()->SetLabelSize(18);
	histoTop->GetYaxis()->SetTitleSize(histoTop->GetYaxis()->GetTitleSize()/0.65);
	histoTop->GetYaxis()->SetTitleOffset(histoTop->GetYaxis()->GetTitleOffset()*0.65);
	if(label!=string("")) histoTop->GetXaxis()->SetTitle(label.c_str());
    }
    if(leg!=0) leg->Draw("same");
    //Bottom
    c1->cd(2);
    //c1->cd(2)->SetLogy();
    if(histoBottom!=0){
   	histoBottom->SetTitle(0);
	histoBottom->SetStats(0);
   	//histoBottom->GetXaxis()->Clear();
   	//histoBottom->GetXaxis()->SetNdivisions(0);
   	//histoBottom->GetYaxis()->SetNdivisions(5);
	histoBottom->GetXaxis()->SetLabelFont(43);
	histoBottom->GetXaxis()->SetLabelSize(18);
	histoBottom->GetXaxis()->CenterLabels();
	histoBottom->GetXaxis()->SetTitleSize(histoBottom->GetXaxis()->GetTitleSize()/0.45);
	histoBottom->GetXaxis()->SetTitleOffset(histoBottom->GetXaxis()->GetTitleOffset()*0.65);
   	histoBottom->GetYaxis()->SetRangeUser(defaultRatioYmin,defaultRatioYmax);
	histoBottom->GetYaxis()->SetLabelFont(43);
	histoBottom->GetYaxis()->SetLabelSize(18);
	histoBottom->GetYaxis()->SetTitleSize(histoBottom->GetYaxis()->GetTitleSize()/0.45);
	histoBottom->GetYaxis()->SetTitleOffset(histoBottom->GetYaxis()->GetTitleOffset()*0.45);
    	histoBottom->Draw("E1");
    	if(line!=0)line->SetX1(histoBottom->GetXaxis()->GetXmin());
    	if(line!=0)line->SetX2(histoBottom->GetXaxis()->GetXmax());
    	if(line!=0)line->Draw("esame");
	if(doFit) histoBottom->Fit("pol0","IREM");
    }

    return c1;
}

TCanvas* TCanvasCreator(TH1F* histoTop, TGraphAsymmErrors* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit){
    TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
    if(canvasName!=string("")) c1->SetName(canvasName.c_str());
    //c1->Range(0,0,1,1);
    c1->Divide(1,2);
    TVirtualPad *pad1 = c1->GetPad(1);
    pad1->SetPad(0.0,0.35,1.0,1.0);
    pad1->SetMargin(0.13,0.05,0.00,0.05);
    pad1->SetFillStyle(0);
    TVirtualPad *pad2 = c1->GetPad(2);
    pad2->SetPad(0.0,0.0,1.0,0.35);
    pad2->SetMargin(0.13,0.05,0.20,0.00);
    pad2->SetFillStyle(0);

    float defaultRatioYmin = 0.;
    float defaultRatioYmax = MaxY;
    if(line!=0){
	line->SetLineColor(kRed);
	line->SetLineWidth(2);
    }
   
    //Top
    c1->cd(1);
    float Ymax = 1.2*histoTop->GetMaximum();
    if(histoTop!=0) {
    	histoTop->Draw("");
    	histoTop->SetTitle(0);
	histoTop->GetYaxis()->SetRangeUser(0,Ymax);
	histoTop->GetYaxis()->SetLabelFont(43);
	histoTop->GetYaxis()->SetLabelSize(18);
	histoTop->GetYaxis()->SetTitleSize(histoTop->GetYaxis()->GetTitleSize()/0.65);
	histoTop->GetYaxis()->SetTitleOffset(histoTop->GetYaxis()->GetTitleOffset()*0.65);
	if(label!=string("")) histoTop->GetXaxis()->SetTitle(label.c_str());
    }
    if(leg!=0) leg->Draw("same");
    //Bottom
    c1->cd(2);
    //c1->cd(2)->SetLogy();
    if(histoBottom!=0){
   	histoBottom->SetTitle(0);
	//histoBottom->SetStats(0);
 	//histoBottom->SetFillColor(kBlue);
	//histoBottom->SetFillStyle(3005);
    	//histoBottom->Draw("a4");
	histoBottom->Draw("ap");
   	histoBottom->GetXaxis()->SetNdivisions(515);
	histoBottom->GetXaxis()->SetLabelFont(43);
	histoBottom->GetXaxis()->SetLabelSize(18);
	histoBottom->GetXaxis()->CenterLabels();
	histoBottom->GetXaxis()->SetTitleSize(histoBottom->GetXaxis()->GetTitleSize()/0.45);
	histoBottom->GetXaxis()->SetTitleOffset(histoBottom->GetXaxis()->GetTitleOffset()*0.65);
   	histoBottom->GetXaxis()->SetRangeUser(histoTop->GetXaxis()->GetXmin(),histoTop->GetXaxis()->GetXmax());
   	if(MaxY!=-1)histoBottom->GetYaxis()->SetRangeUser(defaultRatioYmin,defaultRatioYmax);
	histoBottom->GetYaxis()->SetLabelFont(43);
	histoBottom->GetYaxis()->SetLabelSize(18);
	histoBottom->GetYaxis()->SetTitleSize(histoBottom->GetYaxis()->GetTitleSize()/0.45);
	histoBottom->GetYaxis()->SetTitleOffset(histoBottom->GetYaxis()->GetTitleOffset()*0.45);
    	if(line!=0)line->SetX1(histoBottom->GetXaxis()->GetXmin());
    	if(line!=0)line->SetX2(histoBottom->GetXaxis()->GetXmax());
    	if(line!=0)line->Draw("esame");
	if(doFit) histoBottom->Fit("pol0","EX0");
    }

    return c1;
}

TCanvas* TCanvasCreator(THStack* histoTop, TGraphAsymmErrors* histoBottom, TLine* line, TLegend* leg, string label, string canvasName, float MaxY, bool doFit){
    TCanvas *c1 = new TCanvas("c1", "c1",0,0,600,600);
    if(canvasName!=string("")) c1->SetName(canvasName.c_str());
    //c1->Range(0,0,1,1);
    c1->Divide(1,2);
    TVirtualPad *pad1 = c1->GetPad(1);
    pad1->SetPad(0.0,0.35,1.0,1.0);
    pad1->SetMargin(0.13,0.05,0.00,0.05);
    pad1->SetFillStyle(0);
    TVirtualPad *pad2 = c1->GetPad(2);
    pad2->SetPad(0.0,0.0,1.0,0.35);
    pad2->SetMargin(0.13,0.05,0.20,0.00);
    pad2->SetFillStyle(0);

    float defaultRatioYmin = 0.;
    float defaultRatioYmax = MaxY;
    if(line!=0){
	line->SetLineColor(kRed);
	line->SetLineWidth(2);
    }
   
    //Top
    c1->cd(1);
    float Ymax = 1.2*histoTop->GetMaximum();
    if(histoTop!=0) {
    	histoTop->Draw("");
    	histoTop->SetTitle(0);
	histoTop->GetYaxis()->SetRangeUser(0,Ymax);
	histoTop->GetYaxis()->SetLabelFont(43);
	histoTop->GetYaxis()->SetLabelSize(18);
	histoTop->GetYaxis()->SetTitleSize(histoTop->GetYaxis()->GetTitleSize()/0.65);
	histoTop->GetYaxis()->SetTitleOffset(histoTop->GetYaxis()->GetTitleOffset()*0.65);
	if(label!=string("")) histoTop->GetXaxis()->SetTitle(label.c_str());
    }
    if(leg!=0) leg->Draw("same");
    //Bottom
    c1->cd(2);
    //c1->cd(2)->SetLogy();
    if(histoBottom!=0){
   	histoBottom->SetTitle(0);
	//histoBottom->SetStats(0);
	//histoBottom->SetFillColor(kBlue);
	//histoBottom->SetFillStyle(3005);
    	//histoBottom->Draw("a4");
	histoBottom->Draw("ap");
   	histoBottom->GetXaxis()->SetNdivisions(515);
	histoBottom->GetXaxis()->SetLabelFont(43);
	histoBottom->GetXaxis()->SetLabelSize(18);
	histoBottom->GetXaxis()->CenterLabels();
	histoBottom->GetXaxis()->SetTitleSize(histoBottom->GetXaxis()->GetTitleSize()/0.45);
	histoBottom->GetXaxis()->SetTitleOffset(histoBottom->GetXaxis()->GetTitleOffset()*0.65);
   	histoBottom->GetXaxis()->SetRangeUser(histoTop->GetXaxis()->GetXmin(),histoTop->GetXaxis()->GetXmax());
   	if(MaxY!=-1)histoBottom->GetYaxis()->SetRangeUser(defaultRatioYmin,defaultRatioYmax);
	histoBottom->GetYaxis()->SetLabelFont(43);
	histoBottom->GetYaxis()->SetLabelSize(18);
	histoBottom->GetYaxis()->SetTitleSize(histoBottom->GetYaxis()->GetTitleSize()/0.45);
	histoBottom->GetYaxis()->SetTitleOffset(histoBottom->GetYaxis()->GetTitleOffset()*0.45);
    	if(line!=0)line->SetX1(histoBottom->GetXaxis()->GetXmin());
    	if(line!=0)line->SetX2(histoBottom->GetXaxis()->GetXmax());
    	if(line!=0)line->Draw("esame");
	if(doFit) histoBottom->Fit("pol0","EX0");
    }

    return c1;
}

TCanvas* TwoScales(TH1F* h1, TH1F* h2, string label)
{
   TCanvas *c1 = new TCanvas(label.c_str(),"",600,600);
   //((TPad*)c1->GetPadPointer())->SetTicky(0);
   h1->Draw();
   c1->Update();  
   //scale h2 to the pad coordinates
   TH1F* h2Clone = (TH1F*) h2->Clone();
   Float_t rightmax = 1.05*h2Clone->GetMaximum();
   Float_t scale = c1->GetUymax()/rightmax;
   h2Clone->SetLineColor(kRed);
   if(h2Clone->GetSumw2N()==0) h2Clone->Sumw2();
   h2Clone->Scale(scale);
   h2Clone->Draw("hsame");
   
   //draw an axis on the right side
   //TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
   TGaxis *axis = new TGaxis(c1->GetUxmax(),c1->GetUymin(),c1->GetUxmax(), c1->GetUymax(),0,rightmax,510,"+L");
   axis->SetLineColor(kRed);
   axis->SetLabelColor(kRed);
   axis->SetLabelSize(0.03);
   axis->SetTitle(h2Clone->GetYaxis()->GetTitle());
   axis->Draw();

   return c1;
}

TCanvas* TwoScales(TH1F* h1, TH1F* h2, vector<TH1F*> histos, string label, float LeftMax, float RightMax, bool scaled)
{
   TCanvas *c1 = new TCanvas(label.c_str());
   if(LeftMax>0) h1->GetYaxis()->SetRangeUser(0.,LeftMax);
   h1->Draw();
   if(scaled) c1->SetLogy();
   c1->Update();  
   //scale h2 to the pad coordinates
   TH1F* h2Clone = (TH1F*) h2->Clone();
   Float_t rightmax = 0;
   if(RightMax>0) rightmax = RightMax;
   else rightmax = 1.1*h2Clone->GetMaximum();
   Float_t scale = c1->GetUymax()/rightmax;
   h2Clone->SetLineColor(kRed);
   if(h2Clone->GetSumw2N()==0) h2Clone->Sumw2();
   h2Clone->Scale(scale);
   h2Clone->Draw("same");

   for(unsigned int i=0;i<histos.size();i++){
   	TH1F* clone = (TH1F*) histos[i]->Clone();
	if(clone->GetSumw2N()==0) clone->Sumw2();
	clone->Scale(scale);
	clone->Draw("same");
   }
   
   //draw an axis on the right side
   //TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
   TGaxis *axis = new TGaxis(c1->GetUxmax(),c1->GetUymin(),c1->GetUxmax(), c1->GetUymax(),0,rightmax,510,"+L");
   axis->SetLineColor(kRed);
   axis->SetLabelColor(kRed);
   axis->SetTitle(h2Clone->GetYaxis()->GetTitle());
   axis->Draw();

   return c1;
}
