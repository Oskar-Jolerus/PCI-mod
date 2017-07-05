#include "Xport.h"
#include "TimeAndDate.h"
/*----------------------------------------------------------------------------*/
class TurbData
{
protected:
	Xport *m_Xport;
	TimeAndDate *m_TimeAndDate;

	float xval, yval, zval, Tval, tval;
private:
	char indata[1024];
	ofstream out_file;
	ofstream rawDataFile;
	int portnr;
	string ip;

	int skippedReadings;

	string extractDataBuff;

	/*Vector buffers to calculate means for*/
	vector <float> xbuff, ybuff, zbuff, Tbuff;


	static const float cp;    // specific heat capacity air [J*kg-1*K-1]
	static const float kappa; // von-Karman constant
	static const float g;     // gravity [m/s2]
	static const float T0;    // 0.0 degrees Celsius in Kelvin
	static const float rho0;  // air density at 0.0 degrees Celsius [kg/m3]

	int baud;	// Baud rate
	int ave_number;	// Averaging number
	int ave_time;	// Averaging time [s]
	int s_f;	// Sampling frequency [s^-1]

	float xmean, ymean, zmean, Tmean;				// Mean values
	float xsig, ysig, zsig, Tsig;					// Standard deviation
	float xycov, xzcov, xTcov, yzcov, yTcov, zTcov; // Covariances


	/*Function: Calculates the standard deviation.
				The value "valuePrimeSquared (i.e x',y',*/
	//float CalcStandardDeviation(vector <float> valueBuff, float meanValue, float& valuePrimeSquared);
	char *in_str1;
	int in_chars;
	string in_str2;




public:

	TurbData();
	~TurbData();

	void Open();
	void setPortNr(int a) { portnr = a; }
	void setIp(string p) { ip = p; }
	//void RawDataToFile();

	/*Function: Checks the format on the incoming data and writes it to file.
	Input:    rawData - pointer the the file stream that data should be stored.
	inputData - pointer to the character array where Gill data has been stored
	Output:   The number of rows that has been succesfully written to file
	Note:		Incoming data from Gill seem to come in blocks of 5 rows.*/
	int CheckFormatAndWriteRawDataToFile(char* inputData);


	void DoCalculations();

	bool OpenRawDataFile();

	void TestFuncionToRun();

	void ReadGillData(char* indata) { m_Xport->StringFromSensor(indata); }

	void SetHeaderInFile();

	void setBaud(int baudRate) { baud = baudRate; };
	//void setAveNr(int aveNr) { ave_number = aveNr; };
	void setAveTime(int aveTime) { ave_time = aveTime; };
	void setSampFreq(int sampFreq) { s_f = sampFreq; };

	int getBaud() { return baud; };
	//int getAveNr() { return ave_number; };
	int getAveTime() { return ave_time; };
	int getSampFreq() { return s_f; };

	int aveNrFunc() { return s_f*ave_time; };

	void GillStartConfig();

};

