#include "mks_ast.h"


string 
Statement::token_literal() const
{
    return "Statement ";
}

string 
Identifier::token_literal() const
{
    return token.literal;
}

//------------------------------------------------------------------------------

string 
LetStatement::token_literal() const
{
    return token.literal;
}

