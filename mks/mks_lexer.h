#ifndef MKS_LEXER_H_
#define MKS_LEXER_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

using std::string;
using std::string_view;
using std::vector;
using std::unordered_map;
using std::unique_ptr, std::make_unique;

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
    BANG,       // !
    PLUS,       // +
    MINUS,      // -
    ASTRISK,    // *
    SLASH,      // /
    LT,         // <
    GT,         // >
    LT_EQ,      // <=
    GT_EQ,      // >=
    EQ,         // ==
    NOT_EQ,     // !=

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
    RETURN,     // return
    IF,         // if
    ELSE,       // else
    TRUE,       // true
    FALSE,      // false
};

constexpr string
TokenTypeToString(TokenType type);

std::ostream &
operator<<(std::ostream &out, const TokenType token_type);

//------------------------------------------------------------------------------

static const unordered_map<string_view, TokenType> 
KEYWORDS = {
    {"let", TokenType::LET},
    {"fn", TokenType::FUNCTION},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
};

struct Token {
    TokenType type = TokenType::ILLEGAL;
    string literal{};

    void print() const;
};

Token
TokenNew(TokenType type, byte ch);

Token
TokenNew(TokenType type, const string &literal);

TokenType
CheckKeywordOrIdentifier(const string_view &identifier);

//------------------------------------------------------------------------------

struct Lexer {
    string input{};
    i32 position = 0;       // current position in the input
    i32 read_position = 0;  // current reading position, 1 char ahead of the current one
    byte ch = '\0';

    void read_char();

    byte peek_char() const;

    Token next_token();

    string read_identifier();

    string read_number();

    void skip_whitespaces();

    void print() const;

    void print_tokens();
};

unique_ptr<Lexer>
LexerNew(const string &input);

//------------------------------------------------------------------------------

bool
ValidIdentifierLetter(const byte c);

#endif // MKS_LEXER_H_
