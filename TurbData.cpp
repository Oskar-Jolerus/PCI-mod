#include "TurbData.h"

TurbData::TurbData()
{
}


TurbData::~TurbData()
{
}

void TurbData::Open() {

	m_Xport = new Xport(portnr, ip);

}

void TurbData::RawDataToFile() {

	out_file.open("turbData.txt");
	int count = 0;
	int cnum;

	if (out_file.is_open()) {

		do {

			count++;
			int res = m_Xport->StringFromSensor(buffer);

			if ((buffer[7] == '+' || buffer[7] == '-') &&
				(cnum = sscanf_s(buffer + 7, "%lf,%lf,%lf,%lf", &yval, &xval, &zval, &Tval)) == 4) {

				out_file << "x = " << xval << " y = " << yval << " z = " << zval << " T = " << Tval << endl;
				cout << "hej" << endl;
			}

		} while (count < 10);
	
	}
	
	else {

		cout << "Unable to open file";
	}

	out_file.close();

}