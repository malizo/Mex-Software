#include <stdio.h>
#include <fcntl.h>

class IPololu {
public:
	unsigned char setTarget(unsigned char channel,unsigned short target)
	{
		unsigned char serialBytes[4];
		serialBytes[0] = 0x84;
		serialBytes[1] = channel;
		serialBytes[2] = target & 0x7F;
		serialBytes[3] = (target >> 7) & 0x7F;
		
	}
	unsigned char setMultipleTargets(unsigned char numberoftargets,unsigned char channel, unsigned short target){
		unsigned char serialBytes[5];
		serialBytes[0] = 0x9F;
		serialBytes[1] = numberoftargets;
		serialBytes[2] = channel;
		serialBytes[3] = target & 0x7F;
		serialBytes[4] = (target >> 7) & 0x7F;
	}
	unsigned char setSpeed(unsigned char channel,unsigned short speed)
	{
		unsigned char command[4];
		command[0] = 0x87;
		command[1] = channel;
		command[2] = speed & 0x7F;
		command[3] = (speed >> 7) & 0x7F;
	}

	unsigned char getPosition(unsigned char channel)
	{
		unsigned char command[2];
		unsigned char response[2];
		command[0] = 0x90;
		command[1] = channel;
		return 0;				//response in Byte umrechnung muss noch erfolgen in Postion response = response[0] + 256*response[1] 
	}
	unsigned char getmovingstate()
	{
		unsigned char command[1];
		unsigned char response[1];
		command[0] = 0x93;

		return 0;

	}


};
