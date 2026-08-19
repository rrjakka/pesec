#ifndef PESEC_VALUE_H
#define PESEC_VALUE_H

#define MAKE_VAL_STR(x) ((value_t) { .type = VALUE_TYPE_STRING, .value.as_string = x })
#define MAKE_VAL_NUM(x) ((value_t) { .type = VALUE_TYPE_NUMBER, .value.as_number = x })
#define MAKE_VAL_BOOL(x) ((value_t) { .type = VALUE_TYPE_BOOLEAN, .value.as_bool = x })
#define MAKE_VAL_FUNC(x) ((value_t) { .type = VALUE_TYPE_FUNCTION, .value.as_function = x })
#define MAKE_VAL_STRUCT(x) ((value_t) { .type = VALUE_TYPE_STRUCTURE, .value.as_structure = x })

#define MAKE_VAL_NUM_CF(x, cf) ((value_t) { .type = VALUE_TYPE_NUMBER, .value.as_number = x, .control_flow = cf })
#define MAKE_VAL_BOOL_CF(x, cf) ((value_t) { .type = VALUE_TYPE_BOOLEAN, .value.as_bool = x, .control_flow = cf })
#define MAKE_VAL_FUNC_CF(x, cf) ((value_t) { .type = VALUE_TYPE_FUNCTION, .value.as_function = x, .control_flow = cf })
#define MAKE_VAL_STRUCT_CF(x, cf) ((value_t) { .type = VALUE_TYPE_STRUCTURE, .value.as_structure = x, .control_flow = cf })

#include "utils/string.h"
#include "control_flow.h"

typedef struct FUNCTION_VALUE_STRUCT function_value_t;
typedef struct STRUCTURE_VALUE_STRUCT structure_value_t;

typedef enum
{
    VALUE_TYPE_STRING,
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_BOOLEAN,
    VALUE_TYPE_FUNCTION,
    VALUE_TYPE_STRUCTURE,
} value_type_t;

typedef union
{
    string_t as_string;
    long double as_number;
    bool as_bool;
    function_value_t* as_function;
    structure_value_t* as_structure;
} value_value_t;

typedef struct
{
    value_type_t type;
    value_value_t value;
    control_flow_t control_flow;
} value_t;

bool value_get_boolean(value_t value);

char* value_get_type(value_t value);

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
