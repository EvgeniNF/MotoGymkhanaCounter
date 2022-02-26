#include "../include/Logic.hpp"
#include "../include/configs.hpp"

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
    m_networkData.espStatus =
        reinterpret_cast<unsigned short int*>(&m_espStatus);
    m_networkData.timeOnEsp = &m_systemTime;
    m_networkData.timerTime = m_timer.getTimerPtr();
    m_sensorIndicatorPin = configs::indicator_led_configs::ledPinNo;
    m_displayUpdatePeriod = configs::display::updatePeriodDisplay;
    m_laserSensor.setCallbackFunction(std::bind(&Logic::sensorIvent, this));
    pinMode(m_sensorIndicatorPin, configs::indicator_led_configs::outputMode);
    initServer();
    initDisplay(configs::display::csPinNo);
    m_espStatus = STATE::READY;
}

void Logic::updateSensorIndicator()
{
    if (configs::indicator_led_configs::invert)
    {
        if (digitalRead(configs::laser_sensor::sensorPinNo))
        {
            digitalWrite(m_sensorIndicatorPin, HIGH);
        }
        else
        {
            digitalWrite(m_sensorIndicatorPin, LOW);
        }
    }
    else
    {
        if (digitalRead(configs::laser_sensor::sensorPinNo))
        {
            digitalWrite(m_sensorIndicatorPin, HIGH);
        }
        else
        {
            digitalWrite(m_sensorIndicatorPin, LOW);
        }
    }
}

void Logic::sensorIvent()
{
    if (m_espStatus == STATE::READY)
    {
        m_timer.startTimer(configs::timer::updateTimePeriod);
        m_espStatus = STATE::COUNT;
    }
    else if (m_espStatus == STATE::COUNT)
    {
        m_timer.stopTimer();
        m_espStatus = STATE::STOPED;
    }
    else if (m_espStatus == STATE::STOPED)
    {
        m_timer.resetTimer();
        m_timer.startTimer(configs::timer::updateTimePeriod);
        m_espStatus = STATE::COUNT;
    }
}

void Logic::updateSystemTime() { m_systemTime = millis(); }

void Logic::updateDisplay()
{
    if (m_systemTime > m_nextTimeUpdateDisplay)
    {
        unsigned long int timerTime = *m_timer.getTimerPtr();
        int timeForDisplay = 0;
        for (int index = 0; index < 4; index++)
        {
            timeForDisplay = static_cast<int>(timerTime / m_timeConvers[index]);
            timerTime = timerTime - (timeForDisplay * m_timeConvers[index]);
            if (index == 0)
                setDigitOnDisplay(7, 1, timeForDisplay);
            else if (index == 1)
                setDigitOnDisplay(5, 2, timeForDisplay);
            else if (index == 2)
                setDigitOnDisplay(3, 2, timeForDisplay);
            else
                setDigitOnDisplay(0, 3, timeForDisplay);
        }
        m_nextTimeUpdateDisplay = m_systemTime + m_displayUpdatePeriod;
    }
}

void Logic::loopWork()
{
    updateSystemTime();
    if (m_resetButton.isClicked())
    {
        resetTimer();
    }
    if (m_resetButton.isHolded())
    {
        reboot();
    }
    updateDisplay();
    updateSensorIndicator();
    m_server.loopUpdateWebServer();
}

void Logic::setZerosOnDisplay()
{
    // Set zeros on the display
    m_display.setDigit(0, 0);
    m_display.setDigit(1, 0);
    m_display.setDigit(2, 0);
    m_display.setDigit(3, 0, true);
    m_display.setDigit(4, 0);
    m_display.setDigit(5, 0, true);
    m_display.setDigit(6, 0);
    m_display.setDigit(7, 0, true);
}

void Logic::initDisplay(int csDisplayPin)
{
    m_display.beginDisplay(D8);
    m_display.shutdown(false);
    m_display.clearDisplay();
    setZerosOnDisplay();
}

void Logic::initServer()
{
    // Init wifi server
    m_server.initializationServer(&m_networkData,
                                  std::bind(&Logic::reboot, this),
                                  std::bind(&Logic::resetTimer, this));
}

void Logic::reboot()
{
    digitalWrite(m_sensorIndicatorPin, LOW);
    m_display.shutdown(true);
    ESP.restart();
}

void Logic::resetTimer()
{
    m_timer.stopTimer();
    m_timer.resetTimer();
    if (m_espStatus != STATE::NOT_INIT)
    {
        m_espStatus = STATE::READY;
    }
}

void Logic::setDigitOnDisplay(int offset, int num_dig, int value)
{
    for (int i = offset; i < num_dig + offset; i++)
    {
        int reg = value % 10;
        if (reg != m_statusDispaly[i])
        {
            m_statusDispaly[i] = reg;
            if (i == 3 || i == 5 || i == 7)
            {
                m_display.setDigit(i, reg, true);
            }
            else
            {
                m_display.setDigit(i, reg);
            }
        }
        value /= 10;
    }
}
