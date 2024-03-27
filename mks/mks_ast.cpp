#include <iostream>
#include "mks_ast.h"


//string
//Expression::token_literal() const
//{
//    return token.literal;
//}
//
//string
//Expression::to_string() const
//{
//    return token_literal();
//}
//
//std::ostream &
//operator<<(std::ostream &out, const Expression &expression) 
//{
//    return out << expression.token_literal();
//}

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
IntegerLiteral::token_literal() const
{
    return token.literal;
}

string
IntegerLiteral::to_string() const
{
    return token_literal();
}

string
PrefixExpression::token_literal() const
{
    return token.literal;
}

#include <format>

string
PrefixExpression::to_string() const
{
    return string{ std::format(
        "({}{})",
        optr,
        ExpressionToString(right)
    ) };
}

string 
InfixExpression::token_literal() const
{
    return token.literal;
}

string
InfixExpression::to_string() const
{
    return string{ std::format(
        "({}{}{})", 
        ExpressionToString(left),
        optr, 
        ExpressionToString(right)
    ) };
}

// NOTE(yemon): Should this return a string_view instead of a new string instance?
string
ExpressionToString(const Expression &expression)
{
    switch (expression.type) {
    case ExpressionType::IDENT: {
        if (const Identifier *ident = std::get_if<Identifier>(&expression.variant)) {
            return ident->to_string();
        }
        return "";
    } break;

    case ExpressionType::INT_LIT: {
        if (const IntegerLiteral *int_lit = std::get_if<IntegerLiteral>(&expression.variant)) {
            return int_lit->to_string();
        }
        return "";
    } break;

    case ExpressionType::PREFIX: {
        if (const PrefixExpression *prefix_expr = std::get_if<PrefixExpression>(&expression.variant)) {
            return prefix_expr->to_string();
        }
        return "";
    } break;

    case ExpressionType::INFIX: {
        if (const InfixExpression *infix_expr = std::get_if<InfixExpression>(&expression.variant)) {
            return infix_expr->to_string();
        }
        return "";
    } break;

    default:
        // TODO(yemon): Error handling and reporting
        break;
    }
}

//------------------------------------------------------------------------------

string
LetStatement::token_literal() const
{
    return token.literal;
}

string
LetStatement::to_string() const
{
    // NOTE(yemon): I did the stream buffer method for the Program.to_string(),
    // so I'm doing the simpler std::string append mechanism for the 
    // statements here, just for comparative reference in the future.
    string buffer{};

    buffer += token_literal();
    buffer += name.value;
    buffer += " = ";
    buffer += ExpressionToString(value);
    buffer += ";";

    return buffer;
}

string 
ReturnStatement::token_literal() const 
{
    return token.literal;
}

string
ReturnStatement::to_string() const
{
    string buffer{};

    buffer += token_literal() + " ";
    buffer += ExpressionToString(value);
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
    return ExpressionToString(expression);
}

string
StatementToString(const Statement &statement)
{
    switch (statement.type) {
    case StatementType::LET: {
        if (const LetStatement *let_statement = std::get_if<LetStatement>(&statement.variant)) {
            return let_statement->to_string();
        }
        return "";
    } break;
        
    case StatementType::RETURN: {
        if (const ReturnStatement *return_statement = std::get_if<ReturnStatement>(&statement.variant)) {
            return return_statement->to_string();
        }
        return "";
    } break;

    case StatementType::EXPRESSION: {
        if (const ExpressionStatement *expr_statement = std::get_if<ExpressionStatement>(&statement.variant)) {
            return expr_statement->to_string();
        }
        return "";
    } break;

    default:
        // TODO(yemon): Error handlings and reporting
        break;
    }
}

