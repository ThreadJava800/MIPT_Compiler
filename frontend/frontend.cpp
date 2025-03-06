#include <FlexLexer.h>

#include "log.hpp"
#include "parser.hpp"

yyFlexLexer *flexer = NULL;

int yylex
(
    yy::parser::semantic_type* yylval, 
    yy::parser::location_type* yylloc
) 
{
    if (flexer == NULL)
    {
        DBG_ERR("Invalid resources!\n");
    }

    yylloc->begin.line = flexer->lineno();
    int token = flexer->yylex();
    if(token == yy::parser::token::VAR_NAME || token == yy::parser::token::NUMBER) {
        yylval->build(std::string(flexer->YYText()));
    }

    return token;
}

void yy::parser::error
(
    const location_type& loc, 
    const std::string& msg
)
{
    std::cerr << "Unexpected character in line(" << loc.begin.line << "): " << msg << "'\n";
    exit(-1);
}

bool proceedFrontEnd(std::istream& source_file)
{
    flexer = new yyFlexLexer(&source_file);
    if (flexer == nullptr)
    {
        DBG_ERR("Failed to allocate resources!\n");
        return false;
    }

    yy::parser parser;
    parser.parse();

    delete flexer;
    return true;
}
