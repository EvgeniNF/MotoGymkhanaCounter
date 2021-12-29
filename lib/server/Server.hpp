#ifndef SERVER_SERVER_HPP
#define SERVER_SERVER_HPP

#include "HTTPServer.hpp"
#include "TCPServer.hpp"

namespace server {

class Server{
  public:
    Server();
    ~Server() = default;
    void initializationServer(NetworkData* data);
    void loopUpdateWebServer();

  public:
    Server(Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(const Server&) = delete;

  private:
    std::unique_ptr<TCPServer> tcpServer;
    std::unique_ptr<HTTPServer> httpServer;
    IPAddress localIp {192, 168, 100, 1};
    IPAddress gateway {192, 168, 100, 1};
    IPAddress subnet  {255, 255, 255, 0};
    const unsigned int wifiChanel {8};
    const unsigned int maxNumConnections {15};
};

}

#endif // SERVER_SERVER_HPP