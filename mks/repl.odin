package mks

import "core:os"
import "core:fmt"


Options :: struct {
    tokenize_only:  bool,
    verbose:        bool,
    show_help:      bool,
}

parse_args :: proc() -> Options
{
    options := Options{ false, false, false }
    for i := 1; i < len(os.args); {
        arg: string = os.args[i]
        if arg == "-t" || arg == "--tokenize" { 
            options.tokenize_only = true
        }
        if arg == "-v" || arg == "--verbose" {
            options.verbose = true
        }
        if arg == "-h" || arg == "--help" {
            options.show_help = true
        }
        i += 1
    }
    return options
}

repl_start :: proc(options: ^Options)
{
    fmt.println("Well, hello there! They say enough monkeys with typewriters can finish a novel.")
    fmt.println("So, why don't you start your shenanigans?")

    if options.verbose {
        fmt.println("(Verbose mode turned on)")
    }

    BUFFER_SIZE  :: 256
    input_buffer :[BUFFER_SIZE]u8

    should_quit  :      = false
    lexer        :Lexer = {}

    for !should_quit {
        fmt.print(">> ")
        total_read, err := os.read(os.stdin, input_buffer[:])
        if err != 0 {
            // TODO(yemon): Handle read error from stdin
            fmt.println("ERR: Unable to read input.")
            should_quit = true
            return
        }

        input_source := cast(string)input_buffer[:total_read]
        lexer = lexer_new(input_source)
        if options.verbose {
            lexer_print_tokens(&lexer)
        }

        input_source = ""
        for i in 0..<BUFFER_SIZE {
            input_buffer[i] = 0
        }
    }
}

