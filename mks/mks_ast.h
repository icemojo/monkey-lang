#ifndef MKS_ATS_
#define MKS_ATS_

#include <string>
#include <vector>
#include "mks_lexer.h"

using std::string;
using std::vector;


struct Node {
    // NOTE(yemon): 'token_literal' and all similar methods in the sub-types
    // are pretty much for debugging purpose.
    string token_literal() const;
};

//------------------------------------------------------------------------------

struct Statement {

    string token_literal() const;

    string statement_node() const;
};

struct Identifier {
    Token token;
    string value;

    //Identifier() {
    //    token = {
    //        .type = TokenType::IDENT,
    //        .literal = "",
    //    };
    //    value = token.literal;
    //}

    string token_literal() const;
};

struct Expression {
    Token token;

    string token_literal() const;

    string expression_node() const;
};

//------------------------------------------------------------------------------

//  let <identifier> = <expression>;
struct LetStatement {
    Token token;
    Identifier *name;
    Expression *value;

    LetStatement() {
        token = TokenNew(TokenType::LET, "let");    //?
        name = nullptr;
        value = {};
    }

    string token_literal() const;
};

#endif  // MKS_ATS_
