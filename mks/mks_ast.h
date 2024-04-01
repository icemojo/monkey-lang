#ifndef MKS_ATS_
#define MKS_ATS_

#include <string>
#include <vector>
#include <concepts>
//#include <optional>
#include <variant>
#include <memory>
#include <cassert>
#include "mks_lexer.h"

using std::string;
using std::vector;
using std::unique_ptr, std::make_unique;
//using std::optional, std::make_optional;
//using std::variant, std::get_if;


//struct Expression {
//    Token token;
//
//    virtual string token_literal() const;
//
//    virtual string to_string() const;
//};
//
//std::ostream &
//operator<<(std::ostream &out, const Expression1 &expression);

enum class ExpressionType {
    INVALID = 0,
    IDENT,
    INT_LIT,
    PREFIX,
    INFIX,
};

//using ExpressionVariant = std::variant<Identifier, 
//                                       IntegerLiteral, 
//                                       PrefixExpression, 
//                                       InfixExpression>;

// NOTE(yemon): Move semantics have to be properly applied for 
// all possible `Expression.variant`, since they'll be handled 
// by a `unique_ptr<void *>`

struct Expression {
    ExpressionType type;
    //ExpressionVariant variant;
    unique_ptr<void *> variant;

    Expression() {
        type = ExpressionType::INVALID;
        variant = nullptr;
    }

    Expression(ExpressionType type) : type(type) {
        variant = nullptr;
    }

    Expression(const Expression &other) {
        type = other.type;
        variant = make_unique<void *>(*(other.variant));
    }
    Expression &operator=(const Expression &other) {
        type = other.type;
        variant = make_unique<void *>(*(other.variant));
    }

    Expression(Expression &&other) noexcept {
        type = other.type;
        variant = std::move(other.variant);
    }
    Expression &operator=(Expression &&other) noexcept {
        type = other.type;
        variant = std::move(other.variant);
    }

    ~Expression() {
        if (variant != nullptr) {
            variant.release();
        }
        assert(variant == nullptr);
    }

    //Expression(ExpressionType type, ExpressionVariant variant) : 
    //    type(type), 
    //    variant(variant) 
    //{}

    //Expression(Identifier identifier) {
    //    type = ExpressionType::IDENT;
    //    variant.emplace<Identifier>(identifier);
    //}

    //template <typename V>
    //Expression(ExpressionType type, V v) : type(type) {
    //    assert(std::holds_alternative<V>(v));
    //    variant.emplace<V>(v);
    //}

    //template <typename V>
    //Expression(const Expression &other) {
    //    type = other.type;
    //    assert(std::holds_alternative<V>(other.variant));
    //    variant.emplace<V>(other.variant);
    //}

    //template <typename V>
    //Expression &operator=(const Expression &other) {
    //    type = other.type;
    //    assert(std::holds_alternative<V>(other.variant));
    //    variant.emplace<V>(other.variant);
    //}
};

string ExpressionToString(const Expression &expression);

struct Identifier { // : Expression {
    Token token;
    string value;

    //Identifier() {}
    //~Identifier() {}

    //Identifier(const Identifier &rhs) = default;
    //Identifier &operator=(const Identifier &rhs) = default;

    //Identifier(const Identifier &&rhs) = default;
    //Identifier &operator=(const Identifier &&rhs) = default;

    string token_literal() const;

    string to_string() const;
};

//struct IdentifierResult {
//    bool success{ false };
//    Identifier identifier;
//};

//  <integer literal>
struct IntegerLiteral { // : Expression {
    Token token;
    int64_t value;

    //IntegerLiteral() {}
    //~IntegerLiteral() {}

    //IntegerLiteral(const IntegerLiteral &rhs) = default;
    //IntegerLiteral &operator=(const IntegerLiteral &rhs) = default;

    //IntegerLiteral(const IntegerLiteral &&rhs) = default;
    //IntegerLiteral &operator=(const IntegerLiteral &&rhs) = default;

    string token_literal() const;

    string to_string() const;
};

//struct IntegerLiteralResult {
//    bool success;
//    IntegerLiteral int_literal;
//};

//  <prefix operator><expression>;
struct PrefixExpression { // : Expression {
    Token token;
    string optr;        // operator; like !, -, etc.
    Expression right;

    //PrefixExpression() {}
    //~PrefixExpression() {}

    //PrefixExpression(const PrefixExpression &other) = default;
    //PrefixExpression &operator=(const PrefixExpression &other) = default;

    //PrefixExpression(const PrefixExpression &&other) = default;
    //PrefixExpression &operator=(const PrefixExpression &&other) = default;

    string token_literal() const;

    string to_string() const;
};

