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
    LexerReadChar(*lexer);
    return lexer;
}

void
LexerReadChar(Lexer &lexer)
{
    if (lexer.read_position >= lexer.input.size()) {
        lexer.ch = '\0';
    }
    else {
        lexer.ch = lexer.input[lexer.read_position];
    }

    lexer.position = lexer.read_position;
    lexer.read_position += 1;
}

Token
LexerNextToken(Lexer &lexer)
{
    Token token{};

    switch (lexer.ch) {
    case '=':
        token = TokenNew(TokenType::ASSIGN, lexer.ch);
        break;
    case ';':
        token = TokenNew(TokenType::SEMICOLON, lexer.ch);
        break;
    case '(':
        token = TokenNew(TokenType::LPAREN, lexer.ch);
        break;
    case ')':
        token = TokenNew(TokenType::RPAREN, lexer.ch);
        break;
    case '{':
        token = TokenNew(TokenType::LBRACE, lexer.ch);
        break;
    case '}':
        token = TokenNew(TokenType::RBRACE, lexer.ch);
        break;
    case ',':
        token = TokenNew(TokenType::COMMA, lexer.ch);
        break;
    case '+':
        token = TokenNew(TokenType::PLUS, lexer.ch);
        break;

    default:
        break;
    }

    LexerReadChar(lexer);
    return token;
}

