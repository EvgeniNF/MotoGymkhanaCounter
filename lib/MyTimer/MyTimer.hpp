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
   * @brief Construct a new My Timer object and starts the timer
   * 
   * @param period Tick time in millis 
   */
  explicit MyTimer(const int period);
  /**
   * @brief Destroy the My Timer object
   * 
   */
  virtual ~MyTimer() = default;
  /**
   * @brief Interrupt function for timer
   * 
   * @param arg Pointer for increment
   */
  static void ICACHE_FLASH_ATTR timer_func(void *arg);
  /**
   * @brief The function starts the timer
   * 
   * @param period Tick time in millis 
   */
  void start_timer(const int period);
  /**
   * @brief The function stops the timer
   * 
   */
  void stop_timer();
  /**
   * @brief The function reset the timer
   * 
   */
  void reset_timer();
  /**
   * @brief Get the timer pointer
   * 
   * @return unsigned long* timer pointer
   */
  const unsigned long int* get_timer_ptr() const;
  /**
   * @brief getting the current time of the timer
   * 
   * @return unsigned long int time
   */
  unsigned long int get_act_time() const;

 private:
  // Timer obj
  os_timer_t os_timer01 {};
  
 protected:
  // Timer register
  unsigned long int timer = 0;
};

}

#endif // LIB_MYTIMER_MYTIMER_HPP