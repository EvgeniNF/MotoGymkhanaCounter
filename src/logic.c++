#include "../include/logic.h"

#include "../include/configs.h"

#include <string>

Logic::Logic()
    : m_display(), m_timer(), m_server(),
      m_resetButton(configs::reset_button::buttonPinNo,
                    configs::reset_button::debounceTimeMs,
                    configs::reset_button::holdTimeMs,
                    configs::reset_button::inputMode,
                    configs::reset_button::buttonType,
                    configs::reset_button::interruptMode),
      m_laserSensor(configs::laser_sensor::sensorPinNo,
                    configs::laser_sensor::inputMode,
                    configs::laser_sensor::interruptMode,
                    configs::laser_sensor::filterTime)
{
    this->m_networkData.m_espStatus =
        reinterpret_cast<unsigned short int*>(&this->m_espStatus);
    this->m_networkData.m_timeOnEsp = &this->m_systemTime;
    this->m_networkData.m_timerTime = this->m_timer.getTimerPtr();
    this->m_sensorIndicatorPin = configs::indicator_led_configs::ledPinNo;
    this->m_displayUpdatePeriod = configs::display::updatePeriodDisplay;
    this->m_laserSensor.setCallbackFunction(
        std::bind(&Logic::sensorIvent, this));
    pinMode(this->m_sensorIndicatorPin,
            configs::indicator_led_configs::outputMode);
    this->initServer();
    this->initDisplay(configs::display::csPinNo);
    this->m_espStatus = STATE::READY;
}
void Logic::updateSensorIndicator()
{
    bool isSensorHigh{false};
    if (configs::indicator_led_configs::invert)
    {
        isSensorHigh =
            !static_cast<bool>(digitalRead(this->m_sensorIndicatorPin));
    }
    else
    {
        isSensorHigh =
            static_cast<bool>(digitalRead(this->m_sensorIndicatorPin));
    }
    if (isSensorHigh)
    {
        digitalWrite(this->m_sensorIndicatorPin, HIGH);
    }
    else
    {
        digitalWrite(this->m_sensorIndicatorPin, LOW);
    }
}
void Logic::sensorIvent()
{
    if (this->m_espStatus == STATE::READY)
    {
        this->m_timer.startTimer(configs::timer::updateTimePeriod);
        this->m_espStatus = STATE::COUNT;
    }
    else if (this->m_espStatus == STATE::COUNT)
    {
        this->m_timer.stopTimer();
        this->m_espStatus = STATE::STOPED;
    }
    else if (this->m_espStatus == STATE::STOPED)
    {
        this->m_timer.resetTimer();
        this->m_timer.startTimer(configs::timer::updateTimePeriod);
        this->m_espStatus = STATE::COUNT;
    }
}
void Logic::updateSystemTime() { this->m_systemTime = millis(); }
void Logic::updateDisplay()
{
    if (this->m_systemTime > this->m_nextTimeUpdateDisplay)
    {
        unsigned long int timerTime = *this->m_timer.getTimerPtr();
        int timeForDisplay = 0;
        for (int index = 0; index < 4; index++)
        {
            timeForDisplay =
                static_cast<int>(timerTime / this->m_timeConvers[index]);
            timerTime =
                timerTime - (timeForDisplay * this->m_timeConvers[index]);
            if (index == 0)
                this->setDigitOnDisplay(7, 1, timeForDisplay);
            else if (index == 1)
                this->setDigitOnDisplay(5, 2, timeForDisplay);
            else if (index == 2)
                this->setDigitOnDisplay(3, 2, timeForDisplay);
            else
                this->setDigitOnDisplay(0, 3, timeForDisplay);
        }
        this->m_nextTimeUpdateDisplay =
            this->m_systemTime + this->m_displayUpdatePeriod;
    }
}
void Logic::mainWork()
{
    this->updateSystemTime();
    if (this->m_resetButton.isClicked())
    {
        this->resetTimer();
    }
    if (this->m_resetButton.isHolded())
    {
        this->reboot();
    }
    this->updateDisplay();
    this->updateSensorIndicator();
    this->m_server.loopUpdateWebServer();
}
void Logic::setZerosOnDisplay()
{
    // Set zeros on the display
    this->m_display.setDigit(0, 0);
    this->m_display.setDigit(1, 0);
    this->m_display.setDigit(2, 0);
    this->m_display.setDigit(3, 0, true);
    this->m_display.setDigit(4, 0);
    this->m_display.setDigit(5, 0, true);
    this->m_display.setDigit(6, 0);
    this->m_display.setDigit(7, 0, true);
}
void Logic::initDisplay(int csDisplayPin)
{
    this->m_display.beginDisplay(D8);
    this->m_display.shutdown(false);
    this->m_display.clearDisplay();
    this->setZerosOnDisplay();
}
void Logic::initServer()
{
    // Init wifi server
    this->m_server.initializationServer(&this->m_networkData,
                                        std::bind(&Logic::reboot, this),
                                        std::bind(&Logic::resetTimer, this));
}
void Logic::reboot()
{
    this->m_display.shutdown(true);
    ESP.restart();
}
void Logic::resetTimer()
{
    this->m_timer.stopTimer();
    this->m_timer.resetTimer();
    if (this->m_espStatus != STATE::NOT_INIT)
    {
        this->m_espStatus = STATE::READY;
    }
}
void Logic::setDigitOnDisplay(int offset, int num_dig, int value)
{
    for (int i = offset; i < num_dig + offset; i++)
    {
        int reg = value % 10;
        if (reg != this->m_statusDispaly[i])
        {
            this->m_statusDispaly[i] = reg;
            if (i == 3 || i == 5 || i == 7)
            {
                this->m_display.setDigit(i, reg, true);
            }
            else
            {
                this->m_display.setDigit(i, reg);
            }
        }
        value /= 10;
    }
}