#include "IoExtender.h"
#include "DebugAssert.h"
#include "Dwt.h"
#include <stdio.h>
#include <stdlib.h>

namespace Hal
{

IoExtender::IoExtender(Gpio *IoPins, I2c *i2c, uint8_t deviceAddress) :
						_gpio(IoPins), _i2c(i2c), _address(deviceAddress), _deviceOnline(false)
{
}

IoExtender::~IoExtender()
{
}

}; // namespace Hal
