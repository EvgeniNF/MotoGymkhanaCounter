
#ifndef SERVER_TCPServer_HPP
#define SERVER_TCPServer_HPP

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <vector>
#include <string>
#include "NetworkData.hpp"

namespace server{

class TCPServer{
  public:
    TCPServer();
    explicit TCPServer(const std::string& hostName);
    ~TCPServer() = default;
    void initializationTcpServer(NetworkData* data);
    void loopDNSRequestes();

  private:  
    void handlerNewClient(void* arg, AsyncClient* client);
    void handleData(void* arg, AsyncClient* client, void *data, size_t len);
    void handleDisconnect(void* arg, AsyncClient* client);
    
  public:
    TCPServer(TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;

  private:
    
    std::vector<AsyncClient*> clients;
    std::unique_ptr<AsyncServer> tcpServer;
    std::unique_ptr<DNSServer> dnsServer;
    std::string host {"tcp.server"};
    const unsigned int tcpPort {7050}; 
    const unsigned int dnsPort {53};
    NetworkData* _data {nullptr};
};

}

#endif  // SERVER_TCPServer_HPP