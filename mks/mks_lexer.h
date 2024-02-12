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

    // Identifiers + literals
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


struct Lexer {
    std::string input{};
    i32 position = 0;
    i32 read_position = 0;
    byte ch = '\0';
};

std::unique_ptr<Lexer>
LexerNew(const std::string &input);

void
LexerReadChar(Lexer &lexer);

Token
LexerNextToken(const Lexer &lexer);

#endif // MKS_LEXER_H_
