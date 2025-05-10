%language "c++"
%locations
%define api.value.type variant

%code top {
    #include "driver.hpp"
}

%code provides {
    int yylex(
        yy::parser::semantic_type* yylval,
        yy::parser::location_type* yylloc
        );
}

%parse-param { Driver_t &driver }

%token DECLARE
%token FUNC_DECL
%token MAIN_DECL
%token CALL_FUNC
%token RETURN

%token TYPE_INT
%token TYPE_VOID

%token <std::string> VAR_NAME
%token <std::string> NUMBER

%precedence NOT

%left ADD
%left SUB
%left MUL
%left DIV
%left EQUALS
%left LESS
%left LESS_OR_EQ
%left MORE
%left MORE_OR_EQ
%left OR
%left AND

%token ASSIGN

%token LBRACE
%token RBRACE
%token LBRACKET
%token RBRACKET
%token SEMICOLON

%token IF
%token ELSE
%token PRINT

%type <const VariableNode_t*> var_node
%type <const ValueNode_t*> number_node
%type <const RuleNode_t*> expr
%type <std::vector<const RuleNode_t*>> many_expr
%type <const FunctionNode_t*> main_func
%type <std::pair<std::string, const FunctionNode_t*>> func
%type <std::map<std::string, const FunctionNode_t*>> all_func
%type <std::vector<const NonTerminalNode_t*>> many_pass_parameters
%type <std::vector<const VariableNode_t*>> many_parameters
%type <const NonTerminalNode_t*> ast_node_leaf
%type <const NonTerminalNode_t*> ast_logic_node
%type <const NonTerminalNode_t*> ast_node_and_or
%type <const NonTerminalNode_t*> ast_node_compare
%type <const NonTerminalNode_t*> ast_node_add
%type <const NonTerminalNode_t*> ast_node_mul
%type <const NonTerminalNode_t*> ast_node_brackets
%type <const NonTerminalNode_t*> ast_node_not

%%

entry_rule:
    all_func main_func
    {
        $1["main"] = $2;
        driver.root = new ProgramNode_t(new CallFuncNode_t("main", {}), $1);
    }
;

types:
    TYPE_INT
    {

    }
;

many_pass_parameters:
    %empty
    {
        $$ = std::vector<const NonTerminalNode_t*>();
    }
|
    many_pass_parameters ast_logic_node
    {
        $1.push_back($2);
        $$ = $1;
    }
;

expr:
    CALL_FUNC VAR_NAME LBRACKET many_pass_parameters RBRACKET SEMICOLON
    {
        $$ = new CallFuncNode_t($2, $4);
    }
|
    RETURN SEMICOLON
    {
        $$ = new ReturnNode_t(std::nullopt);
    }
|
    RETURN ast_logic_node SEMICOLON
    {
        $$ = new ReturnNode_t($2);
    }
|
    PRINT LBRACKET ast_logic_node RBRACKET SEMICOLON
    {
        $$ = new PrintNode_t($3);
    }
|
    IF LBRACKET ast_logic_node RBRACKET LBRACE expr RBRACE
    {
        $$ = new IfNode_t($3, $6);
    }
|
    IF LBRACKET ast_logic_node RBRACKET LBRACE expr RBRACE ELSE LBRACE expr RBRACE
    {
        $$ = new IfElseNode_t($3, $6, $10);
    }
|
    DECLARE types VAR_NAME SEMICOLON
    {
        $$ = new DeclareNode_t($3);
    }
|
    DECLARE types VAR_NAME ASSIGN ast_logic_node SEMICOLON
    {
        $$ = new NopRuleNode_t(new DeclareNode_t($3), new AssignNode_t($3, $5));
    }
|
    VAR_NAME ASSIGN ast_logic_node SEMICOLON
    {
        $$ = new AssignNode_t($1, $3);
    } 
;

many_expr:
    %empty
    {
        $$ = std::vector<const RuleNode_t*>();
    }
|
    many_expr expr
    {
        $1.push_back($2);
        $$ = $1;
    }
