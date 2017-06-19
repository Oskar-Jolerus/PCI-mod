#include "Xport.h"


using namespace std;

int SensorStringSize = 200;
char carriageReturn = '\r';
char lineFeed = '\n';

SOCKET connection; //Socket for connection, global use
sockaddr_in addr; // Hint struct to pass portNr and ip, etc. Global use
WSADATA wsaData;

Xport::Xport(): SocketOK(false) {
	SocketOK = OpenSocket();
}

Xport::Xport(int portnr, string ip) :
	PortNr(portnr), IpAddress(ip), SocketOK(false) {

	SocketOK = OpenSocket();
}

Xport::~Xport() {
	CloseSocket();
}

bool Xport::OpenSocket(){

	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}



	//Set a connection socket
	connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connection == SOCKET_ERROR) {
		cerr << "Can't create the socket, Error" << WSAGetLastError << endl;
		return false;
	}

	//Address that we will bind our listening socket to
	addr.sin_family = AF_INET; //IPv4 Socket
	inet_pton(AF_INET, getIpAddress().c_str(), &addr.sin_addr);
	addr.sin_port = htons(getPortNr());

	int connResult = connect(connection, (sockaddr*)&addr, sizeof(addr));

	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, Err #  " << WSAGetLastError() << endl;
		closesocket(connection);
		WSACleanup();
		return false;
	}
	else {
		cout << "We are connected" << endl;
	}

	return true;
}

void Xport::CloseSocket() {
	closesocket(connection);
	WSACleanup();
}

char* Xport::StringFromSensor() {
	char rowBuffer[200];
	int res;
	if (!configMode) {
		res = recv(connection, rowBuffer, sizeof(rowBuffer), 0);
		cout << "SENSOR > " << string(rowBuffer, 0, res);
	}
	return rowBuffer;
}


void Xport::Write2Sensor(string s) {
	if (s == "IM")
		configMode = true;
	else if (s == "EXIT" || "CONFIG")
		configMode = false;

	//s.push_back(carriageReturn);
	//s.push_back(lineFeed);
	
	send(connection, s.c_str(), s.size(), 0);

}