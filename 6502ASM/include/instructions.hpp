//
// Created by Alexander on 19.02.25.
//

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP
#include <string>

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


OPCODE mnemonicToType(std::string t);
#endif //INSTRUCTIONS_HPP
