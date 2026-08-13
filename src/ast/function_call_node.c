#include "include/ast/function_call_node.h"

#include <stdio.h>

#include "include/ast/ast_node.h"
#include <stdlib.h>

#include "include/function_value.h"


ast_node_t* function_call_node_new(const string_view_t name, ast_node_t* arguments)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_FUNCTION_CALL;
    node->node.function_call = (function_call_node_t*)malloc(sizeof(function_call_node_t));
    node->node.function_call->name = name;
    node->node.function_call->arguments = arguments;
    return node;
}

void function_call_node_free(function_call_node_t* function_call_node)
{
    if (function_call_node->arguments)
        ast_node_free(function_call_node->arguments);
    free(function_call_node);
}

value_t function_call_node_evaluate(const function_call_node_t* function_call_node, context_t* context)
{
    unsigned long long statements_count = 0;
    const statement_sequence_node_queue_t* current = nullptr;
    value_t* evaluated_values = nullptr;

    if (function_call_node->arguments && function_call_node->arguments->node.statement_sequence)
    {
        statements_count = function_call_node->arguments->node.statement_sequence->statements_count;
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

    if (string_view_equals_cstr(function_call_node->name, "println"))
    {
        for (unsigned long long j = 0; j < statements_count; j++)
            value_print(evaluated_values[j]);

        printf("\n");
    }
    else
    {
        const context_item_t* function = context_get(context, function_call_node->name);

        if (function->value.type != VALUE_TYPE_FUNCTION)
        {
            fprintf(stderr, "variable %.*s not a function\n", function->key.length, function->key.string);
            exit(EXIT_FAILURE);
        }

        function_value_call(function->value.value.as_function, context);
    }

    free(evaluated_values);

    return MAKE_VAL_NUM(0);
}
