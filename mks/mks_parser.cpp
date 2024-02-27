#include <iostream>
#include "mks_parser.h"


string
Program::token_literal() const
{
    if (statements.empty()) {
        return "";
    }
    else {
        return statements.at(0).token_literal();
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
        LetStatement *let_statement = it->parse_let_statement();

        if (let_statement != nullptr) {
            //TODO(yemon): program->statements.push_back(*let_statement);
        }

        it.next();
    }

    return program;
}

Statement *
Parser::parse_statement()
{
    switch (cur_token.type) {
    case TokenType::LET:
        return new Statement{};
        break;

    default:
        return nullptr;
    }
}

LetStatement *
Parser::parse_let_statement()
{
    LetStatement *statement = new LetStatement{ };

    if (!expect_peek(TokenType::IDENT)) {
        return nullptr;
    }

    statement->name = new Identifier{
        .token = cur_token,
        .value = cur_token.literal
    };

    if (!expect_peek(TokenType::ASSIGN)) {
        return nullptr;
    }

    // TODO(yemon): Skipping the expression part for now, 
    // since this requires recursive-descent-parsing
    statement->value = nullptr;
    while (!is_cur_token(TokenType::SEMICOLON)) {
        next_token();
    }

    return statement;
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
