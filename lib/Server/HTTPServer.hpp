#pragma once

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
    std::unique_ptr<ESP8266WebServer> m_webServer;
    std::unique_ptr<ESP8266HTTPUpdateServer> m_httpUpdateServer;
    static constexpr unsigned int httpPort {80};
    std::string m_updateHostName {"update"};
    NetworkData* m_transiveData {nullptr};
};

}
