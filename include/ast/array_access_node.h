#ifndef PESEC_ARRAY_ACCESS_NODE_H
#define PESEC_ARRAY_ACCESS_NODE_H

#include "statement_sequence_node.h"
#include "include/value.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* array;
    ast_node_t* index;
} array_access_node_t;

ast_node_t* array_access_node_new(ast_node_t* array, ast_node_t* index);

void array_access_node_free(array_access_node_t* array_access_node);

value_t array_access_node_evaluate(const array_access_node_t* array_access_node, context_t* context);

#endif // PESEC_ARRAY_ACCESS_NODE_H
