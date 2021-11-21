#ifndef INCLUDE_LOGIC_HPP
#define INCLUDE_LOGIC_HPP

#include <Arduino.h>
#include <wifi_server.hpp>
#include <MyTimer.hpp>
#include <max7219_spi.hpp>


class Logic : public mytimer::MyTimer, public max7219_spi::MAX7219_SPI, public wifi_server::Wifi_server{
 
 public:

  Logic() = default;

  ~Logic() = default;

  void init_display(int cs_disp_pin, unsigned short int _update_period = 142);
  
  void main_work();

  void sensor_signal();
  
  void reset_signal();

 private:

  void update_display();

  void set_digit_on_disp(int offset, int num_dig, int value);

 private:
  
  enum class State : unsigned short int{
    NOT_INIT = 0,   // Don't call init function
    READY = 1,      // Ready for start count
    COUNT = 2,      // Work count
    STOPED = 3      // Stoped timer
  };

 private:

  unsigned short int update_period = 1;

  int status[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  unsigned long int prev_time = 0;

  const unsigned long time_convers[4] = {3'600'000, 60'000, 1'000, 1};

  State st_timer = State::NOT_INIT;
  
};

#endif // INCLUDE_LOGIC_HPP