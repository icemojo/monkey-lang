#ifndef MKS_PARSER_
#define MKS_PARSER_

#include "mks_lexer.h"
#include "mks_ast.h"


struct Parser;

Parser *
ParserNew(Lexer *lexer);

struct Parser {
    Lexer *lexer = nullptr;
    Token cur_token = {};
    Token peek_token = {};

    void next_token();

    Program *parse_program();
};

#endif  // MKS_PARSERS_
