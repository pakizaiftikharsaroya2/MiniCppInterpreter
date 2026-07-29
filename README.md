# Mini C++ Expression Interpreter

A small but real interpreter for arithmetic expressions and variable
assignment, written from scratch in C++17 — no parser-generator
libraries, no external dependencies. Every stage (lexing, parsing,
evaluating) is hand-written to show the actual mechanics behind how
interpreters and compilers work.

## What it can do

```
>> 2 + 3 * 4
=> 14

>> (2 + 3) * 4
=> 20

>> radius = 7
=> 7

>> area = 3.14159 * radius * radius
=> 153.938

>> -radius + 2
=> -5

>> 10 % 3
=> 1

>> 5 / 0
Error: Division by zero
```

Supports: `+  -  *  /  %`, parentheses, unary minus/plus, variable
assignment (`x = 5`), multiple statements per session (variables
persist), and proper error messages for syntax errors, division by
zero, and undefined variables.

## Why this project (not just "another calculator")

A calculator app usually just calls a math library. This project
implements the actual **pipeline every real compiler/interpreter uses**:

```
source text  -->  [ Lexer ]  -->  tokens  -->  [ Parser ]  -->  AST  -->  [ Evaluator ]  -->  result
 "2 + 3*4"                     NUMBER(2)                        tree                          14
                                PLUS                          (see below)
                                NUMBER(3)
                                STAR
                                NUMBER(4)
```

1. **Lexer** (`src/Lexer.cpp`) — reads raw characters and groups them
   into meaningful tokens (numbers, operators, identifiers).
2. **Parser** (`src/Parser.cpp`) — a hand-written **recursive descent
   parser** that turns the flat token list into a tree (AST) whose
   *shape* encodes operator precedence, so `2 + 3 * 4` naturally nests
   the multiplication deeper than the addition — no special-casing
   needed later.
3. **AST + Evaluator** (`src/AST.cpp`) — walks the tree recursively
   ("tree-walking interpretation") to compute the final value, using a
   symbol table (`Environment`, an `unordered_map<string, double>`) to
   store variables.

This demonstrates recursion, stacks (the call stack *is* the parsing
stack here), tree data structures, and grammar design — core CS
fundamentals, not just syntax.

## Grammar

```
program    := statement (';' statement)* ';'?
statement  := assignment | expression
assignment := IDENTIFIER '=' expression
expression := term (('+' | '-') term)*
term       := unary (('*' | '/' | '%') unary)*
unary      := ('-' | '+')? primary
primary    := NUMBER | IDENTIFIER | '(' expression ')'
```

## Project structure

```
MiniCppInterpreter/
├── include/
│   ├── Token.h       # token type definitions
│   ├── Lexer.h
│   ├── AST.h         # tree node definitions
│   └── Parser.h
├── src/
│   ├── Lexer.cpp
│   ├── AST.cpp
│   ├── Parser.cpp
│   └── main.cpp       # REPL + file-execution entry point
├── examples/
│   └── sample.mce      # example script
├── Makefile
└── README.md
```

## Building and running

Requires a C++17 compiler (g++ or clang++).

```bash
make              # builds the "interpreter" binary
./interpreter     # starts an interactive REPL
```

Or run a script file directly:
```bash
./interpreter examples/sample.mce
```

Exit the REPL by typing `exit` or `quit`.

## Possible extensions

Ideas for pushing this further (good talking points in interviews,
even if unimplemented):
- Control flow: `if`/`else`, `while` loops
- Functions with parameters and a call stack
- More types: strings, booleans
- Better error messages with line/column numbers
- A bytecode compiler + virtual machine instead of tree-walking
  (this is the natural "next step up" from an AST interpreter,
  similar to how CPython or the JVM work)
