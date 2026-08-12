#include "include/ast/function_definition_node.h"

#include "include/ast/ast_node.h"
#include <stdlib.h>


ast_node_t* function_definition_node_new(ast_node_t* body)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_FUNCTION_DEFINITION;
    node->node.function_definition = (function_definition_node_t*)malloc(sizeof(function_definition_node_t));
    node->node.function_definition->body = body;
    return node;
}

void function_definition_node_free(function_definition_node_t* function_definition_node)
{
    ast_node_free(function_definition_node->body);
    free(function_definition_node);
}

value_t function_definition_node_evaluate(const function_definition_node_t* function_definition_node, context_t* context)
{
    return MAKE_VAL_NUM(0);
}
