#include <FlexLexer.h>
#include <cstdio>

#include "log.hpp"
#include "frontend.hpp"
#include "parser.hpp"

yyFlexLexer *flexer = NULL;

static void graphNode(const AstNode_t *node, FILE *tempFile) {
    fprintf(
                tempFile, 
                "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
                node
            );

    node->printFunc(tempFile);
    
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

void graphDump(const ProgramNode *node) {
    if (!node) return;

    FILE *tempFile = fopen("temp.dot", "w");
    fprintf(tempFile, "digraph tree {\n");
    fprintf(tempFile, "\trankdir=HR;\n");
    if (!tempFile) return;

    graphNode(node, tempFile);

    fprintf(tempFile, "}");
    fclose(tempFile);

    system("dot -Tsvg temp.dot > graph.png && xdg-open graph.png");
}

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
