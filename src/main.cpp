#include <Arduino.h>
#include <Button.h>
#include <Sensor.h>
#include <configs.h>
//#include <logic.hpp>
//#include <Timer.h>
namespace pin
{

//const int sensor{D1};
const int button_reset{D2};
const int indicator{D3};

} // namespace pin
//
// button::Button resetButton(configs::reset_button::buttonPinNo,
// configs::reset_button::debounceTimeMs,
//                           configs::reset_button::holdTimeMs,
//                           configs::reset_button::inputMode,
//                            configs::reset_button::buttonType,
//                            configs::reset_button::interruptMode);
//
//sensor::Sensor sensorLaser(configs::laser_sensor::sensorPinNo,
//                      configs::laser_sensor::inputMode,
//                      configs::laser_sensor::interruptMode,
//                      configs::laser_sensor::filterTime);
void sayHello(){
  Serial.println("Hello");
}

void setup() { 
  Serial.begin(11500);
  
}

void loop()
{
}

//
// Logic logic;
//
// void IRAM_ATTR sensor_iterupt();
// void IRAM_ATTR button_iterupt();

//  // Set pins mode
//  logic.init_pins(pin::sensor, pin::button_reset, pin::indicator);
//  // Iinit wifi
//  logic.init_server();
//
//  // Init dispalay
//  logic.init_display(D8);
//
//  // Set interrupt functions
//  attachInterrupt(digitalPinToInterrupt(pin::sensor), sensor_iterupt,
//  FALLING); attachInterrupt(digitalPinToInterrupt(pin::button_reset),
//  button_iterupt, CHANGE);

// Main work
//  logic.main_work();

// void sensor_iterupt(){
//  // Sensor interupt
//  logic.sensor_signal();
//}
//
// void button_iterupt(){
//  // Button interupt
//  logic.reset_button_signal();
//}
