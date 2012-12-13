#include "../interface/PlotTopMatching.h"


  
  
PlotTopMatching::PlotTopMatching(){
      NofEvts_ = 0;
      NofEvtsSel_ = 0;
      h_Matching = NULL; 
      h_Matching_Sel = NULL; 
      h_MatchingHadTop = NULL; 
      h_MatchingHadTop_Sel = NULL; 
} 

PlotTopMatching::PlotTopMatching(vector<string> jetCollection, vector<string> jetLabels){
      jetLabels_ = jetLabels;
      NofEvts_ = new int[jetCollection.size()];
      NofEvtsSel_ = new int[jetCollection.size()];
      for(unsigned int i=0;i<jetCollection.size();i++) NofEvts_[i] = 0;
      for(unsigned int i=0;i<jetCollection.size();i++) NofEvtsSel_[i] = 0;
      h_Matching = new TH1F("h_Matching","Matching",jetCollection.size(),0,jetCollection.size()); 
      h_Matching_Sel = new TH1F("h_Matching_Sel","Matching_Sel",jetCollection.size(),0,jetCollection.size()); 
      h_MatchingHadTop = new TH1F("h_MatchingHadTop","MatchingHadTop",jetCollection.size(),0,jetCollection.size()); 
      h_MatchingHadTop_Sel = new TH1F("h_MatchingHadTop_Sel","MatchingHadTop_Sel",jetCollection.size(),0,jetCollection.size()); 
      //RMS
      h_WMass_RMS = new TH1F("WMass_RMS","WMass_RMS",jetCollection.size(),0,jetCollection.size());
      h_TopMass_RMS = new TH1F("Top_Mass_RMS","TopMass_RMS",jetCollection.size(),0,jetCollection.size());
      //Mean
      h_WMass_Mean = new TH1F("WMass_Mean","WMass_Mean",jetCollection.size(),0,jetCollection.size());
      h_TopMass_Mean = new TH1F("TopMass_Mean","TopMass_Mean",jetCollection.size(),0,jetCollection.size());
      //Res
      h_WMass_Res = new TH1F("WMass_Res","WMass_Res",jetCollection.size(),0,jetCollection.size());
      h_TopMass_Res = new TH1F("TopMass_Res","TopMass_Res",jetCollection.size(),0,jetCollection.size());
      for(unsigned int i=0;i<jetCollection.size();i++){
        h_Matching->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
        h_Matching_Sel->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
        h_MatchingHadTop->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
        h_MatchingHadTop_Sel->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
	h_WMass_RMS->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
	h_WMass_Mean->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
	h_TopMass_RMS->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
	h_TopMass_Mean->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
	h_WMass_Res->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
	h_TopMass_Res->GetXaxis()->SetBinLabel(i+1,jetLabels_[i].c_str());
	char name[100];
	sprintf(name,"WMass_%s",jetCollection[i].c_str());
	TH1F* WMass = new TH1F(name,"WMass",100,0,200);
	h_WMass.push_back(WMass);
	sprintf(name,"TopMass_%s",jetCollection[i].c_str());
	TH1F* TopMass = new TH1F(name,"TopMass",200,0,400);
	h_TopMass.push_back(TopMass);
	sprintf(name,"MassLeadingJet_%s",jetCollection[i].c_str());
	TH1F* MassLeadingJet = new TH1F(name, name,100,0,400);
        h_MassLeadingJet.push_back(MassLeadingJet);
      }
      c_MassLeadingJet = new TCanvas("c_MassLeadingJet");
} 

void PlotTopMatching::AddObsParam(vector<string> jetCollection, string obsName, string obsLabel, int Nbins, float* bins){
    obsLabels_.push_back(obsLabel);
    vector<TH1F*> h_MatchingHadTop_vs_Var_New;
    vector<TH1F*> h_MatchingHadTop_vs_Var_Ref_New;
    TCanvas* c_MatchingHadTop_Var_New;
    for(unsigned int i=0;i<jetCollection.size();i++){
	char name[100];
	sprintf(name,"c_MatchHadTop_vs_%s",obsName.c_str());
	c_MatchingHadTop_Var_New = new TCanvas(name);
	sprintf(name,"MatchHadTop_vs_%s_%s",jetCollection[i].c_str(),obsName.c_str());
	TH1F* MatchHadTopvsVar = new TH1F(name,name,Nbins,bins);
        h_MatchingHadTop_vs_Var_New.push_back(MatchHadTopvsVar);
	sprintf(name,"MatchHadTop_vs_%s_%s_Ref",jetCollection[i].c_str(),obsName.c_str());
	TH1F* MatchHadTopvsVar_Ref = new TH1F(name,name,Nbins,bins);
        h_MatchingHadTop_vs_Var_Ref_New.push_back(MatchHadTopvsVar_Ref);
    }
    h_MatchingHadTop_vs_Var.push_back(h_MatchingHadTop_vs_Var_New);
    h_MatchingHadTop_vs_Var_Ref.push_back(h_MatchingHadTop_vs_Var_Ref_New);
    c_MatchingHadTop_Var.push_back(c_MatchingHadTop_Var_New);
}

