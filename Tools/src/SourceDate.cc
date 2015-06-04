//////////////////////////////////////////////
// Setup Date string and nTuple for output  //
//////////////////////////////////////////////

#include "../interface/SourceDate.h"

SourceDate::SourceDate(){

}

SourceDate::~SourceDate(){

}

string SourceDate::ReturnDateStr(){

	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );

	int year = now->tm_year + 1900;
	int month =  now->tm_mon + 1;
	int day = now->tm_mday;
	int hour = now->tm_hour;
	int min = now->tm_min;
	int sec = now->tm_sec;

	ostringstream convert;   // stream used for the conversion
	convert << year;      // insert the textual representation of 'Number' in the characters in the stream
	year_str = convert.str();
	convert.str("");
	convert.clear();
	convert << month;      // insert the textual representation of 'Number' in the characters in the stream
	month_str = convert.str();
	convert.str("");
	convert.clear();
	convert << day;      // insert the textual representation of 'Number' in the characters in the stream
	day_str = convert.str();
	convert.str("");
	convert.clear();
	convert << hour;      // insert the textual representation of 'Number' in the characters in the stream
	hour_str = convert.str();
	convert.str("");
	convert.clear();
	convert << min;      // insert the textual representation of 'Number' in the characters in the stream
	min_str = convert.str();
	convert.str("");
	convert.clear();
	convert << day;      // insert the textual representation of 'Number' in the characters in the stream
	sec_str = convert.str();
	convert.str("");
	convert.clear();


	string date_str = day_str + "_" + month_str + "_" + year_str;

	cout <<"DATE STRING   "<<date_str << endl;

	return date_str;

}
