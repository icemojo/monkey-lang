#include <iostream>
#include <format>
#include "mks_lexer.h"


constexpr string
TokenTypeToString(TokenType type)
{
    switch (type) {
    case TokenType::ILLEGAL:
        return "ILLEGAL";
    case TokenType::T_EOF:
        return "T_EOF";

    case TokenType::IDENT:
        return "IDENT";
    case TokenType::INT:
        return "INT";

    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::BANG:
        return "BANG";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::ASTRISK:
        return "ASTRISK";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::LT:
        return "LT";
    case TokenType::GT:
        return "GT";
    case TokenType::LT_EQ:
        return "LT_EQ";
    case TokenType::GT_EQ:
        return "GT_EQ";
    case TokenType::EQ:
        return "EQ";
    case TokenType::NOT_EQ:
        return "NOT_EQ";

    case TokenType::COMMA:
        return "COMMA";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::LPAREN:
        return "LPAREN";
    case TokenType::RPAREN:
        return "RPAREN";
    case TokenType::LBRACE:
        return "LBRACE";
    case TokenType::RBRACE:
        return "RBRACE";

    case TokenType::LET:
        return "LET";
    case TokenType::FUNCTION:
        return "FUNCTION";
    case TokenType::RETURN:
        return "RETURN";
    case TokenType::IF:
        return "IF";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::TRUE:
        return "TRUE";
    case TokenType::FALSE:
        return "FALSE";
        
    default:
        return "TokenType::ILLEGAL";
    }
}

std::ostream &
operator<<(std::ostream &out, const TokenType token_type)
{
    return out << TokenTypeToString(token_type);
}

//------------------------------------------------------------------------------

void 
Token::print() const
{
    std::cout << "  " << type << " " << literal << '\n';
}

Token
TokenNew(TokenType type, char ch) 
{
    return Token{ type, { ch } };
}

Token
TokenNew(TokenType type, const string &literal) 
{
    return Token{ type, literal };
}

TokenType
CheckKeywordOrIdentifier(const string_view &identifier)
{
    if (KEYWORDS.size() == 0) {
        return TokenType::ILLEGAL;
    }

    if (const auto search_it = KEYWORDS.find(identifier); search_it != KEYWORDS.end()) {
        return search_it->second;
    }

    return TokenType::ILLEGAL;
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

char 
Lexer::peek_char() const
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
    skip_whitespaces();

    switch (ch) {
    case '=':
        if (const char nch = peek_char(); nch == '=') {
            token = TokenNew(TokenType::EQ, { ch, nch });
            read_char();
        }
        else {
            token = TokenNew(TokenType::ASSIGN, ch);
        }
        break;
    case '!':
        if (const char nch = peek_char(); nch == '=') {
            token = TokenNew(TokenType::NOT_EQ, { ch, nch });
            read_char();
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
        if (const char nch = peek_char(); nch == '=') {
            token = TokenNew(TokenType::LT_EQ, { ch, nch });
            read_char();
        }
        else {
            token = TokenNew(TokenType::LT, ch);
        }
        break;
    case '>':
        if (const char nch = peek_char(); nch == '=') {
            token = TokenNew(TokenType::GT_EQ, { ch, nch });
            read_char();
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

string
Lexer::read_identifier()
{
    string identifier{};

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

string
Lexer::read_number()
{
    string identifier{};

    for (auto it = input.begin() + position; it != input.end(); it += 1) {
        if (std::isdigit(*it) != 0) {
            identifier.push_back(*it);
            read_char();
            continue;
        }
        else {
            break;
        }
    }

    if (!identifier.empty()) {
        return identifier;
    }
    else {
        return "0";
    }
}

void 
Lexer::skip_whitespaces()
{
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        read_char();
    }
}

void
Lexer::print() const
{
    std::cout << "Lexer (input):\n";
    std::cout << input << std::endl;
}

void 
Lexer::print_tokens()
{
    std::cout << "Lexer tokens: {\n";
    for (Token token = next_token(); token.type != TokenType::T_EOF; token = next_token()) {
        token.print();
    }
    std::cout << '}' << std::endl;
}

unique_ptr<Lexer>
LexerNew(const string &input)
{
    auto lexer = make_unique<Lexer>(input);
    lexer->read_char();
    return lexer;
}

//------------------------------------------------------------------------------

bool 
ValidIdentifierLetter(const char c)
{
    // A-Z, a-z, _
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
}

