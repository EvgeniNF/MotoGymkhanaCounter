#include <Arduino.h>
#include <max7219_spi.hpp>
#include <MyTimer.hpp>

max7219_spi::MAX7219_SPI disp(D8);
mytimer::MyTimer t01(1);

void setup() {
  disp.shutdown(false);
  /* and clear the display */
  disp.clear_display();
}

void for_each_digit(int input){
  if (input == 0) { return; };    
  int current = input % 10;
  for_each_digit(input / 10); // recurse *first*, then process

}

void loop() { 
  disp.clear_display();
  for(int i = 0; i < 8; i++){
    disp.set_digit(i, 0);
  }
}

