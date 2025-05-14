The Parser transforms the token stream into an Abstract Syntax Tree for later evaluation

In LISP everything is either a List or an Atom (e.g. Number, String, etc.)
It should also support quoted lists. such that a list becomes an atom e.g. '(+ 1 2) is not a list/expression but an atom that can be used 
e.g. as a parameter.

For that I have a ASTNode class in which a type value and its children are stored.
In the last version there wre two types of types a general type (List or Atom) and a specific type: (e.g. Number String etc.)
But instead now I use only the specific type, and it is simply a list if children.size() > 0 and an Atom if children.size() = 0.

