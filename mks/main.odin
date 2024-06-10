package mks

import "core:fmt"


main :: proc() 
{
    lexer := lexer_new("let x = 12;")
    fmt.println(lexer)
}

