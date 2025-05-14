The Lexer is the foundation of the language and should therefore be perfected and be solid.
Things to consider:
 - How broad or narrow the token categories should be 
 - How to distinguish code from data, especially for quoted lists
 - Ensuring the lexer is clean, solid and future-proof

Token Types:
 - LPAREN
 - RPAREN
 - QUOTE
 - ATOM (Symbols including operators)
 - NUMBER
 - STRING

An Atom is what is left after everything else is ruled out. This way i dont have to check
std::isalnum(c) || c == '+' || ...

The lexer skips comments:
    ; this is a comment

A Quoted list is not parsed here so
'(+ 1 2) will still be
QUOTE
LPAREN
ATOM
NUMBER
NUMBER
RPAREN


