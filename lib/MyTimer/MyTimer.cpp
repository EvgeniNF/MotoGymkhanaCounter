#include "MyTimer.hpp"
#include <functional>

namespace mytimer{

MyTimer::MyTimer(const int period){
  this->timer_data = std::make_shared<TimerData>();
  this->start_timer(period);
}

MyTimer::MyTimer(){
  this->timer_data = std::make_shared<TimerData>();
}


void ICACHE_FLASH_ATTR MyTimer::timer_func(void *arg){
  auto* timer_data = reinterpret_cast<TimerData*>(arg);
  timer_data->time = millis() - timer_data->start_time;
}

void MyTimer::start_timer(const int period){
  os_timer_disarm(&this->os_timer01);

  os_timer_setfn(&this->os_timer01, reinterpret_cast<os_timer_func_t*>(timer_func), reinterpret_cast<void*>(this->timer_data.get()));
  os_timer_arm(&this->os_timer01, period, true);
  this->timer_data->start_time = millis();
}

void MyTimer::stop_timer(){
  os_timer_disarm(&this->os_timer01);
}

void MyTimer::reset_timer(){
  this->timer_data->time = 0;
}

const unsigned long int* MyTimer::get_timer_ptr() const {
  return &this->timer_data->time;
}

unsigned long int MyTimer::get_act_time() const {
  return this->timer_data->time;
}


}