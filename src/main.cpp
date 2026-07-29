#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Lexer.h"
#include "Parser.h"
#include "AST.h"

// Runs one full program string (possibly many ';'-separated statements)
// against a shared Environment (so variables persist across lines in
// REPL mode, and across the whole file in file mode).
static void runProgram(const std::string& source, Environment& env, bool printResults) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::vector<NodePtr> statements = parser.parseProgram();

    for (const auto& stmt : statements) {
        double result = stmt->evaluate(env);
        if (printResults) {
            std::cout << "=> " << result << "\n";
        }
    }
}

static void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file: " << path << "\n";
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    Environment env;
    try {
        runProgram(buffer.str(), env, /*printResults=*/true);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

static void runRepl() {
    std::cout << "Mini C++ Expression Interpreter\n";
    std::cout << "Type expressions like: 2 + 3 * 4\n";
    std::cout << "Assign variables like:  x = 10\n";
    std::cout << "Type 'exit' to quit.\n\n";

    Environment env;
    std::string line;

    while (true) {
        std::cout << ">> ";
        if (!std::getline(std::cin, line)) break;

        if (line == "exit" || line == "quit") break;
        if (line.empty()) continue;

        // In the REPL, treat each line as one statement — append ';'
        // so it satisfies the same grammar as file mode.
        // In the REPL, treat each line as one statement — append ';'
        // only if the user didn't already type one, so the grammar
        // (which expects statements to be ';'-terminated) is satisfied
        // either way.
        std::string toRun = line;
        size_t lastNonSpace = toRun.find_last_not_of(" \t\r");
        bool alreadyTerminated = (lastNonSpace != std::string::npos && toRun[lastNonSpace] == ';');
        if (!alreadyTerminated) toRun += ";";

        try {
            runProgram(toRun, env, /*printResults=*/true);
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    std::cout << "Goodbye!\n";
}

int main(int argc, char** argv) {
    if (argc >= 2) {
        runFile(argv[1]);
    } else {
        runRepl();
    }
    return 0;
}
