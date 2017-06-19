//This is the Client.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <fstream>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")



using namespace std;

class Xport{
private:
	int PortNr;
	string IpAddress;
	bool SocketOK;
	bool OpenSocket();
	void CloseSocket();
	int SensorStringSize = 200;
	bool configMode;
public:
	Xport(int portnr, string ip);
	Xport();
	~Xport();

	char* StringFromSensor();
	void Write2Sensor(string s);

	int getPortNr() { return PortNr; }
	string getIpAddress(){ return IpAddress;}

	
};