//  <expression><infix operator><expression>
struct InfixExpression { // : Expression {
    Token token;
    Expression left;
    string optr;        // operator; +, <, >=, ==, !=, etc.
    Expression right;

    //InfixExpression() {}
    //~InfixExpression() {}

    //InfixExpression(const InfixExpression &other) = default;
    //InfixExpression &operator=(const InfixExpression &other) = default;

    //InfixExpression(const InfixExpression &&other) = default;
    //InfixExpression &operator=(const InfixExpression &&other) = default;

    string token_literal() const;

    string to_string() const;
};

//------------------------------------------------------------------------------

//  let <identifier> = <expression>;
struct LetStatement {
    Token token;
    Identifier name;
    //optional<Expression> value;
    Expression value;

    LetStatement() {}

    LetStatement(Identifier identifier) {
        token = TokenNew(TokenType::LET, "let");
        name = identifier;

        //value = {};
        value = Expression(ExpressionType::INVALID);
    }

    //~LetStatement() = default;

    //LetStatement(const LetStatement &rhs) = default;
    //LetStatement &operator=(const LetStatement &rhs) = default;

    //LetStatement(const LetStatement &&rhs) = default;
    //LetStatement &operator=(const LetStatement &&rhs) = default;

    string token_literal() const;

    string to_string() const;
};

//  return <expression>;
struct ReturnStatement {
    Token token;
    //optional<Expression> value;
    Expression value;

    ReturnStatement() {
        token = TokenNew(TokenType::RETURN, "return");

        //value = {};
        value = Expression(ExpressionType::INVALID);
    }

    //~ReturnStatement() = default;

    //ReturnStatement(const ReturnStatement &rhs) = default;
    //ReturnStatement &operator=(const ReturnStatement &rhs) = default;

    //ReturnStatement(const ReturnStatement &&rhs) = default;
    //ReturnStatement &operator=(const ReturnStatement &&rhs) = default;

    string token_literal() const;

    string to_string() const;
};

// NOTE(yemon): Simply only because Monkey lang will allow this kind of shennanigan,
//  let x = 10;         let <identifier> = <int-literal>;
//  let c = a + 2;      let <identifier> = <infix-expression>;
//  ++b;                <prefix-expression-statement>;
//  x + 32;             <infix-expression-statement>;
//  return 12;          return <int-literal>;
//  return a + b;       return <infix-expression>;
// All of those statements above are completely legal, and thus 
// expressions are dubbed as expression statements.
struct ExpressionStatement {
    Token token;
    //optional<Expression> expression;
    Expression expression;

    //ExpressionStatement() = default;
    //~ExpressionStatement() = default;

    //ExpressionStatement(const ExpressionStatement &rhs) = default;
    //ExpressionStatement &operator=(const ExpressionStatement &rhs) = default;

    //ExpressionStatement(const ExpressionStatement &&rhs) = default;
    //ExpressionStatement &operator=(const ExpressionStatement &&rhs) = default;

    string token_literal() const;

    string to_string() const;
};

enum class StatementType {
    INVALID = 0,
    LET,
    RETURN,
    EXPRESSION
};

//using StatementVariant = std::variant<LetStatement, 
//                                      ReturnStatement,
//                                      ExpressionStatement>;

// NOTE(yemon): Move semantics have to be properly applied for 
// all possible `Statement.variant`, since they'll be handled 
// by a `unique_ptr<void *>`

struct Statement {
    StatementType type;
    //StatementVariant variant;
    unique_ptr<void *> variant;

    Statement() {
        type = StatementType::INVALID;
        //variant = {};
        variant = nullptr;
    }

    Statement(StatementType type) : type(type) {
        variant = nullptr;
    }

    Statement(const Statement &other) {
        type = other.type;
        variant = make_unique<void *>(*(other.variant));
    }
    Statement &operator=(const Statement &other) {
        type = other.type;
        variant = make_unique<void *>(*(other.variant));
    }

    Statement(Statement &&other) noexcept {
        type = other.type;
        variant = std::move(other.variant);
    }
    Statement &operator=(Statement &&other) noexcept {
        type = other.type;
        variant = std::move(other.variant);
    }

    ~Statement() {
        if (variant != nullptr) {
            variant.release();
        }
        assert(variant == nullptr);
    }
};

string StatementToString(const Statement &statement);

struct StatementResult {
    bool success{ false };
    Statement statement;

    StatementResult() : success(false) {}
};

//template <typename T>
//struct StatementResult {
//    bool success{ false };
//    T statement;
    //T *statement_ptr{ nullptr };

    //static_assert(std::derived_from<T, Statement>, "Type T of StatementResult<T> must be derived from Statement.");
//};


#endif  // MKS_ATS_
