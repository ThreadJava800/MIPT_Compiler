#pragma once

#include <cstdarg>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#define DEBUG

using AstValue_t = int64_t;

class AstNode_t
{
#if defined (DEBUG)
public:
#else
protected:
#endif
    std::vector<const AstNode_t*> children_vec;

public:
    template<typename... Args>
    explicit AstNode_t(Args... children)
    {
        (children_vec.push_back(children), ...);
    }

    AstNode_t(const AstNode_t&) = delete;
    AstNode_t &operator=(const AstNode_t&) = delete;
    AstNode_t(AstNode_t&&) = delete;
    AstNode_t &operator=(AstNode_t&&) = delete;

    virtual ~AstNode_t()
    {
        for (const auto child : children_vec)
        {
            delete child;
        }
    }

#if defined (DEBUG)
    virtual void printFunc(FILE *printFile) const {}
#endif
};

class ProgramNode : public AstNode_t
{
public:
    explicit ProgramNode()
        :
            AstNode_t()
        {}

    void addChild(const AstNode_t *child)
    {
        children_vec.push_back(child);
    }

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "PROGRAM_ENTRY");
    }
};

class NonTerminalNode_t : public AstNode_t
{
public:
    template<typename... Args>
    explicit NonTerminalNode_t(Args... children)
        :
            AstNode_t(children...)
        {}
};

class VariableNode_t : public NonTerminalNode_t
{
protected:
    std::string name;

public:
    explicit VariableNode_t(const std::string name_)
        :
            NonTerminalNode_t(),
            name(std::move(name_))
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "VARIABLE %s", name.c_str());
    }
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

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "VALUE %d", value);
    }
};

class AndNode_t : public NonTerminalNode_t
{
public:
    explicit AndNode_t(
        const NonTerminalNode_t *left,
        const NonTerminalNode_t *right
        )
        :
            NonTerminalNode_t(left, right)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "AND");
    }
};

class OrNode_t : public NonTerminalNode_t
{
public:
    explicit OrNode_t(
        const NonTerminalNode_t *left,
        const NonTerminalNode_t *right
        )
        :
            NonTerminalNode_t(left, right)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "OR");
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
private:
    ComparatorOperators oper;

public:
    explicit ComparatorNode_t(
        const ComparatorOperators oper_,
        const NonTerminalNode_t *left_,
        const NonTerminalNode_t *right_
        )
        :
            NonTerminalNode_t(left_, right_),
            oper  (oper_)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "COMPARE %d", oper);
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
private:
    ArithmeticOperators oper;

public:
    explicit ArithmeticNode_t(
        const ArithmeticOperators oper_,
        const NonTerminalNode_t *left_,
        const NonTerminalNode_t *right_
        )
        :
            NonTerminalNode_t(left_, right_),
            oper  (oper_)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "ARITHMETICS %d", oper);
    }
};

class NotNode_t : public NonTerminalNode_t
{
public:
    explicit NotNode_t(const NonTerminalNode_t *child)
        :
            NonTerminalNode_t(child)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "NOT");
    }
};

class RuleNode_t : public AstNode_t
{
public:
    template<typename... Args>
    explicit RuleNode_t(Args... children)
        :
            AstNode_t(children...)
        {}
};

class AssignNode_t : public RuleNode_t
{
public:
    explicit AssignNode_t(
            const VariableNode_t *variable,
            const NonTerminalNode_t *value
            )
        :
            RuleNode_t(variable, value)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "ASSIGN");
    }
};

class DeclareNode_t : public RuleNode_t
{
public:
    explicit DeclareNode_t(const VariableNode_t *variable_)
        :
            RuleNode_t(variable_)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "DECLARE");
    }
};

class PrintNode_t : public RuleNode_t
{
public:
    explicit PrintNode_t(const NonTerminalNode_t *child)
        :
            RuleNode_t(child)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "PRINT");
    }
};

class IfNode_t : public RuleNode_t
{
public:
    explicit IfNode_t(const NonTerminalNode_t *if_case, const AstNode_t *expr)
        :
            RuleNode_t(if_case, expr)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "IF");
    }
};

class IfElseNode_t : public RuleNode_t
{
public:
    explicit IfElseNode_t(
            const NonTerminalNode_t *if_case,
            const AstNode_t *true_expr,
            const AstNode_t *false_expr
            )
        :
            RuleNode_t(if_case, true_expr, false_expr)
        {}

    void printFunc(FILE *printFile) const override
    {
        fprintf(printFile, "IF + ELSE");
    }
};
