/*******************************************************************
* TurbData
* Skapare: Carl-Johan Möller, Alexander Berggren och Oskar Jolérus
* Datum:	2017-07-10
*
* Klassen tar hand om rådata och beräknignar av turbulensparametrar
* samt skriver dessa till fil.
*******************************************************************/

#include "Xport.h"
#include "TimeAndDate.h"

using std::ofstream;

class TurbData
{
protected:
	Xport *m_Xport;
	TimeAndDate *m_TimeAndDate;

	float xval, yval, zval, Tval, tval;
private:
	char indata[1024];
	ofstream statFile;
	ofstream rawDataFile;
	int portnr;
	string ip;
	ios_base::openmode openstate;

	string remainderOfIndata;
	int skippedReadings;

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
	float alpha; // Angle in radians between instrument coordinates to earth coordinates.
	float az;	// azimuth, angle towards north, degrees, clockwise.

	float u;	// west-east wind component. [m/s]
	float v;	// south-north wind component. [m/s]
	float w;	// vertical wind componentn. [m/s]
	float vel;	// horizontal wind speed. [m/s]
	float dir;	// horizontal wind direction. [degree]
	float theta;

	float xmean, ymean, zmean, Tmean;				// Mean values
	float xsig, ysig, zsig, Tsig;					// Standard deviation
	float xycov, xzcov, xTcov, yzcov, yTcov, zTcov; // Covariances
	vector <float> velvec;    // Horizontal wind velocity vector

	float psig_n; // stdd of p parallel to the mean 3-d wind direction [m/s]
	float qsig_n; // stdd of q orthogonal horizontal to the mean 3-d wind direction [m/s]
	float rsig_n; // stdd of r orthogonal vertical to p and q [m/s]
	float tp_n;	// Longitudinal turbulence intencity 
	float tq_n;	// Transversal turbulence intencity
	float tr_n;	// Vertical turbulence intencity
	float ustar_n;	// Friction velocity [m/s]
	float Tstar_n;	// Characteristic temperature [K]
	float Cd_n;	// Drag coefficient
	float MOs_n;	// Monin-Obukhov-stabillity parameter [m^-1]
	float mf_n;	// Momentum flux [kg/m^3]
	float hf_n;	// heat flux                         [W/m2] 

	float zTcov_nt; //
	float psig_nt, qsig_nt, rsig_nt;
	float tp_nt, tq_nt, tr_nt, ustar_nt, Tstar_nt;
	float Cd_nt;              // Drag coefficient
	float MOs_nt;             // Monin-Obukhov stability parameter [1/m]
	float mf_nt;              // Momentum flux                     [kg/ms2]
	float hf_nt;              // heat flux                         [W/m2]

	void DoCalculations();
	float CalcStddev(vector <float> valueBuff, float meanValue, float& valuePrimeSquared);
	float CalcCovar(vector <float> v1, vector <float> v2, float mean1, float mean2);
	void CalcVelVec();
	inline float airDensity(float T) { return rho0*T0 / T; };	// T och T0 [K]
																// c = sqrt( gamma*R*T), där gamma = Cp/Cv ~ 1004/717 ~ 1,4 och R=287
	inline float soundSpeed(float T) { return 20.05*sqrt(T); };  // [m/s]  T [K]

	int correctRowReadings;

	char *in_str1;
	int in_chars;
	string in_str2;

	int files_count;

public:

	TurbData();
	~TurbData();

	void Open();
	void setPortNr(int a) { portnr = a; }
	void setIp(string p) { ip = p; }

	/* Function: Checks the format on the incoming data and writes it to file.
	 *
	 * Input:	inputData - char pointer to the character array where Gill data 
	 *			has been stored
	 * Output:	Integer representing the number of rows that has been succesfully 
	 *			written to file.
	 *
	 * Note:		Incoming data from Gill seem to come in blocks of 5 rows.*/
	int CheckFormatAndWriteRawDataToFile(char* inputData);

	/* Function: Checks the format on the incoming data and writes it to file.
	*
	* Input:	inputData - pointer to the character array where Gill data has been stored
	* Output:	The number of rows that has been succesfully written to file
	*
	* Note:		Incoming data from Gill seem to come in blocks of 5 rows.*/
	//bool OpenRawDataFile();
	//bool OpenStatFile();
	
	bool OpenFile();

	void TestFuncionToRun();

	void ReadGillData(char* indata, int indataSize) { m_Xport->StringFromSensor(indata, indataSize); }

	void SetHeaderInRawFile();
	void SetHeaderInStatFile();

	void setBaud(int baudRate) { baud = baudRate; };
	void setAveTime(int aveTime) { ave_time = aveTime; };
	void setSampFreq(int sampFreq) { s_f = sampFreq; };
	void setAveNr() { ave_number = s_f*ave_time; };
	void setAlpha(float alphaAngle) { alpha = alphaAngle; };
	void setAzimuth(float azAngle) { az = azAngle; };

	int getBaud() { return baud; };
	int getAveTime() { return ave_time; };
	int getSampFreq() { return s_f; };
	int getAveNr() { return ave_number; };
	float getAlpha() { return alpha; };
	float getAzimuth() { return az; };

	void GillStartConfig();

	void WriteTurbStat();

};

