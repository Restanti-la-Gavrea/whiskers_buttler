#line 1 "D:\\Proiecte\\KittyKeeper\\esp32\\spi_master_communication.h"
#ifndef SPI_MASTER_COMMUNICATION
#define SPI_MASTER_COMMUNICATION

#include <cstdint>

class SpiMasterCommunication
{
public:
    SpiMasterCommunication();
    void addData(const uint8_t command, const uint8_t data);
    void addData(const uint8_t *data,const uint8_t size);
    void init();
    void communication();
    uint8_t* getReceivedData() {return rxData; }
    uint8_t getReceivedDataSize() const { return rxSize; }
private:
    static constexpr uint8_t DATA_SIZE = 32; 
    uint8_t txData[DATA_SIZE];
    uint8_t rxData[DATA_SIZE];
    uint8_t rxSize, txSize;
};



#endif