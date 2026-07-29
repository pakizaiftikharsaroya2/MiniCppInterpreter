#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// Every kind of "word" our language understands.
// The lexer's whole job is to turn raw text into a stream of these.
enum class TokenType {
    NUMBER,       // e.g. 42, 3.14
    IDENTIFIER,   // e.g. x, total, price
    PLUS,         // +
    MINUS,        // -
    STAR,         // *
    SLASH,        // /
    PERCENT,      // %
    ASSIGN,       // =
    LPAREN,       // (
    RPAREN,       // )
    SEMICOLON,    // ;
    END_OF_INPUT, // marks the end of the token stream
    INVALID       // anything we don't recognize
};

struct Token {
    TokenType type;
    std::string text;   // the raw text this token came from
    double numberValue;  // only meaningful if type == NUMBER

    Token(TokenType t, std::string txt, double val = 0.0)
        : type(t), text(std::move(txt)), numberValue(val) {}
};

#endif
