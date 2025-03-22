#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cstdint>
#include <vector>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define MAX_ADDR ((1ULL << 32) - 1)

class Memory {
private:
    u8* mem = nullptr;
    std::vector<std::pair<u32,u32>> locs; // locations can be mapped with a start and an amount
public:

    Memory(u8* mem) : mem(mem){}

    u8 read8(u32 addr) const;
    u16 read16(u32 addr) const;
    u32 read32(u32 addr) const;

    void write8(u32 addr, u8 value) const;
    void write16(u32 addr, u16 value) const;
    void write32(u32 addr, u32 value) const;

    void writeBytes(u32 addr, u32 amount, u8* src) const;

    u32 toPhysical(u16 Page, u16 addr) const {
        return (static_cast<uint32_t>(Page) << 16) | static_cast<uint32_t>(addr);
    }

    void map(u32 addr, u32 amount);
    void unmap(u32 addr);
};

#endif // MEMORY_HPP
