#ifndef LIB_MYTIMER_MYTIMER_HPP
#define LIB_MYTIMER_MYTIMER_HPP

#include <Arduino.h>
#include <gpio.h>
#include <ets_sys.h>
#include <user_interface.h>

namespace mytimer{

class MyTimer{
 
 public:
  /**
   * @brief Construct a new Timer object
   * 
   */
  MyTimer() = default;
  /**
   * @brief 
   * 
   */
  explicit MyTimer(const int period);
  /**
   * @brief Destroy the My Timer object
   * 
   */
  virtual ~MyTimer() = default;
  /**
   * @brief 
   * 
   * @param arg 
   */
  static void ICACHE_FLASH_ATTR timer_func(void *arg);
  /**
   * @brief 
   * 
   */
  void start_timer(const int period);
  /**
   * @brief 
   * 
   */
  void stop_timer();
  /**
   * @brief 
   * 
   */
  void reset_timer();
  /**
   * @brief Get the timer reg object
   * 
   * @return unsigned long* 
   */
  const unsigned long int* get_timer_ptr() const;
  /**
   * @brief Get the act time object
   * 
   * @return unsigned long int 
   */
  unsigned long int get_act_time() const;

 private:
  //
  os_timer_t os_timer01 {};
  //
  unsigned long int timer = 0;
};

}

#endif // LIB_MYTIMER_MYTIMER_HPP