/*
 *	SevenSegment.h
 *	Basic class of Seven Segment Indicator
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/SevenSegment
 */

#ifndef SevenSegment_h
#define SevenSegment_h

#include "Arduino.h"
#include "SevenSegmentFont.h"
#include "SevenSegmentCore.h"

class SevenSegment : public SevenSegmentCore<SEVENSEGMENT_DIGITS>
{
	public:
		#if defined(SEVENSEGMENT_USE_PORT)
		SevenSegment(const uint8_t *pinDigits, volatile uint8_t *port) : SevenSegmentCore(pinDigits, port)
		{
			return;
		}
		#else
		SevenSegment(const uint8_t *pinDigits, const uint8_t *pinSegments) : SevenSegmentCore(pinDigits, pinSegments)
		{
			return;
		}
		#endif
		
		void Begin()
		{
			this->SetupTimer();
			
			return;
		}
		
		void SetNums(int32_t number)		// Установить чисто в указанную позицию, относительно базиса (если в числе появится доп. разряд, то базис определяет направление смещения).
		{
			// Допилить возможность выбора базиса для отрисовки строки.
			
			// Если число отрицательное, то выводить не более чем MAX-1, например для 4 сегментов: "-999" и при превышение замораживать счётчик.
			// Если число положительное, то выводить не более чем MAX, например для 4 сегментов: "9999" и при превышение замораживать счётчик.
			// Если число float, то передавать число и кол-во знаков после запятой, + правила что выше.
			
			char characters[SEVENSEGMENT_DIGITS + 1] = {0x00};
			
			ltoa(number, characters, 10);

			uint8_t idx = 0;
			for(int8_t i = 0; i < SEVENSEGMENT_DIGITS; ++i)
			{
				if(characters[i] == 0x00)
				{
					this->SetChr(i, ' ');
				}
				else
				{
					this->SetChr(i, characters[idx]);
					idx++;
				}
			}
			
			return;
		}
		
		void SetNums(float number)		// Установить чисто в указанную позицию, относительно базиса (если в числе появится доп. разряд, то базис определяет направление смещения).
		{
			// Допилить возможность выбора базиса для отрисовки строки.
			
			// Если число отрицательное, то выводить не более чем MAX-1, например для 4 сегментов: "-999" и при превышение замораживать счётчик.
			// Если число положительное, то выводить не более чем MAX, например для 4 сегментов: "9999" и при превышение замораживать счётчик.
			// Если число float, то передавать число и кол-во знаков после запятой, + правила что выше.
			
			char characters[SEVENSEGMENT_DIGITS + 1] = {0x00};
			
			dtostrf(number, SEVENSEGMENT_DIGITS, 1, characters);

			uint8_t idx = 0;
			for(int8_t i = 0; i < SEVENSEGMENT_DIGITS; ++i)
			{
				if(characters[idx] == 0x00)
				{
					this->SetChr(i, ' ');
				}
				else
				{
					this->SetChr(i, characters[idx]);
					if(characters[idx] == '.')
					{
						this->SetDot(i, true);
						i--;
					}
					idx++;
				}
			}
			
			return;
		}
		
		void SetBrightness(uint8_t brightness)
		{
			if(brightness > 0)
			{
				this->SetPower(true);
				
				#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
				OCR2A = brightness;
				#endif
			}
			else
			{
				this->SetPower(false);
			}
			
			return;
		}
		
		void SetupTimer()
		{
			#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
			ASSR = (0<<EXCLK) | (0<<AS2);
			TCCR2A = (0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B = (0<<WGM22) | (1<<CS22) | (0<<CS21) | (1<<CS20);
			TCNT2 = 0x00;
			OCR2A = 0x80;
			
			TIMSK2=(0<<OCIE2B) | (1<<OCIE2A) | (1<<TOIE2);
			#endif
			
			return;
		}
	private:
};

#endif
