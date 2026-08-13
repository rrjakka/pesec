#ifndef PESEC_FUNCTION_VALUE_H
#define PESEC_FUNCTION_VALUE_H
#include "context.h"
#include "parameter.h"
#include "value.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct FUNCTION_VALUE_STRUCT
{
    parameter_t* parameter;
    ast_node_t* body;
} function_value_t;

function_value_t* function_value_new(parameter_t* parameter, ast_node_t* body);

void function_value_free(function_value_t* function_value);

value_t function_value_call(const function_value_t* function_value, context_t* context);

#endif // PESEC_FUNCTION_VALUE_H
