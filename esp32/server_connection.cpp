#include "server_connection.h"
#include <WiFi.h>
#include <ESPmDNS.h>

using namespace websockets;
ServerConnection::ServerConnection()
  : WebsocketsClient() 
{
    this->setCACert(this->echo_org_ssl_ca_cert);
    this->waitClockMs = millis();
}

bool ServerConnection::connect() {
    if(WiFi.status() != WL_CONNECTED){
        if(millis() - waitClockMs > 3000){
            Serial.println("Try connecting to Wifi");
            WiFi.begin(this->ssid, this->password);
            waitClockMs = millis();
        }
        return false;
    }

    if(!this->available()){
        if(waitClockMs > 2000){
            waitClockMs = millis();
            Serial.println("Try connecting to server");
            WebsocketsClient::connect(this->websockets_connection_string);
        }
        return false;
    }
    return true;
}


bool ServerConnection::isConnected() {
    return WiFi.status() == WL_CONNECTED && this->available();
}

bool ServerConnection::sendBinary(const uint8_t command, const char* data, const size_t len){
    WebsocketsClient::streamBinary();
    WebsocketsClient::sendBinary((const char*)&command, 1);
    if(data != NULL)
        WebsocketsClient::sendBinary(data, len);
    return WebsocketsClient::end();
}

bool ServerConnection::sendBinary(const char* data, const size_t len){
    WebsocketsClient::streamBinary();
    if(data != NULL)
        WebsocketsClient::sendBinary(data, len);
    return  WebsocketsClient::end();
}

bool ServerConnection::loop(){
    if(!this->ping()){
        if(!this->connect())
            return false;
    }
    return this->poll();
}


const char ServerConnection::echo_org_ssl_ca_cert[] PROGMEM = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEZTCCA02gAwIBAgIQQAF1BIMUpMghjISpDBbN3zANBgkqhkiG9w0BAQsFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTIwMTAwNzE5MjE0MFoXDTIxMDkyOTE5MjE0MFow\n" \
"MjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxCzAJBgNVBAMT\n" \
"AlIzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuwIVKMz2oJTTDxLs\n" \
"jVWSw/iC8ZmmekKIp10mqrUrucVMsa+Oa/l1yKPXD0eUFFU1V4yeqKI5GfWCPEKp\n" \
"Tm71O8Mu243AsFzzWTjn7c9p8FoLG77AlCQlh/o3cbMT5xys4Zvv2+Q7RVJFlqnB\n" \
"U840yFLuta7tj95gcOKlVKu2bQ6XpUA0ayvTvGbrZjR8+muLj1cpmfgwF126cm/7\n" \
"gcWt0oZYPRfH5wm78Sv3htzB2nFd1EbjzK0lwYi8YGd1ZrPxGPeiXOZT/zqItkel\n" \
"/xMY6pgJdz+dU/nPAeX1pnAXFK9jpP+Zs5Od3FOnBv5IhR2haa4ldbsTzFID9e1R\n" \
"oYvbFQIDAQABo4IBaDCCAWQwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8E\n" \
"BAMCAYYwSwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5p\n" \
"ZGVudHJ1c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTE\n" \
"p7Gkeyxx+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEE\n" \
"AYLfEwEBATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2Vu\n" \
"Y3J5cHQub3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0\n" \
"LmNvbS9EU1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYf\n" \
"r52LFMLGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjANBgkqhkiG9w0B\n" \
"AQsFAAOCAQEA2UzgyfWEiDcx27sT4rP8i2tiEmxYt0l+PAK3qB8oYevO4C5z70kH\n" \
"ejWEHx2taPDY/laBL21/WKZuNTYQHHPD5b1tXgHXbnL7KqC401dk5VvCadTQsvd8\n" \
"S8MXjohyc9z9/G2948kLjmE6Flh9dDYrVYA9x2O+hEPGOaEOa1eePynBgPayvUfL\n" \
"qjBstzLhWVQLGAkXXmNs+5ZnPBxzDJOLxhF2JIbeQAcH5H0tZrUlo5ZYyOqA7s9p\n" \
"O5b85o3AM/OJ+CktFBQtfvBhcJVd9wvlwPsk+uyOy2HI7mNxKKgsBTt375teA2Tw\n" \
"UdHkhVNcsAKX1H7GNNLOEADksd86wuoXvg==\n" \
"-----END CERTIFICATE-----\n";

#ifdef CLUJ_WIFI
const char ServerConnection::ssid[] PROGMEM = "Faster-Faster";
const char ServerConnection::password[] PROGMEM = "voidmain()";
const char ServerConnection::websockets_connection_string[] PROGMEM = "ws://192.168.1.136:5000/connect/robot";
#elif defined(PHONE_WIFI)
const char ServerConnection::ssid[] PROGMEM = "BRG";
const char ServerConnection::password[] PROGMEM = "brgbrgbrg3";
const char ServerConnection::websockets_connection_string[] PROGMEM = "ws://192.168.58.41:5000/connect/robot";
#elif defined(TURCENI_WIFI)
const char ServerConnection::ssid[] PROGMEM = "DIGI_ea72ea";
const char ServerConnection::password[] PROGMEM = "5de30098";
const char ServerConnection::websockets_connection_string[] PROGMEM = "ws://192.168.1.4:5000/connect/robot";
#else
const char ServerConnection::ssid[] PROGMEM = "BRG-PC";
const char ServerConnection::password[] PROGMEM = "brgbrgbrg3";
const char ServerConnection::websockets_connection_string[] PROGMEM = "ws://192.168.137.1:5000/connect/robot";
#endif