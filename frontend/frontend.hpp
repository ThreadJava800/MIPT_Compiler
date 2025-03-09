#pragma once

#include <fstream>
#include <map>
#include <string>

#include "ast.hpp"

class Driver_t
{
public:
    ProgramNode_t *root;

public:
    explicit Driver_t()
        :
            root(new ProgramNode_t())
        {}

    Driver_t(const Driver_t&) = delete;
    Driver_t &operator=(const Driver_t&) = delete;
    Driver_t(Driver_t&&) = delete;
    Driver_t &operator=(Driver_t&&) = delete;

    void graphDump();
    bool proceedFrontEnd(std::istream& source_file);
    void interpret();

    ~Driver_t()
    {
        delete root;
    }

    static AstValue_t getVariableValue(std::string var_name);
    static void createVariable(std::string var_name);
    static void setVariableValue(std::string var_name, const AstValue_t value);

private:
    void graphNode(const AstNode_t *node, FILE *tempFile);
};
