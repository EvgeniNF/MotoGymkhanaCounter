#pragma once

#include <Arduino.h>

#include <Server.hpp>
#include <Timer.h>
#include <Max7219Spi.h>
#include <NetworkData.hpp>
#include <Button.h>
#include <Sensor.h>


class Logic{
 public:
  /**
   * @brief Construct a new Logic object
   */
  Logic();
  /**
   * @brief Destroy the Logic object
   */
  ~Logic();
  /**
   * @brief Init display
   * @param cs_disp_pin Chip select pin 
   * @param _update_period Display update period
   */
  void initDisplay(int csDisplayPin,
                   unsigned short int updatePeriod = 50);
  /**
   * @brief Init server
   * @param _ssid SSID access point
   * @param _pass Pasword access point
   * @param local_ip Local IP address
   * @param gateway Gateway address
   * @param subnet Net mask
   * @param port Port
   */
  void initServer();
  /**
   * @brief Loop function
   */
  void mainWork();
  /**
   * @brief Sensor handler
   */
  void sensor_signal();
  /**
   * @brief Button handler
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
   * @brief Set zeros on the display
   */
  void setZerosOnDisplay();  
  /**
   * @brief Update digits on display
   */
  void update_display();
  /**
   * @brief Button handler
   */
  void handle_reset_button();
  /**
   * @brief Set the digit on disp object
   * @param offset 
   * @param num_dig 
   * @param value 
   */
  void set_digit_on_disp(int offset, int num_dig, int value);
  
  void reboot();

  void resetTimer();

 private:
  
  // Display
  max7219spi::Max7219Spi m_display{};
  // Timer
  timer::Timer m_timer{};
  // Server
  server::Server m_server{};
  // 
  button::Button m_resetButton;
  //
  sensor::Sensor m_laserSensor;
  /**
   * @brief State of logic object
   */
  enum class STATE : unsigned short int {
    NOT_INIT = 0,     // Don't call init functions
    DISPLAY_INIT = 1, // Call only display init function
    WIFI_INIT = 2,    // Call only wifi init function
    READY = 3,        // Ready for start count
    COUNT = 4,        // Work count
    STOPED = 5        // Stoped timer
  };

 private:
  // Led pin
  int m_sensorIndicatorPin {0}; 
  // Button reset pin
  int m_buttonResetPin {0};
  // Sensor pin
  int m_sensorPin {0};
  // State
  STATE m_state {STATE::NOT_INIT};
  
  // Timer pointer
  const unsigned long int* m_timerRegisterPtr{};
  // Result of counter
  unsigned long int m_countResult {0};
  // Update display period
  unsigned short int m_updatePeriod {1};
  // Status display
  int m_statusDispaly[8] {0, 0, 0, 0, 0, 0, 0, 0};
  // Prev time update display
  unsigned long int m_nextTimeUpdateDisplay {0};
  // Hold time
  unsigned long int m_targetHoldTimeButton {0};
  // Const for convert time
  const unsigned long m_timeConvers[4] = {3'600'000, 60'000, 1'000, 1};
  // Holding button time
  const unsigned long m_holdResetTime {1500};
  // Flag reset button
  bool m_flagResetButton{false};
  // Filter for reset button (ms)
  const unsigned int m_resetButtonDebounceTime { 100 };
  // Prev time button
  unsigned long int m_prevTimeButton {0};
  // Networck data
  NetworkData m_networkData{};
};
