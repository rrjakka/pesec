#include "include/ast/while_loop_node.h"

#include "include/ast/ast_node.h"
#include <stdlib.h>


ast_node_t* while_loop_node_new(ast_node_t *condition, ast_node_t *while_body)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_WHILE_LOOP;
    node->node.while_loop = (while_loop_node_t*)malloc(sizeof(while_loop_node_t));
    node->node.while_loop->condition = condition;
    node->node.while_loop->while_body = while_body;
    return node;
}

void while_loop_node_free(while_loop_node_t* while_loop_node)
{
    free(while_loop_node->condition);
    free(while_loop_node->while_body);
    free(while_loop_node);
}

value_t while_loop_node_evaluate(const while_loop_node_t* while_loop_node, context_t* context)
{
    context_t* local_context = context_new(context);

    value_t result = {
        .type = VALUE_TYPE_NUMBER,
        .value.as_number = 0
    };

    while (value_get_boolean(ast_node_evaluate(while_loop_node->condition, local_context)))
    {
        result = ast_node_evaluate(while_loop_node->while_body, local_context);

        if (result.control_flow == CONTROL_FLOW_BREAK)
        {
            result.control_flow = CONTROL_FLOW_NONE;
            return result;
        }
    }

    context_free(local_context);

    return result;
}
