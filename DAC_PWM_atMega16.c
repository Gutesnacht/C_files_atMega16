/*
 * DAC_PWM_MCT_P2.c
 *
 * Created: 26.05.2016 16:49:18
 * Author : BABO
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void EEPROM_WRITE (unsigned int uiAddresse, unsigned char ucDATA)
{
	while (EECR & (1<<EEWE))// Aktuellen Schreibzugriff abwarten!
	{
		;
	}
	EEAR = uiAddresse;// Adresse angeben, das ding is 16 bit lang!(9 nutzbar==512 byte);
	EEDR = ucDATA; // 1Byte Daten schreiben an og Adressen
	EECR|=(1<<EEMWE); // Master Write Enable, muss vorher passieren!
	EECR|=(1<<EEWE);	//EEProm write enable, starten der Übertragung
}



unsigned char EEPROM_READ (unsigned int uiAdresse)
{
	while(EECR & (1<<EEWE)) // Letzten Lesezyklus abwarten
	;
	EEAR=uiAdresse; // Adresse zuweisen
	EECR|=(1<<EERE); // Lesezyklus starten
	return EEDR; // Rückgabe des EEPROM-Speicherplatzes
}

void FAST_PWM (unsigned char Vergleichswert)
{
	OCR2=Vergleichswert; //Setzen des Vergleichwertes für Timer (Puls-Dauer)
	TCCR2=(1<<WGM20) | (1<<WGM21) | (1<<COM21) | (1<<CS20); // Timer einstellen, schnelle PWM, nichtinvertierend, keine Frequenzuntersetzung
}


int main(void)
{
	
	DDRD=0xFF;
	
	unsigned int Addr[3]={1,2,3}; //Adress Array
	unsigned char Data[3]={0x10,0x4F,0xF0}; // Datenarray
	unsigned char Vergleichswert; //Vergleichswert initialisieren
		
		for (int i=0;i<3;i++) //Beschreiben des EEPROMs, NUR BEI PROGRAMMSTART!
		{
			EEPROM_WRITE(Addr[i],Data[i]);
		}
	


    while (1)  //Endlosschleife
    {	
		for (int j=0;j<3;j++) // Durchschalten der verschiedenen EEPROM-Zustände auf Ausgang
    {
		Vergleichswert=EEPROM_READ(Addr[j]);
		
		FAST_PWM(Vergleichswert);
		//_delay_ms(5000); //Anschalten , für Sim-Zwecke ausgeschaltet
    }

		
    }
}

