//
// Created by Alexander on 19.02.25.
//

#ifndef BUSINTERFACE_HPP
#define BUSINTERFACE_HPP
#include <cstdint>

#include "Memory.hpp"
#include "RegUnit.hpp"




class BusInterface {
public:
    BusInterface(RegUnit& reg, uint16_t& ABus, uint8_t& DBus, Memory& mem)
        : reg(reg), ABus(ABus), DBus(DBus), mem(mem) {
    }

    uint8_t read(uint16_t addr) {
        // set uint16 abus to addr
        // wait for data?
        // read from databus and return
        ABus = addr;
        DBus = mem.readByte(ABus);
        return DBus;
    }

private:
    RegUnit& reg;
    uint16_t& ABus;
    uint8_t& DBus;
    Memory& mem;
};



#endif //BUSINTERFACE_HPP
