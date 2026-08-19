#ifndef PESEC_ARRAY_DEFINITION_NODE_H
#define PESEC_ARRAY_DEFINITION_NODE_H

#include "statement_sequence_node.h"
#include "include/value.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* values;
} array_definition_node_t;

ast_node_t* array_definition_node_new(ast_node_t* values);

void array_definition_node_free(array_definition_node_t* array_definition_node);

value_t array_definition_node_evaluate(const array_definition_node_t* array_definition_node, context_t* context);

#endif // PESEC_ARRAY_DEFINITION_NODE_H
