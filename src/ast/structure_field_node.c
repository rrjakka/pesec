#include "include/ast/structure_field_node.h"

#include "include/ast/ast_node.h"
#include <stdlib.h>

#include "include/structure_value.h"
#include "include/utils/throw.h"


ast_node_t* structure_field_node_new(const string_view_t name, const string_view_t field)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_STRUCTURE_FIELD;
    node->node.structure_field = (structure_field_node_t*)malloc(sizeof(structure_field_node_t));
    node->node.structure_field->name = name;
    node->node.structure_field->field = field;
    return node;
}

void structure_field_node_free(structure_field_node_t* structure_field_node)
{
    free(structure_field_node);
}

value_t structure_field_node_evaluate(const structure_field_node_t* structure_field_node, context_t* context)
{
    context_item_t* item = context_get(context, structure_field_node->name);

    if (item->value.type != VALUE_TYPE_STRUCTURE)
    {
        THROW("Variable %.*s is not structure", item->key.length, item->key.string);
    }

    structure_value_t* structure_value = item->value.value.as_structure;

    return structure_value_get(structure_value, structure_field_node->field);
}
