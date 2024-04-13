#include "spi_slave_connection.h"
#include <SPI.h>
#include <Arduino.h>

SpiSlaveConnection::SpiSlaveConnection(){}

void SpiSlaveConnection::init(){
    rxQueue.clear();
    txQueue.clear();
    bytes_to_transmit = 0;
    bytes_to_receive = 0;
    state = IDLE;
    // Initialize SPI pins and SPI control register as before
    pinMode(MISO, OUTPUT);
    pinMode(SS, INPUT_PULLUP);
    SPCR |= _BV(SPE); // Enable SPI
    SPI.attachInterrupt(); // Attach SPI interrupt

}
void SpiSlaveConnection::addData(const uint8_t command, const uint8_t data[],const uint8_t size){
    if(txQueue.getEmptySpace() < size + 1)
        return;
    txQueue.add(command);
    for(uint8_t i = 0 ; i < size ; i++)
        txQueue.add(data[i]);
    txQueue.commitData();
}
uint8_t SpiSlaveConnection::getData(uint8_t data[]){
    uint8_t n = 0;
    while(rxQueue.getNoElements() > 0){
        data[n++] = rxQueue.get();
    }
    return n;
}

void SpiSlaveConnection::interrupt() {
    if(state == IDLE ){
        if(SPDR == 0xFF){
            state = NO_BYTES;
            SPDR = bytes_to_transmit = txQueue.getNoElements();
            // Serial.println(bytes_to_transmit);
        }
    }else if(state == NO_BYTES){
        //No bytes to transfer state
        bytes_to_receive = SPDR;
        if (bytes_to_transmit > 0) {
            SPDR = txQueue.get();
            --bytes_to_transmit;
        }
        state = RECEIVING;
    }else{
        //Receiving state
        if (bytes_to_receive > 0) {
            rxQueue.add(SPDR);
            --bytes_to_receive;
        }
        if (bytes_to_transmit > 0) {
            SPDR = txQueue.get();
            --bytes_to_transmit;
        }
        if (bytes_to_receive == 0 && bytes_to_transmit == 0) {
            rxQueue.commitData();
            state = IDLE;
        }
    }
}