#ifndef DECODER_H
#define DECODER_H


#include "Memory.hpp"


typedef enum {
    NOP,
    IMM,
    MOV,
    LOAD,
    STOR,
}OPType;

class Decoder {
public:
    u32 instruction;
    u8 OPCode;
    u8 arg0;
    u8 arg1;
    u8 arg2;

    void decode(u32& PC, Memory &mem) {

        OPCode = mem.read8(PC++);
        arg0 =   mem.read8(PC++);
        arg1 =   mem.read8(PC++);
        arg2 =   mem.read8(PC++);

    }
};

#endif
