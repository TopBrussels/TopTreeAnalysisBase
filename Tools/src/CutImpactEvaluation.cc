#include "../interface/CutImpactEvaluation.h"


CutImpactEvaluation::CutImpactEvaluation(string PlotName, int Nbins, float VarMin, float VarMax, int Nplots, float Min, float Max, vector<Dataset> bkg, string XaxisLabel, bool lowercut){
	Nplots_ = Nplots;
	lowercut_  = lowercut;
	plots_ = new TH1F*[Nplots];
	cuts_ = new float[Nplots];
	for(int i=0;i<Nplots;i++){
		std::stringstream ss;
		ss << i;
		string name = PlotName +"_" + ss.str();
		plots_[i] = new TH1F(name.c_str(),"",Nbins,VarMin,VarMax);
		cuts_[i] = i*((float)(Min-Max)/Nplots)+Min;	
	}
	string name = PlotName+"_ref";
	h_ref_ = new TH1F(name.c_str(),"",Nbins,VarMin,VarMax);
	name = PlotName+"_Deform";
	hDeformation_ = new TH1F(name.c_str(),"",Nplots,Min,Max);
	hDeformation_->GetXaxis()->SetTitle(XaxisLabel.c_str());
	hDeformation_->GetYaxis()->SetTitle("#Chi^{2}");
	name = PlotName+"_Eff";
	hEfficiency_ = new TH1F(name.c_str(),"",Nplots,Min,Max);
	hEfficiency_->GetXaxis()->SetTitle(XaxisLabel.c_str());
	hEfficiency_->GetYaxis()->SetTitle("Efficiency");
	hCanvas_ = 0;
	hCanvas2_ = 0;
	BkgList_ = bkg;
	for(unsigned int i=0;i<bkg.size();i++){
		string name;
		name = PlotName+"_rej_"+bkg[i].Name();
		TH1F* h = new TH1F(name.c_str(),"",Nplots,Min,Max);
		h->GetXaxis()->SetTitle(XaxisLabel.c_str());
		h->GetYaxis()->SetTitle("Efficiency");
		hRejection_.push_back(h);
		name = PlotName+"_effXrej_"+bkg[i].Name();
		TH1F* h2 = new TH1F(name.c_str(),"",Nplots,Min,Max);
		h2->GetXaxis()->SetTitle(XaxisLabel.c_str());
		h2->GetYaxis()->SetTitle("EffxRej");
		hEffxRej_.push_back(h2);
		name = PlotName+"_bkg_"+bkg[i].Name();
		TH1F* h3 = new TH1F(name.c_str(),"",Nplots,Min,Max);
		h3->GetXaxis()->SetTitle(XaxisLabel.c_str());
		h3->GetYaxis()->SetTitle("U.A.");
		hBkg_.push_back(h3);
	}
}

CutImpactEvaluation::CutImpactEvaluation(string PlotName, int Nbins, float* binsX, int Nplots, float Min, float Max, vector<Dataset> bkg, string XaxisLabel, bool lowercut){
	Nplots_ = Nplots;
	lowercut_ = lowercut;
	plots_ = new TH1F*[Nplots];
	cuts_ = new float[Nplots];
	for(int i=0;i<Nplots;i++){
		std::stringstream ss;
		ss << i;
		string name = PlotName +"_" + ss.str();
		plots_[i] = new TH1F(name.c_str(),"",Nbins,binsX);
		cuts_[i] = i*((float)(Max-Min)/Nplots)+Min;	
	}
	string name = PlotName+"_ref";
	h_ref_ = new TH1F(name.c_str(),"",Nbins,binsX);
	name = PlotName+"_Deform";
	hDeformation_ = new TH1F(name.c_str(),"",Nplots,Min,Max);
	hDeformation_->GetXaxis()->SetTitle(XaxisLabel.c_str());
	hDeformation_->GetYaxis()->SetTitle("#Chi^{2}");
	name = PlotName+"_Eff";
	hEfficiency_ = new TH1F(name.c_str(),"",Nplots,Min,Max);
	hEfficiency_->GetXaxis()->SetTitle(XaxisLabel.c_str());
	hEfficiency_->GetYaxis()->SetTitle("Efficiency");
	hCanvas_ = 0;
	hCanvas2_ = 0;
	BkgList_ = bkg;
	for(unsigned int i=0;i<bkg.size();i++){
		string name;
		name = PlotName+"_rej_"+bkg[i].Name();
		TH1F* h = new TH1F(name.c_str(),"",Nplots,Min,Max);
		hRejection_.push_back(h);
		name = PlotName+"_effXrej_"+bkg[i].Name();
		TH1F* h2 = new TH1F(name.c_str(),"",Nplots,Min,Max);
		hEffxRej_.push_back(h2);
		name = PlotName+"_bkg_"+bkg[i].Name();
		TH1F* h3 = new TH1F(name.c_str(),"",Nplots,Min,Max);
		hBkg_.push_back(h3);
	}
}

