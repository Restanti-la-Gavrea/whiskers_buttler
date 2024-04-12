#line 1 "D:\\Proiecte\\KittyKeeper\\esp32\\spi_master_commimocatopm.cpp"
#include "spi_master_communication.h"
#include <SPI.h>
#include <algorithm>
#include <Arduino.h>


SpiMasterCommunication::SpiMasterCommunication(/* args */){};

void SpiMasterCommunication::init(){
    rxSize = 0;
    txSize = 0;
    SPI.begin(14, 12,13,15);
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

void SpiMasterCommunication::communication(){
    //transmission of sizes
    SPI.beginTransaction(SPISettings(4000000, SPI_MSBFIRST, SPI_MODE0));
    rxSize = SPI.transfer(txSize);
    delay(1);
    uint8_t maxIterations = std::max(rxSize, txSize);
    //Transmission of data
    for(uint8_t i = 0 ; i < maxIterations; i ++){
        uint8_t txByte; 
        if(i < txSize)
            txByte = txData[i];
        else
            txByte = 0x00;// Default to dummy data
        uint8_t rxByte = SPI.transfer(txByte);
        delay(1);
        if(i < rxSize){
            rxData[i] = rxByte;
        }
    }
    SPI.endTransaction();
    txSize = 0;
    
}