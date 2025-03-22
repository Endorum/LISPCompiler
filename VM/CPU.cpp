
#include "CPU.hpp"

void CPU::execute() {
     switch (decoder.OPCode) {
          case NOP:break;
          case IMM:  _IMM();
          case MOV:  _MOV();
          case LOAD: _LOAD();
          case STOR: _STOR();
     }
}

u16 LOHI(u16 LO, u16 HI) {
     return LO | HI << 8;
}

void CPU::_IMM(){
     u8  regIdx = decoder.arg0;
     u8 LO = decoder.arg1;
     u8 HI = decoder.arg2;
     writeReg(regIdx, LOHI(LO,HI));
}
void CPU::_MOV(){
     u8 dst = decoder.arg0;
     u8 src = decoder.arg1;
     u8 off = decoder.arg2;
     writeReg(dst,readReg(src)+off);
}
void CPU::_LOAD(){

}
void CPU::_STOR(){

}