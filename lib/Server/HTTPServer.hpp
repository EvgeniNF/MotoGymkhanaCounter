#pragma once

#include "NetworkData.hpp"
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <functional>
#include <memory>
#include <string>

namespace server
{

class HTTPServer
{
  public:
    HTTPServer();

    ~HTTPServer() = default;

    HTTPServer(HTTPServer&) = delete;

    HTTPServer(HTTPServer&&) = delete;

    HTTPServer& operator=(const HTTPServer&) = delete;

  public:
    /**
     * @brief Set the Reset Function object
     * @param resetFunction
     */
    void setResetFunction(std::function<void()> const& resetFunction);
    /**
     * @brief Set the Reboot Function object
     * @param rebootFunction
     */
    void setRebootFunction(std::function<void()> const& rebootFunction);
    /**
     * @brief Web server initialization
     * @param data Data for transfer
     */
    void initializationWebServer(NetworkData* data);
    /**
     * @brief Update server initialization
     */
    void initializationUpdateServer();
    /**
     * @brief Handle http clients
     */
    void handleClinets();

  private:
    /**
     * @brief Handle sending start page
     */
    void handleHomePage();
    /**
     * @brief Handle sending error url page
     */
    void handleErrorPage();
    /**
     * @brief Handle sending timer date
     */
    void handleGetTimerTime();
    /**
     * @brief Handle sending access point name
     */
    void handleGetSoftApName();
    /**
     * @brief Handle sending wifi password
     */
    void handleGetPassword();
    /**
     * @brief Handle sending Ip address esp
     */
    void handleGetIpAddress();
    /**
     * @brief Handle sending host name for tcp connection
     */
    void handleGetTcpHostName();
    /**
     * @brief Handle sending timer state
     */
    void handleGetState();
    /**
     * @brief Handle timer reset signal from client
     */
    void handleReset();
    /**
     * @brief Handle reboot signal from client
     */
    void handleReboot();

  private:
    /// Http web server
    std::unique_ptr<ESP8266WebServer> m_webServer;
    /// Http update server
    std::unique_ptr<ESP8266HTTPUpdateServer> m_httpUpdateServer;
    /// Http port
    static constexpr unsigned int httpPort{80};
    /// Data for transive to clients
    NetworkData* m_transiveData{nullptr};
    /// Reset timer finction callback
    std::function<void()> m_resetFunction;
    /// Reboot finction callback
    std::function<void()> m_rebootFunction;
};

} // namespace server
