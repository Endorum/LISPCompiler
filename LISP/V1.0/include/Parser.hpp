//
// Created by Alexander on 24.08.24.
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

#include "Lexer.hpp"

struct Node {
    std::string value;
    std::vector<Node> children;
};

class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer) {}

    Node parseExpression() {
        Token token = lexer.nextToken();
        if(token.type == LPAREN) {
            Node listNode = parseList();
            return listNode;
        }else if(token.type == ATOM) {
            return { token.value, {} };
        }else {
            printf("error in node parse expression\n");exit(1);
        }
    }
    Lexer& lexer;
private:

    Node parseList() {
        Node listNode = { "list", {} };
        while(true) {
            Token token = lexer.nextToken();
            if(token.type == RPAREN) break;
            if(token.type == END) break;
            if(token.type == LPAREN) {
                listNode.children.push_back(parseList());
            }else if(token.type == ATOM) {
                listNode.children.push_back( {token.value, {} } );
            }
        }
        return listNode;
    }
};

#endif //PARSER_HPP
