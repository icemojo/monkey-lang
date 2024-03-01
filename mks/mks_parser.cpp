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

Program *
Parser::parse_program() 
{
    Program *program = new Program{};
    program->statements = {};

    ParserIter it = ParserIter{ this };
    while (it.valid()) {
        if (StatementResult<Statement> result = it->parse_statement(); 
            result.success) {
            program->statements.push_back(*result.statement_ptr);
            delete result.statement_ptr;
        }

        it.next();
    }

    return program;
}

StatementResult<Statement>
Parser::parse_statement()
{
    StatementResult<Statement> result = {
        .success = false,
        .statement_ptr = nullptr,
    };

    switch (cur_token.type) {
    case TokenType::LET:
        if (StatementResult<LetStatement> let_result = parse_let_statement();
            let_result.success) {
            result.success = true;
            result.statement_ptr = let_result.statement_ptr;
            return result;
        }
        break;

    default:
        return result;
    }

    return result;
}

StatementResult<LetStatement>
Parser::parse_let_statement()
{
    StatementResult<LetStatement> result{
        .success = false,
        .statement_ptr = nullptr,
    };

    if (!expect_peek(TokenType::IDENT)) {
        result.success = false;
        return result;
    }

    if (!expect_peek(TokenType::ASSIGN)) {
        result.success = false;
        return result;
    }

    result.statement_ptr = new LetStatement();
    result.statement_ptr->name = Identifier{
        .token = cur_token,
        .value = cur_token.literal,
    };

    // TODO(yemon): Skipping the expression part for now, 
    // since this requires recursive-descent-parsing
    result.statement_ptr->value = Expression{};
    while (!is_cur_token(TokenType::SEMICOLON)) {
        next_token();
    }

    result.success = true;
    return result;
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
