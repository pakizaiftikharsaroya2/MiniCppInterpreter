#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(std::string source) : src(std::move(source)) {}

char Lexer::current() const {
    if (pos >= src.size()) return '\0';
    return src[pos];
}

char Lexer::peekNext() const {
    if (pos + 1 >= src.size()) return '\0';
    return src[pos + 1];
}

void Lexer::advance() { pos++; }

void Lexer::skipWhitespace() {
    while (current() == ' ' || current() == '\t' || current() == '\n' || current() == '\r') {
        advance();
    }
}

Token Lexer::readNumber() {
    size_t start = pos;
    bool sawDot = false;
    while (isdigit(current()) || (current() == '.' && !sawDot)) {
        if (current() == '.') sawDot = true;
        advance();
    }
    std::string text = src.substr(start, pos - start);
    return Token(TokenType::NUMBER, text, std::stod(text));
}

Token Lexer::readIdentifier() {
    size_t start = pos;
    while (isalnum(current()) || current() == '_') {
        advance();
    }
    std::string text = src.substr(start, pos - start);
    return Token(TokenType::IDENTIFIER, text);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhitespace();
        char c = current();

        if (c == '\0') {
            tokens.emplace_back(TokenType::END_OF_INPUT, "");
            break;
        }

        if (isdigit(c)) {
            tokens.push_back(readNumber());
            continue;
        }

        if (isalpha(c) || c == '_') {
            tokens.push_back(readIdentifier());
            continue;
        }

        switch (c) {
            case '+': tokens.emplace_back(TokenType::PLUS, "+"); advance(); break;
            case '-': tokens.emplace_back(TokenType::MINUS, "-"); advance(); break;
            case '*': tokens.emplace_back(TokenType::STAR, "*"); advance(); break;
            case '/': tokens.emplace_back(TokenType::SLASH, "/"); advance(); break;
            case '%': tokens.emplace_back(TokenType::PERCENT, "%"); advance(); break;
            case '=': tokens.emplace_back(TokenType::ASSIGN, "="); advance(); break;
            case '(': tokens.emplace_back(TokenType::LPAREN, "("); advance(); break;
            case ')': tokens.emplace_back(TokenType::RPAREN, ")"); advance(); break;
            case ';': tokens.emplace_back(TokenType::SEMICOLON, ";"); advance(); break;
            default:
                throw std::runtime_error(std::string("Unexpected character: '") + c + "'");
        }
    }

    return tokens;
}
