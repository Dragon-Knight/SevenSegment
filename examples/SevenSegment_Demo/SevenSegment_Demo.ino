//#define SEVENSEGMENT_INVERT_SEGMENTS
#define SEVENSEGMENT_INVERT_DIGITS
#define SEVENSEGMENT_USE_PORT
#define SEVENSEGMENT_DIGITS	4
#include <SevenSegment.h>

// Пример пинов для Arduino Mega 2560.
uint8_t pinDigits[] = {8, 9, 10, 11};
uint8_t pinSegments[] = {22, 23, 24, 25, 26, 27, 28, 29};

//SevenSegment display(pinDigits, pinSegments);
SevenSegment display(pinDigits, &PORTA);


uint32_t lastTime = 0;
int32_t iter = 0;
float iter2 = 0.0;

byte anim[] = {0b00000111, 0b00001110, 0b00011100, 0b00111000, 0b00110001, 0b00100011};

void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(25);
	
	display.Begin();
	
	display.SetChr(0, 'L');
	display.SetChr(1, 'O');
	display.SetChr(2, 'A');
	display.SetChr(3, 'd');
	display.SetDot(0, true);
	
	delay(1000);
	
	display.Clear();
	display.SetRAW(1, 0b01001001);
	display.SetRAW(2, 0b00110110);
	
	delay(1000);
	
	display.Clear();
	for(uint32_t i = 0; i < sizeof(anim) * 10; ++i)
	{
		for(uint8_t j = 0; j < SEVENSEGMENT_DIGITS; ++j)
		{
			display.SetRAW(j, anim[i % sizeof(anim)]);
		}
		
		delay(100);
	}
	
	display.Clear();
	
	return;
}

void loop()
{
	uint32_t time = millis();
	
	if(lastTime + 100 <= time)
	{
		lastTime = time;
		
		iter++;
		iter2 += 0.1;
		
		display.SetNums(iter);			// int
		//display.SetNums(iter2);		// float
		
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
