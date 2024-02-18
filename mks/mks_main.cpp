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
            if (a == b) {
                a * 2;
            }
            else {
                a + b;
            }
        }

        let result = add(x, y) * multiplier;
        let result2 = result / 2;
        !-+/<12> * 1024;

        if (x > y) {
            return true;
        }
        else {
            return false;
        }
        x != y;

        12 < 25 > 7;
    )";
    std::unique_ptr<Lexer> l2 = LexerNew(input2);
    PrintLexation(*l2);

    return 0;
}
