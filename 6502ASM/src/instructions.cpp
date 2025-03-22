//
// Created by Alexander on 19.02.25.
//


#include "../include/instructions.hpp"

#include <string>

std::string strToUpper(std::string str) {
    std::string res = "";

    for(int i=0;i<str.size();i++) {
        res += toupper(str[i]);
    }

    return res;
}

OPCODE mnemonicToType(std::string t) {


    std::string token = strToUpper(t);

    if(token == "ADC") return ADC;
    if(token == "AND") return AND;
    if(token == "ASL") return ASL;
    if(token == "BCC") return BCC;
    if(token == "BCS") return BCS;
    if(token == "BEQ") return BEQ;
    if(token == "BIT") return BIT;
    if(token == "BMI") return BMI;
    if(token == "BNE") return BNE;
    if(token == "BPL") return BPL;
    if(token == "BRK") return BRK;
    if(token == "BVC") return BVC;
    if(token == "BVS") return BVS;
    if(token == "CLC") return CLC;
    if(token == "CLD") return CLD;
    if(token == "CLI") return CLI;
    if(token == "CLV") return CLV;
    if(token == "CMP") return CMP;
    if(token == "CPX") return CPX;
    if(token == "CPY") return CPY;
    if(token == "DEC") return DEC;
    if(token == "DEX") return DEX;
    if(token == "DEY") return DEY;
    if(token == "EOR") return EOR;
    if(token == "INC") return INC;
    if(token == "INX") return INX;
    if(token == "INY") return INY;
    if(token == "JMP") return JMP;
    if(token == "JSR") return JSR;
    if(token == "LDA") return LDA;
    if(token == "LDX") return LDX;
    if(token == "LDY") return LDY;
    if(token == "LSR") return LSR;
    if(token == "NOP") return NOP;
    if(token == "ORA") return ORA;
    if(token == "PHA") return PHA;
    if(token == "PHP") return PHP;
    if(token == "PLA") return PLA;
    if(token == "PLP") return PLP;
    if(token == "ROL") return ROL;
    if(token == "ROR") return ROR;
    if(token == "RTI") return RTI;
    if(token == "RTS") return RTS;
    if(token == "SBC") return SBC;
    if(token == "SEC") return SEC;
    if(token == "SED") return SED;
    if(token == "SEI") return SEI;
    if(token == "STA") return STA;
    if(token == "STX") return STX;
    if(token == "STY") return STY;
    if(token == "TAX") return TAX;
    if(token == "TAY") return TAY;
    if(token == "TSX") return TSX;
    if(token == "TXA") return TXA;
    if(token == "TXS") return TXS;
    if(token == "TYA") return TYA;

    return NON;
}


uint8_t typeToOpcode(OPCODE type) {

}