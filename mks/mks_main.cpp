#include <iostream>
#include <format>
#include "mks_lexer.h"


int main(int argc, char **argv)
{
    const std::string input1 = "=+(){},;";
    std::unique_ptr<Lexer> l1 = LexerNew(input1);

    const std::string input2 = "let x = 1 + 2;";
    Lexer *l2 = new Lexer{ input2 };
    l2->read_char();
    l2->position = 12;

    std::cout << "Lexer1: " << l1->input << " " << l1->position << '\n';
    l1->read_char();

    std::cout << "Lexer2: " << l2->input << " " << l2->position << '\n';
    l2->read_char();

    return 0;
}
