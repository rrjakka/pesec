#ifndef PESEC_FUNCTION_DEFINITION_NODE_H
#define PESEC_FUNCTION_DEFINITION_NODE_H

#include "include/value.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* body;
} function_definition_node_t;

ast_node_t* function_definition_node_new(ast_node_t* body);

void function_definition_node_free(function_definition_node_t* function_definition_node);

value_t function_definition_node_evaluate(const function_definition_node_t* function_definition_node, context_t* context);

#endif // PESEC_FUNCTION_DEFINITION_NODE_H
