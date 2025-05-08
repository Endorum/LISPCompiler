//
// Created by Alexander on 31.03.25.
//

#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <vector>

#include "parser.hpp"


#define OPERATORS "+-*/&|^~>=<!"
#define BIN_OP "+-*/&|^>=<"

struct IRInstruction {
    std::string dst;
    std::string op;
    std::string src1;
    std::string src2;

    [[nodiscard]] std::string str(bool debug=false) const {

        if(debug) return "dst: " + dst + " op: " + op + " src1: " + src1 + ((!src2.empty()) ? " src2: " + src2 : "");
        return  dst + " =" + ((op == "=") ? "" : " " + op)  + " " + src1 + ((!src2.empty()) ? " " + src2 : "");
    }
};

class Generator {
public:
    size_t temp_count = 0;

    std::string ir_result;

    std::unordered_map<std::string, int> variableMap; // store the name as well as the stack offset
    std::unordered_map<std::string, int> functionMap; // store name and labelOffset of function

    int StackOffset = 0;
    int labelOffset = 0;

    std::vector<ASTNode*>& program;

    std::vector<IRInstruction> instructions;

    explicit Generator(std::vector<ASTNode*>& p): program(p) {}


    void emit(const std::string& dst, const std::string& src1="", const std::string& op="", const std::string& src2="") {
        IRInstruction instruction;

        instruction.dst = dst;
        instruction.src1 = src1;
        instruction.op = op;
        instruction.src2 = src2;

        instructions.push_back(instruction);
    }

    std::string generate_tmp() {
        return "t" + std::to_string(temp_count++);
    }


    void generateIR() {
        for(auto node : program) {
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




    bool functionDeclared(std::string UD_funcName);
    bool variableDeclared(std::string name);

    bool isOperator(std::string op) {
        if(op.size() == 1) {
            return std::string(OPERATORS).find(op.at(0)) != std::string::npos;
        }
        return (op == "some other operator i guess?");
    }

    void findFuncDecls();

    std::string handle_operator(ASTNode* node);
    std::string handle_let_keyword(ASTNode* node);



    std::string generate_code(ASTNode* node) {

        if(node->type == NT_None) return "Some kind of error";

        if(node->type == NT_List) {
            // an expression -> evaluate children
            // if first child is a number something
            // went wrong as there shouldve been a quote before that
            // 1. check if its an operator
            // 2. check for various keywords

            ASTNode* firstChild = node->children.at(0);
            if(!firstChild)
                throw std::runtime_error("FirstChild = nullptr?");


            if(isOperator(firstChild->value)) {
                return handle_operator(node);
            }

            if(firstChild->value == "let") {
                return handle_let_keyword(node);
            }

            if(firstChild->value == "defun") {
                return handle_defun_keyword(node);
            }


        } else {
            // if its not an expression/list then its an atom
            if(node->type == NT_Number) {
                std::string temp = generate_tmp();
                emit(temp, node->value, "=");

                return temp;
            }

            if(node->type == NT_Symbol) {
                if(variableDeclared(node->value)) {
                    std::string temp = generate_tmp();
                    // emit(temp + " = load " + node->value);
                    emit(temp, node->value, "load");
                    return temp;
                }

                throw std::runtime_error("Variable " + node->value + " not delcared in this scope");
            }

            if(node->type == NT_Quote) {
                std::string temp = generate_tmp();

                std::string data = serialize_quoted_list(node->children.at(0));

                emit(temp, data, "quote");
                return temp;
            }


        }

        return "0";

    }













    std::string handle_defun_keyword(ASTNode* node);


    std::string handle_functionCall(ASTNode* node);



    std::string handle_if_keyword(ASTNode* node);

    std::string handle_str_keyword(ASTNode* node);

    std::string handle_length_keyword(ASTNode* node);

    std::string handle_print_keyword(ASTNode* node);

    std::string handle_car_keyword(ASTNode* node);

    std::string handle_list_keyword(ASTNode* node);

    // indexes a list (at <a list> <expr>)
    std::string handle_at_keyword(ASTNode* node);

    std::string handle_cdr_keyword(ASTNode* node);

};


// if(node->gtype == GT_Expression) {
//
//     // first child = opeator
//     if(!node->children.empty() && node->children.at(0)->gtype == GT_Operator) {
//
//         // operator
//         return handle_operator(node);
//
//     }
//
//     // handling "let" keyword: (let varname initvalue)
//     if(!node->children.empty() && node->children.at(0)->value == "let") {
//         return handle_let_keyword(node);
//     }
//
//     // handling "defun" keyword: (defun name (arg0 arg1 ...) ( body ) )
//     if (!node->children.empty() && node->children.at(0)->value == "defun") {
//         return handle_defun_keyword(node);
//     }
//
//     // handling "if" keyword (if (<cond>) (<true>) (<false>) )
//     if (!node->children.empty() && node->children.at(0)->value == "if") {
//         return handle_if_keyword(node);
//     }
//
//     // handling "str" keyword (str string index)
//     if (!node->children.empty() && node->children.at(0)->value == "str") {
//         return handle_str_keyword(node);
//     }
//
//     // handling "length" keyword (length string)
//     if (!node->children.empty() && node->children.at(0)->value == "length") {
//         return handle_length_keyword(node);
//     }
//
//     // handling "print" keyword (print string)
//     if (!node->children.empty() && node->children.at(0)->value == "print") {
//         return handle_print_keyword(node);
//     }
//
//     // handling "car" keyword (car (1 2 3)) -> 1
//     if (!node->children.empty() && node->children.at(0)->value == "car") {
//         return handle_car_keyword(node);
//     }
//
//     // handling "cdr" keyword (car (1 2 3)) -> (2 3)
//     if (!node->children.empty() && node->children.at(0)->value == "cdr") {
//         return handle_cdr_keyword(node);
//     }
//
//     // handling "list" keyword (list 1 2 3) -> (1 2 3) as an array in a variable
//     if (!node->children.empty() && node->children.at(0)->value == "list") {
//         return handle_list_keyword(node);
//     }
//
//     // handling "at" keyword (at list index) -> element at index index (0th = first obv)
//     if (!node->children.empty() && node->children.at(0)->value == "at") {
//         return handle_at_keyword(node);
//     }
//
//     // function call or builtin
//     if(!node->children.empty() && node->children.at(0)->type == NT_Symbol && node->children.at(0)->value != "defun") {
//         if(isBuiltIn(node->children.at(0)->value)) {
//             // to be implemented
//         }else {
//             // user defined function call
//             return handle_functionCall(node);
//         }
//
//
//     }
//
// } else if(node->gtype == GT_Terminal) {
//
//     if(node->type == NT_Number) {
//         std::string temp = generate_tmp();
//         emit(temp + " = " + node->value);
//         return temp;
//     }
//
//     if(node->type == NT_Symbol) {
//         std::string temp = generate_tmp();
//         emit(temp + " = load " + node->value);
//         return temp;
//     }
//
//     if(node->type == NT_String) {
//         std::string temp = generate_tmp();
//         emit(temp + " = loadstr " + node->value);
//         return temp;
//     }
//
// }
//
// printf("%s not recognized\n",node->value.c_str());
// exit(1);
//
// return "0";




#endif /*CODEGEN_HPP*/
