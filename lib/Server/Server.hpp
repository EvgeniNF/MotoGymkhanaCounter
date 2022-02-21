#pragma once

#include "HTTPServer.hpp"
#include "TCPServer.hpp"

namespace server
{

class Server
{
  public:
    Server();
    ~Server() = default;
    void initializationServer(NetworkData* data,
                              const std::function<void()>& rebootFn,
                              const std::function<void()>& resetFn);
    void loopUpdateWebServer();

  public:
    Server(Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(const Server&) = delete;

  private:
    std::unique_ptr<TCPServer> m_tcpServer;
    std::unique_ptr<HTTPServer> m_httpServer;
    IPAddress m_localIp{192, 168, 100, 1};
    IPAddress m_gateway{192, 168, 100, 1};
    IPAddress m_subnet{255, 255, 255, 0};
    const unsigned int m_wifiChanel{8};
    const unsigned int m_maxNumConnections{15};
};

} // namespace server
