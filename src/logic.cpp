#include "../include/logic.hpp"

#include <string>

Logic::Logic() 
            : m_display()
            , m_timer()
            , m_server() {
  this->m_networkData.m_timerTime = this->m_timer.getTimerPtr();
  this->m_timerRegisterPtr = this->m_timer.getTimerPtr();
  this->m_networkData.m_timeOnEsp = new unsigned long int(millis());
//  this->m_networkData.m_espStatus = reinterpret_cast<unsigned short int*>(&this->state);
  this->m_state = STATE::NOT_INIT;
}

Logic::~Logic() {
  delete this->m_networkData.m_timeOnEsp;
}
void Logic::setZerosOnDisplay() {
  // Set zeros on the display
  this->m_display.setDigit(0, 0);
  this->m_display.setDigit(1, 0);
  this->m_display.setDigit(2, 0);
  this->m_display.setDigit(3, 0, true);
  this->m_display.setDigit(4, 0);
  this->m_display.setDigit(5, 0, true);
  this->m_display.setDigit(6, 0);
  this->m_display.setDigit(7, 0, true); 
}

void Logic::initDisplay(int csDisplayPin, 
                         unsigned short int updatePeriod){
   
  // Initialisation display function
  this->m_updatePeriod = updatePeriod;
  this->m_display.beginDisplay(D8);
  this->m_display.shutdown(false);  
  this->m_display.clearDisplay();
  this->setZerosOnDisplay();

  // Set state 
  if (this->m_state == STATE::NOT_INIT)
    this->m_state = STATE::DISPLAY_INIT;
  else if (this->m_state == STATE::WIFI_INIT)
    this->m_state = STATE::READY;
}

void Logic::initServer(){
  // Init wifi server
  this->m_server.initializationServer(&this->m_networkData,
                std::bind(&Logic::reboot, this),
                std::bind(&Logic::resetTimer, this));
  
  // Set state Display ready
  if (this->m_state == STATE::NOT_INIT)
    this->m_state = STATE::WIFI_INIT;
  else if (this->m_state == STATE::DISPLAY_INIT)
    this->m_state = STATE::READY;
}

//void Logic::mainWork(){
//  *this->m_networkData.m_timeOnEsp = millis();
//  if (this->m_state == STATE::STOPED) {
//    this->m_networkData.m_timerTime = &this->m_countResult;
//  } else {
//    this->m_networkData.m_timerTime = this->m_timer.getTimerPtr();
//  }
//  this->m_server.loopUpdateWebServer();
//  // Update period in count state
//  if (*this->m_timerRegisterPtr > this->m_time &&
//       this->state == State::COUNT)
//    this->update_display();
//  this->handle_reset_button();
//
//  bool isSensorHigh = digitalRead(this->sensor_pin);
//  if (isSensorHigh) {
//    digitalWrite(this->sensor_indicator_pin, HIGH);
//  } else {
//    digitalWrite(this->sensor_indicator_pin, LOW);
//  }
//}
//
//void Logic::sensor_signal(){
//  if (this->state == State::NOT_INIT ||
//      this->state == Logic::State::DISPLAY_INIT ||
//      this->state == Logic::State::WIFI_INIT) 
//    return;
//  if (*this->timer_register_ptr == 0 && this->state == State::READY){
//    // Start timer
//    this->update_display();
//    this->timer_.start_timer(1);
//    state = State::COUNT;
//  } else if (*this->timer_register_ptr > 3'000 && this->state == State::COUNT){
//    // Stop timer
//    this->timer_.stop_timer();
//    this->result = *this->timer_register_ptr;
//    this->update_display();
//    this->timer_.reset_timer();
//    this->timer_.start_timer(1);
//    this->state = State::STOPED;
//  } else if (*this->timer_register_ptr > 3'000 && this->state == State::STOPED){
//    // Delay after stop timer
//    this->timer_.stop_timer();
//    this->timer_.reset_timer();
//    this->update_display();
//    this->timer_.start_timer(1);
//    this->state = State::COUNT;
//  }
//}
//
//void Logic::update_display(){
//  this->prev_time = *this->timer_register_ptr;
//  int act_time = 0;
//  for (int index = 0; index < 4; index++){
//    act_time = static_cast<int>(this->prev_time / this->time_convers[index]);
//    this->prev_time = this->prev_time - (act_time * this->time_convers[index]); 
//    if (index == 0)
//      this->set_digit_on_disp(7, 1, act_time);
//    else if (index == 1)
//      this->set_digit_on_disp(5, 2, act_time);
//    else if (index == 2)
//      this->set_digit_on_disp(3, 2, act_time);
//    else
//      this->set_digit_on_disp(0, 3, act_time);         
//  }
//  this->prev_time = *this->timer_register_ptr + this->update_period;    
//}
//
//void Logic::set_digit_on_disp(int offset, 
//                              int num_dig, 
//                              int value){
//  for (int i = offset; i < num_dig + offset; i++){
//    int reg = value % 10;
//    if (reg != this->status[i]){
//      this->status[i] = reg;
//      if (i == 3 || i == 5 || i == 7){
//        this->display_.set_digit(i, reg, true);
//      } else {
//        this->display_.set_digit(i, reg);
//      }
//    }
//    value /= 10;
//  }
//}
//
//void Logic::reset_button_signal() {
//  if (!this->flag_reset_button) {
//    this->flag_reset_button = true;
//    this->prev_time_button = millis() + this->button_reset_filter_time;
//    this->target_hold_time = millis() + this->hold_reset_time;
//  }
//}
//
//void Logic::handle_reset_button(){
//  if (this->flag_reset_button) {
//    auto actual_time = millis();
//    if (actual_time >= this->prev_time_button){
//      
//      bool input_state = digitalRead(this->button_reset_pin);
//      
//      if (!input_state) {
//        
//        this->timer_.stop_timer();
//        this->timer_.reset_timer();
//        this->update_display();
//
//        this->state = Logic::State::READY;
//
//        if (actual_time >= this->target_hold_time){
//          this->display_.shutdown(true);
//          ESP.restart();
//        }
//
//      } else {
//        this->flag_reset_button = false;
//      }
//    }
//  }
//}
//
//void Logic::init_pins(int _sensor_pin, 
//                      int _button_reset_pin, 
//                      int _sensor_indicator_pin){
//  this->sensor_indicator_pin = _sensor_indicator_pin;
//  this->button_reset_pin = _button_reset_pin;
//  this->sensor_pin = _sensor_pin;
//  pinMode(this->sensor_indicator_pin, OUTPUT);
//  pinMode(this->button_reset_pin, INPUT_PULLUP);
//  pinMode(this->sensor_pin, INPUT_PULLUP);
//}
//
//void Logic::reboot(){
//  this->display_.shutdown(true);
//  ESP.restart();
//}
//
//
//void Logic::resetTimer(){
//  this->timer_.stop_timer();
//  this->timer_.reset_timer();
//  this->update_display();
//
//  this->state = Logic::State::READY;
//}