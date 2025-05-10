#include <map>

#include "ast.hpp"
#include "visitor.hpp"

class Interpreter : public Visitor
{
private:
    std::map<std::string, const FunctionNode_t*> *functions = nullptr;
    std::map<std::string, AstValue_t> variables;
    AstValue_t shared_value;

public:
    explicit Interpreter() = default;

    void setFunctionsMap(std::map<std::string, const FunctionNode_t*> *functions_)
    {
        functions = functions_;
    }

    void visit(const ProgramNode_t &node) override;
    void visit(const FunctionNode_t &node) override;
    void visit(const VariableNode_t &node) override;
    void visit(const ValueNode_t &node) override;
    void visit(const AndNode_t &node) override;
    void visit(const OrNode_t &node) override;
    void visit(const ComparatorNode_t &node) override;
    void visit(const ArithmeticNode_t &node) override;
    void visit(const NotNode_t &node) override;
    void visit(const CallFuncNode_t &node) override;
    void visit(const ReturnNode_t &node) override;
    void visit(const NopRuleNode_t &node) override;
    void visit(const AssignNode_t &node) override;
    void visit(const DeclareNode_t &node) override;
    void visit(const PrintNode_t &node) override;
    void visit(const IfNode_t &node) override;
    void visit(const IfElseNode_t &node) override;
};
