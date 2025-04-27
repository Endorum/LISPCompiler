#pragma once
#include <string>
#include <vector>
#include "tokenizer.hpp"


typedef enum NodeType {
    NT_None,          // Undefined or empty node
    NT_Number,        // Represents a number literal (e.g., 42, 3.14)
    NT_Symbol,        // Represents symbols like variables or functions
    NT_Operator,      // Represents operators (+, -, *, /)
    NT_Parenthesis,   // Special node for parentheses (optional, for clarity)
    NT_Boolean,       // TRUE/FALSE values
    NT_String,        // String literals (e.g., "hello")
    NT_Nil,           // Represents NIL (null in Lisp-like languages)
    NT_EOF,
} NodeType;

typedef enum GeneralType {
    GT_None,
    GT_Terminal,   // everything without parenthesies
    GT_Expression, // everything with parenthesies
    GT_Operator,   //
}GeneralType;


class ASTNode{
public:
    std::string value;

    NodeType type;
    GeneralType gtype;

    std::vector<ASTNode*> children;
    
    ASTNode(NodeType type, GeneralType gtype, const std::string& value) : type(type), gtype(gtype), value(value) {}

    void addChild(ASTNode* child) {
        children.push_back(child);
    }
};

class Parser{

public:
    Parser(std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

    std::vector<ASTNode*> parse() {
        return parseProgram();
    }



    Token getNext() {
        if(pos+1 < tokens.size())
            return tokens[pos++];
        return {NONE, "EOF"};
    }

    Token getCurrent() {
        if(pos < tokens.size())
            return tokens[pos];
        return {NONE, "EOF"};
    }

private:
    std::vector<Token>& tokens;
    size_t pos;


    std::vector<ASTNode*> parseProgram() {
        std::vector<ASTNode*> expressions;

        while (pos < tokens.size()) {
            ASTNode* expr = parseExpression();
            if (expr) {
                expressions.push_back(expr);
            }
        }

        return expressions;
    }

    size_t parenCounter = 0;

    ASTNode* parseExpression() {
        Token firstToken = getCurrent();
        ASTNode* localTree = nullptr;

        if (firstToken.type == LeftParen) {
            // Start of a new expression
            localTree = new ASTNode(NT_None, GT_Expression, "");
            pos++; // Move past '('

            while (pos < tokens.size() && tokens[pos].type != RightParen) {
                localTree->children.push_back(parseExpression());
            }

            // Ensure we consume the closing ')'
            if (pos < tokens.size() && tokens[pos].type == RightParen) {
                pos++;
            }

        } else if (firstToken.type == Number) {
            localTree = new ASTNode(NT_Number, GT_Terminal, firstToken.value);
            pos++;
        } else if (firstToken.type == String) {
            localTree = new ASTNode(NT_String, GT_Terminal, firstToken.value);
            pos++;
        } else if (
            firstToken.value == "+" ||
            firstToken.value == "-" ||
            firstToken.value == "*" ||
            firstToken.value == "/" ||

            firstToken.value == ">" ||
            firstToken.value == "<" ||
            firstToken.value == "=="

            ) { // the operators
            localTree = new ASTNode(NT_Symbol, GT_Operator, firstToken.value);
            pos++;
        } else if (firstToken.type == Symbol || firstToken.type == Keyword) {
            localTree = new ASTNode(NT_Symbol, GT_Terminal, firstToken.value);
            pos++;
        }

        return localTree;
    }




    bool isTerminal(Token& token){
        return (token.type == Number || token.type == String); // no symbol because this should be a function -> operator
    }


public:
    std::string generalTypeToString(const ASTNode & node) {
        if(node.gtype == GT_Expression) return "Expression";
        if(node.gtype == GT_Terminal) return "Terminal";
        if(node.gtype == GT_Operator) return "Operator";
        return "None";
    }

    std::string printASTNode(ASTNode& node, int indent = 0) {
        std::string res = "";

        for(int i=0;i<indent;i++) res += "\t";



        res += "General Type: \'" + generalTypeToString(node) + "\'";
        res += (node.type != NT_None) ? " Node Type: \'" + nodeTypeToString(node) + "\'" : "";
        res += (!node.value.empty()) ? " Value: \'"+ node.value + "\'" : "";
        res += (!node.children.empty()) ? " Children: \n" : "\n";
        for(auto child : node.children) {
            res += printASTNode(*child, indent + 1);
        }



        return res;
    }

    std::string nodeTypeToString(ASTNode& node) {
        std::string table[] = {
            "None",
            "Number",
            "Symbol",
            "Operator",
            "Parenthesis",
            "Boolean",
            "String",
            "Nil",
            "EOF",
        };

        return table[node.type];
    }

};