//
// Created by Alexander on 19.02.25.
//

#ifndef INSTRUCTIONDECODER_HPP
#define INSTRUCTIONDECODER_HPP
#include <string>

#include "BusInterface.hpp"
#include "RegUnit.hpp"

typedef enum{
    NON,ADC,AND,ASL,BCC,BCS,BEQ,BIT,BMI,BNE,BPL,BRK,BVC,BVS,CLC,CLD,
    CLI,CLV,CMP,CPX,CPY,DEC,DEX,DEY,EOR,INC,INX,INY,JMP,JSR,LDA,LDX,
    LDY,LSR,NOP,ORA,PHA,PHP,PLA,PLP,ROL,ROR,RTI,RTS,SBC,SEC,SED,SEI,
    STA,STX,STY,TAX,TAY,TSX,TXA,TXS,TYA
}OPCODE;

typedef enum{
    NO_AM,
    A,
    Abs,
    abs_X,
    abs_Y,
    Imm,
    impl,
    ind,
    X_ind,
    ind_Y,
    rel,
    zpg,
    zpg_X,
    zpg_Y
}ADDR_MODE;

class InstructionDecoder {
public:
    InstructionDecoder(BusInterface& bus, RegUnit& reg)
        : bus(bus), reg(reg), opcode(0), opcode_type(NON), addr_mode(NO_AM) {
    }

    void fetchOpcode(); // fetches the next opcode from memory
    void decode(); // decodes the opcode into opcode_type and addr_mode

    uint8_t getOpcode() const { return opcode; }
    OPCODE getOpcodeType() const { return opcode_type; }
    ADDR_MODE getAddrMode() const { return addr_mode; }

    std::string str_opcode_type();
    std::string str_addr_mode();

private:
    BusInterface& bus;
    RegUnit& reg;

    uint8_t opcode;

    OPCODE opcode_type;
    ADDR_MODE addr_mode;
};



#endif //INSTRUCTIONDECODER_HPP
