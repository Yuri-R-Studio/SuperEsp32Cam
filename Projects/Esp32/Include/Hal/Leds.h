
#ifndef INCLUDE_HAL_LEDS_H_
#define INCLUDE_HAL_LEDS_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "Timer.h"
#include "I2sDigital.h"

extern gpio_dev_t GPIO;
	/*
		In order to send bits through the leds,
		they need to respect the timing table below
		_____________________________________
		|	Bit		|	T On	|	T Off	|
		_____________________________________
		|	Bit 1	|	800ns	|	450ns	|
		|	Bit 0	|	400ns	|	850ns	|
		-------------------------------------

		The full pixel colour needs to receive 24 bits.
		If the next 24 bits comes withing the reset code timing, the first 24 bit
		will be passed to the next led, otherwise, the new 24 bits
		colours will overwrite the first led colour.
		_________________________________________________________________
		|				   pixel LedColor					|	Reset Code	|
		_________________________________________________________________
		| 8 bits Green	|	8 bits Red	|	8 bits Blue	|	  >=50us	|
		-----------------------------------------------------------------
	*/

namespace Hal
{

class Leds : public Timer::Callback
{

public:

	enum class LedIndex : uint8_t
	{
		Blue
	};

	Leds(Gpio *IoPins, Timer *timer, I2sDigital *i2s);
	~Leds();

	bool SetLedColour(uint16_t ledIndex, LedColor colour);
	void SetAllLeds(LedsArray &array);
	void ResetAllLeds();
	void Refresh();

private:

	/*
		As the T On and T Off are (800ns and 450) or (400 and 850),
		we can get the Least Common Multiple is equal to 50ns.

		So:

		1/50ns = 20000Hz -> 20kHz
	*/
	static constexpr uint32_t BitFrequency = 100; // 20kHz

	/*
		To send 1 Bit , we need to count how many block of 50ns has passed.
		
		_________________________________________________________________________
		|	Bit		|			T On			|				T Off			|
		_________________________________________________________________________
		|	Bit 1	| 	800ns/50ns = 16	counts	|		450ns/50ns = 9 counts	|
		|	Bit 0	|	400ns/50ns = 8 counts	|		850ns/50ns = 17 counts	|
		-------------------------------------------------------------------------

		A whole bit would take 25 counts of 50ns = 1250ns (1.25us)
	*/
	// How many leds supported
	static constexpr uint16_t MaxLeds = MaxAddressableLeds;
	// How many bytes per color each led needs
	// Color ex.:            0x00									0xff
	//			   0b10010010 0b01001001 0b00100100 | 0b11011011 0b01101101 0b10110110
	//				 |0||0||  0 ||0||0||  0 ||0||0|	|   |1||1||  1 ||1||1||  1 ||1||1|
	static constexpr uint16_t BytesPerColor = 3;
	// How many colos each leds has
	static constexpr uint8_t LedColors = 3;
	static constexpr uint32_t RefreshFrequency = 25000000;
	uint16_t _highCounter = 0;

	void TimerCallback() override;

	Gpio *_gpio;
	Timer *_timer;
	static constexpr LedColor::Colors BlankColor = {0, 255, 0};
	
	LedsArray _outputLeds = { };
	I2sDigital *_i2s;
	// Gpio::GpioIndex _ledPin = Gpio::GpioIndex::Gpio25;
	// GPIO.out1_w1ts.data = (1 << (gpio_num - 32));
	Gpio::GpioIndex _ledPin = Gpio::GpioIndex::Gpio33;
	uint32_t gpioPinIndex = (1 << ((static_cast<uint32_t>(_ledPin) - 32)));
	// GPIO.out1_w1ts.data = (1 << (gpio_num - 32));
};
} // namespace Hal

#endif /* INCLUDE_HAL_LEDS_H_ */
