
#ifndef SERVER_HTTPSERVER_HPP
#define SERVER_HTTPSERVER_HPP

#include <memory>
#include <string>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include "NetworkData.hpp"

namespace server {

class HTTPServer {
  public:
    HTTPServer();
    explicit HTTPServer(const std::string& updateHostname);
    ~HTTPServer() = default;

    void initializationWebServer(NetworkData* data);
    void initializationUpdateServer();

    void handleClinets();

  private:
    void handleHomePage();
    void handleErrorPage();

  public:
    HTTPServer(HTTPServer&) = delete;
    HTTPServer(HTTPServer&&) = delete;
    HTTPServer& operator=(const HTTPServer&) = delete;

  private:
    std::unique_ptr<ESP8266WebServer> webServer;
    std::unique_ptr<ESP8266HTTPUpdateServer> httpUpdateServer;
    const unsigned int http_port {80};
    std::string _updateHostname {"update"};
    NetworkData* _data {nullptr};
};

}

#endif // SERVER_HTTPSERVER_HPP