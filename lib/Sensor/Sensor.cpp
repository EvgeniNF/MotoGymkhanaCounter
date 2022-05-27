#include "Sensor.hpp"

#include <FunctionalInterrupt.h>

namespace sensor
{

Sensor::Sensor(int pinNo, int inputMode, int interruptMode,
               unsigned int filterTime) :
    m_filterTimer(filterTime, std::bind(&Sensor::attach, this))
{
    // Set pin mode
    pinMode(pinNo, inputMode);
    // Set interrupt
    m_interruptPin = digitalPinToInterrupt(pinNo);
    attach();
}

void Sensor::setCallbackFunction(const std::function<void()>& function)
{
    m_callbackFunction = function;
}

void Sensor::attach() noexcept 
{
    if (m_interruptPin != NOT_AN_INTERRUPT) 
    {
        // Binding interrupt function
        attachInterrupt(m_interruptPin, std::bind(&Sensor::interruptHeandler, this),
                        m_interruotMode);
    }
}

void Sensor::interruptHeandler()
{
    m_callbackFunction();
    detachInterrupt(m_interruptPin);
    m_filterTimer.startOnece();
}

} // namespace sensor
