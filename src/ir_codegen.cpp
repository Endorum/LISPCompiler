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
                throw std::runtime_error("ir_codegen.cpp: ERROR: Function '" + funcName + "' is declared multiple times");
            }

            seenFunctions.insert(funcName);
            declaredFunctions.push_back(funcName);
        }
    }
}



std::string opCharToWord(std::string op) {
    if(op.size() == 1) switch (op.at(0)) {
        default: break;
        case '+': return "add";
        case '-': return "sub";
        case '*': return "imul";
        case '/': return "idiv";
        case '%': return "mod";

        case '&': return "and";
        case '|': return "or";
        case '^': return "xor";
        case '~': return "neg";

        case '<': return "l";
        case '>': return "g";
        case '=': return "eq";
        case '!': return "not";
    }

    if(op == "!="){
        return "noteq";
    }else if(op == ">="){
        return "geq";
    }else if(op == "<="){
        return "leq";
    }else if(op == "+="){
        return "addeq";
    }else if(op == "-="){
        return "subeq";
    }else if(op == "*="){
        return "imuleq";
    }else if(op == "/="){
        return "idiveq";
    }else if(op == "++"){
        return "inc";
    }else if(op == "--"){
        return "dec";
    }else if(op == "<<"){
        return "shl";
    }else if(op == "<<="){
        return "shleq";
    }else if(op == ">>"){
        return "shr";
    }else if(op == ">>="){
        return "shreq";
    }




    return op;
}

std::string Generator::wordToChar(std::string op){
    if(op == "add") return "+";
    if(op == "sub") return "-";
    if(op == "mul") return "*";
    if(op == "div") return "/";
    if(op == "mod") return "%";

    if(op == "and") return "&";
    if(op == "or") return "|";
    if(op == "xor") return "^";
    if(op == "neg") return "~";
    if(op == "not") return "!"; 

    if(op == "less") return "<"; 
    if(op == "greater") return ">"; 
    if(op == "equal") return "="; 

    if(op == "noteq") return "!=";
    if(op == "inc") return "++";
    if(op == "dec") return "--";

    return op;
}

Value Generator::handle_operator(ASTNode *node) {
    std::string op = node->children.at(0)->value;
    op = wordToChar(op);

    // Unary operators
    if (op == "~" || op == "!") {
        if (node->children.size() != 2) {
            throw std::runtime_error("ir_codegen.cpp: ERROR: Expected one argument for unary operator");
        }

        Value dst = generate_tmp();
        Value operand = generate_code(node->children.at(1));
        emit(dst, opCharToWord(op), operand);
        return dst;
    }

    // Assignment-style ops (+=, -=, etc.)
    if (op == "+=" || op == "-=" || op == "*=" || op == "/=" || op == "<<=" || op == ">>=") {
        if (node->children.size() != 3) {
            throw std::runtime_error("ir_codegen.cpp: ERROR: Expected exactly two arguments for compound assignment operator");
        }
        ASTNode* local = node->children.at(1);
        Value loc_local = variable_table[local->value];
        Value right = generate_code(node->children.at(2));
        emit(loc_local, opCharToWord(op), loc_local, right);
        return loc_local;
    }

    // Variadic binary operators
    if (node->children.size() < 3) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected at least two arguments for binary operator: " + op);
    }

    // Left fold: (((arg1 op arg2) op arg3) op arg4)
    Value acc = generate_code(node->children.at(1));
    for (size_t i = 2; i < node->children.size(); ++i) {
        Value next = generate_code(node->children.at(i));
        Value tmp = generate_tmp();
        emit(tmp, opCharToWord(op), acc, next);
        acc = tmp;
    }

    return acc;
}


// (let x 5) TODO: maybe implement scoped variables
Value Generator::handle_let_keyword(ASTNode *node) {
    if (node->children.size() < 3) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Invalid let expression");
    }

    if (node->children.at(1)->type != NT_Symbol) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected a Symbol for the variable name");
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

    if(node->children.at(2)->type == NT_String){
        localVar.valueType = IMM_STR;
    }else if(node->children.at(2)->type == NT_Symbol){
        localVar.valueType = LOCAL;
    }


    // Store the initializer value into the local variable
    emit(localVar, "assign", initVal);

    // Register in symbol table
    variable_table[varname] = localVar;

    return localVar;
}

Value Generator::handle_set_keyword(ASTNode *node){
    if (node->children.size() < 3) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Invalid set expression");
    }

    if (node->children.at(1)->type != NT_Symbol) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected a Symbol for the variable name");
    }

    std::string varname = node->children.at(1)->value;

    if(variable_table.find(varname) == variable_table.end()){
        throw std::runtime_error("ir_codegen.cpp: Cant set variable, " + varname + " not declared in this scope");
    }

    Value var = variable_table[varname];

    // Evaluate the initializer expression
    Value initVal = generate_code(node->children.at(2));

    variable_table[varname] = initVal;

    // Allocate a new local variable slot
    // Value localVar;
    // localVar.value = varname;
    // localVar.type = LOCAL;
    // localVar.offSet = local_count++;  // Track how many locals exist
    // localVar.loc = getLocalLocation(localVar.offSet);

    // if(node->children.at(2)->type == NT_String){
    //     localVar.valueType = IMM_STR;
    // }else if(node->children.at(2)->type == NT_Symbol){
    //     localVar.valueType = LOCAL;
    // }


    // Store the initializer value into the local variable
    emit(var, "assign", initVal);

    return var;
}


