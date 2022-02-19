#pragma once

#include <Arduino.h>

namespace button
{

class Button
{
  public:
    /**
     * @brief Construct a new Button object
     * @param pinNo Pin number
     * @param debounceTimeMs Debounce time
     * @param holdTimeMs Hold time
     * @param inputMode Input mode INPUT or INPUT_PULLUP
     * @param buttonType Norm close or norm open contact, NC -> true, NO -> false 
     * @param interruptMode Interrupt mode true -> On, false -> Off
     */
    explicit Button(int pinNo, unsigned int debounceTimeMs,
                    unsigned int holdTimeMs, int inputMode, bool buttonType, bool interruptMode);
    /**
     * @brief Destroy the Button object
     */
    ~Button() = default;
    /**
     * @brief Check click button
     * @return true if clicked
     * @return false if not clicked
     */
    bool isClicked();
    /**
     * @brief Check hold button
     * @return true if holded
     * @return false if not holded
     */
    bool isHolded();
    /**
     * @brief Get the State Button
     * @return true if push button
     * @return false if not push button
     */
    bool getStateButton();
    /**
     * @brief Loop heandler button
     */
    void buttonNoInterruptHeandler();

  private:
    // Number of pin
    int m_pinNo;
    // Debounce time
    unsigned int m_debounceTimeMs;
    // Hold time
    unsigned int m_holdTimeMs;
    // Intterupt mode false - Off, true - On
    bool m_intteruptMode;
    /**
     * @brief State button enum
     */
    enum class BUTTON_STATE
    {
      WAIT_CLICK,
      IN_DEBOUNCE_TIME,
      IN_CLICK_TIME,
      IN_HOLD_TIME
    };
    // Button state var
    Button::BUTTON_STATE m_buttonState{Button::BUTTON_STATE::WAIT_CLICK};
    /**
     * @brief Button type enum
     */
    enum class BUTTON_TYPE
    {
      NO_PULLUP,
      NC_PULLUP,
      NO,
      NC
    };
    // Button type var
    Button::BUTTON_TYPE m_buttonType{Button::BUTTON_TYPE::NC};
    /**
     * @brief Heandler of interrupts
     */
    void IRAM_ATTR interruptSignal();
    // Time for check click
    unsigned long int m_checkTimeClick{0};
    // Time for check hold
    unsigned long int m_checkTimeHold{0};
    // Out filter time
    unsigned long int m_waitDebounceTime{0};
};

} // namespace button