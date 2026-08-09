#include <stdio.h>
#include <stdlib.h>

#include "include/lexer.h"
#include "include/parser.h"
#include "include/context.h"



int main(const int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: pesec <file>\n");
        return EXIT_SUCCESS;
    }

    const char* filename = argv[1];
    char* source = nullptr;
    unsigned long long source_size = 0;

    FILE* file = fopen(filename, "r");

    if (!file)
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    source_size = ftell(file);
    fseek (file, 0, SEEK_SET);
    source = (char*)malloc(source_size);
    fread(source, 1, source_size, file);
    fclose(file);

    lexer_t* lexer = lexer_new(source, source_size);
    parser_t* parser = parser_new(lexer);
    ast_node_t* ast = parser_parse(parser);
    context_t* context = context_new();

    const value_t result = ast_node_evaluate(ast, context);

    switch (result.control_flow)
    {
    case CONTROL_FLOW_BREAK:
        fprintf(stderr, "Break outside of loop\n");
        break;
    case CONTROL_FLOW_CONTINUE:
        fprintf(stderr, "Continue outside of loop\n");
        break;
    default:
        break;
    }

    context_free(context);
    ast_node_free(ast);
    lexer_free(lexer);
    parser_free(parser);

    // printf("finished with %Lf", result);

    return EXIT_SUCCESS;
}
