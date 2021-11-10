#include "MyTimer.hpp"

namespace mytimer{

MyTimer::MyTimer(){
  os_timer_disarm(&os_timer01);
  os_timer_setfn(&os_timer01, reinterpret_cast<os_timer_func_t*>(timer_func), &timer_ptr);
  os_timer_arm(&os_timer01, 500, true);
}

MyTimer::MyTimer(const int period){
  os_timer_disarm(&this->os_timer01);
  os_timer_setfn(&this->os_timer01, reinterpret_cast<os_timer_func_t*>(timer_func), &this->timer_ptr);
  os_timer_arm(&this->os_timer01, period, true);
}

void ICACHE_FLASH_ATTR MyTimer::timer_func(void *arg){
  if(arg != nullptr){
    unsigned long int* _incr = reinterpret_cast<unsigned long int*>(arg);
    (*_incr)++;
  }
}

void MyTimer::start_timer(){
  this->timer_ptr = &this->timer;
}

void MyTimer::stop_timer(){
  this->timer_ptr = nullptr;
}

void MyTimer::reset_timer(){
  this->timer = 0;
}

const unsigned long int* MyTimer::get_timer_reg() const {
  return &this->timer;
}

unsigned long int MyTimer::get_act_time() const {
  return this->timer;
}


}