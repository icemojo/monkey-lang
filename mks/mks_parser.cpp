#include "mks_parser.h"


Parser *
ParserNew(Lexer *lexer)
{
    Parser *parser = new Parser{
        .lexer = lexer
    };

    // Read two tokens, so that current token and peek tokens are set
    parser->next_token();
    parser->next_token();

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
    return nullptr;
}

