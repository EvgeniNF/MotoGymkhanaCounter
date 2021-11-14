#include <Arduino.h>
#include <logic.hpp>

Logic logic;

void IRAM_ATTR sensor_iterupt();
void IRAM_ATTR button_iterupt();

void setup() {
  // Set pins mode
  pinMode(D3, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);

  // Set interrupt functions
  attachInterrupt(digitalPinToInterrupt(D3), sensor_iterupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(D2), button_iterupt, FALLING);
  
  // Init dispalay 
  logic.init(D8);
}

void loop() {
  // Main work
  logic.main_work();
}

void sensor_iterupt(){
  logic.sensor_signal();
}

void button_iterupt(){
  logic.reset_signal();
}

