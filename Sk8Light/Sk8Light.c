/*
 * Sk8Light.c
 *
 * Created: 28-06-2014 08:56:29
 * Author: donjo9
 * Attiny85 8Mhz
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define RedLED 0x01 //Tiny 0
#define GreenLED 0x02 //Tiny 1
#define BlueLED 0x08 //Tiny 2

#define TransitionTime 5 //Sekunder fra slukket til helt tændt

#define DelayValue 1000/(255/TransitionTime)
int Mode = 0;

int RedValue = 0x00;
int GreenValue = 0xFF;
int BlueValue = 0xFF;

void init(void);
void UpdateValues(void);

int main(void)
{
	init();
	int counter = 0;
	int delay = (int)DelayValue;
    while(1)
    {
        
		_delay_ms(delay);
		
		if (counter >= 255)
		{
			counter = 0;
			Mode = (Mode + 1)%6;
		}
		switch(Mode)
		{
			//Rød
			case 1: //Skrue Ned
				RedValue++;
				break;
			case 4: //Skrue Op
				RedValue--;
				break;
			//Grøn
			case 0: //Skrue Op
				GreenValue--;
				break;
			case 3: //Skrue Ned
				GreenValue++;
				break;
			//Blå
			case 2: //Skrue Op
				BlueValue--;
				break;
			case 5: //Skrue Ned
				BlueValue++;
				break;
			default:
				break;		
		}
		UpdateValues();
		counter++;
		
    }
}



void init()
{
	DDRB |= RedLED | GreenLED | BlueLED; //Sætter Diode Pins til Output
	
	GTCCR |= TSM; //Stopper Timers under init
	
	TCCR0A |= (1<<COM0A0) | (1<<COM0B0) | (1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);
					//Slukker Dioder i Bund og tænder ved Compare Match
					//WGM = Fast PWM opdatering @ BOTTOM, TOV @ MAX
	TCCR0B |= (1<<CS01) | (1<<CS00); //Prescale = 64, overflow med 500Hz
	
	TCCR1 |= (1<<CS12) | (1<<CS11) | (1<<CS10); //Prescale = 64, overflow med 500Hz
	GTCCR |= (1<<PWM1B) | (1<<COM1B0); //Enable PWM B, //Slukker Dioder i Bund og tænder ved Compare Match
	OCR1C = 0xFF; //Sætter Max på Timer1 til 0xFF
	
	UpdateValues(); //Sætter Overflow Compare Registre
	GTCCR &= ~TSM; //Starter Timers efter init
	
}

void UpdateValues()
{
	OCR0A = RedValue; //Rød Diode
	OCR0B = GreenValue; //Grøn Diode
	OCR1B = BlueValue; //Blå Diode
}
