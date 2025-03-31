# Idea

A simple minimalistic subset of a LISP compiler which is compiled to assembly and assembled using NASM.
For a x86-64 Target.

# LISP basic Syntax:

S-Expressions:
- Either an Atom
- Or a List (of S-Expressions)

e.g.

(+ 1 2)

# Evalutation:

Evaluation is recursivly:
if an expression is an atom, returns its value
if its a list treat the first element as a function/operator and evaluate the rest as arguments.
e.g.
(+ (* 2 3) (- 5 1))  ; Evaluates to 6 + 4 = 10

# Data types:
Numbers: (0-9)^* (.) (0-9)^*
Symbols: eg x, foo
Strings: "this is a String"
Lists: eg (1 2 3)
booleans: TRUE / FALSE (optional could also use 0/1 (?) )

# built-in functions:
Arithmetic: +, -, * ,/
List operator:
- car: get first element
- cdr: rest of list
- cons: prepend element
- ..(?)
Comparisons:
=
<
>
Logic:
and
or
not

e.g.:
(car '(1 2 3))  ; Returns 1
(cdr '(1 2 3))  ; Returns (2 3)
(cons 0 '(1 2 3)) ; Returns (0 1 2 3)

# Function definition:
keyword: defun
e.g.

(defun square (x)
(* x x)
)

(square 5) ; Returns 25

# Conditionals:
e.g.
(if (> 5 3) "yes" "no")

# Recursion:
(defun factorial (n)
(if (= n 0) 1
(* n (factorial (- n 1)))))


# Tokenizer
The input stream is tokenized into simple tokens with a token type and a value which is just
its string. see tokenizer.cpp/hpp.

# AST
From the token stream an AST should be generated which is later converted into assembly/bytecode/whatever (im not sure yet)

# Could be a relevant resource:
https://github.com/LensPlaysGames/Corth/tree/master