#include "HTTPServer.hpp"
#include "../../include/configs.hpp"
#include "WebPage.hpp"
#include <Arduino.h>

namespace server
{

HTTPServer::HTTPServer()
{
    m_webServer = std::make_unique<ESP8266WebServer>();
    m_httpUpdateServer = std::make_unique<ESP8266HTTPUpdateServer>();
}

void HTTPServer::initializationWebServer(NetworkData* data)
{
    m_transiveData = data;
    auto handleHomePageFunction = std::bind(&HTTPServer::handleHomePage, this);
    auto handleErrorPageFunction =
        std::bind(&HTTPServer::handleErrorPage, this);
    m_webServer->on("/", handleHomePageFunction);
    m_webServer->on("/timerTime",
                    std::bind(&HTTPServer::handleGetTimerTime, this));
    m_webServer->on("/softApName",
                    std::bind(&HTTPServer::handleGetSoftApName, this));
    m_webServer->on("/password",
                    std::bind(&HTTPServer::handleGetPassword, this));
    m_webServer->on("/ipAddress",
                    std::bind(&HTTPServer::handleGetIpAddress, this));
    m_webServer->on("/tcpHostName",
                    std::bind(&HTTPServer::handleGetTcpHostName, this));
    m_webServer->on("/state", std::bind(&HTTPServer::handleGetState, this));
    m_webServer->on("/reset", std::bind(&HTTPServer::handleReset, this));
    m_webServer->on("/reboot", std::bind(&HTTPServer::handleReboot, this));
    m_webServer->onNotFound(handleErrorPageFunction);
    m_webServer->begin(HTTPServer::httpPort);
    m_httpUpdateServer->setup(m_webServer.get());
}

void HTTPServer::initializationUpdateServer()
{
    MDNS.begin(configs::network_settings::updateUrlName, WiFi.softAPIP());
    MDNS.addService("http", "tcp", HTTPServer::httpPort);
}

void HTTPServer::handleHomePage()
{
    m_webServer->send(200, "text/html", webpage);
}

void HTTPServer::handleErrorPage()
{
    this->m_webServer->send(404, "text/plain", "Error not found this uri!");
}

void HTTPServer::handleClinets()
{
    this->m_webServer->handleClient();
    MDNS.update();
}

void HTTPServer::handleGetTimerTime()
{
    std::string timeFormat;
    unsigned long int timerTime = *m_transiveData->timerTime;
    const size_t arrayTimeConvert = 4;
    const int timeConvert[arrayTimeConvert] = {3'600'000, 60'000, 1'000, 1};
    for (size_t i = 0; i < arrayTimeConvert; ++i)
    {
        int timeForDisplay = static_cast<int>(timerTime / timeConvert[i]);
        timerTime = timerTime - (timeForDisplay * timeConvert[i]);
        timeFormat += std::to_string(timeForDisplay);
        if (i != arrayTimeConvert - 1)
        {
            timeFormat += ":";
        }
    }
    m_webServer->send(200, "text/plane", timeFormat.c_str());
}

void HTTPServer::handleGetSoftApName()
{
    m_webServer->send(200, "text/plane", WiFi.softAPSSID().c_str());
}

void HTTPServer::handleGetPassword()
{
    m_webServer->send(200, "text/plane", WiFi.softAPPSK().c_str());
}

void HTTPServer::handleGetIpAddress()
{
    m_webServer->send(200, "text/plane", WiFi.softAPIP().toString().c_str());
}

void HTTPServer::handleGetTcpHostName()
{
    m_webServer->send(200, "text/plane",
                      configs::network_settings::tcpHostName);
}

void HTTPServer::handleGetState()
{
    switch (*m_transiveData->espStatus)
    {
        case 0:
            m_webServer->send(200, "text/plane", "Don't call init functions");
            break;
        case 1:
            m_webServer->send(200, "text/plane",
                              "Call only display init function");
            break;
        case 2:
            m_webServer->send(200, "text/plane",
                              "Call only wifi init function");
            break;
        case 3:
            m_webServer->send(200, "text/plane", "Ready for start count");
            break;
        case 4:
            m_webServer->send(200, "text/plane", "Count");
            break;
        case 5:
            m_webServer->send(200, "text/plane", "Timer stoped");
            break;
    }
}

void HTTPServer::handleReset()
{
    m_webServer->send(200, "text/plane", "Ok");
    m_resetFunction();
}

void HTTPServer::handleReboot()
{
    m_webServer->send(200, "text/plane", "Ok");
    m_rebootFunction();
}

void HTTPServer::setResetFunction(std::function<void()> const& resetFunction)
{
    m_resetFunction = resetFunction;
}

void HTTPServer::setRebootFunction(std::function<void()> const& rebootFunction)
{
    m_rebootFunction = rebootFunction;
}

} // namespace server
