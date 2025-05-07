//
// Created by Alexander on 31.03.25.
//

#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <vector>

#include "parser.hpp"


#define OPERATORS "+-*/&|^~>=<!"
#define BIN_OP "+-*/&|^>=<"

class Generator {
public:
    size_t temp_count = 0;

    std::string result;

    std::unordered_map<std::string, int> variableMap; // store the name as well as the stack offset
    int nextStackOffset = 0;

    std::vector<std::unordered_map<std::string, std::string>> scopes;

    std::vector<std::string> functionNames;

    std::vector<ASTNode*>& program;

    explicit Generator(std::vector<ASTNode*>& p): program(p) {}


    void emit(const std::string &str) {
        result += str + "\n";
    }

    std::string generate_tmp() {
        return "t" + std::to_string(temp_count++);
    }

    void generate() {
        for(auto node : program) {
            generate_code(node);
        }
    }

    void enterScope() {
        scopes.push_back({});
    }

    void exitScope() {
        scopes.pop_back();
    }

    std::string declareVariable(const std::string& name, const std::string& temp) {
        scopes.back()[name] = temp;
        return temp;
    }

    std::string resolveVariable(const std::string& name) {
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            if(it->count(name)) {
                return it->at(name); // return the internal name (temp)
            }
        }

        throw std::runtime_error("Undefined Variable: " + name);
    }


    bool functionDeclared(std::string UD_funcName);
    bool variableDeclared(std::string name);

    bool isOperator(std::string op) {
        if(op.size() == 1) {
            return std::string(OPERATORS).find(op.at(0)) != std::string::npos;
        }
        return (op == "some other operator i guess?");
    }

    void declareFunctions();

    std::string handle_operator(ASTNode* node);
    std::string handle_let_keyword(ASTNode* node);

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


            if( isOperator(firstChild->value) ) {
                return handle_operator(node);
            }

            if( firstChild->value == "let" ) {
                return handle_let_keyword(node);
            }

            if(firstChild->value == "defun") {
                return handle_defun_keyword(node);
            }

            if(firstChild->value == "IR") {
                if(node->children.size() != 3 || node->children.at(1)->type != NT_String || node->children.at(2)->type != NT_Number) {
                    throw std::runtime_error("Expected a string and an integer");
                }

                size_t tmpSave = temp_count;

                if(node->children.at(1)) {
                    emit("\n# IR injection: \n" + node->children.at(1)->value + "\n# End of IR injection\n");
                }

                temp_count = tmpSave + std::stoi(node->children.at(2)->value);

                return "";
            }




        } else {
            // if its not an expression/list then its an atom
            if(node->type == NT_Number) {
                std::string temp = generate_tmp();
                emit(temp + " = " + node->value);
                return temp;
            }

            if(node->type == NT_Symbol) {
                if(variableDeclared(node->value)) {
                    std::string temp = generate_tmp();
                    emit(temp + " = load " + node->value);
                    return temp;
                }

                throw std::runtime_error("Variable " + node->value + " not delcared in this scope");




            }
        }

        return "0";

    }


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
