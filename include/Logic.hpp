#pragma once

#include <Arduino.h>

#include <Button.hpp>
#include <Max7219Spi.hpp>
#include <NetworkData.hpp>
#include <Sensor.hpp>
#include <Server.hpp>
#include <Timer.hpp>

class Logic
{
  public:
    /**
     * @brief Construct a new Logic object
     */
    Logic();
    /**
     * @brief Destroy the Logic object
     */
    ~Logic() = default;

  public:
    /**
     * @brief Loop work
     */
    void loopWork();

  private:
    /**
     * @brief Init display
     * @param cs_disp_pin Chip select pin
     * @param _update_period Display update period
     */
    void initDisplay(int csDisplayPin);
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
     * @brief Set zeros on the display
     */
    void setZerosOnDisplay();
    /**
     * @brief Update digits on display
     */
    void updateDisplay();
    /**
     * @brief Set the digit on disp object
     * @param offset
     * @param num_dig
     * @param value
     */
    void setDigitOnDisplay(int offset, int num_dig, int value);
    /**
     * @brief
     *
     */
    void updateSystemTime();
    /**
     * @brief
     *
     */
    void updateSensorIndicator();
    /**
     * @brief
     *
     */
    void reboot();
    /**
     * @brief
     *
     */
    void sensorIvent();
    /**
     * @brief
     *
     */
    void resetTimer();
    /**
     * @brief State of logic object
     */
    enum class STATE : unsigned short int
    {
        NOT_INIT = 0,     // Don't call init functions
        DISPLAY_INIT = 1, // Call only display init function
        WIFI_INIT = 2,    // Call only wifi init function
        READY = 3,        // Ready for start count
        COUNT = 4,        // Work count
        STOPED = 5        // Stoped timer
    };
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
    //
    int m_displayUpdatePeriod;
    //
    unsigned long int m_systemTime;
    //
    unsigned long int m_nextTimeUpdateDisplay;
    //
    STATE m_espStatus = {STATE::NOT_INIT};
    // Led pin
    int m_sensorIndicatorPin{0};
    // Status display
    int m_statusDispaly[8]{0, 0, 0, 0, 0, 0, 0, 0};
    // Const for convert time
    const unsigned long m_timeConvers[4] = {3'600'000, 60'000, 1'000, 1};
    // Network data
    NetworkData m_networkData{};
};
