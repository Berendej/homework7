#include <unistd.h> /* for getopt */
#include "parser.h"

int main(int c, char *args[])
{
    bool param_ok = false;
    char *cval;
    int n = getopt(c, args, "n:");
    if ('n' == n )
    {
        param_ok = bulker_c::set_n(std::stoi(optarg));
    }
    if (not param_ok)
    {
        bulker_c::usage();
        return -1;
    }
    yyparse();
    return 0;
}