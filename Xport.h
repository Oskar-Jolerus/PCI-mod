/*******************************************************************************
* Xport.cpp
* Creaters: Alexander Berggren, Oskar Jolérus & Carl-Johan Möller
* Date : 2017-07-10
*
* Class description: Handles the socket programming.Opens and connects the the
*					 given IP address.Also contain functions to read and write
*					 to the sensor.
* Note : The main library used to handles sockets is Winsock2.h
*******************************************************************************/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAXBUFFER 1024

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

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class Xport {
private:
	int PortNr;
	string IpAddress;
	bool SocketOK;
	bool ConfigMode;
	
	/*Ofstream and string to handle the error log file.*/
	string ErrorFile;
	/*Function: Closes the socket connection, the error log file and also
				cleans/removes winsock*/
	void CloseSocket();

public:

	/*Constructor: Main constructor that will set port and IP address in the 
	*			   private variables. Will also run function to open socket.*/
	Xport(int port, string Ip, string errFileName);

	/*Construtor: Constructor used if one has already set the port and IP
	*			  address. Will run the open socket function.
	* Note: This constructor will usually not be used. */
	Xport();

	/*Destructor: Closes the socket using the CloseSocket function.*/
	~Xport();

	/*Function: Opens a socket and connects to the port and IP address.
	*
	* Output: Boolean that represents successful or unsuccessful opening.
	*
	* Note: Make sure that the private variables for port and IP address
	*		already has been assigned. Either by using the constructor
	*		or manually setting them with the "set" functions. */
	bool OpenSocket();

	/*Function: Reads the incoming socket stream that comes from sensor.
	* 
	* Input: char* s - Character pointer or arrary that will contain the
	*				   incoming data.
	*		 int size - The size(bytes) of data that should be sampled
	*					each time the function is called. (Use MAXBUFFER)*/
	void StringFromSensor(char* s, int size);

	/*Function: Writes a string to the sensor
	*
	* Input: string s - The string that will be sent to the sensor.
	*
	* Note: Commands available can be found in the manual. */
	void Write2Sensor(string s);

	/*Functions that sets private variables*/
	void setPortNr(int nr) { PortNr = nr; }
	void setIpAddress(string s) { IpAddress = s; }

	/*Functions to set and get values in private*/
	int getPortNr() { return PortNr; }
	string getIpAddress() { return IpAddress; }
	bool getConfigMode() { return ConfigMode; }

	int skippedReadings;
	ofstream errorRef;
};
