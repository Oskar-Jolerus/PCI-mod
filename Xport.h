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
#include <vector>
#include <iterator>
#include <ctime>
/*----------------------------------------------------------------------------*/
#pragma comment(lib, "Ws2_32.lib")

#define MAXBUFFER 1024

using namespace std;

class Xport{
private:
	int PortNr;
	string IpAddress;
	bool SocketOK;
	void CloseSocket();
	int SensorStringSize = 200;
	bool ConfigMode;
public:
	Xport();
	/*Constructor that will set the port number and the ip address 
	  and also open the socket*/
	Xport(int port, string Ip);
	~Xport();

	/*Opens the socket, be aware to set the variables PortNr & IpAddress
	before using this function*/
	bool OpenSocket();

	//Functions to communicate to sensor
	void StringFromSensor(char* s, int size);
	void Write2Sensor(string s);

	/*Functions to set and get values in private*/
	int getPortNr() { return PortNr; }
	string getIpAddress(){ return IpAddress;}
	bool getConfigMode() {	return ConfigMode;}
	void setPortNr(int nr) { PortNr = nr; }
	void setIpAddress(string s) { IpAddress = s; }


	
};
