
#ifndef INCLUDE_HAL_LEDS_H_
#define INCLUDE_HAL_LEDS_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "Timer.h"
#include "Rmt.h"

namespace Hal
{

class Leds : public Timer::Callback
{

public:
	enum class LedIndex : uint8_t
	{
		Blue
	};

	Leds(Gpio *IoPins, Timer* timer, Rmt* rmt);
	~Leds();

	bool SetLedColour(uint16_t ledIndex, LedColor colour);
	void SetAllLeds(LedsArray &array);
	void ResetAllLeds();
	void Refresh();

private:

	static constexpr uint16_t MaxLeds = MaxAddressableLeds;
	// How many bytes per color each led needs
	// Color ex.:            0x00									0xff
	//			   0b10010010 0b01001001 0b00100100 | 0b11011011 0b01101101 0b10110110
	//				 |0||0||  0 ||0||0||  0 ||0||0|	|   |1||1||  1 ||1||1||  1 ||1||1|
	static constexpr uint16_t BytesPerColor = 3;
	// How many colos each leds has
	static constexpr uint8_t LedColors = 3;

	void TimerCallback() override;

	Gpio *_gpio;
	Timer *_timer;
	Rmt *_rmt;
	static constexpr LedColor::Colors BlankColor = {0, 255, 0};
	
	LedsArray _outputLeds = { };
};
} // namespace Hal

#endif /* INCLUDE_HAL_LEDS_H_ */
