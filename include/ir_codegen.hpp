//
// Created by Alexander on 31.03.25.
//

#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <vector>

#include "parser.hpp"


#define OPERATORS "+-*/&|^~>=<!%"
#define BIN_OP "+-*/&|^>=<%"



typedef enum VarType{
    VT_NONE,
    PARAM,       // PARAMument in a function  [rbp + offset]
    LOCAL,     // User defined using let  [rbp - offset]
    TEMP,      // like t0 etc.            reused or mapped to registers
    IMM_NUM,   // like 1 2 3 123.134     
    IMM_STR,   // self explanitory
    REG,       // eax mostly
}VarType;

inline std::string VarTypeToString(VarType type){
    switch (type){
        default: return "NONE";
        case PARAM: return "PARAM";
        case LOCAL: return "LOCAL";
        case TEMP: return "TEMP";
        case IMM_NUM: return "IMM_NUM";
        case IMM_STR: return "IMM_STR";
        case REG: return "REG";
    }
}

struct Value{
    
    std::string value;
    VarType type = VT_NONE;

    std::string loc = "";
    size_t offSet = 0;

    // in let the dst variable should pass information about the type
    VarType valueType = VT_NONE; // for preventing segfaults when doing deref etc

    

    Value() = default;
    Value(const std::string& n, VarType t = VT_NONE)
        : value(n), type(t) {}

    std::string str(){
        return ((!value.empty()) ? "value: " + value + ((type != VT_NONE) ? " type: " + VarTypeToString(type) : "") + ((!loc.empty()) ? (" loc: " + loc ) : "") + " ValueType: " + VarTypeToString(valueType) : "");
    }

};

struct IRInstruction {
    Value dst;
    std::string op;
    Value src1;
    Value src2;
    

    std::string str(){
        std::string dstString = "dst: " + dst.str();
        
        std::string src1String = (!src1.value.empty()) ? "src1: " + src1.str() + "\n": "";
        std::string src2String = (!src2.value.empty()) ? "src2: " + src2.str() + "\n": "";
        return dstString + "\n" + op + "\n" + src1String + src2String;
    }

};

struct Function {
    std::string name;
    
    size_t localVarCount = 0;

    size_t paramCount = 0;
    size_t stackFrameSize = 0;

    

    

    std::vector<IRInstruction> instructions;
};

class Generator {
public:
    
    std::vector<ASTNode*>& ast;

    std::vector<std::string> declaredFunctions;
    std::vector<Function> functions; 
    Function current_function;
    
    std::unordered_map<std::string, Value> variable_table; // track local variables including parameters

    std::vector<IRInstruction> instructions;

    size_t local_count = 0; // for all let variables
    size_t temp_count = 0;  // for all temps (t0 ...)
    size_t spill_start = 1; // calculated after locals are declared
    size_t labelId = 0; // for lables like if etc

    size_t heap_ptr = 0;

    explicit Generator(std::vector<ASTNode*>& p): ast(p) {
        current_function.name = "GLOBAL";
    }


    void printError(const std::string& msg);


    void emit(Value dst, std::string op);
    void emit(Value dst, std::string op, Value src1);
    void emit(Value dst, std::string op, Value src1, Value src2);

    

    std::string getPARAMLocation(size_t index) {
        return "[ebp + " + std::to_string(8 + index * 4) + "]";
    }

    std::string getLocalLocation(size_t index) {
        return "[ebp - " + std::to_string((index + 1) * 4)+"]";
    }

    int countLocalsInFunction(ASTNode* functionBody) {
        int count = 0;

        // Walk the AST subtree recursively:
        std::function<void(ASTNode*)> walk = [&](ASTNode* node) {
            if (!node) return;

            if (node->type == NT_List && !node->children.empty()) {
                ASTNode* first = node->children[0];
                if (first->value == "let") {
                    count++;
                }
                for (auto child : node->children) {
                    walk(child);
                }
            } else {
                // For atoms or other nodes, no locals here
            }
        };

        walk(functionBody);

        return count;
    }



    std::string getTempOffset(int index){
        // cant use the registers anymore as they are needed for asm gen e.g. idiv 
        switch (temp_count){
            default: break;
            case 0: return "[ebp]";
            case 1: return "esi";
            case 2: return "edi";
            case 3: return "[t0]";
            case 4: return "[t1]";
            case 5: return "[t2]";
            case 6: return "[t3]";
            case 7: return "[t4]";
            case 8: return "[t5]";
            case 9: return "[t6]";
            case 10: return "[t7]";
            case 11: return "[t8]";
            case 12: return "[t9]";
        }
        return "[ebp - " + std::to_string(++local_count * 4) + "]"; // spill after locals
    }

    Value generate_tmp() {
        Value temp;
        temp.value = "t" + std::to_string(temp_count);
        temp.type = TEMP;
        temp.offSet = temp_count;
        temp.loc = getTempOffset(temp_count);
        temp_count++;
        return temp;
    }

