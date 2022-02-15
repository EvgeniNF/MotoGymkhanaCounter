#include "HTTPServer.hpp"

namespace server {

HTTPServer::HTTPServer() {
    this->webServer = std::make_unique<ESP8266WebServer>();
    this->httpUpdateServer = std::make_unique<ESP8266HTTPUpdateServer>();
}

HTTPServer::HTTPServer(const std::string& updateHostname) {
    this->webServer = std::make_unique<ESP8266WebServer>();
    this->httpUpdateServer = std::make_unique<ESP8266HTTPUpdateServer>();
    this->_updateHostname = updateHostname;
}

void HTTPServer::initializationWebServer(NetworkData* data) {
    this->_data = data;
    auto handleHomePageFunction = std::bind(&HTTPServer::handleHomePage,
                                    this);
    auto handleErrorPageFunction = std::bind(&HTTPServer::handleErrorPage,
                                    this);
    this->webServer->on("/", handleHomePageFunction);
    this->webServer->onNotFound(handleErrorPageFunction);
    this->webServer->begin(this->http_port);
    this->httpUpdateServer->setup(this->webServer.get());
}
    
void HTTPServer::initializationUpdateServer() {
    MDNS.begin("update", WiFi.softAPIP());
    MDNS.addService("http", "tcp", this->http_port);
}

void HTTPServer::handleHomePage() {
    std::string req {"Moto-Gymkhana counter\n"};
 //   req += "Timer time: " + std::to_string(*this->_data->timer_time) + "\n";
 //   req += "Time on esp: " + std::to_string(*this->_data->time_on_esp) + "\n";
    switch (*this->_data->m_espStatus){
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
//    req += "TCP host name: " + *this->_data->host_name + "\n";
    
    this->webServer->send(200, "text/plain", req.c_str());
}

void HTTPServer::handleErrorPage(){
    this->webServer->send(404, "text/plain", "Error not found this uri!");
}

void HTTPServer::handleClinets(){
    this->webServer->handleClient();
    MDNS.update();
}

}