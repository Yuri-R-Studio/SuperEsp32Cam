
#include "HalCommon.h"
#include "Leds.h"
#include "Dwt.h"

// #define DEBUG_LEDS

#ifdef DEBUG_LEDS
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x00800000 ? '1' : '0'), \
  (byte & 0x00400000 ? '1' : '0'), \
  (byte & 0x00200000 ? '1' : '0'), \
  (byte & 0x00100000 ? '1' : '0'), \
  (byte & 0x00080000 ? '1' : '0'), \
  (byte & 0x00040000 ? '1' : '0'), \
  (byte & 0x00020000 ? '1' : '0'), \
  (byte & 0x00010000 ? '1' : '0'), \
  (byte & 0x00008000 ? '1' : '0'), \
  (byte & 0x00004000 ? '1' : '0'), \
  (byte & 0x00002000 ? '1' : '0'), \
  (byte & 0x00001000 ? '1' : '0'), \
  (byte & 0x00000800 ? '1' : '0'), \
  (byte & 0x00000400 ? '1' : '0'), \
  (byte & 0x00000200 ? '1' : '0'), \
  (byte & 0x00000100 ? '1' : '0'), \
  (byte & 0x00000080 ? '1' : '0'), \
  (byte & 0x00000040 ? '1' : '0'), \
  (byte & 0x00000020 ? '1' : '0'), \
  (byte & 0x00000010 ? '1' : '0'), \
  (byte & 0x00000008 ? '1' : '0'), \
  (byte & 0x00000004 ? '1' : '0'), \
  (byte & 0x00000002 ? '1' : '0'), \
  (byte & 0x00000001 ? '1' : '0') 
#endif		

namespace Hal
{

Leds::Leds(Gpio *IoPins, Timer* timer, I2sDigital *i2s) : _gpio(IoPins), _timer(timer), _i2s(i2s)
{
	// Initializing all leds as output
	_gpio->ConfigOutput(_ledPin, Gpio::OutputType::PullUp);
	_timer->Initlialize();
	_timer->SetTimer(BitFrequency);
	_timer->AddCallback(this);
	// _cacheLeds[0].Green = 255;
	_i2s->Init(Hal::I2sBitSample::Sample16Bits, MaxLeds * LedColors * BytesPerColor, RefreshFrequency, true);
	// _timer->Start();
	// Gpio::GpioIndex _ledPin = Gpio::GpioIndex::Gpio33;
	// uint32_t gpioPinIndex = (1 << ((static_cast<uint32_t>(_ledPin) - 32)));
	printf("\n\nInit Led\nLed pin:%d, index:%d\n",(static_cast<uint32_t>(_ledPin)), gpioPinIndex);

	uint8_t ledBuffer [LedColors * BytesPerColor] = {};
	uint32_t ledIndexBuffer = 0;
	for (uint16_t ledIndex = 0; ledIndex < 3; ledIndex++)
	{
		_outputLeds[ledIndex] = BlankColor;
		for(uint8_t ledByte = 0; ledByte < sizeof(LedColor); ledByte++)
		{
			uint32_t bitsLong = 0;
			for(uint8_t bit = 7; bit <= 7; bit--)
			{
				bitsLong |= ((((_outputLeds[ledIndex].Led.Bytes.data()[ledByte] >> bit) & 1) << 1) | 0b100) << (bit * 3 + 8);
			}
			bitsLong = bitsLong >> 8;
#ifdef DEBUG_LEDS
			printf("ledByte %d ", ledByte);
			printf("+0x%02x ", _outputLeds[ledIndex].Led.Bytes.data()[ledByte]);
			
			printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(bitsLong));
#endif	
			ledBuffer[ledIndexBuffer++] =  (bitsLong | 0x00FF0000) >> 16;
			ledBuffer[ledIndexBuffer++] =  (bitsLong | 0x0000FF00) >> 8;
			ledBuffer[ledIndexBuffer++] =  (bitsLong | 0x000000FF);
		
			printf("Led Buffer size: %d, offset:%d\n", 3, ledIndexBuffer - 3);
			_i2s->UpdateBuffer(ledBuffer, 3, ledIndexBuffer - 3);
		}
	}
	printf("Led Buffer size: %d\n",ledIndexBuffer);
}

void Leds::TimerCallback()
{
	// Refresh();
	// sendBit1();
	// sendBit0();
	// GPIO.out1_w1ts.data = gpioPinIndex;
	// GPIO.out1_w1ts.data = gpioPinIndex;
	// _gpio->Set(_ledPin);
	// _gpio->Reset(_ledPin);
	// _gpio->Reset(_ledPin);
}

Leds::~Leds()
{
}

bool Leds::SetLedColour(uint16_t ledIndex, LedColor colour)
{
	// if (ledIndex >= _cacheLeds.size())
	// 	return false;
	
	// _cacheLeds[ledIndex] = colour;
	return true;
}

void Leds::SetAllLeds(LedsArray &array)
{
	// for(uint8_t i = 0; i < _cacheLeds.size(); i ++)
	// 	_cacheLeds[i] = array[i];
}

void Leds::ResetAllLeds()
{
	// LedColor empty = {};
	// for(uint8_t i = 0; i < _cacheLeds.size(); i ++)
	// 	_cacheLeds[i] = empty;
}

void Leds::Refresh()
{
	// uint32_t colour = (_cacheLeds[0].Green << 16)
	// 					+ (_cacheLeds[0].Red << 8)
	// 					+ (_cacheLeds[0].Blue);

}

} // namespace Hal
