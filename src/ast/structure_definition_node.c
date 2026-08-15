#include "include/ast/structure_definition_node.h"

#include "include/ast/ast_node.h"
#include <stdlib.h>

#include "include/structure_value.h"


ast_node_t* structure_definition_node_new(parameter_t* parameter, ast_node_t* values)
{
    const auto node = (ast_node_t*)malloc(sizeof(ast_node_t));
    node->type = AST_NODE_STRUCTURE_DEFINITION;
    node->node.structure_definition = (structure_definition_node_t*)malloc(sizeof(structure_definition_node_t));
    node->node.structure_definition->parameter = parameter;
    node->node.structure_definition->values = values;
    return node;
}

void structure_definition_node_free(structure_definition_node_t* structure_definition_node)
{
    parameter_free(structure_definition_node->parameter);
    ast_node_free(structure_definition_node->values);
    free(structure_definition_node);
}

value_t structure_definition_node_evaluate(const structure_definition_node_t* structure_definition_node, context_t* context)
{
    structure_value_t* structure_value = structure_value_new();

    const statement_sequence_node_t* values_sequence = structure_definition_node->values->node.statement_sequence;

    parameter_queue_t* current_parameter = structure_definition_node->parameter->parameters;
    statement_sequence_node_queue_t* current_value = values_sequence->statements;

    while (current_parameter && current_value)
    {
        structure_value_set(structure_value, current_parameter->value, ast_node_evaluate(current_value->statement, context));
        current_parameter = current_parameter->next;
        current_value = current_value->next;
    }

    const auto value = MAKE_VAL_STRUCT(structure_value);
    return value;
}
