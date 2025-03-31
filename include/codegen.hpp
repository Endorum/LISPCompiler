//
// Created by Alexander on 31.03.25.
//

#ifndef CODEGEN_HPP
#define CODEGEN_HPP
#include <vector>

#include "parser.hpp"

class Function {
public:
    std::string name;
    ASTNode* paramList;
    ASTNode* body;
    // int loc (?)
};

class Generator {
private:
    std::vector<ASTNode*> nodes;

    std::vector<Function> functions;

public:
    Generator(std::vector<ASTNode*>& nodes) : nodes(nodes) {}


    ASTNode* getChild(ASTNode* parent, size_t index) {
        if(index < parent->children.size()) return parent->children[index];
        return nullptr;
    }

    std::string res = "";

    bool isBuiltIn(ASTNode* node) {
        std::string name = node->value;

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
        res += "POP AX\n";
        res += "POP BX\n";

        if(op.length() == 1) {
            char c = op[0];

            switch (c) {
                case '+': res += "ADD AX, BX\n"; break;
                case '-': res += "SUB AX, BX\n"; break;
                case '*': res += "MUL AX, BX\n"; break;
                case '/': res += "DIV AX, BX\n"; break;
            }

        }else {
            // car cons etc
        }

        res += "PUSH AX\n";
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
            // unary or more op
        }


    }

    void evalFuncDecl(ASTNode* expr, Function func) {

    }

    Function* getFuncByName(std::string name) {
        for(int i=0;i<functions.size();i++) {
            if(name == functions[i].name) return &functions[i];
        }

        return nullptr;
    }

    void evalFuncCall(ASTNode* expr) {
        printf("evaluating function call: %s\n",getFuncByName(getChild(expr, 0)->value)->name.c_str());
        if (expr->children.empty()) return;

        Function* func = getFuncByName(getChild(expr, 0)->value);

        if(func == nullptr) {
            printf("ERROR: function %s not found\n",getChild(expr, 0)->value.c_str());
            exit(1);
        }

        if(expr->children.size() - 1 != func->paramList->children.size()) {
            printf("Error: Incorrect argument count for %s\n", func->name.c_str());
            exit(1);
        }

        // evaluating the arguments
        for (size_t i = 1; i < expr->children.size(); i++) {
            evalTerminal(expr->children[i]); // the values get pushed to the stack
        }


        evalExpr(func->body);



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
                Function newFunc;
                // eval func decl
                ASTNode* funcNameNode = getChild(expr, 1);
                if(!funcNameNode) {
                    printf("ERROR: expected function name\n");
                    exit(1);
                }

                newFunc.name = funcNameNode->value;


                ASTNode* funcArgsNode = getChild(expr, 2);
                if(!funcArgsNode) {
                    printf("ERROR: expected argument list\n");
                    exit(1);
                }

                newFunc.paramList = funcArgsNode;

                ASTNode* funcBodyNode = getChild(expr, 3);
                if(!funcBodyNode) {
                    printf("ERROR: expected function body\n");
                    exit(1);
                }

                newFunc.body = funcBodyNode;

                evalFuncDecl(expr, newFunc);

                functions.push_back(newFunc);
            }

            else if(isBuiltIn(firstChild)) {
                // build in operator
                evalBuiltIn(expr);
            }

            else if(isFuncCall(firstChild)) {
                // function call
                evalFuncCall(expr);
            }

            else {
                // symbol is part of data list
            }

        }

    }

    void evalTerminal(ASTNode* terminal) {
        if(terminal->type == NT_Number) {
            printf("eval term number: %s\n",terminal->value.c_str());
            res += "PUSH " + terminal->value + "\n";
            return;
        } else if(terminal->type == NT_String) {
            printf("eval term String: %s\n",terminal->value.c_str());
        } else if(terminal->type == NT_Symbol) {
            printf("eval term symbol %s\n",terminal->value.c_str());
            res += "PUSH " + terminal->value + "\n";
        } else {

            evalExpr(terminal);
        }
    }

    void evalList(ASTNode* list) {

    }



};

#endif //CODEGEN_HPP
