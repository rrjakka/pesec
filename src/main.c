#include <stdio.h>
#include <stdlib.h>

#include "include/context.h"
#include "include/function_value.h"
#include "include/utils/execute_file.h"

int main(const int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: pesec <file>\n");
        return EXIT_SUCCESS;
    }

    const char* filename = argv[1];

    context_t* context = context_new(nullptr);
    execute_file(filename, context);
    context_free(context);

    return EXIT_SUCCESS;
}
