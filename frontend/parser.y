%language "c++"
%locations
%define api.value.type variant

%code top {
    #include <cassert>
    #include <cstdlib>
    #include <vector>

    #include "frontend.hpp"
}

%code provides {
    int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%token DECLARE
%token <std::string> VAR_NAME
%token <std::string> NUMBER

%token ASSIGN

%token LBRACE
%token RBRACE
%token LBRACKET
%token RBRACKET
%token SEMICOLON

%token IF
%token ELSE

%token PRINT

%type <const AstNode_t*> var_node
%type <const AstNode_t*> number_node
%type <const AstNode_t*> expr
%type <ProgramNode*> all_expr

%%

program_entry:
    all_expr
    {
        graphDump($1);
    }
;

all_expr:
    %empty
    {
        $$ = new ProgramNode();
    }
|
    all_expr expr
    {
        $1->addChild($2);
        $$ = $1;
    }
;

expr:
    DECLARE var_node SEMICOLON
    {
        $$ = dynamic_cast<const AstNode_t*>(new DeclareNode_t(dynamic_cast<const VariableNode_t*>($2)));
    }
|
    var_node ASSIGN number_node SEMICOLON
    {
        $$ = new AssignNode_t(dynamic_cast<const VariableNode_t*>($1), dynamic_cast<const ValueNode_t*>($3));
    }
;

var_node:
    VAR_NAME
    {
        $$ = dynamic_cast<const AstNode_t*>(new VariableNode_t($1));
    }
;

number_node:
    NUMBER
    {
        $$ = dynamic_cast<const AstNode_t*>(new ValueNode_t(atoi($1.c_str())));
    }
;

%%