PlotTopMatching::PlotTopMatching(const PlotTopMatching& psel){
      jetLabels_ = psel.jetLabels_;
      obsLabels_ = psel.obsLabels_;
      NofEvts_ = psel.NofEvts_;
      NofEvtsSel_ = psel.NofEvtsSel_;
      h_Matching = psel.h_Matching; 
      h_Matching_Sel = psel.h_Matching_Sel; 
      h_MatchingHadTop = psel.h_MatchingHadTop; 
      h_MatchingHadTop_Sel = psel.h_MatchingHadTop_Sel;
      h_MassLeadingJet = psel.h_MassLeadingJet;
      c_MassLeadingJet = psel.c_MassLeadingJet;
      h_WMass = psel.h_WMass;
      h_TopMass = psel.h_TopMass;
      h_WMass_RMS = psel.h_WMass_RMS;
      h_WMass_Mean = psel.h_WMass_Mean;
      h_WMass_Res = psel.h_WMass_Res;
      h_MatchingHadTop_vs_Var = psel.h_MatchingHadTop_vs_Var;
      h_MatchingHadTop_vs_Var_Ref = psel.h_MatchingHadTop_vs_Var_Ref;
      c_MatchingHadTop_Var = psel.c_MatchingHadTop_Var;
}

PlotTopMatching::~PlotTopMatching(){;}

void PlotTopMatching::Fill(TopMatching* topmatch, vector<TRootJet>& jets, TRootGenEvent& genEvt, int& iter, bool& selected){
	NofEvts_[iter]++;
	if(selected) NofEvtsSel_[iter]++;
	float wmass = -888.;
	float topmass = -888.;
	if(topmatch->isMCMatched(jets,genEvt,wmass,topmass)){
		h_Matching->Fill(iter);	
		if(selected) h_Matching->Fill(iter);	
		//for(unsigned int i=0;i<h_WMass.size();i++){
			h_WMass[iter]->Fill(wmass);
			h_TopMass[iter]->Fill(topmass);
		//}
	}
	if(topmatch->isMCMatchedHadTop(jets,genEvt,wmass,topmass)){
		h_MatchingHadTop->Fill(iter);	
		if(selected) h_MatchingHadTop_Sel->Fill(iter);	
		//for(unsigned int i=0;i<h_WMass.size();i++){
			h_WMass[iter]->Fill(wmass);
			h_TopMass[iter]->Fill(topmass);
		//}
	}
	if(jets.size()>0 && jets[0].Pt()>500) h_MassLeadingJet[iter]->Fill(jets[0].M());
}

void PlotTopMatching::Fill(TopMatching* topmatch, vector<TRootJet>& jets, TRootNPGenEvent& genEvt, int& iter, bool& selected){
	NofEvts_[iter]++;
	if(selected) NofEvtsSel_[iter]++;
	float wmass = -888.;
	float topmass = -888.;
	if(topmatch->isMCMatchedHadTop(jets,genEvt,wmass,topmass)){
		h_MatchingHadTop->Fill(iter);	
		if(selected) h_MatchingHadTop_Sel->Fill(iter);	
		//for(unsigned int i=0;i<h_WMass.size();i++)
			h_WMass[iter]->Fill(wmass);
			h_TopMass[iter]->Fill(topmass);
		//}
	}
	if(jets.size()>0 && jets[0].Pt()>500) h_MassLeadingJet[iter]->Fill(jets[0].M());
}

