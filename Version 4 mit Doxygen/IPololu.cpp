#include <stdio.h>
#include <Windows.h>
#include <fcntl.h>

class IPololu {
public:
/**Positions Setzfunktion*/
	unsigned char setTarget(HANDLE port,unsigned char channel,unsigned short target)
	{
		/** Position die angefahren werden in ganzzahligen Ticks angegeben meistens von 1000-8000*/
		DWORD bytesTransfered;
		unsigned char serialBytes[4];
		serialBytes[0] = 0x84; /**Hersteller Protokollwert*/
		serialBytes[1] = channel;	/**Servonummer die angesprochen werden soll*/
		serialBytes[2] = target & 0x7F;	 
		serialBytes[3] = (target >> 7) & 0x7F;
		WriteFile(port, serialBytes, sizeof(serialBytes), &bytesTransfered, NULL); /**Befehluebergabe zur Einstellung der Position*/
		return 1;
		
	}
	/** Einstellung von mehreren Servos gleichzeitig*/
	unsigned char setMultipleTargets(HANDLE port,unsigned char numberoftargets,unsigned char channel, unsigned short target){
		unsigned char serialBytes[5];
		DWORD bytesTransfered;
		serialBytes[0] = 0x9F; /**Hersteller Protokollwert*/
		serialBytes[1] = numberoftargets; /** Anzahl der anzusprechen Servos/Ziele ; Ganzzahlige int übergabe*/
		serialBytes[2] = channel; /**Servonummer die angesprochen werden soll ;*/
		serialBytes[3] = target & 0x7F; /**uebergabe der Position wie in der Settarget Funktion in Ticks*/
		serialBytes[4] = (target >> 7) & 0x7F;
		WriteFile(port,serialBytes, sizeof(serialBytes), &bytesTransfered, NULL); /**Befehlübergabe zur Einstellung mehrerer Positionen auf mehreren Servos*/
		return 1;
	}
	/**Geschwindigkeiteinstellung*/
	unsigned char setSpeed(HANDLE port,unsigned char channel,unsigned short speed)
	{
		unsigned char command[4];
		
		DWORD bytesTransfered;
		command[0] = 0x87; /**Hersteller Protokollwert*/
		command[1] = channel; /**Servonummer die angesprochen werden soll*/
		command[2] = speed & 0x7F; /** uebergabe der Geschwindigkeit in (0,25us/10ms) ; Ein guter Wert hierfür waere 10 ; Wenn die Zahl 0 hier übergeben wird bedeutet dies das der Servo die Maximal Geschwindigkeit benutzt; 1 entspricht dem kleinsten Wert*/
		command[3] = (speed >> 7) & 0x7F;
		WriteFile(port,command, sizeof(command), &bytesTransfered, NULL); //Befehlübergabe zur Geschwindigkeitseinstellung
		return 1;
	}
	/** Positions Rueckgabe */
	unsigned char getPosition(HANDLE port ,unsigned char channel, unsigned int* position) /** Zugriff auf den Inhalt der Position*/
	{
		/** Uebergabe */
		unsigned char command[2];
		unsigned char response[2];
		
		DWORD bytesTransfered;
		BOOL success;
		BOOL read;
		command[0] = 0x90; /**Hersteller Protokollwert*/
		command[1] = channel; /**Servonummer die angesprochen werden soll*/

		success=WriteFile(port, command, sizeof(command), &bytesTransfered, NULL); /** Befehluebergabe zur Positionsbestimmung*/

		read=ReadFile(port, response, sizeof(response), &bytesTransfered, NULL);

		*position = response[0] + 256 * response[1]; /** umrechnung der Position von Byte auf Ticks*/

		return 1;				
	}
	/**Bewegungsbestimmung*/
	unsigned char getmovingstate(HANDLE port)
	{
		DWORD bytesTransfered;
		BOOL success;
		BOOL read;
		unsigned char command[1];
		unsigned char response[1];
		command[0] = 0x93; /**Hersteller Protokollwert*/

		success=WriteFile(port,command, sizeof(command), &bytesTransfered, NULL); /**Befehluebergabe zur Bewegungsbestimmung*/

		read=ReadFile(port,response, sizeof(response), &bytesTransfered, NULL);

		return 1; /**Rueckgabewert 1 wenn dieser sich noch bewegt*/

	}


};
