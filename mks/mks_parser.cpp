#include <sstream>
#include "mks_parser.h"
#include "mks_types.h"


string
Program::token_literal() const
{
    if (statements.empty()) {
        return "";
    }
    else {
        //return statements.at(0).token_literal();
        return "";
    }
}

string 
Program::to_string() const
{
    // NOTE(yemon): Using std::ostringstream might be more expensive than just 
    // simple std::string.push_back() when building strings in memory, since streams has
    // additional formatting and type conversion mechanisms built-in to "stream" strings
    // from different sources. I'm just leaving this here just for future reference.
    // This is not based on any special design decisions or anything.
    std::ostringstream buffer{};

    for (const Statement &statement : statements) {
        //buffer << statement.to_string();
        buffer << StatementToString(statement);
    }

    return buffer.str();
}

//------------------------------------------------------------------------------

unique_ptr<Parser>
ParserNew(unique_ptr<Lexer> lexer)
{
    auto parser = make_unique<Parser>();
    parser->lexer = move(lexer);

    // Read two tokens, so that current token and peek tokens are set
    parser->next_token();
    parser->next_token();

    return parser;
}

[[nodiscard]] bool 
ParserIter::valid() const
{
    if (parser == nullptr) return false;
    return parser->cur_token.type != TokenType::T_EOF;
}

void 
ParserIter::next()
{
    parser->next_token();
}

void
Parser::next_token()
{
    cur_token = peek_token;
    peek_token = lexer->next_token();
}

bool
Parser::is_cur_token(const TokenType type) const
{
    return cur_token.type == type;
}

bool 
Parser::is_peek_token(const TokenType type) const
{
    return peek_token.type == type;
}

bool 
Parser::expect_peek(const TokenType type)
{
    if (is_peek_token(type)) {
        next_token();
        return true;
    }
    else {
        return false;
    }
}

//------------------------------------------------------------------------------

Program *
ParseProgram(Parser *parser)
{
    Program *program = new Program{};
    program->statements = {};

    ParserIter it = ParserIter{ parser };
    while (it.valid()) {
        if (StatementResult result = ParseStatement(it.parser);
            result.success) {
            program->statements.push_back(result.statement);
            //program->statements.push_back(*result.statement_ptr);
            //delete result.statement_ptr;
        }

        it.next();
    }

    return program;
}

StatementResult //<Statement>
ParseStatement(Parser *parser)
{
    //StatementResult<Statement> result = {
    //    .success = false,
    //    .statement_ptr = nullptr,
    //};
    //if (parser == nullptr) {
    //    return result;
    //}

    //StatementResult result{};

    switch (parser->cur_token.type) {
    case TokenType::LET:
        //if (StatementResult<LetStatement> parse_result = ParseLetStatement(parser);
        //    parse_result.success) {
        //    result.success = true;
        //    result.statement_ptr = parse_result.statement_ptr;
        //    return result;
        //}
        return ParseLetStatement(parser);

    case TokenType::RETURN:
        //if (StatementResult<ReturnStatement> parse_result = ParseReturnStatement(parser);
        //    parse_result.success) {
        //    result.success = true;
        //    result.statement_ptr = parse_result.statement_ptr;
        //    return result;
        //}
        return ParseReturnStatement(parser);

    default:
        //if (StatementResult<ExpressionStatement> parse_result = ParseExpressionStatement(parser);
        //    parse_result.success) {
        //    result.success = true;
        //    result.statement_ptr = parse_result.statement_ptr;
        //    return result;
        //}
        StatementResult result{};
        ExpressionStatement expr_statement = ParseExpressionStatement(parser);

        result.statement = Statement(StatementType::EXPRESSION, expr_statement);
        result.success = true;
        return result;
    }

    //return result;
}

StatementResult //<LetStatement>
ParseLetStatement(Parser *parser)
{
    //StatementResult<LetStatement> result = {
    //    .success = false,
    //    .statement_ptr = nullptr,
    //};
    //if (parser == nullptr) {
    //    return result;
    //}

    StatementResult result{};

    if (!parser->expect_peek(TokenType::IDENT)) {
        result.success = false;
        return result;
    }

    if (!parser->expect_peek(TokenType::ASSIGN)) {
        result.success = false;
        return result;
    }

    Identifier identifier{};
    identifier.token = parser->cur_token;
    identifier.value = parser->cur_token.literal;
    LetStatement let_statement{ identifier };

    ExpressionStatement expr_statement = ParseExpression(parser, Prec::LOWEST);
    let_statement.value = expr_statement.expression;

    //if (StatementResult<ExpressionStatement> statement_result = ParseExpressionStatement(parser);
    //    statement_result.success) {
    //    ExpressionStatement statement = statement_result.statement;

    //    result.statement.value = Expression{
    //        .type = statement_result.statement.token.type,  // ???
    //    };
    //    result.statement.value = statement_result.statement.expression;
    //}

    result.statement = Statement(StatementType::LET, let_statement);
    result.success = true;
    return result;
}

StatementResult //<ReturnStatement>
ParseReturnStatement(Parser *parser)
{
    //StatementResult<ReturnStatement> result = {
    //    .success = false,
    //    .statement_ptr = nullptr,
    //};
    //if (parser == nullptr) {
    //    return result;
    //}

    StatementResult result{};

    parser->next_token();       // TODO(yemon): What was I skipping here exactly?

    ReturnStatement return_statement{};

    ExpressionStatement expr_statement = ParseExpression(parser, Prec::LOWEST);
    return_statement.value = expr_statement.expression;

    // NOTE(yemon): Do I still need this?
    while (!parser->is_cur_token(TokenType::SEMICOLON)) {
        parser->next_token();
    }

    result.statement = Statement(StatementType::RETURN, return_statement);
    result.success = true;
    return result;
}

