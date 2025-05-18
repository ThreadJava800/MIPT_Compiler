#pragma once

class ProgramNode_t;
class VariableNode_t;
class ValueNode_t;
class AndNode_t;
class OrNode_t;
class ComparatorNode_t;
class ArithmeticNode_t;
class NotNode_t;
class NewScopeNode_t;
class NopRuleNode_t;
class AssignNode_t;
class DeclareNode_t;
class PrintNode_t;
class IfNode_t;
class IfElseNode_t;

class Visitor 
{
public:
    explicit Visitor() = default;

    Visitor(const Visitor&) = delete;
    Visitor &operator=(const Visitor&) = delete;
    Visitor(Visitor&&) = delete;
    Visitor &operator=(Visitor&&) = delete;

    virtual ~Visitor() = default;

    virtual void visit(const ProgramNode_t &node) = 0;
    virtual void visit(const VariableNode_t &node) = 0;
    virtual void visit(const ValueNode_t &node) = 0;
    virtual void visit(const AndNode_t &node) = 0;
    virtual void visit(const OrNode_t &node) = 0;
    virtual void visit(const ComparatorNode_t &node) = 0;
    virtual void visit(const ArithmeticNode_t &node) = 0;
    virtual void visit(const NotNode_t &node) = 0;
    virtual void visit(const NewScopeNode_t &node) = 0;
    virtual void visit(const NopRuleNode_t &node) = 0;
    virtual void visit(const AssignNode_t &node) = 0;
    virtual void visit(const DeclareNode_t &node) = 0;
    virtual void visit(const PrintNode_t &node) = 0;
    virtual void visit(const IfNode_t &node) = 0;
    virtual void visit(const IfElseNode_t &node) = 0;
};
