//
// Created by Alexander on 08.05.25.
//

#ifndef ASMGEN_HPP
#define ASMGEN_HPP
#include <string>
#include <map>
#include <regex>

#define START_SYMBOL "_start"
#define CLEANUP true

// so space for around 100k list items
// 1048576 = 2^20
#define LIST_SPACE 1048576// / 8 because we're working with full 2x 32 bit integers here (car and cdr)

#include "ir_codegen.hpp"


class Asmgen {

public:
    Asmgen(std::vector<IRInstruction>& instructions, std::unordered_map<std::string, Value> variable_table, std::vector<Function> functions)
        : input(instructions), variable_table(variable_table), functions(functions) {}

    void generate() {
        asm_result += asm_overhead + "\n";

        
        for(auto instr : input) {
            generate_asm(instr);
        }
        
        if(CLEANUP) cleanup();
        dataSection += "list_ptr: dd list_memory ; pointer to next free cell\n";
        dataSection += "section .bss\n";
        dataSection += "list_memory: resb " + std::to_string(LIST_SPACE) + "; reserved (uninitialized!!) space for cons cells\n";
        asm_result += dataSection;
    }

    std::string getAsm() {
        return asm_result + "\n";
    }

private:
    std::string asm_result;
    std::vector<IRInstruction> input;
    std::unordered_map<std::string, Value> variable_table;
    std::vector<Function> functions;

    std::map<std::string, std::string> stringLabelMap;
    int stringCounter = 0;
    int listPtrCounter = 0;
    std::string dataSection = "section .data\n";

    Function currentScope = {"global", 0};


