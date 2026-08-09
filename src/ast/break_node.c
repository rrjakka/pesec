#include "include/ast/break_node.h"

#include "include/ast/ast_node.h"
#include <stdlib.h>

ast_node_t* break_node_new()
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_BREAK;
    node->node._break = (break_node_t*)malloc(sizeof(break_node_t));
    return node;
}

void break_node_free(break_node_t* break_node)
{
    free(break_node);
}

value_t break_node_evaluate(const break_node_t* break_node, context_t* context)
{
    return (value_t){
        .type = VALUE_TYPE_NUMBER,
        .value.as_number = 0
    };
}
