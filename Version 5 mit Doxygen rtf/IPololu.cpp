#include <stdio.h>
#include <Windows.h>
#include <fcntl.h>

class IPololu {
public:
/**Positions Setzfunktion
 Target uebergabe in Low bits und High Bits (Wertebereich 0-8000)
 Channel uebergabe als einen Ganzzahligen Integer je nach angeschlossem Eingang
 Port uebergabe durch die WindowsAPI in HANDLE (wird vom ISerialCom bestimmt)
 Rueckgabe der Funktion durch eine 1
 Bewegung des Servos auf die eingebene Position
*/
	unsigned char setTarget(HANDLE port,unsigned char channel,unsigned short target)
	{
		/** Position die angefahren werden in Absolute Positionen angegeben meistens von 1000-8000*/
		DWORD bytesTransfered;
		unsigned char serialBytes[4];
		serialBytes[0] = 0x84; /**Command-Byte vom Hersteller des Controller vorgegeben*/
		serialBytes[1] = channel;	/**First data Byte holds the channel number*/
		serialBytes[2] = target & 0x7F;	 /** Second byte holds the lower 7 bits of target */
		serialBytes[3] = (target >> 7) & 0x7F; /** Third data byte holds the bits 7-13 of target*/
		WriteFile(port, serialBytes, sizeof(serialBytes), &bytesTransfered, NULL); /**Befehluebergabe zur Einstellung der Position*/
		return 1;
		
	}
	/** Einstellung von mehreren Servos gleichzeitig
	 Target uebergabe in Low bits und High Bits (Wertebereich 0-8000)
	 Channel uebergabe als einen Ganzzahligen Integer je nach angeschlossem Eingang
	 Port uebergabe durch die WindowsAPI in HANDLE (wird vom ISerialCom bestimmt)
	 numberoftargets uebergabe in ganzahligen Zahlen je nach Wertebereich
	 Rueckgabe der Funktion durch eine 1
     Gleichzeitige Bewegung einer Beliebigen Anzahl von Servos auf die eingebene Position
	*/
	unsigned char setMultipleTargets(HANDLE port,unsigned char numberoftargets,unsigned char channel, unsigned short target){
		unsigned char serialBytes[5];
		DWORD bytesTransfered;
		serialBytes[0] = 0x9F; /**Command-Byte vom Hersteller des Controller vorgegeben*/
		serialBytes[1] = numberoftargets; /** First Byte holds how many channels are in one block*/
		serialBytes[2] = channel; /**Second data Byte holds the lowest channel number*/
		serialBytes[3] = target & 0x7F;  /** Third byte holds the lower 7 bits of target */
		serialBytes[4] = (target >> 7) & 0x7F;  /** Fourth data byte holds the bits 7-13 of target*/
		WriteFile(port,serialBytes, sizeof(serialBytes), &bytesTransfered, NULL); /**Befehluebergabe zur Einstellung mehrerer Positionen auf mehreren Servos*/
		return 1;
	}
	/**Geschwindigkeiteinstellung
	 Channel uebergabe als einen Ganzzahligen Integer je nach angeschlossem Eingang
	 Port uebergabe durch die WindowsAPI in HANDLE (wird vom ISerialCom bestimmt)
	 Speed uebergabe in (0.25μs)/(10ms) auch schreibbar als Dezimalzahlen
	 Der Wert 0 in der Variable Speed bedeutet das die Maximal Geschwindigkeit eingestellt wird ,
	 somit ist 1 die kleinste Einstellbare Groeße und ein guter Default Wert waere hier fuer 100
	 Rueckgabe ist 1 wenn die Funktion ausgefuehrt ist 
	 Zusaetzlich eine Bewegung Sichtbar am Test-Servo
	
	*/
	unsigned char setSpeed(HANDLE port,unsigned char channel,unsigned short speed)
	{
		unsigned char command[4];
		
		DWORD bytesTransfered;
		command[0] = 0x87;  /**Command-Byte vom Hersteller des Controller vorgegeben*/
		command[1] = channel; /**First data Byte holds the channel number*/
		command[2] = speed & 0x7F; /**Second data Byte holds the speed Low bits*/
		command[3] = (speed >> 7) & 0x7F;  /**Third data Byte holds the speed High bits*/
		WriteFile(port,command, sizeof(command), &bytesTransfered, NULL); //Befehluebergabe zur Geschwindigkeitseinstellung
		return 1;
	}
	/** Positions Rueckgabe
	Channel uebergabe als einen Ganzzahligen Integer je nach angeschlossem Eingang
	 Port uebergabe durch die WindowsAPI in HANDLE (wird vom ISerialCom bestimmt)
	 uebermittlung des Writebefehls zur ausfuehrung des Befehles
	 Position Zugriff auf den Speicher der vorausgefuehrten Befehle
	 Rueckgabe der Position in Absoulten Positionen nach einer Umrechnung von Bytes
	*/
	unsigned char getPosition(HANDLE port ,unsigned char channel, unsigned int* position) /** Zugriff auf den Inhalt der Position*/
	{
		
		unsigned char command[2];
		unsigned char response[2];
		
		DWORD bytesTransfered;
		BOOL success; 
		BOOL read;
		command[0] = 0x90; /**Command-Byte vom Hersteller des Controller vorgegeben*/
		command[1] = channel; /**First data Byte holds the channel number*/

		success=WriteFile(port, command, sizeof(command), &bytesTransfered, NULL); /** Befehluebergabe zur Positionsbestimmung*/

		read=ReadFile(port, response, sizeof(response), &bytesTransfered, NULL);

		*position = response[0] + 256 * response[1]; /** umrechnung der Position von Byte auf Absoulte Positionen*/

		return 1;				
	}
	/**Bewegungsbestimmung
	Port uebergabe durch die WindowsAPI in HANDLE (wird vom ISerialCom bestimmt)
	Rueckgabe Boolscher Wert 1 fuer eine vorhandende Bewegung und 0 fuer einen Standbetrieb
	*/
	unsigned char getmovingstate(HANDLE port)
	{
		DWORD bytesTransfered;
		BOOL success;
		BOOL read;
		unsigned char command[1];
		unsigned char response[1];
		command[0] = 0x93; /**Command-Byte vom Hersteller des Controller vorgegeben*/

		success=WriteFile(port,command, sizeof(command), &bytesTransfered, NULL); /**Befehluebergabe zur Bewegungsbestimmung*/

		read=ReadFile(port,response, sizeof(response), &bytesTransfered, NULL);

		return 1; /**Rueckgabewert 1 wenn dieser sich noch bewegt*/

	}


};
