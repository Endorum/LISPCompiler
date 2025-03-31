#include <iostream>

#include "../include/tokenizer.hpp"
#include "../include/parser.hpp"


int main(){
    std::string input = "(1 2 3)";

    Tokenizer tokenizer;
    tokenizer.setInput(input);
    tokenizer.tokenize();

    tokenizer.printTokens();


    std::vector<Token> tokens = tokenizer.getTokens();

    Parser parser(tokens);

    ASTNode* root = parser.parse();

    std::cout << parser.printASTNode(*root);

    return 0;


}