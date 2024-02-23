#ifndef MKS_ATS_
#define MKS_ATS_

#include <string>
#include <vector>
#include "mks_lexer.h"


struct Node {
    // NOTE(yemon): 'token_literal' and all similar methods in the sub-types
    // are pretty much for debugging purpose.
    std::string token_literal() const;
};

struct Statement {

    std::string token_literal() const;

    std::string statement_node() const;
};

struct Expression {

    std::string token_literal() const;

    std::string expression_node() const;
};

//------------------------------------------------------------------------------

struct Identifier {
    Token token;
    std::string value;

    Identifier() {
        token = {
            .type = TokenType::IDENT,
            .literal = "",
        };
        value = token.literal;
    }

    std::string token_literal() const;
};

//  let <identifier> = <expression>;
struct LetStatement {
    Token token;
    Identifier *name;
    Expression value;

    LetStatement() {
        token = TokenNew(TokenType::LET, "let");    //?
        name = nullptr;
        value = {};
    }

    std::string token_literal() const;
};

//------------------------------------------------------------------------------

struct Program {
    std::vector<Statement> statements;

    std::string token_literal() const;
};

#endif  // MKS_ATS_
