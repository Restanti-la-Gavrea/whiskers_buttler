#ifndef SPI_SLAVE_CONNECTION
#define SPI_SLAVE_CONNECTION
#include <stdint.h>
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
        size++;
    }

    void commitData() {
        commitedSize = size;
    }

    uint8_t get() {
        if (commitedSize == 0) { // Check if there are no committed numbers.
            return 0; // Indicate an error or empty queue.
        }
        uint8_t x = buffer[base];
        commitedSize--;
        size--;
        base = (base + 1) % CAPACITY;
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

private:
    static constexpr uint8_t CAPACITY = 32;
    uint8_t buffer[CAPACITY];
    uint8_t base, size, commitedSize;
};

class SpiSlaveConnection
{
public:
    SpiSlaveConnection(/* args */);
    void init();
    void addData(const uint8_t command, const uint8_t data[],const uint8_t size);
    uint8_t getData(uint8_t data[]);
    void interrupt();
private:
    CommitQueue txQueue, rxQueue;
    uint8_t bytes_to_transmit, bytes_to_receive;
    // Define the TransmissionState enum
    enum TransmissionState {
        IDLE,           // Waiting for command or data
        RECEIVING,      // Currently receiving data
    } state; 
};



#endif