void PlotTopMatching::FillObs(int nobs, TopMatching* topmatch, vector<TRootJet>& jets, TRootGenEvent& genEvt, int& iter, float& obs){
	if(nobs>=(int)h_MatchingHadTop_vs_Var.size()) cerr<<"Number of variable incorrect .. too high"<<endl;
	float wmass = -888.;
	float topmass = -888.;
	if(topmatch->isMCMatchedHadTop(jets,genEvt,wmass,topmass)){
		h_MatchingHadTop_vs_Var[nobs][iter]->Fill(obs);	
	}
	h_MatchingHadTop_vs_Var_Ref[nobs][iter]->Fill(obs);	
}

void PlotTopMatching::FillObs(int nobs, TopMatching* topmatch, vector<TRootJet>& jets, TRootNPGenEvent& genEvt, int& iter, float& obs){
	if(nobs>=(int)h_MatchingHadTop_vs_Var.size()) cerr<<"Number of variable incorrect .. too high"<<endl;
	float wmass = -888.;
	float topmass = -888.;
	if(topmatch->isMCMatchedHadTop(jets,genEvt,wmass,topmass)){
		h_MatchingHadTop_vs_Var[nobs][iter]->Fill(obs);	
	}
	h_MatchingHadTop_vs_Var_Ref[nobs][iter]->Fill(obs);	
}

void PlotTopMatching::Compute(){
   for(unsigned int i=0;i<h_WMass.size();i++){
	h_WMass_RMS->SetBinContent(i+1,h_WMass[i]->GetRMS());
      	h_TopMass_RMS->SetBinContent(i+1,h_TopMass[i]->GetRMS());
        //Fit distribution with gaussian arount the expected mean value ...
        //Output of the fit ... should go in the corresponding files  ...
   }
   for(unsigned int nobs=0;nobs<h_MatchingHadTop_vs_Var.size();nobs++){
   	for(unsigned int i=0;i<h_MatchingHadTop_vs_Var[nobs].size();i++){
   		for(int j=0;j<h_MatchingHadTop_vs_Var[nobs][i]->GetNbinsX();j++){
			float eff = 0; 
			float err = 0;
			if(h_MatchingHadTop_vs_Var_Ref[nobs][i]->GetBinContent(j)>0){
				eff = (float) h_MatchingHadTop_vs_Var[nobs][i]->GetBinContent(j+1)/h_MatchingHadTop_vs_Var_Ref[nobs][i]->GetBinContent(j+1);
				err = (float) sqrt(eff*(1-eff)/h_MatchingHadTop_vs_Var_Ref[nobs][i]->GetBinContent(j+1));
			}
			h_MatchingHadTop_vs_Var[nobs][i]->SetBinContent(j+1,eff);
   			h_MatchingHadTop_vs_Var[nobs][i]->SetBinError(j+1,err);
		}
	}
   }
}
  
