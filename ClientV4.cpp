#include "Xport.h"

int main() {

	int port = 4001;

	string ipAddress = "192.168.10.253";

	Xport myXport(port, ipAddress);

	string stop = "IM";
	string go = "CONFIG";

	//myXport.Write2Sensor(stop);
	myXport.Write2Sensor("IM\r\n");
	myXport.Write2Sensor("IM\r\n");
	myXport.StringFromSensor();
	myXport.Write2Sensor("CONFIG\r\n");
	myXport.StringFromSensor();
	Sleep(5000);
	//myXport.Write2Sensor("EXIT\r\n");
	myXport.Write2Sensor("EXIT\r\n");
	myXport.StringFromSensor();
	myXport.StringFromSensor();
	myXport.StringFromSensor();
	myXport.StringFromSensor();

				
	//
	//myXport.Write2Sensor(go);
	//myXport.StringFromSensor();
	//myXport.Write2Sensor(go);
	//
	//myXport.StringFromSensor();
	
	char buf[200];
	int i = 0;
	//while(bool go = true) {
	//	myXport.StringFromSensor();
	//	cout << string(myXport.StringFromSensor(),0,40) << endl;
	//	cout << "Data" << endl;
	//	if (i == 100) {
	//		myXport.Write2Sensor(stop);
	//		myXport.StringFromSensor();
	//		go = false;
	//	}
	//	i++;
	//}
	
	
	//(portNr, ipAddress);


	system("pause");
	return 0;

}