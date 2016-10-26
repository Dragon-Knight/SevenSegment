//#define SEVENSEGMENT_INVERT_ANODE
#define SEVENSEGMENT_INVERT_CATHODE
#define SEVENSEGMENT_USE_PORT
#define SEVENSEGMENT_DIGITS	4

#include <SevenSegment.h>


uint8_t pinDigits[] = {8, 9, 10, 11};
uint8_t pinSegments[] = {22, 23, 24, 25, 26, 27, 28, 29};

//SevenSegment display(pinDigits, pinSegments);
SevenSegment display(pinDigits, &PORTA);


uint32_t lastTime = 0;
int32_t iter = -10;
float iter2 = -10.0;

void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(25);
	
	display.Begin();
	display.SetChr(0, '1');
	display.SetChr(1, '2');
	display.SetChr(2, '3');
	display.SetChr(3, '4');
	display.SetDot(0, true);
	
	delay(500);
	
	display.SetRAW(2, 0b10000000);
	display.SetRAW(3, 0b00000001);
	
	delay(500);
	
	display.Clear();
	
	return;
}

void loop()
{
	uint32_t time = millis();
	
	if(lastTime + 10 <= time)
	{
		lastTime = time;
		iter++;
		iter2 += 0.1;
		display.SetNum(iter);
		//display.SetNum(iter2);
		if(iter % 2 == 0)
		{
			display.SetDot(1, true);
		}
		else
		{
			display.SetDot(1, false);
		}
	}
	
	if(Serial.available() > 0)
	{
		display.SetBrightness(Serial.parseInt());
	}
	
	return;
}


ISR(TIMER2_OVF_vect)
{
	cli();
	
	display.Lighting();
	
	sei();
}

ISR(TIMER2_COMPA_vect)
{
	cli();
	
	display.Dimming();
	
	sei();
}
