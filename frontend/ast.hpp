#pragma once

#include <cstdarg>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// #include "visitor.hpp"

using AstValue_t = int64_t;

class AstNode_t
{
public:
    explicit AstNode_t() = default;

    AstNode_t(const AstNode_t&) = delete;
    AstNode_t &operator=(const AstNode_t&) = delete;
    AstNode_t(AstNode_t&&) = delete;
    AstNode_t &operator=(AstNode_t&&) = delete;

    virtual ~AstNode_t() {}

    // virtual void accept(Visitor& visitor) const
    // {
    //     visitor.visit(*this);
    // }
};

class ProgramNode_t : public AstNode_t
{
private:
    std::vector<const AstNode_t*> children_vec;

public:
    explicit ProgramNode_t() = default;

    void addChild(const AstNode_t *child)
    {
        children_vec.push_back(child);
    }

    ~ProgramNode_t()
    {
        for (const auto child : children_vec)
        {
            delete child;
        }
    }
};

class NonTerminalNode_t : public AstNode_t
{
public:
    explicit NonTerminalNode_t() = default;
    virtual ~NonTerminalNode_t() = default;
};

class VariableNode_t : public NonTerminalNode_t
{
protected:
    std::string name;

public:
    explicit VariableNode_t(const std::string name_)
        :
            name(std::move(name_))
    {}
};

class ValueNode_t : public NonTerminalNode_t
{
protected:
    const AstValue_t value;

public:
    explicit ValueNode_t(const AstValue_t value_)
        :
            NonTerminalNode_t(),
            value(std::move(value_))
    {}
};

class AndNode_t : public NonTerminalNode_t
{
private:
    const NonTerminalNode_t *left;
    const NonTerminalNode_t *right;

public:
    explicit AndNode_t(
        const NonTerminalNode_t *left_,
        const NonTerminalNode_t *right_
        )
        :
            left(left_),
            right(right_)
    {}
};

class OrNode_t : public NonTerminalNode_t
{
private:
    const NonTerminalNode_t *left;
    const NonTerminalNode_t *right;

public:
    explicit OrNode_t(
        const NonTerminalNode_t *left_,
        const NonTerminalNode_t *right_
        )
        :
            left(left_),
            right(right_)
    {}
};

enum class ComparatorOperators
{
    LESS,
    LESS_OR_EQ,
    MORE,
    MORE_OR_EQ,
    EQ
};

class ComparatorNode_t : public NonTerminalNode_t
{
private:
    const NonTerminalNode_t *left;
    const NonTerminalNode_t *right;
    const ComparatorOperators oper;

public:
    explicit ComparatorNode_t(
        const ComparatorOperators oper_,
        const NonTerminalNode_t *left_,
        const NonTerminalNode_t *right_
        )
        :
            left(left_),
            right(right_),
            oper(oper_)
    {}
};

enum class ArithmeticOperators
{
    ADD,
    SUB,
    MUL,
    DIV
};

class ArithmeticNode_t : public NonTerminalNode_t
{
private:
    const NonTerminalNode_t *left;
    const NonTerminalNode_t *right;
    const ArithmeticOperators oper;

public:
    explicit ArithmeticNode_t(
        const ArithmeticOperators oper_,
        const NonTerminalNode_t *left_,
        const NonTerminalNode_t *right_
        )
        :
            left(left_),
            right(right_),
            oper(oper_)
    {}
};

class NotNode_t : public NonTerminalNode_t
{
private:
    const NonTerminalNode_t *child;

public:
    explicit NotNode_t(const NonTerminalNode_t *child_)
        :
            child(child_)
    {}
};

class RuleNode_t : public AstNode_t
{
public:
    explicit RuleNode_t() = default;
    virtual ~RuleNode_t() = default;
};

class NopRuleNode_t : public RuleNode_t
{
private:
    std::vector<const AstNode_t*> children_vec;

public:
    template<typename... Args>
    explicit NopRuleNode_t(Args... children)
    {
        (children_vec.push_back(children), ...);
    }
};

class AssignNode_t : public RuleNode_t
{
private:
    const NonTerminalNode_t *value;
    std::string name;

public:
    explicit AssignNode_t(
            std::string name_,
            const NonTerminalNode_t *value_
            )
        :
            value(value_),
            name(name_)
    {}
};

class DeclareNode_t : public RuleNode_t
{
private:
    std::string name;

public:
    explicit DeclareNode_t(const std::string name_)
        :
            name(std::move(name_))
    {}
};

class PrintNode_t : public RuleNode_t
{
private:
    const NonTerminalNode_t *child;

public:
    explicit PrintNode_t(const NonTerminalNode_t *child_)
        :
            child(child_)
    {}
};

class IfNode_t : public RuleNode_t
{
private:
    const NonTerminalNode_t *if_case;
    const RuleNode_t *expr;

public:
    explicit IfNode_t(const NonTerminalNode_t *if_case_, const RuleNode_t *expr_)
        :
            if_case(if_case_),
            expr(expr_)
    {}
};

class IfElseNode_t : public RuleNode_t
{
private:
    const NonTerminalNode_t *if_case;
    const RuleNode_t *true_expr;
    const RuleNode_t *false_expr;

public:
    explicit IfElseNode_t(
            const NonTerminalNode_t *if_case_,
            const RuleNode_t *true_expr_,
            const RuleNode_t *false_expr_
            )
        :
           if_case(if_case_),
           true_expr(true_expr_),
           false_expr(false_expr_)
    {}
};
