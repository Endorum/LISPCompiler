#pragma once
#include <string>
#include <vector>

typedef enum TokenType {
  NONE,       // Default/invalid token
  Number,     // Numeric values
  Symbol,     // Variable names, function names, operators
  LeftParen,  // '('
  RightParen, // ')'
  String,     // "text"
  Quote,      // "'"
  Boolean,    // TRUE / FALSE
  Keyword,    // defun, if, ...
} TokenType;

typedef struct Token {
  TokenType type;
  std::string value;
} Token;

/*

The Tokenizer should accept a string with the input
and return a stream of tokens where each token has a type and a value


*/

class Tokenizer {
public:
  void tokenize();

  bool isSymbolChar(char c);
  Token readNumber();
  Token readSymbol();
  Token readString();
  void skipComment();

  std::vector<Token> getTokens() { return tokens; }

  void setInput(std::string &v) { input = v; }

  void printError(std::string msg) {
    printf("ERROR at position: %zu\n", position);
    printf("    %s\n", msg.c_str());
    exit(1);
  }

  void printToken(Token &token) {

    std::string ttypeTable[] = {
        "NONE",   "Number", "Symbol",  "LeftParen", "RightParen",
        "String", "Quote",  "Boolean", "Keyword",   "Dot",
    };

    printf("Token:\n");
    printf("    type: %s\n", ttypeTable[token.type].c_str());
    printf("    value: %s\n", token.value.c_str());
  }

  void printTokens() {
    for (int i = 0; i < tokens.size(); i++) {
      printToken(tokens[i]);
    }
  }

private:
  std::string input;
  std::vector<Token> tokens;

  size_t position = 0;
};
