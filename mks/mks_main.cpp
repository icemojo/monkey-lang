#include <iostream>
#include <format>
#include "mks_lexer.h"
#include "mks_repl.h"


int main(int argc, char **argv)
{
    Options options = ParseOptions(argc, argv);
    ReplStart(options);

    return 0;
}
