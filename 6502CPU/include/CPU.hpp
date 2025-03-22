//
// Created by Alexander on 19.02.25.
//

#ifndef CPU_HPP
#define CPU_HPP
#include <cstdint>

#include "ALU.hpp"
#include "BusInterface.hpp"
#include "InstructionDecoder.hpp"
#include "InterruptHandler.hpp"
#include "Memory.hpp"
#include "RegUnit.hpp"
#include "TimingUnit.hpp"




class CPU {
public:
    // Pinout
    uint16_t ABus{}; // Memory Address Bus
    uint8_t  DBus{}; // Memory Data Bus

    bool RDY{}; // Memory ready Singal
    bool PHI0{}; // Clock in
    bool PHI1{}; // Clock out
    bool PHI2{}; // Clock out inverted
    bool IRQ{}; // Interrupt request signal
    bool NMI{}; // Non maskable interrupt signal
    bool SYNC{}; // is true when CPU is fetching an opcode for debugging etc
    bool HALT{}; // Halts the CPU
    bool RW{}; // Read/Write
    bool SO{}; // Set overflow flag
    bool RST{}; // Resets the CPU

    RegUnit reg_unit;
    InstructionDecoder instruction_decoder; // std::unordered_map<uint8, std::function<void()>>
    ALU alu;
    InterruptHandler interrupt_handler;
    TimingUnit timing_unit;
    BusInterface bus_interface;

    //CPU() = default;
    explicit CPU(Memory& mem)
        : instruction_decoder(bus_interface, reg_unit),
          bus_interface(reg_unit, ABus, DBus, mem), mem(mem) {
    }

    void enableDebug() {
        debug = true;
    }

    void disableDebug() {
        debug = false;
    }

    void fetch() {
        instruction_decoder.fetchOpcode();
    }
    void decode() {
        instruction_decoder.decode();
    }


    void execute();

    void cycle() {
        fetch();
        decode();
        execute();
        if(debug) showCPU();
        if(debug) showStack();
        if(debug) showZpg();
    }

    void reset();

    void showCPU();
    void showStack();
    void showZpg(uint8_t page=0);

private:
    Memory& mem;

    bool debug = false;

    uint8_t getNextByte() {
        return bus_interface.read(reg_unit.incPC());
    }

    void pushByte(uint8_t value);
    uint8_t popByte();

    void pushWord(uint16_t value);
    uint16_t popWord();

    void impliedAddressing();
    uint8_t immediateAddressing();
    uint16_t absoluteAddressing();
    uint16_t zpgAddressing();
    uint16_t abs_XAddressing();
    uint16_t abs_YAddressing();
    uint16_t zpg_XAddressing();
    uint16_t zpg_YAddressing();
    uint16_t indirectAddressing();
    uint16_t X_indAddressing();
    uint16_t ind_YAddressing();
    uint16_t relativeAddressing();

    uint16_t getEffectiveAddress();

    void writeByteUsingEA(uint8_t value);
    uint8_t readByteUsingEA();

    void changeArithemticFlags(uint8_t LHS, uint8_t RHS, uint16_t res);

    void _adc();
    void _and();
    void _asl();
    void _bcc();
    void _bcs();
    void _beq();
    void _bit();
    void _bmi();
    void _bne();
    void _bpl();
    void _brk();
    void _bvc();
    void _bvs();
    void _clc();
    void _cld();
    void _cli();
    void _clv();
    void _cmp();
    void _cpx();
    void _cpy();
    void _dec();
    void _dex();
    void _dey();
    void _eor();
    void _inc();
    void _inx();
    void _iny();
    void _jmp();
    void _jsr();
    void _lda();
    void _ldx();
    void _ldy();
    void _lsr();
    void _nop();
    void _ora();
    void _pha();
    void _php();
    void _pla();
    void _plp();
    void _rol();
    void _ror();
    void _rti();
    void _rts();
    void _sbc();
    void _sec();
    void _sed();
    void _sei();
    void _sta();
    void _stx();
    void _sty();
    void _tax();
    void _tay();
    void _tsx();
    void _txa();
    void _txs();
    void _tya();

};



#endif //CPU_HPP
