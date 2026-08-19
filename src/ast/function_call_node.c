#include "include/ast/function_call_node.h"

#include <stdio.h>

#include "include/ast/ast_node.h"
#include <stdlib.h>

#include "include/function_value.h"
#include "include/utils/throw.h"


ast_node_t* function_call_node_new(ast_node_t* callee, ast_node_t* arguments)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_FUNCTION_CALL;
    node->node.function_call = (function_call_node_t*)malloc(sizeof(function_call_node_t));
    node->node.function_call->callee = callee;
    node->node.function_call->arguments = arguments;
    return node;
}

void function_call_node_free(function_call_node_t* function_call_node)
{
    if (function_call_node->arguments)
        ast_node_free(function_call_node->arguments);
    if (function_call_node->callee)
        ast_node_free(function_call_node->callee);
    free(function_call_node);
}

value_t function_call_node_evaluate(const function_call_node_t* function_call_node, context_t* context)
{
    unsigned long long statements_count = 0;
    const statement_sequence_node_queue_t* current = nullptr;
    value_t* evaluated_values = nullptr;

    auto result = MAKE_VAL_NUM(0);

    if (function_call_node->arguments && function_call_node->arguments->node.statement_sequence)
    {
        statements_count = function_call_node->arguments->node.statement_sequence->count;
        current = function_call_node->arguments->node.statement_sequence->statements;

        if (statements_count > 0)
        {
            evaluated_values = (value_t*)calloc(statements_count, sizeof(value_t));
            if (!evaluated_values)
            {
                return MAKE_VAL_NUM(0);
            }
        }
    }

    for (unsigned long long i = 0; current && i < statements_count; i++, current = current->next)
    {
        if (current->statement)
            evaluated_values[i] = ast_node_evaluate(current->statement, context);
        else
            evaluated_values[i] = MAKE_VAL_NUM(0);
    }

    const value_t callee_value = ast_node_evaluate(function_call_node->callee, context);

    if (callee_value.type != VALUE_TYPE_FUNCTION)
    {
        THROW(
            "variable %s not a function\n",
            callee_value.value.as_string.data
            );
    }

    const auto function = callee_value.value.as_function;

    if (function->parameter->count != statements_count)
    {
        THROW(
            "function %s takes %llu but %llu given\n",
            callee_value.value.as_string.data,
            function->parameter->count, statements_count
            );
    }

    context_t* local_context = context_new(context);

    const parameter_queue_t* parameter = function->parameter->parameters;

    for (unsigned long long i = 0; parameter; ++i, parameter = parameter->next)
    {
        context_push(local_context, parameter->value, evaluated_values[i], false);
    }

    result = function_value_call(
        function,
        local_context
    );

    context_free(local_context);

    free(evaluated_values);

    return result;
}
