#include "TurbData.h"
#include <iomanip>

const float TurbData::cp = 1004.0;  // specific heat capacity air [J*kg-1*K-1]
const float TurbData::kappa = 0.4;  // von-Karman constant
const float TurbData::g = 9.81;     // gravity [m/s2]
const float TurbData::T0 = 273.16;  // 0.0 degrees Celsius in Kelvin
const float TurbData::rho0 = 1.292; // air density at 0.0 degrees Celsius [kg/m3]



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
	string delimeter = "\r\n";
	string tempstr;
	size_t pos;

	if (out_file.is_open()) {
		do {
			count++; 
				int res = m_Xport->StringFromSensor(buffer);
				string str(buffer);

				/*Incoming string is about 5 rows long, split and write to file if 
				  format is on the correct format*/
				while ((pos = str.find(delimeter)) != string::npos) {
					tempstr = str.substr(0, pos);
					str.erase(0, pos + delimeter.length());
					cout << tempstr << endl; // REMOVE LATER
					if ((tempstr[7] == '+' || tempstr[7] == '-') &&
						(cnum = sscanf_s(tempstr.c_str() + 7, "%f,%f,%f,%f", &yval, &xval, &zval, &Tval)) == 4){

						out_file << left << setw(4) << "x = "  << right << setw(6) << xval << "  "
								 << left << setw(5) << " y = " << right << setw(6) << yval << "  "
								 << left << setw(5) << " z = " << right << setw(6) << zval << "  "
								 << left << setw(5) << " T = " << right << setw(6) << Tval << endl;
						//out_file << " x = " << xval << "        " << " y = " << yval << "        " << " z = " << zval << "        " << " T = " << Tval << endl;
					
						

					}

				}


			} while (count < 1500);
		}
	else {

		cout << "Unable to open file";
	}
	out_file.close();

}