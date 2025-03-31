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

    NT_Expression,    // Represents a general expression (a list or function call)
    NT_List,          // A list where at least the first element is a Terminal
    NT_Terminal,      // a number or a String
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

    ASTNode* parseExpression() {
        Token firstToken = getCurrent();
        ASTNode* localTree = nullptr;

        if (firstToken.type == LeftParen) {
            // Opening parenthesis indicates a new list
            localTree = new ASTNode(NT_List, "");  // Value is irrelevant for the list itself

            Token elmToken = firstToken;

            while(tokens[pos+1].type != RightParen) {
                pos++;
                localTree->children.push_back(parseExpression());
            }



        } else if (firstToken.type == Number || firstToken.type == String) {
            // If it's a terminal (number/string), return a node for it
            localTree = new ASTNode(NT_Terminal, firstToken.value);
        } else if (firstToken.type == Symbol) {
            // Handle symbol if necessary, e.g., function names or operators
            localTree = new ASTNode(NT_Terminal, firstToken.value);
        } else if(firstToken.type == NONE && firstToken.value == "EOF") {
            localTree = nullptr;
        }

        return localTree;
    }



    bool isTerminal(Token& token){
        return (token.type == Number || token.type == String); // no symbol because this should be a function -> operator
    }


public:
    std::string printASTNode(ASTNode& node, int indent = 0) {
        std::string res = "";

        for(int i=0;i<indent;i++) res+=" ";

        // Print node type
        if(indent == 0) res += "Node:\n";
        res += "type: " + nodeToString(node) + "\n";

        // Print the value of the node (if applicable)
        if (!node.value.empty()) {
            indent++;
            for(int i=0;i<indent;i++) res+=" ";
            res += "value: " + node.value + "\n";
        }

        // Print children if there are any
        if (!node.children.empty()) {
            res += "children:\n";
            indent++;
            for(int i=0;i<indent;i++) res+=" ";

            // Recursively print each child node
            for (size_t i = 0; i < node.children.size(); ++i) {
                res += "child" + std::to_string(i) + ":\n" + printASTNode(*node.children[i], indent);
            }
        }

        return res;
    }

    std::string nodeToString(ASTNode& node) {
        std::string table[] = {
            "None",
            "Number",
            "Symbol",
            "Operator",
            "Expression",
            "Parenthesis",
            "Boolean",
            "String",
            "Nil",
            "Terminal",
            "List",
        };

        return table[node.type];
    }

};