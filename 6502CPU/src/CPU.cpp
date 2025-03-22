//
// Created by Alexander on 19.02.25.
//

#include "../include/CPU.hpp"

#include <sys/types.h>


void CPU::execute() {
    printf("%02X : ", instruction_decoder.getOpcode());
    switch (instruction_decoder.getOpcodeType()) {
        default: printf("Unknown Instruction\n"); return;
        case NON:           break;
        case ADC: _adc(); break;
        case AND: _and(); break;
        case ASL: _asl(); break;
        case BCC: _bcc(); break;
        case BCS: _bcs(); break;
        case BEQ: _beq(); break;
        case BIT: _bit(); break;
        case BMI: _bmi(); break;
        case BNE: _bne(); break;
        case BPL: _bpl(); break;
        case BRK: _brk(); break;
        case BVC: _bvc(); break;
        case BVS: _bvs(); break;
        case CLC: _clc(); break;
        case CLD: _cld(); break;
        case CLI: _cli(); break;
        case CLV: _clv(); break;
        case CMP: _cmp(); break;
        case CPX: _cpx(); break;
        case CPY: _cpy(); break;
        case DEC: _dec(); break;
        case DEX: _dex(); break;
        case DEY: _dey(); break;
        case EOR: _eor(); break;
        case INC: _inc(); break;
        case INX: _inx(); break;
        case INY: _iny(); break;
        case JMP: _jmp(); break;
        case JSR: _jsr(); break;
        case LDA: _lda(); break;
        case LDX: _ldx(); break;
        case LDY: _ldy(); break;
        case LSR: _lsr(); break;
        case NOP: _nop(); break;
        case ORA: _ora(); break;
        case PHA: _pha(); break;
        case PHP: _php(); break;
        case PLA: _pla(); break;
        case PLP: _plp(); break;
        case ROL: _rol(); break;
        case ROR: _ror(); break;
        case RTI: _rti(); break;
        case RTS: _rts(); break;
        case SBC: _sbc(); break;
        case SEC: _sec(); break;
        case SED: _sed(); break;
        case SEI: _sei(); break;
        case STA: _sta(); break;
        case STX: _stx(); break;
        case STY: _sty(); break;
        case TAX: _tax(); break;
        case TAY: _tay(); break;
        case TSX: _tsx(); break;
        case TXA: _txa(); break;
        case TXS: _txs(); break;
        case TYA: _tya(); break;
    }
}

void CPU::reset() {
    uint16_t resetVector = mem.readWord(0xFFFC);

    reg_unit.setPC(resetVector);

    reg_unit.setSP(0xFF);

    reg_unit.unsetDF();

    reg_unit.setIF();

    IRQ = false;
    NMI = false;

    if (debug)
        printf("CPU Reset: PC set to 0x%04X\n",resetVector);
}

void CPU::showCPU() {
    printf("CPU----------------------------\n");
    printf(" A:%02X   X:%02X  Y:%02X\n",reg_unit.getA(),reg_unit.getX(),reg_unit.getY());
    printf("SR:%02X  SP:%02X\n",reg_unit.getSR(),reg_unit.getSP());
    printf("PC:%04X\n",reg_unit.getPC());
}

void CPU::showStack() {
    printf("STACK--------------------------\n");
    for(int i=0;i<0x100;i++) {
        if(i % 16 == 0) printf("\n");
        printf("%02X ", mem.readByte(i | 0x0100));
    }
    printf("\n");
}

void CPU::showZpg(uint8_t page) {
    printf("PAGE%d-------------------------\n",page);
    for(int i=0;i<0x100;i++) {
        if(i % 16 == 0) printf("\n");
        printf("%02X ", mem.readByte(i | (static_cast<uint16_t>(page)<<8)));
    }
    printf("\n");
}

void CPU::pushByte(uint8_t value) {
    mem.writeByte(reg_unit.getSP() | 0x0100, value);
    reg_unit.decSP();
}

uint8_t CPU::popByte() {
    reg_unit.incSP();
    return mem.readByte(reg_unit.getSP() | 0x0100);
}

