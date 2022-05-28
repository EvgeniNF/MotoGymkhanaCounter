#pragma once

#include <Arduino.h>

namespace configs
{

namespace reset_button
{
constexpr int buttonPinNo{D2};
constexpr unsigned int debounceTimeMs{400};
constexpr unsigned int holdTimeMs{1000};
constexpr int inputMode{INPUT_PULLUP};
constexpr bool buttonType{false};
constexpr bool interruptMode{true};
} // namespace reset_button

namespace laser_sensor
{
constexpr int sensorPinNo{D1};
constexpr unsigned inputMode{INPUT_PULLUP};
constexpr int interruptMode{FALLING};
constexpr unsigned int filterTime{4000};
} // namespace laser_sensor

namespace indicator_led_configs
{
constexpr int ledPinNo{D3};
constexpr int outputMode{OUTPUT};
constexpr bool invert{true};
} // namespace indicator_led_configs

namespace timer
{
constexpr int updateTimePeriod{5};
}

namespace display
{
constexpr int csPinNo{D8};
constexpr int updatePeriodDisplay{50};
} // namespace display

namespace network_settings
{
constexpr unsigned short int firstNumForWriteSettings{10};
constexpr char softApName[]{"MotoCounter_1"};
constexpr char softApPassword[]{"19741974"};
constexpr unsigned short int wifiChanel{8};
constexpr unsigned short int maxNumConnecteions{15};
constexpr unsigned short int localIp[4]{192, 168, 100, 1};
constexpr unsigned short int getway[4]{192, 168, 100, 1};
constexpr unsigned short int mask[4]{255, 255, 255, 0};

constexpr char tcpHostName[]{"tcp.server"};
constexpr unsigned int tcpPort{7050};

constexpr unsigned int dnsPort{53};

constexpr unsigned int httpPort{80};
constexpr char updateUrlName[]{"update"};
} // namespace network_settings

} // namespace configs
