#include <fcntl.h>
#include <windows.h>
#include <stdio.h>
#include "IPololu.cpp"
#include <iostream>
#include <time.h>
using namespace std;
/** Open Port Funktion zum Aufbau eines Seriellen Ports
uebergabe des Portnamen der Schnittstelle durch die Main mithilfe einem const Char*
Baudrate uebergabe durch vordefinierte Baudrate in der Main als Integerwert 9600 ueblicher Wert
*/
HANDLE openPort(const char* portName, unsigned int baudRate)
{
	HANDLE port;
	DCB commState;
	BOOL success;


	/** 
	oeffnen des Serielen Ports mithilfe von WindowsAPI
	*/
	port = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (port == INVALID_HANDLE_VALUE)
	{
		switch (GetLastError())
		{
		case ERROR_ACCESS_DENIED:
			fprintf(stderr, "Error: Access denied.  Try closing all other programs that are using the device.\n");
			break;
		case ERROR_FILE_NOT_FOUND:
			fprintf(stderr, "Error: Serial port not found.  "
				"Make sure that \"%s\" is the right port name.  "
				"Try closing all programs using the device and unplugging the "
				"device, or try rebooting.\n", portName);
			break;
		default:
			fprintf(stderr, "Error: Unable to open serial port.  Error code 0x%x.\n", GetLastError());
			break;
		}
		return INVALID_HANDLE_VALUE;
	}
	/** Set the baud rate.
	Baudrate dem System uebermittelen mithilfe von WindowsAPI
	*/
	success = GetCommState(port, &commState);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to get comm state.  Error code 0x%x.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}
	commState.BaudRate = baudRate;
	success = SetCommState(port, &commState);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to set comm state.  Error code 0x%x.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}
	/** Flush out any bytes received from the device earlier. */
	success = FlushFileBuffers(port);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to flush port buffers.  Error code 0x%x.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}
	return port;
};
/** MainFunktion Testausfuehrung der Funktionen 
	Definierung der Variable Port durch die Class Openport
	Benutzung der Funktionen der IPololu-klasse
	Initalisierung der Variablen Baudrate , Portname , Target und Position
*/
int main (){

	HANDLE port;
	const char* portName;
	int baudRate;
	unsigned int target, position;
	IPololu Roboter;

	/** Test der Funktionen*/
	cout << "Test" << endl;
	portName = "COM4";  /**Portname ;Der Portname muss nach dem jeweiligen Anschluss geaendert werden*/
	/** Choose the baud rate (bits per second).*/
	 
	baudRate = 9600;

	/* Open the Maestro's serial port. */
	port = openPort(portName, baudRate);
	if (port == INVALID_HANDLE_VALUE) { return -1; }

	//Test Funktionen
	Roboter.getPosition(port, 0, &position);	/**Getfunktion auf dem Channel 0 mit den Zugriff auf die Position*/
	Roboter.getmovingstate(port);	/**Motionen bestaetigung */
	cout << position<< endl;	/**Ausgabe der Position*/

	target = (position < 6000) ? 7000 : 5000;
	Roboter.setSpeed(port, 0, 100);     /**Speed wird in (0,25us/10ms) 0 entspricht unlimitiert*/

	Roboter.setTarget(port, 0, 1000);	/**Setfunktionen auf dem Channel 0 mit der Position 2000*/
	Sleep(1000);
	Roboter.setTarget(port, 0, 7999);	/**Setfunktionen auf dem Channel 0 mit der Position 5000*/

	
	

}