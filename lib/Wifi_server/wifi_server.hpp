#ifndef WIFI_SERVER_WIFI_SERVER_HPP
#define WIFI_SERVER_WIFI_SERVER_HPP

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

namespace wifi_server{

class Wifi_server : public ESP8266WebServer {
  
 public:

  Wifi_server() = default; 

  Wifi_server(const String& _ssid, const String& _pass, IPAddress local_ip, IPAddress gateway, IPAddress subnet, int port);
  
  Wifi_server(const Wifi_server&) = delete;

  Wifi_server(const Wifi_server&&) = delete;

  ~Wifi_server() = default;

  void init_wifi(const String& _ssid, const String& _pass, IPAddress local_ip, IPAddress gateway, IPAddress subnet, int port);
  
  void send_request(String mess);

 private:

  enum class Status_wifi : unsigned short int{
    READY = 0,
    SEND_MESSAGE = 1,
    SEND_ERROR = 2,
    ERROR = 3,
    NOT_INIT = 4  
  };
  
 private:

  static void handle_resive_data();

  static void handle_not_found();

 private:

  ESP8266HTTPUpdateServer httpUpdater;
  static Status_wifi status;
};

}


#endif // WIFI_SERVER_WIFI_SERVER_HPP