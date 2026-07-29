#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

// The Lexer (a.k.a. "tokenizer" or "scanner") reads the raw source
// string character by character and groups those characters into
// meaningful chunks called Tokens. This is the first stage of any
// interpreter/compiler: turning "3 + 4" into
//   [NUMBER(3), PLUS, NUMBER(4), END_OF_INPUT]
class Lexer {
public:
    explicit Lexer(std::string source);

    // Tokenizes the ENTIRE source in one pass and returns the list.
    std::vector<Token> tokenize();

private:
    std::string src;
    size_t pos = 0;

    char current() const;
    char peekNext() const;
    void advance();
    void skipWhitespace();

    Token readNumber();
    Token readIdentifier();
};

#endif
