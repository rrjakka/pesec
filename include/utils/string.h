#ifndef PESEC_STRING_H
#define PESEC_STRING_H

typedef struct
{
    char* data;
    unsigned long long size;
    unsigned long long capacity;
} string_t;

string_t string_new();

void string_push_back(string_t* string, char data);

bool string_equals(string_t left, string_t right);

void string_push_concat(string_t* left, string_t* right);

void string_free(string_t string);

#endif // PESEC_STRING_H