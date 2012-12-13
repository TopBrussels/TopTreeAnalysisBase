#ifndef MCWeighter_h
#define MCWeighter_h

//system includes
#include <memory>
#include <vector>
#include <string>

#include <TH1F.h>
#include <TF1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TKey.h>
#include <TROOT.h>
#include <TClass.h>

using namespace std;

class MCWeighter
{
	public:
		MCWeighter();
		~MCWeighter();

		void Clear();

		float GetWeight(unsigned int index, float value);
		float GetWeight(string variable, float value);
		float GetWeight(vector<float> values);
		
		void LoadVariable(string name, bool histoWeights, TH1F* hMC, TH1F* hData, TF1* fMC, TF1* fData);

		vector<string> GetListofVariables();
		string GetVarName(unsigned int index);
		unsigned int GetIndex(string variable);
		
		TH1F* GetHistoData(unsigned int index);
		TH1F* GetHistoData(string variable);
		TH1F* GetHistoMC(unsigned int index);
		TH1F* GetHistoMC(string variable);
		TH1F* GetHistoWeight(unsigned int index);
		TH1F* GetHistoWeight(string variable);

		TF1* GetFuncData(unsigned int index);
		TF1* GetFuncData(string variable);
		TF1* GetFuncMC(unsigned int index);
		TF1* GetFuncMC(string variable);
		
		void LoadFile(TFile* fin, int verbosity = 0);/** Load the histos and functions from a TFile*/
		void ExportFile(TFile* fout, vector<string> histovarname, vector<TH1F*> MCHistos, vector<TH1F*> DataHistos, vector<string> funcvarname, vector<TF1*> MCFuncs, vector<TF1*> DataFuncs, int verbosity = 0);/** Will write the histos and functions in a given format inside a TFile*/

		void Draw();
		void Write(TFile* fout);
		

	private:
		vector<string> variables_;
		vector<TH1F*> hMC_;
		vector<TH1F*> hData_;
		vector<TH1F*> hWeight_;
		vector<TF1*> fMC_;
		vector<TF1*> fData_;
		vector<TCanvas*> DataMCCanvas_;

};

#endif
