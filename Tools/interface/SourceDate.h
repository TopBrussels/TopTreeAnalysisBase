// SourceDate.h

#ifndef SourceDate_h
#define SourceDate_h

#define _USE_MATH_DEFINES
#include "TStyle.h"
#include "TPaveText.h"
#include "TTree.h"
#include "TNtuple.h"
#include <ctime>

#include <cmath>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <errno.h>
#include "TRandom3.h"
#include "TRandom.h"
#include "TProfile.h"
#include <iostream>
#include <map>
#include <cstdlib>

using namespace std;


class SourceDate{
	public:
		SourceDate();
		~SourceDate();
		string ReturnDateStr();

		string year_str;
		string month_str;
		string day_str;
		string hour_str;
		string min_str;
		string sec_str;
	private:


};

#endif