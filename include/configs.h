#pragma once

#include <Arduino.h>

namespace configs
{

namespace reset_button 
{
inline const int buttonPinNo{D2};   
inline const unsigned int debounceTimeMs{100};
inline const unsigned int holdTimeMs{1500};
inline const int inputMode{INPUT_PULLUP};
inline const bool buttonType{false}; 
inline const bool interruptMode{true};
}

}