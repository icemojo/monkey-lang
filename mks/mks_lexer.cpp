#include <iostream>
#include <format>
#include "mks_lexer.h"


Token
TokenNew(TokenType type, byte ch) 
{
    return Token{ type, { ch } };
}

Token
TokenNew(TokenType type, const std::string &literal) 
{
    return Token{ type, literal };
}

TokenType
CheckKeywordOrIdentifier(const std::string_view &identifier)
{
    if (KEYWORDS.size() == 0) {
        return TokenType::IDENT;
    }

    if (const auto search_it = KEYWORDS.find(identifier); search_it != KEYWORDS.end()) {
        return search_it->second;
    }

    return TokenType::IDENT;
}

//------------------------------------------------------------------------------

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

byte 
Lexer::peek_char()
{
    if (read_position >= input.size()) {
        return '\0';
    }
    else {
        return input[read_position];
    }
}

Token
Lexer::next_token()
{
    Token token{};
    skip_whitespace();

    switch (ch) {
    case '=':
        if (const byte nch = peek_char(); nch == '=') {
            read_char();
            token = TokenNew(TokenType::EQ, { ch, nch });
        }
        else {
            token = TokenNew(TokenType::ASSIGN, ch);
        }
        break;
    case '!':
        if (const byte nch = peek_char(); nch == '=') {
            read_char();
            token = TokenNew(TokenType::NOT_EQ, { ch, nch });
        }
        else {
            token = TokenNew(TokenType::BANG, ch);
        }
        break;
    case '+':
        token = TokenNew(TokenType::PLUS, ch);
        break;
    case '-':
        token = TokenNew(TokenType::MINUS, ch);
        break;
    case '*':
        token = TokenNew(TokenType::ASTRISK, ch);
        break;
    case '/':
        token = TokenNew(TokenType::SLASH, ch);
        break;
    case '<':
        if (const byte nch = peek_char(); nch == '=') {
            read_char();
            token = TokenNew(TokenType::LT_EQ, { ch, nch });
        }
        else {
            token = TokenNew(TokenType::LT, ch);
        }
        break;
    case '>':
        if (const byte nch = peek_char(); nch == '=') {
            read_char();
            token = TokenNew(TokenType::GT_EQ, { ch, nch });
        }
        else {
            token = TokenNew(TokenType::GT, ch);
        }
        break;

    case ',':
        token = TokenNew(TokenType::COMMA, ch);
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

        token = TokenNew(TokenType::ILLEGAL, ch);
        break;
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

//------------------------------------------------------------------------------

bool 
ValidIdentifierLetter(const byte c)
{
    // A-Z, a-z, _
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
}