void CPU::pushWord(uint16_t value) {
}

uint16_t CPU::popWord() {
    return 0x0;
}

void CPU::impliedAddressing() { }

uint8_t CPU::immediateAddressing() {
    return getNextByte();
}

uint16_t CPU::absoluteAddressing() {
    const uint8_t LL = getNextByte();
    const uint8_t HH = getNextByte();
    const uint16_t addr = static_cast<uint16_t>(HH) << 8 | LL;
    return addr;
}

uint16_t CPU::zpgAddressing() {
    const uint8_t LL = getNextByte();
    return LL;
}

uint16_t CPU::abs_XAddressing() {
    const uint8_t LL = getNextByte();
    const uint8_t HH = getNextByte();
    const uint16_t addr = static_cast<uint16_t>(HH) << 8 | LL;
    const uint16_t indexedAddr = addr + reg_unit.getX();

    if( (addr & 0xFF00) != (indexedAddr & 0xFF00) )
        timing_unit.addCycle();

    return indexedAddr;
}

uint16_t CPU::abs_YAddressing() {
    const uint8_t LL = getNextByte();
    const uint8_t HH = getNextByte();
    const uint16_t addr = static_cast<uint16_t>(HH) << 8 | LL;
    const uint16_t indexedAddr = addr + reg_unit.getY();

    if( (addr & 0xFF00) != (indexedAddr & 0xFF00) )
        timing_unit.addCycle();

    return indexedAddr;
}

uint16_t CPU::zpg_XAddressing() {
    uint8_t LL = getNextByte();
    return LL + reg_unit.getX();
}

uint16_t CPU::zpg_YAddressing() {
    uint8_t LL = getNextByte();
    return LL + reg_unit.getY();
}

uint16_t CPU::indirectAddressing() {
    const uint8_t LL = getNextByte();
    const uint8_t HH = getNextByte();
    const uint16_t addr = (static_cast<uint16_t>(HH) << 8) | LL;

    uint8_t low = mem.readByte(addr);
    uint8_t high = mem.readByte( (addr & 0xFF00) | ((addr + 1) & 0x00FF) );

    return static_cast<uint16_t>(high) << 8 | low;
}

uint16_t CPU::X_indAddressing() {
    const uint8_t LL = getNextByte();
    const uint16_t addr = (LL + reg_unit.getX()) & 0xFF; // Stay within page

    return mem.readWord(addr);
}

uint16_t CPU::ind_YAddressing() {
    const uint8_t LL = getNextByte();
    const uint16_t baseAddr = mem.readWord(LL);
    const uint16_t indexedAddr = baseAddr + reg_unit.getY();

    if ((baseAddr & 0xFF00) != (indexedAddr & 0xFF00))
        timing_unit.addCycle();


    return indexedAddr;
}

uint16_t CPU::relativeAddressing() {
    const auto offset = static_cast<int8_t>(getNextByte());
    const uint16_t baseAddr = reg_unit.getPC();
    const uint16_t targetAddr = static_cast<uint16_t>(baseAddr + offset);

    if ((baseAddr & 0xFF00) != (targetAddr & 0xFF00)) {
        timing_unit.addCycle();  // Extra cycle if crossing a page
    }

    return targetAddr;
}

uint16_t CPU::getEffectiveAddress() {
    ADDR_MODE mode = instruction_decoder.getAddrMode();
    switch (mode) {
        default:
        case A:
        case impl:
        case Imm: return 0;
        case Abs: return absoluteAddressing();
        case abs_X: return abs_XAddressing();
        case abs_Y: return abs_YAddressing();
        case ind: return indirectAddressing();
        case X_ind: return X_indAddressing();
        case ind_Y: return ind_YAddressing();
        case rel: return relativeAddressing();
        case zpg: return zpgAddressing();
        case zpg_X: return zpg_XAddressing();
        case zpg_Y: return zpg_YAddressing();
    }
}

