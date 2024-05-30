#ifndef SPI_SLAVE_CONNECTION
#define SPI_SLAVE_CONNECTION
#include <stdint.h>
#include <Arduino.h>


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

template <uint8_t CAPACITY>
class CommitQueue {
public:
    CommitQueue() : base(0), size(0), commitedSize(0) {}

    void clear() {
        size = 0;
        base = 0;
        commitedSize = 0;
    }

    void add(uint8_t number) {
        if (size == CAPACITY) { // Check if the queue is full.
            return; // Queue is full, do nothing.
        }
        buffer[(base + size) % CAPACITY] = number;
        noInterrupts();
        size++;
        interrupts();
    }

    void commitData() {
        noInterrupts();
        commitedSize = size;
        interrupts();
    }

    uint8_t get() {
        if (commitedSize == 0) { // Check if there are no committed numbers.
            return 0; // Indicate an error or empty queue.
        }
        uint8_t x = buffer[base];
        noInterrupts();
        commitedSize--;
        size--;
        base = (base + 1) % CAPACITY;
        interrupts();
        return x;
    }

    // Returns the number of committed elements in the queue.
    uint8_t getNoElements() {
        return commitedSize;
    }

    // Returns the number of empty spaces left in the queue.
    uint8_t getEmptySpace() {
        return CAPACITY - size;
    }
    void print(){
        Serial.print(commitedSize);
        Serial.print(' ');
        Serial.println(size);
        for(int i = 0 ;i < size; i ++){
            Serial.print(buffer[(base+ i) % CAPACITY]);
            Serial.print(' ');
        }Serial.println();
    }

private:
    uint8_t buffer[CAPACITY];
    uint8_t base, size, commitedSize;
};

class SpiSlaveConnection
{
public:
    static constexpr uint8_t BUFFER_CAPACTITY = 32;
    SpiSlaveConnection(/* args */);
    void init();
    void addData(const uint8_t command, const uint8_t data);
    void addData(const uint8_t command, const uint8_t data[],const uint8_t size);
    uint8_t getData(uint8_t data[]);
    void interrupt();
private:
    CommitQueue<BUFFER_CAPACTITY> txQueue, rxQueue;
    volatile uint8_t bytes_to_transmit, bytes_to_receive;
    // Define the TransmissionState enum
    volatile enum TransmissionState {
        IDLE,           // Waiting for command or data
        NO_BYTES,
        RECEIVING,      // Currently receiving data
    } state; 

    void commit();
};



#endif