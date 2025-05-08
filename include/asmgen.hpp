//
// Created by Alexander on 08.05.25.
//

#ifndef ASMGEN_HPP
#define ASMGEN_HPP

#include <string>



class Asmgen {
    std::string source;

    std::string asm_result_tmp;

    // asm_result += "section .text\n";
    // asm_result += "global _start\n";
    // asm_result += "_start:\n";
    // asm_result += "    push ebp\n";
    // asm_result += "    mov ebp, esp\n";

    // void generateASMforBinOp(const IRInstruction& instr, const std::string op) {
    //     asm_result_tmp += "mov eax, [ebp" + offsetStr(instr.src1) + "]\n";
    //     asm_result_tmp += op + " eax, [ebp" + offsetStr(instr.src2) + "]\n";
    //     asm_result_tmp += "mov [ebp" + offsetStr(instr.dst) + "], eax\n";
    // }
    //
    // void generateASMforImm(const IRInstruction& instr) {
    //     asm_result_tmp += "mov eax, " + instr.src1 + "\n";
    //     asm_result_tmp += "mov [ebp" + offsetStr(instr.dst) + "], eax\n";
    // }
    //
    // void generateASMforCmp(const IRInstruction& instr, const std::string& jmpOp) {
    //     asm_result_tmp += "mov eax, [ebp" + offsetStr(instr.src1) + "]\n";
    //     asm_result_tmp += "cmp eax, [ebp" + offsetStr(instr.src2) + "]\n";
    //     asm_result_tmp += "mov eax, 0\n";
    //     asm_result_tmp += jmpOp + " .set_true" + std::to_string(labelCount) + "\n";
    //     asm_result_tmp += "jmp .end" + std::to_string(labelCount) + "\n";
    //     asm_result_tmp += ".set_true" + std::to_string(labelCount) + ":\n";
    //     asm_result_tmp += "mov eax, 1\n";
    //     asm_result_tmp += ".end" + std::to_string(labelCount) + ":\n";
    //     asm_result_tmp += "mov [ebp" + offsetStr(instr.dst) + "], eax\n";
    //     labelCount++;
    // }
    //
    // void generateASMforPrintInt(const IRInstruction& instr) {
    //     asm_result_tmp += "mov esp, ebp\n";
    //     asm_result_tmp += "pop ebp\n";
    //     asm_result_tmp += "mov eax, [ebp" + offsetStr(instr.src1) + "]\n";
    //     asm_result_tmp += "int 0x80\n";
    // }
    //
    // std::string offsetStr(std::string str) { // t0 -> -4 t1 -> -8 etc
    //     std::string num = str.substr(1, str.size());
    //     size_t tmpNum = std::stoi(num);
    //     return " - "+std::to_string(tmpNum * 4);
    // }

    // asm_result += "    sub esp, " + std::to_string((temp_count + 1)*4) + "\n";
    //
    //
    // asm_result += indentAsm(asm_result_tmp);

};



#endif //ASMGEN_HPP
