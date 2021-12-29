#include "TCPServer.hpp"
#include "EEPROM.h"

namespace server{

namespace requests {

const std::string get_data {"get_data"};
const std::string set_ssid {"set_ssid"};
const std::string set_password {"set_password"};
const std::string get_settings {"get_settings"};
const std::string reboot {"reboot"};

}

namespace responces {

const std::string notFoundRequest {"Error! Request not found."};
const std::string ssidWrited {"ssid writed"};
const std::string settingNotWrited {"Error! Setting not writed."};
const std::string passWrited {"pass writed"};

}


TCPServer::TCPServer() {
    this->tcpServer = std::make_unique<AsyncServer>(this->tcpPort);
    this->dnsServer = std::make_unique<DNSServer>();
}

TCPServer::TCPServer(const std::string& hostName) {
    this->tcpServer = std::make_unique<AsyncServer>(this->tcpPort);
    this->dnsServer = std::make_unique<DNSServer>();
    this->host = hostName;
}

void TCPServer::initializationTcpServer(NetworkData* data) {
    this->dnsServer->start(this->dnsPort, 
                           this->host.c_str(),
                           WiFi.softAPIP());
    auto functionNewClient = std::bind(&TCPServer::handlerNewClient, 
                                         this, 
                                         std::placeholders::_1, 
                                         std::placeholders::_2);
    this->tcpServer->onClient(functionNewClient, nullptr);
    this->tcpServer->begin();
    this->_data = data;
    this->_data->host_name = &this->host;
}

void TCPServer::handlerNewClient(void* arg,
                                 AsyncClient* client) {
    auto functionData = std::bind(&TCPServer::handleData, 
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2,
                                  std::placeholders::_3,
                                  std::placeholders::_4);
    auto functionDisconnect = std::bind(&TCPServer::handleDisconnect, 
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2);
    this->clients.push_back(client);
    client->onData(functionData, nullptr);
    client->onDisconnect(functionDisconnect, nullptr);
}

void TCPServer::handleDisconnect(void* arg, AsyncClient* client) {
    auto lambda = 
            [client](const AsyncClient* arg2) {
                return client == arg2 ? true : false;
            };
    auto disconectedClient = std::find_if(this->clients.begin(), this->clients.end(), lambda);
    this->clients.erase(disconectedClient);
}

void TCPServer::handleData(void* arg, 
                           AsyncClient* client, 
                           void *data, 
                           size_t len) {
    std::string request;
    auto buff = reinterpret_cast<unsigned char*>(data);
    for(size_t index = 0; index < len; ++index){
        request.push_back(static_cast<char>(buff[index]));
    }
    
    if (client->space() > 32 && client->canSend()) {
        std::string responce;
        if (request == requests::get_data) {
            responce += "state:" 
                        + std::to_string(*_data->status)
                        + ';';
            responce += "timer:" 
                        + std::to_string(*_data->timer_time)
                        + ';';
            responce += "time:" 
                        + std::to_string(*_data->timer_time)
                        + ';';
            client->add(responce.c_str(), responce.size());
            client->send();
        } else if (request == requests::get_settings) {
            responce += "ssid:"
                        + std::string(WiFi.softAPSSID().c_str())
                        + ';';
            responce += "pass:"
                        + std::string(WiFi.softAPPSK().c_str())
                        + ';';
            client->add(responce.c_str(), responce.size());
            client->send();
        } else if (request.find(requests::set_password) != std::string::npos) {
            responce = request.substr(request.find(':') + 1);
            if (responce.size() < 100) {
                EEPROM.begin(201);
                EEPROM.write(1, static_cast<uint8_t>(responce.size()));
                uint8_t offset = 2;
                for (size_t index = 0; index < responce.size(); ++index) {
                    EEPROM.write(index + offset,
                                 static_cast<uint8_t>(responce.at(index)));
                }
                EEPROM.end();
                client->add(responces::ssidWrited.c_str(),
                            responces::ssidWrited.size());
                client->send();
            } else {
                client->add(responces::settingNotWrited.c_str(),
                            responces::settingNotWrited.size());
                client->send();
            }
        } else if (request.find(requests::set_ssid) != std::string::npos) {
            responce = request.substr(request.find(':') + 1);
            if (responce.size() < 100) {
                EEPROM.begin(201);
                EEPROM.write(101, static_cast<uint8_t>(responce.size()));
                uint8_t offset = 102;
                for (size_t index = 0; index < responce.size(); ++index) {
                    EEPROM.write(index + offset,
                                 static_cast<uint8_t>(responce.at(index)));
                }
                EEPROM.end();
                client->add(responces::passWrited.c_str(),
                            responces::passWrited.size());
                client->send();
            } else {
                client->add(responces::settingNotWrited.c_str(),
                            responces::settingNotWrited.size());
                client->send();
            }
        } else if (request == requests::reboot) {
            ESP.restart();
        } else {
            client->add(responces::notFoundRequest.c_str(), 
                        responces::notFoundRequest.size());
            client->send();
        }
    }

}

void TCPServer::loopDNSRequestes() {
    this->dnsServer->processNextRequest();
}

}