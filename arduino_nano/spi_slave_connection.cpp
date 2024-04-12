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
    SPDR = 0x00; // Ensure SPDR is clear at start
    SPCR |= _BV(SPE); // Enable SPI
    SPI.attachInterrupt(); // Attach SPI interrupt

}
void SpiSlaveConnection::addData(const uint8_t command, const uint8_t data[],const uint8_t size){
    if(txQueue.getEmptySpace() < size + 1)
        return;
    txQueue.add(command);
    for(uint8_t i = 0 ; i < size ; i++)
        txQueue.add(data[i]);
    cli();
    txQueue.commitData();
    if(state == IDLE)
        bytes_to_transmit = SPDR = txQueue.getNoElements();
    sei();
}
uint8_t SpiSlaveConnection::getData(uint8_t data[]){
    uint8_t n = 0;
    while(rxQueue.getNoElements() > 0){
        data[n++] = rxQueue.get();
    }
    return n;
}

void SpiSlaveConnection::interrupt(){

    if(state == IDLE){
        //Store number of bytes to receive
        bytes_to_receive = SPDR;
        SPDR = txQueue.get();
        state = RECEIVING;
        return;
    }

    if(state == RECEIVING){
        //Continue Receiving
        if(bytes_to_receive > 0){
            rxQueue.add(SPDR);
            bytes_to_receive--;
        }

        //Continue Transmitting
        if(bytes_to_transmit > 0){
            SPDR = txQueue.get();
            bytes_to_transmit--;
        }else{
            SPDR = 0;
        }

        //Check if the transaction is complete
        if(bytes_to_receive == 0 && bytes_to_transmit == 0){
            state = IDLE;
            bytes_to_transmit = SPDR = txQueue.getNoElements();
            rxQueue.commitData();
        }
        
        return;
    }
}