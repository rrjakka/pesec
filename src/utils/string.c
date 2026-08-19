#include "include/utils/string.h"

#include <stdlib.h>
#include <string.h>


string_t string_new()
{
    string_t string;

    string.capacity = 16;
    string.size = 0;
    string.data = (char*)malloc(string.capacity * sizeof(char));
    string.data[string.size] = '\0';

    return string;
}

void string_push_back(string_t* string, const char data)
{
    if (string->size + 1 >= string->capacity)
    {
        string->capacity = string->capacity * 2;
        string->data = (char*)realloc(string->data, string->capacity);
    }

    string->data[string->size] = data;
    string->size++;

    string->data[string->size] = '\0';
}

bool string_equals(const string_t left, const string_t right)
{
    if (left.size != right.size) return false;

    return memcmp(left.data, right.data, left.size) == 0;
}

void string_push_concat(string_t* left, string_t* right)
{
    // well, fuck
}

void string_free(string_t string)
{
    free(string.data);
}