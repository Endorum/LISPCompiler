//
// Created by Alexander on 19.02.25.
//

#ifndef MACHINE_HPP
#define MACHINE_HPP
#include "CPU.hpp"
#include "Memory.hpp"


class Machine {
public:

    Machine() : cpu(mem) {}



    Memory mem;
    CPU cpu;
};



#endif //MACHINE_HPP
