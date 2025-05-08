//
// Created by Alexander on 08.05.25.
//

#ifndef ASMGEN_HPP
#define ASMGEN_HPP
#include <string>
#include <map>

#define START_SYMBOL "_start"

#include "codegen.hpp"


class Asmgen {

public:
    Asmgen(std::vector<IRInstruction>& instructions, std::unordered_map<std::string, int>& offsets, std::vector<Function>& funcs)
        : input(instructions), varOffsets(offsets), funcs(funcs) {}

    void generate() {
        asm_result += asm_overhead + "\n";


        for(auto instr : input) {
            generate_asm(instr);
        }
    }

    std::string getAsm() {
        return asm_result + "\n\nsection .data\n"  + dataSection;
    }

private:
    std::string asm_result;
    std::vector<IRInstruction> input;
    std::unordered_map<std::string, int> varOffsets;
    std::vector<Function> funcs;

    std::map<std::string, std::string> stringLabelMap;
    int stringCounter = 0;
    std::string dataSection;

    Function currentScope = {"global", 0};


    std::string start = START_SYMBOL;
    std::string asm_overhead = R"(
%include "../stdio.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret)";

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

        asm_result += getCurrentIntendStr() +"; " + instr.str(false) + "\n";

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
            currentIndent++;

            size_t localVars = -1;
            for(const auto& func : funcs) {
                if(func.name == instr.src1) {
                    localVars = func.localVars;
                    currentScope = func;
                }
            }


            if(localVars == -1) {
                throw std::runtime_error("ERROR: localsVars = -1: Could not find function with label: " + instr.src1);
            }

            // stack frame
            asm_result += getCurrentIntendStr() + "; Stack frame\n";
            asm_result += getCurrentIntendStr() + "push ebp\n";
            asm_result += getCurrentIntendStr() + "mov ebp, esp\n";
            asm_result += getCurrentIntendStr() + "sub esp, " + std::to_string(localVars*4) + "\n"; // <- adjust depending on how many local variables there are

        }

        else if(op == "pop") {
            asm_result += getCurrentIntendStr() + "pop eax\n";
            asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.dst) + ", eax\n";
        }

        else if(op == "push") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "push eax\n";
        }

        else if(op == "return") {
            asm_result += getCurrentIntendStr() + "mov ebx, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "mov eax, ebx\n";
            asm_result += getCurrentIntendStr() + "mov " + offsetStr("return_value") + ", eax\n";

            // stack frame cleanup
            asm_result += getCurrentIntendStr() + "; Stack frame cleanup\n";
            asm_result += getCurrentIntendStr() + "mov esp, ebp\n";
            asm_result += getCurrentIntendStr() + "pop ebp\n";
            asm_result += getCurrentIntendStr() + "ret\n";
            currentIndent = (currentIndent > 1) ? currentIndent-- : 0;
        }


        else if(op == "call") {
            asm_result += getCurrentIntendStr() + "call " + instr.src1 + "\n";
        }

        else if(op == "loadstr") {
            std::string str = instr.src1;
            std::string label;

            if(stringLabelMap.find(str) == stringLabelMap.end()) {
                // string hasnt been added yet
                label = "str_" + std::to_string(stringCounter++);
                stringLabelMap[str] = label;

                dataSection += label + ": db \"" + str + "\", 0\n";
            }else {
                label = stringLabelMap[str];
            }

            std::string dstOffset = offsetStr(instr.dst);
            asm_result += getCurrentIntendStr() + "mov eax, " + label + "\n";
            asm_result += getCurrentIntendStr() + "mov " + dstOffset + ", eax\n";
        }

        else if(op == "print") {
            std::string stringAddr = offsetStr(instr.src1);
            asm_result += getCurrentIntendStr() + "mov eax, " + stringAddr + "\n";
            asm_result += getCurrentIntendStr() + "call _internal_print_string";
        }



        else {
            throw std::runtime_error("ERROR: Unhandled Instruction: " + op);
        }


        return asm_result;
    }
};



#endif //ASMGEN_HPP
