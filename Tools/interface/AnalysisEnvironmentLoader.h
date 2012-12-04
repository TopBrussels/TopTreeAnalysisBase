#ifndef AnalysisEnvironmentLoader_h
#define AnalysisEnvironmentLoader_h

#include <vector>
#include <string>
#include <iostream>
#include "TObject.h"
#include "TopTreeAnalysis/tinyxml/tinyxml.h"
#include "TopTreeAnalysis/Content/interface/AnalysisEnvironment.h"

using namespace std;

class AnalysisEnvironmentLoader
{
public:

  AnalysisEnvironmentLoader (AnalysisEnvironment& ana, const char* filename);
  ~AnalysisEnvironmentLoader ();

  void StringSeparator(string s, string sep, std::vector<int>& out);
  void StringSeparator(string s, string sep, std::vector<float>& out);
  void StringSeparator(string s, string sep, std::vector<string>& out);

  void BtagWPLoader(int nbofbtag, string s, string sep, float* out);
  
};

#endif
