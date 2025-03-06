%option c++ noyywrap yylineno

%{
    #include <iostream>

    #include "frontend.hpp"
    #include "parser.hpp"
%}

%%

program                     return yy::parser::token::PROGRAM;

%%
