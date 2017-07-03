#include "Xport.h"
/*----------------------------------------------------------------------------*/
class TurbData
{
protected:
	Xport *m_Xport;

	float xval, yval, zval, Tval;
private:
	char buffer[1024];
	ofstream out_file;
	int portnr;
	string ip;


	/*Vector buffers to calculate means for*/
	vector <float> xbuff,ybuff, zbuff, Tbuff;


	static const float cp;    // specific heat capacity air [J*kg-1*K-1]
	static const float kappa; // von-Karman constant
	static const float g;     // gravity [m/s2]
	static const float T0;    // 0.0 degrees Celsius in Kelvin
	static const float rho0;  // air density at 0.0 degrees Celsius [kg/m3]

	/*Parameters to calculate*/
	float xmean, ymean, zmean, Tmean;				// Mean values
	float xsig, ysig, zsig, Tsig;					// Standard deviation
	float xycov, xzcov, xTcov, yzcov, yTcov, zTcov; // Covariances


public:

	TurbData();
	~TurbData();

	void Open();
	void setPortNr(int a) { portnr = a; }
	void setIp(string p) { ip = p; }
	void RawDataToFile();

};

