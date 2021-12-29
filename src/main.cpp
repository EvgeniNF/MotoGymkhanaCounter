#include <Arduino.h>
#include <logic.hpp>
#include <MyTimer.hpp>
namespace pin{

const int sensor{D1};
const int button_reset{D2};
const int indicator{D3};

}

Logic logic;

void IRAM_ATTR sensor_iterupt();
void IRAM_ATTR button_iterupt();

void setup() {
  // Set pins mode
  logic.init_pins(pin::sensor, pin::button_reset, pin::indicator);
  // Iinit wifi
  logic.init_server();
  
  // Init dispalay 
  logic.init_display(D8);
  
  // Set interrupt functions
  attachInterrupt(digitalPinToInterrupt(pin::sensor), sensor_iterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin::button_reset), button_iterupt, CHANGE);
}

void loop() {
  // Main work
  logic.main_work();
}

void sensor_iterupt(){
  // Sensor interupt
  logic.sensor_signal();
}

void button_iterupt(){
  // Button interupt
  logic.reset_button_signal();
}

