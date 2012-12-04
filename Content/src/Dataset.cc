#include "../interface/Dataset.h"

ClassImp(Dataset);
	
Dataset::Dataset(const Dataset &d){
	Name_ = d.Name_;
	Title_ = d.Title_;
	DotIt_ = d.DotIt_;
	Color_ = d.Color_;
	LineStyle_ = d.LineStyle_;
	LineWidth_ = d.LineWidth_;
	NormFactor_ = d.NormFactor_;
	Xsection_ = d.Xsection_;
	XsectionError_ = d.XsectionError_;
	Mass_ = d.Mass_;
	EqLumi_ = d.EqLumi_;
	PreSelEfficiency_ = d.PreSelEfficiency_;
	NofEvtsToRunOver_ = d.NofEvtsToRunOver_;
	Filenames_ = d.Filenames_;
	JSONInfo_ = d.JSONInfo_;
	eventTree_ = new TChain("eventTree");
	runTree_   = new TChain("runTree");
	for(unsigned int i=0;i<Filenames_.size();i++) eventTree_->Add(Filenames_[i].c_str());
	for(unsigned int i=0;i<Filenames_.size();i++)   runTree_->Add(Filenames_[i].c_str());
}

void
Dataset::ReadJSONFile(string JSONFile) {
	string inputJSON;

	ifstream myfile (JSONFile.c_str());
	if (myfile.is_open())
	{
		getline (myfile,inputJSON); // Only the first line is needed
		myfile.close();
	}
	
	vector<string> splittedInputJSON;
	size_t begin = 2, end = 2;

	while(end < inputJSON.size())
	{
		end = inputJSON.find("]], \"",begin);
		string splitted = inputJSON.substr(begin, end - begin + 1);
		begin = end + 5;
		
		size_t tempEnd = splitted.find("\": [[", 0);
		string runNr = splitted.substr(0, tempEnd);
		stringstream ss(runNr);
		int runNumber = 0;
		ss >> runNumber;
		
		string remain = splitted.substr(tempEnd + 4, splitted.size() - ( tempEnd + 3 ) );
		size_t tempEnd2 = remain.find("]", 0);
		size_t tempBegin2 = 0;

		while(tempEnd2 < remain.size())
		{
			string lumiInfo = remain.substr(tempBegin2 + 1, tempEnd2 - tempBegin2 - 1);
			tempBegin2 = tempEnd2 + 3;
			tempEnd2 = remain.find("]", tempBegin2);
			
			// parse lumiInfo string
			size_t tempBegin3 = lumiInfo.find(", ",0);
			string minLS = lumiInfo.substr(0,tempBegin3);
			string maxLS = lumiInfo.substr(tempBegin3 + 2, lumiInfo.size());
			int minLumiSection = 0;
			int maxLumiSection = 0;
			stringstream ssMin(minLS);		
			stringstream ssMax(maxLS);
			ssMin >> minLumiSection;
			ssMax >> maxLumiSection;
			
			vector<int> tempInfo;
			tempInfo.push_back(runNumber);
			tempInfo.push_back(minLumiSection);
			tempInfo.push_back(maxLumiSection);
			JSONInfo_.push_back(tempInfo);
		}
	}
}
