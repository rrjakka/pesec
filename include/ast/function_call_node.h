#ifndef PESEC_FUNCTION_CALL_NODE_H
#define PESEC_FUNCTION_CALL_NODE_H

#include "include/value.h"
#include "include/utils/string_view.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* callee;
    ast_node_t* arguments;
} function_call_node_t;

ast_node_t* function_call_node_new(ast_node_t* callee, ast_node_t* arguments);

void function_call_node_free(function_call_node_t* function_call_node);

value_t function_call_node_evaluate(const function_call_node_t* function_call_node, context_t* context);

#endif // PESEC_FUNCTION_CALL_NODE_H
