The Preprocessor runs BEFORE the lexer/parser etc.

Features of the Preprocessor:
- File inclusion: #include "file.lisp"
- Simple macros: #define PI "3.14" or #define inc "(lambda (x) (+ x 1))" <- using double quotes
- to allow multi line macros, which can also be nested

1. resolve includes
2. resolve definitions

# file inclusion
1. The file in the quotes is loaded 
2. and the content put into the resulting file in the place where the line #include "file.lisp" was
3. check if there are more inclusions left if so go to step 1

# macros
just a simple string -> string replacer
1. it runs through the code and saves the macros in an internal data structure
2. it removes the defintion lines 
3. it runs again and replaces every occourence of the first string with the second
4. check if there are still definitions left (for nested macros)
5. if so go to step 1