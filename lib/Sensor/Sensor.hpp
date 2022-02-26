#pragma once

#include <Arduino.h>

#include <functional>

namespace sensor
{

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

  private:
    /**
     * @brief Sensor interrupt heandler
     */
    void IRAM_ATTR interruptHeandler();
    /// Filter time
    unsigned int m_filterTime;
    /// Next time work callback function
    unsigned long int m_nextTime;
    /// CallbackFunction
    std::function<void()> m_callbackFunction;
};

} // namespace sensor
