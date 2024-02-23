#ifndef MKS_PARSER_
#define MKS_PARSER_

#include <memory>
#include "mks_lexer.h"
#include "mks_ast.h"


struct Parser;

Parser *
ParserNew(std::unique_ptr<Lexer> lexer);

struct Parser {
    std::unique_ptr<Lexer> lexer;
    Token cur_token = {};
    Token peek_token = {};

    void next_token();

    Program *parse_program();
};

#endif  // MKS_PARSERS_
