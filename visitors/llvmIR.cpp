#include "llvmIR.hpp"

LLVMBuilder::LLVMBuilder() :
    lmodule("MIPT language", context),
    builder(context)
{}

void LLVMBuilder::visit(const ProgramNode_t &node)
{

}

void LLVMBuilder::visit(const VariableNode_t &node)
{

}

void LLVMBuilder::visit(const ValueNode_t &node)
{

}

void LLVMBuilder::visit(const AndNode_t &node)
{

}

void LLVMBuilder::visit(const OrNode_t &node)
{

}

void LLVMBuilder::visit(const ComparatorNode_t &node)
{

}

void LLVMBuilder::visit(const ArithmeticNode_t &node)
{

}

void LLVMBuilder::visit(const NotNode_t &node)
{

}

void LLVMBuilder::visit(const NopRuleNode_t &node)
{

}

void LLVMBuilder::visit(const AssignNode_t &node)
{

}

void LLVMBuilder::visit(const DeclareNode_t &node)
{

}

void LLVMBuilder::visit(const PrintNode_t &node)
{

}

void LLVMBuilder::visit(const IfNode_t &node)
{

}

void LLVMBuilder::visit(const IfElseNode_t &node)
{

}

void LLVMBuilder::generateLLVMIR(const char *output_file, const ProgramNode_t &root)
{
    root.accept(*this);
}

llvm::Value *LLVMBuilder::codegen(const ProgramNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const VariableNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const ValueNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const AndNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const OrNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const ComparatorNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const ArithmeticNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const NotNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const NopRuleNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const AssignNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const DeclareNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const PrintNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const IfNode_t &node)
{

}

llvm::Value *LLVMBuilder::codegen(const IfElseNode_t &node)
{
    
}
