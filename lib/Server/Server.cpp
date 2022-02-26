#include "Server.hpp"
#include "../../include/configs.hpp"
#include <EEPROM.h>

namespace server
{

Server::Server()
{
    this->m_tcpServer = std::make_unique<TCPServer>();
    this->m_httpServer = std::make_unique<HTTPServer>();
}

void Server::initializationServer(NetworkData* data,
                                  const std::function<void()>& rebootFn,
                                  const std::function<void()>& resetFn)
{
    WiFi.mode(WIFI_AP_STA);

    WiFi.softAPConfig({configs::network_settings::localIp[0],
                       configs::network_settings::localIp[1],
                       configs::network_settings::localIp[2],
                       configs::network_settings::localIp[3]},
                      {configs::network_settings::getway[0],
                       configs::network_settings::getway[1],
                       configs::network_settings::getway[2],
                       configs::network_settings::getway[3]},
                      {configs::network_settings::mask[0],
                       configs::network_settings::mask[1],
                       configs::network_settings::mask[2],
                       configs::network_settings::mask[3]});

    std::string ssid{configs::network_settings::softApName};
    std::string pass{configs::network_settings::softApPassword};

    EEPROM.begin(201);
    if (EEPROM.read(0) != configs::network_settings::firstNumForWriteSettings)
    {
        EEPROM.write(1, static_cast<uint8_t>(ssid.size()));
        uint8_t offset = 2;
        for (size_t index = 0; index < ssid.size(); ++index)
        {
            EEPROM.write(index + offset, static_cast<uint8_t>(ssid.at(index)));
        }

        EEPROM.write(101, static_cast<uint8_t>(pass.size()));
        offset = 102;
        for (size_t index = 0; index < pass.size(); ++index)
        {
            EEPROM.write(index + offset, static_cast<uint8_t>(pass.at(index)));
        }

        EEPROM.write(0, 100);
    }
    else
    {
        ssid.clear();
        pass.clear();

        uint8_t len = EEPROM.read(1);
        uint8_t offset = 2;
        ssid.resize(len);
        for (size_t index = 0; index < len; ++index)
        {
            ssid.at(index) = static_cast<char>(EEPROM.read(index + offset));
        }

        len = EEPROM.read(101);
        offset = 102;
        pass.resize(len);
        for (size_t index = 0; index < len; ++index)
        {
            pass.at(index) = static_cast<char>(EEPROM.read(index + offset));
        }
    }
    EEPROM.end();
    Serial.println(ssid.c_str());
    Serial.println(pass.c_str());
    while (!WiFi.softAP(ssid.c_str(), pass.c_str(),
                        configs::network_settings::wifiChanel, 0,
                        configs::network_settings::maxNumConnecteions))
    {
        delay(100);
    }

    m_httpServer->initializationUpdateServer();
    m_httpServer->initializationWebServer(data);
    m_httpServer->setRebootFunction(rebootFn);
    m_httpServer->setResetFunction(resetFn);
    m_tcpServer->initializationTcpServer(data);
    m_tcpServer->setRebootFunction(rebootFn);
    m_tcpServer->setResetFunction(resetFn);
}

void Server::loopUpdateWebServer()
{
    m_httpServer->handleClinets();
    m_tcpServer->loopDNSRequestes();
}

} // namespace server
