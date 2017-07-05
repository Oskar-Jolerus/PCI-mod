#include "TurbData.h"
#include <iomanip>

#include <math.h>
#include <numeric>

#pragma package(smart_init)

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
	delete m_Xport;
	delete m_TimeAndDate;
}

void TurbData::Open() {

	m_Xport = new Xport(portnr, ip);
	m_TimeAndDate = new TimeAndDate();

}


bool TurbData::OpenRawDataFile() {

	rawDataFile.open("turbData.txt"); //Här måste vi hämta från nån annan class som har läst in namnet från användaren
	skippedReadings = 0;
	if (!rawDataFile.is_open()) {
		cout << "Unable to open file";
		return false;
	}
	return true;
}

int TurbData::CheckFormatAndWriteRawDataToFile(char* inputData) {

	string delimeter = "\r\n";
	string str(inputData);
	string extractDataBuff;
	size_t pos;
	int cnum;
	int correctRowReadings = 0;

	/*Incoming data usually comes with 5 consecutive rows. Splits and writes
	each one to file if it's on the correct format */
	while ((pos = str.find(delimeter)) != string::npos) {

		extractDataBuff = str.substr(0, pos);
		str.erase(0, pos + delimeter.length());
		//cout << tempstr << endl; // REMOVE LATER
		/*Checks that the row has the correct form*/
		if ((int)pos >= 7 && (extractDataBuff[7] == '+' || extractDataBuff[7] == '-') &&
			(cnum = sscanf_s(extractDataBuff.c_str() + 7, "%f,%f,%f,%f", &yval, &xval, &zval, &Tval)) == 4) {

			/*Insert values to be used for calculating mean values and other things*/
			xbuff.push_back(xval);
			ybuff.push_back(yval);
			zbuff.push_back(zval);
			Tbuff.push_back(Tval);

			m_TimeAndDate->currentDateTime();


			/*Write data to the raw data file*/
			rawDataFile << left << setw(17) << m_TimeAndDate->date_x
				<< left << setw(16) << m_TimeAndDate->time_x
				<< left << setw(18) << xval
				<< left << setw(18) << yval
				<< left << setw(18) << zval
				<< left << setw(10) << Tval << endl;

			correctRowReadings++;

		}
		else
			skippedReadings++;
	}

	return correctRowReadings;
}

void TurbData::SetHeaderInFile() {

	rawDataFile << left << setw(17) << "Date(YYYY-MM-DD)"
		<< left << setw(8) << "Time(h:m:s)     "
		<< left << setw(8) << "x-values(m/s)     "
		<< left << setw(14) << "y-values(m/s)     "
		<< left << setw(14) << "z-values(m/s)     "
		<< left << setw(10) << "T-values(degC)" << endl;

}

void TurbData::GillStartConfig() {

	m_Xport->Write2Sensor("IM");	// Enter interactive mode in Gill

	m_Xport->Write2Sensor("IM");

	/* Convert sample frequency to block average. Wind vel measured
	every 10 ms in Gill*/
	m_Xport->Write2Sensor("AVERAGE " + to_string(100 / s_f));
	m_Xport->Write2Sensor("STRFMT ASCII PAD");		// Sets the result message string format
	m_Xport->Write2Sensor("ASCTERM CRLF");			// Sets ASCII output string terminator
	m_Xport->Write2Sensor("MSGMODE CONT");			// Result messages are reported continuosly
	m_Xport->Write2Sensor("ABSTEMP OFF");			// Disable absolute temperatur reporting from the PRT sensor
	m_Xport->Write2Sensor("SOSREP SONICTEMP C");	// Sets the speed of sound reporting format
	m_Xport->Write2Sensor("ANAIP 1,2,3,4,5,6 D");	// Turn off the analogue inputs
	m_Xport->Write2Sensor("WINDREP UVW UNCAL");		// Report wind velocity in UVW format uncalibrated
	m_Xport->Write2Sensor("ALIGNUVW SPAR");			// Align the U axis with the North spar
	m_Xport->Write2Sensor("VER");					// report software version
	m_Xport->Write2Sensor("CONFIG");				// report configuration
	Sleep(5000);
	//m_Xport->Write2Sensor("EXIT");					// Go back to measurement mode

}

void TurbData::DoCalculations() {

	/*Calculate the means from the x,y,z,t buffer*/
	xmean = accumulate(xbuff.begin(), xbuff.end(), 0.0) / ave_number;
	ymean = accumulate(ybuff.begin(), ybuff.end(), 0.0) / ave_number;
	zmean = accumulate(zbuff.begin(), zbuff.end(), 0.0) / ave_number;



}

//float TurbData::CalcStandardDeviation(vector <float> valueBuff, float meanValue, float& valuePrimeSquared) {
//
//
//	return 0.0
//
//
//}
/*void TurbData::GillStartConfig() {

m_Xport->Write2Sensor("IM");
m_Xport->Write2Sensor("AVERAGE");

}*/

void TurbData::TestFuncionToRun() {

	int count = 0;

	m_Xport->Write2Sensor("IM");
	
	m_Xport->Write2Sensor("IM");

	OpenRawDataFile();
	SetHeaderInFile();
	do {
		ReadGillData(indata);
		CheckFormatAndWriteRawDataToFile(indata);
		cout << string(indata);
		count++;
	} while (count < 1500);

	rawDataFile.close();

}

//void TurbData::RawDataToFile() {
//
//	out_file.open("turbData.txt");
//	int count = 0;
//	int cnum;
//	string delimeter = "\r\n";
//	string tempstr;
//	size_t pos;
//
//	if (out_file.is_open()) {
//		do {
//			count++; 
//				int res = m_Xport->StringFromSensor(buffer);
//				string str(buffer);
//				cout << str;
//				/*Incoming string is about 5 rows long, split and write to file if 
//				  format is on the correct format*/
//				while ((pos = str.find(delimeter)) != string::npos) {
//					tempstr = str.substr(0, pos);
//					str.erase(0, pos + delimeter.length());
//					cout << tempstr << endl; // REMOVE LATER
//					if ((tempstr[7] == '+' || tempstr[7] == '-') &&
//						(cnum = sscanf_s(tempstr.c_str() + 7, "%f,%f,%f,%f", &yval, &xval, &zval, &Tval)) == 4){
//
//						out_file << left << setw(4) << "x = "  << right << setw(6) << xval << "  "
//								 << left << setw(5) << " y = " << right << setw(6) << yval << "  "
//								 << left << setw(5) << " z = " << right << setw(6) << zval << "  "
//								 << left << setw(5) << " T = " << right << setw(6) << Tval << endl;
//						out_file << " x = " << xval << "        " << " y = " << yval << "        " << " z = " << zval << "        " << " T = " << Tval << endl;
//					
//					}
//
//				}
//
//
//			} while (count < 1500);
//		}
//	else {
//
//		cout << "Unable to open file";
//	}
//	out_file.close();
//
//}