#include "Xport.h"

class TurbData
{
protected:
	Xport *m_Xport;

	double xval, yval, zval, Tval;
private:
	char buffer[200];
	ofstream out_file;
	int portnr;
	string ip;
public:
	TurbData();
	~TurbData();

	void Open();
	void setPortNr(int a) { portnr = a; }
	void setIp(string p) { ip = p; }
	void RawDataToFile();

};

