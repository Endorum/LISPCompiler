//
// Created by Alexander on 08.05.25.
//

#ifndef ASMGEN_HPP
#define ASMGEN_HPP
#include <string>
#include <map>
#include <regex>

#define START_SYMBOL "_start"
#define CLEANUP true // _should_ work 
#define REMOVE_TWO_WAY_MOV false // not tested yet

// so space for around 100k list items
// 1048576 = 2^20
#define LIST_SPACE 8192// / 8 because we're working with full 2x 32 bit integers here (car and cdr)
#define HEAP_SIZE  8192


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
        dataSection += "t0: dd 0\n";
        dataSection += "t1: dd 0\n";
        dataSection += "t2: dd 0\n";
        dataSection += "t3: dd 0\n";
        dataSection += "t4: dd 0\n";
        dataSection += "t5: dd 0\n";
        dataSection += "t6: dd 0\n";
        dataSection += "t7: dd 0\n";
        dataSection += "t8: dd 0\n";
        dataSection += "t9: dd 0\n";
        dataSection += "list_ptr: dd list_memory ; pointer to next free cell\n";
        dataSection += "heap_ptr: dd heap_start ; pointer to heap begin\n";
        dataSection += "free_list: dd heap_start\n";
        

        dataSection += "section .bss\n";
        dataSection += "list_memory: resb " + std::to_string(LIST_SPACE) + "; reserved (uninitialized!!) space for cons cells\n";
        dataSection += "heap_start: resb " + std::to_string(HEAP_SIZE) + "; heap space\n";
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
    int currentIndent = 0;


    std::string dataSection = "section .data\n";
    std::string start = START_SYMBOL;
    std::string asm_overhead = R"(
%include "stdlib.asm"
section .text
global _start
_start: 
    ;; init malloc
    mov eax, )" + std::to_string(HEAP_SIZE) + R"(
    mov [heap_start], eax ; size
    mov dword [heap_start+4], 0 ; next = 0
    mov dword [free_list], heap_start
    call main
    mov eax, 1
    mov ebx, 0
    int 0x80
    ret)";

    void generate_assign(IRInstruction& instr);
    void generate_add(IRInstruction& instr);
    void generate_sub(IRInstruction& instr);
    void generate_imul(IRInstruction& instr);
    void generate_idiv(IRInstruction& instr);
    void generate_and(IRInstruction& instr);
    void generate_or(IRInstruction& instr);
    void generate_xor(IRInstruction& instr);
    void generate_less(IRInstruction& instr);
    void generate_less_equal(IRInstruction& instr);
    void generate_equal(IRInstruction& instr);
    void generate_not_equal(IRInstruction& instr);
    void generate_greater(IRInstruction& instr);
    void generate_greater_equal(IRInstruction& instr);
    void generate_mod(IRInstruction& instr);
    void generate_shl(IRInstruction& instr);
    void generate_shr(IRInstruction& instr);
    void generate_neg(IRInstruction& instr);
    void generate_not(IRInstruction& instr);
    void generate_addeq(IRInstruction& instr);
    void generate_subeq(IRInstruction& instr);
    void generate_imuleq(IRInstruction& instr);
    void generate_idiveq(IRInstruction& instr);
    void generate_inc(IRInstruction& instr);
    void generate_dec(IRInstruction& instr);
    void generate_load(IRInstruction& instr);
    void generate_defun(IRInstruction& instr);
    void generate_push(IRInstruction& instr);
    void generate_return(IRInstruction& instr);
    void generate_call(IRInstruction& instr);
    void generate_add_esp(IRInstruction& instr);
    void generate_loadstr(IRInstruction& instr);
    void generate_print(IRInstruction& instr);
    void generate_if(IRInstruction& instr);
    void generate_label(IRInstruction& instr);
    void generate_jump(IRInstruction& instr);
    void generate_cons(IRInstruction& instr);
    void generate_car(IRInstruction& instr);
    void generate_cdr(IRInstruction& instr);
    void generate_deref(IRInstruction& instr);
    void generate_let(IRInstruction& instr);
    void generate_set(IRInstruction& instr);
    void generate_cond(IRInstruction& instr);
    void generate_while(IRInstruction& instr);
    void generate_setbyte(IRInstruction& instr);
    void generate_malloc(IRInstruction& instr);
    void generate_free(IRInstruction& instr);

    

    std::string getCurrentIndentStr() const{
        return (currentIndent != 0) ? "    " : "";
    }

    

    std::string _mov(Value dst, Value src);


    bool isNumber(const std::string& s) {
        if (s.empty()) return false;
        for (char c : s) {
            if (!std::isdigit(c) && c != '-') return false;
        }
        return true;
    }

    void optimize_redundant_movs(std::string& asm_code);
    std::string cleanup();

    std::string formatStringForNASM(const std::string& input);
    


    std::string generate_asm(IRInstruction& instr) {
        const std::string op = instr.op;
        if(op.empty()) {
            throw std::runtime_error("asmgen.hpp: ERROR: op was empty");
        }

        if(op == "assign") generate_assign(instr);
        else if(op == "add") generate_add(instr);
        else if(op == "sub") generate_sub(instr);
        else if(op == "imul") generate_imul(instr);
        else if(op == "idiv") generate_idiv(instr);
        else if(op == "and") generate_and(instr);
        else if(op == "or") generate_or(instr);
        else if(op == "xor") generate_xor(instr);
        else if(op == "less") generate_less(instr);
        else if(op == "less_equal") generate_less_equal(instr);
        else if(op == "equal") generate_equal(instr);
        else if(op == "not_equal") generate_not_equal(instr);
        else if(op == "greater") generate_greater(instr);
        else if(op == "greater_equal") generate_greater_equal(instr);
        else if(op == "mod") generate_mod(instr);
        else if(op == "shl") generate_shl(instr);
        else if(op == "shr") generate_shr(instr);
        else if(op == "neg") generate_neg(instr);
        else if(op == "not") generate_not(instr);
        else if(op == "addeq") generate_addeq(instr);
        else if(op == "subeq") generate_subeq(instr);
        else if(op == "imuleq") generate_imuleq(instr);
        else if(op == "idiveq") generate_idiveq(instr);
        else if(op == "inc") generate_inc(instr);
        else if(op == "dec") generate_dec(instr);
        else if(op == "load") generate_load(instr);
        else if(op == "defun") generate_defun(instr);
        else if(op == "push") generate_push(instr);
        else if(op == "return") generate_return(instr);
        else if(op == "call") generate_call(instr);
        else if(op == "add_esp") generate_add_esp(instr);
        else if(op == "loadstr") generate_loadstr(instr);
        else if(op == "print") generate_print(instr);
        else if(op == "if") generate_if(instr);
        else if(op == "label") generate_label(instr);
        else if(op == "jump") generate_jump(instr);
        else if(op == "cons") generate_cons(instr);
        else if(op == "car") generate_car(instr);
        else if(op == "cdr") generate_cdr(instr);
        else if(op == "deref") generate_deref(instr);
        else if(op == "setbyte") generate_setbyte(instr);
        else if(op == "malloc") generate_malloc(instr);
        else if(op == "free") generate_free(instr);


        else {
            throw std::runtime_error("asmgen.hpp: ERROR: Unhandled Instruction: " + op);
        }


        return asm_result;
    }
};



#endif //ASMGEN_HPP
