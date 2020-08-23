
#ifndef INCLUDE_HAL_IO_EXTENDER_H_
#define INCLUDE_HAL_IO_EXTENDER_H_

#include "HalCommon.h"
#include "Gpio.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

namespace Hal
{

class IoExtender
{

public:
	IoExtender(Gpio *IoPins);
	~IoExtender();
private:
	Gpio *_gpio;
	
};

} // namespace Hal

#endif /* INCLUDE_HAL_IO_EXTENDER_H_ */
