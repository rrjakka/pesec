#include "include/ast/break_node.h"

#include "include/ast/ast_node.h"
#include <stdlib.h>

ast_node_t* break_node_new(ast_node_t* expression)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_BREAK;
    node->node._break = (break_node_t*)malloc(sizeof(break_node_t));
    node->node._break->expression = expression;
    return node;
}

void break_node_free(break_node_t* break_node)
{
    free(break_node);
}

value_t break_node_evaluate(const break_node_t* break_node, context_t* context)
{
    value_t result;

    if (break_node->expression)
        result = ast_node_evaluate(break_node->expression, context);
    else
    {
        result = (value_t) {
            .type = VALUE_TYPE_NUMBER,
            .value.as_number = 0
        };
    }

    result.control_flow = CONTROL_FLOW_BREAK;
    return result;
}
