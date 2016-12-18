





















void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(25);
	
	return;
}

void loop()
{
	// INT
	if(Serial.available() > 0)
	{
		int32_t number = Serial.parseInt();
		
		Serial.print(" '");
		Serial.print(number);
		Serial.print("' - '");
		
		char characters[4] = {0x00};
		ltoa(number, characters, 10);
		
		
		
		
		uint8_t num = GetCount(characters);
		
		
		
		
		
		
		//for(int8_t i = (4 - 1); i >= 0; --i)
		//{
		//	if()
		//}
		
		for(uint8_t i = 0; i < (sizeof(characters) / sizeof(char)); ++i)
		{
			Serial.print(characters[i]);
			Serial.print(" ");
		}
		
		Serial.println("'.");
	}
	
	/*
	// FLOAT
	if(Serial.available() > 0)
	{
		float number = Serial.parseFloat();
		
		Serial.print(" '");
		Serial.print(number);
		Serial.print("' - '");
		
		char characters[4] = {0x00};
		dtostrf(number, 4, 2, characters);
		
		for(uint8_t i = 0; i < (sizeof(characters) / sizeof(char)); ++i)
		{
			Serial.print(characters[i]);
			Serial.print(" ");
		}
		
		Serial.println("'.");
	}
	*/
	
	
	return;
}


uint8_t GetCount(char *array)
{
	uint8_t count = 0;
	for(uint8_t i = 0; i < sizeof(array); ++i)
	{
		if(array[i] != ' ')
		{
			++count;
		}
	}
	
	return count;
}



