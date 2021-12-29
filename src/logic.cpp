#include "../include/logic.hpp"
#include <string>

Logic::Logic() 
            : display_()
            , timer_()
            , server_() {
  this->_data.timer_time = this->timer_.get_timer_ptr();
  this->_data.time_on_esp = new unsigned long int(millis());
  this->_data.status = reinterpret_cast<unsigned short int*>(&this->state);
  state = Logic::State::NOT_INIT;
}

Logic::~Logic(){
  delete this->_data.time_on_esp;
}

void Logic::init_display(int cs_disp_pin, 
                         unsigned short int _update_period){
   
  // Initialisation display function
  this->update_period = _update_period;
  this->display_.begin_display(D8);
  this->display_.shutdown(false);  
  this->display_.clear_display();

  // Set zeros on the display
  this->display_.set_digit(0, 0);
  this->display_.set_digit(1, 0);
  this->display_.set_digit(2, 0);
  this->display_.set_digit(3, 0, true);
  this->display_.set_digit(4, 0);
  this->display_.set_digit(5, 0, true);
  this->display_.set_digit(6, 0);
  this->display_.set_digit(7, 0, true);

  // Set state Display ready
  if (state == Logic::State::NOT_INIT)
    this->state = State::DISPLAY_INIT;
  else if (state == Logic::State::WIFI_INIT)
    this->state = State::READY;
}

void Logic::init_server(){
  // Init wifi server
  this->server_.initializationServer(&this->_data);
  // Set state Display ready
  if (state == Logic::State::NOT_INIT)
    this->state = State::WIFI_INIT;
  else if (state == Logic::State::DISPLAY_INIT)
    this->state = State::READY;
}

void Logic::main_work(){
  *this->_data.time_on_esp = millis();
  this->server_.loopUpdateWebServer();

  // Update period in count state
  if (*this->_data.timer_time > this->prev_time &&
       this->state == State::COUNT)
    this->update_display();
  
  this->handle_reset_button();
}

void Logic::sensor_signal(){
  if (this->state == State::NOT_INIT ||
      this->state == Logic::State::DISPLAY_INIT ||
      this->state == Logic::State::WIFI_INIT) 
    return;
  if (*this->_data.timer_time == 0 && this->state == State::READY){
    // Start timer
    this->update_display();
    this->timer_.start_timer(1);
    state = State::COUNT;
  } else if (*this->_data.timer_time > 3'000 && this->state == State::COUNT){
    // Stop timer
    this->timer_.stop_timer();
    this->result = *this->_data.timer_time;
    this->update_display();
    this->timer_.reset_timer();
    this->timer_.start_timer(1);
    this->state = State::STOPED;
  } else if (*this->_data.timer_time > 3'000 && this->state == State::STOPED){
    // Delay after stop timer
    this->timer_.stop_timer();
    this->timer_.reset_timer();
    this->update_display();
    this->timer_.start_timer(1);
    this->state = State::COUNT;
  }
}

void Logic::update_display(){
  this->prev_time = *this->_data.timer_time;
  int act_time = 0;
  for (int index = 0; index < 4; index++){
    act_time = static_cast<int>(this->prev_time / this->time_convers[index]);
    this->prev_time = this->prev_time - (act_time * this->time_convers[index]); 
    if (index == 0)
      this->set_digit_on_disp(7, 1, act_time);
    else if (index == 1)
      this->set_digit_on_disp(5, 2, act_time);
    else if (index == 2)
      this->set_digit_on_disp(3, 2, act_time);
    else
      this->set_digit_on_disp(0, 3, act_time);         
  }
  this->prev_time = *this->_data.timer_time + this->update_period;    
}

void Logic::set_digit_on_disp(int offset, 
                              int num_dig, 
                              int value){
  for (int i = offset; i < num_dig + offset; i++){
    int reg = value % 10;
    if (reg != this->status[i]){
      this->status[i] = reg;
      if (i == 3 || i == 5 || i == 7){
        this->display_.set_digit(i, reg, true);
      } else {
        this->display_.set_digit(i, reg);
      }
    }
    value /= 10;
  }
}

void Logic::reset_button_signal() {
  if (!this->flag_reset_button) {
    this->flag_reset_button = true;
    this->prev_time_button = millis() + this->button_reset_filter_time;
    this->target_hold_time = millis() + this->hold_reset_time;
  }
}

void Logic::handle_reset_button(){
  if (this->flag_reset_button) {
    auto actual_time = millis();
    if (actual_time >= this->prev_time_button){
      
      bool input_state = digitalRead(this->button_reset_pin);
      
      if (!input_state) {
        
        this->timer_.stop_timer();
        this->timer_.reset_timer();
        this->update_display();

        this->state = Logic::State::READY;

        if (actual_time >= this->target_hold_time){
          this->display_.shutdown(true);
          ESP.restart();
        }

      } else {
        this->flag_reset_button = false;
      }
    }
  }
}

void Logic::init_pins(int _sensor_pin, 
                      int _button_reset_pin, 
                      int _sensor_indicator_pin){
  this->sensor_indicator_pin = _sensor_indicator_pin;
  this->button_reset_pin = _button_reset_pin;
  this->sensor_pin = _sensor_pin;
  pinMode(this->sensor_indicator_pin, OUTPUT);
  pinMode(this->button_reset_pin, INPUT_PULLUP);
  pinMode(this->sensor_pin, INPUT_PULLUP);
}