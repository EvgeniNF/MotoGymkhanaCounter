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
  
  // Iinit wifi
  logic.init_wifi("MotoCounter",
                "19741974", 
                {192, 168, 100, 1},
                {192, 168, 100, 1},
                {255, 255, 255, 0},
                80);

  
  // Init dispalay 
  logic.init_display(D8);
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
  logic.reset_signal();
}

