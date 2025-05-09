//
// Created by Alexander on 08.05.25.
//

#ifndef ASMGEN_HPP
#define ASMGEN_HPP
#include <string>
#include <map>

#define START_SYMBOL "_start"
#define LIST_SPACE 1024

#include "codegen.hpp"


class Asmgen {

public:
    Asmgen(std::vector<IRInstruction>& instructions, std::unordered_map<std::string, int>& offsets, std::vector<Function>& funcs)
        : input(instructions), varOffsets(offsets), funcs(funcs) {}

    void generate() {
        asm_result += asm_overhead + "\n";

        dataSection += "list_memory: times " + std::to_string(LIST_SPACE) + " db 0 ; reserved space for cons cells\n";
        dataSection += "list_ptr: dd list_memory ; pointer to next free cell\n";

        for(auto instr : input) {
            generate_asm(instr);
        }
    }

    std::string getAsm() {
        return asm_result + "\n"  + dataSection;
    }

private:
    std::string asm_result;
    std::vector<IRInstruction> input;
    std::unordered_map<std::string, int> varOffsets;
    std::vector<Function> funcs;

    std::map<std::string, std::string> stringLabelMap;
    int stringCounter = 0;
    int listPtrCounter = 0;
    std::string dataSection = "section .data\n";

    Function currentScope = {"global", 0};


