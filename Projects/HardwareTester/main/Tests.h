
#ifndef TESTS_H_
#define TESTS_H_

#include <cstring>
#include <string>
#include "stdio.h"
#include <cstdint>
#include <cstdarg>
#include "stdio.h"
#include "wchar.h"
#include "Hardware.h"

using Hal::Timer;
using Hal::Hardware;

void SoftwareResetTest();
void PutCpuToSleep();
void TestSpiffs();
void WifiMenu();
void LedMenu();
char ReadKey();
void ReadString(char * string, uint8_t size);
void CameraMenu();
void TestTimer();
void TestI2sClock();
void TestSdCard();

const char *GetTestPhrase();

class TestClass : Timer::Callback
{
public:
    TestClass()
    {
        // Hal::Hardware::Instance()->GetTimer0().AddCallback(this);
        Hardware::Instance()->GetGpio().ConfigOutput(Hal::Gpio::GpioIndex::Gpio26,
            Hal::Gpio::OutputType::PullUp);
    }
    void TimerCallback() override
    {
       Hardware::Instance()->GetGpio().Set(Hal::Gpio::GpioIndex::Gpio26);
       Hal::Dwt::DelayMilliseconds(40);
       Hardware::Instance()->GetGpio().Reset(Hal::Gpio::GpioIndex::Gpio26);
    }
};

#endif /* TESTS_H_ */
