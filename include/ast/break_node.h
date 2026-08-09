#ifndef PESEC_BREAK_NODE_H
#define PESEC_BREAK_NODE_H

#include "include/value.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* expression;
} break_node_t;

ast_node_t* break_node_new(ast_node_t* expression);

void break_node_free(break_node_t* break_node);

value_t break_node_evaluate(const break_node_t* break_node, context_t* context);

#endif // PESEC_BREAK_NODE_H
