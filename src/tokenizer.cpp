#include "../include/tokenizer.hpp"


void Tokenizer::tokenize() {

    while(position < input.size()){
        char c = input[position];

        if(isspace(c)){
            position++;
        }else if(c == ';'){
            skipComment();
        }else if(c == '('){
            tokens.push_back({LeftParen, "("});
            position++;
        }else if(c == ')'){
            tokens.push_back({RightParen, ")"});
            position++;
        }else if(c == '\''){
            tokens.push_back({Quote, "\'"});
            position++;
        }else if(c == '.'){
            tokens.push_back({Dot, "."});
            position++;
        }else if(c == '\"'){
            tokens.push_back(readString());
        }else if(std::isdigit(c) || (c == '-' && std::isdigit(input[position+1]))){
            tokens.push_back(readNumber());
        }else if (std::isalpha(c) || isSymbolChar(c)){
            tokens.push_back(readSymbol());
        }else{
            printError("Unknown character");
            tokens.push_back({NONE, std::string() + c});
            
        }
    }
}

Token Tokenizer::readNumber() {
    size_t start = position;
    bool hasDot = false;
    while(position < input.size() && (std::isdigit(input[position]) || input[position] == '.' )){
        if(input[position] == '.'){
            if(hasDot){
                printError("Can only contain one dot!");
                exit(1);
            }else{
                hasDot = true;
            }

        }
        position++; 
    }
    return {Number, input.substr(start, position - start)};
}

bool Tokenizer::isSymbolChar(char c) {
    return std::isalnum(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '!';
}


Token Tokenizer::readSymbol()
{
    size_t start = position;
    while (position < input.size() && isSymbolChar(input[position])) {
        position++;
    }
    std::string value = input.substr(start, position - start);

    // Check for Lisp keywords
    if (value == "defun" || value == "if" || 0) {
        return {Keyword, value};
    }

    if (value == "TRUE" || value == "FALSE") {
        return {Boolean, value};
    }

    return {Symbol, value}; // Default case
}

Token Tokenizer::readString() {
    
    size_t start = position;
    position++;
    while(position < input.size() && input[position] != '\"'){
        position++;
        printf("%c\n",input[position]);
    }
    position++;
    return {String, input.substr(start, position - start)};
}

void Tokenizer::skipComment() {
    while (position < input.size() && input[position] != '\n') {
        position++;
    }
}