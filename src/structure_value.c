#include "include/structure_value.h"

#include <stdlib.h>

#include "include/ast/ast_node.h"

structure_value_t* structure_value_new()
{
    const auto structure_value = (structure_value_t*) malloc(sizeof(structure_value_t));

    structure_value->context = context_new(nullptr);

    return structure_value;
}

void structure_value_push(const structure_value_t* structure_value, string_view_t name, value_t value)
{
    context_push(structure_value->context, name, value, false);
}

void structure_value_set(const structure_value_t* structure_value, string_view_t name, value_t value)
{
    context_set(structure_value->context, name, value);
}

value_t structure_value_get(const structure_value_t* structure_value, const string_view_t name)
{
    return context_get(structure_value->context, name)->value;
}

void structure_value_free(structure_value_t* structure_value)
{
    context_free(structure_value->context);
    free(structure_value);
}