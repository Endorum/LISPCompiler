//
// Created by Alexander on 08.05.25.
//

#include "../include/asmgen.hpp"

std::string Asmgen::cleanup() {
        
    std::istringstream iss(asm_result);
    std::vector<std::string> lines;
    std::string line;

    // Read all lines into a vector
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    std::vector<std::string> cleaned_lines;

    // Regex for mov instruction with two operands
    std::regex mov_pattern(R"(^\s*mov\s+([^,]+),\s*(.+)\s*$)", std::regex::icase);

    for (size_t i = 0; i < lines.size(); ++i) {
        if (i + 1 < lines.size()) {
            std::smatch m1, m2;

            if (std::regex_match(lines[i], m1, mov_pattern) && std::regex_match(lines[i + 1], m2, mov_pattern)) {
                std::string dst1 = m1[1].str();
                std::string src1 = m1[2].str();

                std::string dst2 = m2[1].str();
                std::string src2 = m2[2].str();

                // Trim whitespace from operands (optional but recommended)
                auto trim = [](std::string& s) {
                    size_t start = s.find_first_not_of(" \t");
                    size_t end = s.find_last_not_of(" \t");
                    if (start == std::string::npos) s = "";
                    else s = s.substr(start, end - start + 1);
                };

                trim(dst1);
                trim(src1);
                trim(dst2);
                trim(src2);

                if (dst1 == src2 && src1 == dst2) {
                    // Found matching mov X, Y and mov Y, X pair — skip both lines
                    i++;
                    continue;
                }
                }
            }

            cleaned_lines.push_back(lines[i]);
        }

        std::ostringstream oss;
        for (auto& l : cleaned_lines) {
            oss << l << "\n";
        }

        asm_result = oss.str();
        return asm_result;
}

