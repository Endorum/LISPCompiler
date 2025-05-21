# 1 Overview
- This is LISPC a LISP Compiler written in C++, it takes LISP like code and produces x86 32 bit linux assembly code.
- The usage is simply: ./LISPC <path/to/code.lisp> <path/to/result.asm>
- The purpose of this language is primarily for me to learn making a compiler, which it has done successfully
- Here is some example code:
```LISP
#include "stdlib.lisp"

(defun reverse_str (str)
    (let len (strlen str))
    (let out "..........")
    (let i 0)
    (while (< i len)
        (let c ( 
            deref (+ str (- (- len i ) 1) )
            )
        )
        (setbyte out i c)
        (set i (+ i 1))
    )
    out
)

(defun itoa (num base)
    (let tmp "..........") ;; pre allocated result
    (let i 0)

    (while (> num 0)
        (let digit (mod num base))
        (let ch (digitToString digit))
        (setbyte tmp i ch) ;; set a character in tmp
        (+= i 1)
        (/= num base)
    )

    (reverse_str tmp)
)


(defun main()
  (let result 0)
  (let A (+ 1 2))
  (let B (if (< 1 2) 321 123))
  (printLn (itoa (* A B) 10))
)

```

- Key features are:
    - a preprocessor which solves #include "name.lisp" and #define PI "3.14"
    - functional language with multiline function bodies 
    - if/cond statements 
    - while loops
    - locally scoped variables
    - compound assignment operators (+= etc)

- The compiler goes through the following steps and using these core structures:
    - preprocessor
        - #include "libname.lisp" looks for the file and inserts its content right where the #include directiv is
        - #define NAME "value" a simple string -> string replacer searches for every occurence and replaces NAME with value
    - lexer / tokenizer -> generates a token stream:
        ```C
        typedef struct Token {
            TokenType type;
            std::string value;
        } Token;
        ```
    - parser -> takes in the token stream and generates an Abstract Syntax Tree (AST):
        ```C
        typedef enum NodeType {
            NT_None,          // Undefined or empty node
            NT_List,
            NT_Symbol,
            NT_Number,
            NT_String,
            NT_Quote
        } NodeType;

        class ASTNode{
        public:
            std::string value;

            NodeType type;

            std::vector<ASTNode*> children;

            void addChild(ASTNode* child) {
                children.push_back(std::move(child));
            }
        };
        ```

    - Intermediate Representation (IR) generation -> works through the AST 
        recursively and creates objects like:
        ```C
        struct IRInstruction {
            Value dst;
            std::string op;
            Value src1;
            Value src2;
            

            std::string str(){
                std::string dstString = "dst: " + dst.str();
                
                std::string src1String = (!src1.value.empty()) ? "src1: " + src1.str() + "\n": "";
                std::string src2String = (!src2.value.empty()) ? "src2: " + src2.str() + "\n": "";
                return dstString + "\n" + op + "\n" + src1String + src2String;
            }

        };
        ```
        Where each Value is:
        ```C
        struct Value{
    
            std::string value;
            VarType type = VT_NONE;

            std::string loc = "";
            size_t offSet = 0;

            VarType valueType = VT_NONE;

        };

        ```
        - The loc field is determined in the IR generation phase it stores something like [ebp - 4] or just 31 for numbers and similar
        - it uses a more dynamic Three address code syntax
    - Assembly code generation -> generates x86, 32-Bit linux assembly (for int's) :
        It only consists out of one object, but it stores:
        ```C
        std::string asm_result; // storing the result just as a string
        std::vector<IRInstruction> input; // the input is a vector of IRInstructions
        std::unordered_map<std::string, Value> variable_table; // stores variables including parameters
        std::vector<Function> functions; // stores the declared functions, so they are visable for all other functions
        std::map<std::string, std::string> stringLabelMap; // stores the strings which are stored in the data section with name and content

        int stringCounter = 0; // increased everytime a string is created so that every string gets a uniqe name e.g. str_0 str_1 etc
        int listPtrCounter = 0; // points to the beginning of the list space 
        ```



# 2 Syntax and semantics
- S-expressions
- Literals ( Integer numbers and strings (so far) )
- the operators also work as words. e.g. (& 1 2) = (and 1 2), (< x 5) = (less x 5) etc
- _some_ pointer functionallity:    
    (deref ptr) ≈ *ptr
    (setbyte ptr index char) ≈ ptr[index] = char
## Operators:
### Binary Operators
    - all binary opeators can have inf. arguments
    - as the conditional operators just return 0 or 1, the bitwise operations can also be used as logical operators e.g. (if (and (< 1 2) (< 4 5)))
    - (+ 1 2 3 4) -> 10
    - (- 1 2 3 4) -> -8
    - (* 1 2 3 4) -> 24
    - (/ 8 2 2) -> 2
    - (& 4 6) -> 4 Bitwise AND
    - (| 4 6) -> 6 Bitwise OR
    - (^ 4 6) -> 2 Bitwise XOR
    - (> 1 2) -> 0
    - (>= 1 2) -> 0
    - (= 1 2) -> 0
    - (!= 1 2) -> 1
    - (< 1 2) -> 1
    - (<= 1 2) -> 1
    - (% 10 3) -> 1, mod x,y
    - (<< 1 10) -> 1024
    - (>> 1024 1) -> 512
### Unary Operators:
    - (~ 20) -> 0xffffffec, 2s Complement negation 
    - (! 20) -> 0xffffffeb, Bitwise NOT
### compound assignment operators
    - all can just be replaced by (set x (<op> x y))
    - (+= x 1)
    - (-= x 1)
    - (*= x 1)
    - (/= x 1)
    - (++ x) 
    - (-- x)

## Keywords and their syntax:
Keywords as of right now:
- let
- set
- defun
- func
- if
- print
- cond
- cons
- car
- cdr
- deref
- while
- setbyte
- malloc
- free
### let:
example:
    (let X 5)
    (let Y 6)
    (let Z (+ X Y))
variables with the same name can simply be overwritten.
a new variables first evaluates the right side.
it creates a new Value localVar and fills it with content like location etc.
And its stored in the variable_table[name] = localVar
it returns the new variable, this way things like (let X (let Y (+ 1 2))) -> X <- Y=3
### set:



