#pragma once

#include <Arduino.h>

namespace configs
{

namespace reset_button
{
inline const int buttonPinNo{D2};
inline const unsigned int debounceTimeMs{100};
inline const unsigned int holdTimeMs{1000};
inline const int inputMode{INPUT_PULLUP};
inline const bool buttonType{false};
inline const bool interruptMode{true};
} // namespace reset_button

namespace laser_sensor
{
inline const int sensorPinNo{D1};
inline const unsigned inputMode{INPUT_PULLUP};
inline const int interruptMode{FALLING};
inline const unsigned int filterTime{3000};
} // namespace laser_sensor

namespace indicator_led_configs
{
inline const int ledPinNo{D3};
inline const int outputMode{OUTPUT};
inline const bool invert{true};
} // namespace indicator_led_configs

namespace timer
{
inline const int updateTimePeriod{5};
}

namespace display
{
inline const int csPinNo{D8};   
inline const int updatePeriodDisplay{140}; 
}

} // namespace configs