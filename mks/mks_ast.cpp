#include <sstream>
#include "mks_ast.h"

using std::ostringstream;


string 
Identifier::token_literal() const
{
    return token.literal;
}

string 
Identifier::to_string() const
{
    return value;
}

string
Expression::token_literal() const
{
    return token.literal;
}

string
Expression::to_string() const
{
    return token_literal();
}

std::ostream &
operator<<(std::ostream &out, const Expression &expression) 
{
    return out << expression.token_literal();
}

//------------------------------------------------------------------------------

string 
Statement::token_literal() const
{
    return string{ "Statement base: " };
}

string Statement::to_string() const
{
    return token_literal();
}

string 
LetStatement::token_literal() const
{
    return token.literal;
}

string
LetStatement::to_string() const
{
    // NOTE(yemon): Is using the stream objects the right way to
    // compose strings like that in memory? Idk!
    // Maybe this doesn't make a difference for short strings like a simple 
    // program statement, or maybe even more expensive compared to 
    // std::string push_back(). But I do like the syntax structure though...
    ostringstream buffer{};

    buffer << token_literal();
    buffer << name.value;
    buffer << " = ";

    if (value) {
        buffer << *value;
    }

    buffer << ";";

    return buffer.str();
}

string 
ReturnStatement::token_literal() const 
{
    return token.literal;
}

string
ReturnStatement::to_string() const
{
    // NOTE(yemon): I did the stream buffer method for the LetStatement.
    // So I'm doing the simpler std::string append mechanism here, 
    // just for comparative reference in the future.
    string buffer{};

    buffer += token_literal() + " ";

    if (value) {
        buffer += (*value).token_literal();
    }

    buffer += ";";

    return buffer;
}

string
ExpressionStatement::token_literal() const
{
    return token.literal;
}

string 
ExpressionStatement::to_string() const
{
    if (expression) {
        return (*expression).to_string();
    }
    return "";
}

