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
};

//------------------------------------------------------------------------------

template <typename T>
struct StatementResult {
    bool success{ false };
    T *statement_ptr{};
};

struct Statement {
    virtual string token_literal() const;
};

//  let <identifier> = <expression>;
struct LetStatement : Statement {
    Token token;
    Identifier name;
    Expression value;

    LetStatement() {
        token = TokenNew(TokenType::LET, "let");    //?
        name = {},
        value = {};
    }

    string token_literal() const override;

    //~LetStatement() {
    //    token = {};
    //    if (name != nullptr) {
    //        delete name;
    //    }
    //    if (value != nullptr) {
    //        delete value;
    //    }
    //}

    //string token_literal() const;
};

//  return <expression>;
struct ReturnStatement : Statement {
    Token token;
    Expression value;

    ReturnStatement() {
        token = TokenNew(TokenType::RETURN, "return");
        value = {};
    }

    string token_literal() const override;
};


#endif  // MKS_ATS_