CutImpactEvaluation::~CutImpactEvaluation(){
	if(hCanvas_) delete hCanvas_;
	if(hCanvas2_) delete hCanvas2_;
	if(hDeformation_) delete hDeformation_;
	if(hEfficiency_) delete hEfficiency_;
	for(int i=0;i<Nplots_;i++){
		if(plots_[i]) delete plots_[i];
	}
	for(unsigned int i=0;i<BkgList_.size();i++){
		if(hRejection_[i]) delete hRejection_[i];
		if(hEffxRej_[i]) delete hEffxRej_[i];
		if(hBkg_[i]) delete hBkg_[i];
	}
	delete [] plots_;
	delete[] cuts_;
	delete h_ref_;
}


void CutImpactEvaluation::Fill(float value, float variable, bool isBkg, unsigned int iDataset){
	if(isBkg){
		if(iDataset<=hRejection_.size())hBkg_[iDataset]->Fill(value);		
	}
	else{
		h_ref_->Fill(variable);
		for(int i=0;i<Nplots_;i++){
			if(lowercut_) { if(value>cuts_[i]) plots_[i]->Fill(variable); }
			else {if(value<cuts_[i]) plots_[i]->Fill(variable);}
		}
	}
}


void CutImpactEvaluation::Draw(string label){
	for(int i=0;i<Nplots_;i++){
		float chi2 = 0;
		if(h_ref_->Integral()>0 && plots_[i]->Integral()>0) chi2 = Chi2Normalized(plots_[i],h_ref_);
		float eff = 0;
		//if(h_ref_->Integral()>0) eff = plots_[i]->Integral(1, plots_[i]->GetNbinsX()+1)/h_ref_->Integral(1,h_ref_->GetNbinsX()+1);
		if(h_ref_->Integral()>0) eff = plots_[i]->GetEntries()/h_ref_->GetEntries();
	  	hDeformation_->SetBinContent(i+1,chi2);	
		hEfficiency_->SetBinContent(i+1,eff);
		hEfficiency_->SetBinError(i+1,0);
	}
	for(unsigned int i=0;i<hBkg_.size();i++){
		float eff = 0;
		float integ = hBkg_[i]->Integral(1, Nplots_+1);
		for(int j=1;j<Nplots_+1;j++){
			if(lowercut_)	eff = hBkg_[i]->Integral(1,j)/integ;
			else eff = hBkg_[i]->Integral(j,hBkg_[i]->GetNbinsX()+1)/integ;
			hRejection_[i]->SetBinContent(j,eff);
		}
	}
	for(unsigned int i=0;i<hRejection_.size();i++) hRejection_[i]->SetLineColor(BkgList_[i].Color());
	hCanvas_ = TwoScales(hDeformation_, hEfficiency_, hRejection_, label, 10., 1.1);
	TLegend* leg = new TLegend(0.4,0.7,0.6,0.88);
	TLegend* leg2 = new TLegend(0.4,0.7,0.6,0.88);
	leg->AddEntry(hDeformation_,"#Chi^{2}","l");
	hEfficiency_->SetLineColor(2);
	leg->AddEntry(hEfficiency_,"Efficiency","l");
	for(unsigned int i=0;i<hRejection_.size();i++){
		leg->AddEntry(hRejection_[i],BkgList_[i].Name().c_str(),"l");
		leg2->AddEntry(hRejection_[i],BkgList_[i].Name().c_str(),"l");
	}	
	leg->Draw("same");
	
	//fill canvas2 with effxrej
	string cname    = label+"_EffxRej";
	hCanvas2_ = new TCanvas(cname.c_str());
	hCanvas2_->cd();
	for(unsigned int i=0;i<hEffxRej_.size();i++) hEffxRej_[i]->SetLineColor(BkgList_[i].Color());
	for(unsigned int i=0;i<hBkg_.size();i++){
		for(int j=1;j<Nplots_+1;j++){
			float value = 0;
			if(hRejection_[i]->GetBinContent(j)!=0 && hEfficiency_->GetBinContent(j)!=0) value = hRejection_[i]->GetBinContent(j)*hEfficiency_->GetBinContent(j);
			hEffxRej_[i]->SetBinContent(j,value);
		}
		if(i==0) hEffxRej_[i]->Draw();
		else hEffxRej_[i]->Draw("same");
	}
	leg2->Draw("same");
}

