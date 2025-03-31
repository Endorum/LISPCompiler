#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "../include/tokenizer.hpp"
#include "../include/parser.hpp"
#include "../include/codegen.hpp"



std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // Read entire file into buffer
    return buffer.str();
}

int main(){
    std::string input = readFile("../test.lisp");

    Tokenizer tokenizer;
    tokenizer.setInput(input);
    tokenizer.tokenize();

    tokenizer.printTokens();



    std::vector<Token> tokens = tokenizer.getTokens();

    Parser parser(tokens);

    std::vector<ASTNode*> program = parser.parse();

    for(auto expr : program) {
        std::cout << parser.printASTNode(*expr);
    }

    Generator generator(program);

    generator.evalExpr(program[0]);
    generator.evalExpr(program[1]);


    printf("%s\n",generator.res.c_str());



    return 0;


}