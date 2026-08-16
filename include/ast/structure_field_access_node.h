#ifndef PESEC_STRUCTURE_FIELD_ACCESS_NODE_H
#define PESEC_STRUCTURE_FIELD_ACCESS_NODE_H

#include "include/value.h"
#include "include/context.h"
#include "include/parameter.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* object;
    string_view_t field;
} structure_field_access_node_t;

ast_node_t* structure_field_access_node_new(ast_node_t* object, string_view_t field);

void structure_field_access_node_free(structure_field_access_node_t* structure_field_node);

value_t structure_field_access_node_evaluate(const structure_field_access_node_t* structure_field_node, context_t* context);

#endif // PESEC_STRUCTURE_FIELD_ACCESS_NODE_H
