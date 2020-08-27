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
	for(uint8_t i = 0; i < 0x1f; i++)
	{
		if (_i2c->IsDeviceConnected(i))
		{
			printf("Device present at %d\n", i);
		}
		Dwt::DelayMilliseconds(10);
	}

}

IoExtender::~IoExtender()
{
}

}; // namespace Hal