Value Generator::handle_defun_keyword(ASTNode *node) {
    std::string functionName = node->children.at(1)->value;
    ASTNode* parameters = node->children.at(2);

    variable_table.clear(); // clear from previous scope

    if (!parameters) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected parameters");
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

    // check if body is empty
    if(node->children.size() < 4){
        throw std::runtime_error("ir_codegen.cpp: Missing a function body for function: \'" + func.name + "\'");
    }

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
        throw std::runtime_error("ir_codegen.cpp: ERROR: Function " + funcName + " is not declared");
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
            throw std::runtime_error("ir_codegen.cpp: ERROR: Function " + funcName + " expects " +
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

Value Generator::handle_if_keyword(ASTNode *node) {
    if(node->children.size() < 4) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected: (if (<cond>) (<true body>) (<false body>) )");
    }
    ASTNode* cond = node->children.at(1);
    ASTNode* trueBody = node->children.at(2);
    ASTNode* falseBody = node->children.at(3);

    Value condValue = generate_code(cond);
    Value result = generate_tmp();

    std::string id = "_" + std::to_string(labelId++);
    std::string labelTrue = "label_if_true" + id;
    std::string labelFalse = "label_if_false" + id;
    std::string labelEnd = "label_if_end" + id;

    emit(Value(labelTrue), "if", condValue, Value(labelFalse));

    emit(Value(labelFalse), "label");
    Value resIfFalse = generate_code(falseBody);
    emit(result, "assign", resIfFalse);
    emit(Value(labelEnd),"jump");
    
    emit(Value(labelTrue), "label");
    Value resIfTrue = generate_code(trueBody);
    emit(result, "assign", resIfTrue);

    emit(Value(labelEnd), "label");



    return result;
}

Value Generator::handle_print_keyword(ASTNode *node) {
    // (print string/stringvar/var)
    if(node->children.size() < 2) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected: (print <string/stringvariable>)");
    }

    Value strAddr = generate_code(node->children.at(1)); // address of string

    emit(Value("stdout"), "print", strAddr);

    return strAddr;
}

Value Generator::handle_cond_keyword(ASTNode *node) {
    if (node->children.size() < 2) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: cond expects at least one clause");
    }

    Value res = generate_tmp();
    std::string id = "_" + std::to_string(labelId++);
    std::string labelEnd = "label_cond_end" + id;
    bool hasElse = false;


    for(int i=1;i<node->children.size();i++){
        ASTNode* clause = node->children.at(i);

        if(clause->children.size() != 2){
            throw std::runtime_error("ir_codegen.cpp: Each cond clause must have exactly 2 elements");
        }

        ASTNode* test = clause->children.at(0);
        ASTNode* expr = clause->children.at(1);

        std::string clauseLabel = "label_clause_" + std::to_string(i) + id;
        std::string NextClauseLabel = "label_next_" + std::to_string(i) + id;

        if(test->type == NT_Symbol && test->value == "else"){
            hasElse = true;
            emit(Value(clauseLabel), "label");
            Value elseResult = generate_code(expr);
            emit(res, "assign", elseResult);
            emit(Value(labelEnd), "jump");
        }else{
            Value condValue = generate_code(test);
            emit(Value(clauseLabel), "if", condValue, Value(NextClauseLabel));
            // emit(Value(NextClauseLabel), "jump");
            emit(Value(clauseLabel), "label");
            Value bodyResult = generate_code(expr);
            emit(res, "assign", bodyResult);
            emit(Value(labelEnd), "jump");
            emit(Value(NextClauseLabel), "label");
        }
    }

    emit(Value(labelEnd), "label");

    if(!hasElse){
        // emit(res, "assign", Value("0",IMM_NUM));
        throw std::runtime_error("ir_codegen.cpp: Expected an else");
    }

    return res;
}

Value Generator::handle_cons_keyword(ASTNode *node) {
    if (node->children.size() < 3) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Invalid cons expression: (cons <first> <second>)");
    }

    Value left = generate_code(node->children.at(1));
    Value right = generate_code(node->children.at(2));
    Value temp = generate_tmp();
    emit(temp, "cons", left, right);
    return temp;
}


Value Generator::handle_car_keyword(ASTNode* node) {
    if (node->children.size() != 2) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected usage: (car <list>)");
    }

    Value listAddr = generate_code(node->children.at(1));
    Value dst = generate_tmp();

    // Emit a high-level IR op like: "t2 = car t1"
    emit(dst, "car", listAddr);

    return dst;
}

Value Generator::handle_cdr_keyword(ASTNode *node) {
    // (cdr <list>)
    if(node->children.size() < 2) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected: (cdr <list>)");
    }

    Value listAddr = generate_code(node->children.at(1)); // address of the list
    Value dst = generate_tmp();

    // Return the pointer to the rest of the list, starting from element 2 (offset 4 bytes)
    emit(dst, "cdr", listAddr);

    return dst;
}

