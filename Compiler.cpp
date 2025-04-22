// noiria_compiler_core.cpp
// Core implementation for the Noiria language

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cctype>

//--------------------------------------
// TOKEN TYPES
//--------------------------------------

enum class TokenType {
    Identifier, Number, Keyword,
    Plus, Minus, Asterisk, Slash,
    Equals, Semicolon,
    LParen, RParen,
    EOFToken,
};

struct Token {
    TokenType type;
    std::string value;
};

//--------------------------------------
// LEXER
//--------------------------------------

class Lexer {
    std::string src;
    size_t pos = 0;
    char current() { return pos < src.size() ? src[pos] : '\0'; }
    void advance() { if (pos < src.size()) ++pos; }

public:
    Lexer(const std::string& input) : src(input) {}

    Token nextToken() {
        while (std::isspace(current())) advance();
        if (std::isalpha(current())) {
            std::string id;
            while (std::isalnum(current())) { id += current(); advance(); }
            if (id == "let" || id == "print") return {TokenType::Keyword, id};
            return {TokenType::Identifier, id};
        }
        if (std::isdigit(current())) {
            std::string num;
            while (std::isdigit(current())) { num += current(); advance(); }
            return {TokenType::Number, num};
        }

        switch (current()) {
            case '+': advance(); return {TokenType::Plus, "+"};
            case '-': advance(); return {TokenType::Minus, "-"};
            case '*': advance(); return {TokenType::Asterisk, "*"};
            case '/': advance(); return {TokenType::Slash, "/"};
            case '=': advance(); return {TokenType::Equals, "="};
            case ';': advance(); return {TokenType::Semicolon, ";"};
            case '(': advance(); return {TokenType::LParen, "("};
            case ')': advance(); return {TokenType::RParen, ")"};
            case '\0': return {TokenType::EOFToken, ""};
            default: advance(); return {TokenType::EOFToken, ""};
        }
    }
};

//--------------------------------------
// AST NODES
//--------------------------------------

struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    int value;
    NumberExpr(int val) : value(val) {}
};

struct VariableExpr : Expr {
    std::string name;
    VariableExpr(const std::string& n) : name(n) {}
};

struct BinaryExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> left, right;
    BinaryExpr(std::string o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

struct Statement {
    virtual ~Statement() = default;
};

struct PrintStatement : Statement {
    std::unique_ptr<Expr> expr;
    PrintStatement(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
};

struct LetStatement : Statement {
    std::string name;
    std::unique_ptr<Expr> expr;
    LetStatement(std::string n, std::unique_ptr<Expr> e) : name(n), expr(std::move(e)) {}
};

//--------------------------------------
// PARSER
//--------------------------------------

class Parser {
    Lexer& lexer;
    Token current;

    void consume() { current = lexer.nextToken(); }

    bool match(TokenType t) {
        if (current.type == t) { consume(); return true; }
        return false;
    }

    std::unique_ptr<Expr> parseExpression() {
        auto left = parsePrimary();
        while (current.type == TokenType::Plus || current.type == TokenType::Minus) {
            std::string op = current.value;
            consume();
            auto right = parsePrimary();
            left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
        }
        return left;
    }

    std::unique_ptr<Expr> parsePrimary() {
        if (current.type == TokenType::Number) {
            int value = std::stoi(current.value);
            consume();
            return std::make_unique<NumberExpr>(value);
        } else if (current.type == TokenType::Identifier) {
            std::string name = current.value;
            consume();
            return std::make_unique<VariableExpr>(name);
        } else {
            return nullptr;
        }
    }

public:
    Parser(Lexer& l) : lexer(l) { consume(); }

    std::unique_ptr<Statement> parseStatement() {
        if (current.type == TokenType::Keyword && current.value == "let") {
            consume();
            std::string name = current.value;
            consume();
            match(TokenType::Equals);
            auto expr = parseExpression();
            match(TokenType::Semicolon);
            return std::make_unique<LetStatement>(name, std::move(expr));
        } else if (current.type == TokenType::Keyword && current.value == "print") {
            consume();
            auto expr = parseExpression();
            match(TokenType::Semicolon);
            return std::make_unique<PrintStatement>(std::move(expr));
        }
        return nullptr;
    }
};

//--------------------------------------
// OPCODE EMITTER
//--------------------------------------

class CodeGenerator {
    std::unordered_map<std::string, int> symbolTable;
    int registerCounter = 0;

public:
    void generate(const Statement* stmt) {
        if (auto let = dynamic_cast<const LetStatement*>(stmt)) {
            std::cout << "LOAD_CONST " << evaluate(let->expr.get()) << " -> R" << registerCounter << "\n";
            symbolTable[let->name] = registerCounter++;
        } else if (auto print = dynamic_cast<const PrintStatement*>(stmt)) {
            std::cout << "PRINT " << evaluate(print->expr.get()) << "\n";
        }
    }

    int evaluate(const Expr* expr) {
        if (auto num = dynamic_cast<const NumberExpr*>(expr)) {
            return num->value;
        } else if (auto var = dynamic_cast<const VariableExpr*>(expr)) {
            return symbolTable[var->name];
        } else if (auto bin = dynamic_cast<const BinaryExpr*>(expr)) {
            int l = evaluate(bin->left.get());
            int r = evaluate(bin->right.get());
            std::cout << bin->op << " R" << l << ", R" << r << " -> R" << registerCounter << "\n";
            return registerCounter++;
        }
        return -1;
    }
};

//--------------------------------------
// MAIN (DRIVER)
//--------------------------------------

int main() {
    std::string input = "let x = 10 + 5; print x;";
    Lexer lexer(input);
    Parser parser(lexer);
    CodeGenerator codegen;

    while (true) {
        auto stmt = parser.parseStatement();
        if (!stmt) break;
        codegen.generate(stmt.get());
    }

    return 0;
}
