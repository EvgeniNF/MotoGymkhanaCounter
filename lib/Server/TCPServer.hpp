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

    ~TCPServer() = default;

    TCPServer(TCPServer&) = delete;

    TCPServer(TCPServer&&) = delete;

    TCPServer& operator=(const TCPServer&) = delete;

  public:
    /**
     * @brief Tcp server initialization
     * @param data Data for transfer
     */
    void initializationTcpServer(NetworkData* data);
    /**
     * @brief Loop for DNS server
     */
    void loopDNSRequestes();
    /**
     * @brief Set the Reboot Function object
     * @param function Callback reboot function
     */
    void setRebootFunction(std::function<void()> const& rebootFunction);
    /**
     * @brief Set the Reset Function object
     * @param function Callback reset function
     */
    void setResetFunction(std::function<void()> const& resetFunction);

  private:
    /**
     * @brief Handle connect new clients
     * @param client Connected tcp client
     */
    void handlerNewClient(void*, AsyncClient* client);
    /**
     * @brief Handle request data
     * @param client Client who send request
     * @param data Client data
     * @param len Len data
     */
    void handleData(void*, AsyncClient* client, void* data, size_t len);
    /**
     * @brief Handle disconnect client
     * @param client Disconnected client
     */
    void handleDisconnect(void*, AsyncClient* client);

  private:
    /// Callback reboot function
    std::function<void()> m_callBackRebootFunction;
    /// Callback reset function
    std::function<void()> m_callBackResetFunction;
    /// Tcp clients
    std::vector<AsyncClient*> m_clients;
    /// Tcp server
    std::unique_ptr<AsyncServer> m_tcpServer;
    /// Dns server
    std::unique_ptr<DNSServer> m_dnsServer;
    /// Network transive data
    NetworkData* m_transiveData{nullptr};
};

} // namespace server
