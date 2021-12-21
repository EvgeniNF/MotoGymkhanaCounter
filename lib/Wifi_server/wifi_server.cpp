#include "wifi_server.hpp"

#include <ESP8266mDNS.h>



namespace wifi_server{

Wifi_server::Wifi_server(const String& _ssid,
                        const String& _pass, 
                        IPAddress local_ip, 
                        IPAddress gateway, 
                        IPAddress subnet, 
                        int port) { 
  this->begin(port);    
  WiFi.softAP(_ssid, _pass);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  this->on("/data", handle_resive_data);
  this->onNotFound(handle_not_found);
  this->begin();
  status = Status_wifi::READY;
}

void Wifi_server::handle_resive_data() {
    status = Status_wifi::SEND_MESSAGE;  
}

void Wifi_server::handle_not_found(){
  status = Status_wifi::ERROR;    
}

void Wifi_server::send_request(String mess){
  this->handleClient();
 //http.handleClient();
  MDNS.update();
  if (status == Status_wifi::SEND_MESSAGE){
    this->send(200, "text/plain", mess);  
    status = Status_wifi::READY;  
  } else if (status == Status_wifi::ERROR){
    this->send(404, "text/plain", "ERROR, not found this henler!");
    status = Status_wifi::READY;
  }
}

void Wifi_server::init_wifi(const String& _ssid, const String& _pass, 
                 IPAddress local_ip, IPAddress gateway, 
                 IPAddress subnet, int port){
  this->begin(port);    
  WiFi.softAP(_ssid, _pass);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  this->on("/data", handle_resive_data);
  this->onNotFound(handle_not_found);
  this->begin();

  MDNS.begin("update");
  httpUpdater.setup(this);
  MDNS.addService("http", "tcp", 80);

  status = Status_wifi::READY;
}

Wifi_server::Status_wifi Wifi_server::status = Wifi_server::Status_wifi::NOT_INIT;

}