#pragma once

#include "ast.hpp"

class Visitor 
{
public:
    virtual void visit(const ProgramNode_t& node) = 0;
    virtual void visit(const VariableNode_t& node) = 0;
    virtual void visit(const ValueNode_t& node) = 0;
    virtual void visit(const AndNode_t& node) = 0;
    virtual void visit(const OrNode_t& node) = 0;
    virtual void visit(const ComparatorNode_t& node) = 0;
    virtual void visit(const ArithmeticNode_t& node) = 0;
    virtual void visit(const NotNode_t& node) = 0;
    virtual void visit(const NopRuleNode_t& node) = 0;
    virtual void visit(const AssignNode_t& node) = 0;
    virtual void visit(const DeclareNode_t& node) = 0;
    virtual void visit(const PrintNode_t& node) = 0;
    virtual void visit(const IfNode_t& node) = 0;
    virtual void visit(const IfElseNode_t& node) = 0;
};