pair<float,float> CutImpactEvaluation::GetInformation(float cut){
	int ibin = (int)(hDeformation_->GetNbinsX()*(cut-hDeformation_->GetXaxis()->GetXmin())/(hDeformation_->GetXaxis()->GetXmax() - hDeformation_->GetXaxis()->GetXmin()));
	pair<float,float> output = pair<float,float>(hDeformation_->GetBinContent(ibin), hEfficiency_->GetBinContent(ibin));
	return output;
}

pair<float,float> CutImpactEvaluation::GetCutForAGivenEfficiency(float eff){
	int ibin = 1 ;
	for(int i=2;i<hEfficiency_->GetNbinsX();i++){
		if(hEfficiency_->GetBinContent(i-1)< eff && hEfficiency_->GetBinContent(i+1)>eff){
			ibin = i; break;
		}
		if(hEfficiency_->GetBinContent(i-1)> eff && hEfficiency_->GetBinContent(i+1)<eff){
			ibin = i; break;
		}
	}
	return (pair<float,float>(hEfficiency_->GetBinCenter(ibin),hDeformation_->GetBinContent(ibin)));
}

pair<float,float> CutImpactEvaluation::GetCutForAGivenChi2(float chi2){
	int ibin = 1 ;
	for(int i=2;i<hDeformation_->GetNbinsX();i++){
		if(hDeformation_->GetBinContent(i-1)< chi2 && hDeformation_->GetBinContent(i+1)>chi2){
			ibin = i; break;
		}
		if(hDeformation_->GetBinContent(i-1)> chi2 && hDeformation_->GetBinContent(i+1)<chi2){
			ibin = i; break;
		}
	}
	return (pair<float,float> (hDeformation_->GetBinCenter(ibin),hEfficiency_->GetBinContent(ibin)));
}

void CutImpactEvaluation::Write(TFile* fout, string label){
	 string dirname = "TtJetEstimation"+label;
	 if(fout->Get(dirname.c_str())==0)	fout->mkdir(dirname.c_str());
	 fout->cd(dirname.c_str());
	 string subdirname = dirname+"/Plots";
	 if(fout->Get(subdirname.c_str())==0) ((TDirectory*)(fout->Get(dirname.c_str())))->mkdir("Plots");
	 fout->cd(subdirname.c_str());
	 hDeformation_->Write();
	 hEfficiency_->Write();
	 if(hCanvas_) hCanvas_->Write();
	 if(hCanvas2_) hCanvas2_->Write();
	 for(unsigned int i=0;i<hRejection_.size();i++){
	 	hRejection_[i]->Write();
	 }
}

																			
