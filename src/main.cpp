#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// #include "../include/codegen.hpp"
#include "../include/parser.hpp"
#include "../include/preproc.hpp"
#include "../include/tokenizer.hpp"

std::string readFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Could not open file: " + filename);
  }

  std::stringstream buffer;
  buffer << file.rdbuf(); // Read entire file into buffer
  return buffer.str();
}



int main() {
  std::string input = readFile("../test.lisp");

  // Preprocessor see preproc.md
  Preproc preproc(input);

  preproc.resolveAllIncludes();
  preproc.resolveAllMacros();

  std::cout << input << std::endl;
  // Preprocessor


  // Tokenizer see toknizer.md
  Tokenizer tokenizer;
  tokenizer.setInput(input);
  tokenizer.tokenize();

  std::vector<Token> tokens = tokenizer.getTokens();

  tokenizer.printTokens();
  // Tokenizer



  Parser parser(tokens);

  std::vector<std::unique_ptr<ASTNode>> program = parser.parse();

  for (const auto& expr : program) {
    std::cout << parser.printASTNode(*expr);
  }

  return 0;

  // Generator generator(program);
  // generator.declareFunctions();
  //
  // generator.generate();
  //
  // for (int i = 0; i < generator.functionNames.size(); i++) {
  //   // printf("function: %s\n", generator.functionNames.at(i).c_str());
  // }
  //
  // return 0;
}
