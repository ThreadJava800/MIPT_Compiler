#include "llvmIR.hpp"
#include "log.hpp"

LLVMBuilder::LLVMBuilder() :
    lmodule("MIPT language", context),
    builder(context)
{}

void LLVMBuilder::visit(const ProgramNode_t &node)
{
    DEV_ASSERT(node.main_fun_trampoline == nullptr);

    llvm::FunctionType *void_type = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *main_func = llvm::Function::Create(void_type, llvm::Function::ExternalLinkage, "main", lmodule);
    llvm::BasicBlock *program_entry = llvm::BasicBlock::Create(context, "", main_func);

    builder.SetInsertPoint(program_entry);
    node.main_fun_trampoline->accept(*this);
    builder.CreateRetVoid();
}

void LLVMBuilder::visit(const FunctionNode_t &node)
{

}

void LLVMBuilder::visit(const VariableNode_t &node)
{
    if (values.count(node.name) != 0)
    {
        shared_llvm_value = builder.CreateLoad(values[node.name]->getAllocatedType(), values[node.name]);
    }
    else
    {
        USER_ABORT("Variable (%s) was not created!\n", node.name.c_str());
    }
}

void LLVMBuilder::visit(const ValueNode_t &node)
{
    shared_llvm_value = llvm::ConstantInt::get(context, llvm::APInt(64, node.value, true));
}

void LLVMBuilder::visit(const AndNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    llvm::Value *value1 = shared_llvm_value;

    node.right->accept(*this);
    llvm::Value *value2 = shared_llvm_value;

    shared_llvm_value = builder.CreateLogicalOp(llvm::Instruction::BinaryOps::And, value1, value2);
}

void LLVMBuilder::visit(const OrNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    llvm::Value *value1 = shared_llvm_value;

    node.right->accept(*this);
    llvm::Value *value2 = shared_llvm_value;

    shared_llvm_value = builder.CreateLogicalOp(llvm::Instruction::BinaryOps::Or, value1, value2);
}

void LLVMBuilder::visit(const ComparatorNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    llvm::Value *value1 = shared_llvm_value;

    node.right->accept(*this);
    llvm::Value *value2 = shared_llvm_value;

    switch (node.oper)
    {
    case ComparatorOperators::LESS:
        shared_llvm_value = builder.CreateICmpULT(value1, value2);
        break;
    case ComparatorOperators::LESS_OR_EQ:
        shared_llvm_value = builder.CreateICmpULE(value1, value2);
        break;
    case ComparatorOperators::MORE:
        shared_llvm_value = builder.CreateICmpUGT(value1, value2);
        break;
    case ComparatorOperators::MORE_OR_EQ:
        shared_llvm_value = builder.CreateICmpUGE(value1, value2);
        break;
    case ComparatorOperators::EQ:
        shared_llvm_value = builder.CreateICmpEQ(value1, value2);
        break;
    default:
        DEV_ASSERT(true);
        break;
    }
}

void LLVMBuilder::visit(const ArithmeticNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    llvm::Value *value1 = shared_llvm_value;

    node.right->accept(*this);
    llvm::Value *value2 = shared_llvm_value;

    switch (node.oper)
    {
    case ArithmeticOperators::ADD:
        shared_llvm_value = builder.CreateAdd(value1, value2);
        break;
    case ArithmeticOperators::SUB:
        shared_llvm_value = builder.CreateSub(value1, value2);
        break;
    case ArithmeticOperators::MUL:
        shared_llvm_value = builder.CreateMul(value1, value2);
        break;
    case ArithmeticOperators::DIV:
        shared_llvm_value = builder.CreateSDiv(value1, value2);
        break;
    default:
        DEV_ASSERT(true);
        break;
    }
}

void LLVMBuilder::visit(const NotNode_t &node)
{
    DEV_ASSERT(node.child == nullptr);

    node.child->accept(*this);
    llvm::Value *value1 = shared_llvm_value;

    shared_llvm_value = builder.CreateNot(value1);
}

void LLVMBuilder::visit(const CallFuncNode_t &node)
{

}

void LLVMBuilder::visit(const ReturnNode_t &node)
{

}

