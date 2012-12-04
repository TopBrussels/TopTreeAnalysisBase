#include "../interface/TemplateComparator.h"

using namespace std;


TemplateComparator::TemplateComparator(vector<TH1F*> histos_cat1, vector<TH1F*> histos_cat2, vector<string> obsNames){
	hcat1 = histos_cat1;
	hcat2 = histos_cat2;
	ObsNames = obsNames;

	if(hcat1.size() != hcat2.size()) cerr<<"list of vectors doesn't have the same size !! Correct it"<<endl;
	if(hcat1.size() != ObsNames.size()) cerr<<"list of observable names is not correct !!"<<endl;
	MomentNames.push_back("Mean");
	MomentNames.push_back("RMS");
	MomentNames.push_back("Skewness");
	MomentNames.push_back("Kurtosis");
	//add other ...

	TestNames.push_back("Chi2");
	//add other ...
}

TemplateComparator::TemplateComparator(const TemplateComparator& tc){
	MomentNames = tc.MomentNames;
	TestNames = tc.TestNames;
	hcat1 = tc.hcat1;
	hcat2 = tc.hcat2;
	mcat1 = tc.mcat1;
	mcat2 = tc.mcat2;
	tests = tc.tests;
}

void TemplateComparator::AddNormalisation(pair<float,float> cat1, pair<float,float> cat2){
	ncat1 = cat1;
	ncat2 = cat2;
}


void TemplateComparator::Compute(){
	vector<pair<float,float> > vvalue;
	//fill moments
	pair<float,float> a;
	for(unsigned int i=0;i<hcat1.size();i++){
		vvalue.clear();
		//respect that order - as in constructor
		a = make_pair(hcat1[i]->GetMean(),hcat1[i]->GetMeanError());
		vvalue.push_back(a);
		a = make_pair(hcat1[i]->GetRMS(),hcat1[i]->GetRMSError());
		vvalue.push_back(a);
		if(hcat1[i]->GetRMS()>0){
			a = make_pair(hcat1[i]->GetSkewness(),sqrt(6)*hcat1[i]->GetMeanError()/hcat1[i]->GetRMS());
			vvalue.push_back(a);
			a = make_pair(hcat1[i]->GetKurtosis(),sqrt(24)*hcat1[i]->GetMeanError()/hcat1[i]->GetRMS());
			vvalue.push_back(a);
		}
		else{
			a = make_pair(hcat1[i]->GetSkewness(),0.);
			vvalue.push_back(a);
			a = make_pair(hcat1[i]->GetKurtosis(),0.);
			vvalue.push_back(a);
		}
		mcat1.push_back(vvalue);
	}	
	for(unsigned int i=0;i<hcat2.size();i++){
		vvalue.clear();
		//respect that order - as in constructor
		a = make_pair(hcat2[i]->GetMean(),hcat2[i]->GetMeanError());
		vvalue.push_back(a);
		a = make_pair(hcat2[i]->GetRMS(),hcat2[i]->GetRMSError());
		vvalue.push_back(a);
		if(hcat2[i]->GetRMS()>0){
			a = make_pair(hcat2[i]->GetSkewness(),sqrt(6)*hcat2[i]->GetMeanError()/hcat2[i]->GetRMS());
			vvalue.push_back(a);
			a = make_pair(hcat2[i]->GetKurtosis(),sqrt(24)*hcat2[i]->GetMeanError()/hcat2[i]->GetRMS());
			vvalue.push_back(a);
		}
		else{
			a = make_pair(hcat2[i]->GetSkewness(),0.);
			vvalue.push_back(a);
			a = make_pair(hcat2[i]->GetKurtosis(),0.);
			vvalue.push_back(a);
		}
		mcat2.push_back(vvalue);
	}	
	//fill test
	for(unsigned int i=0;i<hcat1.size();i++){
		vector<float> tvalue;
		tvalue.push_back(hcat1[i]->Chi2Test(hcat2[i],"UUNORM"));
		tests.push_back(tvalue);
		
	}
}

