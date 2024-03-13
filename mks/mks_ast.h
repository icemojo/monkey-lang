#ifndef MKS_ATS_
#define MKS_ATS_

#include <string>
#include <vector>
#include <concepts>
#include "mks_lexer.h"

using std::string;
using std::vector;


struct Identifier {
    Token token;
    string value;

    string token_literal() const;
};

struct Expression {
    Token token;

    string token_literal() const;
};

//------------------------------------------------------------------------------

struct Statement {
    virtual string token_literal() const;
};

template <typename T>
struct StatementResult {
    bool success{ false };
    T *statement_ptr{ nullptr };

    static_assert(std::derived_from<T, Statement>, "Type T of StatementResult<T> must be derived from Statement.");
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

// NOTE(yemon): Simply only because Monkey lang will allow this kind of shennanigan,
//  x + 12;
struct ExpressionStatement : Statement {
    Token token;
    Expression expression;

    string token_literal()  const override;
};


#endif  // MKS_ATS_
