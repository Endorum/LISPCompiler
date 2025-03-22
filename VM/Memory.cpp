#include "Memory.hpp"

#include <cassert>

// Implementation of read methods
u8 Memory::read8(u32 addr) const {
    assert(addr <= MAX_ADDR);
    return mem[addr];
}

u16 Memory::read16(u32 addr) const {
    assert(addr <= MAX_ADDR);
    return *(reinterpret_cast<u16*>(&mem[addr]));
}

u32 Memory::read32(u32 addr) const {
    assert(addr <= MAX_ADDR);
    return *(reinterpret_cast<u32*>(&mem[addr]));
}


void Memory::write8(u32 addr, u8 value) const {
    assert(addr <= MAX_ADDR);
    mem[addr] = value;
}

void Memory::write16(u32 addr, u16 value) const {
    assert(addr <= MAX_ADDR);
    *reinterpret_cast<u16*>(&mem[addr]) = value;
}

void Memory::write32(u32 addr, u32 value) const {
    assert(addr <= MAX_ADDR);
    *reinterpret_cast<u32*>(&mem[addr]) = value;
}

void Memory::writeBytes(uint32_t addr, uint32_t amount, uint8_t* src) const {
    assert(addr+amount <= MAX_ADDR);
    assert(src);
    for(int i=0;i<amount;i++) {
        write8(addr+i,src[i]);
    }
}

// Implementation of map method
void Memory::map(u32 addr, u32 amount) {
    assert(addr <= MAX_ADDR);
    locs.push_back({addr,amount});
}

void Memory::unmap(u32 addr) {
    // Use std::remove_if to locate and remove the pair with the matching address
    auto it = std::remove_if(locs.begin(), locs.end(),
                             [addr](const std::pair<u32, u32>& loc) {
                                 return loc.first == addr;
                             });

    // Erase the element if found
    if (it != locs.end()) {
        locs.erase(it, locs.end());
    }
}

