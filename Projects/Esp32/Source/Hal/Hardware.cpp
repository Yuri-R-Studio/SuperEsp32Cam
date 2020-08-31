
#include "stdio.h"
#include "wchar.h"
#include "Hardware.h"
#include "soc/rtc.h"

#ifndef HARDWARE_TESTER
#include "RTOSExtra.h"
#endif

namespace Hal
{

Hardware *Hardware::_pHardware;

Hardware::Hardware() :	_gpio(),
						_adc(&_gpio),
						_debugPort(&_gpio, UartPort::Uart0, 115200, Gpio::GpioIndex::Gpio3, Gpio::GpioIndex::Gpio1),
						_spiffs(),
						_camera(&_gpio),
						_rng(),
						_wifiDriver(),
						_flash(),
						_bankConfig(),
						_spi(),
						_timerInterruptHandler(),
						_timer0(&_timerInterruptHandler, TimerSelect::Timer0),
						_timer1(&_timerInterruptHandler, TimerSelect::Timer1),
						_dac(&_gpio, Gpio::GpioIndex::Gpio25),
						_sdCard(&_gpio, Gpio::GpioIndex::Gpio2, Gpio::GpioIndex::Gpio15, Gpio::GpioIndex::Gpio14, Gpio::GpioIndex::Gpio13),
						_rmt(&_gpio, Gpio::GpioIndex::Gpio33, RmtChannel::RmtChannel0),
						_leds(&_gpio, &_timer0, &_rmt),
						_i2c(&_gpio, Hal::I2cPort::I2c0, Gpio::GpioIndex::Gpio4, Gpio::GpioIndex::Gpio27),
						_ioExtender(&_gpio, &_i2c, Gpio::GpioIndex::Gpio32, 0x18)
{
	esp_chip_info(&_mcuInfo);
	esp_base_mac_addr_get(_macAdrress.data());
	printf("SDK Version         		: %s\n", (char *)esp_get_idf_version());
	printf("CPU Cores           		: %d\n", _mcuInfo.cores);
	printf("APB Clock           		: %d Hz\n", GetSystemClockBase());
	printf("CPU Revision        		: %d\n", _mcuInfo.revision);
	printf("Embedded Flash      		: %s\n", (_mcuInfo.features & CHIP_FEATURE_EMB_FLASH) ? "YES" : "NO");
	printf("Wi-Fi Modle         		: %s\n", (_mcuInfo.features & CHIP_FEATURE_WIFI_BGN) ? "YES" : "NO");
	printf("Bluetooth Classic   		: %s\n", (_mcuInfo.features & CHIP_FEATURE_BT) ? "YES" : "NO");
	printf("Bluetooth LE        		: %s\n", (_mcuInfo.features & CHIP_FEATURE_BLE) ? "YES" : "NO");
	printf("MAC Address         		: %02X:%02X:%02X:%02X\n",
		   _macAdrress[0],
		   _macAdrress[1],
		   _macAdrress[2],
		   _macAdrress[3]);
	printf("MCU Free Heap       		: %d\n", esp_get_free_heap_size());
#ifndef HARDWARE_TESTER
	printf("MCU Project Heap Allocated	: %d\n", configTOTAL_PROJECT_HEAP_SIZE_ALLOCATED);
#endif
	printf("Firmware Image Size	        : %d of %d\n", _bankConfig.GetCurrentBank().ImageSize, _bankConfig.GetCurrentBank().PartitionSize);
	printf("Reset Reason        		: %s\n", GetResetReasonAsString(GetResetReason()));
	printf("Running On	                : %s\n", (_bankConfig.GetCurrentBank().BankRunning == Bank::Bank1) ?
												"Bank 1" : "Bank 2");
	printf("Bank Info	                : %s %s Version: %s\n",	_bankConfig.GetCurrentBank().Date.data(),
									_bankConfig.GetCurrentBank().Time.data(),
									_bankConfig.GetCurrentBank().Version.data());
	printf("\n");

	if (_pHardware == nullptr)
		_pHardware = this;
	else
		printf("!!! Error: Only one instance of System can be created !!!\n");

#ifdef HARDWARE_TESTER
	_i2c.ScanDevices();
	_ioExtender.Refresh(true);
#endif
	// _spiffs.Mount();
	// _sdCard.Mount();
	// i2s_write;
	//_timer0.Initlialize();
	// _timer0.AddCallback(this);
	// _timer0.Start();
}

uint32_t Hardware::GetSystemClockBase()
{
	return rtc_clk_apb_freq_get();
}

void Hardware::SoftwareReset()
{
	esp_restart();
}

void Hardware::DeepSleep(uint32_t uSeconds)
{
	esp_sleep_enable_timer_wakeup(uSeconds);
	vTaskDelay(20);
	esp_deep_sleep_start();
}

ResetReason Hardware::GetResetReason()
{
	esp_reset_reason_t info = esp_reset_reason();
	return static_cast<ResetReason>(info);
}

char *Hardware::GetResetReasonAsString(ResetReason reason)
{
	switch (reason)
	{
	case ResetReason::Unknown:
		return (char *)"Unknown";
	case ResetReason::PowerOn:
		return (char *)"Power On";
	case ResetReason::ExternalReset:
		return (char *)"ExternalReset";
	case ResetReason::SoftwareReset:
		return (char *)"Software Reset";
	case ResetReason::ExceptionPanicReset:
		return (char *)"Exception/Panic Reset";
	case ResetReason::IntWatchDog:
		return (char *)"Internal External WatchDog";
	case ResetReason::TaskWatchDog:
		return (char *)"Task WatchDog";
	case ResetReason::WatchDog:
		return (char *)"Other WatchDog";
	case ResetReason::DeepSleep:
		return (char *)"Deep Sleep";
	case ResetReason::Brownout:
		return (char *)"Brownout";
	case ResetReason::Sdio:
		return (char *)"Sdio";
	default:
		return (char *)"";
	}
}

uint32_t Hardware::GetHeapSize()
{
	multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    return info.total_free_bytes + info.total_allocated_bytes;
}

uint32_t Hardware::GetMinFreeHeap(void)
{
    return heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
}

uint32_t Hardware::GetMaxAllocHeap(void)
{
    return heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
}

uint32_t Hardware::GetRandomNumber()
{
	return esp_random();
}

uint32_t Hardware::GetPsramSize(void)
{
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    return info.total_free_bytes + info.total_allocated_bytes;
}

uint32_t Hardware::GetFreePsram(void)
{
    return heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
}

uint32_t Hardware::GetMinFreePsram(void)
{
    return heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM);
}

uint32_t Hardware::GetMaxAllocPsram(void)
{
    return heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
}

Hardware::~Hardware()
{
}

uint32_t Hardware::Milliseconds()
{
	return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

} // namespace Hal
