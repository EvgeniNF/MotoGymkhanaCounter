#ifndef MAX7219_SPI_MAX7219_SPI_HPP
#define MAX7219_SPI_MAX7219_SPI_HPP
#include "Arduino.h"
#include <pgmspace.h>
#include <SPI.h>

namespace max7219_spi{

const static byte charTable [] PROGMEM  = {
  B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, // 000-007 :
  B01111111, B01111011, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, // 008-015 :
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, // 016-023 :
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, // 024-031 :
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, // 032-039 :
  B00000000, B00000000, B00000000, B00000000, B10000000, B00000001, B10000000, B00000000, // 040-047 :
  B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, // 048-055 : 0-7
  B01111111, B01111011, B00000000, B00000000, B00000000, B00001001, B00000000, B00000000, // 056-063 : 8-?
  B00000000, B01110111, B00011111, B01001110, B00111101, B01001111, B01000111, B01011110, // 064-071 : @-G
  B00110111, B00110000, B00111100, B00000000, B00001110, B00000000, B00000000, B01111110, // 072-079 : H-O
  B01100111, B00000000, B00000000, B01011011, B00000000, B00111110, B00111110, B00000000, // 080-087 : P-W
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00001000, // 088-095 : X-_
  B00000000, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, B00000000, // 096-103 : `-g
  B00110111, B00010000, B00000000, B00000000, B00001110, B00000000, B00010101, B00011101, // 104-113 : h-o
  B01100111, B00000000, B00000000, B00000000, B00001111, B00011100, B00000000, B00000000, // 114-119 : p-w
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000  // 120-127 : x-[del]
};

class MAX7219_SPI{
 
 public:
  /**
   * @brief Construct a new max7219 spi object
   * 
   */
  MAX7219_SPI() = default;
  /**
   * @brief Construct a new max7219 spi object and start work whith display
   * 
   * @param _cs_pin Chip select pin
   */
  explicit MAX7219_SPI(const int _cs_pin);
  /**
   * @brief Destroy the max7219 spi object
   * 
   */
  ~MAX7219_SPI() = default;
  /**
   * @brief Function clear display
   * 
   */
  void clear_display();
  /**
   * @brief Set the digit object
   * 
   * @param digit Number of digit on display (0..7)
   * @param value Number (0..9)
   * @param dp Dot status
   */
  void set_digit(int digit, byte value, bool dp=false);
  /**
   * @brief Function init display
   * 
   * @param cs_pin Chip select pin
   */
  void begin_display(int cs_pin);
  /**
   * @brief Shutdown mode
   * 
   * @param b true - on, false - off 
   */
  void shutdown(bool b);
  
 private:
  /**
   * @brief SPI transaction
   * 
   * @param opcode Function code
   * @param data Data
   */
  void spi_transfer(volatile byte opcode, volatile byte data);
  
  
  
 private:
  // Data for transfer
  byte spidata[2] {0x00, 0x00};
  // Select pin
  int spi_cs = 0;
  // Transfer speed
  unsigned long spi_speed = 1'000'000;

};

}


#endif // MAX7219_SPI_MAX7219_SPI_HPP