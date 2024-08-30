//
// Created by Alexander on 24.08.24.
//

#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <unordered_map>

#include "Parser.hpp"

struct Environment {
    std::unordered_map<std::string, int> variables;
};

struct Function {
    std::vector<std::string> parameters;
    Node body;
};

inline std::unordered_map<std::string, Function> functions;

int evaluate(Node node, Environment& env);
int evaluateList(Node node, Environment& env);
int evaluateIf(Node node, Environment& env);
int evalualteFunctionCall(Node node, Environment& env);

// Node evaluateCons(Node node, Environment& env) {
//     Node first = evaluate(node.children[1], env);
//     Node second = evaluate(node.children[2], env);
//
//     Node pair;
//     pair.children.push_back(first);
//     pair.children.push_back(second);
//     return pair;
// }

// Node evaluateCar(Node node, Environment& env) {
//     Node list = evaluate(node.children[1], env);
//     if (!list.children.empty()) {
//         return list.children[0];
//     }
//     throw std::runtime_error("car: Argument is not a list or is empty");
// }

// Node evaluateCdr(Node node, Environment& env) {
//     Node list = evaluate(node.children[1], env);
//     if (!list.children.empty()) {
//         Node rest;
//         rest.children.assign(list.children.begin() + 1, list.children.end());
//         return rest;
//     }
//     throw std::runtime_error("cdr: Argument is not a list or is empty");
// }

// Node evaluateList(Node node, Environment& env) {
//     Node list;
//     for (size_t i = 1; i < node.children.size(); ++i) {
//         list.children.push_back(evaluate(node.children[i], env));
//     }
//     return list;
// }

// Node evaluateLength(Node node, Environment& env) {
//     Node list = evaluate(node.children[1], env);
//     Node lengthNode;
//     lengthNode.value = std::to_string(list.children.size());
//     return lengthNode;
// }
//
// Node evaluateAppend(Node node, Environment& env) {
//     Node firstList = evaluate(node.children[1], env);
//     Node secondList = evaluate(node.children[2], env);
//
//     Node resultList = firstList;
//     resultList.children.insert(resultList.children.end(), secondList.children.begin(), secondList.children.end());
//
//     return resultList;
// }


int evaluateIf(Node node, Environment& env) {
    if(evaluate(node.children[1], env)) {
        return evaluate(node.children[2], env);
    }else {
        if(node.children.size() > 3) {
            return evaluate(node.children[3], env);
        }else {
            return 0;
        }
    }
}

int evalualteFunctionCall(Node node, Environment& env) {
    std::string funcName = node.children[0].value;
    Function func = functions[funcName];

    // Create new local enviroment
    Environment localEnv = env;
    for(int i=0;i<func.parameters.size(); i++) {
        localEnv.variables[func.parameters[i]] = evaluate(node.children[i + 1],env);
    }

    return evaluate(func.body, localEnv);
}

int evaluateList(Node node, Environment& env) {
    if(node.children.empty()) return 0;

    std::string operation = node.children[0].value;

    if(functions.find(operation) != functions.end()) {
        return evalualteFunctionCall(node, env);
    }else if(operation == "define") {
        std::string varName = node.children[1].value;
        int value = evaluate(node.children[2], env);
        env.variables[varName] = value;
        return value;
    }else if(operation == "defun"){
        std::string funcName = node.children[1].value;
        std::vector<std::string> params;
        for (size_t i = 0; i < node.children[2].children.size(); ++i) {
            params.push_back(node.children[2].children[i].value);
        }
        functions[funcName] = {params, node.children[3]};

        return 0;
    }else if(operation == "set"){ // It's the same?
        std::string varName = node.children[1].value;
        int value = evaluate(node.children[2], env);
        env.variables[varName] = value;
    }else if(operation == "if") {
        return evaluateIf(node, env);
    }else if(operation == "cond") {
        int args = node.children.size();
        for(int i=1;i<args;i++) {
            Node cond = node.children[i];
            printf("testing for %s\n",cond.children[0].value.c_str());
            int isTrue = evaluate(cond.children[0],env);
            if(isTrue) return evaluate(cond.children[1], env);
        }
    }else if(operation == "print") {
        int a = evaluate(node.children[1],env);
        printf("%d\n",a);
        return 0;
    }else if(operation == "printc"){
        char a = (char)evaluate(node.children[1],env);
        printf("%c\n",a);
        return 0;
    }else if(operation == "else"){
        return 1;
    }else{
        int a = evaluate(node.children[1], env);
        int b = evaluate(node.children[2], env);
        int c = 0;

        if(operation == "+") {
            c = a + b;
        }
        if(operation == "-") {
            c = a - b;
        }
        if(operation == "*") {
            c = a * b;

        }
        if(operation == "/") {
            c = a / b;
        }
        if(operation == "==") {
            c = (a == b) ? 1 : 0;
        }
        if(operation == "!=") {
            c = (a != b) ? 1 : 0;
        }
        if(operation == ">") {
            c = (a > b) ? 1 : 0;
        }

        if(operation == "<") {
            c = (a < b) ? 1 : 0;
        }
        if(operation == "or") {
            c = (a || b) ? 1 : 0;
        }
        if(operation == "and") {
            c = (a && b) ? 1 : 0;
        }
        printf("%d %s %d = %d\n",a, operation.c_str(), b, c);
        printf("--------------\n");

        return c;
    }

    return 0;
}

int evaluate(Node node, Environment& env) {
    if(node.children.empty()){ // atom
        if(isdigit(node.value[0]) || (node.value[0] == '-' && node.value.size() > 1)) {
            return std::stoi(node.value);
        } else {
            if(node.value == "else") return 1;
            return env.variables[node.value];
        }
    } else {
        return evaluateList(node, env);
    }




}

#endif //EVALUATOR_HPP
