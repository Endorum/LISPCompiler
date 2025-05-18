//
// Created by Alexander on 31.03.25.
//

#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <vector>

#include "parser.hpp"


#define OPERATORS "+-*/&|^~>=<!"
#define BIN_OP "+-*/&|^>=<"

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

    // ebp + i*4 when type == PARAM
    // ebp - i*4 when type == LOCAL 
    // and if type == TEMP: 
    /*
        0 : eax
        1 : ecx
        2 : edx
        3 : ebx
    */
    std::string loc = "";
    size_t offSet = 0;

    

    Value() = default;
    Value(const std::string& n, VarType t = VT_NONE)
        : value(n), type(t) {}

    std::string str(){
        return ((!value.empty()) ? "value: " + value + ((type != VT_NONE) ? " type: " + VarTypeToString(type) : "") + ((!loc.empty()) ? (" loc: " + loc ) : "") : "");
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
        // switch (temp_count){
        //     default: break;
        //     case 0: return "eax";
        //     case 1: return "ecx";
        //     case 2: return "edx";
        //     case 3: return "ebx";
        // }
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

    bool isOperator(std::string op) {
        if(op.size() == 1) {
            return std::string(OPERATORS).find(op.at(0)) != std::string::npos;
        }
        return (op == "some other operator i guess?");
    }

    void findFuncDecls();

    Value handle_operator(ASTNode* node);
    Value handle_let_keyword(ASTNode* node);
    Value handle_defun_keyword(ASTNode* node);
    Value handle_functionCall(ASTNode* node);

    Value handle_if_keyword(ASTNode* node);
    Value handle_print_keyword(ASTNode* node);
    Value handle_cond_keyword(ASTNode * node);

    Value handle_cons_keyword(ASTNode* node);

    // Value handle_car_keyword(ASTNode* node);
    // Value handle_cdr_keyword(ASTNode* node);
    // Value handle_null_keyword(ASTNode * node);
    // Value handle_toList_keyword(ASTNode * node);
    // Value handle_toString_keyword(ASTNode * node);
    // Value handle_length_keyword(ASTNode * node);
    // Value handle_read_keyword(ASTNode * node);
    // Value handle_scan_keyword(ASTNode *node);

    Value generate_code(ASTNode* node) {
        if(!node){
            throw std::runtime_error("ERROR: node == nullptr");
        }
        if(node->type == NT_None) {
            throw std::runtime_error("ERROR: node->type == NT_None");
        }

        if(node->type == NT_List) {
            // an expression -> evaluate children
            // if first child is a number something
            // went wrong as there shouldve been a quote before that
            // 1. check if its an operator
            // 2. check for various keywords
            // 3. check if its a known function

            ASTNode* firstChild = node->children.at(0);
            if(!firstChild)
                throw std::runtime_error("FirstChild = nullptr?");


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

            
            if(isDeclaredFunction(firstChild->value)) {
                return handle_functionCall(node);
            }

            

            else {
                printf("Node with unrecognized first child:\n");

                std::cout << printASTNode(*node) << std::endl;
                throw std::runtime_error("\'" + firstChild->value + "\' was not recognized as a built in or userdefined function");
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

            if(node->type == NT_Symbol) {
                if(node->value == "nil"){
                    Value ret("nil", IMM_NUM);
                    ret.loc = "0";
                    return ret;
                }
                auto it = variable_table.find(node->value);
                if (it == variable_table.end()) {
                    throw std::runtime_error("ERROR: Variable '" + node->value + "' not declared in current scope.");
                }

                Value src = it->second;
                Value dst = generate_tmp();
                emit(dst, "load", src); // or "assign", depending on your IR naming

                return dst;
            }

            if(node->type == NT_String) {
                Value temp = generate_tmp();
                emit(temp, "loadstr", Value(node->value));
                return temp;
            }

            // if(node->type == NT_Quote) {
            //     std::string temp = generate_tmp();

            //     std::string data = serialize_quoted_list(node->children.at(0));

            //     emit(temp, data, "quote");
            //     return temp;
            // }


        }
        
        throw std::runtime_error("ERROR theres probably missing a return somewhere: " + printASTNode(*node));
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