#include "include/context.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

context_t* context_new()
{
    const auto context = (context_t*)malloc(sizeof(context_t));

    context->capacity = 256;
    context->size = 0;
    context->items = (context_item_t**)calloc(context->capacity,sizeof(context_item_t*));

    return context;
}

unsigned long long context_hash(const context_t* context, const string_view_t key)
{
    unsigned long long sum = 0;
    for (unsigned long long i = 0; i < key.length; i++)
    {
        sum += (key.string[i] * (i + 256)) ^ 'f' ^ 'u' ^ 'c' ^ 'k';
    }
    return sum % context->capacity;
}

void context_push(const context_t* context, const string_view_t key, const value_t value)
{
    const unsigned long long hash_index = context_hash(context, key);

    const auto item = (context_item_t*)malloc(sizeof(context_item_t));
    item->key = key;
    item->value = value;
    item->next = nullptr;

    context_item_t* node = context->items[hash_index];

    if (node)
    {
        while (node->next)
        {
            node = node->next;
        }
        node->next = item;
    }
    else context->items[hash_index] = item;
}

void context_set(const context_t* context, const string_view_t key, const value_t value)
{
    context_item_t* node = context_get(context, key);
    node->value = value;
}

context_item_t* context_get(const context_t* context, const string_view_t key)
{
    const unsigned long long hash_index = context_hash(context, key);

    context_item_t* node = context->items[hash_index];

    while (node)
    {
        if (node->key.length == key.length && string_view_equals(node->key, key))
            return node;

        node = node->next;
    }

    fprintf(stderr, "Variable %.*s doesn't exist\n", key.length, key.string);
    exit(EXIT_FAILURE);
}

void context_free(context_t* context)
{
    for (unsigned long long i = 0; i < context->capacity; ++i)
    {
        context_item_t* node = context->items[i];

        while (node != nullptr)
        {
            context_item_t* next = node->next;
            free(node);
            node = next;
        }
    }

    free(context->items);
    free(context);
}