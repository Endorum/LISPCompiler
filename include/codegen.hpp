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

    bool isBinary(std::string op) {
        return (
            op == "+" || op == "-" || op == "*" || op == "/" || /* Binary Arithemtiop */
            op == "<" || op == ">" || op == "==" /* Comparisons */
        );
    }

    bool isBuiltIn(std::string str) {
        // if(str == "if") return true;
        // if(str == "else") return true; -> handled in handle_if_keyword
        return false;
    }

    bool functionDeclared(std::string UD_funcName) {
        for(auto funcName : functionNames) {
            if(UD_funcName == funcName) return true;
        }
        return false;
    }



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


    std::string handle_operator(ASTNode* node) {
        std::string op = node->children.at(0)->value;

        // binary op
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

        return "ERROR: gt_operator wasn't an operator?";
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

        for(size_t i=parameters->children.size(); i > 0; i--) {
            emit(parameters->children.at(i - 1)->value + " = pop");
        }

        // Generate code for the body
        std::string result = generate_code(body);

        // Emit a return instruction
        emit("return " + result);

        return result;
    }

    std::string handle_if_keyword(ASTNode* node) {
        if(node->children.size() < 4) {
            throw std::runtime_error("ERROR: Expected: (if (<cond>) (<true body>) (<false body>) )");
        }
        ASTNode* cond = node->children.at(1);
        ASTNode* trueBody = node->children.at(2);
        ASTNode* falseBody = node->children.at(3);

        std::string temp = generate_code(cond); // temp = (cond) ? 1 : 0

        std::string addr = generate_tmp();

        std::string trueLabel = "trueLabel" + addr;
        std::string falseLabel = "falseLabel" + addr;
        std::string endLabel = "endLabel" + addr;
        std::string returnValue = "retValue" + addr;

        emit("if " + temp + " jump " + trueLabel);
        emit("jump " + falseLabel);

        emit(trueLabel + ":");
        std::string trueOut = generate_code(trueBody);
        emit(returnValue + " = " + trueOut);
        emit("jump " + endLabel);

        emit(falseLabel + ":");
        std::string falseOut = generate_code(falseBody);
        emit(returnValue + " = " + falseOut);
        emit("jump " + endLabel);

        emit(endLabel + ":");

        return returnValue;
    }

    std::string handle_str_keyword(ASTNode* node) {
        // ('str' string index)

        if(node->children.size() < 3) {
            throw std::runtime_error("ERROR: Expected: (str <string/variable> <index expr> )");
        }

        std::string strAddr = generate_code(node->children.at(1)); // address of string
        std::string index = generate_code(node->children.at(2));    // index
        std::string tempAddr = generate_tmp();
        emit(tempAddr + " = " + strAddr + " + " + index);
        std::string tempChar = generate_tmp();
        emit(tempChar + " = loadbyte " + tempAddr);
        return tempChar;


    }

    std::string handle_length_keyword(ASTNode* node) {
        // (length string)
        if(node->children.size() < 2) {
            throw std::runtime_error("ERROR: Expected: (length <string/stringvariable>)");
        }
        std::string strAddr = generate_code(node->children.at(1)); // address of string
        std::string res = generate_tmp();
        emit(res + " = len " + strAddr);

        return res;
    }

    std::string handle_print_keyword(ASTNode* node) {
        // (print string/stringvar/var)
        if(node->children.size() < 2) {
            throw std::runtime_error("ERROR: Expected: (print <string/stringvariable>)");
        }
        std::string strAddr = generate_code(node->children.at(1)); // address of string
        emit("print " + strAddr);

        return "";
    }

    std::string handle_car_keyword(ASTNode* node) {
        // (car <list>)
        if(node->children.size() < 2) {
            throw std::runtime_error("ERROR: Expected: (car <list>)");
        }
        std::string strAddr = generate_code(node->children.at(1)); // address of the list
        std::string dst = generate_tmp();
        emit(dst + " = " + strAddr + "[0]");

        return dst;
    }

    std::string handle_list_keyword(ASTNode* node) {
        std::vector<std::string> elements;

        if(node->children.size() < 2) {
            throw std::runtime_error("ERROR: Expected: (list <element> ...)");
        }

        // generate code for each element
        for (int i = 1; i < node->children.size(); i++) {
            elements.push_back(generate_code(node->children[i]));
        }

        // allocate memory
        std::string listTemp = generate_tmp();
        emit(listTemp + " = alloc " + std::to_string(elements.size()));

        // store elements
        for (int i = 0; i < elements.size(); i++) {
            emit("store [" + listTemp + " + " + std::to_string(i * 4) + "] = " + elements[i]); // 32 bit integer assumed!!!
        }

        return listTemp;
    }

    // indexes a list (at <a list> <expr>)
    std::string handle_at_keyword(ASTNode* node) {
        if(node->children.size() < 3) {
            throw std::runtime_error("ERROR: Expected: (at <list> <index>)");
        }

        std::string list = generate_code(node->children.at(1));
        std::string index = generate_code(node->children.at(2));
        std::string dst = generate_tmp();

        emit(dst + " = " + list + "[" + index + "]");
        return dst;
    }


    std::string handle_cdr_keyword(ASTNode* node) {
        // (cdr <list>)
        if(node->children.size() < 2) {
            throw std::runtime_error("ERROR: Expected: (cdr <list>)");
        }

        std::string listAddr = generate_code(node->children.at(1)); // address of the list
        std::string dst = generate_tmp();

        // Return the pointer to the rest of the list, starting from element 2 (offset 4 bytes)
        emit(dst + " = " + listAddr + " + 4");

        return dst;
    }


    std::string generate_code(ASTNode* node) {
        if(node->gtype == GT_Expression) {

            // first child = opeator
            if(!node->children.empty() && node->children.at(0)->gtype == GT_Operator) {

                // operator
                return handle_operator(node);

            }

            // handling "let" keyword: (let varname initvalue)
            if(!node->children.empty() && node->children.at(0)->value == "let") {
                return handle_let_keyword(node);
            }

            // handling "defun" keyword: (defun name (arg0 arg1 ...) ( body ) )
            if (!node->children.empty() && node->children.at(0)->value == "defun") {
                return handle_defun_keyword(node);
            }

            // handling "if" keyword (if (<cond>) (<true>) (<false>) )
            if (!node->children.empty() && node->children.at(0)->value == "if") {
                return handle_if_keyword(node);
            }

            // handling "str" keyword (str string index)
            if (!node->children.empty() && node->children.at(0)->value == "str") {
                return handle_str_keyword(node);
            }

            // handling "length" keyword (length string)
            if (!node->children.empty() && node->children.at(0)->value == "length") {
                return handle_length_keyword(node);
            }

            // handling "print" keyword (print string)
            if (!node->children.empty() && node->children.at(0)->value == "print") {
                return handle_print_keyword(node);
            }

            // handling "car" keyword (car (1 2 3)) -> 1
            if (!node->children.empty() && node->children.at(0)->value == "car") {
                return handle_car_keyword(node);
            }

            // handling "cdr" keyword (car (1 2 3)) -> (2 3)
            if (!node->children.empty() && node->children.at(0)->value == "cdr") {
                return handle_cdr_keyword(node);
            }

            // handling "list" keyword (list 1 2 3) -> (1 2 3) as an array in a variable
            if (!node->children.empty() && node->children.at(0)->value == "list") {
                return handle_list_keyword(node);
            }

            // handling "at" keyword (at list index) -> element at index index (0th = first obv)
            if (!node->children.empty() && node->children.at(0)->value == "at") {
                return handle_at_keyword(node);
            }

            // function call or builtin
            if(!node->children.empty() && node->children.at(0)->type == NT_Symbol && node->children.at(0)->value != "defun") {
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

            if(node->type == NT_String) {
                std::string temp = generate_tmp();
                emit(temp + " = loadstr " + node->value);
                return temp;
            }

        }

        printf("%s not recognized\n",node->value.c_str());
        exit(1);

        return "0";


    }


};


#endif /*CODEGEN_HPP*/