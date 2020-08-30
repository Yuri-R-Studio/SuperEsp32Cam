
#ifndef INCLUDE_HAL_IO_EXTENDER_H_
#define INCLUDE_HAL_IO_EXTENDER_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "I2c.h"

namespace Hal
{

class IoExtender
{
public:
	IoExtender(Gpio *IoPins, I2c *i2c, Gpio::GpioIndex resetPin, uint8_t deviceAddress);
	~IoExtender();

	enum class IoPin : uint8_t
	{
		Gpio1,
		Gpio2,
		Gpio3,
		Gpio4,
		Gpio5,
		Gpio6,
		Gpio7,
		Gpio8
	};
	
	/// @brief Get the input/output status. 
	bool Get(IoPin gpio);
	
	/// @brief Set the output state.
	bool Set(IoPin gpio);

	/// @brief Configure the Gpio as input.
	void ConfigureInput(IoPin gpio);

	/// @brief Configure the Gpio as output.
	void ConfigureOutput(IoPin gpio);

	/// @brief Flush the input/output and the Io configuration
	void Refresh(bool updateConfig = false);

private:
	
	// I2C register addresses
	static constexpr uint8_t InputPortRegister 			= 0x00; 
	static constexpr uint8_t OutputPortRegister 		= 0x03; 
	static constexpr uint8_t PolarityInversionRegister 	= 0x03; 
	static constexpr uint8_t ConfigurationRegister 		= 0x03; 

	union IoExtenderPins
	{
		struct
		{
			uint8_t Gpio1 : 1;
			uint8_t Gpio2 : 1;
			uint8_t Gpio3 : 1;
			uint8_t Gpio4 : 1;
			uint8_t Gpio5 : 1;
			uint8_t Gpio6 : 1;
			uint8_t Gpio7 : 1;
			uint8_t Gpio8 : 1;
		}Status;
		uint8_t Value;
	};

	/// @brief Get the input/output from struct. 
	bool getIoStatus(IoPin gpio);

	/// @brief Check if the Gpio pin is configured as input.
	bool isInput(IoPin gpio);

	/// @brief Check if the Gpio pin is configured as input.
	bool getBit(IoPin gpio, IoExtenderPins &ioExtender);

	Gpio *_gpio;
	I2c *_i2c;
	uint8_t _address;
	bool _deviceOnline;
	IoExtenderPins _ioMode;
	IoExtenderPins _ioStatus;
	Gpio::GpioIndex _resetPin;
};

} // namespace Hal

#endif /* INCLUDE_HAL_IO_EXTENDER_H_ */