#ifndef MKS_LEXER_H_
#define MKS_LEXER_H_

#include <string>
#include <vector>
#include <memory>

using byte   = char;
using ubyte  = unsigned char;
using ushort = unsigned short;
using i16    = short;
using i32    = int;
using u16    = unsigned short;
using u32    = unsigned int;
using i64    = long long;
using u64    = unsigned long long;


enum class TokenType {
    ILLEGAL,
    T_EOF,

    // User-defined identifiers + literals
    IDENT,      // add, foobar, x, y,...
    INT,        // 1234, -42

    // Operators
    ASSIGN,     // =
    PLUS,       // +

    // Delimiters
    COMMA,      // ,
    SEMICOLON,  // ;

    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }

    // Keywords
    LET,        // let
    FUNCTION,   // fn
};

struct Token {
    TokenType type = TokenType::ILLEGAL;
    std::string literal{};
};

Token
TokenNew(TokenType type, byte ch);

TokenType
CheckKeywordOrIdentifier(const std::string identifier);


struct Lexer {
    std::string input{};
    i32 position = 0;       // current position in the input
    i32 read_position = 0;  // current reading position, 1 char ahead of the current one
    byte ch = '\0';

    void read_char();

    Token next_token();

    std::string read_identifier();

    std::string read_number();

    void skip_whitespace();
};

std::unique_ptr<Lexer>
LexerNew(const std::string &input);

void
PrintLexation(const Lexer &lexer);

bool
ValidIdentifierLetter(const byte c);

#endif // MKS_LEXER_H_
