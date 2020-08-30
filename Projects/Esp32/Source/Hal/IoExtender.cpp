#include "IoExtender.h"
#include "DebugAssert.h"
#include "Dwt.h"
#include <stdio.h>
#include <stdlib.h>
#include "DebugAssert.h"

namespace Hal
{

IoExtender::IoExtender(Gpio *IoPins, I2c *i2c, Gpio::GpioIndex resetPin, uint8_t deviceAddress) :
						_gpio(IoPins), _i2c(i2c), _address(deviceAddress), _deviceOnline(false),
						_resetPin(resetPin)
{
	// Set all Gpio as Input
	_ioMode.Value = 0xFF;
	_ioStatus.Value = 0;
	//if (_gpio->Get(_resetPin) == false)
	{
		_gpio->ConfigOutput(_resetPin, Gpio::OutputType::PullUp);
		_gpio->Reset(_resetPin);
		_gpio->Set(_resetPin);
	}

	if(_i2c->IsDeviceConnected(_address))
		_deviceOnline = true;
	else
		printf("IO extender not found\n");

	DebugAssertMessage(_deviceOnline, "IO Extender is not connected.");

	if (_deviceOnline)
		Refresh(true);
}

IoExtender::~IoExtender()
{
	
}

bool IoExtender::Get(IoPin gpio)
{
	if (!_deviceOnline)
		return false;

	if (isInput(gpio))
		Refresh();

	return getIoStatus(gpio);
}

bool IoExtender::Set(IoPin gpio)
{
	if (!_deviceOnline)
		return false;

	Refresh();
	return false;
}

void IoExtender::ConfigureInput(IoPin gpio)
{
	if (!_deviceOnline)
		return;
	Refresh(true);
}

void IoExtender::ConfigureOutput(IoPin gpio)
{
	if (!_deviceOnline)
		return;

	Refresh(true);
}

void IoExtender::Refresh(bool updateConfig)
{
	if (!_deviceOnline)
		return;

	if (updateConfig)
	{
		// Update configuration in the Chip
		_ioMode.Value = 5;
		uint8_t data = _ioMode.Value;
		_i2c->WriteRegister(_address, ConfigurationRegister, data);
		_i2c->ReadRegister(_address, ConfigurationRegister, &data);
		DebugAssertMessage(data == _ioMode.Value, "Byte Written %d, Byte Read %d", _ioMode.Value, data);
	}

}

bool IoExtender::isInput(IoPin gpio)
{
	if (!_deviceOnline)
		return false;

	return getBit(gpio, _ioMode);
}

bool IoExtender::getIoStatus(IoPin gpio)
{
	if (!_deviceOnline)
		return false;

	return getBit(gpio, _ioStatus);
}

bool IoExtender::getBit(IoPin gpio, IoExtenderPins &ioExtender)
{
	switch (gpio)
	{
	case IoPin::Gpio1:
		return ioExtender.Status.Gpio1;
		break;
	case IoPin::Gpio2:
		return ioExtender.Status.Gpio2;
		break;
	case IoPin::Gpio3:
		return ioExtender.Status.Gpio3;
		break;
	case IoPin::Gpio4:
		return ioExtender.Status.Gpio4;
		break;
	case IoPin::Gpio5:
		return ioExtender.Status.Gpio5;
		break;
	case IoPin::Gpio6:
		return ioExtender.Status.Gpio6;
		break;
	case IoPin::Gpio7:
		return ioExtender.Status.Gpio7;
		break;
	case IoPin::Gpio8:
		return ioExtender.Status.Gpio8;
		break;
	default:
		break;
	}
	return false;
}

}; // namespace Hal
