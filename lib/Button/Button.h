#pragma once

#include <Arduino.h>

namespace button
{

class Button
{
  public:
    /**
     * @brief Construct a new Button object
     * @param pinNo
     */
    explicit Button(const int pinNo, unsigned int debounceTimeMs,
                    unsigned int holdTimeMs, int inputMode, bool interruptMode);
    /**
     * @brief Destroy the Button object
     */
    ~Button() = default;
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool isClicked();
    /**
     * @brief
     *
     * @return true
     * @return false
     */
    bool isHolded();
    /**
     * @brief
     */
    bool getStateButton();

  private:
    /**
     * @brief
     */
    void IRAM_ATTR interruptSignal();
    //
    bool m_intteruptMode{false};
    //
    unsigned int m_debounceTimeMs{0};
    //
    unsigned int m_holdTimeMs{0};
    //
    int m_pinNo{0};
    //
    bool m_stateButton{false};
    //
    unsigned long int m_checkTimeClick;
    //
    unsigned long int m_checkTimeHold;
    // 
    bool m_clickedFlag{false};
    //
    bool m_holdedFlag{false};
};

} // namespace button