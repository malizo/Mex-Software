#include <fcntl.h>
#include <windows.h>
#include <stdio.h>
#include "IPololu.cpp"
using namespace std;
HANDLE openPort(const char* portName, unsigned int baudRate)
{
	HANDLE port;
	DCB commState;
	BOOL success;


	/* Open the serial port. */
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
	/* Set the baud rate. */
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
	/* Flush out any bytes received from the device earlier. */
	success = FlushFileBuffers(port);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to flush port buffers.  Error code 0x%x.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}
	return port;		
}

BOOL setposition(HANDLE port,unsigned char channel,unsigned short target){
	IPololu setposi;
	setposi.setTarget.serialBytes;
	DWORD bytesTransferred;
	WriteFile(port, setposi.setTarget.serialBytes, sizeof(setposi.setTarget.serialBytes), &bytesTransferred, NULL);
}
BOOL setMultiple(HANDLE port,unsigned char numberoftargets, unsigned char channel, unsigned short target) {
	IPololu multi;
	multi.setMultipleTargets.serialBytes;
	DWORD bytesTransferred;
	WriteFile(port, multi.setMultipleTargets.serialBytes , sizeof(multi.setMultipleTargets.serialBytes), &bytesTransferred, NULL);


}
BOOL setspeed(HANDLE port, unsigned char channel, unsigned short speed) {
	IPololu Tempo;
	Tempo.setSpeed.command;
	DWORD bytesTransferred;
	WriteFile(port, Tempo.setSpeed.command, sizeof(Tempo.setSpeed.command), &bytesTransferred, NULL);
}

BOOL Getposition(HANDLE port, unsigned char channel, unsigned short * position)
{
	IPololu Posi;
	Posi.getPosition.command;
	DWORD bytesTransferred;
	WriteFile(port, Posi.getPosition.command, sizeof(Posi.getPosition.command), &bytesTransferred, NULL);
	
	ReadFile(port, Posi.getPosition.response, sizeof(Posi.getPosition.response), &bytesTransferred, NULL);
}
BOOL getmoving(HANDLE port) {
	IPololu move;
	move.getPosition.command;
	DWORD bytesTransferred;
	WriteFile(port, move.getmovingstate.command, sizeof(move.getmovingstate.command), &bytesTransferred, NULL);

	ReadFile(port, move.getmovingstate.response, sizeof(move.getmovingstate.response), &bytesTransferred, NULL);
}
int main (){
	HANDLE port;
	char* portname;
	int baudRate;
}

