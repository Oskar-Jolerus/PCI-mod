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

	rawDataFile.open("rawData.txt"); //Här måste vi hämta från nån annan class som har läst in namnet från användaren
	skippedReadings = 0;
	if (!rawDataFile.is_open()) {
		cout << "Unable to open file";
		return false;
	}
	return true;
}

bool TurbData::OpenStatFile() {

	statFile.open("turbStat.txt"); //här måste vi hämta från nån annan class som har läst in namnet från användaren
								   //skippedreadings = 0;
	if (!statFile.is_open()) {
		cout << "unable to open file";
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
	remainderOfIndata = remainderOfIndata + str;

	/*Incoming data usually comes with 5 consecutive rows. Splits and writes
	each one to file if it's on the correct format */
	while ((pos = remainderOfIndata.find(delimeter)) != string::npos) {

		extractDataBuff = remainderOfIndata.substr(0, pos);
		remainderOfIndata.erase(0, pos + delimeter.length());
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
		else {
			skippedReadings++;
		}

	}
	

	if (correctRowReadings >= ave_number) {
		DoCalculations();
		statFile << left << setw(17) << m_TimeAndDate->date_x
			<< left << setw(16) << m_TimeAndDate->time_x
			<< left << setw(18) << xmean
			<< left << setw(18) << ymean
			<< left << setw(18) << zmean
			<< left << setw(10) << Tmean 
			<< skippedReadings << endl;
		correctRowReadings = 0;
	}

	return correctRowReadings;
}

void TurbData::SetHeaderInRawFile() {

	rawDataFile << left << setw(17) << "Date(YYYY-MM-DD)"
		<< left << setw(8) << "Time(h:m:s)     "
		<< left << setw(8) << "x-values(m/s)     "
		<< left << setw(14) << "y-values(m/s)     "
		<< left << setw(14) << "z-values(m/s)     "
		<< left << setw(10) << "T-values(degC)" << endl;

}

void TurbData::SetHeaderInStatFile() {

	statFile << left << setw(17) << "Date(YYYY-MM-DD)"
		<< left << setw(8) << "Time(h:m:s)     "
		<< left << setw(8) << "x-mean(m/s)     "
		<< left << setw(14) << "y-mean(m/s)     "
		<< left << setw(14) << "z-mean(m/s)     "
		<< left << setw(10) << "T-mean(degC)" << endl;

}




void TurbData::GillStartConfig() {

	char waste[25000];
	ReadGillData(waste, 25000);

	m_Xport->Write2Sensor("IM");	// Enter interactive mode in Gill
	m_Xport->Write2Sensor("IM");

	/* Convert sample frequency to block average. Wind vel measured
	every 10 ms in Gill*/
	m_Xport->Write2Sensor("AVERAGE " + to_string(100.0 / s_f));
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
	Sleep(2000);
	m_Xport->Write2Sensor("EXIT");					// Go back to measurement mode

}


//--------------------------------------------------------------------------------
// Turbulence calculations
//--------------------------------------------------------------------------------
void TurbData::DoCalculations() {

	/*Calculate the means from the x,y,z,T buffer.*/
	xmean = accumulate(xbuff.begin(), xbuff.end(), 0.0) / ave_number;	// [m/s]
	ymean = accumulate(ybuff.begin(), ybuff.end(), 0.0) / ave_number;	// [m/s]
	zmean = accumulate(zbuff.begin(), zbuff.end(), 0.0) / ave_number;	// [m/s]
	Tmean = accumulate(Tbuff.begin(), Tbuff.end(), 0.0) / ave_number;	// [degC]

																		/* Declaring variables for recurring use. These represents the mean of the
																		squared deviation in x,y,z,T */
	float xPrime2mean, yPrime2mean, zPrime2mean, TPrime2mean;

	/* Standard deviations in x,y,z,T*/
	xsig = CalcStddev(xbuff, xmean, xPrime2mean);	// [m/s]
	ysig = CalcStddev(ybuff, ymean, yPrime2mean);	// [m/s]
	zsig = CalcStddev(zbuff, zmean, zPrime2mean);	// [m/s]
	Tsig = CalcStddev(Tbuff, Tmean, TPrime2mean);	// [K]

													/* Covariance*/
	xycov = CalcCovar(xbuff, ybuff, xmean, ymean);	// [m^2/s^2]
	xzcov = CalcCovar(xbuff, zbuff, xmean, zmean);	// [m^2/s^2]
	xTcov = CalcCovar(xbuff, Tbuff, xmean, Tmean);	// [mK/s]
	yzcov = CalcCovar(ybuff, zbuff, ymean, zmean);	// [m^2/s^2]
	yTcov = CalcCovar(ybuff, Tbuff, ymean, Tmean);	// [mK/s]
	zTcov = CalcCovar(zbuff, Tbuff, zmean, Tmean);	// [mK/s]

													/* Wind components and horizontal wind speed.*/
	u = xmean*sin(alpha) + ymean*cos(alpha);
	v = xmean*cos(alpha) - ymean*sin(alpha);
	w = zmean;
	vel = sqrt(u*u + v*v);

	if (u == 0.0 && v == 0.0) {
		// do something
		//continue;
	}

	if (xmean == 0.0 && ymean == 0.0) {
		// do something
		//continue;
	}

	dir = -atan2(u, -v);	// [rad]
	theta = atan2(ymean, xmean);	// [rad]
	float sin2theta = sin(theta)*sin(theta);
	float cos2theta = cos(theta)*cos(theta);

	float lPrime2mean = yPrime2mean*cos2theta + 2.0*xycov*sin(theta)*cos(theta) + xPrime2mean*sin2theta;
	float lPrimezPrimemean = xzcov*sin(theta) + yzcov*cos(theta);

	// instrumentets horisontalplan, fi == 0
	// variabler enligt detta system namnges med suffix _n
	// psig_n = sqrt(lPrime2mean*1.0 + 2.0*lPrimezPrimemean*0.0 + zPrime2mean*0.0);
	psig_n = sqrt(lPrime2mean);
	qsig_n = sqrt(yPrime2mean*sin2theta - 2.0*xycov*sin(theta)*cos(theta) + xPrime2mean*cos2theta);
	// rsig_n = sqrt(lPrime2mean*0.0 - 2.0*lPrimezPrimemean*0.0 + zPrime2mean*1.0);
	rsig_n = sqrt(xPrime2mean);

	tp_n = psig_n / sqrt(vel*vel + zmean*zmean);
	tq_n = qsig_n / sqrt(vel*vel + zmean*zmean);
	tr_n = rsig_n / sqrt(vel*vel + zmean*zmean);

	// float utmp = fabs(lPrimezPrimemean*(sin2fi-cos2fi) + (lPrime2mean-zPrime2mean)*sincosfi);
	// float utmp = fabs(lPrimezPrimemean*(1.0-0.0) + (lPrime2mean-zPrime2mean)*0.0);
	float utmp = fabs(lPrimezPrimemean);

	ustar_n = sqrt(utmp);
	if (ustar_n < 0.00001) {
		ustar_n = 0.001;
		Tstar_n = 999999.99;
	}
	else {
		Tstar_n = zTcov / ustar_n;
	}

	Cd_n = ustar_n*ustar_n / (vel*vel + zmean*zmean);

	MOs_n = -kappa*g*zTcov / ((Tmean + T0)*fabs(ustar_n*ustar_n*ustar_n));

	mf_n = -airDensity(Tmean)*ustar_n*ustar_n;

	CalcVelVec();
	float velzcov = CalcCovar(velvec, zbuff, vel, zmean);
	float c = soundSpeed(Tmean + T0);
	hf_n = airDensity(Tmean)*cp*(zTcov + 3.0 / 2.0*velzcov*(Tmean + T0)*vel / (c*c));

	// "tiltat system" vi bestämmer fi != 0 så att wmean blir noll.
	// variabler enligt detta system namnges med suffix _nt
	float sinfi = zmean / sqrt(vel*vel + zmean*zmean);
	float cosfi = sqrt(xmean*xmean + ymean*ymean) / sqrt(vel*vel + zmean*zmean);
	float sin2fi = sinfi*sinfi;
	float cos2fi = cosfi*cosfi;
	float sincosfi = sinfi*cosfi;
	float lPrimeTPrimemean = xTcov*sin(theta) + yTcov*cos(theta);

	float vPrimetTPrimemean = sqrt(lPrimeTPrimemean*lPrimeTPrimemean + zTcov*zTcov);

	if (zmean == 0.0 && vel == 0.0) {
		// do something
		// continue;
	}

	if (lPrimeTPrimemean == 0.0 && zTcov == 0.0) {
		// do something
		// continue;
	}

	float fi = atan2(zmean, vel);
	float delta1 = atan2(lPrimeTPrimemean, zTcov);
	float delta2 = delta1 + fi;

	zTcov_nt = vPrimetTPrimemean*cos(delta2);
	psig_nt = sqrt(lPrime2mean*cos2fi + 2.0*lPrimezPrimemean*sincosfi + zPrime2mean*sin2fi);
	qsig_nt = sqrt(yPrime2mean*sin2theta - 2.0*xycov*sin(theta)*cos(theta) + xPrime2mean*cos2theta);
	rsig_nt = sqrt(lPrime2mean*sin2fi - 2.0*lPrimezPrimemean*sincosfi + zPrime2mean*cos2fi);

	tp_nt = psig_n / sqrt(vel*vel + zmean*zmean);
	tq_nt = qsig_n / sqrt(vel*vel + zmean*zmean);
	tr_nt = rsig_n / sqrt(vel*vel + zmean*zmean);
	utmp = fabs(lPrimezPrimemean*(sin2fi - cos2fi) + (lPrime2mean - zPrime2mean)*sincosfi);
	ustar_nt = sqrt(utmp);
	if (ustar_nt < 0.00001) {
		ustar_nt = 0.001;
		Tstar_nt = 999999.99;
	}
	else {
		Tstar_nt = zTcov_nt / ustar_nt;
	}

	Cd_nt = ustar_nt*ustar_nt / (vel*vel + zmean*zmean);
	MOs_nt = -kappa*g*zTcov_nt / ((Tmean + T0)*fabs(ustar_nt*ustar_nt*ustar_nt));
	mf_nt = -airDensity(Tmean)*ustar_n*ustar_n;
	hf_nt = airDensity(Tmean)*cp*(zTcov_nt + 3.0 / 2.0*velzcov*(Tmean + T0)*vel / (c*c));

	xbuff.clear();
	ybuff.clear();
	zbuff.clear();
	Tbuff.clear();
	velvec.clear();


}

//--------------------------------------------------------------------------------
// Standard deviation calculation
//--------------------------------------------------------------------------------
float TurbData::CalcStddev(vector <float> vBuff, float mean, float& vPrime2mean) {

	typedef vector <float>::iterator iterator;
	float sig = 0.0;
	float diff;

	for (iterator it = vBuff.begin(); it != vBuff.end(); it++) {
		diff = *it - mean;
		sig += diff*diff;
	}

	vPrime2mean = sig / ave_number;

	return sqrt(vPrime2mean);

}

//--------------------------------------------------------------------------------
// Covariance calculation
//--------------------------------------------------------------------------------
float TurbData::CalcCovar(vector <float> v1, vector <float> v2, float mean1, float mean2) {

	typedef vector <float>::iterator iterator;
	float cov = 0.0;
	iterator it2 = v2.begin();

	for (iterator it1 = v1.begin(); it1 != v1.end(); it1++, it2++) {
		cov += (*it1 - mean1)*(*it2 - mean2);
	}

	return cov / ave_number;
}

void TurbData::CalcVelVec() {

	typedef vector<float>::iterator iterator;
	iterator it2 = ybuff.begin();

	for (iterator it1 = xbuff.begin(); it1 != xbuff.end(); it1++, it2++) {
		velvec.push_back(sqrt((*it1)*(*it1) + (*it2)*(*it2)));
	}

}

void TurbData::WriteTurbStat() {

	/*Write turb calculations to the stat file*/
	statFile <<
		xmean << " " <<
		ymean << " " <<
		zmean << " " <<
		Tmean << " " <<
		xsig << " " <<
		ysig << " " <<
		zsig << " " <<
		Tsig << " " <<
		xycov << " " <<
		xzcov << " " <<
		xTcov << " " <<
		yzcov << " " <<
		yTcov << " " <<
		zTcov << " " <<
		u << " " <<
		v << " " <<
		w << " " <<
		vel << " " <<
		dir << " " <<
		psig_n << " " <<
		qsig_n << " " <<
		rsig_n << " " <<
		tp_n << " " <<
		tq_n << " " <<
		tr_n << " " <<
		ustar_n << " " <<
		Tstar_n << " " <<
		Cd_n << " " <<
		MOs_n << " " <<
		mf_n << " " <<
		hf_n << " " <<
		zTcov_nt << " " <<
		psig_nt << " " <<
		qsig_nt << " " <<
		rsig_nt << " " <<
		tp_nt << " " <<
		tq_nt << " " <<
		tr_nt << " " <<
		ustar_nt << " " <<
		Tstar_nt << " " <<
		Cd_nt << " " <<
		MOs_nt << " " <<
		mf_nt << " " <<
		hf_nt << " " <<
		endl;

}

void TurbData::TestFuncionToRun() {

	int count = 0;
	correctRowReadings = 0;
	GillStartConfig();

	//m_Xport->Write2Sensor("IM");

	//m_Xport->Write2Sensor("IM");

	OpenRawDataFile();
	OpenStatFile();
	SetHeaderInRawFile();
	SetHeaderInStatFile();
	do {

		ReadGillData(indata, MAXBUFFER);
		CheckFormatAndWriteRawDataToFile(indata);
		Sleep(100);
		cout << string(indata);
		//cout << "-------------------------------------" << endl;
		count++;

	} while (count < 15000);

	rawDataFile.close();

}