    Value create_local(const std::string& name) {
        Value local;
        local.value = name;
        local.type = LOCAL;
        local.offSet = local_count;
        local.loc = getLocalLocation(local_count);
        local_count++;
        return local;
    }

    Value create_parameter(const std::string& name, size_t index){
        Value param;
        param.value = name;
        param.type = PARAM;
        param.offSet = index; // index of parameter
        param.loc = "[ebp + " + std::to_string(8 + index * 4) + "]"; // 8 to let room for old bp and ret addr
        return param;
    }

    void prepareFunctionFrame() {
        // call this after parsing function header
        spill_start = local_count;
        temp_count = 0; // reset for each function
    }


    void generateIR() {
        for(auto node : ast) {
            generate_code(node);
        }
    }

    std::string serialize_quoted_list(ASTNode* node) {
        if (node->type == NT_List) {
            std::string result = "(";
            for (size_t i = 0; i < node->children.size(); ++i) {
                result += serialize_quoted_list(node->children[i]);
                if (i + 1 < node->children.size()) result += " ";
            }
            result += ")";
            return result;
        } if (node->type == NT_Symbol || node->type == NT_Number) {
            return node->value;
        } else {
            return "<unsupported>";
        }
    }




    bool isDeclaredFunction(std::string UD_funcName);
    bool isDeclaredVariable(std::string name);

    std::string wordToChar(std::string op);

    bool isOperator(std::string op) {
        op = wordToChar(op);

        if(op.size() == 1) {
            return std::string(OPERATORS).find(op.at(0)) != std::string::npos;
        }
        return (
            op == "!=" ||
            op == ">=" ||
            op == "<=" ||
            op == "+=" ||
            op == "-=" ||
            op == "*=" ||
            op == "/=" ||
            op == "<<" ||
            op == ">>" ||
            op == "++" ||
            op == "--" 
            // op == "<<=" || 
            // op == ">>=" 
        );
    }

    void findFuncDecls();

    Value handle_operator(ASTNode* node);
    Value handle_let_keyword(ASTNode* node);
    Value handle_set_keyword(ASTNode* node);
    Value handle_defun_keyword(ASTNode* node);
    Value handle_functionCall(ASTNode* node);
    Value handle_if_keyword(ASTNode* node);
    Value handle_print_keyword(ASTNode* node);
    Value handle_cond_keyword(ASTNode * node);
    Value handle_cons_keyword(ASTNode* node);
    Value handle_car_keyword(ASTNode* node);
    Value handle_cdr_keyword(ASTNode* node);
    Value handle_deref_keyword(ASTNode* node);
    Value handle_while_keyword(ASTNode* node);
    Value handle_setbyte_keyword(ASTNode* node);
    Value handle_malloc_keyword(ASTNode* node);
    Value handle_free_keyword(ASTNode* node);

    // Value handle_null_keyword(ASTNode * node);
    // Value handle_toList_keyword(ASTNode * node);
    // Value handle_toString_keyword(ASTNode * node);
    // Value handle_length_keyword(ASTNode * node);
    // Value handle_read_keyword(ASTNode * node);
    // Value handle_scan_keyword(ASTNode *node);

