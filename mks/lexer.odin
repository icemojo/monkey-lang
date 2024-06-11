package mks

import "core:fmt"
import "core:strings"


Token_Type :: enum {
    ILLEGAL,
    EOF,

    // User-defined identifiers + literals
    IDENT,
    INT,

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
    LET,
    FUNCTION,
    RETURN,
    IF, 
    ELSE,
    TRUE,
    FALSE,
}

check_keyword_or_identifier :: proc(identifier_name: string) -> Token_Type 
{
    // NOTE(yemon): Not sure how to make this lookup table a bit more efficient.
    // Right now, it'll just locally create a map everytime this function gets called.
    keywords := map[string]Token_Type {
        "let"    = Token_Type.LET,
        "fn"     = Token_Type.FUNCTION,
        "return" = Token_Type.RETURN,
        "if"     = Token_Type.IF,
        "else"   = Token_Type.ELSE,
        "true"   = Token_Type.TRUE,
        "false"  = Token_Type.FALSE,
    }
    if token_type, ok := keywords[identifier_name]; ok {
        return token_type
    }
    else {
        return .IDENT
    }
}

Token :: struct {
    type    :Token_Type,
    literal :string,
}

token_new_char :: proc(token_type: Token_Type, char: u8) -> Token
{
    char := char
    return Token{
        type = token_type,
        literal = strings.clone_from_ptr(&char, 1),
    }
}

token_new_str :: proc(token_type: Token_Type, str: string) -> Token
{
    return Token{
        type = token_type,
        literal = str,
    }
}

token_new :: proc {
    token_new_char,
    token_new_str,
}

//------------------------------------------------------------------------------

Lexer :: struct {
    input         :string,
    position      :int,    // current position in 'input'
    read_position :int,    // current reading position in 'input', right after current char
    ch            :u8,
}

lexer_new :: proc(input: string) -> Lexer 
{
    lexer := Lexer{ input = input }
    lexer_read_char(&lexer)
    return lexer
}

lexer_read_char :: proc(using lexer: ^Lexer)
{
    if read_position >= len(input) {
        ch = 0
    }
    else {
        ch = input[read_position]
    }
    position = read_position
    read_position += 1
}

lexer_peek_char :: proc(using lexer: ^Lexer) -> u8
{
    if read_position >= len(input) {
        return 0
    }
    else {
        return input[read_position]
    }
}

lexer_skip_whitespaces :: proc(using lexer: ^Lexer) 
{
    for ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' {
        lexer_read_char(lexer)
    }
}

lexer_next_token :: proc(using lexer: ^Lexer) -> Token
{
    token := Token{}
    lexer_skip_whitespaces(lexer)

    switch ch {
        case '=': {
            if nch := lexer_peek_char(lexer); nch == '=' {
                token = token_new(.EQ, "==")
                lexer_read_char(lexer)
            }
            else {
                token = token_new(.ASSIGN, ch)
            }
        }
        case '!': {
            if nch := lexer_peek_char(lexer); nch == '=' {
                token = token_new(.NOT_EQ, "!=")
                lexer_read_char(lexer)
            }
            else {
                token = token_new(.BANG, ch)
            }
        }

        case '+': {
            token = token_new(.PLUS, ch)
        }
        case '-': {
            token = token_new(.MINUS, ch)
        }
        case '*': {
            token = token_new(.ASTRISK, ch)
        }
        case '/': {
            token = token_new(.SLASH, ch)
        }

        case '<': {
            if nch := lexer_peek_char(lexer); nch == '=' {
                token = token_new(.LT_EQ, "<=")
                lexer_read_char(lexer)
            }
            else {
                token = token_new(.LT, ch)
            }
        }
        case '>': {
            if nch := lexer_peek_char(lexer); nch == '=' {
                token = token_new(.GT_EQ, ">=")
                lexer_read_char(lexer)
            }
            else {
                token = token_new(.GT, ch)
            }
        }
        case ';': {
            token = token_new(.SEMICOLON, ch)
        }
        case ',': {
            token = token_new(.COMMA, ch)
        }
        case '(': {
            token = token_new(.LPAREN, ch)
        }
        case ')': {
            token = token_new(.RPAREN, ch)
        }
        case '{': {
            token = token_new(.LBRACE, ch)
        }
        case '}': {
            token = token_new(.RBRACE, ch)
        }

        case 0: {   // '\0'
            token = token_new(.EOF, 0)
        }

        case: {
            if is_alpha(ch) {
                token.literal = lexer_read_identifier(lexer)
                token.type    = check_keyword_or_identifier(token.literal)
                return token
            }
            else if is_number(ch) {
                token.literal = lexer_read_number(lexer)
                token.type    = .INT
                return token
            }
            else {
                token = token_new(.ILLEGAL, ch)
                break
            }
        }
    }

    lexer_read_char(lexer)
    return token
}

lexer_read_identifier :: proc(using lexer: ^Lexer) -> string
{
    start_pos := position
    for i in position..<len(input) {
        if is_alpha(input[i]) {
            lexer_read_char(lexer)
            continue
        }
        else {
            identifier := input[start_pos:i]
            return identifier
        }
    }

    return ""
}

lexer_read_number :: proc(using lexer: ^Lexer) -> string
{
    start_pos := position
    end_pos   := position
    for i in position..<len(input) {
        if !is_number(input[i]) {
            break
        }
        end_pos += 1
        lexer_read_char(lexer)
    }

    number := input[start_pos:end_pos]
    return number
}

lexer_print_tokens :: proc(using lexer: ^Lexer) 
{
    fmt.println("Lexer tokens: {")
    for token := lexer_next_token(lexer); token.type != .EOF; token = lexer_next_token(lexer) {
        fmt.printf("    %v\n", token)
    }
    fmt.println("}")
}

//------------------------------------------------------------------------------

is_alpha :: proc(ch: u8) -> bool
{
    switch ch {
        case 'A'..='Z', 'a'..='z', '_':
            return true
        case:
            return false
    }
}

is_number :: proc(ch: u8) -> bool
{
    switch ch {
        case '0'..='9':
            return true
        case:
            return false
    }
}

