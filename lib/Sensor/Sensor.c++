#include "Sensor.h"

#include <FunctionalInterrupt.h>

namespace sensor
{
Sensor::Sensor(int pinNo, int inputMode, int interruptMode,
               unsigned int filterTime)
    : m_filterTime(filterTime)
{
    // Set pin mode
    pinMode(pinNo, inputMode);
    // Set interrupt
    int interruptPin = digitalPinToInterrupt(pinNo);
    if (interruptPin != NOT_AN_INTERRUPT)
    {
        // Binding interrupt function
        attachInterrupt(interruptPin, std::bind(&Sensor::interruptHeandler, this),
                        interruptMode);
    }
}
void Sensor::setCallbackFunction(const std::function<void()>& function)
{
    this->m_callbackFunction = function;
}
void Sensor::interruptHeandler()
{
    unsigned long int actualTime = millis();
    if (actualTime > this->m_nextTime)
    {
        this->m_callbackFunction();
        this->m_nextTime = actualTime + m_filterTime;
    }
}

} // namespace sensor