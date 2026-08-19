#include "include/array_value.h"

#include <stdlib.h>

#include "include/ast/ast_node.h"


array_value_t* array_value_new(value_t* values, ull_t size)
{
    const auto array_value = (array_value_t*)malloc(sizeof(array_value_t));

    array_value->capacity = 256;
    array_value->size = size;
    array_value->values = values;

    return array_value;
}

value_t array_value_get(const array_value_t* array_value, ull_t index)
{
    return array_value->values[index];
}

void array_value_set(const array_value_t* array_value, ull_t index, value_t value)
{
    array_value->values[index] = value;
}

void array_value_free(array_value_t* array_value)
{
    free(array_value->values);
    free(array_value);
}