#include <iostream>
#include "mks_parser.h"


string
Program::token_literal() const
{
    if (statements.empty()) {
        return "";
    }
    else {
        //return statements.at(0).token_literal();
        return "";
    }
}

//------------------------------------------------------------------------------

Parser *
ParserNew(unique_ptr<Lexer> &lexer)
{
    Parser *parser = new Parser{
        .lexer = std::move(lexer),
    };

    // Read two tokens, so that current token and peek tokens are set
    parser->next_token();
    parser->next_token();

    return parser;
}

[[nodiscard]] bool 
ParserIter::valid() const
{
    if (parser == nullptr) return false;
    return parser->cur_token.type != TokenType::T_EOF;
}

void 
ParserIter::next()
{
    parser->next_token();
}

Parser *
ParserIter::operator*() const
{
    return parser;
}

Parser *
ParserIter::operator->() const
{
    return parser;
}

void
Parser::next_token()
{
    cur_token = peek_token;
    peek_token = lexer->next_token();
}

bool
Parser::is_cur_token(const TokenType type) const
{
    return cur_token.type == type;
}

bool 
Parser::is_peek_token(const TokenType type) const
{
    return peek_token.type == type;
}

bool 
Parser::expect_peek(const TokenType type)
{
    if (is_peek_token(type)) {
        next_token();
        return true;
    }
    else {
        return false;
    }
}

Program *
ParseProgram(Parser *parser)
{
    Program *program = new Program{};
    program->statements = {};

    ParserIter it = ParserIter{ parser };
    while (it.valid()) {
        if (StatementResult<Statement> result = ParseStatement(*it);
            result.success) {
            program->statements.push_back(*result.statement_ptr);
            delete result.statement_ptr;
        }

        it.next();
    }

    return program;
}

StatementResult<Statement>
ParseStatement(Parser *parser)
{
    StatementResult<Statement> result = {
        .success = false,
        .statement_ptr = nullptr,
    };

    if (parser == nullptr) {
        return result;
    }

    switch (parser->cur_token.type) {
    case TokenType::LET:
        if (StatementResult<LetStatement> parse_result = ParseLetStatement(parser);
            parse_result.success) {
            result.success = true;
            result.statement_ptr = parse_result.statement_ptr;
            return result;
        }
        break;

    case TokenType::RETURN:
        if (StatementResult<ReturnStatement> parse_result = ParseReturnStatement(parser);
            parse_result.success) {
            result.success = true;
            result.statement_ptr = parse_result.statement_ptr;
            return result;
        }
        break;

    default:
        return result;
    }

    return result;
}

StatementResult<LetStatement>
ParseLetStatement(Parser *parser)
{
    StatementResult<LetStatement> result = {
        .success = false,
        .statement_ptr = nullptr,
    };

    if (parser == nullptr) {
        return result;
    }

    if (!parser->expect_peek(TokenType::IDENT)) {
        result.success = false;
        return result;
    }

    if (!parser->expect_peek(TokenType::ASSIGN)) {
        result.success = false;
        return result;
    }

    // TODO(yemon): Should this be the correct way to initialize this?
    result.statement_ptr = new LetStatement();
    result.statement_ptr->name = Identifier{
        .token = parser->cur_token,
        .value = parser->cur_token.literal,
    };

    result.success = true;
    return result;
}

StatementResult<ReturnStatement>
ParseReturnStatement(Parser *parser)
{
    StatementResult<ReturnStatement> result = {
        .success = false,
        .statement_ptr = nullptr,
    };

    if (parser == nullptr) {
        return result;
    }

    parser->next_token();

    // TODO(yemon): Skipping the expression parsing for now
    // TODO(yemon): Should this be the correct way to initialize this?
    result.statement_ptr = new ReturnStatement();   
    result.statement_ptr->value = Expression{};

    while (!parser->is_cur_token(TokenType::SEMICOLON)) {
        parser->next_token();
    }

    result.success = true;
    return result;
}

ExpressionStatement
ParseExpression(Parser *parser)
{
    ExpressionStatement statement{};

    return statement;
}
