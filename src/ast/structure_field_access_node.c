#include "include/ast/structure_field_access_node.h"

#include "include/ast/ast_node.h"
#include <stdlib.h>

#include "include/structure_value.h"
#include "include/utils/throw.h"


ast_node_t* structure_field_access_node_new(ast_node_t* object, const string_view_t field)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_STRUCTURE_FIELD_ACCESS;
    node->node.structure_field_access = (structure_field_access_node_t*)malloc(sizeof(structure_field_access_node_t));
    node->node.structure_field_access->object = object;
    node->node.structure_field_access->field = field;
    return node;
}

void structure_field_access_node_free(structure_field_access_node_t* structure_field_node)
{
    free(structure_field_node);
}

value_t structure_field_access_node_evaluate(const structure_field_access_node_t* structure_field_node, context_t* context)
{
    value_t item = ast_node_evaluate(structure_field_node->object, context);

    if (item.type != VALUE_TYPE_STRUCTURE)
    {
        THROW("Variable is not structure");
    }

    const structure_value_t* structure_value = item.value.as_structure;

    return structure_value_get(structure_value, structure_field_node->field);
}
