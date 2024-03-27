#ifndef MKS_ATS_
#define MKS_ATS_

#include <string>
#include <vector>
#include <concepts>
//#include <optional>
#include <variant>
#include <cassert>
#include "mks_lexer.h"

using std::string;
using std::vector;
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

struct Identifier { // : Expression {
    Token token;
    string value;

    Identifier() {}
    ~Identifier() {}

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

    IntegerLiteral() {}
    ~IntegerLiteral() {}

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

    PrefixExpression() {}
    ~PrefixExpression() {}

    string token_literal() const;

    string to_string() const;
};

//  <expression><infix operator><expression>
struct InfixExpression { // : Expression {
    Token token;
    Expression left;
    string optr;        // operator; +, <, >=, ==, !=, etc.
    Expression right;

    InfixExpression() {}
    ~InfixExpression() {}

    string token_literal() const;

    string to_string() const;
};


enum class ExpressionType {
    INVALID = 0,
    IDENT,
    INT_LIT,
    PREFIX,
    INFIX,
};

using ExpressionVariant = std::variant<Identifier, 
                                       IntegerLiteral, 
                                       PrefixExpression, 
                                       InfixExpression>;

struct Expression {
    ExpressionType type;
    ExpressionVariant variant;

    Expression() {
        type = ExpressionType::INVALID;
    }

    //Expression(ExpressionType type, ExpressionVariant variant) : 
    //    type(type), 
    //    variant(variant) 
    //{}

    //Expression(Identifier identifier) {
    //    type = ExpressionType::IDENT;
    //    variant.emplace<Identifier>(identifier);
    //}

    template <typename V>
    Expression(ExpressionType type, V v) : type(type) {
        assert(std::holds_alternative<V>(v));
        variant.emplace<V>(v);
    }

    template <typename V>
    Expression(const Expression &other) {
        type = other.type;
        assert(std::holds_alternative<V>(other.variant));
        variant.emplace<V>(other.variant);
    }

    template <typename V>
    Expression &operator=(const Expression &other) {
        type = other.type;
        assert(std::holds_alternative<V>(other.variant));
        variant.emplace<V>(other.variant);
    }

    ~Expression() {}
};

string ExpressionToString(const Expression &expression);

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
    }

    ~LetStatement() = default;

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
    }

    ~ReturnStatement() = default;

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

    ExpressionStatement() = default;
    ~ExpressionStatement() = default;

    string token_literal() const;

    string to_string() const;
};

enum class StatementType {
    INVALID = 0,
    LET,
    RETURN,
    EXPRESSION
};

using StatementVariant = std::variant<LetStatement, 
                                      ReturnStatement,
                                      ExpressionStatement>;

struct Statement {
    StatementType type;
    StatementVariant variant;

    Statement() {
        type = StatementType::INVALID;
        //variant = {};
    }

    //Statement(StatementType type, StatementVariant variant) : 
    //    type(type), 
    //    variant(variant) 
    //{}

    template <typename V>
    Statement(StatementType type, V v) : type(type) {
        assert(std::holds_alternative<V>(v));
        variant.emplace<V>(v);
    }

    ~Statement() = default;
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
