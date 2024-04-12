
#include "spi_slave_connection.h"
SpiSlaveConnection spiConnection;

void setup(){
  Serial.begin(115200);
  spiConnection.init();
  Serial.println("Start:");
}

void loop()
{
    uint8_t v[100], n;
    n = spiConnection.getData(v);
    if(n != 0){
    //    for(int i = 0 ; i < n ; i ++){
    //         Serial.print((int)v[i]);
    //         Serial.print(" ");
    //     }Serial.println();
        uint8_t v[] = {9,11,12,13,14,15,16,17,18,19,20,21,22};
        spiConnection.addData((uint8_t)0x07,v,13);
    }
    
}
ISR(SPI_STC_vect){
    spiConnection.interrupt();
}
