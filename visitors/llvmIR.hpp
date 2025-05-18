#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <map>

#include "ast.hpp"
#include "scope.hpp"
#include "visitor.hpp"

class LLVMBuilder : public Visitor
{
private:
    std::stack<const NewScopeNode_t*> scope_stack;

    llvm::LLVMContext context;
    llvm::Module lmodule;
    llvm::IRBuilder<> builder;

    ScopeTreeBuilder scope_tree;
    llvm::Value *shared_llvm_value = nullptr;

public:
    explicit LLVMBuilder();

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

    void generateLLVMIR(const char *output_file, const ProgramNode_t &root);

private:
    void createPrintFunction();
    void createStdFunctions();
};
