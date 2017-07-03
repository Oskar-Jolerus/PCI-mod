#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

class TimeAndDate
{

public:

	TimeAndDate();
	~TimeAndDate();
	
	char temp_date[80];
	string date_x;
	string time_x;

	void currentDateTime();

};

