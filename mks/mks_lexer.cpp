#include "mks_lexer.h"


Token
TokenNew(TokenType type, byte ch) 
{
    return Token{ type, { ch } };
}


std::unique_ptr<Lexer>
LexerNew(const std::string &input)
{
    auto lexer = std::make_unique<Lexer>(input);
    lexer->read_char();
    return lexer;
}

void 
Lexer::read_char()
{
    if (read_position >= input.size()) {
        ch = '\0';
    }
    else {
        ch = input[read_position];
    }

    position = read_position;
    read_position += 1;
}

Token
Lexer::next_token()
{
    Token token{};

    switch (ch) {
    case '=':
        token = TokenNew(TokenType::ASSIGN, ch);
        break;
    case ';':
        token = TokenNew(TokenType::SEMICOLON, ch);
        break;
    case '(':
        token = TokenNew(TokenType::LPAREN, ch);
        break;
    case ')':
        token = TokenNew(TokenType::RPAREN, ch);
        break;
    case '{':
        token = TokenNew(TokenType::LBRACE, ch);
        break;
    case '}':
        token = TokenNew(TokenType::RBRACE, ch);
        break;
    case ',':
        token = TokenNew(TokenType::COMMA, ch);
        break;
    case '+':
        token = TokenNew(TokenType::PLUS, ch);
        break;

    default:
        break;
    }

    read_char();
    return token;
}