Value Generator::handle_deref_keyword(ASTNode* node){
    if(node->children.size() < 2) {
        throw std::runtime_error("ir_codegen.cpp: ERROR: Expected: (deref <variable / const string>)");
    }


    ASTNode* addr = node->children.at(1);
    Value Addr = generate_code(addr);
    Value dst = generate_tmp(); // stores value at dst which is stored at [Addr]

    if(addr->type == NT_Symbol){
        // try to find out if the symbol can be dereferenced, so right now if it points to a string
        const std::string& name = addr->value;
        auto it = variable_table.find(name);
        if(it == variable_table.end()){
            throw std::runtime_error("ir_codegen.cpp: deref called on undefined symbol " + printASTNode(*node));    
        }

        const Value& info = it->second;

        // if(info.type != VarType::IMM_STR && info.type != VarType::LOCAL){
        //     throw std::runtime_error("ir_codegen.cpp: deref can only be used on string and locals right now" + printASTNode(*node));
        // }

    }else if(addr->type == NT_String){
        // this always works
    }else if(addr->type == NT_Number){
        throw std::runtime_error("ir_codegen.cpp: An immediate can not be dereferenced " + printASTNode(*node));
    }else if(addr->type == NT_List){
        // should work depending on the result of the list ofcourese
    }else{
        throw std::runtime_error("ir_codegen.cpp: deref operand must be a symbol, string literal or an expression\n" + printASTNode(*node));
    }

    emit(dst, "deref", Addr);

    return dst;
}

Value Generator::handle_while_keyword(ASTNode* node){
    if(node->children.size() < 3){
        throw std::runtime_error("ir_codegen.cpp: Invalid while expression" + printASTNode(*node));
    }

    
    std::string id = "_" + std::to_string(labelId++);
    std::string labelStart = "label_while_start" + id;
    std::string labelBody = "label_while_body" + id;
    std::string labelEnd = "label_while_end" + id;
    
    emit(Value(labelStart), "label");
    
    Value cond = generate_code(node->children.at(1));
    emit(Value(labelBody), "if", cond, Value(labelEnd));
    // emit(Value(labelEnd), "jump");
    emit(Value(labelBody), "label");


    Value lastOutput;
    int i=2;
    for(;i<node->children.size();i++){
        lastOutput = generate_code(node->children.at(i));
    }
    // lastOutput = generate_code(node->children.at(i));

    emit(Value(labelStart), "jump");

    emit(Value(labelEnd), "label");
    

    
    return lastOutput;
}

Value Generator::handle_setchar_keyword(ASTNode* node){
    if(node->children.size() != 4){
        throw std::runtime_error("ir_codegen.cpp: invalid setchar expression");
    }

    Value ptr = generate_code(node->children.at(1));
    Value index = generate_code(node->children.at(2));
    Value character = generate_code(node->children.at(3));

    emit(Value(ptr), "setchar", index, character);

    return ptr;
}

Value Generator::handle_malloc_keyword(ASTNode* node){
    if(node->children.size() != 2){
        throw std::runtime_error("ir_codegen.cpp: malloc expects a single integer argument");
    } 

    Value size = generate_code(node->children.at(1));
    Value dst = generate_tmp();

    emit(dst, "malloc", size);

    
    return dst;
}

// std::string Generator::handle_null_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ir_codegen.cpp: ERROR: Invalid null? expression: (null? <list>)");
//     }

//     std::string listAddr = generate_code(node->children.at(1));
//     std::string temp = generate_tmp();

//     emit(temp, listAddr, "null?");

//     return temp;
// }

// std::string Generator::handle_toList_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ir_codegen.cpp: ERROR: Invalid toList expression: (toList <string>)");
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
//         throw std::runtime_error("ir_codegen.cpp: ERROR: Invalid toString expression: (toString <list>)");
//     }


//     const std::string listAddr = generate_code(node->children.at(1));
//     std::string temp = generate_tmp();

//     emit(temp, listAddr, "list_to_string");

//     return temp;
// }







// std::string Generator::handle_length_keyword(ASTNode *node) {
//     // (length list)
//     if(node->children.size() < 2) {
//         throw std::runtime_error("ir_codegen.cpp: ERROR: Expected: (length <list>)");
//     }


//     std::string listAddr = generate_code(node->children.at(1)); // address of list
//     std::string temp = generate_tmp();
//     emit(temp, listAddr, "listLength");

//     return temp;
// }








// std::string Generator::handle_read_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ir_codegen.cpp: ERROR: Expected only a filename");
//     }

//     std::string filename = generate_code(node->children.at(1));
//     std::string temp = generate_tmp();

//     emit(temp, filename, "read_file");

//     return temp;
// }

// std::string Generator::handle_scan_keyword(ASTNode *node) {
//     if (node->children.size() != 2) {
//         throw std::runtime_error("ir_codegen.cpp: ERROR: Expected only a variable");
//     }

//     std::string temp = generate_tmp();

//     emit(temp, "stdin", "scan_user_input");

//     return temp;
// }