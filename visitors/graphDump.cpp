#include"graphDump.hpp"
#include "log.hpp"

void GraphDumper::visit(const ProgramNode_t &node)
{
    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "PROGRAM_ENTRY");
    fprintf(dot_file, "}\"];\n");

    for (const auto fun_ptr : node.children)
    {
        fun_ptr->accept(*this);
        fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, fun_ptr);
    }
}

void GraphDumper::visit(const VariableNode_t &node)
{
    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "VARIABLE %s", node.name.c_str());
    fprintf(dot_file, "}\"];\n");
}

void GraphDumper::visit(const ValueNode_t &node)
{
    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "VALUE %ld", node.value);
    fprintf(dot_file, "}\"];\n");
}

void GraphDumper::visit(const AndNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "AND");
    fprintf(dot_file, "}\"];\n");

    node.left->accept(*this);
    node.right->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.left);
    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.right);
}

void GraphDumper::visit(const OrNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "OR");
    fprintf(dot_file, "}\"];\n");

    node.left->accept(*this);
    node.right->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.left);
    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.right);
}

void GraphDumper::visit(const ComparatorNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "COMPARE %d", (int)node.oper);
    fprintf(dot_file, "}\"];\n");

    node.left->accept(*this);
    node.right->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.left);
    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.right);
}

void GraphDumper::visit(const ArithmeticNode_t &node)
{
    DEV_ASSERT(node.left == nullptr);
    DEV_ASSERT(node.right == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "ARITHMETICS %d", (int)node.oper);
    fprintf(dot_file, "}\"];\n");

    node.left->accept(*this);
    node.right->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.left);
    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.right);
}

void GraphDumper::visit(const NotNode_t &node)
{
    DEV_ASSERT(node.child == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "NOT");
    fprintf(dot_file, "}\"];\n");

    node.child->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.child);
}

void GraphDumper::visit(const NewScopeNode_t &node)
{
    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "SCOPE START");
    fprintf(dot_file, "}\"];\n");

    for (const auto fun_ptr : node.children_vec)
    {
        fun_ptr->accept(*this);
        fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, fun_ptr);
    }
}

void GraphDumper::visit(const NopRuleNode_t &node)
{
    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "NOP");
    fprintf(dot_file, "}\"];\n");

    for (const auto child : node.children_vec)
    {
        child->accept(*this);
    }
    for (const auto child : node.children_vec)
    {
        fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, child);
    }
}

void GraphDumper::visit(const AssignNode_t &node)
{
    DEV_ASSERT(node.value == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "ASSIGN %s", node.name.c_str());
    fprintf(dot_file, "}\"];\n");

    node.value->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.value);
}

void GraphDumper::visit(const DeclareNode_t &node)
{
    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "DECLARE %s", node.name.c_str());
    fprintf(dot_file, "}\"];\n");
}

void GraphDumper::visit(const PrintNode_t &node)
{
    DEV_ASSERT(node.child == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "PRINT");
    fprintf(dot_file, "}\"];\n");

    node.child->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.child);
}

void GraphDumper::visit(const IfNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.expr == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "IF");
    fprintf(dot_file, "}\"];\n");

    node.if_case->accept(*this);
    node.expr->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.if_case);
    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.expr);
}

void GraphDumper::visit(const IfElseNode_t &node)
{
    DEV_ASSERT(node.if_case == nullptr);
    DEV_ASSERT(node.true_expr == nullptr);
    DEV_ASSERT(node.false_expr == nullptr);

    fprintf(
        dot_file, 
        "\tlabel%p[shape=record, style=\"rounded, filled\", fillcolor=red, label=\"{val: ",
        &node
    );
    fprintf(dot_file, "IF + ELSE");
    fprintf(dot_file, "}\"];\n");

    node.if_case->accept(*this);
    node.true_expr->accept(*this);
    node.false_expr->accept(*this);

    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.if_case);
    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.true_expr);
    fprintf(dot_file, "\tlabel%p->label%p [color=\"red\", style=\"dashed\",arrowhead=\"none\"]", &node, node.false_expr);
}

void GraphDumper::createGraph(const char *image_name, const ProgramNode_t &root)
{
    dot_file = fopen("temp.dot", "w");
    if (!dot_file) 
    {
        DEV_DBG_ERR("failed to create .dot file!\n");
        return;
    }

    fprintf(dot_file, "digraph tree {\n");
    fprintf(dot_file, "\trankdir=HR;\n");

    root.accept(*this);

    fprintf(dot_file, "}");
    fclose(dot_file);

    char command[1024] = {0};
    snprintf(
        command, 
        sizeof(command), 
        "dot -Tsvg temp.dot > %s && xdg-open %s", 
        image_name, 
        image_name
    );

    system(command);
    system("rm temp.dot");
}
