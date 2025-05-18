#pragma once

#include <map>
#include <string>
#include <string_view>
#include <functional>
#include <map>
#include <stack>
#include <variant>
#include <vector>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>

#include "ast.hpp"
#include "visitor.hpp"

class Scope
{
public:
    Scope *parent;
    std::vector<Scope*> children;
    std::map<std::string, llvm::AllocaInst*> scope_vars;

public:
    explicit Scope(
            Scope *parent_
        ) :
        parent(parent_)
    {}

    llvm::AllocaInst *getVariableByName(const std::string var_name) const;

    ~Scope()
    {
        for (const auto child : children)
        {
            delete child;
        }
    }
};

class ScopeTreeBuilder : public Visitor
{
private:
    Scope *root;
    Scope *current;

    llvm::LLVMContext &context;
    llvm::IRBuilder<> &builder;

    std::map<const NewScopeNode_t*, const Scope*> scope_view_model;

public:
    explicit ScopeTreeBuilder(
            llvm::LLVMContext &context_,
            llvm::IRBuilder<> &builder_
        ) :
        root(nullptr),
        current(nullptr),
        context(context_),
        builder(builder_)
    {}

    void build(const ProgramNode_t &node);
    llvm::AllocaInst *getVariable(const NewScopeNode_t *view_scope, const std::string var_name) const;

    void visit(const ProgramNode_t &node) override;
    void visit(const VariableNode_t &node) override;
    void visit(const ValueNode_t &node) override;
    void visit(const AndNode_t &node) override;
    void visit(const OrNode_t &node) override;
    void visit(const ComparatorNode_t &node) override;
    void visit(const ArithmeticNode_t &node) override;
    void visit(const NotNode_t &node) override;
    void visit(const NewScopeNode_t &node) override;
    void visit(const NopRuleNode_t &node) override;
    void visit(const AssignNode_t &node) override;
    void visit(const DeclareNode_t &node) override;
    void visit(const PrintNode_t &node) override;
    void visit(const IfNode_t &node) override;
    void visit(const IfElseNode_t &node) override;

    ~ScopeTreeBuilder()
    {
        delete root;
    }
};
