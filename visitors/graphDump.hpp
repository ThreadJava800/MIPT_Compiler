#pragma once

#include <cstdio>

#include "ast.hpp"
#include "visitor.hpp"

class GraphDumper : public Visitor
{
private:
    FILE *dot_file;

public:
    explicit GraphDumper()
        :
            dot_file(nullptr)
    {}

    void visit(const ProgramNode_t &node) override;
    void visit(const VariableNode_t &node) override;
    void visit(const ValueNode_t &node) override;
    void visit(const AndNode_t &node) override;
    void visit(const OrNode_t &node) override;
    void visit(const ComparatorNode_t &node) override;
    void visit(const ArithmeticNode_t &node) override;
    void visit(const NotNode_t &node) override;
    void visit(const NopRuleNode_t &node) override;
    void visit(const AssignNode_t &node) override;
    void visit(const DeclareNode_t &node) override;
    void visit(const PrintNode_t &node) override;
    void visit(const IfNode_t &node) override;
    void visit(const IfElseNode_t &node) override;

    void createGraph(const char *image_name, const ProgramNode_t &root);
};
