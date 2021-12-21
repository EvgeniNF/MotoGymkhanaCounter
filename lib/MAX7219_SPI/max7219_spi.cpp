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
  this->spi_transfer(15, 0);
  this->spi_transfer(11, 7);
  this->spi_transfer(9, 0);
  this->clear_display();
  this->shutdown(true);
  this->spi_transfer(10, 15);
  
  this->status = Status_display::RUN;
}

void MAX7219_SPI::begin_display(const int cs_pin) {
  
  this->spi_cs = cs_pin;
  
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
  this->spi_transfer(15, 0);
  this->spi_transfer(11, 7);
  this->spi_transfer(9, 0);
  this->clear_display();
  this->shutdown(true);
  this->spi_transfer(10, 15);

  this->status = Status_display::RUN;
}

void MAX7219_SPI::spi_transfer(volatile byte opcode, volatile byte data){

  //Enable the line
  digitalWrite(spi_cs, LOW);
  
  //Now shift out the data
  SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
  SPI.transfer(opcode);
  SPI.transfer(data);
  
  //Latch the data into the display
  digitalWrite(spi_cs, HIGH);
  SPI.endTransaction();
}

void MAX7219_SPI::clear_display() {
  for(int i = 0; i < 8; i++)
    this->spi_transfer(i + 1, 0);
}

void MAX7219_SPI::shutdown(bool b) {
  if(b)
    this->spi_transfer(12, 0);
  else
    this->spi_transfer(12, 1);
} 

void MAX7219_SPI::set_digit(int digit, byte value, bool dp){
  if (status == Status_display::NOT_INIT)
    return;

  if(digit < 0 || digit > 7 || value > 9)
    return;

  value = pgm_read_byte_near(charTable + value); 
  
  if(dp)
    value |= B10000000;

  this->spi_transfer(digit + 1, value);
}

}