#include "../include/logic.hpp"
#include <string>

void Logic::init_display(int cs_disp_pin, unsigned short int _update_period){
  // Initialisation function
  this->update_period = _update_period;
  this->begin_display(D8);
  this->shutdown(false);  
  this->clear_display();
  this->set_digit(0, 0);
  this->set_digit(1, 0);
  this->set_digit(2, 0);
  this->set_digit(3, 0, true);
  this->set_digit(4, 0);
  this->set_digit(5, 0, true);
  this->set_digit(6, 0);
  this->set_digit(7, 0, true);
  this->st_timer = State::READY;
}

void Logic::main_work(){
  // Update period in count state
  if (this->timer > this->prev_time && st_timer == State::COUNT)
    this->update_display();
  String req = String(static_cast<unsigned short int>(this->st_timer));
  req += ';';
  req += String(this->timer);
  req += ';';
  req += String(millis());
  this->send_request(req);
}

void Logic::sensor_signal(){
  if (st_timer == State::NOT_INIT) 
    return;
  if (this->timer == 0 && st_timer == State::READY){
    // Start timer
    this->update_display();
    this->start_timer(1);
    st_timer = State::COUNT;
  } else if (this->timer > 3'000 && st_timer == State::COUNT){
    // Stop timer
    this->stop_timer();
    this->update_display();
    st_timer = State::STOPED;
    this->start_timer(1);
  } else if (this->timer > 3'000 && st_timer == State::STOPED){
    // Delay after stop timer
    this->stop_timer();
    this->reset_timer();
    st_timer = State::READY;
  }
}

void Logic::reset_signal(){
  if (this->timer != 0 && st_timer != State::NOT_INIT){
    // Reset timer
    this->stop_timer();
    this->reset_timer();
    this->update_display();
    st_timer = State::READY;
  }
}

void Logic::update_display(){
  this->prev_time = this->timer;
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
  this->prev_time = this->timer + this->update_period;    
}

void Logic::set_digit_on_disp(int offset, int num_dig, int value){
  for (int i = offset; i < num_dig + offset; i++){
    int reg = value % 10;
    if (reg != this->status[i]){
      this->status[i] = reg;
      if (i == 3 || i == 5 || i == 7){
        this->set_digit(i, reg, true);
      } else {
        this->set_digit(i, reg);
      }
    }
    value /= 10;
  }
}