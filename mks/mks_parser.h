#ifndef MKS_PARSER_
#define MKS_PARSER_

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <unordered_map>
#include "mks_lexer.h"
#include "mks_ast.h"

using std::string;
using std::vector;
using std::unique_ptr, std::move;
using std::make_optional, std::nullopt;


struct Program {
    vector<Statement> statements{};

    string token_literal() const;

    string to_string() const;
};

//------------------------------------------------------------------------------

struct Parser;

unique_ptr<Parser>
ParserNew(unique_ptr<Lexer> lexer);

struct ParserIter {
    Parser *parser{};

    [[nodiscard]] bool valid() const;

    void next();
};

struct Parser {
    unique_ptr<Lexer> lexer;
    Token cur_token = {};
    Token peek_token = {};

    vector<string> errors = {};

    // NOTE(yemon): `next_token()` sort of "stream up" the tokens from the lexer.
    // IOW, calling this once on an empty parser will assign the `peek_token` first,
    // while leaving the `cur_token` as default/empty.
    // Calling it again will advance the `peek_token` into `cur_token`, and then
    // pull up the following token into `peek_token`, etc etc.
    void next_token();

    bool is_cur_token(const TokenType type) const;

    bool is_peek_token(const TokenType type) const;

    bool expect_peek(const TokenType type);
};

//------------------------------------------------------------------------------

Program *
ParseProgram(Parser *parser);

StatementResult<Statement>
ParseStatement(Parser *parser);

StatementResult<LetStatement>
ParseLetStatement(Parser *parser);

StatementResult<ReturnStatement>
ParseReturnStatement(Parser *parser);

enum class Prec {
    LOWEST = 0,
    EQUALS,         // ==, !=
    LESS_GREATER,   // <, >
    SUM,            // +, -
    PRODUCT,        // *, /
    PREFIX,         // -x or !x
    CALL,           // doSomething(x)
};

inline const unordered_map<TokenType, Prec>
PRECEDENCES = {
    {TokenType::EQ, Prec::EQUALS},
    {TokenType::NOT_EQ, Prec::EQUALS},
    {TokenType::LT, Prec::LESS_GREATER},
    {TokenType::GT, Prec::LESS_GREATER},
    {TokenType::PLUS, Prec::SUM},
    {TokenType::MINUS, Prec::SUM},
    {TokenType::ASTRISK, Prec::PRODUCT},
    {TokenType::SLASH, Prec::PRODUCT},
};

Prec
CheckPrecedence(const TokenType &token_type);

StatementResult<ExpressionStatement>
ParseExpressionStatement(Parser *parser);

ExpressionStatement
ParseExpression(Parser *parser, const Prec prec);

Expression
ParseIdentifier(Parser *parser);

IntegerLiteral
ParseIntegerLiteral(Parser *parser);

PrefixExpression
ParsePrefixExpression(Parser *parser);

InfixExpression
ParseInfixExpression(Parser *parser, const Expression &left);

#endif  // MKS_PARSERS_
