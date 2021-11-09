#include <Arduino.h>
#include <gpio.h>
#include <ets_sys.h>
#include <user_interface.h>


class MyTimer{
 
 public:
  /**
   * @brief Construct a new My Timer object
   * 
   */
  MyTimer() {
    Serial.begin(115200);
    Serial.print("Constructor!");
    Serial.end();
    pinMode(D6, OUTPUT);
    os_timer_disarm(&os_timer01);
    os_timer_setfn(&os_timer01, reinterpret_cast<os_timer_func_t*>(timer_func_user), &led_state);
    os_timer_arm(&os_timer01, 500, true);
  }
  /**
   * @brief Destroy the My Timer object
   * 
   */
  ~MyTimer() = default;
  /**
   * @brief 
   * 
   * @param arg 
   */
  static void ICACHE_FLASH_ATTR timer_func_user(void *arg){
    if(arg != nullptr){
      bool* _state = reinterpret_cast<bool*>(arg);
      *_state = (*_state == 0) ? 1 : 0;
      digitalWrite(D6, *_state);
    }
  }
 private:
  os_timer_t os_timer01 {};
  bool led_state = false;

};

//MyTimer timer_0;

void setup() {
  Serial.begin(115200);
  Serial.print("Hello world!");
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
}

void loop() {
  Serial.print("Debug!");
  delay(1000);
}
