#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.h"
#include "AST.h"

// The Parser takes the flat list of Tokens from the Lexer and builds
// a tree (the AST) that reflects operator precedence and grouping.
//
// This is a RECURSIVE DESCENT parser — the classic, most intuitive way
// to hand-write a parser. Each grammar rule becomes one function, and
// those functions call each other recursively (using the call stack
// itself as an implicit stack, mirroring how you'd use an explicit
// stack in a shunting-yard algorithm).
//
// Grammar (highest-level rule first, EBNF-ish):
//
//   program    := statement (';' statement)* ';'?
//   statement  := assignment | expression
//   assignment := IDENTIFIER '=' expression
//   expression := term (('+' | '-') term)*
//   term       := unary (('*' | '/' | '%') unary)*
//   unary      := ('-' | '+')? primary
//   primary    := NUMBER | IDENTIFIER | '(' expression ')'
//
// Lower rules bind TIGHTER — that's what gives us correct precedence
// (multiplication happens before addition) without any special-casing
// in the evaluator.
class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    // Parses the whole token stream into a list of top-level statements.
    std::vector<NodePtr> parseProgram();

private:
    std::vector<Token> tokens;
    size_t pos = 0;

    const Token& current() const;
    const Token& peekNext() const;
    bool check(TokenType type) const;
    Token advance();
    Token expect(TokenType type, const std::string& errorMessage);

    NodePtr parseStatement();
    NodePtr parseExpression();
    NodePtr parseTerm();
    NodePtr parseUnary();
    NodePtr parsePrimary();
};

#endif
