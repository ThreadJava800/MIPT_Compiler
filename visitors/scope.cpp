#include <utility>

#include "log.hpp"
#include "scope.hpp"

llvm::AllocaInst *Scope::getVariableByName(const std::string var_name) const
{
    if (scope_vars.count(var_name) != 0)
    {
        return scope_vars.at(var_name);
    }

    if (parent != nullptr)
    {
        return parent->getVariableByName(var_name);
    }
    throw std::invalid_argument("No variable with such name!");
}

void ScopeTreeBuilder::build(const ProgramNode_t &node)
{
    visit(node);
}

llvm::AllocaInst *ScopeTreeBuilder::getVariable(const NewScopeNode_t *view_scope, const std::string var_name) const
{
    return scope_view_model.at(view_scope)->getVariableByName(var_name);
}

void ScopeTreeBuilder::visit(const ProgramNode_t &node)
{
    for (const auto child : node.children)
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
    catch(...)
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

void ScopeTreeBuilder::visit(const NewScopeNode_t &node)
{
    const auto new_scope = new Scope(current);
    if (current == nullptr)
    {
        root = new_scope;
    }
    else
    {
        current->children.push_back(new_scope);
    }

    current = new_scope;

    for (const auto child : node.children_vec)
    {
        child->accept(*this);
    }

    scope_view_model[&node] = new_scope;
    current = current->parent;
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
