#include "include/value.h"

#include <stdio.h>
#include <stdlib.h>

bool value_get_boolean(const value_t value)
{
    switch (value.type)
    {
        case VALUE_TYPE_NUMBER: return value.value.as_number != 0;
        case VALUE_TYPE_STRING: return value.value.as_string.length != 0;
        case VALUE_TYPE_BOOLEAN: return value.value.as_bool;
    }

    fprintf(stderr, "Value type '%d' is not a valid value type\n", value.type);
}

void value_print(const value_t value)
{
    switch (value.type)
    {
        case VALUE_TYPE_NUMBER: printf("%Lf", value.value.as_number); return;
        case VALUE_TYPE_STRING: printf("%.*s", value.value.as_string.length, value.value.as_string.string); return;
        case VALUE_TYPE_BOOLEAN: printf(value.value.as_bool ? "true" : "false"); return;
    }

    fprintf(stderr, "Value type '%d' is not a valid value type\n", value.type);
}

value_t value_operation_add(const value_t left, const value_t right)
{
    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_NUMBER,
            .value.as_number = left.value.as_number + right.value.as_number,
        };

    fprintf(stderr, "Non number type can't use operator '+'\n");
}

value_t value_operation_sub(const value_t left, const value_t right)
{
    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_NUMBER,
            .value.as_number = left.value.as_number - right.value.as_number,
        };

    fprintf(stderr, "Non number type can't use operator '-'\n");
}

value_t value_operation_mul(const value_t left, const value_t right)
{
    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_NUMBER,
            .value.as_number = left.value.as_number * right.value.as_number,
        };

    fprintf(stderr, "Non number type can't use operator '*'\n");
}

value_t value_operation_div(const value_t left, const value_t right)
{
    // please never use right equal 0
    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_NUMBER,
            .value.as_number = left.value.as_number / right.value.as_number,
        };

    fprintf(stderr, "Non number type can't use operator '/'\n");
}

value_t value_operation_equals(const value_t left, const value_t right)
{
    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_BOOLEAN,
            .value.as_bool = left.value.as_number == right.value.as_number,
        };
    if (left.type == VALUE_TYPE_STRING && right.type == VALUE_TYPE_STRING)
        return (value_t) {
            .type = VALUE_TYPE_BOOLEAN,
            .value.as_bool = string_view_equals(left.value.as_string, right.value.as_string),
        };

    return (value_t) {
        .type = VALUE_TYPE_BOOLEAN,
        .value.as_bool = false,
    };
}

value_t value_operation_less_or_equals(const value_t left, const value_t right)
{
    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_BOOLEAN,
            .value.as_bool = left.value.as_number <= right.value.as_number,
        };

    return (value_t) {
        .type = VALUE_TYPE_BOOLEAN,
        .value.as_bool = false,
    };
}

value_t value_operation_less(const value_t left, const value_t right)
{
    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_BOOLEAN,
            .value.as_bool = left.value.as_number < right.value.as_number,
        };

    return (value_t) {
        .type = VALUE_TYPE_BOOLEAN,
        .value.as_bool = false,
    };
}

value_t value_operation_greater_or_equals(const value_t left, const value_t right)
{

    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_BOOLEAN,
            .value.as_bool = left.value.as_number >= right.value.as_number,
        };

    return (value_t) {
        .type = VALUE_TYPE_BOOLEAN,
        .value.as_bool = false,
    };
}

value_t value_operation_greater(const value_t left, const value_t right)
{

    if (left.type == VALUE_TYPE_NUMBER && right.type == VALUE_TYPE_NUMBER)
        return (value_t) {
            .type = VALUE_TYPE_BOOLEAN,
            .value.as_bool = left.value.as_number > right.value.as_number,
        };

    return (value_t) {
        .type = VALUE_TYPE_BOOLEAN,
        .value.as_bool = false,
    };
}