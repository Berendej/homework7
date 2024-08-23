%option noyywrap

%{
#include "parser.h"
#include <thread>
#include <chrono>

void do_pause()
{
    using namespace std::chrono_literals;
     std::this_thread::sleep_for(1000ms);
}

%}

%%

cmd[0-9]+      { yylval = fake_cmd_c::build_cmd(yytext); do_pause(); return CMD; }
"{"            { return '{'; }
"}"            { return '}'; }
"\n"           { /* eat anything else */ }
<<EOF>>        {  return EF; }
.              { /* eat anything else */ }

%%