void TemplateComparator::PrintTable(bool doLatexMode, int nofSigmas){
	if(doLatexMode){
		 cout<<"\\begin{table}"<<endl;
	         cout<<"\\centering"<<endl;
		 cout<<"\\begin{tabular}{l|";
		 if(mcat1.size()>0) for(unsigned int j=0;j<mcat1[0].size();j++) cout<<"c";
		 if(tests.size()>0) for(unsigned int j=0;j<tests[0].size();j++) cout<<"c";
		 cout<<"c}"<<endl;
		 cout<<"\\hline"<<endl;
		 //cout<<"\\multicolumn{5}{l}{Estimation / Monte Carlo prediction : }\\\\"<<endl;
	}
	//fill first line of the table
	cout<<"Observables";
	if(doLatexMode)cout<<" & ";
	cout<<"\t";
	//loop on moments
	for(unsigned int i=0;i<MomentNames.size();i++){
		cout<<MomentNames[i]<<"\t";
		if(doLatexMode)cout<<" & ";
	}
	//loop on tests
	for(unsigned int i=0;i<TestNames.size();i++){
		cout<<TestNames[i]<<"\t";
		if(doLatexMode && i<TestNames.size())cout<<" & ";
	}
	cout<<"Compatibility";
	if(doLatexMode) cout<<" \\\\";
	cout<<endl;
	if(doLatexMode) cout<<"\\hline"<<endl;
	//loop on histos
	for(unsigned int i=0;i<hcat1.size();i++){
		//give the name of the histo
		if(doLatexMode)cout<<"$";
		cout<<ObsNames[i];
		if(doLatexMode)cout<<"$";
		if(doLatexMode)cout<<" & ";
		cout<<"\t";
		//loop on moments - categ 1
		for(unsigned int j=0;j<mcat1[i].size();j++){
			cout<<mcat1[i][j].first<<" $\\pm$ "<<mcat1[i][j].second<<"/"<<mcat2[i][j].first<<" $\\pm$ "<<mcat2[i][j].second<<"\t";
			if(doLatexMode)cout<<" & ";
		}
		//loop on tests
		for(unsigned int j=0;j<tests[i].size();j++){
			cout<<tests[i][j]<<"\t";
			if(doLatexMode && j<tests[i].size())cout<<" & ";
		}
		AreCompatibleDistributions(i,nofSigmas)? cout<<"true":cout<<"false";
		if(doLatexMode) cout<<" \\\\";
		cout<<endl;
	}
	//for #events
	cout<<"\\#events";
	if(doLatexMode)cout<<" & ";
	cout<<"\t";
	cout<<ncat1.first<<" $\\pm$ "<<ncat1.second<<" / "<<ncat2.first<<" $\\pm$ "<<ncat2.second<<"\t";
	if(doLatexMode){ 
		for(unsigned int j=0;j<mcat1[0].size()+tests[0].size()-1;j++) cout<<" & ";
		if(doLatexMode) cout<<" \\\\";
	}
	cout<<endl;
	if(doLatexMode){
		 cout<<"\\hline"<<endl;
		 cout<<"\\end{tabular}"<<endl;
		 cout<<"\\end{table}"<<endl;
	}
}

bool TemplateComparator::Compatibility(pair<float,float> v1, pair<float,float> v2, int nsigmas){
	if(v2.first>v1.first){
		if(v2.first-(nsigmas*v2.second)<v1.first+nsigmas*v1.second) return true;
		return false;
	}
	else{
		if(v1.first-(nsigmas*v1.second)<v2.first+nsigmas*v2.second) return true;
		return false;
	}
}

bool TemplateComparator::AreCompatibleDistributions(int varNum, int nofSigmas){
	if(varNum<0 || varNum>=(int)MomentNames.size()) return false;
	bool compatible = true;
	for(unsigned int j=0;j<mcat1[varNum].size();j++){
		if(!Compatibility(mcat1[varNum][j], mcat2[varNum][j], nofSigmas)){
			compatible = false;
			break;
		}
	}
	return compatible;
}

