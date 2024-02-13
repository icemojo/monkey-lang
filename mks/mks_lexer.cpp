#include <iostream>
#include <format>
//#include <cctype>
#include "mks_lexer.h"


Token
TokenNew(TokenType type, byte ch) 
{
    return Token{ type, { ch } };
}

TokenType
CheckKeywordOrIdentifier(const std::string identifier)
{
    if (identifier == "fn") {
        return TokenType::FUNCTION;
    }
    if (identifier == "let") {
        return TokenType::LET;
    }
    return TokenType::IDENT;
}


std::unique_ptr<Lexer>
LexerNew(const std::string &input)
{
    auto lexer = std::make_unique<Lexer>(input);
    lexer->read_char();
    return lexer;
}

void
PrintLexation(const Lexer &lexer)
{
    std::cout << "Lexer (input):\n";
    std::cout << lexer.input;
    std::cout << '\n';

    std::cout << "Lexer:\n";
    std::cout << '\n';
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
    skip_whitespace();

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

    case '\0':
        token.type = TokenType::T_EOF;
        token.literal = {};
        break;

    default:
        // NOTE(yemon): Identifier starts with a proper alphabet
        if (std::isalpha(ch) != 0) {
            token.literal = read_identifier();
            token.type = CheckKeywordOrIdentifier(token.literal);
            return token;
        }
        else if (std::isdigit(ch) != 0) {
            token.literal = read_number();
            token.type = TokenType::INT;
            return token;
        }
        else {
            token = TokenNew(TokenType::ILLEGAL, ch);
        }
    }

    read_char();
    return token;
}

std::string
Lexer::read_identifier()
{
    std::string identifier{};

    const auto start_it = input.begin() + position;
    for (auto it = input.begin() + position; it != input.end(); it += 1) {
        if (ValidIdentifierLetter(*it)) {
            read_char();
            continue;
        }
        else {
            return identifier = { start_it, it };
        }
    }

    return "";
}

std::string
Lexer::read_number()
{
    std::string identifier{};

    const auto start_it = input.begin() + position;
    for (auto it = input.begin() + position; it != input.end(); it += 1) {
        if (std::isdigit(*it) != 0) {
            read_char();
            continue;
        }
        else {
            return identifier = { start_it, it };
        }
    }

    return "0";
}

void 
Lexer::skip_whitespace()
{
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        read_char();
    }
}

bool 
ValidIdentifierLetter(const byte c)
{
    // A-Z, a-z, _
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
}

