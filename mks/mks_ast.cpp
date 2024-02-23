#include "mks_ast.h"


std::string 
Statement::token_literal() const
{
    return "Statement ";
}

//------------------------------------------------------------------------------

std::string 
Identifier::token_literal() const
{
    return token.literal;
}

std::string 
LetStatement::token_literal() const
{
    return token.literal;
}

//------------------------------------------------------------------------------

std::string
Program::token_literal() const
{
    if (statements.empty()) {
        return "";
    }
    else {
        return statements.at(0).token_literal();
    }
}

