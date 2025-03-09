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
};

class NonTerminalNode_t : public AstNode_t
{
public:
    explicit NonTerminalNode_t()
        :
            AstNode_t()
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
            const ValueNode_t *value
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
