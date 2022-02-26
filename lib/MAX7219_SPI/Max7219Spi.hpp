#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <pgmspace.h>

namespace max7219spi
{

class Max7219Spi
{
  public:
    /**
     * @brief Construct a new max7219 spi object
     */
    Max7219Spi() = default;
    /**
     * @brief Construct a new max7219 spi object and start work whith display
     * @param csPin Chip select pin
     */
    explicit Max7219Spi(const int csPin);
    /**
     * @brief Destroy the max7219 spi object
     */
    ~Max7219Spi() = default;

  public:
    /**
     * @brief Function clear display
     */
    void clearDisplay();
    /**
     * @brief Set the digit object
     * @param digit Number of digit on display (0..7)
     * @param value Number (0..9)
     * @param dot Dot status
     */
    void setDigit(int digit, byte value, bool dot = false);
    /**
     * @brief Function init display
     * @param cs_pin Chip select pin
     */
    void beginDisplay(int csPin);
    /**
     * @brief Shutdown mode
     * @param statusShutdonw true - on, false - off
     */
    void shutdown(bool statusShutdonw);

  private:
    /**
     * @brief SPI transaction
     * @param opcode Function code
     * @param data Data
     */
    void spiTransfer(volatile byte opcode, volatile byte data);
    /**
     * @brief Status of dispaly
     */
    enum class STATUS_DISPLAY
    {
        NOT_INIT, // Not init
        RUN       // Running
    };
    // Status display
    STATUS_DISPLAY m_statusDisplay = STATUS_DISPLAY::NOT_INIT;
    // Select pin
    int m_csSpi = 0;
    // Transfer speed
    unsigned long m_spiSpeed = 1'000'000;
};

} // namespace max7219spi
