#include "max7219_spi.hpp"

namespace max7219_spi{

MAX7219_SPI::MAX7219_SPI(int cs_pin) : spi_cs(cs_pin) {
  // Set pin mode for SPI pins
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(spi_cs, OUTPUT);
  
  // Set SPI mode
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  // Statrt SPI
  SPI.begin();
  digitalWrite(spi_cs, HIGH);  
  spi_transfer(15, 0);
  spi_transfer(11, 7);
  spi_transfer(9, 0);
  clear_display();
  shutdown(true);
  spi_transfer(10, 15);
}

void MAX7219_SPI::begin(const int cs_pin) {
  spi_cs = cs_pin;
  // Set pin mode for SPI pins
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(spi_cs, OUTPUT);
  
  // Set SPI mode
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  // Statrt SPI
  SPI.begin();
  digitalWrite(spi_cs, HIGH);  
  spi_transfer(15, 0);
  spi_transfer(11, 7);
  spi_transfer(9, 0);
  clear_display();
  shutdown(true);
}

void MAX7219_SPI::spi_transfer(volatile byte opcode, volatile byte data){
  //Create an array with the data to shift out
  spidata[0] = static_cast<byte>(0);
  spidata[1] = static_cast<byte>(0);

  //put our device data into the array
  spidata[1] = opcode;
  spidata[0] = data;
  
  //enable the line
  digitalWrite(spi_cs, LOW);
  
  //Now shift out the data
  SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
  SPI.transfer(spidata[1]);
  SPI.transfer(spidata[0]);
  
  //latch the data onto the display
  digitalWrite(spi_cs, HIGH);
  SPI.endTransaction();

}

void MAX7219_SPI::clear_display() {
  for(int i = 0; i < 8; i++)
    spi_transfer(i + 1, 0);
}

void MAX7219_SPI::shutdown(bool b) {
  if(b)
    spi_transfer(12, 0);
  else
    spi_transfer(12, 1);
} 

void MAX7219_SPI::set_digit(int digit, byte value, bool dp){
  if(digit < 0 || digit > 7 || value > 9)
    return;
  value = pgm_read_byte_near(charTable + value); 
  if(dp)
    value |= B10000000;
  spi_transfer(digit + 1, value);
}

}