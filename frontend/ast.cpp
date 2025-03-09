#include <cstdio>

#include "ast.hpp"
#include "frontend.hpp"
#include "log.hpp"

void ProgramNode_t::interpret()
{
    for (const auto child : children_vec)
    {
        dynamic_cast<const RuleNode_t*>(child)->interpret();
    }
}

AstValue_t VariableNode_t::interpret() const
{
    return Driver_t::getVariableValue(name);
}

AstValue_t ValueNode_t::interpret() const
{
    return value;
}

AstValue_t AndNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 2);
    return 
        dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret()
        &&
        dynamic_cast<const NonTerminalNode_t*>(children_vec[1])->interpret();
}

AstValue_t OrNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 2);
    return 
        dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret()
        ||
        dynamic_cast<const NonTerminalNode_t*>(children_vec[1])->interpret();
}

AstValue_t ComparatorNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 2);

    const AstValue_t value1 = std::move(dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret());
    const AstValue_t value2 = std::move(dynamic_cast<const NonTerminalNode_t*>(children_vec[1])->interpret());

    switch (oper)
    {
    case ComparatorOperators::LESS:
        return value1 < value2;
    case ComparatorOperators::LESS_OR_EQ:
        return value1 <= value2;
    case ComparatorOperators::MORE:
        return value1 > value2;
    case ComparatorOperators::MORE_OR_EQ:
        return value1 >= value2;
    case ComparatorOperators::EQ:
        return value1 == value2;
    default:
        DEV_ASSERT(true);
        break;
    }

    return 0;
}

AstValue_t ArithmeticNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 2);

    const AstValue_t value1 = std::move(dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret());
    const AstValue_t value2 = std::move(dynamic_cast<const NonTerminalNode_t*>(children_vec[1])->interpret());

    switch (oper)
    {
    case ArithmeticOperators::ADD:
        return value1 + value2;
    case ArithmeticOperators::SUB:
        return value1 - value2;
    case ArithmeticOperators::MUL:
        return value1 * value2;
    case ArithmeticOperators::DIV:
        return value1 / value2;
    default:
        DEV_ASSERT(true);
        break;
    }

    return 0;
}

AstValue_t NotNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 1);
    return 
        !dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret();
}

void NopRuleNode_t::interpret() const
{
    for (const auto child : children_vec)
    {
        dynamic_cast<const RuleNode_t*>(child)->interpret();
    }
}

void AssignNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 1);
    const AstValue_t value =
        dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret();
    Driver_t::setVariableValue(name, value);
}

void DeclareNode_t::interpret() const
{
    Driver_t::createVariable(name);    
}

void PrintNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 1);
    const AstValue_t value = 
        dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret();

    printf("%d\n", value);
}

void IfNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 2);
    const AstValue_t if_case = 
        dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret();

    if (if_case)
    {
        dynamic_cast<const RuleNode_t*>(children_vec[1])->interpret();
    }
}

void IfElseNode_t::interpret() const
{
    DEV_ASSERT(children_vec.size() != 3);
    const AstValue_t if_case = 
        dynamic_cast<const NonTerminalNode_t*>(children_vec[0])->interpret();

    if (if_case)
    {
        dynamic_cast<const RuleNode_t*>(children_vec[1])->interpret();
    }
    else
    {
        dynamic_cast<const RuleNode_t*>(children_vec[2])->interpret();
    }
}