    std::string start = START_SYMBOL;
    std::string asm_overhead = R"(
%include "stdlib.asm"
section .text
global _start
_start:
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret)";

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

    std::string getCurrentIndentStr() const{
        return (currentIndent != 0) ? "    " : "";
    }

    Function getFunction(std::string name){
        for(auto& func : functions){
            if(func.name == name) return func;
        }
        throw std::runtime_error("Function " + name + " not declared");
    }

    void generateBinaryArithmeticOp(const IRInstruction& instr) {
        if(instr.op == "idiv") {
            asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
            asm_result += "    cdq\n"; // sign extend eax into edx:eax
            asm_result += "    idiv dword " + instr.src2.loc + "\n";
            asm_result += "    mov " + instr.dst.loc + ", eax\n";
            return;
        }
        asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
        asm_result += getCurrentIndentStr() + instr.op + " eax, " + instr.src2.loc + "\n";
        asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
    }

    std::string _mov(Value dst, Value src) {
        std::string code;

        bool dstIsMem = dst.loc.find('[') != std::string::npos;
        bool srcIsMem = src.loc.find('[') != std::string::npos;

        if (dstIsMem && srcIsMem) {
            // Memory to memory: use ecx as temporary because edx is used for idiv and imul
            code += "mov ecx, " + src.loc + "\n";
            code += getCurrentIndentStr() + "mov " + dst.loc + ", ecx\n";
        } else {
            code += "mov " + dst.loc + ", " + src.loc + "\n";
        }

        return code;
    }


    void generateAssignment(const IRInstruction &instr) {
        Value src = instr.src1;
        Value dst = instr.dst;

        if (src.type == IMM_NUM){
            asm_result += getCurrentIndentStr() + "mov dword " + dst.loc + ", " + src.value + "\n";
        }else{
            asm_result += getCurrentIndentStr() + _mov(dst, src);
        }
    }

    bool isNumber(const std::string& s) {
        if (s.empty()) return false;
        for (char c : s) {
            if (!std::isdigit(c) && c != '-') return false;
        }
        return true;
    }


    void generateCmps(const IRInstruction & instr) {
        asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
        asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
        if(instr.op == "eq") {
            asm_result += getCurrentIndentStr() + "sete" + " al\n";
        }else {
            asm_result += getCurrentIndentStr() + "set" + instr.op + " al\n";
        }
        asm_result += getCurrentIndentStr() + "movzx eax, al\n";
        asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";

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

    /*
    cleanup things like

    mov [ebp - 8], eax
    mov eax, [ebp - 8]

    which can just be removed
    
    */
    std::string cleanup();


    std::string generate_asm(IRInstruction& instr) {
        const std::string op = instr.op;
        if(op.empty()) {
            throw std::runtime_error("ERROR: op was empty");
        }

        asm_result += getCurrentIndentStr() + ";; " + op + " " + instr.dst.value + ", " + instr.src1.value + ", " + instr.src2.value +"\n";

        // asm_result += getCurrentIndentStr() +"; " + instr.str() + "\n";

        if(op == "assign") {
            generateAssignment(instr);
        }

        else if(isBinaryArithmeticOp(op)) {
            generateBinaryArithmeticOp(instr);
        }

        else if(op == "neg") {
            asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
            asm_result += getCurrentIndentStr() + "neg eax\n";
            asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
        }

        else if(op == "not") {
            asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
            asm_result += getCurrentIndentStr() + "not eax\n";
            asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
        }

        else if(op == "mod") {
            asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
            asm_result += getCurrentIndentStr() + "cdq\n"; // sign-extend eax into edx:eax
            asm_result += getCurrentIndentStr() + "mov ebx, dword " + instr.src2.loc + "\n"; // sign-extend eax into edx:eax
            asm_result += getCurrentIndentStr() + "idiv dword ebx\n";
            asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", edx\n"; // remainder → dst
        }


        else if(op == "l" || op == "g" || op == "eq") {
            generateCmps(instr);
        }

        else if(op == "noteq"){
            asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
            asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
            asm_result += getCurrentIndentStr() + "setne" + " al\n";
            asm_result += getCurrentIndentStr() + "movzx eax, al\n";
            asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
        }

        else if (op == "addeq" || op == "subeq" || op == "imuleq" || op == "idiveq") {
            // if (instr.src1.type != LOCAL) {
            //     throw std::runtime_error("Can only use compound assignment on local variables");
            // }

            std::string op_instr;
            if (op == "addeq")      op_instr = "add";
            else if (op == "subeq") op_instr = "sub";

            if(op != "idiveq") asm_result += getCurrentIndentStr() + "mov eax, " + instr.src2.loc + "\n"; // value to add sub mul div (the right side like (+= x 2))

            if (op == "addeq" || op == "subeq") {
                asm_result += getCurrentIndentStr() + "mov ebx, " + instr.src1.loc + "\n";       // load original local
                asm_result += getCurrentIndentStr() + op_instr + " ebx, eax\n";                  // ebx = ebx ± eax
                asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", ebx\n";       // store back to local
            }

            else if (op == "imuleq") {
                asm_result += getCurrentIndentStr() + "mov ebx, " + instr.src1.loc + "\n";       // load local
                asm_result += getCurrentIndentStr() + "imul ebx, eax\n";                         // ebx *= eax
                asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", ebx\n";       // store back to local
            }

            else if (op == "idiveq") {
                // For signed division, use eax for dividend, edx for high bits (clear to 0 for 32-bit division)
                asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
                asm_result += getCurrentIndentStr() + "cdq\n";                                    // sign extend eax -> edx:eax
                asm_result += getCurrentIndentStr() + "mov ebx, " + instr.src2.loc + "\n";       // divisor
                asm_result += getCurrentIndentStr() + "idiv ebx\n";                               // eax = eax / ebx
                asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", eax\n";       // store back to local
            }
        }



        else if(op == "load") {
            asm_result += getCurrentIndentStr() + _mov(instr.dst, instr.src1);
        }

        else if(op == "defun") {
            currentIndent++;

            std::string funcName = instr.src1.value;

            Function func = getFunction(funcName);
            
            asm_result += funcName + ":\n";

            int local_bytes = func.localVarCount * 4;
            // stack frame
            asm_result += getCurrentIndentStr() + "push ebp\n";
            asm_result += getCurrentIndentStr() + "mov ebp, esp\n";
            asm_result += getCurrentIndentStr() + "sub esp, " + std::to_string(local_bytes) + "\n"; // adjusting the stack for local vars (let)

        }

        else if(op == "push") {
            if (instr.src1.type == IMM_NUM) {
                asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.value + "\n";
                asm_result += getCurrentIndentStr() + "push dword eax\n";
            } else {
                // instr.src1.loc is a register or memory location, can push directly
                asm_result += getCurrentIndentStr() + "push dword " + instr.src1.loc + "\n";
            }

        }

        else if(op == "return") {
            asm_result += getCurrentIndentStr() + _mov(instr.dst,instr.src1);
            asm_result += getCurrentIndentStr() + "mov esp, ebp\n";
            asm_result += getCurrentIndentStr() + "pop ebp\n";
            asm_result += getCurrentIndentStr() + "ret\n";
            currentIndent--;
        }


        else if(op == "call") {
            asm_result += getCurrentIndentStr() + "call " + instr.src1.value + "\n";
        }

        // esp is restored simply using mov esp, ebp, nvm its necessary
        else if(op == "add_esp"){
            asm_result += getCurrentIndentStr() + "add esp, " + std::to_string(std::stoi(instr.src1.value)) + "\n";
        }

        else if(op == "loadstr") {
            std::string str = instr.src1.value;
            std::string label;

            // if(stringLabelMap.find(str) == stringLabelMap.end()) {
            //     // string hasnt been added yet
            //     label = "str_" + std::to_string(stringCounter++);
            //     stringLabelMap[str] = label;

            //     dataSection += label + ": db " + formatStringForNASM(str) + ", 0\n";
            // }else {
            //     label = stringLabelMap[str];
            // }

            label = "str_" + std::to_string(stringCounter++);
            stringLabelMap[str] = label;

            dataSection += label + ": db " + formatStringForNASM(str) + ", 0\n";

            std::string dstOffset = instr.dst.loc;
            asm_result += getCurrentIndentStr() + "mov eax, " + label + "\n";
            asm_result += getCurrentIndentStr() + "mov " + dstOffset + ", eax\n";
        }

        else if(op == "print") {
            std::string stringAddr = instr.src1.loc;
            asm_result += getCurrentIndentStr() + "mov eax, " + stringAddr + "\n";
            asm_result += getCurrentIndentStr() + "call _internal_print_string\n";
        }

        else if(op == "if") {
            std::string trueLabel = instr.dst.value;
            std::string falseLabel = instr.src2.value;

            Value cond = instr.src1;

            asm_result += getCurrentIndentStr() + "mov eax, " + cond.loc + "\n";
            asm_result += getCurrentIndentStr() + "cmp eax, 0\n";
            asm_result += getCurrentIndentStr() + "je  " + falseLabel + "\n";
            asm_result += getCurrentIndentStr() + "jmp " + trueLabel + "\n";

        }

        else if(op == "label") {
            std::string label = instr.dst.value;
            asm_result += label + ":\n";
        }

        else if(op == "jump") {
            std::string label = instr.dst.value;
            asm_result += getCurrentIndentStr() + "jmp " + label + "\n";
        }

        else if(op == "cons") {
            const std::string dst = instr.dst.loc;
            const std::string leftSide = instr.src1.loc;
            const std::string rightSide = instr.src2.loc;

            // load car (src1) into eax
            asm_result += getCurrentIndentStr() + "mov eax, " + leftSide + "\n";
            // store car into [list_ptr]
            asm_result += getCurrentIndentStr() + "mov ebx, [list_ptr]\n";
            asm_result += getCurrentIndentStr() + "mov [ebx], eax\n";
            // load cdr (src2) into eax
            asm_result += getCurrentIndentStr() + "mov eax, " + rightSide + "\n";
            // Load cdr into [list_ptr + 4]
            asm_result += getCurrentIndentStr() + "mov ebx, [list_ptr]\n";
            asm_result += getCurrentIndentStr() + "mov [ebx + 4], eax\n";
            // Store address of cons cell into dst
            asm_result += getCurrentIndentStr() + "mov eax, [list_ptr]\n";
            asm_result += getCurrentIndentStr() + "mov " + dst + ", eax\n";

            listPtrCounter += 8;
            if(listPtrCounter > LIST_SPACE) {
                throw std::runtime_error("Too many cons cells -> increase LIST_SPACE. currently at: " + std::to_string(LIST_SPACE) + "bytes");
            }

            // Increment list pointer by 8 for next allocation
            asm_result += getCurrentIndentStr() + "add dword [list_ptr], 8\n";
        }


        else if(op == "car") {
            const std::string dst = instr.dst.loc;
            const std::string src = instr.src1.loc;

            asm_result += getCurrentIndentStr() + "mov ebx, " + src + "\n";       // Load pointer to cons cell
            asm_result += getCurrentIndentStr() + "mov eax, [ebx]\n";             // Load car (first 4 bytes)
            asm_result += getCurrentIndentStr() + "mov " + dst + ", eax\n";       // Store car into dst
        }

        else if(op == "cdr") {
            const std::string dst = instr.dst.loc;
            const std::string src = instr.src1.loc;

            asm_result += getCurrentIndentStr() + "mov ebx, " + src + "\n";       // Load pointer to cons cell
            asm_result += getCurrentIndentStr() + "mov eax, [ebx + 4]\n";         // Load cdr (second 4 bytes)
            asm_result += getCurrentIndentStr() + "mov " + dst + ", eax\n";       // Store cdr into dst
        }

        else if(op == "deref"){
            const std::string dst = instr.dst.loc;
            const std::string src = instr.src1.loc;

            //TODO: some kind of type safety

            
            asm_result += getCurrentIndentStr() + "mov eax, " + src + "\n";
            asm_result += getCurrentIndentStr() + "movzx ebx, byte [eax]\n";
            asm_result += getCurrentIndentStr() + "mov " + dst + ", ebx\n";
        }

        else if(op == "setchar"){
            const std::string ptrLoc = instr.dst.loc;
            const std::string indexLoc = instr.src1.loc;
            const std::string character = instr.src2.loc;
            asm_result += getCurrentIndentStr() + "mov eax, " + ptrLoc + "\n";
            asm_result += getCurrentIndentStr() + "add eax, " + indexLoc + "\n";
            asm_result += getCurrentIndentStr() + "mov ecx, " + character + "\n"; // character saved in ecx
            asm_result += getCurrentIndentStr() + "mov [eax], cl\n"; // write into memory
        }




        else {
            throw std::runtime_error("ERROR: Unhandled Instruction: " + op);
        }


        return asm_result;
    }
};



#endif //ASMGEN_HPP
