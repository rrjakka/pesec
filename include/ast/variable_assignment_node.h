#ifndef PESEC_VARIABLE_ASSIGNMENT_NODE_H
#define PESEC_VARIABLE_ASSIGNMENT_NODE_H

#include "include/value.h"
#include "include/utils/string_view.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* target;
    ast_node_t* value;
} variable_assignment_node_t;

ast_node_t* variable_assignment_node_new(ast_node_t* target, ast_node_t* value);

void variable_assignment_node_free(variable_assignment_node_t* variable_assignment_node);

value_t variable_assignment_node_evaluate(const variable_assignment_node_t* variable_assignment_node, context_t* context);

#endif // PESEC_VARIABLE_ASSIGNMENT_NODE_H
