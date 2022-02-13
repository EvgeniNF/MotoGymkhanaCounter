#include "Max7219Spi.h"

namespace max7219spi{

Max7219Spi::Max7219Spi(int csPin){
  this->beginDisplay(csPin);
}
void Max7219Spi::beginDisplay(const int csPin) {
  
  if (this->m_statusDisplay == STATUS_DISPLAY::RUN) { return; }

  this->m_csSpi = csPin;
  
  // Set pin mode for SPI pins
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(this->m_csSpi, OUTPUT);
  
  // Set SPI mode
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  // Statrt SPI
  SPI.begin();
  digitalWrite(this->m_csSpi, HIGH);  
  this->spiTransfer(15, 0);
  this->spiTransfer(11, 7);
  this->spiTransfer(9, 0);
  this->clearDisplay();
  this->shutdown(true);
  this->spiTransfer(10, 15);

  this->m_statusDisplay = STATUS_DISPLAY::RUN;
}
void Max7219Spi::spiTransfer(volatile byte opcode, volatile byte data){

  //Enable the line
  digitalWrite(this->m_csSpi, LOW);
  
  //Now shift out the data
  SPI.beginTransaction(SPISettings(this->m_spiSpeed, MSBFIRST, SPI_MODE0));
  SPI.transfer(opcode);
  SPI.transfer(data);
  
  //Latch the data into the display
  digitalWrite(this->m_csSpi, HIGH);
  SPI.endTransaction();
}
void Max7219Spi::clearDisplay() {
  for(int i = 0; i < 8; i++)
    this->spiTransfer(i + 1, 0);
}
void Max7219Spi::shutdown(bool statusShutdonw) {
  statusShutdonw ? this->spiTransfer(12, 0) : this->spiTransfer(12, 1);
} 
void Max7219Spi::setDigit(int digit, byte value, bool dot){
  if (this->m_statusDisplay == STATUS_DISPLAY::NOT_INIT)
    return;

  if (digit < 0 || digit > 7 || value > 9)
    return;

  value = pgm_read_byte_near(charTable + value); 
  
  if (dot)
   value |= B10000000;

  this->spiTransfer(digit + 1, value);
}

}