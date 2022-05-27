#pragma once

#include <Arduino.h>
#include <Timer.hpp>
#include <functional>


namespace sensor
{

using FilterTimer = timer::TickTimer;

class Sensor
{
  public:
    /**
     * @brief Construct a new Sensor object
     * @param pinNo  Number of sensor pin
     * @param inputMode Sensor pin mode
     * @param interruptMode Interrupt mode
     * @param filterTime Filter time
     */
    explicit Sensor(int pinNo, int inputMode, int interruptMode,
                    unsigned int filterTime);
    /**
     * @brief Destroy the Sensor object
     */
    ~Sensor() = default;

  public:
    /**
     * @brief Set the Callback Function object
     * @param function New callback function
     */
    void setCallbackFunction(const std::function<void()>& function);

    void attach() noexcept;

  private:
    /**
     * @brief Sensor interrupt heandler
     */
    void IRAM_ATTR interruptHeandler();

  private:
    /// Interrupt input pin
    int m_interruptPin;
    /// Interrupt mode
    int m_interruotMode;
    /// CallbackFunction
    std::function<void()> m_callbackFunction;
    /// Timer
    FilterTimer m_filterTimer;
};

} // namespace sensor
