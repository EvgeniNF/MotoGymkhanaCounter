#ifndef INCLUDE_LOGIC_HPP
#define INCLUDE_LOGIC_HPP

#include <Arduino.h>
#include <wifi_server.hpp>
#include <MyTimer.hpp>
#include <max7219_spi.hpp>


class Logic{
 public:
  /**
   * @brief Construct a new Logic object
   * 
   */
  Logic();
  /**
   * @brief Destroy the Logic object
   * 
   */
  ~Logic() = default;
  /**
   * @brief Init display
   * 
   * @param cs_disp_pin Chip select pin 
   * @param _update_period Display update period
   */
  void init_display(int cs_disp_pin, unsigned short int _update_period = 142);
  /**
   * @brief Init server
   * 
   * @param _ssid SSID access point
   * @param _pass Pasword access point
   * @param local_ip Local IP address
   * @param gateway Gateway address
   * @param subnet Net mask
   * @param port Port
   */
  void init_wifi_server(const String& _ssid, const String& _pass, IPAddress local_ip, IPAddress gateway, IPAddress subnet, int port);
  /**
   * @brief Loop function
   * 
   */
  void main_work();
  /**
   * @brief Sensor handler
   * 
   */
  void sensor_signal();
  /**
   * @brief Button handler
   * 
   */
  void reset_button_signal();
  /**
   * @brief Configure pins
   * 
   * @param _sensor_pin 
   * @param _button_reset_pin 
   * @param _sensor_indicator_pin 
   */
  void init_pins(int _sensor_pin, int _button_reset_pin, int _sensor_indicator_pin);

 private:
  /**
   * @brief Update digits on display
   * 
   */
  void update_display();
  /**
   * @brief Button handler
   * 
   */
  void handle_reset_button();
  /**
   * @brief Set the digit on disp object
   * 
   * @param offset 
   * @param num_dig 
   * @param value 
   */
  void set_digit_on_disp(int offset, int num_dig, int value);

 private:
  /**
   * @brief State of logic object
   * 
   */
  enum class State : unsigned short int{
    NOT_INIT = 0,     // Don't call init functions
    DISPLAY_INIT = 1, // Call only display init function
    WIFI_INIT = 2,    // Call only wifi init function
    READY = 3,        // Ready for start count
    COUNT = 4,        // Work count
    STOPED = 5        // Stoped timer
  };

 private:

  // Led pin
  int sensor_indicator_pin {0}; 
  // Button reset pin
  int button_reset_pin {0};
  // Sensor pin
  int sensor_pin {0};
  // State
  State state {State::NOT_INIT};
  // Display
  max7219_spi::MAX7219_SPI display_{};
  // Timer
  mytimer::MyTimer timer_{};
  // Server
  wifi_server::Wifi_server server_{};
  // Timer pointer
  const unsigned long int* timer_register_ptr{};
  // Result of counter
  unsigned long int result {0};
  // Update display period
  unsigned short int update_period {1};
  // Status display
  int status[8] {0, 0, 0, 0, 0, 0, 0, 0};
  // Prev time update display
  unsigned long int prev_time {0};
  // Hold time
  unsigned long int target_hold_time {0};
  // Const for convert time
  const unsigned long time_convers[4] = {3'600'000, 60'000, 1'000, 1};
  // Holding button time
  const unsigned long hold_reset_time {1500};
  // Flag reset button
  bool flag_reset_button{false};
  // Filter for reset button (ms)
  const unsigned int button_reset_filter_time { 100 };
  // Prev time button
  unsigned long int prev_time_button {0};
};

#endif // INCLUDE_LOGIC_HPP