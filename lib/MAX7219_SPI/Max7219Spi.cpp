#include "Max7219Spi.hpp"
#include "CharTable.hpp"


namespace max7219spi
{

Max7219Spi::Max7219Spi(int csPin) { beginDisplay(csPin); }

void Max7219Spi::beginDisplay(const int csPin)
{
    if (m_statusDisplay == STATUS_DISPLAY::RUN)
    {
        return;
    }

    m_csSpi = csPin;

    // Set pin mode for SPI pins
    pinMode(MOSI, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(m_csSpi, OUTPUT);

    // Set SPI mode
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);

    // Statrt SPI
    SPI.begin();
    digitalWrite(m_csSpi, HIGH);
    spiTransfer(15, 0);
    spiTransfer(11, 7);
    spiTransfer(9, 0);
    clearDisplay();
    shutdown(true);
    spiTransfer(10, 15);

    m_statusDisplay = STATUS_DISPLAY::RUN;
}

void Max7219Spi::spiTransfer(volatile byte opcode, volatile byte data)
{
    // Enable the line
    digitalWrite(m_csSpi, LOW);

    // Now shift out the data
    SPI.beginTransaction(SPISettings(m_spiSpeed, MSBFIRST, SPI_MODE0));
    SPI.transfer(opcode);
    SPI.transfer(data);

    // Latch the data into the display
    digitalWrite(m_csSpi, HIGH);
    SPI.endTransaction();
}

void Max7219Spi::clearDisplay()
{
    for (int i = 0; i < 8; i++)
        spiTransfer(i + 1, 0);
}

void Max7219Spi::shutdown(bool statusShutdonw)
{
    statusShutdonw ? spiTransfer(12, 0) : spiTransfer(12, 1);
}

void Max7219Spi::setDigit(int digit, byte value, bool dot)
{
    if (m_statusDisplay == STATUS_DISPLAY::NOT_INIT)
        return;

    if (digit < 0 || digit > 7 || value > 9)
        return;

    value = pgm_read_byte_near(charTable + value);

    if (dot)
        value |= B10000000;

    spiTransfer(digit + 1, value);
}

} // namespace max7219spi