void PlotTopMatching::Write(TFile* fout){
 Compute(); 

 for(unsigned nobs=0;nobs<h_MatchingHadTop_vs_Var.size();nobs++){
 	c_MatchingHadTop_Var[nobs]->cd();
	TLegend leg(0.1,0.2,0.4,0.5);
 	h_MatchingHadTop_vs_Var[nobs][0]->SetLineWidth(2);
 	h_MatchingHadTop_vs_Var[nobs][0]->SetLineColor(1);
 	h_MatchingHadTop_vs_Var[nobs][0]->GetYaxis()->SetRangeUser(0.,1.0);
 	h_MatchingHadTop_vs_Var[nobs][0]->GetYaxis()->SetTitle("Top matching purity");
 	h_MatchingHadTop_vs_Var[nobs][0]->GetXaxis()->SetTitle(obsLabels_[nobs].c_str());
 	h_MatchingHadTop_vs_Var[nobs][0]->Draw();
 	leg.AddEntry(h_MatchingHadTop_vs_Var[nobs][0],h_WMass_RMS->GetXaxis()->GetBinLabel(1),"l");
 	for(unsigned int i=1;i<h_MatchingHadTop_vs_Var[nobs].size();i++){
 		h_MatchingHadTop_vs_Var[nobs][i]->SetLineWidth(2);
 		h_MatchingHadTop_vs_Var[nobs][i]->SetLineColor(1+i);
 		h_MatchingHadTop_vs_Var[nobs][i]->Draw("same");
		//leg.AddEntry(h_MatchingHadTop_vs_Var[nobs][i],h_WMass_RMS->GetXaxis()->GetBinLabel(i+1),"l");
		leg.AddEntry(h_MatchingHadTop_vs_Var[nobs][i],jetLabels_[i].c_str(),"l");
 	}
 	leg.Draw("same");
 	c_MatchingHadTop_Var[nobs]->Write();
  }

 c_MassLeadingJet->cd();
 TLegend leg(0.1,0.2,0.4,0.5);
 h_MassLeadingJet[0]->SetLineWidth(2);
 h_MassLeadingJet[0]->SetLineColor(1);
 h_MassLeadingJet[0]->GetYaxis()->SetRangeUser(0.,1.0);
 h_MassLeadingJet[0]->GetYaxis()->SetTitle("U.A.");
 h_MassLeadingJet[0]->GetXaxis()->SetTitle("Mass of leading jet");
 h_MassLeadingJet[0]->Draw();
 leg.AddEntry(h_MassLeadingJet[0],h_WMass_RMS->GetXaxis()->GetBinLabel(1),"l");
 for(unsigned int i=1;i<h_MassLeadingJet.size();i++){
 	h_MassLeadingJet[i]->SetLineWidth(2);
 	h_MassLeadingJet[i]->SetLineColor(1+i);
 	h_MassLeadingJet[i]->Draw("same");
	leg.AddEntry(h_MassLeadingJet[i],jetLabels_[i].c_str(),"l");
 }
 leg.Draw("same");
 c_MassLeadingJet->Write();

 h_WMass_RMS->Write();
 h_WMass_Mean->Write();
 h_WMass_Res->Write();
 h_TopMass_RMS->Write();
 h_TopMass_Mean->Write();
 h_TopMass_Res->Write();
 
 for(int i=0;i<h_Matching->GetNbinsX();i++){
     float eff = 0;
     float err = 0;

     if(h_Matching->GetBinContent(i+1)>0) eff =(float) h_Matching->GetBinContent(i+1)/NofEvts_[i];
     if(eff>0) err = eff*sqrt(eff*(1-eff)/NofEvts_[i]);
     h_Matching->SetBinContent(i+1,eff);
     h_Matching->SetBinError(i+1,err);
  
     eff = 0;
     err = 0;
     if(h_Matching_Sel->GetBinContent(i+1)>0) eff =(float) h_Matching_Sel->GetBinContent(i+1)/NofEvtsSel_[i];
     if(eff>0) err = eff*sqrt(eff*(1-eff)/NofEvts_[i]);
     h_Matching_Sel->SetBinContent(i+1,eff);
     h_Matching_Sel->SetBinError(i+1,err);
  
     eff = 0;
     err = 0;
     if(h_MatchingHadTop->GetBinContent(i+1)>0) eff =(float) h_MatchingHadTop->GetBinContent(i+1)/NofEvts_[i];
     if(eff>0) err = eff*sqrt(eff*(1-eff)/NofEvts_[i]);
     h_MatchingHadTop->SetBinContent(i+1,eff);
     h_MatchingHadTop->SetBinError(i+1,err);
  
     eff = 0;
     err = 0;
     if(h_MatchingHadTop_Sel->GetBinContent(i+1)>0) eff =(float) h_MatchingHadTop_Sel->GetBinContent(i+1)/NofEvtsSel_[i];
     if(eff>0) err = eff*sqrt(eff*(1-eff)/NofEvts_[i]);
     h_MatchingHadTop_Sel->SetBinContent(i+1,eff);
     h_MatchingHadTop_Sel->SetBinError(i+1,err);
  }
  h_Matching->Write();
  h_Matching_Sel->Write();
  h_MatchingHadTop->Write();
  h_MatchingHadTop_Sel->Write();
  for(unsigned int i=0;i<h_WMass.size();i++)  h_WMass[i]->Write();
  for(unsigned int i=0;i<h_TopMass.size();i++)  h_TopMass[i]->Write();
  for(unsigned int i=0;i<h_MassLeadingJet.size();i++)  h_MassLeadingJet[i]->Write();
  for(unsigned int nobs=0;nobs<h_MatchingHadTop_vs_Var.size();nobs++)
  	for(unsigned int i=0;i<h_MatchingHadTop_vs_Var[i].size();i++) h_MatchingHadTop_vs_Var[nobs][i]->Write();
}
