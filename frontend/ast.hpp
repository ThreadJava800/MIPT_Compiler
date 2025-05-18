#pragma once

#include <cstdarg>
#include <cstdint>
#include <optional>
#include <map>
#include <string>
#include <vector>

#include "visitor.hpp"

class Interpreter;
class GraphDumper;
class LLVMBuilder;
class ScopeTreeBuilder;
#define DEFINE_FRIENDS friend Interpreter;\
                       friend GraphDumper;\
                       friend LLVMBuilder;\
                       friend ScopeTreeBuilder;

using AstValue_t = int64_t;

class AstNode_t
{
public:
    explicit AstNode_t() = default;

    AstNode_t(const AstNode_t&) = delete;
    AstNode_t &operator=(const AstNode_t&) = delete;
    AstNode_t(AstNode_t&&) = delete;
    AstNode_t &operator=(AstNode_t&&) = delete;

    virtual ~AstNode_t() = default;
    virtual void accept(Visitor& visitor) const = 0;
};

class NonTerminalNode_t : public AstNode_t
{
public:
    explicit NonTerminalNode_t() = default;
    virtual ~NonTerminalNode_t() = default;
};

class RuleNode_t : public AstNode_t
{
public:
    explicit RuleNode_t() = default;
    virtual ~RuleNode_t() = default;
};

class VariableNode_t : public NonTerminalNode_t
{
DEFINE_FRIENDS
protected:
    std::string name;

public:
    explicit VariableNode_t(const std::string name_)
        :
            name(std::move(name_))
    {}

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ProgramNode_t : public AstNode_t
{
DEFINE_FRIENDS
private:
    std::vector<const RuleNode_t*> children;

public:
    explicit ProgramNode_t(
            std::vector<const RuleNode_t*> children_
        ) :
        children(children_)
    {}

    ~ProgramNode_t()
    {
        for (const auto child : children)
        {
            delete child;
        }
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class ValueNode_t : public NonTerminalNode_t
{
DEFINE_FRIENDS
protected:
    const AstValue_t value;

public:
    explicit ValueNode_t(const AstValue_t value_)
        :
            NonTerminalNode_t(),
            value(std::move(value_))
    {}

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class AndNode_t : public NonTerminalNode_t
{
DEFINE_FRIENDS
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

    ~AndNode_t()
    {
        delete left;
        delete right;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class OrNode_t : public NonTerminalNode_t
{
DEFINE_FRIENDS
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

    ~OrNode_t()
    {
        delete left;
        delete right;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
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
DEFINE_FRIENDS
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

    ~ComparatorNode_t()
    {
        delete left;
        delete right;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
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
DEFINE_FRIENDS
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

    ~ArithmeticNode_t()
    {
        delete left;
        delete right;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class NotNode_t : public NonTerminalNode_t
{
DEFINE_FRIENDS
private:
    const NonTerminalNode_t *child;

public:
    explicit NotNode_t(const NonTerminalNode_t *child_)
        :
            child(child_)
    {}

    ~NotNode_t()
    {
        delete child;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class NewScopeNode_t : public RuleNode_t
{
DEFINE_FRIENDS
private:
    std::vector<const RuleNode_t*> children_vec;

public:
    explicit NewScopeNode_t(const std::vector<const RuleNode_t*> children_)
        : children_vec(children_)
    {}

    ~NewScopeNode_t()
    {
        for (const auto child : children_vec)
        {
            delete child;
        }
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class NopRuleNode_t : public RuleNode_t
{
DEFINE_FRIENDS
private:
    std::vector<const RuleNode_t*> children_vec;

public:
    template<typename... Args>
    explicit NopRuleNode_t(Args... children)
    {
        (children_vec.push_back(children), ...);
    }

    ~NopRuleNode_t()
    {
        for (const auto child : children_vec)
        {
            delete child;
        }
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class AssignNode_t : public RuleNode_t
{
DEFINE_FRIENDS
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

    ~AssignNode_t()
    {
        delete value;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class DeclareNode_t : public RuleNode_t
{
DEFINE_FRIENDS
private:
    std::string name;

public:
    explicit DeclareNode_t(const std::string name_)
        :
            name(std::move(name_))
    {}

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class PrintNode_t : public RuleNode_t
{
DEFINE_FRIENDS
private:
    const NonTerminalNode_t *child;

public:
    explicit PrintNode_t(const NonTerminalNode_t *child_)
        :
            child(child_)
    {}

    ~PrintNode_t()
    {
        delete child;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class IfNode_t : public RuleNode_t
{
DEFINE_FRIENDS
private:
    const NonTerminalNode_t *if_case;
    const RuleNode_t *expr;

public:
    explicit IfNode_t(const NonTerminalNode_t *if_case_, const RuleNode_t *expr_)
        :
            if_case(if_case_),
            expr(expr_)
    {}

    ~IfNode_t()
    {
        delete if_case;
        delete expr;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};

class IfElseNode_t : public RuleNode_t
{
DEFINE_FRIENDS
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

    ~IfElseNode_t()
    {
        delete if_case;
        delete true_expr;
        delete false_expr;
    }

    void accept(Visitor& visitor) const override
    {
        visitor.visit(*this);
    }
};
