#include <Arduino.h>
#include <Button.h>
//#include <logic.hpp>
//#include <Timer.h>
namespace pin{

const int sensor{D1};
const int button_reset{D2};
const int indicator{D3};

}
//
//Logic logic;
//
//void IRAM_ATTR sensor_iterupt();
//void IRAM_ATTR button_iterupt();
button::Button resetButton(pin::button_reset, 100, 1500, INPUT_PULLUP, true);

void setup() {
  Serial.begin(11500);
//  // Set pins mode
//  logic.init_pins(pin::sensor, pin::button_reset, pin::indicator);
//  // Iinit wifi
//  logic.init_server();
//  
//  // Init dispalay 
//  logic.init_display(D8);
//  
//  // Set interrupt functions
//  attachInterrupt(digitalPinToInterrupt(pin::sensor), sensor_iterupt, FALLING);
//  attachInterrupt(digitalPinToInterrupt(pin::button_reset), button_iterupt, CHANGE);
}

void loop() {
  if (resetButton.isClicked()) {
    Serial.println("Clicked");
  } else if (resetButton.isHolded()) {
    Serial.println("Holded");
  }
  // Main work
//  logic.main_work();
}

//void sensor_iterupt(){
//  // Sensor interupt
//  logic.sensor_signal();
//}
//
//void button_iterupt(){
//  // Button interupt
//  logic.reset_button_signal();
//}

