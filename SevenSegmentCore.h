/*
	SevenSegmentCore.h - Класс динамической индикации семисегментного индикатора.
	Автор: Dragon_Knight, 2016 ( https://vk.com/globalzone_edev ).
	
	Методы:
		SevenSegmentCore:
			В качестве первого параметра выступает массив пинов разрядов, слева направо.
			В качестве второго параметра выступает или массив сегментов в общепринятом порядке, или ссылка на порт.
		void SetPower(bool status) - Выключить экран, без очистки.
		void SetChr(uint8_t position, char character) - Установить указанный символ в указанную позицию.
		void SetNum(uint8_t position, uint8_t number) - Установить указанное число в указанную позицию.
		void SetDot(uint8_t position, bool dot) - Установить\Снять точку в указанною позицию.
		void SetRAW(uint8_t position, byte data) - Установить указанный байт в указанную позицию (Порядок бит как порядок сегментов).
		void Clear() - Очистить экран.
		void Lighting() - Обновление динамической индикации. Нужно вызывать с частотой не реже 20мс\сегмент.
		void Dimming() - Принудительное гашение разрядя. Используется для настройки яркости.
	Константы:
		SEVENSEGMENT_USE_PORT - Включает указание порта в качестве пинов сегментом.
		SEVENSEGMENT_INVERT_SEGMENTS - Включает инверсию анодов.
		SEVENSEGMENT_INVERT_DIGITS - Включает инверсию катодов.
	Примечания:
		По умолчанию, аноды и катоды имеют включённое состояние - лог. 1, и выключенное состояние - лог. 0,
		однако и аноды и катоды можно инвертировать. Для этого перед подключением класса используем константы.
		Также для увеличения скорости работы индикатора в десятки раз добавлена возможность использовать порт,
		в качестве сегментов инжектора. Порядок сегментов от младшего пина порта. Для включения этой возможности
		используем определённую константу.
		
		Может работать как самостоятельный класс.
*/

#ifndef SevenSegmentCore_h
#define SevenSegmentCore_h

#include "Arduino.h"

#define PIN_TO_BASEREG(pin)				(portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)				(digitalPinToBitMask(pin))
#define DIRECT_READ(base, mask)			(((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)	((*((base)+1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask)	((*((base)+1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)	((*((base)+2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)	((*((base)+2)) |= (mask))

template <uint8_t _digits>
class SevenSegmentCore
{
	public:
	
		#if defined(SEVENSEGMENT_USE_PORT)
		SevenSegmentCore(const uint8_t *pinDigits, volatile uint8_t *port)
		#else
		SevenSegmentCore(const uint8_t *pinDigits, const uint8_t *pinSegments)
		#endif
		{
			for(uint8_t i = 0; i < _digits; ++i)
			{
				this->_digits_base[i] = PIN_TO_BASEREG(pinDigits[i]);
				this->_digits_mask[i] = PIN_TO_BITMASK(pinDigits[i]);
				DIRECT_MODE_OUTPUT(this->_digits_base[i], this->_digits_mask[i]);
				#if defined(SEVENSEGMENT_INVERT_DIGITS)
				DIRECT_WRITE_HIGH(this->_digits_base[i], this->_digits_mask[i]);
				#else
				DIRECT_WRITE_LOW(this->_digits_base[i], this->_digits_mask[i]);
				#endif
			}
			
			#if defined(SEVENSEGMENT_USE_PORT)
			this->_port = port;
			*((this->_port)-1) = 0b11111111;
			#if defined(SEVENSEGMENT_INVERT_SEGMENTS)
			*(this->_port) = 0b11111111;
			#else
			*(this->_port) = 0b00000000;
			#endif
			#else
			for(uint8_t i = 0; i < 8; ++i)
			{
				this->_segments_base[i] = PIN_TO_BASEREG(pinSegments[i]);
				this->_segments_mask[i] = PIN_TO_BITMASK(pinSegments[i]);
				DIRECT_MODE_OUTPUT(this->_segments_base[i], this->_segments_mask[i]);
				#if defined(SEVENSEGMENT_INVERT_SEGMENTS)
				DIRECT_WRITE_HIGH(this->_segments_base[i], this->_segments_mask[i]);
				#else
				DIRECT_WRITE_LOW(this->_segments_base[i], this->_segments_mask[i]);
				#endif
			}
			#endif
			
			this->Clear();
			this->SetPower(true);
			
			return;
		}
		
		void SetPower(bool status)
		{
			this->_power = status;
			
			return;
		}
		
