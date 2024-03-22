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

inline const unordered_map<string_view, TokenType> 
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
TokenNew(TokenType type, char ch);

Token
TokenNew(TokenType type, const string &literal);

TokenType
CheckKeywordOrIdentifier(const string_view &identifier);

//------------------------------------------------------------------------------

struct Lexer {
    string input{};
    int32_t position = 0;       // current position in the input
    int32_t read_position = 0;  // current reading position, 1 char ahead of the current one
    char ch = '\0';

    void read_char();

    char peek_char() const;

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
ValidIdentifierLetter(const char c);

#endif // MKS_LEXER_H_
