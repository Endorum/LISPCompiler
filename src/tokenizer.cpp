#include "../include/tokenizer.hpp"

void Tokenizer::tokenize() {

  while (position < input.size()) {
    char c = input.at(position);

    if (isspace(c)) {
      position++;
    } else if (c == ';') {
      skipComment();
    } else if (c == '(') {
      tokens.push_back({LPAREN, "("});
      position++;
    } else if (c == ')') {
      tokens.push_back({RPAREN, ")"});
      position++;
    } else if (c == '\'') {
      tokens.push_back({QUOTE, "\'"});
      position++;
    } else if (c == '\"') {
      tokens.push_back(readString());
      position++;
    } else if (std::isdigit(c) || (c == '-' && std::isdigit(input.at(position + 1)))) {
      tokens.push_back(readNumber());
    } else {
      tokens.push_back(readSymbol());
    }
  }
}

Token Tokenizer::readNumber() {
  size_t start = position;
  bool hasDot = false;
  while (position < input.size() &&
         (std::isdigit(input.at(position)) || input.at(position) == '.')) {
    if (input.at(position) == '.') {
      if (hasDot) {
        printError("Can only contain one dot!");
        exit(1);
      }
      hasDot = true;
    }
    position++;
  }
  return {NUMBER, input.substr(start, position - start)};
}

Token Tokenizer::readSymbol() {
  size_t start = position;

  // advance until a space/(/)/'/etc is found
  while (position < input.size()
      && !(
        isspace(input.at(position)) ||
        input.at(position) == '(' ||
        input.at(position) == ')' ||
        input.at(position) == '.' ||
        input.at(position) == ',' ||
        input.at(position) == '\'' ||
        input.at(position) == '\"' ) ) {
    position++;
  }
  std::string value = input.substr(start, position - start);

  return {ATOM, value};
}

Token Tokenizer::readString() {

  size_t start = position;
  position++;

  while (position < input.size() && input.at(position) != '\"') {
    position++;
    if (position >= input.size()) {
      printError("Unterminated string literal");
      exit(1);
    }
  }


  return {STRING, input.substr(start + 1, position - start - 1)};
}

void Tokenizer::skipComment() {
  while (position < input.size() && input.at(position) != '\n') {
    position++;
  }
}