		void SetChr(uint8_t position, char character)
		{
			if(position < _digits)
			{
				this->_drawingData[position] = pgm_read_byte_near(&SEVENSEGMENTFONT[character] - 32);
			}
			
			return;
		}
		
		void SetNum(uint8_t position, uint8_t number)
		{
			if(position < _digits)
			{
				this->_drawingData[position] = pgm_read_byte_near(&SEVENSEGMENTFONT[number] + 16);
			}
			
			return;
		}
		
		void SetDot(uint8_t position, bool dot)
		{
			if(position < _digits)
			{
				if(dot == true)
				{
					this->_drawingData[position] |= pgm_read_byte_near(&SEVENSEGMENTFONT[46] - 32);
				}
				else
				{
					this->_drawingData[position] &= ~(pgm_read_byte_near(&SEVENSEGMENTFONT[46] - 32));
				}
			}
			
			return;
		}
		
		void SetRAW(uint8_t position, byte data)
		{
			if(position < _digits)
			{
				this->_drawingData[position] = data;
			}
			
			return;
		}
		
		void Clear()
		{
			for(uint8_t i = 0; i < _digits; ++i)
			{
				this->_drawingData[i] = 0x00;
				this->_drawingDataIndex = 0;
			}
			
			return;
		}
		
		void Lighting()
		{
			// Выключаем предыдущий разряд //
			this->Dimming();
			// //
			
			// Итератор //
			if(++this->_drawingDataIndex == _digits)
			{
				this->_drawingDataIndex = 0;
			}
			// //
			
			if(this->_power == true)
			{
				#if defined(SEVENSEGMENT_USE_PORT)
				#if defined(SEVENSEGMENT_INVERT_SEGMENTS)
				*(this->_port) = ~this->_drawingData[this->_drawingDataIndex];
				#else
				*(this->_port) = this->_drawingData[this->_drawingDataIndex];
				#endif
				#else
				for(uint8_t s = 0; s < 8; ++s)
				{
					if(bitRead(this->_drawingData[this->_drawingDataIndex], s) == HIGH)
					{
						// Включаем нужный сегмент //
						#if defined(SEVENSEGMENT_INVERT_SEGMENTS)
						DIRECT_WRITE_LOW(this->_segments_base[s], this->_segments_mask[s]);
						#else
						DIRECT_WRITE_HIGH(this->_segments_base[s], this->_segments_mask[s]);
						#endif
						// //
					}
					else
					{
						// Выключаем нужный сегмент //
						#if defined(SEVENSEGMENT_INVERT_SEGMENTS)
						DIRECT_WRITE_HIGH(this->_segments_base[s], this->_segments_mask[s]);
						#else
						DIRECT_WRITE_LOW(this->_segments_base[s], this->_segments_mask[s]);
						#endif
						// //
					}
				}
				#endif
				
				// Включаем нужный разряд //
				#if defined(SEVENSEGMENT_INVERT_DIGITS)
				DIRECT_WRITE_LOW(this->_digits_base[this->_drawingDataIndex], this->_digits_mask[this->_drawingDataIndex]);
				#else
				DIRECT_WRITE_HIGH(this->_digits_base[this->_drawingDataIndex], this->_digits_mask[this->_drawingDataIndex]);
				#endif
				// //
			}
			
			return;
		}
		
		void Dimming()
		{
			// Выключаем все сегменты на порту (Что-бы не было засветки) //
			#if defined(SEVENSEGMENT_USE_PORT)
			#if defined(SEVENSEGMENT_INVERT_SEGMENTS)
			*(this->_port) = 0b11111111;
			#else
			*(this->_port) = 0b00000000;
			#endif
			#endif
			// //
			
			// Выключаем текущий разряд //
			#if defined(SEVENSEGMENT_INVERT_DIGITS)
			DIRECT_WRITE_HIGH(this->_digits_base[this->_drawingDataIndex], this->_digits_mask[this->_drawingDataIndex]);
			#else
			DIRECT_WRITE_LOW(this->_digits_base[this->_drawingDataIndex], this->_digits_mask[this->_drawingDataIndex]);
			#endif
			// //
			
			return;
		}
	
	private:
	
		volatile uint8_t *_digits_base[_digits];
		uint8_t _digits_mask[_digits];
		#if defined(SEVENSEGMENT_USE_PORT)
		volatile uint8_t *_port;
		#else
		volatile uint8_t *_segments_base[8];
		uint8_t _segments_mask[8];
		#endif
		
		byte _drawingData[_digits];
		uint8_t _drawingDataIndex;
		bool _power;
};

#endif
