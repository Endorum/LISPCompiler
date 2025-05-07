#pragma once
#include <string>
#include <vector>
#include "tokenizer.hpp"


typedef enum NodeType {
    NT_None,          // Undefined or empty node
    NT_List,
    NT_Symbol,
    NT_Number,
    NT_String,
    NT_Quote
} NodeType;

class ASTNode{
public:
    std::string value;

    NodeType type;

    std::vector<ASTNode*> children;
    
    explicit ASTNode(const NodeType type) : type(type) { value = ""; }
    ASTNode(const NodeType type, std::string  value) : value(std::move(value)), type(type) {}

    void addChild(ASTNode* child) {
        children.push_back(std::move(child));
    }

    ~ASTNode() {
        for(auto child : children) {
            delete child; // test
        }
    }
};


class Parser {
public:
    explicit Parser(std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

    std::vector<ASTNode*> parse() {
        return parseProgram();
    }


    Token getNext() {
        if(pos+1 < tokens.size())
            return tokens.at(pos++);
        return {NONE, "EOF"};
    }

    Token getCurrent() {
        if(pos < tokens.size())
            return tokens.at(pos);
        return {NONE, "EOF"};
    }

private:
    std::vector<Token>& tokens;
    size_t pos;

    std::vector<ASTNode*> AST;


    void checkSyntax() const { //TODO: expand on that
        if(parenCounter > 0) {
            throw std::runtime_error("Expeced closing parentheses");
        }

        if(parenCounter < 0) {
            throw std::runtime_error("too many closing parentheses");
        }
    }

    void semanticAnalysis() {
        // for(const auto& expr : AST) {
        //     // basic type checking
        //
        // }
    }

    std::vector<ASTNode*> parseProgram(){

        // building the AST
        while (pos < tokens.size()) {
            ASTNode* expr = parseExpression();
            if (expr) {
                AST.push_back(std::move(expr));
            }
        }

        // Syntax Checking
        checkSyntax();

        // Semantic analysis
        semanticAnalysis();

        return std::move(AST);
    }

    size_t parenCounter = 0;

    ASTNode* parseExpression() {
        Token token = getCurrent();
        ASTNode* localTree = nullptr;

        if (token.type == LPAREN) {
            parenCounter++;

            // Start of a new expression
            localTree = new ASTNode(NT_List, "");
            pos++; // Move past '('

            // fill list with content
            while (pos < tokens.size() && tokens.at(pos).type != RPAREN) {
                localTree->addChild(parseExpression());
            }

            // Ensure we consume the closing ')'
            if (pos < tokens.size() && tokens.at(pos).type == RPAREN) {
                parenCounter--;
                pos++;
            }

        } else if (token.type == NUMBER) {
            localTree = new ASTNode(NT_Number, token.value);
            pos++;

        } else if (token.type == STRING) {
            localTree = new ASTNode(NT_String, token.value);
            pos++;

        } else if (token.type == ATOM) {
            localTree = new ASTNode(NT_Symbol, token.value);
            pos++;
        } else if (token.type == QUOTE) {
            pos++;
            localTree = new ASTNode(NT_Quote, "quote");
            localTree->addChild(parseExpression());

        } else {
            throw std::runtime_error("not yet implemented: " + std::to_string(token.type));
        }

        return localTree;
    }


public:

    std::string printASTNode(ASTNode& node, int indent = 0) {
        std::string res;

        for(int i=0;i<indent;i++) res += "\t";

        res += (node.type != NT_None) ? " Node Type: \'" + nodeTypeToString(node) + "\'" : "";
        res += (!node.value.empty()) ? " Value: \'"+ node.value + "\'" : "";
        res += (!node.children.empty()) ? " Children (" + std::to_string(node.children.size()) + "): \n" : "\n";
        for (const auto& child : node.children) {
            res += printASTNode(*child, indent + 1);
        }



        return res;
    }

    std::string nodeTypeToString(ASTNode& node) {
        std::string table[] = {
            "None",
            "List",
            "Symbol",
            "Number",
            "String",
            "Quote",
        };

        return table[node.type];
    }

};