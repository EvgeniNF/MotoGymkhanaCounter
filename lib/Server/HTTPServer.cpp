#include "HTTPServer.hpp"

namespace server {

HTTPServer::HTTPServer() {
    this->m_webServer = std::make_unique<ESP8266WebServer>();
    this->m_httpUpdateServer = std::make_unique<ESP8266HTTPUpdateServer>();
}

HTTPServer::HTTPServer(const std::string& updateHostname) {
    this->m_webServer = std::make_unique<ESP8266WebServer>();
    this->m_httpUpdateServer = std::make_unique<ESP8266HTTPUpdateServer>();
    this->m_updateHostName = updateHostname;
}

void HTTPServer::initializationWebServer(NetworkData* data) {
    this->m_transiveData = data;
    auto handleHomePageFunction = std::bind(&HTTPServer::handleHomePage,
                                    this);
    auto handleErrorPageFunction = std::bind(&HTTPServer::handleErrorPage,
                                    this);
    this->m_webServer->on("/", handleHomePageFunction);
    this->m_webServer->onNotFound(handleErrorPageFunction);
    this->m_webServer->begin(HTTPServer::httpPort);
    this->m_httpUpdateServer->setup(this->m_webServer.get());
}
    
void HTTPServer::initializationUpdateServer() {
    MDNS.begin("update", WiFi.softAPIP());
    MDNS.addService("http", "tcp", HTTPServer::httpPort);
}

void HTTPServer::handleHomePage() {
    std::string req {"Moto-Gymkhana counter\n"};
    req += "Timer time: " + std::to_string(*this->m_transiveData->timerTime) + "\n";
    req += "Time on esp: " + std::to_string(*this->m_transiveData->timeOnEsp) + "\n";
    switch (*this->m_transiveData->espStatus){
    case 0:
        req += "Status: Don't call init functions\n";
        break;
    case 1:
        req += "Status: Call only display init function\n";
        break;
    case 2:
        req += "Status: Call only wifi init function\n";
        break;
    case 3:
        req += "Status: Ready for start count\n";
        break;
    case 4:
        req += "Status: Count\n";
        break;
    case 5:
        req += "Status: Timer stoped\n";
        break;
    }
    req += "IP address: " + std::string(WiFi.softAPIP().toString().c_str()) + "\n";
    req += "Access point name: " + std::string(WiFi.softAPSSID().c_str()) + "\n";
    req += "Password: " + std::string(WiFi.softAPPSK().c_str()) + "\n";
    req += "TCP host name: " + *this->m_transiveData->hostName + "\n";
    
    this->m_webServer->send(200, "text/plain", req.c_str());
}

void HTTPServer::handleErrorPage(){
    this->m_webServer->send(404, "text/plain", "Error not found this uri!");
}

void HTTPServer::handleClinets(){
    this->m_webServer->handleClient();
    MDNS.update();
}

}