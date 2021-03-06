#include "Timer.hpp"

#include <functional>

namespace timer
{

Timer::Timer(const int period)
{
    m_timerData = std::make_shared<TimerData>();
    startTimer(period);
}

Timer::Timer() { m_timerData = std::make_shared<TimerData>(); }

void ICACHE_FLASH_ATTR Timer::tickInterrupt(void* arg)
{
    // Calculate actual time
    auto* timerData = reinterpret_cast<TimerData*>(arg);
    timerData->time = millis() - timerData->startTime;
}

void Timer::startTimer(const int period)
{
    os_timer_disarm(&m_osTimer);
    // Set callback function
    os_timer_setfn(&m_osTimer,
                   reinterpret_cast<os_timer_func_t*>(Timer::tickInterrupt),
                   reinterpret_cast<void*>(m_timerData.get()));
    os_timer_arm(&this->m_osTimer, period, true);
    m_timerData->startTime = millis();
}

void Timer::stopTimer() { os_timer_disarm(&m_osTimer); }

void Timer::resetTimer()
{
    m_timerData->time = 0;
    m_timerData->startTime = millis();
}

const unsigned long int* Timer::getTimerPtr() const
{
    return &m_timerData->time;
}

unsigned long int Timer::getActTime() const { return m_timerData->time; }

TickTimer::TickTimer(uint16 tickTime, std::function<void()> const& callbackFunction) :
m_callback(callbackFunction), m_tickTime(tickTime) 
{}

void TickTimer::startOnece() noexcept 
{
    os_timer_disarm(&m_timer);
    os_timer_setfn(&m_timer,
                   reinterpret_cast<os_timer_func_t*>(TickTimer::tickInterrupt),
                   reinterpret_cast<void*>(&m_callback));
    os_timer_arm(&m_timer, m_tickTime, false);
}

void ICACHE_FLASH_ATTR TickTimer::tickInterrupt(void* arg)
{
    auto* callback = reinterpret_cast<std::function<void()>*>(arg);
    (*callback)();
}



} // namespace timer
