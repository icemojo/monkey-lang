#include "mks_ast.h"


string 
Identifier::token_literal() const
{
    return token.literal;
}

string
Expression::token_literal() const
{
    return token.literal;
}

//------------------------------------------------------------------------------

string 
Statement::token_literal() const
{
    return string{ "Statement base: " };
}

string 
LetStatement::token_literal() const
{
    return token.literal;
}

string 
ReturnStatement::token_literal() const 
{
    return token.literal;
}

string
ExpressionStatement::token_literal() const
{
    return token.literal;
}

