#define BOOST_TEST_MODULE homework7 test

#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <iostream>
#include <cstring>

#include "parser.h"
#include "scan.h"

const char *input = 
"cmd1\n"
"cmd2\n"
"{\n"
"    cmd3\n"
"    {\n"
"        cmd4\n"
"    }\n"
"    cmd5\n"
"}\n"
"cmd6";

const char * expected_output = 
"bulk: cmd1, cmd2\n"
"bulk: cmd3, cmd4, cmd5\n"
"bulk: cmd6\n";

bool test_bulk()
{
    bulker_c::set_n(3);
    // redirect input to our input string
    yy_scan_string(input);
    // redirect cout to string stream
    // save original cout 
    std::streambuf *coutbuf_ = std::cout.rdbuf();
    std::ostringstream out_;
    std::cout.rdbuf(out_.rdbuf());
    
    // do the work
    yyparse();

    yylex_destroy();
    std::string result = out_.str();
    // restore original cout
    std::cout.rdbuf(coutbuf_);  
    
    // compare 
    std::string etalon(expected_output);
    return etalon.compare(result) == 0;
}

BOOST_AUTO_TEST_CASE(bulk_test)
{
    BOOST_CHECK( test_bulk()  );
}
