#line 1 "D:\\Proiecte\\KittyKeeper\\esp32\\spi_master_commimocatopm.cpp"
#include "spi_master_communication.h"
#include <SPI.h>
#include <algorithm>
#include <Arduino.h>
const int8_t SPI_SCK_PIN = 14;
const int8_t SPI_MISO_PIN = 12;
const int8_t SPI_MOSI_PIN = 13;
const int8_t SPI_SS_PIN = 15;


SpiMasterCommunication::SpiMasterCommunication(/* args */){};

void SpiMasterCommunication::init(){
    rxSize = 0;
    txSize = 0;
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN,SPI_MOSI_PIN,SPI_SS_PIN);
    lastTransfer = micros();
}

void SpiMasterCommunication::addData(const uint8_t command,const uint8_t data){
    if(DATA_SIZE - txSize >= 2){
        txData[txSize++] = command;
        txData[txSize++] = data;
    }
}


void SpiMasterCommunication::addData(const uint8_t *data,const uint8_t size){
    if(DATA_SIZE - txSize >= size){
        for(uint8_t i = 0; i < size; i ++){
            txData[txSize++] = data[i];
        }
    }
    
}

uint8_t SpiMasterCommunication::transfer(uint8_t data){
    while(micros() - lastTransfer <= 16);
    uint8_t recData = SPI.transfer(data);
    lastTransfer = micros();
    return recData;
}

void SpiMasterCommunication::communication() {
    // Start SPI transaction
    SPI.beginTransaction(SPISettings(4000000, SPI_MSBFIRST, SPI_MODE0));
    transfer(0xFF);
    // Transmission of sizes and immediate data transfer
    rxSize = transfer(txSize);
    // Transmission of data
    uint8_t maxIterations = std::max(rxSize, txSize);
    rxSize = min(DATA_SIZE, rxSize);
    for (uint8_t i = 0; i < maxIterations; i++) {
        uint8_t txByte = (i < txSize) ? txData[i] : 0x00;  // Use dummy data if txSize exceeded
        uint8_t rxByte = transfer(txByte);

        if (i < rxSize) {
            rxData[i] = rxByte;  // Store received data if within rxSize
        }
    }
    

    // End SPI transaction
    SPI.endTransaction();

    // Reset transaction size
    txSize = 0;
}