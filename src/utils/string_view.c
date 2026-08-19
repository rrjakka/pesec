#include "include/utils/string_view.h"

#include <string.h>

string_view_t string_view_from(const char* string)
{
    return (string_view_t) {
        .data = string,
        .length = strlen(string)
    };
}

bool string_view_equals(const string_view_t left, const string_view_t right)
{
    if (left.length != right.length) return false;
    return memcmp(left.data, right.data, left.length) == 0;
}

bool string_view_equals_cstr(const string_view_t left, const char* right)
{
    if (left.length != strlen(right)) return false;
    return memcmp(left.data, right, left.length) == 0;
}