#pragma once

#include <cstdarg>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class Driver_t;

using AstValue_t = int64_t;

class AstNode_t
{
friend Driver_t;

protected:
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

    virtual void graphDumpLabel(FILE *printFile) const = 0;
};

class ProgramNode_t : public AstNode_t
{
public:
    explicit ProgramNode_t()
        :
            AstNode_t()
        {}

    void addChild(const AstNode_t *child)
    {
        children_vec.push_back(child);
    }

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "PROGRAM_ENTRY");
    }

    void interpret();
};

class NonTerminalNode_t : public AstNode_t
{
public:
    template<typename... Args>
    explicit NonTerminalNode_t(Args... children)
        :
            AstNode_t(children...)
        {}

    void graphDumpLabel(FILE *printFile) const override {}

    virtual AstValue_t interpret() const = 0;
    virtual ~NonTerminalNode_t() {}
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

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "VARIABLE %s", name.c_str());
    }

    AstValue_t interpret() const override;
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

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "VALUE %d", value);
    }

    AstValue_t interpret() const override;
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

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "AND");
    }

    AstValue_t interpret() const override;
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

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "OR");
    }

    AstValue_t interpret() const override;
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

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "COMPARE %d", oper);
    }

    AstValue_t interpret() const override;
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

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "ARITHMETICS %d", oper);
    }

    AstValue_t interpret() const override;
};

class NotNode_t : public NonTerminalNode_t
{
public:
    explicit NotNode_t(const NonTerminalNode_t *child)
        :
            NonTerminalNode_t(child)
        {}

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "NOT");
    }

    AstValue_t interpret() const override;
};

class RuleNode_t : public AstNode_t
{
public:
    template<typename... Args>
    explicit RuleNode_t(Args... children)
        :
            AstNode_t(children...)
        {}

    virtual void interpret() const = 0;
    virtual ~RuleNode_t() {}
};

class NopRuleNode_t : public RuleNode_t
{
public:
    template<typename... Args>
    explicit NopRuleNode_t(Args... children)
        :
            RuleNode_t(children...)
        {}

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "NOP");
    }

    void interpret() const override;
};

class AssignNode_t : public RuleNode_t
{
private:
    std::string name;

public:
    explicit AssignNode_t(
            std::string name_,
            const NonTerminalNode_t *value
            )
        :
            RuleNode_t(value),
            name(name_)
        {}

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "ASSIGN %s", name.c_str());
    }

    void interpret() const override;
};

class DeclareNode_t : public RuleNode_t
{
private:
    std::string name;

public:
    explicit DeclareNode_t(const std::string name_)
        :
            RuleNode_t(),
            name(std::move(name_))
        {}

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "DECLARE %s", name.c_str());
    }

    void interpret() const override;
};

class PrintNode_t : public RuleNode_t
{
public:
    explicit PrintNode_t(const NonTerminalNode_t *child)
        :
            RuleNode_t(child)
        {}

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "PRINT");
    }

    void interpret() const override;
};

class IfNode_t : public RuleNode_t
{
public:
    explicit IfNode_t(const NonTerminalNode_t *if_case, const RuleNode_t *expr)
        :
            RuleNode_t(if_case, expr)
        {}

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "IF");
    }

    void interpret() const override;
};

class IfElseNode_t : public RuleNode_t
{
public:
    explicit IfElseNode_t(
            const NonTerminalNode_t *if_case,
            const RuleNode_t *true_expr,
            const RuleNode_t *false_expr
            )
        :
            RuleNode_t(if_case, true_expr, false_expr)
        {}

    void graphDumpLabel(FILE *printFile) const override
    {
        fprintf(printFile, "IF + ELSE");
    }

    void interpret() const override;
};
