#include <utility>

#include "log.hpp"
#include "scope.hpp"

llvm::AllocaInst *Scope::getVariableByName(const std::string var_name)
{
    if (scope_vars.count(var_name) != 0)
    {
        return scope_vars[var_name];
    }

    if (parent != nullptr)
    {
        return parent->getVariableByName(var_name);
    }
    throw std::invalid_argument("No variable with such name!");
}

void ScopeTreeBuilder::build(const ProgramNode_t &node)
{
    call_stack.push(root);
    visit(node);
}

void ScopeTreeBuilder::visit(const ProgramNode_t &node)
{
    DEV_ASSERT(node.main_fun_trampoline == nullptr);

    for (const auto &[fun_name, fun_ptr] : node.functions)
    {
        auto new_func_scope = new Scope(current);
        current->children.push_back(new_func_scope);

        call_stack.push(new_func_scope);
        current = new_func_scope;

        fun_ptr->accept(*this);
    }
    
    const auto main_scope = new Scope(current);
    current->children.push_back(main_scope);
    node.main_fun_trampoline->accept(*this);
}

void ScopeTreeBuilder::visit(const FunctionNode_t &node)
{
    for (const auto param : node.parameters)
    {
        // current->scope_vars[param->name] = /* func param */
    }
    for (auto child : node.children_vec)
    {
        child->accept(*this);
    }
}

void ScopeTreeBuilder::visit(const VariableNode_t &node)
{
    // check whether it is accessible from current scope
    try
    {
        std::ignore = current->getVariableByName(node.name);
    }
    catch(std::invalid_argument e)
    {
        USER_ABORT("Variable %s not found", node.name.c_str());
    }
}

void ScopeTreeBuilder::visit(const ValueNode_t &node)
{

}

void ScopeTreeBuilder::visit(const AndNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    node.right->accept(*this);
}

void ScopeTreeBuilder::visit(const OrNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    node.right->accept(*this);
}

void ScopeTreeBuilder::visit(const ComparatorNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    node.right->accept(*this);
}

void ScopeTreeBuilder::visit(const ArithmeticNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    node.right->accept(*this);
}

void ScopeTreeBuilder::visit(const NotNode_t &node)
{
    DEV_ASSERT(node.child == nullptr);

    node.child->accept(*this);
}

void ScopeTreeBuilder::visit(const CallFuncNode_t &node)
{

}

void ScopeTreeBuilder::visit(const ReturnNode_t &node)
{

}

void ScopeTreeBuilder::visit(const NopRuleNode_t &node)
{
    for (const auto child : node.children_vec)
    {
        child->accept(*this);
    }
}

void ScopeTreeBuilder::visit(const AssignNode_t &node)
{
    DEV_ASSERT(node.value == nullptr);
}

void ScopeTreeBuilder::visit(const DeclareNode_t &node)
{
    current->scope_vars[node.name] = builder.CreateAlloca(llvm::Type::getInt64Ty(context));
}

void ScopeTreeBuilder::visit(const PrintNode_t &node)
{

}

void ScopeTreeBuilder::visit(const IfNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.expr == nullptr);
}

void ScopeTreeBuilder::visit(const IfElseNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.true_expr == nullptr);
    DEV_ASSERT(node.false_expr == nullptr);
}
