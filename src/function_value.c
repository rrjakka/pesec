#include "include/function_value.h"

#include <stdlib.h>

#include "include/ast/ast_node.h"

function_value_t* function_value_new(parameter_t* parameter, ast_node_t* body)
{
    const auto function_value = (function_value_t*) malloc(sizeof(function_value_t));
    function_value->body = body;
    function_value->parameter = parameter;
    return function_value;
}

void function_value_free(function_value_t* function_value)
{
    ast_node_free(function_value->body);
    free(function_value);
}

value_t function_value_call(const function_value_t* function_value, context_t* context)
{
    return ast_node_evaluate(function_value->body, context);
}