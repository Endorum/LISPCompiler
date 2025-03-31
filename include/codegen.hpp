//
// Created by Alexander on 31.03.25.
//

#ifndef CODEGEN_HPP
#define CODEGEN_HPP
#include <vector>

#include "parser.hpp"
class Function;

class Variable {
public:
    std::string name;
    Function* scope = nullptr; // nullptr -> global
    uint64_t loc; // just an offset its later calculated * 8 = space between
    bool parameter = false;
};

class Function {
public:
    std::string name;
    std::vector<Variable> parameters;
    ASTNode* body;

    Function* scope=nullptr;
    // int loc (?)
};



class Generator {
private:
    std::vector<ASTNode*> nodes;

    std::vector<Function> functions;
    Function* currentScope=nullptr;

    std::vector<Variable> globalVars;

    uint64_t locOff=0;

public:
    Generator(std::vector<ASTNode*>& nodes) : nodes(nodes) {}


    ASTNode* getChild(ASTNode* parent, size_t index) {
        if(index < parent->children.size()) return parent->children[index];
        return nullptr;
    }

    std::string res = "";

    bool isBuiltIn(std::string name) {

        return (
            name == "+" ||
            name == "-" ||
            name == "*" ||
            name == "/" || 0 // etc like car cons ...

        );

    }

    bool isFuncCall(ASTNode* node) {
        std::string name = node->value;

        for(int i=0;i<functions.size();i++) {
            if(name == functions[i].name) return true;
        }

        return false;
    }

    void evalBinaryOp(std::string op) {
        res += "pop bx ; first argument\n";
        res += "pop ax ; second argument\n";

        if(op.length() == 1) {
            char c = op[0];

            switch (c) {
                default: res += "; Unknown op: " + op; res += "\n"; break;
                case '+': res += "add ax, bx\n"; break;
                case '-': res += "sub ax, bx\n"; break;
                case '*': res += "mul ax, bx\n"; break;
                case '/': res += "div ax, bx\n"; break;
            }

        } else {
            // car cons etc
        }

        res += "push ax ; result\n";
    }

    void evalBuiltIn(ASTNode* expr) {
        std::string op = getChild(expr, 0)->value;

        printf("evaluating built in function: %s\n",op.c_str());

        for(int i=1;i<expr->children.size();i++) {
            ASTNode* arg = expr->children[i];
            evalTerminal(arg);
        }

        if(expr->children.size() == 3) {
            evalBinaryOp(op);
        }else {
            // unary or more - op
        }


    }

    void evalFuncDecl(ASTNode* expr) {
        printf("evaluating function decl: %s\n",getChild(expr, 1)->value.c_str());

        ASTNode* funcNameNode = getChild(expr, 1);
        if(!funcNameNode) {
            printf("ERROR: \n");
            printf("    Expected function name\n");
            exit(1);
        }

        ASTNode* funcParamsNode = getChild(expr, 2);
        if(!funcParamsNode) {
            printf("ERROR: \n");
            printf("    Expected parameter list or empty list\n");
            exit(1);
        }

        ASTNode* funcBodyNode = getChild(expr, 3);
        if(!funcBodyNode) {
            printf("ERROR: \n");
            printf("    Expected body\n");
            exit(1);
        }

        Function func;
        func.name = funcNameNode->value;
        func.body = funcBodyNode;

        for(const auto & i : funcParamsNode->children) {
            Variable param;
            param.name = i->value;
            param.loc = locOff++;
            param.scope = &func;
            param.parameter = true;

            func.parameters.push_back(param);
        }

        functions.push_back(func);

        currentScope = &functions[functions.size() - 1];

        res += "_" + funcNameNode->value + ":\n";

        evalTerminal(func.body);

    }

    Function* getFuncByName(std::string name) {
        for(int i=0;i<functions.size();i++) {
            if(name == functions[i].name) return &functions[i];
        }

        return nullptr;
    }

    void evalFuncCall(ASTNode* expr) {
        printf("evaluating function call: %s\n",getChild(expr, 0)->value.c_str());

        if (expr->children.empty()) return;

        // first one is the function name
        ASTNode* funcNameNode = getChild(expr, 0);
        std::string funcName = (funcNameNode) ? funcNameNode->value : "NULL";

        // the rest are arguments
        for(int i=1;i<expr->children.size();i++) {
            ASTNode* arg = getChild(expr, i);
            evalTerminal(arg);
        }

        if(isBuiltIn(funcName)) {
            if(expr->children.size() == 3) { // binary
                evalBinaryOp(funcName);
            }
        } else {
            res += "call " + funcName + "\n";
        }

        for(int i=0;i<functions.size();i++) {
            if(functions[i].name == funcName) {
                currentScope = &functions[i];
            }
        }


    }

    void evalExpr(ASTNode* expr){
        // if first == number / string  -> evaluate every item in it
        // if first == symbol (including +-*/ etc) -> evaluate second third etc child
        // if first == keyword=defun -> eval func decl

        if (!expr) return;

        ASTNode* firstChild = getChild(expr, 0);

        if (firstChild->type == NT_Number || firstChild->type == NT_String) {
            // It's a list: evaluate every child as a terminal
            for (ASTNode* child : expr->children) {
                evalTerminal(child);
            }
        }

        else if(firstChild->type == NT_Symbol) {
            if(firstChild->value == "defun") {
                // function declaration
                evalFuncDecl(expr);
            } else { // otherwise it should be a function call, therefore evaluate second third etc child first then call
                evalFuncCall(expr);
            }
        }



        // else if(firstChild->type == NT_Symbol) {
        //     if(firstChild->value == "defun") {
        //         // function declaration
        //         evalFuncDecl(expr);
        //     }
        //
        //     else if(isBuiltIn(firstChild)) {
        //         // build in operator
        //         evalBuiltIn(expr);
        //     }
        //
        //     else if(isFuncCall(firstChild)) {
        //         // function call
        //         evalFuncCall(expr);
        //     }
        //
        //     else {
        //         // symbol is part of data list
        //
        //     }
        //
        // }

    }

    void evalVariable(ASTNode* terminal) {
        //printf("eval variable: %s\n",terminal->value.c_str());

        if(currentScope != nullptr) {
            for(int i=0;i<currentScope->parameters.size();i++) {
                if(currentScope->parameters[i].name == terminal->value) {
                    Variable var = currentScope->parameters[i];
                    printf("read variable %s from loc %llu\n", var.name.c_str(), var.loc);
                }
            }
        }else {
            for(int i=0;i<globalVars.size();i++) {
                if(globalVars[i].name == terminal->value) {
                    Variable var = globalVars[i];
                    printf("read global variable %s from loc %llu\n", var.name.c_str(), var.loc);
                }
            }
        }
    }

    void evalTerminal(ASTNode* terminal, bool first=false) {
        if(terminal->type == NT_Number) {
            printf("eval term number: %s\n",terminal->value.c_str());
            res += "push " + terminal->value + "\n";
        } else if(terminal->type == NT_String) {
            printf("eval term String: %s\n",terminal->value.c_str());
        } else if(terminal->type == NT_Symbol) {
            /*
             * decide if the Symbol is a function call or just a variable eg as a parameter
             * if its the first element in a list it should be a call if not its a variable
             * which are handled using look ups.
             */
            if(first) {
                // then its a function call
                evalFuncCall(terminal);
            }else {
                // its a variable
                evalVariable(terminal);
            }


        } else {

            evalExpr(terminal);
        }
    }

    void evalList(ASTNode* list) {

    }



};

#endif //CODEGEN_HPP
