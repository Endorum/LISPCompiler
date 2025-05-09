//
// Created by Alexander on 07.05.25.
//

#include <unordered_set>

#include "../include/codegen.hpp"

void Generator::printError(const std::string &msg) {
    printf("not yet implemented\n");
    exit(1);
}

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

    size_t parameterCount = node->children.size() - 1; // -1 because the first child is the function name

    for(int i=0;i<functions.size();i++) {
        if(functions.at(i).name == UDfuncName) {
            if(parameterCount != functions.at(i).parameters) {
                throw std::runtime_error("ERROR: Invalid parameter count for function: " + UDfuncName +", wanted: " + std::to_string(functions.at(i).parameters) + " provided: " + std::to_string(parameterCount));
            }
        }
    }

    emit("function call", UDfuncName, "call");

    // after call, result is "somewhere" — but you might want to make it explicit
    std::string temp = generate_tmp();
    emit(temp, "return_value", "=");

    return temp;
}

std::string Generator::handle_cond_keyword(ASTNode *node) {
    if (node->children.size() < 2) {
        throw std::runtime_error("ERROR: cond expects at least one clause");
    }

    std::string addr = generate_tmp(); // base for labels
    std::string returnValue = "retValue" + addr;
    variableMap[returnValue] = StackOffset += 4;

    std::string endLabel = "cond_" + addr + "_end";

    bool hasElseClause = false;

    for (int i = 1; i < node->children.size(); ++i) {
        ASTNode* clause = node->children.at(i);

        if (clause->children.size() != 2) {
            throw std::runtime_error("ERROR: Each cond clause must have exactly 2 elements");
        }

        ASTNode* condExpr = clause->children.at(0);
        ASTNode* bodyExpr = clause->children.at(1);

        // Check for (else ...)
        if (condExpr->type == NodeType::NT_Symbol && condExpr->value == "else") {
            hasElseClause = true;

            std::string elseLabel = "cond_" + addr + "_else";
            emit(elseLabel, "", "label");

            std::string elseOut = generate_code(bodyExpr);
            emit(returnValue, elseOut, "=");
            emit(endLabel, "", "jump");

        } else {
            // Regular (test body) clause
            std::string condTemp = generate_code(condExpr);
            std::string clauseLabel = "cond_" + addr + "_clause_" + std::to_string(i);
            std::string nextClauseLabel = "cond_" + addr + "_next_" + std::to_string(i);

            emit(clauseLabel, condTemp, "if", "jump"); // if condTemp != 0 jump to clauseLabel
            emit(nextClauseLabel, "", "jump");         // otherwise skip to next clause

            emit(clauseLabel, "", "label");
            std::string result = generate_code(bodyExpr);
            emit(returnValue, result, "=");
            emit(endLabel, "", "jump");

            emit(nextClauseLabel, "", "label");
        }
    }

    emit(endLabel, "", "label");

    if (!hasElseClause) {
        // default return nil/0 if no condition matched
        emit(returnValue, "0", "=");
    }

    return returnValue;
}


std::string Generator::handle_read_keyword(ASTNode *node) {
    if (node->children.size() != 2) {
        throw std::runtime_error("ERROR: Expected only a filename");
    }

    std::string filename = generate_code(node->children.at(1));
    std::string temp = generate_tmp();

    emit(temp, filename, "read_file");

    return temp;
}

std::string Generator::handle_scan_keyword(ASTNode *node) {
    if (node->children.size() != 2) {
        throw std::runtime_error("ERROR: Expected only a variable");
    }

    std::string temp = generate_tmp();

    emit(temp, "stdin", "scan_user_input");

    return temp;
}

std::string opCharToWord(std::string op) {
    switch (op.at(0)) {
        default: return op;
        case '+': return "add";
        case '-': return "sub";
        case '*': return "imul";
        case '/': return "idiv";

        case '&': return "and";
        case '|': return "or";
        case '^': return "xor";
        case '~': return "neg";

        case '<': return "l";
        case '>': return "g";
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

    if(node->children.at(1)->type != NT_Symbol) {
        throw std::runtime_error("ERROR: Expected a Symbol for the variable name");
    }

    std::string varname = node->children.at(1)->value;

    std::string temp = generate_code(node->children.at(2));

    variableMap[varname] = StackOffset += 4;

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


    const std::string listAddr = generate_code(node->children.at(1));
    std::string temp = generate_tmp();

    emit(temp, listAddr, "list_to_string");

    return temp;
}

std::string Generator::handle_defun_keyword(ASTNode *node) {
    std::string functionName = node->children.at(1)->value;
    ASTNode* parameters = node->children.at(2);

    if (!parameters) {
        throw std::runtime_error("ERROR: Expected parameters");
    }


    Function func;
    func.name = functionName;
    func.parameters = parameters->children.size();



    // Emit the function label
    emit("label", functionName, "defun");



    for(size_t i=parameters->children.size(); i > 0; i--) {
        variableMap[parameters->children.at(i - 1)->value] = StackOffset += 4;
        // emit(parameters->children.at(i - 1)->value, "stack", "pop"); // <- wrong the arguemnts are NOT popped they are near the bp not the sp
    }

    // size_t parameterCount = parameters->children.size();

    size_t stackOffBefore = StackOffset;
    size_t tempCountBefore = temp_count;


    // Generate code for the body
    // make it work multiple lines and only return the result of the last
    std::string result;


    // switch if multi line body for functions should be possible
    // right now it makes problems but that might be another issue
    // it seems to be an issue with let or maybe the whole custom variables shit
    if(true) {
        for(int i=3;i<node->children.size();i++) {
            ASTNode *body = node->children.at(i);
            result = generate_code(body);
        }
    } else {
        ASTNode *body = node->children.at(3);
        result = generate_code(body);
    }

    size_t tempCountAfter = temp_count;
    size_t stackAfterBody = StackOffset;

    size_t variables = (stackAfterBody - stackOffBefore) / 4 + (tempCountAfter - tempCountBefore);

    temp_count = 0;

    // Emit a return instruction
    func.localVars = variables;
    functions.push_back(func);

    emit("return_value", result, "return");
    variableMap["return_value"] = StackOffset += 4;

    return result;
}

std::string Generator::handle_if_keyword(ASTNode *node) {
    if(node->children.size() != 4) {
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

    variableMap[returnValue] = StackOffset += 4;

    emit(trueLabel, temp, "if", "jump");
    emit(falseLabel, "", "jump");

    emit(trueLabel, "", "label");
    std::string trueOut = generate_code(trueBody);
    emit(returnValue, trueOut, "=");
    emit(endLabel, "", "jump");

    emit(falseLabel, "", "label");
    std::string falseOut = generate_code(falseBody);
    emit(returnValue, falseOut, "=");

    emit(endLabel, "", "label");

    return returnValue;
}



std::string Generator::handle_length_keyword(ASTNode *node) {
    // (length list)
    if(node->children.size() < 2) {
        throw std::runtime_error("ERROR: Expected: (length <list>)");
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
