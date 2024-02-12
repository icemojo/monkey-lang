#include <iostream>
#include <format>
#include "mks_lexer.h"

int main(int argc, char **argv)
{
    const std::string input1 = "=+(){},;";
    std::unique_ptr<Lexer> l1 = LexerNew(input1);

    const std::string input2 = "let x = 1 + 2;";
    Lexer *l2 = new Lexer{ input2 };
    l2->position = 12;

    std::cout << "Lexer1: " << l1->input << " " << l1->position << '\n';
    LexerReadChar(*l1);

    std::cout << "Lexer2: " << l2->input << " " << l2->position << '\n';
    LexerReadChar(*l2);

    return 0;
}
