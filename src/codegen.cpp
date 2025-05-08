//
// Created by Alexander on 07.05.25.
//

#include <unordered_set>

#include "../include/codegen.hpp"

bool Generator::isDeclaredFunction(std::string UD_funcName) {
    for(const auto& pair : functionMap) {
        if(UD_funcName == pair.first) return true;
    }
    return false;
}

bool Generator::isDeclaredVariable(std::string name) {
    for(const auto& pair : variableMap) {
        if(pair.first == name) return true;
    }
    return false;
}

void Generator::findFuncDecls() {
    std::unordered_set<std::string> seenFunctions;

    for (auto node : program) {
        if (node->children.size() > 0 && node->children.at(0)->value == "defun") {
            std::string funcName = node->children.at(1)->value;

            if (seenFunctions.count(funcName)) {
                throw std::runtime_error("ERROR: Function '" + funcName + "' is declared multiple times");
            }

            seenFunctions.insert(funcName);
            // functionNames.push_back(funcName);
            functionMap.insert({funcName, 69});
        }
    }
}

std::string Generator::handle_functionCall(ASTNode *node) {
    std::string UDfuncName = node->children.at(0)->value;

    if (!isDeclaredFunction(UDfuncName)) {
        throw std::runtime_error("ERROR: Function " + UDfuncName + " is not declared");
    }

    // child 1 and onwards are the parameters
    for (size_t i = 1; i < node->children.size(); ++i) {
        ASTNode* param = node->children.at(i);
        std::string res = generate_code(param);
        emit("stack", res, "push");
    }

    emit("call " + UDfuncName);

    // after call, result is "somewhere" — but you might want to make it explicit
    std::string temp = generate_tmp();
    emit(temp, "return_value", "=");
    return temp;
}

std::string opCharToWord(std::string op) {
    switch (op.at(0)) {
        default: return op;
        case '+': return "add";
        case '-': return "sub";
        case '*': return "mul";
        case '/': return "div";

        case '&': return "and";
        case '|': return "or";
        case '^': return "xor";
        case '~': return "neg";

        case '<': return "le";
        case '>': return "gr";
        case '=': return "eq";
        case '!': return "not";

    }
}

std::string Generator::handle_operator(ASTNode *node) {
    std::string op = node->children.at(0)->value;

    // binary op
    if( !(op == "~" || op == "!") ) {

        if (node->children.size() < 3) {
            throw std::runtime_error("ERROR: Expected at least two arguments for binary operator");
        }

        // Initialize temp with the first argument
        std::string temp = generate_code(node->children.at(1));

        // Loop over the remaining arguments
        for (int i = 2; i < node->children.size(); i++) {
            std::string arg = generate_code(node->children.at(i));
            std::string res = generate_tmp();
            // emit(res + " = " + temp + " " + opCharToWord(op) + " " + arg);
            emit(res, temp, opCharToWord(op), arg);
            temp = res; // update temp to chain the next operation
        }

        return temp;
    } else {
        if (node->children.size() != 2) {
            throw std::runtime_error("ERROR: Expected one argument for unary operator");
        }

        std::string temp = generate_code(node->children.at(1));
        std::string res = generate_tmp();
        emit(res, temp, opCharToWord(op));

        return res;
    }

    return "ERROR: handle_operator wasn't given an operator?";
}

// (let x 5) TODO: maybe implement scoped variables
std::string Generator::handle_let_keyword(ASTNode *node) {
    if (node->children.size() < 3) {
        throw std::runtime_error("ERROR: Invalid let expression");
    }

    std::string varname = node->children.at(1)->value;

    std::string temp = generate_code(node->children.at(2));

    variableMap[varname] = StackOffset++;

    emit(varname, temp, "store");

    return temp;
}

std::string Generator::handle_cons_keyword(ASTNode *node) {
    if (node->children.size() < 3) {
        throw std::runtime_error("ERROR: Invalid cons expression: (cons <first> <second>)");
    }

    std::string left = generate_code(node->children.at(1));
    std::string right = generate_code(node->children.at(2));
    std::string temp = generate_tmp();
    emit(temp, left, "cons", right);
    return temp;
}

std::string Generator::handle_null_keyword(ASTNode *node) {
    if (node->children.size() != 2) {
        throw std::runtime_error("ERROR: Invalid null? expression: (null? <list>)");
    }

    std::string listAddr = generate_code(node->children.at(1));
    std::string temp = generate_tmp();

    emit(temp, listAddr, "null?");

    return temp;
}

