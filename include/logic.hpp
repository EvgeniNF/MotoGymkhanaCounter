#ifndef INCLUDE_LOGIC_HPP
#define INCLUDE_LOGIC_HPP

#include <Arduino.h>
#include <MyTimer.hpp>
#include <max7219_spi.hpp>
#include <functional>

class Logic : private mytimer::MyTimer, private max7219_spi::MAX7219_SPI{
 
 public:

  Logic() = default;

  ~Logic() = default;

  void init(int cs_disp_pin, unsigned short int _update_period = 142);
  
  void main_work();

  void sensor_signal();
  
  void reset_signal();

 private:

  void update_display();

  void set_digit_on_disp(int offset, int num_dig, int value);

 private:
  
  enum class State{
    NOT_INIT, // Don't call init function
    READY,    // Ready for start count
    COUNT,    // Work count
    STOPED    // Stoped timer
  };

 private:

  unsigned short int update_period = 1;

  int status[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  unsigned long int prev_time = 0;

  const unsigned long time_convers[4] = {3'600'000, 60'000, 1'000, 1};

  State st_timer = State::NOT_INIT;
  
};

#endif // INCLUDE_LOGIC_HPP