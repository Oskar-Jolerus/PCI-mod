/*******************************************************************************
* Xport.cpp
* Creaters: Alexander Berggren, Oskar Jolérus & Carl-Johan Möller
* Date:	2017-07-10
*
* Class description: Handles the socket programming. Opens and connects the the 
					 given IP address. Also contain functions to read and write
					 to the sensor.
* Note: The main library used to handles sockets is Winsock2.h
*******************************************************************************/

#include "Xport.h"
#include "Winerror.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

/*Global variables to be used to connecting to a socket and
 * specific port and IP address */
SOCKET connection; 
sockaddr_in addr; 
WSADATA wsaData;

/*Constructor: Main constructor that will set port and IP address in the
*			   private variables. Will also run function to open socket.*/
Xport::Xport(int port, string Ip, string errorFileName) :
	PortNr(port), IpAddress(Ip), ConfigMode(false), ErrorFile(errorFileName) {

	SocketOK = OpenSocket();

}

/*Construtor: Constructor used if one has already set the port and IP
*			  address. Will run the open socket function.
* Note: This constructor will usually not be used. */
Xport::Xport() : SocketOK(false), ConfigMode(false) {

}

/*Destructor: Closes the socket using the CloseSocket function.*/
Xport::~Xport() {
	CloseSocket();
	errorRef.close();

}

/*Function: Opens a socket and connects to the port and IP address.
*
* Output: Boolean that represents successful or unsuccessful opening.
*
* Note: Make sure that the private variables for port and IP address
*		already has been assigned. Either by using the constructor
*		or manually setting them with the "set" functions. */
bool Xport::OpenSocket() {

	errorRef.open(ErrorFile);

	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		errorRef << "-- ERROR -- " << endl << "WSAStartup failed. Error code " << iResult << "." << endl <<
			"For more information on the error code, see https://msdn.microsoft.com/en-us/library/windows/desktop/ms742213(v=vs.85).aspx " << endl;
		exit(1);
		//return false;
	}

	//Set a connection socket
	connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connection == SOCKET_ERROR) {
		errorRef << "-- ERROR -- " << endl << "Could not create the socket. Error code " << WSAGetLastError() << endl <<
			"For more information on the error code, see https://msdn.microsoft.com/en-us/library/windows/desktop/ms740668(v=vs.85).aspx#WSASYSNOTREADY " << endl;
		exit(1);
		//return false;
	}

	//Address that we will bind our listening socket to
	addr.sin_family = AF_INET; //IPv4 Socket
	inet_pton(AF_INET, getIpAddress().c_str(), &addr.sin_addr);
	addr.sin_port = htons(getPortNr());

	int connResult = connect(connection, (sockaddr*)&addr, sizeof(addr));

	if (connResult == SOCKET_ERROR) {
		errorRef << "-- ERROR -- " << endl << "Could not connect to server. Error code " << WSAGetLastError() << endl <<
			"For more information on the error code, see https://msdn.microsoft.com/en-us/library/windows/desktop/ms740668(v=vs.85).aspx#WSASYSNOTREADY " << endl;
		closesocket(connection);
		WSACleanup();
		exit(1);
		//return false;
	}
	else {
		cout << "We are connected" << endl;
	}

	return true;
}

/*Function: Closes the socket connection, the error log file and also 
			cleans/removes winsock*/
void Xport::CloseSocket() {
	closesocket(connection);
	WSACleanup();
}

/*Function: Reads the incoming socket stream that comes from sensor.
*
* Input: char* s - Character pointer or arrary that will contain the
*				   incoming data.
*		 int size - The size(bytes) of data that should be sampled
*					each time the function is called. (Use MAXBUFFER)*/
void Xport::StringFromSensor(char* s, int size) {
	memset(s, 0, size);
	int res = 0;
	char buf[MAXBUFFER];
	
	if (!ConfigMode) {

		res = recv(connection, buf, MAXBUFFER, 0);
		memcpy(s, buf, res);
	
	}
}

/*Function: Writes a string to the sensor
*
* Input: string s - The string that will be sent to the sensor.
*
* Note: Commands available can be found in the manual.*/
void Xport::Write2Sensor(string s) {

		if (s == "IM")
			ConfigMode = true;
		else if (s == "EXIT")
			ConfigMode = false;

		string sendString = s + "\r\n";
		send(connection, sendString.c_str(), sendString.size() , 0);
		Sleep(50);
}