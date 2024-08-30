//
// Created by Alexander on 24.08.24.
//

#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>

enum TokenType {
    ATOM,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    std::string value;
};

bool charInString(char c, char* str) {
    while(str) {
        if(c == *str) return true;
        str++;
    }
    return false;
}

class Lexer {
public:

    explicit Lexer(const std::string& input) : input(input), position(0) {}

    Token nextToken() {
        skipWhiteSpace();
        if(position >= input.size()) return {END, ""};
        char current = input[position++];
        if(current == '(') return {LPAREN, "("};
        if(current == ')') return {RPAREN, ")"};
        if(isalnum(current) || charInString(current, "+-*/<>=!")) {
            return atom();
        }
        printf("something didnt go right: %c\n",current);exit(1);

    }

    void printToken(Token token) {
        printf("Token: \n");
        switch (token.type) {
            default:break;
            case ATOM:
                printf("\tatom\n");
                printf("\t%s\n",token.value.c_str());
                break;
        }
    }

private:
    size_t position;
    std::string input;

    void skipWhiteSpace() {
        while(position < input.size() && isspace(input[position])) position++;
    }

    Token atom() {
        size_t start = position - 1;
        while(position < input.size() && !isspace(input[position]) &&
            input[position] != '(' && input[position] != ')' )
        {
            position++;
        }
        return { ATOM, input.substr(start, position - start)};
    }
};

#endif //LEXER_HPP
