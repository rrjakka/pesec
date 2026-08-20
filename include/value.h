#ifndef PESEC_VALUE_H
#define PESEC_VALUE_H

#define MAKE_VAL_STR(x) ((value_t) { .type = VALUE_TYPE_STRING, .data.as_string = x })
#define MAKE_VAL_NUM(x) ((value_t) { .type = VALUE_TYPE_NUMBER, .data.as_number = x })
#define MAKE_VAL_BOOL(x) ((value_t) { .type = VALUE_TYPE_BOOLEAN, .data.as_bool = x })
#define MAKE_VAL_FUNC(x) ((value_t) { .type = VALUE_TYPE_FUNCTION, .data.as_function = x })
#define MAKE_VAL_STRUCT(x) ((value_t) { .type = VALUE_TYPE_STRUCTURE, .data.as_structure = x })
#define MAKE_VAL_ARR(x) ((value_t) { .type = VALUE_TYPE_ARRAY, .data.as_array = x })

#define MAKE_VAL_NUM_CF(x, cf) ((value_t) { .type = VALUE_TYPE_NUMBER, .data.as_number = x, .control_flow = cf })
#define MAKE_VAL_BOOL_CF(x, cf) ((value_t) { .type = VALUE_TYPE_BOOLEAN, .data.as_bool = x, .control_flow = cf })
#define MAKE_VAL_FUNC_CF(x, cf) ((value_t) { .type = VALUE_TYPE_FUNCTION, .data.as_function = x, .control_flow = cf })
#define MAKE_VAL_STRUCT_CF(x, cf) ((value_t) { .type = VALUE_TYPE_STRUCTURE, .data.as_structure = x, .control_flow = cf })
#define MAKE_VAL_ARR_CF(x, cf) ((value_t) { .type = VALUE_TYPE_ARRAY, .data.as_array = x, .control_flow = cf })

#include "utils/string.h"
#include "control_flow.h"

typedef struct FUNCTION_VALUE_STRUCT function_value_t;
typedef struct STRUCTURE_VALUE_STRUCT structure_value_t;
typedef struct ARRAY_VALUE_STRUCT array_value_t;

typedef enum
{
    VALUE_TYPE_STRING,
    VALUE_TYPE_NUMBER,
    VALUE_TYPE_BOOLEAN,
    VALUE_TYPE_FUNCTION,
    VALUE_TYPE_STRUCTURE,
    VALUE_TYPE_ARRAY,
} value_type_t;

typedef union
{
    string_t as_string;
    long double as_number;
    bool as_bool;
    function_value_t* as_function;
    structure_value_t* as_structure;
    array_value_t* as_array;
} value_value_t;

typedef struct
{
    value_type_t type;
    value_value_t data;
    control_flow_t control_flow;
} value_t;

bool value_get_boolean(value_t value);

char* value_get_type(value_t value);

void value_print(value_t value);

void value_print_string(string_t value);

void value_print_number(long double value);

void value_print_boolean(bool value);

void value_print_function(const function_value_t* value);

void value_print_structure(const structure_value_t* value);

void value_print_array(const array_value_t* value);

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
