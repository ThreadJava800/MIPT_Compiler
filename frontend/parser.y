%language "c++"
%locations
%define api.value.type variant

%code top {
    #include <cassert>
    #include <cstdlib>
    #include <vector>

    #include "ast.hpp"
}

%code provides {
    int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%token PROGRAM
%token <std::string> VAR_NAME
%token <std::string> NUMBER

%%
program_entry:
{

}
;
%%
