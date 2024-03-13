#ifndef MKS_ATS_
#define MKS_ATS_

#include <string>
#include <vector>
#include <concepts>
#include <optional>
#include "mks_lexer.h"

using std::string;
using std::vector;
using std::optional;


struct Identifier {
    Token token;
    string value;

    string token_literal() const;

    string to_string() const;
};

struct Expression {
    Token token;

    string token_literal() const;

    string to_string() const;
};

std::ostream &
operator<<(std::ostream &out, const Expression &expression);

//------------------------------------------------------------------------------

struct Statement {
    virtual string token_literal() const;

    virtual string to_string() const;
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
    optional<Expression> value;

    LetStatement() {
        token = TokenNew(TokenType::LET, "let");    //?
        name = {},
        value = {};
    }

    string token_literal() const override;

    string to_string() const override;
};

//  return <expression>;
struct ReturnStatement : Statement {
    Token token;
    optional<Expression> value;

    ReturnStatement() {
        token = TokenNew(TokenType::RETURN, "return");
        value = {};
    }

    string token_literal() const override;

    string to_string() const override;
};

// NOTE(yemon): Simply only because Monkey lang will allow this kind of shennanigan,
//  let x = 10;
//  x + 12;
// Both of those statements above are completely legal, and thus 
// expressions are dubbed as expression statements.
struct ExpressionStatement : Statement {
    Token token;
    optional<Expression> expression;

    string token_literal() const override;

    string to_string() const override;
};

#endif  // MKS_ATS_
