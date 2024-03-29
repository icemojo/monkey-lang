#include <iostream>
#include <vector>
#include <ranges>
#include "mks_lexer.h"
#include "mks_repl.h"


Options 
ParseOptions(int argc, char **argv)
{
    Options options = {};
    if (argc == 1) {
        return options;
    }

    auto to_string = [](const char *v) { return std::string(v); };
    auto args = std::span(argv, argc) | std::views::transform(to_string);

    for (auto it = args.begin() + 1; it != args.end(); it += 1) {
        if (*it == "-t" || *it == "--token") {
            options.tokenize_only = true;
        }
        if (*it == "-v" || *it == "--verbose") {
            options.verbose = true;
        }
        if (*it == "-h" || *it == "--help") {
            options.show_help = true;
        }
    }

    return options;
}

void 
ReplStart(const Options &options)
{
    std::cout << "Well, hello there! They say enough monkeys with typewriters can finish a novel." << std::endl;
    std::cout << "So, why don't you start your shenanigans?" << std::endl;

    bool should_quit = false;
    std::string input_buffer{};

    while (!should_quit) {
        std::cout << PROMPT;
        std::getline(std::cin, input_buffer);

        if (std::cin.fail()) {
            std::cin.clear();
            break;
        }
        if (input_buffer.empty()) {
            continue;
        }

        std::unique_ptr<Lexer> lexer = LexerNew(input_buffer);
        input_buffer.clear();

        if (options.verbose) {
            lexer->print_tokens();
        }
    }
}
