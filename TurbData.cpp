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
	//int res = m_Xport->StringFromSensor(buffer);
	int count = 0;
	int cnum;
	
	do {
		count++;

		if (out_file.is_open()) {
			int res = m_Xport->StringFromSensor(buffer);
			
			if ((buffer[7] == '+' || buffer[7] == '-') &&
				(cnum = sscanf_s(buffer + 7, "%Lf,%Lf,%Lf,%Lf", &yval, &xval, &zval, &Tval)) == 4) {
				
				out_file << "x = " << xval << " y = " << yval << " z = " << zval << " T = " << Tval << endl;
				//out_file << x.at(count - 1);
				cout << "hej" << endl;
			}

		}
		else {

			cout << "Unable to open file";
		}

	} while (count < 1500);
	
	out_file.close();

}