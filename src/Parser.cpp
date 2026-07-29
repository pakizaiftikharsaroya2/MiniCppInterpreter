#include "Parser.h"
#include <stdexcept>

Parser::Parser(std::vector<Token> toks) : tokens(std::move(toks)) {}

const Token& Parser::current() const { return tokens[pos]; }

const Token& Parser::peekNext() const {
    if (pos + 1 < tokens.size()) return tokens[pos + 1];
    return tokens.back(); // END_OF_INPUT
}

bool Parser::check(TokenType type) const { return current().type == type; }

Token Parser::advance() {
    Token t = current();
    if (pos < tokens.size() - 1) pos++;
    return t;
}

Token Parser::expect(TokenType type, const std::string& errorMessage) {
    if (!check(type)) {
        throw std::runtime_error("Parse error: " + errorMessage + " (got '" + current().text + "')");
    }
    return advance();
}

std::vector<NodePtr> Parser::parseProgram() {
    std::vector<NodePtr> statements;

    while (!check(TokenType::END_OF_INPUT)) {
        statements.push_back(parseStatement());

        if (check(TokenType::SEMICOLON)) {
            advance(); // consume ';'
        } else if (!check(TokenType::END_OF_INPUT)) {
            throw std::runtime_error("Parse error: expected ';' between statements (got '" + current().text + "')");
        }
    }

    return statements;
}

// statement := assignment | expression
NodePtr Parser::parseStatement() {
    // Lookahead: IDENTIFIER followed by '=' means this is an assignment.
    if (check(TokenType::IDENTIFIER) && peekNext().type == TokenType::ASSIGN) {
        std::string name = advance().text; // consume identifier
        advance();                         // consume '='
        NodePtr value = parseExpression();
        return std::make_unique<AssignNode>(name, std::move(value));
    }
    return parseExpression();
}

// expression := term (('+' | '-') term)*
NodePtr Parser::parseExpression() {
    NodePtr left = parseTerm();

    while (check(TokenType::PLUS) || check(TokenType::MINUS)) {
        char op = advance().text[0];
        NodePtr right = parseTerm();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

// term := unary (('*' | '/' | '%') unary)*
NodePtr Parser::parseTerm() {
    NodePtr left = parseUnary();

    while (check(TokenType::STAR) || check(TokenType::SLASH) || check(TokenType::PERCENT)) {
        char op = advance().text[0];
        NodePtr right = parseUnary();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

// unary := ('-' | '+')? primary
NodePtr Parser::parseUnary() {
    if (check(TokenType::MINUS) || check(TokenType::PLUS)) {
        char op = advance().text[0];
        NodePtr operand = parseUnary(); // allows chained unary like --5
        return std::make_unique<UnaryOpNode>(op, std::move(operand));
    }
    return parsePrimary();
}

// primary := NUMBER | IDENTIFIER | '(' expression ')'
NodePtr Parser::parsePrimary() {
    if (check(TokenType::NUMBER)) {
        double v = advance().numberValue;
        return std::make_unique<NumberNode>(v);
    }
    if (check(TokenType::IDENTIFIER)) {
        std::string name = advance().text;
        return std::make_unique<VariableNode>(name);
    }
    if (check(TokenType::LPAREN)) {
        advance(); // consume '('
        NodePtr inner = parseExpression();
        expect(TokenType::RPAREN, "expected ')'");
        return inner;
    }
    throw std::runtime_error("Parse error: unexpected token '" + current().text + "'");
}