;

many_parameters:
    %empty
    {
        $$ = std::vector<const VariableNode_t*>();
    }
|
    many_parameters types VAR_NAME
    {
        $1.push_back(new VariableNode_t($3));
        $$ = $1;
    }
;

func:
    FUNC_DECL TYPE_VOID VAR_NAME LBRACKET many_parameters RBRACKET LBRACE many_expr RBRACE
    {
        $$ = { $3, new FunctionNode_t($8, $5) };
    }
|
    FUNC_DECL types VAR_NAME LBRACKET many_parameters RBRACKET LBRACE many_expr RBRACE
    {
        $$ = { $3, new FunctionNode_t($8, $5) };
    }
;

all_func:
    %empty
    {
        $$ = std::map<std::string, const FunctionNode_t*>();
    }
|
    all_func func
    {
        $1[$2.first] = $2.second;
        $$ = $1;
    }
;

main_func:
    FUNC_DECL TYPE_VOID MAIN_DECL LBRACKET RBRACKET LBRACE many_expr RBRACE
    {
        $$ = new FunctionNode_t($7, {});
    }
;

ast_logic_node:
    ast_node_and_or
    {
        $$ = $1;
    }
;

ast_node_and_or:
    ast_node_compare
    {
        $$ = $1;
    }
|
    ast_logic_node AND ast_logic_node
    {
        $$ = new AndNode_t($1, $3);
    }
|
    ast_logic_node OR ast_logic_node
    {
        $$ = new OrNode_t($1, $3);
    }
;

ast_node_compare:
    ast_node_add
    {
        $$ = $1;
    }
|
    ast_node_add LESS ast_node_add
    {
        $$ = new ComparatorNode_t(ComparatorOperators::LESS, $1, $3);
    }
|
    ast_node_add LESS_OR_EQ ast_node_add
    {
        $$ = new ComparatorNode_t(ComparatorOperators::LESS_OR_EQ, $1, $3);
    }
|
    ast_node_add MORE ast_node_add
    {
        $$ = new ComparatorNode_t(ComparatorOperators::MORE, $1, $3);
    }
|
    ast_node_add MORE_OR_EQ ast_node_add
    {
        $$ = new ComparatorNode_t(ComparatorOperators::MORE_OR_EQ, $1, $3);
    }
|
    ast_node_add EQUALS ast_node_add
    {
        $$ = new ComparatorNode_t(ComparatorOperators::EQ, $1, $3);
    }
;

ast_node_add:
    ast_node_mul
    {
        $$ = $1;
    }
|
    ast_node_mul ADD ast_node_mul
    {
        $$ = new ArithmeticNode_t(ArithmeticOperators::ADD, $1, $3);
    }
|
    ast_node_mul SUB ast_node_mul
    {
        $$ = new ArithmeticNode_t(ArithmeticOperators::SUB, $1, $3);
    }
;

ast_node_mul:
    ast_node_brackets
    {
        $$ = $1;
    }
|
    ast_node_brackets MUL ast_node_brackets
    {
        $$ = new ArithmeticNode_t(ArithmeticOperators::MUL, $1, $3);
    }
|
    ast_node_brackets DIV ast_node_brackets
    {
        $$ = new ArithmeticNode_t(ArithmeticOperators::DIV, $1, $3);
    }
;

ast_node_brackets:
    ast_node_not
    {
        $$ = $1;
    }
|
    LBRACKET ast_logic_node RBRACKET
    {
        $$ = $2;
    }
;

ast_node_not:
    ast_node_leaf
    {
        $$ = $1;
    }
|
    NOT ast_node_brackets
    {
        $$ = new NotNode_t($2);
    }
;

ast_node_leaf:
    var_node
    {
        $$ = $1;
    }
|
    number_node
    {
        $$ = $1;
    }
;

var_node:
    VAR_NAME
    {
        $$ = new VariableNode_t($1);
    }
;

number_node:
    NUMBER
    {
        $$ = new ValueNode_t(atoi($1.c_str()));
    }
;

%%
