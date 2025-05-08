//
// Created by Alexander on 08.05.25.
//

#ifndef ASMGEN_HPP
#define ASMGEN_HPP
#include <string>
#include <map>



#include "codegen.hpp"


class Asmgen {

public:
    Asmgen(std::vector<IRInstruction>& instructions, std::unordered_map<std::string, int>& offsets) : input(instructions), varOffsets(offsets) {}

    void generate() {
        for(auto instr : input) {
            generate_asm(instr);
        }
    }

    std::string getAsm(){return asm_result;}

private:
    std::string asm_result;
    std::vector<IRInstruction> input;
    std::unordered_map<std::string, int> varOffsets;

    int getOffset(const std::string& name) const {
        // User-defined variables

        std::unordered_map<std::string, int>::const_iterator it = varOffsets.find(name);
        if (it != varOffsets.end()) {
            int offset = varOffsets.at(name);
            return offset; // positive offsets (e.g., [ebp + 0])
        }

        // Temporary variables like t0, t1, t2...
        if (name.size() >= 2 && name.at(0) == 't' && std::isdigit(name.at(1))) {
            int tempIndex = std::stoi(name.substr(1));
            return -4 * (tempIndex + 1); // [ebp - 4], [ebp - 8], etc.
        }



        throw std::runtime_error("Unknown variable name or temporary: " + name);
    }




    std::string offsetStr(const std::string& name) {
        int offset = getOffset(name);
        return "[ebp" + (offset < 0 ? " - " + std::to_string(-offset) : " + " + std::to_string(offset)) + "]";
    }


    bool isBinaryArithmeticOp(std::string op) {
        return (
            op == "add"  ||
            op == "sub"  ||
            op == "imul" ||
            op == "idiv" ||
            op == "and"  ||
            op == "or"   ||
            op == "xor"
        );
    }

    int currentIndent = 0;

    std::string getCurrentIntendStr() const{
        return (currentIndent != 0) ? "    " : "";
    }

    void generateBinaryArithmeticOp(const IRInstruction& instr) {
        if(instr.op == "idiv") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += "    cdq\n"; // sign extend eax into edx:eax
            asm_result += "    idiv dword " + offsetStr(instr.src2) + "\n";
            asm_result += "    mov " + offsetStr(instr.dst) + ", eax\n";
            return;
        }
        asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
        asm_result += getCurrentIntendStr() + instr.op + " eax, " + offsetStr(instr.src2) + "\n";
        asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
    }

    void generateAssignment(const IRInstruction & instr) {
        if(instr.src1 != "return_value") {
            asm_result += getCurrentIntendStr() + "mov eax, " + instr.src1 + "\n";
        }else {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr("return_value") + "\n";
        }

        asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
    }

    void generateCmps(const IRInstruction & instr) {
        asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
        asm_result += getCurrentIntendStr() + "cmp eax, " + offsetStr(instr.src2) + "\n";
        if(instr.op == "eq") {
            asm_result += getCurrentIntendStr() + "sete" + " al\n";
        }else {
            asm_result += getCurrentIntendStr() + "set" + instr.op + " al\n";
        }
        asm_result += getCurrentIntendStr() + "movzx eax, al\n";
        asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";

    }

    std::string generate_asm(IRInstruction& instr) {
        const std::string op = instr.op;
        if(op.empty()) {
            throw std::runtime_error("ERROR: op was empty");
        }

        asm_result += "# " + instr.str(false) + "\n";

        if(op == "=") {
            generateAssignment(instr);
        }

        else if(isBinaryArithmeticOp(op)) {
            generateBinaryArithmeticOp(instr);
        }

        else if(op == "neg") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "neg eax\n";
            asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
        }

        else if(op == "not") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "not eax\n";
            asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
        }

        else if(op == "lt" || op == "gt" || op == "eq") {
            generateCmps(instr);
        }

        else if(op == "store") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
        }

        else if(op == "load") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
        }

        else if(op == "defun") {
            asm_result += instr.src1 + ":\n";
        }

        else if(op == "pop") {
            asm_result += getCurrentIntendStr() + "pop eax\n";
            asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
        }

        else if(op == "push") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "push eax\n";
        }


        else {
            // throw std::runtime_error("ERROR: Unhandled Instruction: " + op);
        }


        return asm_result;
    }
};



#endif //ASMGEN_HPP
