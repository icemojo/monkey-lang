#include "mks_ast.h"


string 
Identifier::token_literal() const
{
    return token.literal;
}

//------------------------------------------------------------------------------

string 
Statement::token_literal() const
{
    return string{ "Statment: " };
}

string 
LetStatement::token_literal() const
{
    return string{ "LetStatement: " };
}