void CPU::writeByteUsingEA(uint8_t value) {
    const ADDR_MODE mode = instruction_decoder.getAddrMode();
    assert(mode != impl && "Cant write using implied addressing");
    assert(mode != Imm && "Cant write using immediate addressing");

    if(mode == A) {
        reg_unit.setA(value);
        return;
    }
    const uint16_t addr = getEffectiveAddress();
    mem.writeByte(addr, value);
}

uint8_t CPU::readByteUsingEA() {
    const ADDR_MODE mode = instruction_decoder.getAddrMode();

    if(mode == A) {
        return reg_unit.getA();
    }
    if(mode == Imm) {
        return immediateAddressing();
    }
    const uint16_t addr = getEffectiveAddress();
    return mem.readByte(addr);
}

void CPU::changeArithemticFlags(uint8_t LHS, uint8_t RHS, uint16_t res) {
    reg_unit.setCF(res > 0xFF);
    reg_unit.setZF((res & 0xFF) == 0x00);
    reg_unit.setNF(res & 0x80);
    reg_unit.setVF( (~(LHS ^ RHS) & (LHS ^ res) & 0x80 ) != 0 );
}

void CPU::_adc() {

    const uint8_t AC = reg_unit.getA();
    const uint8_t v = readByteUsingEA();

    if(debug) printf("%s %s 0x%02X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),v);

    const uint16_t res = AC + v + reg_unit.getCF();

    changeArithemticFlags(AC, v, res);

    reg_unit.setA(static_cast<uint8_t>(res));
}

void CPU::_and() {
    const uint8_t AC = reg_unit.getA();
    const uint8_t v = readByteUsingEA();

    if(debug) printf("%s %s 0x%02X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),v);

    const uint16_t res = AC & v;

    reg_unit.setZF((res & 0xFF) == 0x0);
    reg_unit.setZF(res & 0x80);

    reg_unit.setA(static_cast<uint8_t>(res));
}

void CPU::_asl() {

    uint8_t AC = 0;
    uint16_t res = 0;
    uint8_t v = 0;
    uint16_t EA = 0;

    switch (instruction_decoder.getAddrMode()) {
        default:break;
        case A:
            AC = reg_unit.getA();
            res = AC << 1;
            reg_unit.setA(res);
            break;
        case zpg: EA = zpgAddressing(); break;
        case zpg_X: EA = zpg_XAddressing(); break;
        case Abs: EA = absoluteAddressing(); break;
        case abs_X: EA = abs_XAddressing(); break;
    }

    if(instruction_decoder.getAddrMode() != A) {
        v = mem.readByte(EA);
        res = v << 1;
        mem.writeByte(EA, res);
        if(debug) printf("%s %s 0x%02X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),v);
    }else {
        if(debug) printf("%s %s\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str());
    }

    reg_unit.setNF(res & 0x80);
    reg_unit.setZF((res & 0xFF) == 0x0);
    reg_unit.setCF(res > 0xFF);
}

void CPU::_bcc() {

    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(!reg_unit.getCF()) {
        reg_unit.setPC(EA);
    }

}


void CPU::_bcs() {
    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(reg_unit.getCF()) {
        reg_unit.setPC(EA);
    }
}

void CPU::_beq() {
    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(!reg_unit.getZF()) {
        reg_unit.setPC(EA);
    }
}

void CPU::_bit() {
    uint8_t v = readByteUsingEA();

    uint8_t SR = reg_unit.getSR() & 0x3F;

    reg_unit.setSR( SR | (v & 0xC0) );

    reg_unit.setZF(((reg_unit.getA() & v) & 0xFF) == 0x0);
}

void CPU::_bmi() {
    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(reg_unit.getNF()) {
        reg_unit.setPC(EA);
    }
}
void CPU::_bne() {
    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(!reg_unit.getZF()) {
        reg_unit.setPC(EA);
    }
}
void CPU::_bpl() {
    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(!reg_unit.getNF()) {
        reg_unit.setPC(EA);
    }
}
void CPU::_brk() {
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
    reg_unit.incPC(2);

    const uint16_t retAddr = reg_unit.getPC();
    pushByte( (retAddr >> 8) & 0xFF ); // Push PC High Byte
    pushByte( retAddr & 0xFF );        // Push PC Low Byte

    reg_unit.setBF();
    pushByte(reg_unit.getSR());

    // new PC from BRK vector (0xFFFE-0xFFFF)
    const uint8_t LL = bus_interface.read(0xFFFE);
    const uint8_t HH = bus_interface.read(0xFFFF);
    reg_unit.setPC(static_cast<uint16_t>(HH) << 8 | LL);

}
void CPU::_bvc() {
    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(!reg_unit.getVF()) {
        reg_unit.setPC(EA);
    }
}
void CPU::_bvs() {
    uint16_t EA = getEffectiveAddress();
    if(instruction_decoder.getAddrMode() != rel)
        printf("Can only branch using relative addresses\n");
    if(debug) printf("%s %s 0x%04X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),EA);
    if(reg_unit.getVF()) {
        reg_unit.setPC(EA);
    }
}
void CPU::_clc() {
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
    reg_unit.unsetCF();
}
void CPU::_cld() {
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
    reg_unit.unsetDF();
}
void CPU::_cli() {
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
    reg_unit.unsetIF();
}
void CPU::_clv() {
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
    reg_unit.unsetVF();
}
void CPU::_cmp() {
    const uint8_t v = readByteUsingEA();

    const uint8_t AC = reg_unit.getA();

    if(AC < v) {
        reg_unit.unsetZF();
        reg_unit.unsetCF();
        reg_unit.setSR( reg_unit.getSR() | (AC - v) & 0x80 );
    }else if(AC == v) {
        reg_unit.setZF();
        reg_unit.setCF();
        reg_unit.unsetNF();
    }else if(AC > v) {
        reg_unit.unsetZF();
        reg_unit.setCF();
        reg_unit.setSR( reg_unit.getSR() | (AC - v) & 0x80 );
    }

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);
}
void CPU::_cpx() {
    const uint8_t v = readByteUsingEA();

    const uint8_t XR = reg_unit.getX();

    if(XR < v) {
        reg_unit.unsetZF();
        reg_unit.unsetCF();
        reg_unit.setSR( reg_unit.getSR() | (XR - v) & 0x80 );
    }else if(XR == v) {
        reg_unit.setZF();
        reg_unit.setCF();
        reg_unit.unsetNF();
    }else if(XR > v) {
        reg_unit.unsetZF();
        reg_unit.setCF();
        reg_unit.setSR( reg_unit.getSR() | (XR - v) & 0x80 );
    }

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);
}
void CPU::_cpy() {
    const uint8_t v = readByteUsingEA();

    const uint8_t YR = reg_unit.getY();

    if(YR < v) {
        reg_unit.unsetZF();
        reg_unit.unsetCF();
        reg_unit.setSR( reg_unit.getSR() | (YR - v) & 0x80 );
    }else if(YR == v) {
        reg_unit.setZF();
        reg_unit.setCF();
        reg_unit.unsetNF();
    }else if(YR > v) {
        reg_unit.unsetZF();
        reg_unit.setCF();
        reg_unit.setSR( reg_unit.getSR() | (YR - v) & 0x80 );
    }

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);
}
void CPU::_dec() {

    uint8_t EA = getEffectiveAddress();

    uint16_t res = mem.readByte(EA) - 1;

    reg_unit.setNF( res & 0x80 );
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),EA);
}
void CPU::_dex() {
    uint16_t res = reg_unit.getX() - 1;

    reg_unit.setNF( res & 0x80 );
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    reg_unit.setX(res);

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_dey() {
    uint16_t res = reg_unit.getY() - 1;

    reg_unit.setNF( res & 0x80 );
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    reg_unit.setY(res);

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_eor() {

    uint8_t v = readByteUsingEA();
    uint16_t res = reg_unit.getA() ^ v;

    reg_unit.setNF(res & 0x80);
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    reg_unit.setA(res);

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);
}
void CPU::_inc() {
    uint8_t EA = getEffectiveAddress();

    uint16_t res = mem.readByte(EA) + 1;

    reg_unit.setNF( res & 0x80 );
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),EA);
}
void CPU::_inx() {
    uint16_t res = reg_unit.getX() + 1;

    reg_unit.setNF( res & 0x80 );
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    reg_unit.setX(res);

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_iny() {
    uint16_t res = reg_unit.getY() + 1;

    reg_unit.setNF( res & 0x80 );
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    reg_unit.setY(res);

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_jmp() {
    uint16_t newAddr = getEffectiveAddress();

    reg_unit.setPC(newAddr);

    if(debug) printf("%s %04X\n",instruction_decoder.str_opcode_type().c_str(),newAddr);
}
void CPU::_jsr() {
    const uint16_t newAddr = getEffectiveAddress();

    const uint16_t returnAddr = reg_unit.getPC() + 1; // PC+2 (since PC hase already been increased

    pushByte( (returnAddr >> 8) & 0xFF );
    pushByte(returnAddr & 0xFF);

    reg_unit.setPC(newAddr);
    if(debug) printf("%s %04X\n",instruction_decoder.str_opcode_type().c_str(),newAddr);
}
void CPU::_lda() {

    uint16_t addr = getEffectiveAddress();
    reg_unit.setA( readByteUsingEA() );

    uint8_t AC = reg_unit.getA();
    reg_unit.setNF(AC & 0x80);
    reg_unit.setZF(AC == 0x0);

    if(debug) printf("%s %04X\n",instruction_decoder.str_opcode_type().c_str(),addr);
}
void CPU::_ldx() {
    uint16_t addr = getEffectiveAddress();
    reg_unit.setX( mem.readByte(addr) );

    uint8_t XR = reg_unit.getX();
    reg_unit.setNF(XR & 0x80);
    reg_unit.setZF(XR == 0x0);

    if(debug) printf("%s %04X\n",instruction_decoder.str_opcode_type().c_str(),addr);
}
void CPU::_ldy() {
    uint16_t addr = getEffectiveAddress();
    reg_unit.setY( mem.readByte(addr) );

    uint8_t YR = reg_unit.getY();
    reg_unit.setNF(YR & 0x80);
    reg_unit.setZF(YR == 0x0);

    if(debug) printf("%s %04X\n",instruction_decoder.str_opcode_type().c_str(),addr);
}

void CPU::_lsr() {
    uint8_t AC = 0;
    uint8_t v = 0;
    uint16_t EA = 0;
    uint8_t res = 0;  // LSR produces an 8-bit result

    switch (instruction_decoder.getAddrMode()) {
        default: break;
        case A:
            AC = reg_unit.getA();
        reg_unit.setCF(AC & 0x01);  // Set carry flag to bit 0 before shifting
        res = AC >> 1;
        reg_unit.setA(res);
        break;
        case zpg:   EA = zpgAddressing(); break;
        case zpg_X: EA = zpg_XAddressing(); break;
        case Abs:   EA = absoluteAddressing(); break;
        case abs_X: EA = abs_XAddressing(); break;
    }

    if (instruction_decoder.getAddrMode() != A) {
        v = mem.readByte(EA);
        reg_unit.setCF(v & 0x01);  // Set carry flag to bit 0 before shifting
        res = v >> 1;
        mem.writeByte(EA, res);

        if (debug)
            printf("%s %s 0x%02X\n", instruction_decoder.str_opcode_type().c_str(),
                                     instruction_decoder.str_addr_mode().c_str(), v);
    } else {
        if (debug)
            printf("%s %s\n", instruction_decoder.str_opcode_type().c_str(),
                               instruction_decoder.str_addr_mode().c_str());
    }

    reg_unit.setZF(res == 0);  // Zero Flag (ZF) set if result is zero
    reg_unit.setNF(false);      // Negative Flag (NF) is always cleared in LSR
}


void CPU::_nop() {
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}

void CPU::_ora() {
    uint8_t v = readByteUsingEA();
    uint16_t res = reg_unit.getA() | v;

    reg_unit.setNF(res & 0x80);
    reg_unit.setZF( (res & 0xFF) == 0x0 );

    reg_unit.setA(res);

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);
}
void CPU::_pha() {

    pushByte(reg_unit.getA());

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_php() {
    reg_unit.setBF();
    reg_unit.setSR( reg_unit.getSR() | 0x20 );

    pushByte(reg_unit.getSR());

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_pla() {
    uint8_t v = popByte();
    reg_unit.setA(v);
    reg_unit.setNF(v & 0x80);
    reg_unit.setZF(v == 0x0);

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_plp() {
    uint8_t v = popByte();

    v = v & ~(0x30);

    reg_unit.setSR(v);

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_rol() {

    uint8_t v = readByteUsingEA();

    bool oldCarry = reg_unit.getCF();
    bool carry = (v & 0x80) >> 7;
    v = (v << 1) | oldCarry;
    reg_unit.setCF(carry);

    writeByteUsingEA(v);

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);
}
void CPU::_ror() {
    uint8_t v = readByteUsingEA();

    bool oldCarry = reg_unit.getCF();
    bool carry = (v & 0x01);
    v = (v >> 1) | oldCarry << 7;
    reg_unit.setCF(carry);

    writeByteUsingEA(v);

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);
}
void CPU::_rti() {
    uint8_t v = popByte();

    v = v & ~(0x30);

    reg_unit.setSR(v);

    uint8_t PCL = popByte();
    uint8_t PCH = popByte();
    uint16_t newPC = (static_cast<uint16_t>(PCH) << 8) | PCL;
    reg_unit.setPC(newPC);

    if(debug) printf("%s %02X\n",instruction_decoder.str_opcode_type().c_str(),v);

}
void CPU::_rts() {

    uint8_t PCL = popByte();
    uint8_t PCH = popByte();
    uint16_t newPC = (static_cast<uint16_t>(PCH) << 8) | PCL;
    reg_unit.setPC(newPC);

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_sbc() {
    const uint8_t AC = reg_unit.getA();
    const uint8_t v = readByteUsingEA();

    if(debug) printf("%s %s 0x%02X\n",instruction_decoder.str_opcode_type().c_str(),instruction_decoder.str_addr_mode().c_str(),v);

    const uint16_t res = AC - v - reg_unit.getCF();

    changeArithemticFlags(AC, v, res);

    reg_unit.setA(static_cast<uint8_t>(res));
}
void CPU::_sec() {
    reg_unit.setCF();

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_sed() {
    reg_unit.setDF();

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_sei() {
    reg_unit.setIF();

    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_sta() {
    uint16_t addr = getEffectiveAddress();
    mem.writeByte(addr, reg_unit.getA());
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_stx() {
    uint16_t addr = getEffectiveAddress();
    mem.writeByte(addr, reg_unit.getX());
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_sty() {
    uint16_t addr = getEffectiveAddress();
    mem.writeByte(addr, reg_unit.getY());
    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_tax() {

    uint8_t AC = reg_unit.getA();

    reg_unit.setX( AC );

    reg_unit.setNF( AC & 0x80 );
    reg_unit.setZF( AC == 0x0 );


    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_tay() {
    uint8_t AC = reg_unit.getA();

    reg_unit.setY( AC );

    reg_unit.setNF( AC & 0x80 );
    reg_unit.setZF( AC == 0x0 );


    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_tsx() {
    uint8_t SP = reg_unit.getSP();

    reg_unit.setX( SP );

    reg_unit.setNF( SP & 0x80 );
    reg_unit.setZF( SP == 0x0 );


    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_txa() {
    uint8_t XR = reg_unit.getX();

    reg_unit.setA( XR );

    reg_unit.setNF( XR & 0x80 );
    reg_unit.setZF( XR == 0x0 );


    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_txs() {
    uint8_t XR = reg_unit.getX();

    reg_unit.setSP( XR );



    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}
void CPU::_tya() {
    uint8_t YR = reg_unit.getY();

    reg_unit.setA( YR );

    reg_unit.setNF( YR & 0x80 );
    reg_unit.setZF( YR == 0x0 );


    if(debug) printf("%s\n",instruction_decoder.str_opcode_type().c_str());
}






