//
// Created by Alexander on 19.02.25.
//

#ifndef MEMORY_HPP
#define MEMORY_HPP
#include <assert.h>
#include <cstdint>


#define ZPG_BEG    0x0000 // - 0x00FF 256b
#define STACK_BEG  0x0100 // - 0x01FF 256b
#define INPUT_BEG  0x0200 // - 0x02FF 256b
#define PAGE3_BEG  0x0300 // - 0x03FF 256b
#define RAM_BEG    0x0400 // - 0xBFFF 48kb
#define OUTPUT_BEG 0xC000 // - 0xCFFF 4kb
#define ROM_BEG    0xD000 // - 0xDFFF 4kb
#define GRAPH_BEG  0xE000 // - 0xFFEF 8175b
#define VECTOR_BEG 0xFFF0 // - 0xFFFF 16b


class Memory {

public:



    Memory() {
        mem = new uint8_t[0x10000];
    }

    // ~Memory() {
    //     delete mem;
    // }


    uint8_t readByte(uint16_t addr) {
        assert(addr <= 0xFFFF);
        return mem[addr];
    }

    uint16_t readWord(uint16_t addr) {
        assert(addr+1 <= 0xFFFF);

        uint8_t LL = readByte(addr);
        uint8_t HH = readByte(addr + 1);

        return static_cast<uint16_t>(HH) << 8 | LL;
    }

    void writeByte(uint16_t addr, uint8_t value) {
        assert(addr <= 0xFFFF);
        mem[addr] = value;
    }

private:
    uint8_t* mem = nullptr;
};



#endif //MEMORY_HPP
