#include "Xport.h"

//Test för Github

using namespace std;

SOCKET connection; //Socket for connection, global use
sockaddr_in addr; // Hint struct to pass portNr and ip, etc. Global use
WSADATA wsaData;


Xport::Xport() : SocketOK(false), ConfigMode(false) {

}

Xport::Xport(int port, string Ip) : PortNr(port), IpAddress(Ip), ConfigMode(false) {

	SocketOK = OpenSocket();
}


Xport::~Xport() {

	CloseSocket();

}

bool Xport::OpenSocket() {

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

//void Xport::setUserParameters(int baudRate, int aveNr, int meanNr, int sampFreq) {
//
//
//
//}

void Xport::StringFromSensor(char* s, int size) {
	memset(s, 0, size);
	int res = 0;
	char buf[MAXBUFFER];

	if (!ConfigMode) {

		res = recv(connection, buf, MAXBUFFER, 0);
		memcpy(s, buf, res);


	}
}


void Xport::Write2Sensor(string s) {

	if (s == "IM")
		ConfigMode = true;
	else if (s == "EXIT")
		ConfigMode = false;

	string sendString = s + "\r\n";
	send(connection, sendString.c_str(), sendString.size(), 0);
	Sleep(50);
}
