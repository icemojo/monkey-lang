package mks

import "core:fmt"


main :: proc() 
{
    options := parse_args()
    repl_start(&options)
}

