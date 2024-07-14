%define api.value.type { sp_cmd_t }

%code requires {
#include <iostream>

#include "bulk.h"

extern int yylex(void);
static void yyerror(const char* s) 
{
    std::cout << s << std::endl;
}

}

%token CMD EF

%%

bulk: CMD      { bulker_c::get_instance().add($1); }
|     bulk CMD { bulker_c::get_instance().add($2); }
|     bulk '{' { bulker_c::get_instance().down(); }
|     bulk '}' { bulker_c::get_instance().up(); }
|     bulk EF  { bulker_c::get_instance().drop(); return 0; }
;

%%

int bulker_c::N = 0;