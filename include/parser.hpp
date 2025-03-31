#pragma once
#include <string>
#include <vector>
#include "tokenizer.hpp"


typedef enum NodeType {
    NT_None,          // Undefined or empty node
    NT_Number,        // Represents a number literal (e.g., 42, 3.14)
    NT_Symbol,        // Represents symbols like variables or functions
    NT_Operator,      // Represents operators (+, -, *, /)
    NT_Expression,    // Represents a general expression (a list or function call)
    NT_Parenthesis,   // Special node for parentheses (optional, for clarity)
    NT_Boolean,       // TRUE/FALSE values
    NT_String,        // String literals (e.g., "hello")
    NT_Nil,           // Represents NIL (null in Lisp-like languages)

    NT_Terminal,
} NodeType;


class ASTNode{
public:
    std::string value;
    NodeType type;
    std::vector<ASTNode*> children;
    
    ASTNode(NodeType type, const std::string& value) : type(type), value(value) {}

    void addChild(ASTNode* child) {
        children.push_back(child);
    }
};

class Parser{

public:
    Parser(std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

    ASTNode* parse() {
        return parseExpression();
    }

private:
    std::vector<Token>& tokens;
    size_t pos;

    ASTNode* parseExpression(){
        Token token = tokens[pos++];

        if(token.type == LeftParen){ // opening to a new expression
            /*
            switch first element
            case number/char/string -> a list
            case defun -> func decl
            case symbol/ +*.. -> operator
            
            */

            Token firstToken = tokens[pos++];

            if(isTerminal(firstToken)){
                // -> a list
            }
            else if(firstToken.value == "defun"){
                // -> func decl
            }
            else if(firstToken.type == Symbol){
                // -> func call / operation
                ASTNode* operatorNode = new ASTNode(NT_Operator, firstToken.value);
                operatorNode->addChild(parseOperands());  // Recursively parse operands
                return operatorNode;
            }

        }

    }

    bool isTerminal(Token& token){
        return (token.type == Number || token.type == String); // no symbol because this should be a function -> operator
    }

    ASTNode* parseList(Token& firstToken) {
        ASTNode* listNode = new ASTNode(NT_Expression, "list");
        listNode->addChild(new ASTNode(NT_Terminal, firstToken.value)); // Add the first element (if it's a terminal)

        while (tokens[pos].type != RightParen) {
            listNode->addChild(parseExpression());  // Recursively parse more expressions in the list
        }

        pos++;  // Consume the closing parenthesis
        return listNode;
    }

    ASTNode* parseFunctionDeclaration() {
        // Example format: defun functionName (arg1 arg2) body
        Token functionNameToken = tokens[pos++];  // Function name token
        Token leftParenToken = tokens[pos++];     // Opening parenthesis for argument list

        // Parse arguments (just assuming they're symbols for simplicity)
        ASTNode* funcNode = new ASTNode(NT_Operator, "defun");
        funcNode->addChild(new ASTNode(NT_Symbol, functionNameToken.value));

        while (tokens[pos].type != RightParen) {
            funcNode->addChild(parseExpression());  // Recursively parse arguments (could be symbols, etc.)
        }

        pos++;  // Consume the closing parenthesis
        funcNode->addChild(parseExpression());  // Parse the body of the function
        return funcNode;
    }

    ASTNode* parseOperands() {
        ASTNode* operandNode = new ASTNode(NT_Expression, "operands");

        // Recursively parse operands until we hit a closing parenthesis or end of the expression
        while (tokens[pos].type != RightParen) {
            operandNode->addChild(parseExpression());
        }

        return operandNode;
    }

    
};