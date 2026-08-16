#ifndef PESEC_VARIABLE_DEFINITION_NODE_H
#define PESEC_VARIABLE_DEFINITION_NODE_H

#include "include/value.h"
#include "include/utils/string_view.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    string_view_t name;
    ast_node_t* value;
    bool constant;
} variable_definition_node_t;

ast_node_t* variable_definition_node_new(string_view_t name, ast_node_t* value, bool constant);

void variable_definition_node_free(variable_definition_node_t* variable_definition_node);

value_t variable_definition_node_evaluate(const variable_definition_node_t* variable_definition_node, context_t* context);

#endif // PESEC_VARIABLE_DEFINITION_NODE_H
