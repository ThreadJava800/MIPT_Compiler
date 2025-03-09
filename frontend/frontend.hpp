#pragma once

#include <fstream>
#include <vector>

#include "ast.hpp"

bool proceedFrontEnd(std::istream& source_file);
void graphDump(const ProgramNode *node);

class Driver_t
{
private:
    std::vector<AstValue_t> variables;
    const AstNode_t *root;

public:
    explicit Driver_t()
        :
            root(new AstNode_t())
        {}
};
