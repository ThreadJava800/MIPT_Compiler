#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <map>

#include "ast.hpp"
#include "visitor.hpp"

class LLVMBuilder : public Visitor
{
private:
    llvm::LLVMContext context;
    llvm::Module lmodule;
    llvm::IRBuilder<> builder;
    std::map<std::string, llvm::Value*> values;

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
    void visit(const NopRuleNode_t &node) override;
    void visit(const AssignNode_t &node) override;
    void visit(const DeclareNode_t &node) override;
    void visit(const PrintNode_t &node) override;
    void visit(const IfNode_t &node) override;
    void visit(const IfElseNode_t &node) override;

    void generateLLVMIR(const char *output_file, const ProgramNode_t &root);

private:
    llvm::Value *codegen(const ProgramNode_t &node);
    llvm::Value *codegen(const VariableNode_t &node);
    llvm::Value *codegen(const ValueNode_t &node);
    llvm::Value *codegen(const AndNode_t &node);
    llvm::Value *codegen(const OrNode_t &node);
    llvm::Value *codegen(const ComparatorNode_t &node);
    llvm::Value *codegen(const ArithmeticNode_t &node);
    llvm::Value *codegen(const NotNode_t &node);
    llvm::Value *codegen(const NopRuleNode_t &node);
    llvm::Value *codegen(const AssignNode_t &node);
    llvm::Value *codegen(const DeclareNode_t &node);
    llvm::Value *codegen(const PrintNode_t &node);
    llvm::Value *codegen(const IfNode_t &node);
    llvm::Value *codegen(const IfElseNode_t &node);
};
