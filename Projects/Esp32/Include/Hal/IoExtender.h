
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
	IoExtender(Gpio *IoPins, I2c *i2c, uint8_t deviceAddress);
	~IoExtender();
private:
	Gpio *_gpio;
	I2c *_i2c;
	uint8_t _address;
	bool _deviceOnline;
};

} // namespace Hal

#endif /* INCLUDE_HAL_IO_EXTENDER_H_ */
