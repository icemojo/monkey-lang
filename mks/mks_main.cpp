#include <iostream>
#include <format>
#include "mks_lexer.h"


int main(int argc, char **argv)
{
    const std::string input1 = "=+(){},;";
    Lexer *l1 = new Lexer{ input1 };
    PrintLexation(*l1);

    const std::string input2 = R"(
        let x = 5;
        let y = 11;
        let multiplier = 2;

        let add = fn(a, b) {
            a + b;
        }

        let result = add(x, y) * multiplier;
    )";
    std::unique_ptr<Lexer> l2 = LexerNew(input2);
    PrintLexation(*l2);

    return 0;
}
