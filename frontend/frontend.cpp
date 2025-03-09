#include <cstdio>
#include <FlexLexer.h>
#include <string>

#include "log.hpp"
#include "frontend.hpp"
#include "parser.hpp"

static std::map<std::string, AstValue_t> variables;

yyFlexLexer *flexer = NULL;

int yylex
(
    yy::parser::semantic_type* yylval, 
    yy::parser::location_type* yylloc
) 
{
    if (flexer == NULL)
    {
        DEV_DBG_ERR("Invalid resources!\n");
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
    USER_ABORT("Unexpected character in line(%d): %s\n", loc.begin.line, msg.c_str());
}

AstValue_t Driver_t::getVariableValue(std::string var_name)
{
    if (variables.count(var_name) != 0)
    {
        return variables[var_name];
    }
    else
    {
        USER_ABORT("Variable (%s) was not created!\n", var_name.c_str());
    }
    return 0;   // should not reach this code
}

void Driver_t::createVariable(std::string var_name)
{
    variables[var_name] = 0;
}

void Driver_t::setVariableValue(std::string var_name, const AstValue_t value)
{
    if (variables.count(var_name) != 0)
    {
        variables[var_name] = value;
    }
    else
    {
        USER_ABORT("Variable (%s) was not created!\n", var_name.c_str());
    }
}

void Driver_t::graphNode(const AstNode_t *node, FILE *tempFile)
{
    fprintf(
                tempFile, 
                "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
                node
            );

    node->graphDumpLabel(tempFile);
    
    fprintf(tempFile, "}\"];\n");

    for (const auto child : node->children_vec)
    {
        graphNode(child, tempFile);
    }
    for (const auto child : node->children_vec)
    {
        fprintf(tempFile, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", node, child);
    }
}

void Driver_t::graphDump(const char *image_name)
{
    if (!root) {
        DEV_DBG_ERR("root == NULL\n");
        return;
    }

    FILE *tempFile = fopen("temp.dot", "w");
    fprintf(tempFile, "digraph tree {\n");
    fprintf(tempFile, "\trankdir=HR;\n");
    if (!tempFile) 
    {
        DEV_DBG_ERR("failed to create .dot file!\n");
        return;
    }

    graphNode(root, tempFile);

    fprintf(tempFile, "}");
    fclose(tempFile);

    char command[1024] = {0};
    snprintf(command, sizeof(command), "dot -Tsvg temp.dot > %s && xdg-open %s", image_name, image_name);

    system(command);
    system("rm temp.dot");
}

bool Driver_t::proceedFrontEnd(std::istream& source_file)
{
    flexer = new yyFlexLexer(&source_file);
    if (flexer == nullptr)
    {
        DEV_DBG_ERR("Failed to allocate resources!\n");
        return false;
    }

    yy::parser parser(*this);
    parser.parse();

    delete flexer;
    return true;
}

void Driver_t::interpret()
{
    root->interpret();
}
