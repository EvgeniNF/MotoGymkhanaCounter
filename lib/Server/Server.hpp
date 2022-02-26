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
    
    Server(Server&) = delete;
    
    Server(Server&&) = delete;
    
    Server& operator=(const Server&) = delete;
  
  public:  
    /**
     * @brief Network initialization
     * @param data Data for transfer
     * @param rebootFn Callback reboot function
     * @param resetFn Callback reset function
     */
    void initializationServer(NetworkData* data,
                              const std::function<void()>& rebootFn,
                              const std::function<void()>& resetFn);
    /**
     * @brief Web server loop
     */
    void loopUpdateWebServer();

  private:
    /// Tcp server
    std::unique_ptr<TCPServer> m_tcpServer;
    /// Http server
    std::unique_ptr<HTTPServer> m_httpServer;
};

} // namespace server
