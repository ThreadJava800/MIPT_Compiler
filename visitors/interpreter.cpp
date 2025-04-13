#include "interpreter.hpp"
#include "log.hpp"

void Interpreter::visit(const ProgramNode_t &node)
{
    for (const auto child : node.children_vec)
    {
        child->accept(*this);
    }
}

void Interpreter::visit(const VariableNode_t &node)
{
    if (variables.count(node.name) != 0)
    {
        shared_value = variables[node.name];
    }
    else
    {
        USER_ABORT("Variable (%s) was not created!\n", node.name.c_str());
    }
}

void Interpreter::visit(const ValueNode_t &node)
{
    shared_value = node.value;
}

void Interpreter::visit(const AndNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    const AstValue_t left_val = shared_value;

    node.right->accept(*this);
    const AstValue_t right_val = shared_value;

    shared_value = left_val && right_val;
}

void Interpreter::visit(const OrNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    const AstValue_t left_val = shared_value;

    node.right->accept(*this);
    const AstValue_t right_val = shared_value;

    shared_value = left_val || right_val;
}

void Interpreter::visit(const ComparatorNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    const AstValue_t value1 = shared_value;

    node.right->accept(*this);
    const AstValue_t value2 = shared_value;

    switch (node.oper)
    {
    case ComparatorOperators::LESS:
        shared_value = value1 < value2;
        break;
    case ComparatorOperators::LESS_OR_EQ:
        shared_value = value1 <= value2;
        break;
    case ComparatorOperators::MORE:
        shared_value = value1 > value2;
        break;
    case ComparatorOperators::MORE_OR_EQ:
        shared_value = value1 >= value2;
        break;
    case ComparatorOperators::EQ:
        shared_value = value1 == value2;
        break;
    default:
        DEV_ASSERT(true);
        break;
    }
}

void Interpreter::visit(const ArithmeticNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    node.left->accept(*this);
    const AstValue_t value1 = shared_value;

    node.right->accept(*this);
    const AstValue_t value2 = shared_value;

    switch (node.oper)
    {
    case ArithmeticOperators::ADD:
        shared_value = value1 + value2;
        break;
    case ArithmeticOperators::SUB:
        shared_value = value1 - value2;
        break;
    case ArithmeticOperators::MUL:
        shared_value = value1 * value2;
        break;
    case ArithmeticOperators::DIV:
        shared_value = value1 / value2;
        break;
    default:
        DEV_ASSERT(true);
        break;
    }
}

void Interpreter::visit(const NotNode_t &node)
{
    DEV_ASSERT(node.child == nullptr);

    node.child->accept(*this);
    shared_value = !shared_value;
}

void Interpreter::visit(const NopRuleNode_t &node)
{
    for (const auto child : node.children_vec)
    {
        child->accept(*this);
    }
}

void Interpreter::visit(const AssignNode_t &node)
{
    DEV_ASSERT(node.value == nullptr);

    node.value->accept(*this);
    const AstValue_t value = shared_value;

    if (variables.count(node.name) != 0)
    {
        variables[node.name] = value;
    }
    else
    {
        USER_ABORT("Variable (%s) was not created!\n", node.name.c_str());
    }
}

void Interpreter::visit(const DeclareNode_t &node)
{
    variables[node.name] = 0;
}

void Interpreter::visit(const PrintNode_t &node)
{
    DEV_ASSERT(node.child == nullptr);

    node.child->accept(*this);
    printf("%d\n", shared_value);
}

void Interpreter::visit(const IfNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.expr == nullptr);

    node.if_case->accept(*this);
    const AstValue_t if_case = shared_value;

    if (if_case)
    {
        node.expr->accept(*this);
    }
}

void Interpreter::visit(const IfElseNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.true_expr == nullptr);
    DEV_ASSERT(node.false_expr == nullptr);

    node.if_case->accept(*this);
    const AstValue_t if_case = shared_value;

    if (if_case)
    {
        node.true_expr->accept(*this);
    }
    else
    {
        node.false_expr->accept(*this);
    } 
}
