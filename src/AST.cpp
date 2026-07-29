#include "AST.h"
#include <stdexcept>
#include <cmath>

double VariableNode::evaluate(Environment& env) const {
    auto it = env.find(name);
    if (it == env.end()) {
        throw std::runtime_error("Undefined variable: '" + name + "'");
    }
    return it->second;
}

double BinaryOpNode::evaluate(Environment& env) const {
    double l = left->evaluate(env);
    double r = right->evaluate(env);

    switch (op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/':
            if (r == 0.0) throw std::runtime_error("Division by zero");
            return l / r;
        case '%':
            if (r == 0.0) throw std::runtime_error("Modulo by zero");
            return std::fmod(l, r);
        default:
            throw std::runtime_error("Unknown binary operator");
    }
}

double UnaryOpNode::evaluate(Environment& env) const {
    double v = operand->evaluate(env);
    return op == '-' ? -v : v;
}

double AssignNode::evaluate(Environment& env) const {
    double v = value->evaluate(env);
    env[name] = v;
    return v;
}