std::string Asmgen::formatStringForNASM(const std::string& input){
     {
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
}

std::string Asmgen::_mov(Value dst, Value src) {
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

void Asmgen::generate_assign(IRInstruction& instr){
    if (instr.src1.type == IMM_NUM){
        asm_result += getCurrentIndentStr() + "mov dword " + instr.dst.loc + ", " + instr.src1.value + "\n";
    }else{
        asm_result += getCurrentIndentStr() + _mov(instr.dst, instr.src1);
    }
}

void Asmgen::generate_add(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "add eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_sub(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "sub eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_imul(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "imul eax, " + instr.src2.loc + "\n"; // edx:eax if (a > 2^16) && (b > 2^16), the result is wrong, as the rest is stored in edx, but in only have one integer type right now
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_idiv(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cdq\n"; // sign extend eax into edx:eax
    asm_result += getCurrentIndentStr() + "idiv dword " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}

void Asmgen::generate_and(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "and eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_or(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "or eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_xor(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "xor eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_less(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "setl al\n";
    asm_result += getCurrentIndentStr() + "and al, 1\n";
    asm_result += getCurrentIndentStr() + "movzx eax, al\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_less_equal(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "setle al\n";
    asm_result += getCurrentIndentStr() + "and al, 1\n";
    asm_result += getCurrentIndentStr() + "movzx eax, al\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_equal(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "sete al\n";
    asm_result += getCurrentIndentStr() + "and al, 1\n";
    asm_result += getCurrentIndentStr() + "movzx eax, al\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_not_equal(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "sete al\n";
    asm_result += getCurrentIndentStr() + "and al, 1\n";
    asm_result += getCurrentIndentStr() + "movzx eax, al\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_greater(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "setg al\n";
    asm_result += getCurrentIndentStr() + "and al, 1\n";
    asm_result += getCurrentIndentStr() + "movzx eax, al\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_greater_equal(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cmp eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "setge al\n";
    asm_result += getCurrentIndentStr() + "and al, 1\n";
    asm_result += getCurrentIndentStr() + "movzx eax, al\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}

void Asmgen::generate_mod(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "cdq\n";
    asm_result += getCurrentIndentStr() + "idiv " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + "\n";
}
void Asmgen::generate_shl(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov ecx, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "shl eax, cl\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_shr(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov ecx, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "sar eax, cl\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_neg(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "xor eax, -1\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_not(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "cmp " + instr.src1.loc + ", 0\n";
    asm_result += getCurrentIndentStr() + "setne al\n";
    asm_result += getCurrentIndentStr() + "xor al -1\n";
    asm_result += getCurrentIndentStr() + "and al 1\n";
    asm_result += getCurrentIndentStr() + "movzx eax, al\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}
void Asmgen::generate_addeq(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "add eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", eax\n";
}
void Asmgen::generate_subeq(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "sub eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", eax\n";
}
void Asmgen::generate_imuleq(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "imul eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", eax\n";
}
void Asmgen::generate_idiveq(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov ebx, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src2.loc + "\n";
    asm_result += getCurrentIndentStr() + "cdq\n";
    asm_result += getCurrentIndentStr() + "idiv ebx\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", eax\n";
}
void Asmgen::generate_inc(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "add eax, 1\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", eax\n";
}
void Asmgen::generate_dec(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "\n";
    asm_result += getCurrentIndentStr() + "sub eax, 1\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.src1.loc + ", eax\n";
}
void Asmgen::generate_load(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + _mov(instr.dst, instr.src1);
}
void Asmgen::generate_defun(IRInstruction& instr){
    currentIndent = 1;
    
    std::string funcName = instr.src1.value;
    Function func;
    for(auto& f : functions){
        if(f.name == funcName) {func = f; break;}
    }

    int local_bytes = func.localVarCount * 4;
    
    asm_result += funcName + ": ; " + std::to_string(func.paramCount) +" parameters \n";
    // stack frame
    asm_result += getCurrentIndentStr() + "push ebp\n";
    asm_result += getCurrentIndentStr() + "mov ebp, esp\n";
    asm_result += getCurrentIndentStr() + "sub esp, " + std::to_string(local_bytes) + "\n"; // adjusting the stack for local vars (let)
}
void Asmgen::generate_push(IRInstruction& instr){
    if (instr.src1.type == IMM_NUM) {
        asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.value + "\n";
        asm_result += getCurrentIndentStr() + "push dword eax\n";
    } else {
        // instr.src1.loc is a register or memory location, can push directly
        asm_result += getCurrentIndentStr() + "push dword " + instr.src1.loc + "\n";
    }
}
void Asmgen::generate_return(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + _mov(instr.dst,instr.src1);
    asm_result += getCurrentIndentStr() + "mov esp, ebp\n";
    asm_result += getCurrentIndentStr() + "pop ebp\n";
    asm_result += getCurrentIndentStr() + "ret\n";
    currentIndent = 0;
}
void Asmgen::generate_call(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "call " + instr.src1.value + "\n";
}
void Asmgen::generate_add_esp(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "add esp, " + std::to_string(std::stoi(instr.src1.value)) + "\n";
}
void Asmgen::generate_loadstr(IRInstruction& instr){
    std::string str = instr.src1.value;
    std::string label = "str_" + std::to_string(stringCounter++);
    
    dataSection += label + ": db " + formatStringForNASM(str) + ", 0\n";
    asm_result += getCurrentIndentStr() + "mov eax, " + label + "\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax\n";
}

void Asmgen::generate_print(IRInstruction& instr){
    std::string stringAddr = instr.src1.loc;
    asm_result += getCurrentIndentStr() + "mov eax, " + stringAddr + "\n";
    asm_result += getCurrentIndentStr() + "call _internal_print_string\n";
}
void Asmgen::generate_if(IRInstruction& instr){
    std::string trueLabel = instr.dst.value;
    std::string falseLabel = instr.src2.value;

    std::string condLoc = instr.src1.loc;

    asm_result += getCurrentIndentStr() + "mov eax, " + condLoc + "\n";
    asm_result += getCurrentIndentStr() + "cmp eax, 0\n";
    asm_result += getCurrentIndentStr() + "je " + falseLabel + "\n";
    asm_result += getCurrentIndentStr() + "jmp " + trueLabel + "\n";

}
void Asmgen::generate_label(IRInstruction& instr){
    asm_result += instr.dst.value + ":\n";
}
void Asmgen::generate_jump(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "jmp " + instr.dst.value + "\n";
}
void Asmgen::generate_cons(IRInstruction& instr){
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
        throw std::runtime_error("asmgen.hpp: Too many cons cells -> increase LIST_SPACE. currently at: " + std::to_string(LIST_SPACE) + "bytes");
    }

    // Increment list pointer by 8 for next allocation
    asm_result += getCurrentIndentStr() + "add dword [list_ptr], 8\n";
}
void Asmgen::generate_car(IRInstruction& instr){
    const std::string dst = instr.dst.loc;
    const std::string src = instr.src1.loc;

    asm_result += getCurrentIndentStr() + "mov ebx, " + src + "\n";       // Load pointer to cons cell
    asm_result += getCurrentIndentStr() + "mov eax, [ebx]\n";             // Load car (first 4 bytes)
    asm_result += getCurrentIndentStr() + "mov " + dst + ", eax\n";       // Store car into dst
}
void Asmgen::generate_cdr(IRInstruction& instr){
    const std::string dst = instr.dst.loc;
    const std::string src = instr.src1.loc;

    asm_result += getCurrentIndentStr() + "mov ebx, " + src + "\n";       // Load pointer to cons cell
    asm_result += getCurrentIndentStr() + "mov eax, [ebx + 4]\n";         // Load cdr (second 4 bytes)
    asm_result += getCurrentIndentStr() + "mov " + dst + ", eax\n";       // Store cdr into dst
}
void Asmgen::generate_deref(IRInstruction& instr){
    const std::string dst = instr.dst.loc;
    const std::string src = instr.src1.loc;

    //TODO: some kind of type safety

    
    asm_result += getCurrentIndentStr() + "mov eax, " + src + "\n";
    asm_result += getCurrentIndentStr() + "movzx ebx, byte [eax]\n";
    asm_result += getCurrentIndentStr() + "mov " + dst + ", ebx\n";
}
void Asmgen::generate_setbyte(IRInstruction& instr){
    const std::string ptrLoc = instr.dst.loc;
    const std::string indexLoc = instr.src1.loc;
    const std::string character = instr.src2.loc;
    asm_result += getCurrentIndentStr() + "mov eax, " + ptrLoc + "\n";
    asm_result += getCurrentIndentStr() + "add eax, " + indexLoc + "\n";
    asm_result += getCurrentIndentStr() + "mov ecx, " + character + "\n"; // character saved in ecx
    asm_result += getCurrentIndentStr() + "mov [eax], cl\n"; // write into memory
}
void Asmgen::generate_malloc(IRInstruction& instr){
    // TODO: test internal malloc and free
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "; block size\n";
    asm_result += getCurrentIndentStr() + "call malloc ; calling internal malloc\n";
    asm_result += getCurrentIndentStr() + "mov " + instr.dst.loc + ", eax ; output pointer in eax\n";
}
void Asmgen::generate_free(IRInstruction& instr){
    asm_result += getCurrentIndentStr() + "mov eax, " + instr.src1.loc + "; ptr\n";
    asm_result += getCurrentIndentStr() + "call free ; calling internal free\n";
}
