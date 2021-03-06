# SevenSegment
Библиотека реализует высокопроизводительный драйвер управления семисегментным индикатором.

Класс реализует два конструктора, позволяя указывать в качестве пинов сегментов или массив или ссылку на порт. Второй вариант работает значительно быстрее, но не всегда возможен.
По умолчанию используется конструктор с массивом сегментов. Что-бы переопределить используется константа `SEVENSEGMENT_USE_PORT`, объявленная перед подключением класса.
Порядок сегментов и порядок пинов порта совпадают, т.е. пин Px0 = сегмент А, пин Px1 = сегмент B, ..., пин Px7 = сегмент DP.

Логика выходных сигналов по умолчанию прямая, т.е. для включения подаётся '1', для выключения '0'. Данное поведение можно изменить с помощью констант, объявленные перед подключением класса.
* `SEVENSEGMENT_INVERT_DIGITS` - Инверсия сигнала разрядов индикатора.
* `SEVENSEGMENT_INVERT_SEGMENTS` - Инверсия сигнала сегментов индикатора.

В целях оптимизации и экономии ресурсов, количество сегментов указывается через константу `SEVENSEGMENT_DIGITS`.

Например:
```c
#define SEVENSEGMENT_INVERT_SEGMENTS
#define SEVENSEGMENT_INVERT_DIGITS
#define SEVENSEGMENT_USE_PORT
#define SEVENSEGMENT_DIGITS	4
#include <SevenSegment.h>
...
```



### Методы класса

| Метод | Описание | Параметры |
| --- | :---: | --- |
| `SevenSegmentCore(const uint8_t *pinDigits, volatile uint8_t *port)` | Конструктор класса | `pinDigits` - Массив пинов разрядов<br>`port` - Ссылка на порт сегментов |
| `SevenSegmentCore(const uint8_t *pinDigits, const uint8_t *pinSegments)` | Конструктор класса | `pinDigits` - Массив пинов разрядов<br>`pinSegments` - Массив пинов сегментов |
| `void Begin()` | Инициализировать класс и таймер | - |
| `void SetBrightness(uint8_t brightness)` | Установить яркость индикатором | `brightness` - Яркость |
| `void SetNums(int32_t number)` | Установка числа (не реализовано) | `number` - Число |
| `void SetNums(float number)` | Установка числа (не реализовано) | `number` - Число |
| `void SetPower(bool status)` | Управление питанием индикатора | `status` - Включить или выключить индикатор |
| `void SetChr(uint8_t position, char character)` | Установить символ в указанную позицию | `position` - Позиция<br>`character` - Символ |
| `void SetNum(uint8_t position, uint8_t number)` | Установить цифру в указанную позицию | `position` - Позиция<br>`number` - Цифра |
| `void SetDot(uint8_t position, bool dot)` | Установить\Снять точку в указанную позицию | `position` - Позиция<br>`dot` - Точка |
| `void SetRAW(uint8_t position, byte data)` | Установить RAW байт в указанную позицию | `position` - Позиция<br>`data` - Байт |
| `void Clear()` | Очистить индикатор | - |
| `void Lighting()` | Метод включения следующего разряда | - |
| `void Dimming() | Метод гашения текущего разряда | - |



### Заключение
