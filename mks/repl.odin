package mks

import "core:os"


Options :: struct {
    tokenize_only:  bool,
    verbose:        bool,
    show_help:      bool,
}

parse_args :: proc() -> Options
{
    options := Options{ false, false, false }
    i := 1
    for i < len(os.args) {
        arg: string = os.args[i]
        if arg == "-t" || arg == "--tokenize" { 
            i += 1
            options.tokenize_only = true
        }
        if arg == "-v" || arg == "--verbose" {
            i += 1
            options.verbose = true
        }
        if arg == "-h" || arg == "--help" {
            i += 1
            options.show_help = true
        }
        i += 1
    }
    return options
}

repl_start :: proc()
{
    //
}

