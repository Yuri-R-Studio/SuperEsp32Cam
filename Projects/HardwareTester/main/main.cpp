
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <esp_attr.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "Hardware.h"
#include "Tests.h"
#include "Rng.h"
#include "esp_http_server.h"
#include "CameraStreamTest.h"
#include <cstring>

void executetMenu(char Test)
{
	switch (Test)
	{

	case 't':
	case 'T':
		LedMenu();
		break;
	case 'r':
	case 'R':
		SoftwareResetTest();
		break;
	case 'f':
	case 'F':
		PutCpuToSleep();
		break;
	case 's':
	case 'S':
		TestSpiffs();
		break;
	case 'v':
	case 'V':
		TestSdCard();
		break;
	case 'w':
	case 'W':
		WifiMenu();
		break;
	case 'c':
	case 'C':
		CameraMenu();
		break;
	case 'd':
	case 'D':
		TestLed();
		break;
	case 'i':
	case 'I':
		IoExtenderMenu();
		break;
	default:
		break;
	}

	printf("\n");
	printf("Main menu:\n");
	printf("----------\n");
	printf("[T] - Led Menu\n");
	printf("[R] - Software Reset Test\n");
	printf("[F] - Deep Sleep for 5 Seconds.\n");
	printf("[S] - Test SPIFFS\n");
	printf("[V] - Test SD Card\n");
	printf("[W] - WiFi Menu\n");
	printf("[C] - Camera Menu\n");
	printf("[D] - Test Timer [ON/OFF]\n");
	printf("[I] - IO Extender menu\n");
}

extern "C" void app_main(void)
{
	Hal::Hardware::Instance();
 	printf("Hardware Tester for ESP32\n");

	//Hal::Hardware::Instance()->GetCamera().Init();
	//startCameraServer();
	
	TestClass testClass;
	char test = 0;
	

	//TestLed();

	Hal::Hardware::Instance()->GetWifi().Disable();
	Hal::Hardware::Instance()->GetWifi().SetSsid("Alga&Paula", strlen("Alga&Paula"));
	Hal::Hardware::Instance()->GetWifi().SetPassword("minha@casa@australia", strlen("minha@casa@australia"));
	Hal::Hardware::Instance()->GetWifi().SetMode(Hal::WifiModeConfiguration::Client);
	Hal::Hardware::Instance()->GetWifi().SetAuthentication(Hal::WifiAuthenticationMode::Wpa2Psk);
	Hal::Hardware::Instance()->GetWifi().Enable();

	while (1)
	{
		executetMenu(test);
		test = ReadKey();
	}
}