    Value generate_code(ASTNode* node) {
        if(!node){
            throw std::runtime_error("ir_codegen.hpp: ERROR: node == nullptr");
        }
        if(node->type == NT_None) {
            throw std::runtime_error("ir_codegen.hpp: ERROR: node->type == NT_None");
        }

        if(node->type == NT_List) {
            // an expression -> evaluate children
            // if first child is a number something
            // went wrong as there shouldve been a quote before that
            // 1. check if its an operator
            // 2. check for various keywords
            // 3. check if its a known function

            if(node->children.size() <= 0){
                throw std::runtime_error("ir_codegen.hpp: Empty expression");
            }

            ASTNode* firstChild = node->children.at(0);
            if(!firstChild){
                throw std::runtime_error("ir_codegen.hpp: FirstChild = nullptr?");
            }

            


            if(isOperator(firstChild->value)) {
                return handle_operator(node);
            }

            // (defun add (x y) (+ x y) )
            if(firstChild->value == "defun") {
                return handle_defun_keyword(node);
            }


            // (let x 5)
            if(firstChild->value == "let") {
                return handle_let_keyword(node);
            }

            if(firstChild->value == "set") { // just an alias witch checking if variable is declared
                return handle_set_keyword(node);
            }

            if(firstChild->value == "if") {
                return handle_if_keyword(node);
            }

            if(firstChild->value == "print"){
                return handle_print_keyword(node);
            }

            if(firstChild->value == "cond"){
                return handle_cond_keyword(node);
            }

            if(firstChild->value == "cons"){
                return handle_cons_keyword(node);
            }

            if(firstChild->value == "car"){
                return handle_car_keyword(node);
            }

            if(firstChild->value == "cdr"){
                return handle_cdr_keyword(node);
            }

            // (deref x) reads value stored AT x ( [x] )
            if(firstChild->value == "deref"){
                return handle_deref_keyword(node);
            }

            if(firstChild->value == "while"){
                return handle_while_keyword(node);
            }

            if(firstChild->value == "setbyte"){
                return handle_setbyte_keyword(node);
            }

            if(firstChild->value == "malloc"){
                return handle_malloc_keyword(node);
            }

            if(firstChild->value == "free"){
                return handle_free_keyword(node);
            }

            
            if(isDeclaredFunction(firstChild->value)) {
                return handle_functionCall(node);
            }

            if(firstChild->type == NT_List){
                Value retval;
                for(int i=0;i<firstChild->children.size();i++){
                    retval = generate_code(firstChild->children.at(i));
                }
                return retval;
            }
            

            else {
                // if first is not a function maybe (variable) was meant and should be interpreted as such
                
                printf("Node with unrecognized first child:\n");

                std::cout << printASTNode(*node) << std::endl;
                throw std::runtime_error("ir_codegen.hpp: \'" + firstChild->value + "\' was not recognized as a built in or userdefined function");
            }


        } else {
            // if its not an expression/list then its an atom
            if(node->type == NT_Number) {
                Value immVal(node->value, IMM_NUM);
                immVal.loc = node->value;

                // isnt that much simpler? (commented out in case i still need it) i do because later i just use instr.loc 
                // but thats an easy fix just put the number into instr.loc 

                // Value dst = generate_tmp();

                // emit(dst, "assign", immVal);

                return immVal;
            }

            if (node->type == NT_Symbol) {
                if (node->value == "nil") {
                    Value ret("nil", IMM_NUM);
                    ret.loc = "0";
                    return ret;
                }

                std::string scope = "";
                for (const auto& pair : variable_table) {
                    scope += "Variable: \'" + pair.first + "\'\n";
                }

                auto it = variable_table.find(node->value);
                if (it != variable_table.end()) {
                    // Variable is found
                    Value src = it->second;
                    Value dst = generate_tmp();

                    dst.valueType = src.type;
                    emit(dst, "load", src); // or "assign", depending on your IR naming

                    return dst;
                } else if (isDeclaredFunction(node->value)) {
                    // Symbol is a declared function
                    // return Value("tried to call function: " + node->value);
                    return handle_functionCall(node);
                } else {
                    // Neither variable nor function is found — throw an error
                    throw std::runtime_error("ir_codegen.hpp: ERROR: Variable or Function '" + node->value +
                                            "' not declared in current scope. \nScope:\n" + scope);
                }
            }


            if(node->type == NT_String) {
                Value temp = generate_tmp();
                Value str = Value(node->value, IMM_STR);
                temp.valueType = IMM_STR;
                emit(temp, "loadstr", str);
                return temp;
            }

            // if(node->type == NT_Quote) {
            //     std::string temp = generate_tmp();

            //     std::string data = serialize_quoted_list(node->children.at(0));

            //     emit(temp, data, "quote");
            //     return temp;
            // }


        }
        
        throw std::runtime_error("ir_codegen.hpp: ERROR theres probably missing a return somewhere: " + printASTNode(*node));
    }

};

#endif /*CODEGEN_HPP*/


// // (if (> x y) x y))
// if(firstChild->value == "if") {
//     return handle_if_keyword(node);
// }

// // constructs a new pair
// // (cons 1 '(2 3)) -> (1 2 3)
// // (cons 1 (cons 2 3)) -> (1 2 3)
// if(firstChild->value == "cons") {
//     return handle_cons_keyword(node);
// }

// // (car '(1 2 3)) -> 1
// if(firstChild->value == "car") {
//     return handle_car_keyword(node);
// }

// // (cdr '(1 2 3)) -> (2 3)
// if(firstChild->value == "cdr") {
//     return handle_cdr_keyword(node);
// }

// if(firstChild->value == "null?") {
//     return handle_null_keyword(node);
// }

// // (toList "test") -> '(116(t) 101(e) 115(s) 116(t))
// // this way i can unify string length and list length as well as the indexing etc.
// // and car and cdr etc. could still be used.
// if(firstChild->value == "toList") {
//     return handle_toList_keyword(node);
// }

// // (toString '(116 101 115 116) -> "test"
// if(firstChild->value == "toString") {
//     return handle_toString_keyword(node);
// }

// // (lenght '(1 2 3)) -> 3
// if(firstChild->value == "length") {
//     return handle_length_keyword(node);
// }

// // (print "hello World") stdout -> "hello World"
// if(firstChild->value == "print") {
//     return handle_print_keyword(node);
// }

// // (read "filename")
// if(firstChild->value == "read") {
//     return handle_read_keyword(node);
// }

// (cond
//      (<condition1> <expr1>)
//      (<condition2> <expr2>)
//      (<condition3> <expr3>)
//      (else <exprN>)
// )
// if(firstChild->value == "cond") {
//     return handle_cond_keyword(node);
// }



// if(firstChild->type == NT_List) {
//     return generate_code(firstChild);
// }