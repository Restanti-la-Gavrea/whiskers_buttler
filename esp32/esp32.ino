#include "server_connection.h"
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
#include "spi_master_communication.h"

using namespace websockets;
ServerConnection serverConnection;
SpiMasterCommunication spiCommunication;

const uint8_t ROBOT_UUID_SIZE = 16;
char robot_uuid[ROBOT_UUID_SIZE];
long long lastLoginClock = millis();

bool gotFirstUUID = false;
bool authentificatedWithUID = false;
bool transmitVideStreaming = true;

enum Commands{
    COMMAND_REGISTER = 0x00,
    COMMAND_LINK = 0x01,
    COMMAND_CONNECTION = 0x02,
    COMMAND_VIDEO_STREAMING = 0x04,
    COMMAND_FRAME = 0x05,
    COMMAND_MOTOR_POWER =0x06,
    COMMAND_DISTANCE_DATA=0x07,
    COMMAND_RING_EDGE_DATA= 0x08,
};


void try_login(){
    if(gotFirstUUID == false){
        serverConnection.sendBinary((char)COMMAND_REGISTER,NULL, 0);
    }else{
        serverConnection.sendBinary((char)COMMAND_REGISTER, robot_uuid, ROBOT_UUID_SIZE);
    }
}


void onMessageCallback(WebsocketsMessage message) {
    const uint8_t *s = (uint8_t *)message.c_str();
    uint8_t command = s[0];
    switch(command){
        case COMMAND_REGISTER:
            memcpy(robot_uuid, s+1, ROBOT_UUID_SIZE);
            gotFirstUUID = true;
            authentificatedWithUID = true;
            spiCommunication.addData(COMMAND_CONNECTION, 0x01);
        break;
        case COMMAND_LINK:
        break;
        case COMMAND_CONNECTION:
        break;
        case COMMAND_VIDEO_STREAMING:
            transmitVideStreaming = (bool) s[1];
        break;
        case COMMAND_FRAME:
        break;
        case COMMAND_MOTOR_POWER:
            spiCommunication.addData(s, (uint8_t) 3);
        break;

        default:
        break;
    }
}
void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        spiCommunication.addData(COMMAND_CONNECTION, 0x00);
        authentificatedWithUID = false;
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        // Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        // Serial.println("Got a Pong!");
    }
}

void initCamera(){
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.frame_size = FRAMESIZE_QVGA; //FRAMESIZE_96X96; //FRAMESIZE_QQVGA; //FRAMESIZE_SVGA;
    config.jpeg_quality = 63;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Msg: Camera init failed with error 0x%x", err);
        return;
    }
    sensor_t * s = esp_camera_sensor_get();
    // drop down frame size for higher initial frame rate
    s->set_framesize(s, FRAMESIZE_QVGA);
}


void setup() {
    Serial.begin(250000);
    serverConnection.onMessage(onMessageCallback);
    serverConnection.onEvent(onEventsCallback);
    serverConnection.connect();
    initCamera();
    spiCommunication.init();
}

long long sterge = millis();

void transmitSensorsData(){
    uint8_t *p =  spiCommunication.getReceivedData();
    const uint8_t *end = p + spiCommunication.getReceivedDataSize();
    // for(uint8_t * i = p; i < end; i ++){
    //     Serial.print(i[0]);
    //     Serial.print(" ");
    // }Serial.println();

    while(p < end){

        switch(p[0]){
            case COMMAND_MOTOR_POWER:
                serverConnection.sendBinary((const char *)p, 3);
                p+=3;
            break;
            case COMMAND_DISTANCE_DATA:
                serverConnection.sendBinary((const char *)p, 14);
                p+=14;
            break;
            case COMMAND_RING_EDGE_DATA:
                serverConnection.sendBinary((const char *)p, 2);
                p+=2;
            break;
            default:
                p++;
            break;
        }
    }
}

void loop() {
    //Eschange information with arduino nano
    spiCommunication.addData(0x01, 0x02);
    spiCommunication.communication();
    uint8_t *p =  spiCommunication.getReceivedData();
    const uint8_t *end = p + spiCommunication.getReceivedDataSize();
    for(uint8_t * i = p; i < end; i ++){
        Serial.print(i[0]);
        Serial.print(" ");
    }Serial.println();
    // delay(500);

    // //Check and restablish internet connection
    // if(!serverConnection.loop() ){
    //     return;
    // }

    // //Authentificating to Server
    // if(!authentificatedWithUID){
    //     if(millis()- lastLoginClock > 500 ){
    //        Serial.println("Try Login...");
    //        try_login();
    //        lastLoginClock = millis();
    //     }
    //     return;
    // }

    // //transmit Data getted from sensors
    // transmitSensorsData();

    // //Transmit Video Freame
    // if(transmitVideStreaming){
    //     camera_fb_t * fb = NULL;
    //     fb = esp_camera_fb_get();
    //     if(serverConnection.sendBinary(COMMAND_FRAME,(const char *)fb->buf, fb->len)){
    //         Serial.print("Transmitted ");
    //         Serial.print(fb->len);
    //         Serial.print(" : ");
    //         Serial.println(millis()- sterge);
    //         sterge = millis();
    //     } else{
    //         Serial.println("Not Transmitted");
    //     }
    //     esp_camera_fb_return(fb);
    // }
}