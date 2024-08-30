#include <iostream>
#include <sstream>

#include "../include/Lexer.hpp"
#include "../include/Parser.hpp"
#include "../include/Evaluator.hpp"

inline int expr_start = 0;
inline int expr_end = 0;

std::string nextExpression(std::string input) {
    int parens = 0;
    int length = input.length();

    for (int i = expr_end; i < length; ++i) {
        if (input[i] == '(') {
            if (parens == 0) {
                expr_start = i;
            }
            parens++;
        } else if (input[i] == ')') {
            parens--;
            if (parens == 0) {
                expr_end = i + 1;
                return input.substr(expr_start, expr_end - expr_start);
            }
        }
    }

    return ""; // Return empty string if no more expressions
}

int main(int argc, char** argv) {


    // Test variable definition
    std::string input = "(defun square (x) (* x x) )"
                        "(print (square -5))"
                        "";

    Environment env;
    std::string expr = nextExpression(input);
    while(expr != "") {
        std::cout << "expr: " << expr << std::endl;
        Lexer lexer(expr);
        Parser parser(lexer);
        Node ast = parser.parseExpression();

        int res = evaluate(ast, env);
        printf("res %d\n",res);

        expr = nextExpression(input);
    }






    return 0;
}

