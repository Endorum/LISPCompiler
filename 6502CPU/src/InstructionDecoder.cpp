//
// Created by Alexander on 19.02.25.
//

#include "../include/InstructionDecoder.hpp"

void InstructionDecoder::fetchOpcode() {
    opcode = bus.read(reg.getPC());
    reg.incPC();
}

OPCODE getOpCodeTypeByTable(uint8_t opcode) {

    constexpr OPCODE opcodeTable[256] = {
        BRK, ORA, NON, NON, NON, ORA, ASL, NON, PHP, ORA, ASL, NON, NON, ORA, ASL, NON,
        BPL, ORA, NON, NON, NON, ORA, ASL, NON, CLC, ORA, NON, NON, NON, ORA, ASL, NON,
        JSR, AND, NON, NON, BIT, AND, ROL, NON, PLP, AND, ROL, NON, BIT, AND, ROL, NON,
        BMI, AND, NON, NON, NON, AND, ROL, NON, SEC, AND, NON, NON, NON, AND, ROL, NON,
        RTI, EOR, NON, NON, NON, EOR, LSR, NON, PHA, EOR, LSR, NON, JMP, EOR, LSR, NON,
        BVC, EOR, NON, NON, NON, EOR, LSR, NON, CLI, EOR, NON, NON, NON, EOR, LSR, NON,
        RTS, ADC, NON, NON, NON, ADC, ROR, NON, PLA, ADC, ROR, NON, JMP, ADC, ROR, NON,
        BVS, ADC, NON, NON, NON, ADC, ROR, NON, SEI, ADC, NON, NON, NON, ADC, ROR, NON,
        NON, STA, NON, NON, STY, STA, STX, NON, DEY, NON, TXA, NON, STY, STA, STX, NON,
        BCC, STA, NON, NON, STY, STA, STX, NON, TYA, STA, TXS, NON, NON, STA, NON, NON,
        LDY, LDA, LDX, NON, LDY, LDA, LDX, NON, TAY, LDA, TAX, NON, LDY, LDA, LDX, NON,
        BCS, LDA, NON, NON, LDY, LDA, LDX, NON, CLV, LDA, TSX, NON, LDY, LDA, LDX, NON,
        CPY, CMP, NON, NON, CPY, CMP, DEC, NON, INY, CMP, DEX, NON, CPY, CMP, DEC, NON,
        BNE, CMP, NON, NON, NON, CMP, DEC, NON, CLD, CMP, NON, NON, NON, CMP, DEC, NON,
        CPX, SBC, NON, NON, CPX, SBC, INC, NON, INX, SBC, NOP, NON, CPX, SBC, INC, NON,
        BEQ, SBC, NON, NON, NON, SBC, INC, NON, SED, SBC, NON, NON, NON, SBC, INC, NON,
    };

    return opcodeTable[opcode];
}

