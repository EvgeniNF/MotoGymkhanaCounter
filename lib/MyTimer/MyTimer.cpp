#include "MyTimer.hpp"

namespace mytimer{

MyTimer::MyTimer(const int period){
  this->start_timer(period);
}

void ICACHE_FLASH_ATTR MyTimer::timer_func(void *arg){
  unsigned long int* _incr = reinterpret_cast<unsigned long int*>(arg);
  *_incr = (*_incr) + 1;
}

void MyTimer::start_timer(const int period){
  os_timer_disarm(&this->os_timer01);
  os_timer_setfn(&this->os_timer01, reinterpret_cast<os_timer_func_t*>(timer_func), &this->timer);
  os_timer_arm(&this->os_timer01, period, true);
}

void MyTimer::stop_timer(){
  os_timer_disarm(&this->os_timer01);
}

void MyTimer::reset_timer(){
  this->timer = 0;
}

const unsigned long int* MyTimer::get_timer_ptr() const {
  return &this->timer;
}

unsigned long int MyTimer::get_act_time() const {
  return this->timer;
}


}