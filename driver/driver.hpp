#pragma once

#include <fstream>
#include <map>
#include <string>

#include "ast.hpp"
#include "graphDump.hpp"
#include "interpreter.hpp"
#include "llvmIR.hpp"

class Driver_t
{
public:
    ProgramNode_t *root;
    Interpreter interpreter;
    GraphDumper graph_dumper;
    LLVMBuilder llvm_builder;

public:
    explicit Driver_t()
        :
            root(new ProgramNode_t())
        {}

    Driver_t(const Driver_t&) = delete;
    Driver_t &operator=(const Driver_t&) = delete;
    Driver_t(Driver_t&&) = delete;
    Driver_t &operator=(Driver_t&&) = delete;

    bool proceedFrontEnd(std::istream& source_file);
    void interpret();
    void graphDump(const char *image_name);
    void generateLLVMIR(const char *output_file);

    ~Driver_t()
    {
        delete root;
    }
};
