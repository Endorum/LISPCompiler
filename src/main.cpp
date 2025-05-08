#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../include/asmgen.hpp"
#include "../include/codegen.hpp"
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


  // Parser see parser.md
  Parser parser(tokens);

  std::vector<ASTNode*> program = parser.parse();

  for (const auto& expr : program) {
    std::cout << parser.printASTNode(*expr);
  }
  // Parser

  // IR code generation see codegen.md
  Generator generator(program);
  generator.findFuncDecls();

  generator.generateIR();

  std::vector<IRInstruction> instructions = generator.instructions;

  for(const auto& instr : instructions) {
    std::cout << instr.str(true) << std::endl;
  }
  // IR code generation

  for(const auto& pair : generator.variableMap) {
    std::cout << pair.first << " : " << std::to_string(pair.second) << std::endl;
  }


  // Assembly generation
  Asmgen asmgenerator(instructions,generator.variableMap);
  asmgenerator.generate();
  std::cout << asmgenerator.getAsm() << std::endl;
  // Assembly generation


  return 0;
}
