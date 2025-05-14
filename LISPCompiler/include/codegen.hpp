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

        if(debug) {
            return "dst: '" + dst + "' op: '" + op + "' src1: '" + src1 + "' src2: '" + src2 + "'";
        }
        return  dst + ((op == "=") ? " =" : " " + op)  + " " + src1 + ((!src2.empty()) ? " " + src2 : "");
    }
};

struct Function {
    std::string name;
    size_t localVars;
    size_t parameters;
};

class Generator {
public:
    size_t temp_count = 0;

    std::string ir_result;

    std::unordered_map<std::string, int> variableMap; // store the name as well as the stack offset
    std::unordered_map<std::string, int> functionMap; // store name and labelOffset of function

    std::vector<Function> functions;

    int StackOffset = 4; // to leave space for old bp and return address but for some reason it has to start with 4 instead of 8 idk why
    int labelOffset = 0;

    std::vector<ASTNode*>& program;

    std::vector<IRInstruction> instructions;

    explicit Generator(std::vector<ASTNode*>& p): program(p) {}


    void printError(const std::string& msg);

    void assignOffsets() {
        int offsetCounter = 1; // start from -4, -8, ...

        for (const auto& instr : instructions) {
            auto registerVar = [&](const std::string& name) {
                if (!name.empty() && variableMap.find(name) == variableMap.end()) {
                    variableMap[name] = -4 * offsetCounter++;
                }
            };

            registerVar(instr.dst);
            registerVar(instr.src1);
            registerVar(instr.src2);
        }
    }



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

        // assignOffsets();
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

    std::string handle_operator(ASTNode* node);
    std::string handle_let_keyword(ASTNode* node);
    std::string handle_defun_keyword(ASTNode* node);
    std::string handle_if_keyword(ASTNode* node);
    std::string handle_cons_keyword(ASTNode* node);
    std::string handle_car_keyword(ASTNode* node);
    std::string handle_cdr_keyword(ASTNode* node);
    std::string handle_null_keyword(ASTNode * node);
    std::string handle_toList_keyword(ASTNode * node);
    std::string handle_toString_keyword(ASTNode * node);
    std::string handle_length_keyword(ASTNode * node);
    std::string handle_print_keyword(ASTNode * node);
    std::string handle_read_keyword(ASTNode * node);
    std::string handle_scan_keyword(ASTNode *node);


    std::string handle_functionCall(ASTNode * node);

    std::string handle_cond_keyword(ASTNode * node);

    std::string generate_code(ASTNode* node) {

        if(node->type == NT_None) return "Some kind of error";

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

            // (let x 5)
            if(firstChild->value == "let") {
                return handle_let_keyword(node);
            }

            // (defun add (x y) (+ x y) )
            if(firstChild->value == "defun") {
                return handle_defun_keyword(node);
            }

            // (if (> x y) x y))
            if(firstChild->value == "if") {
                return handle_if_keyword(node);
            }

            // constructs a new pair
            // (cons 1 '(2 3)) -> (1 2 3)
            // (cons 1 (cons 2 3)) -> (1 2 3)
            if(firstChild->value == "cons") {
                return handle_cons_keyword(node);
            }

            // (car '(1 2 3)) -> 1
            if(firstChild->value == "car") {
                return handle_car_keyword(node);
            }

            // (cdr '(1 2 3)) -> (2 3)
            if(firstChild->value == "cdr") {
                return handle_cdr_keyword(node);
            }

            if(firstChild->value == "null?") {
                return handle_null_keyword(node);
            }

            // (toList "test") -> '(116(t) 101(e) 115(s) 116(t))
            // this way i can unify string length and list length as well as the indexing etc.
            // and car and cdr etc. could still be used.
            if(firstChild->value == "toList") {
                return handle_toList_keyword(node);
            }

            // (toString '(116 101 115 116) -> "test"
            if(firstChild->value == "toString") {
                return handle_toString_keyword(node);
            }

            // (lenght '(1 2 3)) -> 3
            if(firstChild->value == "length") {
                return handle_length_keyword(node);
            }

            // (print "hello World") stdout -> "hello World"
            if(firstChild->value == "print") {
                return handle_print_keyword(node);
            }

            // (read "filename")
            if(firstChild->value == "read") {
                return handle_read_keyword(node);
            }

            // (cond
            //      (<condition1> <expr1>)
            //      (<condition2> <expr2>)
            //      (<condition3> <expr3>)
            //      (else <exprN>)
            // )
            if(firstChild->value == "cond") {
                return handle_cond_keyword(node);
            }

            if(isDeclaredFunction(firstChild->value)) {
                return handle_functionCall(node);
            }

            // if(firstChild->type == NT_List) {
            //     return generate_code(firstChild);
            // }

            else {
                printf("Node with unrecognized first child:\n");

                std::cout << printASTNode(*node) << std::endl;
                throw std::runtime_error("\'" + firstChild->value + "\' was not recognized as a built in or userdefined function");
            }


        } else {
            // if its not an expression/list then its an atom
            if(node->type == NT_Number) {
                std::string temp = generate_tmp();
                emit(temp, node->value, "=");

                return temp;
            }

            if(node->type == NT_Symbol) {
                if(isDeclaredVariable(node->value)) {
                    std::string temp = generate_tmp();

                    emit(temp, node->value, "load");
                    return temp;
                }

                throw std::runtime_error("Variable " + node->value + " not delcared in this scope");
            }

            if(node->type == NT_String) {
                std::string temp = generate_tmp();
                emit(temp, node->value, "loadstr");
                return temp;
            }

            if(node->type == NT_Quote) {
                std::string temp = generate_tmp();

                std::string data = serialize_quoted_list(node->children.at(0));

                emit(temp, data, "quote");
                return temp;
            }


        }

        return "ERROR theres probably missing a return somewhere";

    }

};

#endif /*CODEGEN_HPP*/
