#include "../include/tokenizer.hpp"
#include "../include/parser.hpp"


int main(){
    std::string input = "(+ 1 2)";

    Tokenizer tokenizer;
    tokenizer.setInput(input);
    tokenizer.tokenize();

    tokenizer.printTokens();


    std::vector<Token> tokens = tokenizer.getTokens();

    Parser parser(tokens);

    parser.parse();


}