#include "Button.hpp"

#include <FunctionalInterrupt.h>

namespace button
{

Button::Button(int pinNo, unsigned int debounceTimeMs,
               unsigned int holdTimeMs, int inputMode, bool buttonType, bool interruptMode)
    : m_pinNo(pinNo), m_debounceTimeMs(debounceTimeMs),
      m_holdTimeMs(holdTimeMs), m_intteruptMode(interruptMode)
{
    // Set pin mode
    pinMode(this->m_pinNo, inputMode);
    // Set button type
    if (buttonType)
    {
        if (inputMode == INPUT_PULLUP) 
        {
            this->m_buttonType = BUTTON_TYPE::NC_PULLUP;
        } 
        else
        {
            this->m_buttonType = BUTTON_TYPE::NC;
        } 
    }
    else
    {
        if (inputMode == INPUT_PULLUP) 
        {
            this->m_buttonType = BUTTON_TYPE::NO_PULLUP;
        } 
        else
        {
            this->m_buttonType = BUTTON_TYPE::NO;
        } 
    }
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
    // Get actual type
    unsigned long int actualTime = millis();
    // Check state
    if (this->m_buttonState == BUTTON_STATE::WAIT_CLICK && actualTime > this->m_waitDebounceTime) 
    {
        // Set state debounce time
        this->m_buttonState = BUTTON_STATE::IN_DEBOUNCE_TIME;
        this->m_checkTimeClick = actualTime + this->m_debounceTimeMs;
        this->m_checkTimeHold = actualTime + this->m_holdTimeMs;
    }
}

bool Button::isClicked()
{
    // Check state
    if (this->m_buttonState == BUTTON_STATE::IN_DEBOUNCE_TIME) 
    {
        // Check time
        unsigned long int actualTime = millis();
        if (actualTime > this->m_checkTimeClick) 
        {
            // Set state
            this->m_buttonState = BUTTON_STATE::IN_CLICK_TIME;
        }
    } 
    // Check state
    if (this->m_buttonState == BUTTON_STATE::IN_CLICK_TIME)
    {
        // Check pin
        bool stateButtonPin = this->getStateButton();
        if (!stateButtonPin)
        {
            this->m_waitDebounceTime = millis() + this->m_debounceTimeMs;
            this->m_buttonState = BUTTON_STATE::WAIT_CLICK;
            return true;
        }
    }
    return false;
}

bool Button::isHolded()
{
    // Check state
    if (this->m_buttonState == BUTTON_STATE::IN_CLICK_TIME) 
    {
        // Check time
        unsigned long int actualTime = millis();
        if (actualTime > this->m_checkTimeHold) 
        {
            // Set state
            this->m_buttonState = BUTTON_STATE::IN_HOLD_TIME;
        }
    } 
    // Check state
    if (this->m_buttonState == BUTTON_STATE::IN_HOLD_TIME)
    {
        // Check pin
        bool stateButtonPin = this->getStateButton();
        if (!stateButtonPin)
        {
            this->m_waitDebounceTime = millis() + this->m_debounceTimeMs;
            this->m_buttonState = BUTTON_STATE::WAIT_CLICK;
            return true;
        }
    }
    return false;
}

bool Button::getStateButton()
{
    switch (this->m_buttonType)
    {
    case BUTTON_TYPE::NC:
        return (!static_cast<bool>(digitalRead(this->m_pinNo)));
    case BUTTON_TYPE::NO:
        return (static_cast<bool>(digitalRead(this->m_pinNo)));
    case BUTTON_TYPE::NC_PULLUP:
        return (static_cast<bool>(digitalRead(this->m_pinNo)));
    case BUTTON_TYPE::NO_PULLUP:
        return (!static_cast<bool>(digitalRead(this->m_pinNo)));
    default:
        return false;
    }
}

void Button::buttonNoInterruptHeandler()
{
    // Check state pin and button
    bool stateButtonPin = this->getStateButton();
    if (this->m_buttonState == BUTTON_STATE::WAIT_CLICK && stateButtonPin)
    {
        // Check button
        unsigned long int actualTime = millis();
        if (actualTime > this->m_waitDebounceTime) 
        {
            this->m_buttonState = BUTTON_STATE::IN_DEBOUNCE_TIME;
            this->m_checkTimeClick = actualTime + this->m_debounceTimeMs;
            this->m_checkTimeHold = actualTime + this->m_holdTimeMs;
        }
    }
}

} // namespace button
