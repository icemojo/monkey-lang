#ifndef MKS_PARSER_
#define MKS_PARSER_

#include <memory>
#include "mks_lexer.h"
#include "mks_ast.h"

using std::unique_ptr;


struct Program {
    vector<Statement> statements{};

    string token_literal() const;
};

//------------------------------------------------------------------------------

struct Parser;

Parser *
ParserNew(unique_ptr<Lexer> &lexer);

struct ParserIter {
    Parser *parser{};

    [[nodiscard]] bool valid() const;

    void next();

    Parser * operator*() const;
    
    Parser * operator->() const;
};

struct Parser {
    unique_ptr<Lexer> lexer;
    Token cur_token = {};
    Token peek_token = {};

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

Program *
ParseProgram(Parser *parser);

StatementResult<Statement>
ParseStatement(Parser *parser);

StatementResult<LetStatement>
ParseLetStatement(Parser *parser);

StatementResult<ReturnStatement>
ParseReturnStatement(Parser *parser);

Expression
ParseExpression(Parser *parser);

#endif  // MKS_PARSERS_
