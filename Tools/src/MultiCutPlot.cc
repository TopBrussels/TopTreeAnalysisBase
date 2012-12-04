#include "../interface/MultiCutPlot.h"



MultiCutPlot::MultiCutPlot(vector<pair<string,int> > cuts, string PlotName, int Nbins, float Min, float Max, string XaxisLabel, string YaxisLabel){
	for(unsigned int i=0;i<cuts.size();i++){
		string histoName = PlotName+"_"+cuts[i].first;
		TH1F* h = new TH1F(histoName.c_str(),"",Nbins,Min,Max);
		h->Sumw2();
		h->GetXaxis()->SetTitle(XaxisLabel.c_str());
		h->GetYaxis()->SetTitle(YaxisLabel.c_str());
		plots_.push_back(pair<TH1F*,string>(h,cuts[i].first));
		color_.push_back(cuts[i].second);
	}
}

MultiCutPlot::MultiCutPlot(vector<pair<string,int> > cuts, string PlotName, int Nbins, float* binsX, string XaxisLabel, string YaxisLabel){
	for(unsigned int i=0;i<cuts.size();i++){
		string histoName = PlotName+"_"+cuts[i].first;
		TH1F* h = new TH1F(histoName.c_str(),"",Nbins,binsX);
		h->Sumw2();
		h->GetXaxis()->SetTitle(XaxisLabel.c_str());
		h->GetYaxis()->SetTitle(YaxisLabel.c_str());
		plots_.push_back(pair<TH1F*,string>(h,cuts[i].first));
		color_.push_back(cuts[i].second);
	}
}

MultiCutPlot::MultiCutPlot(vector<pair<TH1F*,string> > vec, vector<int> color){
	plots_ = vec;
	color_ = color;
}

MultiCutPlot::~MultiCutPlot(){
	if(hCanvas_) delete hCanvas_;
	if(leg_) delete leg_;

}

void MultiCutPlot::Fill(float value, string cut){
	for(unsigned int i=0;i<plots_.size();i++){
		if(cut==plots_[i].second) plots_[i].first->Fill(value);
	}
}

void MultiCutPlot::Draw(string label, float YMax){
	vector<TH1F*> histos;
	vector<string> cuts;
	leg_ = new TLegend(0.75,0.65,0.88,0.88);
	for(unsigned int i=0;i<plots_.size();i++){
		plots_[i].first->SetLineWidth(2);
		plots_[i].first->SetLineColor(color_[i]);
		histos.push_back(plots_[i].first);
		cuts.push_back(plots_[i].second);
		leg_->AddEntry(histos[i],cuts[i].c_str(),"l");
	}
	string name;
	name = "Canvas_"+label;

	//hCanvas_ = TCanvasCreator(histos, datasets, leg_, string("l"), name);
	vector<TH1F*> histoRatio;
	if(histos.size()>0) {
		func_ = new TF1("func","[0]*x+[1]",histos[0]->GetXaxis()->GetXmin(), histos[0]->GetXaxis()->GetXmax());
		if(histos[0]->Integral()>0) histos[0]->Scale(1/histos[0]->Integral());
	}
	for(unsigned int i=1;i<histos.size();i++){
		if(histos[i]->Integral()) histos[i]->Scale(1/histos[i]->Integral());
		TH1F* h  = PlotRatio(histos[0],histos[i], func_, cuts[i]);
		h->GetXaxis()->SetTitle("Ratio");
		h->SetLineColor(histos[i]->GetLineColor());
		h->SetLineWidth(histos[i]->GetLineWidth());
		histoRatio.push_back(h);
	}
	TLine* line=0;
	hCanvas_ = TCanvasCreator(histos, histoRatio, line, leg_, string(""), label, YMax);

}

void MultiCutPlot::TailEstimation(float cutX0, string label){
	int binCut = 1;
	int NbinsX = 2;
	if(plots_.size()>0){
		int NbinsX = plots_[0].first->GetNbinsX()+1;
		float Xaxis_min = plots_[0].first->GetXaxis()->GetXmin();
		float Xaxis_max = plots_[0].first->GetXaxis()->GetXmax();
		binCut = (int)(NbinsX*(cutX0-Xaxis_min)/(Xaxis_max-Xaxis_min));
	}
	cout<<"-----------------------------------------------------"<<endl;
	cout<<"Estimation for the tail of "<<label<<" for different cuts:"<<endl;
	float estimationRef = 0;
	for(unsigned int i=0;i<plots_.size();i++){
		float estimation = plots_[i].first->Integral(binCut,NbinsX);
		float error = 0;
		for(int j=binCut;j<NbinsX;j++) error+=plots_[i].first->GetBinError(j);
		float diff = 0;
		if(i ==0 ) estimationRef = estimation;
		if(i>0) diff = fabs(estimation-estimationRef)/estimationRef*100; 
		cout<<plots_[i].second<<" "<<estimation<<" +/- "<<error<<" diff "<<diff<<"%"<<endl;
	}
	cout<<"-----------------------------------------------------"<<endl;
}

void MultiCutPlot::Write(TFile* fout,string label){
	fout->cd();
        string dirname = "MultiCut_"+label;
	if(fout->Get(dirname.c_str())==0)
        	fout->mkdir(dirname.c_str());
	fout->cd(dirname.c_str());
	string subdirname = dirname+"/Plots";
	if(fout->Get(subdirname.c_str())==0)
		((TDirectory*)(fout->Get(dirname.c_str())))->mkdir("Plots");
	fout->cd(subdirname.c_str());
	if(hCanvas_) hCanvas_->Write();
}


