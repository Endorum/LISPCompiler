#ifndef CPU_H
#define CPU_H

#include <assert.h>

#include "Decoder.hpp"
#include "Memory.hpp"

class CPU {
private:
    u32 Registers[8] = {0};
    u32 PC = 0;
    u32 CurrentInstr = 0;
    u8 Status = 0;
    u16 Page = 0;

public:

    CPU(u32 PC, Decoder& decoder, Memory& memory) : PC(PC), decoder(decoder), memory(memory) {}

    Memory memory;
    Decoder decoder;

    u32 readReg(u8 idx) {
        assert(idx <= 7 && "Invalid register index");
        return Registers[idx];
    }
    void writeReg(u8 idx, u32 value) {
        assert(idx <= 7 && "Invalid register index");
        Registers[idx] = value;
    }

    u32 getPC() const {
        return PC;
    }
    u8  getStatus() const {
        return Status;
    }

    void decode() {
        decoder.decode(PC, memory);
    }

    void execute();

    void _IMM();
    void _MOV();
    void _LOAD();
    void _STOR();
};

#endif