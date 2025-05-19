#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

#include "../include/asmgen.hpp"
#include "../include/ir_codegen.hpp"
#include "../include/parser.hpp"
#include "../include/preproc.hpp"
#include "../include/tokenizer.hpp"


// Assume all your includes like Preproc, Tokenizer, Parser, Generator, Asmgen, etc., are here

std::string readFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        throw std::runtime_error("main.cpp: Failed to open input file: " + filename);
    }
    return std::string((std::istreambuf_iterator<char>(inFile)),
                        std::istreambuf_iterator<char>());
}

void writeToFile(const std::string& content, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("main.cpp: Failed to open output file: " + filename);
    }

    outFile << content;

    if (!outFile) {
        throw std::runtime_error("main.cpp: Failed to write to file: " + filename);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file.lisp> <output_file.asm>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    try {
        std::string input = readFile(inputFile);

        // Preprocessor
        Preproc preproc(input);
        preproc.resolveAllIncludes();
        preproc.resolveAllMacros();

        // Tokenizer
        Tokenizer tokenizer;
        tokenizer.setInput(input);
        tokenizer.tokenize();
        std::vector<Token> tokens = tokenizer.getTokens();
        // tokenizer.printTokens();

        
        // Parser
        Parser parser(tokens);
        std::vector<ASTNode*> program = parser.parse();
        for (const auto& expr : program) {
            std::cout << parser.printASTNode(*expr);
        }
        
        
        // IR generation
        Generator generator(program);
        generator.findFuncDecls();
        generator.generateIR();
        std::vector<IRInstruction> instructions = generator.instructions;
        for (auto& instr : instructions) {
            std::cout << instr.str() << std::endl;
        }


        for (const auto& pair : generator.variable_table) {
            std::cout << pair.first << " : " << pair.second.valueType  << std::endl;
        }

        // for(const auto& func : generator.functions) {
        //     std::cout << "function: " << func.name << ", parameters: " << std::to_string(func.parameters) << std::endl;
        // }

        // // return 0;

        // Assembly generation
        Asmgen asmgenerator(instructions, generator.variable_table, generator.functions);
        asmgenerator.generate();
        std::string asm_out = asmgenerator.getAsm();
        // std::cout << asm_out << std::endl;

        // Write to file
        writeToFile(asm_out, outputFile);
        std::cout << "Assembly written to: " << outputFile << "\n";
    } catch (const std::exception& e) {
        std::cerr << "ERROR in main: " << e.what() << "\n";
        return 1;
    }

    // std::string nasm = "nasm -f elf32 " + outputFile +" -o " + outputFile + ".o";
    // system(nasm.c_str());

    // // i686-elf-ld -o out out.o
    // std::string ld = "i686-elf-ld -o main " + outputFile + ".o";
    // system(ld.c_str());

    // std::string pass = "multipass transfer main linux:/home/ubuntu/main";
    // system(pass.c_str());

    // std::string clean = "rm *.o";
    // system(clean.c_str());

    return 0;
}