    std::string start = START_SYMBOL;
    std::string asm_overhead = R"(
%include "../stdlib.asm"
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
            return 4 * (tempIndex + 1); // [ebp - 4], [ebp - 8], etc.
        }





        throw std::runtime_error("Unknown variable name or temporary: " + name);
    }




    std::string offsetStr(const std::string& name) {
        int offset = getOffset(name);

        // if name = t0 etc -> sp - 4 * x
        // if name = parameter -> bp + 4 * x

        if (name.size() >= 2 && name.at(0) == 't' && std::isdigit(name.at(1))) {
            return "[ebp - " + std::to_string(offset) + "]";
        }

        return "[ebp + " + std::to_string(offset) + "]";
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

    void generateAssignment(const IRInstruction &instr) {
        std::string src = instr.src1;
        std::string dst = instr.dst;

        // temporary variabes (t0 etc) should be sp - 4 * x
        // parameters should be bp + 4 * x

        std::string src_operand;

        if (src == "return_value") {
            // src_operand = offsetStr("return_value");
            // asm_result += getCurrentIntendStr() + "mov eax, " + src_operand + "\n";
        } else if (isNumber(src)) {
            // Immediate constant
            asm_result += getCurrentIntendStr() + "mov eax, " + src + "\n";
        } else if (src.starts_with("str_")) {
            // It's a string label loaded with `loadstr`
            asm_result += getCurrentIntendStr() + "mov eax, " + src + "\n";
        } else {
            // Assume it's a temporary or variable with an offset
            src_operand = offsetStr(src);
            asm_result += getCurrentIntendStr() + "mov eax, " + src_operand + "\n";
        }

        // Store into destination offset
        asm_result += getCurrentIntendStr() + "mov " + offsetStr(dst) + ", eax\n";
    }

    bool isNumber(const std::string& s) {
        if (s.empty()) return false;
        for (char c : s) {
            if (!std::isdigit(c) && c != '-') return false;
        }
        return true;
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

    std::string formatStringForNASM(const std::string& input) {
        std::ostringstream result;
        result << "\"";

        for (size_t i = 0; i < input.size(); ++i) {
            if (input[i] == '\\' && i + 1 < input.size()) {
                switch (input[i + 1]) {
                    case 'n':
                        result << "\", 10, \""; // newline
                    ++i;
                    break;
                    case 't':
                        result << "\", 9, \"";  // tab
                    ++i;
                    break;
                    case '\\':
                        result << "\\\\";
                    ++i;
                    break;
                    case '\"':
                        result << "\\\"";
                    ++i;
                    break;
                    default:
                        result << input[i];
                    break;
                }
            } else {
                result << input[i];
            }
        }

        result << "\"";
        return result.str();
    }

    std::string generate_asm(IRInstruction& instr) {
        const std::string op = instr.op;
        if(op.empty()) {
            // throw std::runtime_error("ERROR: op was empty");
            return "";
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

        else if(op == "l" || op == "g" || op == "eq") {
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
            asm_result += getCurrentIntendStr() + "sub esp, " + std::to_string((localVars)*4) + "\n"; // <- adjust depending on how many local variables there are

        }

        else if(op == "pop") {
            // do nothing apperently ?
            // asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.dst) + "\n";
            // asm_result += getCurrentIntendStr() + "mov " + offsetStr(instr.src1) + ", eax" +"\n";
        }

        else if(op == "push") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";
            asm_result += getCurrentIntendStr() + "push eax\n";
        }

        else if(op == "return") {
            asm_result += getCurrentIntendStr() + "mov eax, " + offsetStr(instr.src1) + "\n";

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

                dataSection += label + ": db " + formatStringForNASM(str) + ", 0\n";
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

        else if(op == "if") {
            std::string trueLabel = instr.dst;
            std::string srcOffset = offsetStr(instr.src1);
            if(instr.src2 == "jump") {
                asm_result += getCurrentIntendStr() + "mov eax, " + srcOffset + "\n";
                asm_result += getCurrentIntendStr() + "cmp eax, 0\n";
                asm_result += getCurrentIntendStr() + "jne " + trueLabel + "\n";
            }else {
                throw std::runtime_error("ERROR: only jump work right now");
            }
        }

        else if(op == "label") {
            std::string label = instr.dst;
            asm_result += label + ":\n";
        }

        else if(op == "jump") {
            std::string label = instr.dst;
            asm_result += getCurrentIntendStr() + "jmp " + label + "\n";
        }

        else if(op == "cons") {
            const std::string dst = offsetStr(instr.dst);
            const std::string leftSide = offsetStr(instr.src1);
            const std::string rightSide = offsetStr(instr.src2);

            // load car (src1) into eax
            asm_result += getCurrentIntendStr() + "; load car (src1) into eax\n";
            asm_result += getCurrentIntendStr() + "mov eax, " + leftSide + "\n";

            // store car into [list_ptr]
            asm_result += getCurrentIntendStr() + "; store car into [list_ptr]\n";
            asm_result += getCurrentIntendStr() + "mov ebx, [list_ptr]\n";
            asm_result += getCurrentIntendStr() + "mov [ebx], eax\n";

            // load cdr (src2) into eax
            asm_result += getCurrentIntendStr() + "; load cdr (src2) into eax\n";
            asm_result += getCurrentIntendStr() + "mov eax, " + rightSide + "\n";

            // Load cdr into [list_ptr + 4]
            asm_result += getCurrentIntendStr() + "; Load cdr into [list_ptr + 4]\n";
            asm_result += getCurrentIntendStr() + "mov ebx, [list_ptr]\n";
            asm_result += getCurrentIntendStr() + "mov [ebx + 4], eax\n";

            // Store address of cons cell into dst
            asm_result += getCurrentIntendStr() + "; Store address of cons cell into dst\n";
            asm_result += getCurrentIntendStr() + "mov eax, [list_ptr]\n";
            asm_result += getCurrentIntendStr() + "mov " + dst + ", eax\n";

            listPtrCounter += 8;
            if(listPtrCounter > LIST_SPACE) {
                throw std::runtime_error("Too many cons cells -> increase LIST_SPACE. currently at: " + std::to_string(LIST_SPACE) + "bytes");
            }

            // Increment list pointer by 8 for next allocation
            asm_result += getCurrentIntendStr() + "; Increment list pointer by 8 for next allocation\n";
            asm_result += getCurrentIntendStr() + "add dword [list_ptr], 8\n";
        }


        else if(op == "car") {
            const std::string dst = offsetStr(instr.dst);
            const std::string src = offsetStr(instr.src1);

            asm_result += getCurrentIntendStr() + "mov ebx, " + src + "\n";       // Load pointer to cons cell
            asm_result += getCurrentIntendStr() + "mov eax, [ebx]\n";             // Load car (first 4 bytes)
            asm_result += getCurrentIntendStr() + "mov " + dst + ", eax\n";       // Store car into dst
        }

        else if(op == "cdr") {
            const std::string dst = offsetStr(instr.dst);
            const std::string src = offsetStr(instr.src1);

            asm_result += getCurrentIntendStr() + "mov ebx, " + src + "\n";       // Load pointer to cons cell
            asm_result += getCurrentIntendStr() + "mov eax, [ebx + 4]\n";             // Load car (first 4 bytes)
            asm_result += getCurrentIntendStr() + "mov " + dst + ", eax\n";       // Store car into dst
        }




        else {
            throw std::runtime_error("ERROR: Unhandled Instruction: " + op);
        }


        return asm_result;
    }
};



#endif //ASMGEN_HPP
