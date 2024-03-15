#ifndef SERVER_CONNECTION_H 
#define SERVER_CONNECTION_H
#include <ArduinoWebsockets.h>

class ServerConnection : public websockets::WebsocketsClient {
public:
    ServerConnection();
    bool connect();
    bool isConnected();
    bool sendBinary(const char* data, const size_t len);
    
    void loop();
    // uint16_t byte_rate();

private:
    static const char echo_org_ssl_ca_cert[] PROGMEM;
    // websockets::WebsocketsClient client; // Remove this since we are now inheriting
    // Other private members...
};

#endif