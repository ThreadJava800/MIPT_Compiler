%option c++ noyywrap yylineno

%{
    #include "driver.hpp"
    #include "parser.hpp"
%}

%%

[ \t\n]+                    // skip

if                          return yy::parser::token::IF;
else                        return yy::parser::token::ELSE;
print                       return yy::parser::token::PRINT;
declare                     return yy::parser::token::DECLARE;
function                    return yy::parser::token::FUNC_DECL;
main                        return yy::parser::token::MAIN_DECL;
call                        return yy::parser::token::CALL_FUNC;
return                      return yy::parser::token::RETURN;

int                         return yy::parser::token::TYPE_INT;
void                        return yy::parser::token::TYPE_VOID;

"="                         return yy::parser::token::ASSIGN;
"+"                         return yy::parser::token::ADD;
"-"                         return yy::parser::token::SUB;
"*"                         return yy::parser::token::MUL;
"/"                         return yy::parser::token::DIV;
"=="                        return yy::parser::token::EQUALS;
"<"                         return yy::parser::token::LESS;
"<="                        return yy::parser::token::LESS_OR_EQ;
">"                         return yy::parser::token::MORE;
">="                        return yy::parser::token::MORE_OR_EQ;
"!"                         return yy::parser::token::NOT;
"&&"                        return yy::parser::token::AND;
"||"                        return yy::parser::token::OR;

[a-zA-Z_][a-zA-Z_0-9_]*     return yy::parser::token::VAR_NAME;
[-]?[0-9]+                  return yy::parser::token::NUMBER;

"("                         return yy::parser::token::LBRACKET;
")"                         return yy::parser::token::RBRACKET;
"{"                         return yy::parser::token::LBRACE;
"}"                         return yy::parser::token::RBRACE;
";"                         return yy::parser::token::SEMICOLON;

%%
