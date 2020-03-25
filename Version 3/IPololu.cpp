#include <stdio.h>
#include <Windows.h>
#include <fcntl.h>

class IPololu {
public:
	unsigned char setTarget(HANDLE port,unsigned char channel,unsigned short target)
	{
		DWORD bytesTransfered;
		unsigned char serialBytes[4];
		serialBytes[0] = 0x84;
		serialBytes[1] = channel;
		serialBytes[2] = target & 0x7F;
		serialBytes[3] = (target >> 7) & 0x7F;
		WriteFile(port, serialBytes, sizeof(serialBytes), &bytesTransfered, NULL);
		return 1;
		
	}
	unsigned char setMultipleTargets(HANDLE port,unsigned char numberoftargets,unsigned char channel, unsigned short target){
		unsigned char serialBytes[5];
		DWORD bytesTransfered;
		serialBytes[0] = 0x9F;
		serialBytes[1] = numberoftargets;
		serialBytes[2] = channel;
		serialBytes[3] = target & 0x7F;
		serialBytes[4] = (target >> 7) & 0x7F;
		WriteFile(port,serialBytes, sizeof(serialBytes), &bytesTransfered, NULL);
		return 1;
	}
	unsigned char setSpeed(HANDLE port,unsigned char channel,unsigned short speed)
	{
		unsigned char command[4];
		
		DWORD bytesTransfered;
		command[0] = 0x87;
		command[1] = channel;
		command[2] = speed & 0x7F;
		command[3] = (speed >> 7) & 0x7F;
		WriteFile(port,command, sizeof(command), &bytesTransfered, NULL);
		return 0;
	}

	unsigned char getPosition(HANDLE port ,unsigned char channel, unsigned int* position)
	{
		unsigned char command[2];
		unsigned char response[2];
		
		DWORD bytesTransfered;
		BOOL success;
		BOOL read;
		command[0] = 0x90;
		command[1] = channel;

		success=WriteFile(port, command, sizeof(command), &bytesTransfered, NULL);

		read=ReadFile(port, response, sizeof(response), &bytesTransfered, NULL);

		*position = response[0] + 256 * response[1];

		return 1;				//response in Byte umrechnung muss noch erfolgen in Postion response = response[0] + 256*response[1] 
	}
	unsigned char getmovingstate(HANDLE port)
	{
		DWORD bytesTransfered;
		BOOL success;
		BOOL read;
		unsigned char command[1];
		unsigned char response[1];
		command[0] = 0x93;

		success=WriteFile(port,command, sizeof(command), &bytesTransfered, NULL);

		read=ReadFile(port,response, sizeof(response), &bytesTransfered, NULL);

		return 1;

	}


};
