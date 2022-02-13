#include "Timer.h"

#include <functional>

namespace timer{

Timer::Timer(const int period) {
  this->m_timerData = std::make_shared<TimerData>();
  this->startTimer(period);
}
Timer::Timer() {
  this->m_timerData = std::make_shared<TimerData>();
}
void ICACHE_FLASH_ATTR Timer::tickInterrupt(void *arg) {
  // Calculate actual time
  auto* timerData = reinterpret_cast<TimerData*>(arg);
  timerData->m_time = millis() - timerData->m_startTime;
}
void Timer::startTimer(const int period) {
  os_timer_disarm(&this->m_osTimer);
  // Set callback function
  os_timer_setfn(&this->m_osTimer,
                 reinterpret_cast<os_timer_func_t*>(Timer::tickInterrupt),
                 reinterpret_cast<void*>(this->m_timerData.get()));
  os_timer_arm(&this->m_osTimer, period, true);
  this->m_timerData->m_startTime = millis();
}
void Timer::stopTimer(){
  os_timer_disarm(&this->m_osTimer);
}
void Timer::resetTimer(){
  this->m_timerData->m_time = 0;
  this->m_timerData->m_startTime = millis();
}
const unsigned long int* Timer::getTimerPtr() const {
  return &this->m_timerData->m_time;
}
unsigned long int Timer::getActTime() const {
  return this->m_timerData->m_time;
}

}