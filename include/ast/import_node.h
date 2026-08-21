#ifndef PESEC_IMPORT_NODE_H
#define PESEC_IMPORT_NODE_H

#include "include/value.h"
#include "include/context.h"

typedef struct AST_NODE_STRUCT ast_node_t;

typedef struct
{
    ast_node_t* source;
} import_node_t;

ast_node_t* import_node_new(ast_node_t* source);

void import_node_free(import_node_t* import_node);

value_t import_node_evaluate(const import_node_t* import_node, context_t* context);

#endif // PESEC_IMPORT_NODE_H