std::string Generator::handle_toList_keyword(ASTNode *node) {
    if (node->children.size() != 2) {
        throw std::runtime_error("ERROR: Invalid toList expression: (toList <string>)");
    }

    if(node->children.at(1)->type != NT_String) {
        throw std::runtime_error("ERROR: Expected a string");
    }

    std::string strAddr = generate_code(node->children.at(1));
    std::string temp = generate_tmp();

    /*

    List* string_to_list(const char* str) {
        if (!str) return NULL;
        List* result = NULL;
        for (int i = strlen(str) - 1; i >= 0; i--) {
            result = cons(str[i], result);
        }
        return result;
    }

     */

    emit(temp, strAddr, "string_to_list");


    return temp;
}

std::string Generator::handle_toString_keyword(ASTNode *node) {
    if (node->children.size() != 2) {
        throw std::runtime_error("ERROR: Invalid toString expression: (toString <list>)");
    }

    if(node->children.at(1)->type != NT_List) {
        throw std::runtime_error("ERROR: Expected a List");
    }

    const std::string listAddr = generate_code(node->children.at(1));
    std::string temp = generate_tmp();

    emit(temp, listAddr, "list_to_string");

    return temp;
}

std::string Generator::handle_defun_keyword(ASTNode *node) {
    std::string functionName = node->children.at(1)->value;
    ASTNode* parameters = node->children.at(2);
    ASTNode* body = node->children.at(3);

    if (!parameters || !body) {
        throw std::runtime_error("ERROR: Expected parameters and body");
    }

    // Emit the function label
    emit(functionName + ":");

    for(size_t i=parameters->children.size(); i > 0; i--) {
        variableMap[parameters->children.at(i - 1)->value] = StackOffset++;
        emit(parameters->children.at(i - 1)->value, "pop");
    }

    // Generate code for the body
    std::string result = generate_code(body);

    // Emit a return instruction
    emit("return_value", result, "=");

    return result;
}

std::string Generator::handle_if_keyword(ASTNode *node) {
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

    emit(trueLabel, temp, "if", "jump");
    emit(falseLabel, "", "jump");

    emit(trueLabel + ":");
    std::string trueOut = generate_code(trueBody);
    emit(returnValue, trueOut, "=");
    emit(endLabel, "jump");

    emit(falseLabel + ":");
    std::string falseOut = generate_code(falseBody);
    emit(returnValue, falseOut, "=");

    emit(endLabel + ":");

    return returnValue;
}



std::string Generator::handle_length_keyword(ASTNode *node) {
    // (length list)
    if(node->children.size() < 2) {
        throw std::runtime_error("ERROR: Expected: (length <list>)");
    }

    if(node->children.at(1)->type != NT_List) {
        throw std::runtime_error("ERROR: Expected a List");
    }

    std::string listAddr = generate_code(node->children.at(1)); // address of list
    std::string temp = generate_tmp();
    emit(temp, listAddr, "listLength");

    return temp;
}

std::string Generator::handle_print_keyword(ASTNode *node) {
    // (print string/stringvar/var)
    if(node->children.size() < 2) {
        throw std::runtime_error("ERROR: Expected: (print <string/stringvariable>)");
    }

    if(node->children.at(1)->type != NT_String) {
        throw std::runtime_error("ERROR: Expected a string");
    }

    std::string strAddr = generate_code(node->children.at(1)); // address of string

    emit("stdout", strAddr, "print");

    return strAddr;
}

std::string Generator::handle_car_keyword(ASTNode* node) {
    if (node->children.size() != 2) {
        throw std::runtime_error("ERROR: Expected usage: (car <list>)");
    }

    std::string listAddr = generate_code(node->children.at(1));
    std::string dst = generate_tmp();

    // Emit a high-level IR op like: "t2 = car t1"
    emit(dst, listAddr, "car");

    return dst;
}

std::string Generator::handle_cdr_keyword(ASTNode *node) {
    // (cdr <list>)
    if(node->children.size() < 2) {
        throw std::runtime_error("ERROR: Expected: (cdr <list>)");
    }

    std::string listAddr = generate_code(node->children.at(1)); // address of the list
    std::string dst = generate_tmp();

    // Return the pointer to the rest of the list, starting from element 2 (offset 4 bytes)
    emit(dst, listAddr, "cdr");

    return dst;
}
