%option c++ noyywrap yylineno

%{
    #include <iostream>

    #include "frontend.hpp"
    #include "parser.hpp"
%}

%%

[ \t\n]+                    // skip

declare                     return yy::parser::token::DECLARE;
[a-zA-Z_][a-zA-Z_0-9_]*     return yy::parser::token::VAR_NAME;
[0-9]+                      return yy::parser::token::NUMBER;

"="                         return yy::parser::token::ASSIGN;

"("                         return yy::parser::token::LBRACKET;
")"                         return yy::parser::token::RBRACKET;
"{"                         return yy::parser::token::LBRACE;
"}"                         return yy::parser::token::RBRACE;
";"                         return yy::parser::token::SEMICOLON;

if                          return yy::parser::token::IF;
else                        return yy::parser::token::ELSE;

print                       return yy::parser::token::PRINT;

%%
