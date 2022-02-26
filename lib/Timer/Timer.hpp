#pragma once

#include <Arduino.h>

#include <ets_sys.h>
#include <gpio.h>
#include <user_interface.h>

#include <memory>

namespace timer
{

struct TimerData
{
    unsigned long int time{0};
    unsigned long int startTime{0};
};

class Timer
{

  public:
    /**
     * @brief Construct a new Timer object
     */
    Timer();
    /**
     * @brief Construct a new Timer object and starts the timer
     * @param period Tick time in millis
     */
    explicit Timer(const int period);
    /**
     * @brief Destroy the My Timer object
     */
    virtual ~Timer() = default;

  public:
    /**
     * @brief The function starts the timer
     *
     * @param period Tick time in millis
     */
    void startTimer(const int period);
    /**
     * @brief The function stops the timer
     */
    void stopTimer();
    /**
     * @brief The function reset the timer
     */
    void resetTimer();
    /**
     * @brief Get the timer pointer
     * @return unsigned long* timer pointer
     */
    const unsigned long int* getTimerPtr() const;
    /**
     * @brief Getting the current time of the timer
     * @return unsigned long int time
     */
    unsigned long int getActTime() const;

  private:
    /**
     * @brief Interrupt function for timer
     * @param arg Pointer for increment
     */
    static void ICACHE_FLASH_ATTR tickInterrupt(void* arg);
    // System timer
    os_timer_t m_osTimer{};
    // Timer register
    std::shared_ptr<TimerData> m_timerData;
};

} // namespace timer
