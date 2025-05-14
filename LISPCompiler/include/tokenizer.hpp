#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

typedef enum TokenType {
  NONE,       // Default/invalid token
  LPAREN,
  RPAREN,
  ATOM, // Symbol
  NUMBER,
  STRING,
  QUOTE,
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

  void skipComment();
  Token readString();
  Token readNumber();
  Token readSymbol();


  std::vector<Token> getTokens() { return tokens; }

  void setInput(std::string &v) { input = v; }

  void printError(std::string msg) {
    printf("ERROR at position: %zu\n", position);
    printf("    %s\n", msg.c_str());
    exit(1);
  }

  std::string toString(const Token &token) {
    static const std::string ttypeTable[] = {
      "NONE", "LPAREN", "RPAREN", "ATOM", "NUMBER", "STRING", "QUOTE"
  };

    std::ostringstream oss;
    oss << "Token:\n";
    oss << "    type:  " << ttypeTable[token.type] << "\n";
    oss << "    value: " << token.value << "\n";

    return oss.str();
  }


  void printTokens() {
    for (auto token : tokens) {
      std::cout << toString(token) << std::endl;
    }
  }

private:
  std::string input;
  std::vector<Token> tokens;

  size_t position = 0;
};
