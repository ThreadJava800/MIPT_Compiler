#include <cstdio>
#include <FlexLexer.h>
#include <string>

#include "driver.hpp"
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
    DEV_ASSERT(root == nullptr);

    interpreter.setFunctionsMap(root->getFunctions());
    root->accept(interpreter);
}

void Driver_t::graphDump(const char *image_name)
{
    DEV_ASSERT(image_name == nullptr);
    DEV_ASSERT(root == nullptr);

    graph_dumper.createGraph(image_name, *root);
}

void Driver_t::generateLLVMIR(const char *output_file)
{
    DEV_ASSERT(output_file == nullptr);
    DEV_ASSERT(root == nullptr);

    llvm_builder.generateLLVMIR(output_file, *root);
}
