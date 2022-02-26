#include "TCPServer.hpp"
#include "../../include/configs.hpp"
#include "EEPROM.h"

namespace server
{

namespace requests
{

const std::string get_data{"get_data"};
const std::string set_ssid{"set_ssid"};
const std::string set_password{"set_password"};
const std::string get_settings{"get_settings"};
const std::string reboot{"reboot"};
const std::string reset_timer{"reset_timer"};

} // namespace requests

namespace responces
{
const std::string notFoundRequest{"Error! Request not found."};
const std::string ssidWrited{"ssid writed"};
const std::string settingNotWrited{"Error! Setting not writed."};
const std::string passWrited{"pass writed"};
} // namespace responces

TCPServer::TCPServer()
{
    m_tcpServer =
        std::make_unique<AsyncServer>(configs::network_settings::tcpPort);
    m_dnsServer = std::make_unique<DNSServer>();
}

void TCPServer::initializationTcpServer(NetworkData* data)
{
    m_dnsServer->start(configs::network_settings::dnsPort,
                       configs::network_settings::tcpHostName, WiFi.softAPIP());

    auto functionNewClient =
        std::bind(&TCPServer::handlerNewClient, this, std::placeholders::_1,
                  std::placeholders::_2);

    m_tcpServer->onClient(functionNewClient, nullptr);
    m_tcpServer->begin();
    m_transiveData = data;
}

void TCPServer::handlerNewClient(void*, AsyncClient* client)
{
    auto functionData = std::bind(&TCPServer::handleData, this,
                                  std::placeholders::_1, std::placeholders::_2,
                                  std::placeholders::_3, std::placeholders::_4);
    auto functionDisconnect =
        std::bind(&TCPServer::handleDisconnect, this, std::placeholders::_1,
                  std::placeholders::_2);
    m_clients.push_back(client);
    client->onData(functionData, nullptr);
    client->onDisconnect(functionDisconnect, nullptr);
}

void TCPServer::handleDisconnect(void*, AsyncClient* client)
{
    auto lambda = [client](const AsyncClient* activeClient) {
        return client == activeClient ? true : false;
    };

    auto disconectedClient =
        std::find_if(m_clients.begin(), m_clients.end(), lambda);
    
    m_clients.erase(disconectedClient);
}

void TCPServer::handleData(void*, AsyncClient* client, void* data,
                           size_t len)
{
    std::string request;
    auto buff = reinterpret_cast<unsigned char*>(data);
    for (size_t index = 0; index < len; ++index)
    {
        request.push_back(static_cast<char>(buff[index]));
    }

    if (client->space() > 32 && client->canSend())
    {
        std::string responce;
        if (request == requests::get_data)
        {
            responce +=
                "state:" + std::to_string(*m_transiveData->espStatus) + ';';
            responce +=
                "timer:" + std::to_string(*m_transiveData->timerTime) + ';';
            responce +=
                "time:" + std::to_string(*m_transiveData->timeOnEsp) + ';';
            client->add(responce.c_str(), responce.size());
            client->send();
        }
        else if (request == requests::get_settings)
        {
            responce += "ssid:" + std::string(WiFi.softAPSSID().c_str()) + ';';
            responce += "pass:" + std::string(WiFi.softAPPSK().c_str()) + ';';
            client->add(responce.c_str(), responce.size());
            client->send();
        }
        else if (request == requests::set_password)
        {
            responce = request.substr(request.find(':') + 1);
            if (responce.size() < 100)
            {
                EEPROM.begin(201);
                EEPROM.write(1, static_cast<uint8_t>(responce.size()));
                uint8_t offset = 2;
                for (size_t index = 0; index < responce.size(); ++index)
                {
                    EEPROM.write(index + offset,
                                 static_cast<uint8_t>(responce.at(index)));
                }
                EEPROM.end();
                client->add(responces::ssidWrited.c_str(),
                            responces::ssidWrited.size());
                client->send();
            }
            else
            {
                client->add(responces::settingNotWrited.c_str(),
                            responces::settingNotWrited.size());
                client->send();
            }
        }
        else if (request == requests::set_ssid)
        {
            responce = request.substr(request.find(':') + 1);
            if (responce.size() < 100)
            {
                EEPROM.begin(201);
                EEPROM.write(101, static_cast<uint8_t>(responce.size()));
                uint8_t offset = 102;
                for (size_t index = 0; index < responce.size(); ++index)
                {
                    EEPROM.write(index + offset,
                                 static_cast<uint8_t>(responce.at(index)));
                }
                EEPROM.end();
                client->add(responces::passWrited.c_str(),
                            responces::passWrited.size());
                client->send();
            }
            else
            {
                client->add(responces::settingNotWrited.c_str(),
                            responces::settingNotWrited.size());
                client->send();
            }
        }
        else if (request == requests::reboot)
        {
            m_callBackRebootFunction();
        }
        else if (request == requests::reset_timer)
        {
            m_callBackResetFunction();
        }
        else
        {
            client->add(responces::notFoundRequest.c_str(),
                        responces::notFoundRequest.size());
            client->send();
        }
    }
}

void TCPServer::loopDNSRequestes() { m_dnsServer->processNextRequest(); }

void TCPServer::setRebootFunction(std::function<void()> const& rebootFunction)
{
    m_callBackRebootFunction = rebootFunction;
}

void TCPServer::setResetFunction(std::function<void()> const& resetFunction)
{
    m_callBackResetFunction = resetFunction;
}

} // namespace server
