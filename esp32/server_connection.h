#ifndef SERVER_CONNECTION_H 
#define SERVER_CONNECTION_H
#include <ArduinoWebsockets.h>

#define CLUJ_WIFI
// #define PHONE_WIFI
// #define TURCENI_WIFI
using namespace websockets;
class ServerConnection : public websockets::WebsocketsClient {
public:
    ServerConnection();
    bool connect();
    bool isConnected();
    bool sendBinary(const uint8_t command, const char* data, const size_t len);
    bool sendBinary(const char* data, const size_t len);
    bool loop();

private:
    static const char echo_org_ssl_ca_cert[] PROGMEM;
    static const char ssid[] PROGMEM; 
    static const char password[] PROGMEM; 
    static const char websockets_connection_string[] PROGMEM;


    // static void onMessageCallback(WebsocketsMessage message) {
    //     Serial.print("Got Message: ");
    //     Serial.println(message.data());
    // }

    // static void onEventsCallback(ServerConnection& serverConnection, WebsocketsEvent event, String data) {
    //     if(event == WebsocketsEvent::ConnectionOpened) {
    //         Serial.println("Connnection Opened");
    //     } else if(event == WebsocketsEvent::ConnectionClosed) {
    //         Serial.println("Connnection Closed");
    //     } else if(event == WebsocketsEvent::GotPing) {
    //         // Serial.println("Got a Ping!");
    //     } else if(event == WebsocketsEvent::GotPong) {
    //         // Serial.println("Got a Pong!");
    //     }
    // }

};

#endif