#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "TimeAndDate.h"


TimeAndDate::TimeAndDate()
{
}


TimeAndDate::~TimeAndDate()
{
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
void TimeAndDate::currentDateTime() {
	
	time_t now = time(0);
	struct tm  tstruct;
	tstruct = *localtime(&now);
	strftime(temp_date, sizeof(temp_date), "%Y-%m-%d.%X.X", &tstruct);

	string delimeter = ".";
	string str(temp_date);
	//string extractDataBuff;
	size_t pos;

	/*Incoming data usually comes with 5 consecutive rows. Splits and writes
	each one to file if it's on the correct format */
	pos = str.find(delimeter);
	date_x = str.substr(0, pos);
	//str.erase(0, pos + delimeter.length());

	time_x = str.substr(pos+1, str.length());

}
