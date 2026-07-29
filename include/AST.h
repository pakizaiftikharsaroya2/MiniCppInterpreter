#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <unordered_map>

// An AST (Abstract Syntax Tree) is how a parser represents the
// STRUCTURE of an expression, not just its raw text.
//
// For example, "2 + 3 * 4" becomes this tree (multiplication binds
// tighter, so it's nested deeper):
//
//   +
//   |-- 2
//   |-- *
//        |-- 3
//        |-- 4
//
// Evaluating the tree bottom-up (recursively) automatically respects
// operator precedence, without the evaluator needing to know any
// precedence rules itself — the PARSER already encoded that into the
// tree's shape. This is the core idea behind how real compilers work.

using Environment = std::unordered_map<std::string, double>;

// Base class for every kind of node in the tree.
struct Node {
    virtual ~Node() = default;
    virtual double evaluate(Environment& env) const = 0;
};

using NodePtr = std::unique_ptr<Node>;

// A literal number, e.g. `42`
struct NumberNode : Node {
    double value;
    explicit NumberNode(double v) : value(v) {}
    double evaluate(Environment&) const override { return value; }
};

// A variable reference, e.g. `x`
struct VariableNode : Node {
    std::string name;
    explicit VariableNode(std::string n) : name(std::move(n)) {}
    double evaluate(Environment& env) const override;
};

// A binary operation, e.g. `left + right`
struct BinaryOpNode : Node {
    char op; // '+', '-', '*', '/', '%'
    NodePtr left;
    NodePtr right;
    BinaryOpNode(char o, NodePtr l, NodePtr r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
    double evaluate(Environment& env) const override;
};

// A unary operation, e.g. `-x`
struct UnaryOpNode : Node {
    char op; // '-' or '+'
    NodePtr operand;
    UnaryOpNode(char o, NodePtr n) : op(o), operand(std::move(n)) {}
    double evaluate(Environment& env) const override;
};

// An assignment, e.g. `x = 5 + 2`
struct AssignNode : Node {
    std::string name;
    NodePtr value;
    AssignNode(std::string n, NodePtr v) : name(std::move(n)), value(std::move(v)) {}
    double evaluate(Environment& env) const override;
};

#endif
