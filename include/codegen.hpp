//
// Created by Alexander on 31.03.25.
//

#ifndef CODEGEN_HPP
#define CODEGEN_HPP
#include <vector>
#include <unordered_set>

#include "parser.hpp"


class Generator {
public:
    std::vector<ASTNode*>& program;
    size_t temp_count = 0;

    std::string result;

    std::vector<std::string> functionNames;

    Generator(std::vector<ASTNode*>& p) : program(p) {

    }

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

    bool isBinary(const char c) {
        return (
            c == '+' || c == '-' || c == '*' || c == '/' || /* Binary Arithemtic */
            c == '<' || c == '>' || c == '=' /* Comparisons */
        );
    }

    bool isBuiltIn(std::string str) {
        return false;
    }

    bool functionDeclared(std::string UD_funcName) {
        for(auto funcName : functionNames) {
            if(UD_funcName == funcName) return true;
        }
        return false;
    }

#include <unordered_set>

    void declareFunctions() {
        std::unordered_set<std::string> seenFunctions;

        for (auto node : program) {
            if (node->children.size() > 0 && node->children.at(0)->value == "defun") {
                std::string funcName = node->children.at(1)->value;

                if (seenFunctions.count(funcName)) {
                    throw std::runtime_error("ERROR: Function '" + funcName + "' is declared multiple times");
                }

                seenFunctions.insert(funcName);
                functionNames.push_back(funcName);
            }
        }
    }

    std::string handle_functionCall(ASTNode* node) {
        std::string UDfuncName = node->children.at(0)->value;

        if (!functionDeclared(UDfuncName)) {
            throw std::runtime_error("ERROR: Function " + UDfuncName + " is not declared");
        }

        // child 1 and onwards are the parameters
        for (size_t i = 1; i < node->children.size(); ++i) {
            ASTNode* param = node->children[i];
            std::string res = generate_code(param);
            emit("push " + res);
        }

        emit("call " + UDfuncName);

        // after call, result is "somewhere" — but you might want to make it explicit
        std::string temp = generate_tmp();
        emit(temp + " = return_value"); // you need to decide how you model return values
        return temp;
    }


    std::string handle_singCharOp(ASTNode* node) {
        const char op = node->children.at(0)->value[0];

        // single char binary op
        if(isBinary(op)) {
            if (node->children.size() < 3) {
                throw std::runtime_error("ERROR: Expected <op> <arg1> <arg2>");
            }

            std::string left = generate_code(node->children.at(1));
            std::string right = generate_code(node->children.at(2));
            std::string temp = generate_tmp();

            emit(temp + " = " + left + " " + op + " " + right);

            return temp;

        }
    }

    std::string handle_let_keyword(ASTNode* node) {
        // first is the name
        std::string varname = node->children.at(1)->value;
        std::string temp = generate_code(node->children.at(2));
        emit("store " + varname + " = " + temp);
        return temp;
    }

    std::string handle_defun_keyword(ASTNode* node) {
        std::string functionName = node->children.at(1)->value;
        ASTNode* parameters = node->children.at(2);
        ASTNode* body = node->children.at(3);

        if (!parameters || !body) {
            throw std::runtime_error("ERROR: Expected parameters and body");
        }

        // Emit the function label
        emit(functionName + ":");

        for(int i=parameters->children.size() - 1;i>=0;i--) {
            emit(parameters->children.at(i)->value + " = pop");
        }

        // Generate code for the body
        std::string result = generate_code(body);

        // Emit a return instruction
        emit("return " + result);

        return "";
    }

    std::string generate_code(ASTNode* node) {
        if(node->gtype == GT_Expression) {

            // first child = opeator
            if(node->children.size() > 0 && node->children.at(0)->gtype == GT_Operator) {

                // single char op
                if(node->children.at(0)->value.size() == 1){
                    return handle_singCharOp(node);
                }

            }

            // handling "let" keyword: (let varname initvalue)
            if(node->children.size() > 0 && node->children.at(0)->value == "let") {
                return handle_let_keyword(node);
            }

            // handlin "defun" keyword: (defun name (arg0 arg1 ...) ( body ) )
            if (node->children.size() > 0 && node->children.at(0)->value == "defun") {
                return handle_defun_keyword(node);
            }

            // function call or builtin
            if(node->children.size() > 0 && node->children.at(0)->type == NT_Symbol && node->children.at(0)->value != "defun") {
                if(isBuiltIn(node->children.at(0)->value)) {
                    // to be implemented
                }else {
                    // user defined function call
                    return handle_functionCall(node);
                }



            }

        } else if(node->gtype == GT_Terminal) {

            if(node->type == NT_Number) {
                std::string temp = generate_tmp();
                emit(temp + " = " + node->value);
                return temp;
            }

            if(node->type == NT_Symbol) {
                std::string temp = generate_tmp();
                emit(temp + " = load " + node->value);
                return temp;
            }

        }


        return "0";


    }


};


#endif /*CODEGEN_HPP*/