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

} // namespace configs