Prec
CheckPrecedence(const TokenType &token_type)
{
    if (const auto search_it = PRECEDENCES.find(token_type); 
        search_it != PRECEDENCES.end()) {
        return search_it->second;
    }
    return Prec::LOWEST;
}

//StatementResult<ExpressionStatement>
ExpressionStatement
ParseExpressionStatement(Parser *parser)
{
    ExpressionStatement expr_statement = ParseExpression(parser, Prec::LOWEST);

    if (parser->is_peek_token(TokenType::SEMICOLON)) {
        parser->next_token();
    }

    return expr_statement;
}

// TODO(yemon): Could this be simplified better (and possibly combined with) 
// in relation to the `ParseExpressionStatement(..)` above?
ExpressionStatement
ParseExpression(Parser *parser, const Prec prec)
{
    ExpressionStatement left_expr_statement{};

    // NOTE(yemon): The original mechanism in the book relies on having *two* 
    // different maps (with essentially, function pointers), first one for prefixes,
    // and second one for the infixes. So, if a particular token type is not found
    // in the first 'prefix' map first, then the following cases can be handled easily.
    // But... I feel like relying on two different maps with function pointers here
    // is kinda pointless. So I went with a simple double-switch-blocks here.

    // Prefix expressions
    switch (parser->cur_token.type) {
    case TokenType::IDENT: {
        Identifier identifier = ParseIdentifier(parser);
        left_expr_statement.token = identifier.token;
        left_expr_statement.expression = Expression(
            ExpressionType::IDENT,
            identifier
        );
    } break;
    
    case TokenType::INT: {
        IntegerLiteral int_literal = ParseIntegerLiteral(parser);
        left_expr_statement.token = int_literal.token;
        left_expr_statement.expression = Expression(
            ExpressionType::INT_LIT, 
            int_literal
        );
    } break;

    case TokenType::BANG:
    case TokenType::MINUS: {
        PrefixExpression prefix_expr = ParsePrefixExpression(parser);
        left_expr_statement.token = prefix_expr.token;
        left_expr_statement.expression = Expression(
            ExpressionType::PREFIX,
            prefix_expr
        );
    } break;

    default:
        // TODO(yemon): Should there be some sort of error reporting here,
        // if none of the valid expression-like tokens were recognized?
        // Maybe tag the error messages based on the current token type,
        // so that new (or missed-out) tokens can be caught here basically.
        break;
    }

    // Infix expressions
    const Prec next_token_prec = CheckPrecedence(parser->peek_token.type);
    while (!parser->is_peek_token(TokenType::SEMICOLON) && prec < next_token_prec) {
        parser->next_token();

        switch (parser->cur_token.type) {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::ASTRISK:
        case TokenType::SLASH:
        case TokenType::EQ:
        case TokenType::NOT_EQ:
        case TokenType::LT:
        case TokenType::GT: {
            ExpressionStatement right_exp_statement{};
            //InfixExpression infix_expr = ParseInfixExpression(
            //    parser, 
            //    *(left_exp_statement.expression)
            //);
            InfixExpression infix_expr = ParseInfixExpression(
                parser, left_expr_statement.expression);
            right_exp_statement.token = infix_expr.token;
            right_exp_statement.expression = Expression(
                ExpressionType::INFIX,
                infix_expr
            );
            //right_exp_statement.expression = infix_expr;
            //left_exp_statement = right_exp_statement;       // ?
        } break;

        default:
            // TODO(yemon): Error handling and reporting?
            //return left_expr_statement;
            break;
        }
    }

    return left_expr_statement;
}

Identifier
ParseIdentifier(Parser *parser)
{
    Identifier identifier{};
    identifier.token = parser->cur_token;
    identifier.value = parser->cur_token.literal;

    return identifier;
}

IntegerLiteral
ParseIntegerLiteral(Parser *parser)
{
    int64_t value = 0;
    if (mks::stoll_result result = mks::stoll(parser->cur_token.literal);
        result.success) {
        value = *result;
    }

    IntegerLiteral int_literal{};
    int_literal.token = parser->cur_token;
    int_literal.value = value;

    return int_literal;
}

PrefixExpression
ParsePrefixExpression(Parser *parser)
{
    PrefixExpression prefix_expr{};
    prefix_expr.token = parser->cur_token;
    prefix_expr.optr = parser->cur_token.literal;

    parser->next_token();

    ExpressionStatement expr_statement = ParseExpression(parser, Prec::PREFIX);
    //prefix_expression.right = *(expr_statement.expression);
    prefix_expr.right = expr_statement.expression;

    return prefix_expr;
}

InfixExpression
ParseInfixExpression(Parser *parser, const Expression &left)
{
    InfixExpression infix_expr{};
    infix_expr.token = parser->cur_token;
    infix_expr.optr  = parser->cur_token.literal;
    infix_expr.left = left;

    const Prec prec = CheckPrecedence(parser->cur_token.type);
    parser->next_token();
    
    ExpressionStatement right_expr_statement = ParseExpression(parser, prec);
    //infix_expression.right = *(right_exp_statement.expression);
    infix_expr.right = right_expr_statement.expression;

    return infix_expr;
}

