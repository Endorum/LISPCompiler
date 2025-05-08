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

std::string Generator::handle_str_keyword(ASTNode *node) {
    // ('str' string index)

    if(node->children.size() < 3) {
        throw std::runtime_error("ERROR: Expected: (str <string/variable> <index expr> )");
    }

    std::string strAddr = generate_code(node->children.at(1)); // address of string
    std::string index = generate_code(node->children.at(2));    // index
    std::string tempAddr = generate_tmp();
    // emit(tempAddr + " = " + strAddr + " + " + index);
    std::string tempChar = generate_tmp();
    // emit(tempChar + " = loadbyte " + tempAddr);
    return tempChar;


}

std::string Generator::handle_length_keyword(ASTNode *node) {
    // (length string)
    if(node->children.size() < 2) {
        throw std::runtime_error("ERROR: Expected: (length <string/stringvariable>)");
    }
    std::string strAddr = generate_code(node->children.at(1)); // address of string
    std::string res = generate_tmp();
    emit(res + " = len " + strAddr);

    return res;
}

std::string Generator::handle_print_keyword(ASTNode *node) {
    // (print string/stringvar/var)
    if(node->children.size() < 2) {
        throw std::runtime_error("ERROR: Expected: (print <string/stringvariable>)");
    }
    std::string strAddr = generate_code(node->children.at(1)); // address of string

    emit("stdout", strAddr, "printInt");

    return "";
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


std::string Generator::handle_list_keyword(ASTNode *node) {
    std::vector<std::string> elements;

    if(node->children.size() < 2) {
        throw std::runtime_error("ERROR: Expected: (list <element> ...)");
    }

    // generate code for each element
    for (int i = 1; i < node->children.size(); i++) {
        elements.push_back(generate_code(node->children.at(i)));
    }

    // allocate memory
    std::string listTemp = generate_tmp();
    emit(listTemp + " = alloc " + std::to_string(elements.size()));

    // store elements
    for (int i = 0; i < elements.size(); i++) {
        emit("store [" + listTemp + " + " + std::to_string(i * 4) + "] = " + elements.at(i)); // 32 bit integer assumed!!!
    }

    return listTemp;
}

std::string Generator::handle_at_keyword(ASTNode *node) {
    if(node->children.size() < 3) {
        throw std::runtime_error("ERROR: Expected: (at <list> <index>)");
    }

    std::string list = generate_code(node->children.at(1));
    std::string index = generate_code(node->children.at(2));
    std::string dst = generate_tmp();

    emit(dst + " = " + list + "[" + index + "]");
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
    emit(dst + " = " + listAddr + " + 4");

    return dst;
}
