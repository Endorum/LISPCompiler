//
// Created by Alexander on 19.02.25.
//

#ifndef REGUNIT_HPP
#define REGUNIT_HPP
#include <cstdint>

/*

FLAGS:

https://www.nesdev.org/wiki/Status_flags

7  bit  0
---- ----
NV1B DIZC
|||| ||||
|||| |||+- Carry
|||| ||+-- Zero
|||| |+--- Interrupt Disable
|||| +---- Decimal
|||+------ (No CPU effect; see: the B flag)
||+------- (No CPU effect; always pushed as 1)
|+-------- Overflow
+--------- Negative

B Flag:
B is 0 when pushed by interrupts (NMI and IRQ) and 1 when pushed by instructions (BRK and PHP)


 */


class RegUnit {

public:
    RegUnit() = default;

    // General Purpose register operations
    uint8_t getA() const {return A;}
    uint8_t getX() const {return X;}
    uint8_t getY() const {return Y;}
    void setA(const uint8_t v) { A=v; }
    void setX(const uint8_t v) { X=v; }
    void setY(const uint8_t v) { Y=v; }


    uint8_t getSP() const {return SP;}
    void setSP(const uint8_t v) { SP=v; }
    void incSP() { SP++; }
    void decSP() { SP--; }


    uint16_t getPC() const { return PC; }
    void setPC(const uint16_t v) { PC=v; }
    uint16_t incPC(uint8_t imm=1) { uint16_t prev = PC; PC+=imm; return prev; }

    // Flag operations
    uint8_t getSR() const {return SR;}
    void setSR(const uint8_t v) { SR=v; }

    bool getNF() const {return (SR & 0x80) != 0;}
    bool getVF() const {return (SR & 0x40) != 0;}
    bool getBF() const {return (SR & 0x10) != 0;}
    bool getDF() const {return (SR & 0x08) != 0;}
    bool getIF() const {return (SR & 0x04) != 0;}
    bool getZF() const {return (SR & 0x02) != 0;}
    bool getCF() const {return (SR & 0x01) != 0;}

    void unsetNF() { SR &= ~(0x80);}
    void unsetVF() { SR &= ~(0x40);}
    void unsetBF() { SR &= ~(0x10);}
    void unsetDF() { SR &= ~(0x08);}
    void unsetIF() { SR &= ~(0x04);}
    void unsetZF() { SR &= ~(0x02);}
    void unsetCF() { SR &= ~(0x01);}

    void setNF(bool cond=true) { (cond) ? static_cast<void>(SR |= 0x80) : unsetNF(); }
    void setVF(bool cond=true) { (cond) ? static_cast<void>(SR |= 0x40) : unsetVF(); }
    void setBF(bool cond=true) { (cond) ? static_cast<void>(SR |= 0x10) : unsetBF(); }
    void setDF(bool cond=true) { (cond) ? static_cast<void>(SR |= 0x08) : unsetDF(); }
    void setIF(bool cond=true) { (cond) ? static_cast<void>(SR |= 0x04) : unsetIF(); }
    void setZF(bool cond=true) { (cond) ? static_cast<void>(SR |= 0x02) : unsetZF(); }
    void setCF(bool cond=true) { (cond) ? static_cast<void>(SR |= 0x01) : unsetCF(); }



private:
    uint8_t A{0};
    uint8_t X{0};
    uint8_t Y{0};

    uint8_t SP{0xFF}; // Stack pointer
    uint8_t SR{0}; // Status register

    uint16_t PC{0}; // Program counter

};



#endif //REGUNIT_HPP
