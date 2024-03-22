#ifndef MKS_REPL_
#define MKS_REPL_

#include <string>
#include <memory>

using std::string;
using std::string_view;
using std::unique_ptr, std::move;


static const string_view PROMPT = ">> ";

struct Options {
    bool tokenize_only = false;
    bool verbose = false;
    bool show_help = false;
};

Options ParseOptions(int argc, char **argv);

void ReplStart(const Options &options);

#endif // MKS_REPL_
