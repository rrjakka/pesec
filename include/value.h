#ifndef PESEC_VALUE_H
#define PESEC_VALUE_H

#include "utils/string_view.h"
#include "control_flow.h"

typedef enum
{
    VALUE_TYPE_STRING,
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_BOOLEAN,
    // TODO: VALUE_TYPE_FUNCTION,
} value_type_t;

typedef union
{
    string_view_t as_string;
    long double as_number;
    bool as_bool;
} value_value_t;

typedef struct
{
    value_type_t type;
    value_value_t value;
    control_flow_t control_flow;
} value_t;

bool value_get_boolean(value_t value);

void value_print(value_t value);

value_t value_operation_add(value_t left, value_t right);

value_t value_operation_sub(value_t left, value_t right);

value_t value_operation_mul(value_t left, value_t right);

value_t value_operation_div(value_t left, value_t right);


value_t value_operation_equals(value_t left, value_t right);

value_t value_operation_less_or_equals(value_t left, value_t right);

value_t value_operation_less(value_t left, value_t right);

value_t value_operation_greater_or_equals(value_t left, value_t right);

value_t value_operation_greater(value_t left, value_t right);


#endif // PESEC_VALUE_H
