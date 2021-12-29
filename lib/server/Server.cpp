#include "Server.hpp"
#include <EEPROM.h>

namespace server {

Server::Server() {
    this->tcpServer = std::make_unique<TCPServer>();
    this->httpServer = std::make_unique<HTTPServer>();
}

void Server::initializationServer(NetworkData* data) {
    WiFi.mode(WIFI_AP_STA);
  
    WiFi.softAPConfig(this->localIp,
                        this->gateway, 
                        this->subnet);

    std::string ssid {"MotoCounetr1"};
    std::string pass {"19741974"};
    
    EEPROM.begin(201);
    if (EEPROM.read(0) != 50) {
        
        EEPROM.write(1, static_cast<uint8_t>(ssid.size()));
        uint8_t offset = 2;
        for (size_t index = 0; index < ssid.size(); ++index) {
            EEPROM.write(index + offset,
                         static_cast<uint8_t>(ssid.at(index)));
        }

        EEPROM.write(101, static_cast<uint8_t>(pass.size()));
        offset = 102;
        for (size_t index = 0; index < pass.size(); ++index) {
            EEPROM.write(index + offset,
                         static_cast<uint8_t>(pass.at(index)));
        }

        EEPROM.write(0, 50);

    } else {
        ssid.clear();
        pass.clear();

        uint8_t len = EEPROM.read(1);
        uint8_t offset = 2;
        ssid.resize(len);
        for (size_t index = 0; index < len; ++index){
            ssid.at(index) = static_cast<char>(EEPROM.read(index + offset));
        }

        len = EEPROM.read(101);
        offset = 102;
        pass.resize(len);
        for (size_t index = 0; index < len; ++index){
            pass.at(index) = static_cast<char>(EEPROM.read(index + offset));
        }

    }
    EEPROM.end();
    Serial.println(ssid.c_str());
    Serial.println(pass.c_str());
    while(!WiFi.softAP(ssid.c_str(),
                       pass.c_str(),
                       this->wifiChanel,
                       false,
                       this->maxNumConnections)){
        delay(100);
    }

    this->httpServer->initializationUpdateServer();
    this->httpServer->initializationWebServer(data);
    this->tcpServer->initializationTcpServer(data);

}

void Server::loopUpdateWebServer(){
    this->httpServer->handleClinets();
    this->tcpServer->loopDNSRequestes();
}

}