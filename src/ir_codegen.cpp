//
// Created by Alexander on 07.05.25.
//

#include <unordered_set>

#include "../include/ir_codegen.hpp"


void Generator::printError(const std::string &msg) {
    printf("not yet implemented\n");
    exit(1);
}

void Generator::emit(Value dst, std::string op) {
    IRInstruction instruction;
    instruction.dst = dst;
    instruction.op = op;
    instructions.push_back(instruction);
}

void Generator::emit(Value dst, std::string op, Value src1) {
    IRInstruction instruction;
    instruction.dst = dst;
    instruction.op = op;
    instruction.src1 = src1;
    instructions.push_back(instruction);
}

void Generator::emit(Value dst, std::string op, Value src1, Value src2) {
    IRInstruction instruction;
    instruction.dst = dst;
    instruction.op = op;
    instruction.src1 = src1;
    instruction.src2 = src2;
    instructions.push_back(instruction);
}


bool Generator::isDeclaredFunction(std::string name) {
    for(const auto& func : declaredFunctions) {
        if(func == name) return true;
    }
    return false;
}

bool Generator::isDeclaredVariable(std::string name) {
    for(const auto& pair : variable_table) {
        if(pair.first == name) return true;
    }
    return false;
}

void Generator::findFuncDecls() {
    std::unordered_set<std::string> seenFunctions;

    for (auto node : ast) {
        if (node->children.size() > 0 && node->children.at(0)->value == "defun") {
            std::string funcName = node->children.at(1)->value;

            if (seenFunctions.count(funcName)) {
                throw std::runtime_error("ERROR: Function '" + funcName + "' is declared multiple times");
            }

            seenFunctions.insert(funcName);
            declaredFunctions.push_back(funcName);
        }
    }
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

Value Generator::handle_operator(ASTNode *node) {


    std::string op = node->children.at(0)->value;

    // binary op
    if( !(op == "~" || op == "!") ) {

        if (node->children.size() != 3) {
            throw std::runtime_error("ERROR: Expected exactly one leftside, op and one rightside");
        }

        Value dst = generate_tmp();

        Value left = generate_code(node->children.at(1));
        Value right = generate_code(node->children.at(2));

        emit(dst, opCharToWord(op), left, right);


        return dst;

    } else {
        if (node->children.size() != 2) {
            throw std::runtime_error("ERROR: Expected one argument for unary operator");
        }

        Value dst = generate_tmp();
        Value operand = generate_code(node->children.at(1));

        emit(dst, opCharToWord(op), operand);

        return dst;
    }

    throw std::runtime_error("ERROR: not an operator: " + op);
}

// (let x 5) TODO: maybe implement scoped variables
Value Generator::handle_let_keyword(ASTNode *node) {
    if (node->children.size() < 3) {
        throw std::runtime_error("ERROR: Invalid let expression");
    }

    if (node->children.at(1)->type != NT_Symbol) {
        throw std::runtime_error("ERROR: Expected a Symbol for the variable name");
    }

    std::string varname = node->children.at(1)->value;

    // Evaluate the initializer expression
    Value initVal = generate_code(node->children.at(2));

    // Allocate a new local variable slot
    Value localVar;
    localVar.value = varname;
    localVar.type = LOCAL;
    localVar.offSet = local_count++;  // Track how many locals exist
    localVar.loc = getLocalLocation(localVar.offSet);


    // Store the initializer value into the local variable
    emit(localVar, "assign", initVal);

    // Register in symbol table
    variable_table[varname] = localVar;

    return localVar;
}


Value Generator::handle_defun_keyword(ASTNode *node) {
    std::string functionName = node->children.at(1)->value;
    ASTNode* parameters = node->children.at(2);

    variable_table.clear(); // clear from previous scope

    if (!parameters) {
        throw std::runtime_error("ERROR: Expected parameters");
    }

    
    Function func;
    func.name = functionName;
    func.paramCount = parameters->children.size();
    current_function = func;
    

    
    
    // Emit the function label
    emit(Value("label", IMM_STR), "defun", Value(functionName, IMM_STR));
    
    
    // handle parameters
    
    temp_count = 0;
    local_count = 0;

    for(int i=0;i<parameters->children.size();i++){
        const std::string& paramName = parameters->children.at(i)->value;
        Value param = create_parameter(paramName, i);

        variable_table[paramName] = param;
    }

    spill_start = local_count;

    // multi line body
    Value result;

    size_t localVarBefore = local_count;
    for(int i=3;i<node->children.size();i++){
        result = generate_code(node->children.at(i));
        //  += countLocalsInFunction(node->children.at(i));
    }
    
    func.localVarCount = local_count - localVarBefore;
    

    Value dst;
    dst.value = "return_value";
    dst.type = TEMP;
    dst.offSet = 0;
    dst.loc = "eax";

    functions.push_back(func);

    emit(dst, "return", result);


    return {};
}

Value Generator::handle_functionCall(ASTNode *node) {
    std::string funcName = node->children.at(0)->value;

    if (!isDeclaredFunction(funcName)) {
        throw std::runtime_error("ERROR: Function " + funcName + " is not declared");
    }

    std::vector<Value> args;

    // child 1 and onwards are the parameters
    for (size_t i = 1; i < node->children.size(); ++i) {
        args.push_back(generate_code(node->children.at(i)));
    }

    // Validate parameter count
    size_t providedArgs = args.size();
    for (const auto& f : functions) {
        if (f.name == funcName && f.paramCount != providedArgs) {
            throw std::runtime_error("ERROR: Function " + funcName + " expects " +
                std::to_string(f.paramCount) + " arguments, got " + std::to_string(providedArgs));
        }
    }

    for(int i=args.size();  i-- > 0; ){
        emit({}, "push", {args.at(i)});
    }

    emit({}, "call", funcName);

    // esp is restored simply using mov esp, ebp, nvm its necessary
    if(!args.empty()){
        emit({}, "add_esp", {Value(std::to_string(args.size() * 4), IMM_NUM)});
    }

    // Get return value from eax
    Value ret = generate_tmp();
    Value src = Value();
    src.loc = "eax";
    emit(ret, "assign", src);

    return ret;
}


// std::string Generator::handle_cons_keyword(ASTNode *node) {
//     if (node->children.size() < 3) {
//         throw std::runtime_error("ERROR: Invalid cons expression: (cons <first> <second>)");
//     }

//     std::string left = generate_code(node->children.at(1));
//     std::string right = generate_code(node->children.at(2));
//     std::string temp = generate_tmp();
//     emit(temp, left, "cons", right);
//     return temp;
// }

// std::string Generator::handle_null_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ERROR: Invalid null? expression: (null? <list>)");
//     }

//     std::string listAddr = generate_code(node->children.at(1));
//     std::string temp = generate_tmp();

//     emit(temp, listAddr, "null?");

//     return temp;
// }

// std::string Generator::handle_toList_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ERROR: Invalid toList expression: (toList <string>)");
//     }

//     std::string strAddr = generate_code(node->children.at(1));
//     std::string temp = generate_tmp();

//     /*

//     List* string_to_list(const char* str) {
//         if (!str) return NULL;
//         List* result = NULL;
//         for (int i = strlen(str) - 1; i >= 0; i--) {
//             result = cons(str[i], result);
//         }
//         return result;
//     }

//      */

//     emit(temp, strAddr, "string_to_list");


//     return temp;
// }

// std::string Generator::handle_toString_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ERROR: Invalid toString expression: (toString <list>)");
//     }


//     const std::string listAddr = generate_code(node->children.at(1));
//     std::string temp = generate_tmp();

//     emit(temp, listAddr, "list_to_string");

//     return temp;
// }



// std::string Generator::handle_if_keyword(ASTNode *node) {
//     if(node->children.size() != 4) {
//         throw std::runtime_error("ERROR: Expected: (if (<cond>) (<true body>) (<false body>) )");
//     }
//     ASTNode* cond = node->children.at(1);
//     ASTNode* trueBody = node->children.at(2);
//     ASTNode* falseBody = node->children.at(3);

//     std::string temp = generate_code(cond); // temp = (cond) ? 1 : 0

//     std::string addr = generate_tmp();

//     std::string trueLabel = "trueLabel" + addr;
//     std::string falseLabel = "falseLabel" + addr;
//     std::string endLabel = "endLabel" + addr;
//     std::string returnValue = "retValue" + addr;

//     variableMap[returnValue] = StackOffset += 4;

//     emit(trueLabel, temp, "if", "jump");
//     emit(falseLabel, "", "jump");

//     emit(trueLabel, "", "label");
//     std::string trueOut = generate_code(trueBody);
//     emit(returnValue, trueOut, "=");
//     emit(endLabel, "", "jump");

//     emit(falseLabel, "", "label");
//     std::string falseOut = generate_code(falseBody);
//     emit(returnValue, falseOut, "=");

//     emit(endLabel, "", "label");

//     return returnValue;
// }



// std::string Generator::handle_length_keyword(ASTNode *node) {
//     // (length list)
//     if(node->children.size() < 2) {
//         throw std::runtime_error("ERROR: Expected: (length <list>)");
//     }


//     std::string listAddr = generate_code(node->children.at(1)); // address of list
//     std::string temp = generate_tmp();
//     emit(temp, listAddr, "listLength");

//     return temp;
// }

// std::string Generator::handle_print_keyword(ASTNode *node) {
//     // (print string/stringvar/var)
//     if(node->children.size() < 2) {
//         throw std::runtime_error("ERROR: Expected: (print <string/stringvariable>)");
//     }


//     std::string strAddr = generate_code(node->children.at(1)); // address of string

//     emit("stdout", strAddr, "print");

//     return strAddr;
// }

// std::string Generator::handle_car_keyword(ASTNode* node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ERROR: Expected usage: (car <list>)");
//     }

//     std::string listAddr = generate_code(node->children.at(1));
//     std::string dst = generate_tmp();

//     // Emit a high-level IR op like: "t2 = car t1"
//     emit(dst, listAddr, "car");

//     return dst;
// }

// std::string Generator::handle_cdr_keyword(ASTNode *node) {
//     // (cdr <list>)
//     if(node->children.size() < 2) {
//         throw std::runtime_error("ERROR: Expected: (cdr <list>)");
//     }

//     std::string listAddr = generate_code(node->children.at(1)); // address of the list
//     std::string dst = generate_tmp();

//     // Return the pointer to the rest of the list, starting from element 2 (offset 4 bytes)
//     emit(dst, listAddr, "cdr");

//     return dst;
// }

// std::string Generator::handle_cond_keyword(ASTNode *node) {
//     if (node->children.size() < 2) {
//         throw std::runtime_error("ERROR: cond expects at least one clause");
//     }

//     std::string addr = generate_tmp(); // base for labels
//     std::string returnValue = "retValue" + addr;
//     variableMap[returnValue] = StackOffset += 4;

//     std::string endLabel = "cond_" + addr + "_end";

//     bool hasElseClause = false;

//     for (int i = 1; i < node->children.size(); ++i) {
//         ASTNode* clause = node->children.at(i);

//         if (clause->children.size() != 2) {
//             throw std::runtime_error("ERROR: Each cond clause must have exactly 2 elements");
//         }

//         ASTNode* condExpr = clause->children.at(0);
//         ASTNode* bodyExpr = clause->children.at(1);

//         // Check for (else ...)
//         if (condExpr->type == NodeType::NT_Symbol && condExpr->value == "else") {
//             hasElseClause = true;

//             std::string elseLabel = "cond_" + addr + "_else";
//             emit(elseLabel, "", "label");

//             std::string elseOut = generate_code(bodyExpr);
//             emit(returnValue, elseOut, "=");
//             emit(endLabel, "", "jump");

//         } else {
//             // Regular (test body) clause
//             std::string condTemp = generate_code(condExpr);
//             std::string clauseLabel = "cond_" + addr + "_clause_" + std::to_string(i);
//             std::string nextClauseLabel = "cond_" + addr + "_next_" + std::to_string(i);

//             emit(clauseLabel, condTemp, "if", "jump"); // if condTemp != 0 jump to clauseLabel
//             emit(nextClauseLabel, "", "jump");         // otherwise skip to next clause

//             emit(clauseLabel, "", "label");
//             std::string result = generate_code(bodyExpr);
//             emit(returnValue, result, "=");
//             emit(endLabel, "", "jump");

//             emit(nextClauseLabel, "", "label");
//         }
//     }

//     emit(endLabel, "", "label");

//     if (!hasElseClause) {
//         // default return nil/0 if no condition matched
//         emit(returnValue, "0", "=");
//     }

//     return returnValue;
// }


// std::string Generator::handle_read_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ERROR: Expected only a filename");
//     }

//     std::string filename = generate_code(node->children.at(1));
//     std::string temp = generate_tmp();

//     emit(temp, filename, "read_file");

//     return temp;
// }

// std::string Generator::handle_scan_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ERROR: Expected only a variable");
//     }

//     std::string temp = generate_tmp();

//     emit(temp, "stdin", "scan_user_input");

//     return temp;
// }