ADDR_MODE getAddrModeByTable(uint8_t opcode) {

    constexpr ADDR_MODE addrModeTable[256] = {
        impl,  X_ind,  NO_AM,  NO_AM,  NO_AM,   zpg,   zpg,    NO_AM,   impl,  Imm,      A,       NO_AM,  NO_AM,   Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  NO_AM,   zpg_X, zpg_X,  NO_AM,   impl,  abs_Y,    NO_AM,   NO_AM,  NO_AM,   abs_X,   abs_X,  NO_AM,
        Abs,   X_ind,  NO_AM,  NO_AM,  zpg,     zpg,   zpg,    NO_AM,   impl,  Imm,      A,       NO_AM,  Abs,     Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  NO_AM,   zpg_X, zpg_X,  NO_AM,   impl,  abs_Y,    NO_AM,   NO_AM,  NO_AM,   abs_X,   abs_X,  NO_AM,
        impl,  X_ind,  NO_AM,  NO_AM,  NO_AM,   zpg,   zpg,    NO_AM,   impl,  Imm,      A,       NO_AM,  Abs,     Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  NO_AM,   zpg_X, zpg_X,  NO_AM,   impl,  abs_Y,    NO_AM,   NO_AM,  NO_AM,   abs_X,   abs_X,  NO_AM,
        impl,  X_ind,  NO_AM,  NO_AM,  NO_AM,   zpg,   zpg,    NO_AM,   impl,  Imm,      A,       NO_AM,  ind,     Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  NO_AM,   zpg_X, zpg_X,  NO_AM,   impl,  abs_Y,    NO_AM,   NO_AM,  NO_AM,   abs_X,   abs_X,  NO_AM,
        NO_AM, X_ind,  NO_AM,  NO_AM,  zpg,     zpg,   zpg,    NO_AM,   impl,  NO_AM,    impl,    NO_AM,  Abs,     Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  zpg_X,   zpg_X, zpg_Y,  NO_AM,   impl,  abs_Y,    impl,    NO_AM,  NO_AM,   abs_X,   NO_AM,  NO_AM,
        Imm,   X_ind,  Imm,    NO_AM,  zpg,     zpg,   zpg,    NO_AM,   impl,  Imm,      impl,    NO_AM,  Abs,     Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  zpg_X,   zpg_X, zpg_Y,  NO_AM,   impl,  abs_Y,    impl,    NO_AM,  abs_X,   abs_X,   abs_Y,  NO_AM,
        Imm,   X_ind,  NO_AM,  NO_AM,  zpg,     zpg,   zpg,    NO_AM,   impl,  Imm,      impl,    NO_AM,  Abs,     Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  NO_AM,   zpg_X, zpg_X,  NO_AM,   impl,  abs_Y,    NO_AM,   NO_AM,  NO_AM,   abs_X,   abs_X,  NO_AM,
        Imm,   X_ind,  NO_AM,  NO_AM,  zpg,     zpg,   zpg,    NO_AM,   impl,  Imm,      impl,    NO_AM,  Abs,     Abs,     Abs,    NO_AM,
        rel,   ind_Y,  NO_AM,  NO_AM,  NO_AM,   zpg_X, zpg_X,  NO_AM,   impl,  abs_Y,    NO_AM,   NO_AM,  NO_AM,   abs_X,   abs_X,  NO_AM,
    };

    return addrModeTable[opcode];
}

void InstructionDecoder::decode() {
    opcode_type = getOpCodeTypeByTable(opcode);
    addr_mode = getAddrModeByTable(opcode);
}

std::string InstructionDecoder::str_opcode_type() {
    switch (opcode_type) {
        default: return "UNKNOWN";
        case NON: return "NON";
        case ADC: return "ADC";
        case AND: return "AND";
        case ASL: return "ASL";
        case BCC: return "BCC";
        case BCS: return "BCS";
        case BEQ: return "BEQ";
        case BIT: return "BIT";
        case BMI: return "BMI";
        case BNE: return "BNE";
        case BPL: return "BPL";
        case BRK: return "BRK";
        case BVC: return "BVC";
        case BVS: return "BVS";
        case CLC: return "CLC";
        case CLD: return "CLD";
        case CLI: return "CLI";
        case CLV: return "CLV";
        case CMP: return "CMP";
        case CPX: return "CPX";
        case CPY: return "CPY";
        case DEC: return "DEC";
        case DEX: return "DEX";
        case DEY: return "DEY";
        case EOR: return "EOR";
        case INC: return "INC";
        case INX: return "INX";
        case INY: return "INY";
        case JMP: return "JMP";
        case JSR: return "JSR";
        case LDA: return "LDA";
        case LDX: return "LDX";
        case LDY: return "LDY";
        case LSR: return "LSR";
        case NOP: return "NOP";
        case ORA: return "ORA";
        case PHA: return "PHA";
        case PHP: return "PHP";
        case PLA: return "PLA";
        case PLP: return "PLP";
        case ROL: return "ROL";
        case ROR: return "ROR";
        case RTI: return "RTI";
        case RTS: return "RTS";
        case SBC: return "SBC";
        case SEC: return "SEC";
        case SED: return "SED";
        case SEI: return "SEI";
        case STA: return "STA";
        case STX: return "STX";
        case STY: return "STY";
        case TAX: return "TAX";
        case TAY: return "TAY";
        case TSX: return "TSX";
        case TXA: return "TXA";
        case TXS: return "TXS";
        case TYA: return "TYA";
    }
}

std::string InstructionDecoder::str_addr_mode() {
    switch (addr_mode) {
        default: return "UNKNOWN";
        case A: return "A";
        case Abs: return "Abs";
        case abs_X: return "abs_X";
        case abs_Y: return "abs_Y";
        case Imm: return "Imm";
        case impl: return "impl";
        case ind: return "ind";
        case X_ind: return "X_ind";
        case ind_Y: return "ind_Y";
        case rel: return "rel";
        case zpg: return "zpg";
        case zpg_X: return "zpg_X";
        case zpg_Y: return "zpg_Y";
    }
}
