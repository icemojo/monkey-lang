#include "mks_ast.h"


string 
Statement::token_literal() const
{
    return "Statement ";
}

//------------------------------------------------------------------------------

string 
Identifier::token_literal() const
{
    return token.literal;
}

string 
LetStatement::token_literal() const
{
    return token.literal;
}

//------------------------------------------------------------------------------

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

