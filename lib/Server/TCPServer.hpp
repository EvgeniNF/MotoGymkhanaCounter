#pragma once

#include "NetworkData.hpp"
#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <string>
#include <vector>

namespace server
{

class TCPServer
{
  public:
    TCPServer();
    explicit TCPServer(const std::string& hostName);
    ~TCPServer() = default;
    void initializationTcpServer(NetworkData* data);
    void loopDNSRequestes();
    void setRebootFunction(const std::function<void()>& function);
    void setResetFunction(const std::function<void()>& function);

  private:
    void handlerNewClient(void* arg, AsyncClient* client);
    void handleData(void* arg, AsyncClient* client, void* data, size_t len);
    void handleDisconnect(void* arg, AsyncClient* client);

  public:
    TCPServer(TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;

  private:
    std::function<void()> m_callBackRebootFunction;
    std::function<void()> m_callBackResetFunction;
    std::vector<AsyncClient*> m_clients;
    std::unique_ptr<AsyncServer> m_tcpServer;
    std::unique_ptr<DNSServer> m_dnsServer;
    std::string m_host{"tcp.server"};
    const unsigned int m_tcpPort{7050};
    const unsigned int m_dnsPort{53};
    NetworkData* m_transiveData{nullptr};
};

} // namespace server