void LLVMBuilder::visit(const NopRuleNode_t &node)
{
    for (const auto child : node.children_vec)
    {
        child->accept(*this);
    }
}

void LLVMBuilder::visit(const AssignNode_t &node)
{
    DEV_ASSERT(node.value == nullptr);

    node.value->accept(*this);
    llvm::Value *value = shared_llvm_value;

    if (values.count(node.name) != 0)
    {
        shared_llvm_value = builder.CreateStore(value, values[node.name]);
    }
    else
    {
        USER_ABORT("Variable (%s) was not created!\n", node.name.c_str());
    }
}

void LLVMBuilder::visit(const DeclareNode_t &node)
{
    values[node.name] = builder.CreateAlloca(llvm::Type::getInt64Ty(context));
}

void LLVMBuilder::visit(const PrintNode_t &node)
{
    llvm::Function *print_func = lmodule.getFunction("printf");
    DEV_ASSERT(print_func == nullptr);

    node.child->accept(*this);
    llvm::Value *print_value = shared_llvm_value;

    static const auto int_fmt_str = builder.CreateGlobalString("%ld\n");
    std::vector<llvm::Value*> argv = {
        int_fmt_str,
        print_value
    };
    shared_llvm_value = builder.CreateCall(print_func, argv);
}

void LLVMBuilder::visit(const IfNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.expr == nullptr);

    llvm::Function *curr_bb = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *true_bb = llvm::BasicBlock::Create(context, "", curr_bb);
    llvm::BasicBlock *continue_bb = llvm::BasicBlock::Create(context);

    node.if_case->accept(*this);
    llvm::Value *if_cond = builder.CreateICmpNE(
        shared_llvm_value,
        llvm::ConstantInt::get(context, llvm::APInt(1, 0, true))
    );
    builder.CreateCondBr(if_cond, true_bb, continue_bb);

    builder.SetInsertPoint(true_bb);
    node.expr->accept(*this);
    builder.CreateBr(continue_bb);

    curr_bb->insert(curr_bb->end(), continue_bb);
    builder.SetInsertPoint(continue_bb);
}

void LLVMBuilder::visit(const IfElseNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.true_expr == nullptr);
    DEV_ASSERT(node.false_expr == nullptr);

    llvm::Function *curr_bb = builder.GetInsertBlock()->getParent();
    llvm::BasicBlock *true_bb = llvm::BasicBlock::Create(context, "", curr_bb);
    llvm::BasicBlock *false_bb = llvm::BasicBlock::Create(context);
    llvm::BasicBlock *continue_bb = llvm::BasicBlock::Create(context);

    node.if_case->accept(*this);
    llvm::Value *if_cond = builder.CreateICmpNE(
        shared_llvm_value,
        llvm::ConstantInt::get(context, llvm::APInt(1, 0, true))
    );
    builder.CreateCondBr(if_cond, true_bb, false_bb);

    builder.SetInsertPoint(true_bb);
    node.true_expr->accept(*this);
    builder.CreateBr(continue_bb);

    curr_bb->insert(curr_bb->end(), false_bb);
    builder.SetInsertPoint(false_bb);
    node.false_expr->accept(*this);
    builder.CreateBr(continue_bb);

    curr_bb->insert(curr_bb->end(), continue_bb);
    builder.SetInsertPoint(continue_bb);
}

void LLVMBuilder::generateLLVMIR(const char *output_file, const ProgramNode_t &root)
{
    createStdFunctions();
    root.accept(*this);
    
    std::error_code err_code;
    llvm::raw_fd_ostream llvm_out_stream(output_file, err_code);
    lmodule.print(llvm_out_stream, nullptr);

    printf("Generator error code = %s\n", err_code.message().c_str());
}

void LLVMBuilder::createPrintFunction()
{
    std::vector<llvm::Type*> argv_types = {
        llvm::Type::getInt8Ty(context)->getPointerTo()
    };
    llvm::FunctionType *func_type = llvm::FunctionType::get(llvm::FunctionType::getInt32Ty(context), argv_types, true);

    auto func_ptr = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, "printf", lmodule);
    func_ptr->setCallingConv(llvm::CallingConv::C);
}

void LLVMBuilder::createStdFunctions()
{
    createPrintFunction();
}
