#include <fcntl.h>
#include <windows.h>
#include <stdio.h>
#include "IPololu.cpp"
#include <iostream>
#include <time.h>
using namespace std;
/** Open Port Funktion zum Aufbau eines Seriellen Ports*/
HANDLE openPort(const char* portName, unsigned int baudRate)
{
	HANDLE port;
	DCB commState;
	BOOL success;


	/** Open the serial port. */
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
	/** Set the baud rate. */
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
/** MainFunktion Testausführung der Funktionen */
int main (){

	HANDLE port;
	const char* portName;
	int baudRate;
	BOOL success;
	unsigned int target, position;
	IPololu Roboter;

	/** Test der Funktionen*/
	cout << "Test" << endl;
	portName = "COM4";  /**Portname ;Der Portname muss nach dem jeweiligen Anschluss geändert werden*/
	/** Choose the baud rate (bits per second).*/
	 
	baudRate = 9600;

	/* Open the Maestro's serial port. */
	port = openPort(portName, baudRate);
	if (port == INVALID_HANDLE_VALUE) { return -1; }

	//Test Funktionen
	Roboter.getPosition(port, 0, &position);	/**Getfunktion auf dem Channel 0 mit den Zugriff auf die Position*/
	Roboter.getmovingstate(port);	/**Motionen bestätigung */
	cout << position<< endl;	/**Ausgabe der Position*/

	target = (position < 6000) ? 7000 : 5000;
	Roboter.setSpeed(port, 0, 2);     /**Speed wird in (0,25us/10ms) 0 entspricht unlimitiert*/

	Roboter.setTarget(port, 0, 2000);	/**Setfunktionen auf dem Channel 0 mit der Position 2000*/
	Sleep(1000);
	Roboter.setTarget(port, 0, 5000);	/**Setfunktionen auf dem Channel 0 mit der Position 5000*/

	//Roboter.setMultipleTargets(port, 3, 0, target);
	
	

}

/*
BOOL setposition(HANDLE port,unsigned char channel,unsigned short target){
	IPololu setposi;
	DWORD bytesTransferred;	
}
BOOL setMultiple(HANDLE port,unsigned char numberoftargets, unsigned char channel, unsigned short target) {
	IPololu multi;
	DWORD bytesTransferred;
	//WriteFile(port, multi.setMultipleTargets.serialBytes , sizeof(multi.setMultipleTargets.serialBytes), &bytesTransferred, NULL);


}
BOOL setspeed(HANDLE port, unsigned char channel, unsigned short speed) {
	IPololu Tempo;
	DWORD bytesTransferred;
	//WriteFile(port, Tempo.setSpeed.command, sizeof(Tempo.setSpeed.command), &bytesTransferred, NULL);
}

BOOL Getposition(HANDLE port, unsigned char channel, unsigned short * position)
{
	IPololu Posi;
	DWORD bytesTransferred;
	//WriteFile(port, Posi.getPosition.command, sizeof(Posi.getPosition.command), &bytesTransferred, NULL);
	
	//ReadFile(port, Posi.getPosition.response, sizeof(Posi.getPosition.response), &bytesTransferred,NULL);
}
BOOL getmoving(HANDLE port) {
	IPololu move;
	DWORD bytesTransferred;
	//WriteFile(port, move.getmovingstate.command, sizeof(move.getmovingstate.command), &bytesTransferred, NULL);

	//ReadFile(port, move.getmovingstate.response, sizeof(move.getmovingstate.response), &bytesTransferred, NULL);
}*/