#include "Button.h"

#include <FunctionalInterrupt.h>

namespace button
{

Button::Button(const int pinNo, unsigned int debounceTimeMs,
               unsigned int holdTimeMs, int inputMode, bool interruptMode)
    : m_pinNo(pinNo), m_debounceTimeMs(debounceTimeMs),
      m_holdTimeMs(holdTimeMs), m_intteruptMode(interruptMode)
{
    // Set pin mode
    pinMode(this->m_pinNo, inputMode);

    if (this->m_intteruptMode)
    {
        // Set interrupt
        int interruptPin = digitalPinToInterrupt(this->m_pinNo);
        if (interruptPin != NOT_AN_INTERRUPT)
        {
            // Binding interrupt function
            attachInterrupt(interruptPin,
                            std::bind(&Button::interruptSignal, this), CHANGE);
        }
    }
}
void Button::interruptSignal()
{
    if (!this->m_stateButton)
    {
        this->m_stateButton = true;
        unsigned long int actualTime = millis();
        this->m_checkTimeClick = actualTime + this->m_debounceTimeMs;
        this->m_checkTimeHold = actualTime + this->m_holdTimeMs;
        this->m_clickedFlag = false;
        this->m_holdedFlag = false;
    }
    else
    {
        if (this->m_clickedFlag || this->m_holdedFlag)
        {
            this->m_clickedFlag = false;
            this->m_holdedFlag = false;
        }
    }
}
bool Button::isClicked()
{
    if (this->m_stateButton && !this->m_clickedFlag)
    {
        unsigned long int actualTime = millis();
        if (actualTime > this->m_checkTimeClick)
        {
            bool actualStatePin = static_cast<bool>(digitalRead(this->m_pinNo));
            if (actualStatePin)
            {
                this->m_clickedFlag = true;
                return true;
            }
            else
            {
                this->m_clickedFlag = false;
                this->m_holdedFlag = false;
                this->m_stateButton = false;
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool Button::isHolded()
{
    if (this->m_stateButton && !this->m_holdedFlag)
    {
        unsigned long int actualTime = millis();
        if (actualTime > this->m_checkTimeHold)
        {
            bool actualStatePin = static_cast<bool>(digitalRead(this->m_pinNo));
            if (actualStatePin)
            {
                this->m_holdedFlag = true;
                return true;
            }
            else
            {
                this->m_clickedFlag = false;
                this->m_holdedFlag = false;
                this->m_stateButton = false;
                return false;
            }
        }
        else
        {
            return false;
        }
    }
}
bool Button::getStateButton()
{
    return static_cast<bool>(digitalRead(this->m_pinNo));
}

} // namespace button
